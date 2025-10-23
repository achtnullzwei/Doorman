#include "tc_bus_device.h"
#include "util.h"

#include "esphome.h"
#include "esphome/core/application.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/preferences.h"

#ifdef USE_ARDUINO
#include "Arduino.h"
#endif

#include <optional>
#include <utility>
#include <vector>
#include <cinttypes>

#include "esphome/components/tc_bus/tc_bus.h"
#include "esphome/components/tc_bus/protocol.h"

using namespace esphome;

namespace esphome
{
    namespace tc_bus
    {
        void TCBusDeviceComponent::setup()
        {
            ESP_LOGCONFIG(TAG, "Running setup");

            if(this->tc_bus_->is_failed()) {
                this->mark_failed("TC:BUS failed to setup!");
                return;
            }
        
            if(!this->tc_bus_->is_ready()) {
                this->mark_failed("TC:BUS is not setup yet!");
                return;
            }

            uint32_t hash = fnv1_hash("tc_bus_device_" + this->internal_id_);
            this->pref_ = global_preferences->make_preference<TCBusDeviceSettings>(hash, true);

            TCBusDeviceSettings recovered;

            if (!this->pref_.load(&recovered))
            {
                recovered = {MODEL_NONE, 0, false};
            }

            this->serial_number_ = recovered.serial_number;
            this->force_long_door_opener_ = recovered.force_long_door_opener;
            this->set_model(recovered.model, false);

            #ifdef USE_NUMBER
                if (this->serial_number_number_ != nullptr)
                {
                    this->serial_number_number_->publish_state(this->serial_number_);
                }
            #endif

            #ifdef USE_SWITCH
                if (this->force_long_door_opener_switch_ != nullptr)
                {
                    this->force_long_door_opener_switch_->publish_state(this->force_long_door_opener_);
                }
            #endif

            #ifdef USE_BINARY_SENSOR
                // Reset Bunary Sensor Listeners
                for (auto &listener : listeners_)
                {
                    listener->turn_off(&listener->timer_);
                }
            #endif

            // Register remote listener
            this->tc_bus_->register_remote_listener(this);
        }

        void TCBusDeviceComponent::set_model(Model model, bool save)
        {
            this->model_ = model;
            this->model_data_ = getModelData(model);

            // Save to flash
            if(save)
            {
                this->save_preferences();
            }

            // Reserve memory
            if(model != MODEL_NONE)
            {
                ESP_LOGD(TAG, "Reserve Memory Buffer");
                if(this->model_data_.memory_size > 0)
                {
                    this->memory_buffer_.reserve(this->model_data_.memory_size);
                }
            }

            // Update Entities
            #ifdef USE_SELECT
                if (this->model_select_ != nullptr)
                {
                    this->model_select_->publish_state(model_to_string(model));
                }
            #endif

            this->publish_settings();
        }

        void TCBusDeviceComponent::save_preferences()
        {
            TCBusDeviceSettings settings{
                this->model_,
                this->serial_number_,
                this->force_long_door_opener_
            };

            if (!this->pref_.save(&settings))
            {
                ESP_LOGW(TAG, "Failed to save settings to flash memory.");
            }
        }

        void TCBusDeviceComponent::dump_config()
        {
            ESP_LOGCONFIG(TAG, "TC:BUS Device:");

            ESP_LOGCONFIG(TAG, "  Device Group: %d", this->device_group_);
        }

        void TCBusDeviceComponent::loop()
        {
            #ifdef USE_BINARY_SENSOR
            // Turn off binary sensor after ... milliseconds
            uint32_t now_millis = millis();
            for (auto &listener : listeners_)
            {
                if (listener->timer_ && now_millis > listener->timer_)
                {
                    listener->turn_off(&listener->timer_);
                }
            }
            #endif
        }

        bool TCBusDeviceComponent::on_receive(tc_bus::TelegramData telegram_data, bool received)
        {
            if (received)
            {
                // From receiver
                if(telegram_data.type == TELEGRAM_TYPE_DATA)
                {
                    if (read_memory_flow_)
                    {
                        ESP_LOGD(TAG, "Received 4 memory blocks from %i to %i | Data: 0x%08X", (reading_memory_count_ * 4), (reading_memory_count_ * 4) + 4, telegram_data.raw);
                            
                        // Save Data to memory Store
                        memory_buffer_.push_back((telegram_data.raw >> 24) & 0xFF);
                        memory_buffer_.push_back((telegram_data.raw >> 16) & 0xFF);
                        memory_buffer_.push_back((telegram_data.raw >> 8) & 0xFF);
                        memory_buffer_.push_back(telegram_data.raw & 0xFF);

                        // Next 4 Data Blocks
                        reading_memory_count_++;

                        // Memory reading complete
                        if (reading_memory_count_ == reading_memory_max_)
                        {
                            // Turn off
                            this->cancel_timeout("wait_for_memory_reading");

                            ESP_LOGD(TAG, "Reset read_memory_flow_");
                            read_memory_flow_ = false;

                            ESP_LOGD(TAG, "Reset running_flow_");
                            running_flow_ = false;

                            this->read_memory_complete_callback_.call(memory_buffer_);

                            this->publish_settings();
                        }
                        else
                        {
                            read_memory_block();
                        }

                        // Do not proceed
                        return true;
                    }
                    else if (read_memory_update_flow_)
                    {
                        ESP_LOGD(TAG, "Received 4 memory blocks from %i to %i | Data: 0x%08X", (reading_memory_count_ * 4), (reading_memory_count_ * 4) + 4, telegram_data.raw);
                            
                        // Save Data to memory Store
                        memory_buffer_[reading_memory_count_]     = (telegram_data.raw >> 24) & 0xFF;
                        memory_buffer_[reading_memory_count_ + 1] = (telegram_data.raw >> 16) & 0xFF;
                        memory_buffer_[reading_memory_count_ + 2] = (telegram_data.raw >> 8) & 0xFF;
                        memory_buffer_[reading_memory_count_ + 3] = telegram_data.raw & 0xFF;

                        // Turn off
                        this->cancel_timeout("wait_for_memory_reading");

                        ESP_LOGD(TAG, "Reset read_memory_update_flow_");
                        read_memory_update_flow_ = false;

                        ESP_LOGD(TAG, "Reset running_flow_");
                        running_flow_ = false;

                        this->read_memory_complete_callback_.call(memory_buffer_);

                        this->publish_settings();

                        // Do not proceed
                        return true;
                    }
                    else if (identify_model_flow_)
                    {
                        ESP_LOGD(TAG, "Received model identification | Data: %s", telegram_data.hex);

                        ESP_LOGD(TAG, "Reset identify_model_flow_");
                        identify_model_flow_ = false;

                        ESP_LOGD(TAG, "Reset running_flow_");
                        running_flow_ = false;

                        this->cancel_timeout("wait_for_identification_group_0");
                        this->cancel_timeout("wait_for_identification_group_1");
                        this->cancel_timeout("wait_for_identification_other");

                        ModelData device;
                        device.device_group = this->tc_bus_->selected_device_group_;
                        device.memory_size = 0;
                        
                        const char* hex = telegram_data.hex;

                        if (hex[4] == 'D')
                        {
                            // New models

                            // FW Version
                            char fw_buf[4] = { hex[5], hex[6], hex[7], '\0' };
                            device.firmware_version = strtol(fw_buf, nullptr, 16);

                            // Firmware major, minor, patch (1 char each)
                            char tmp[2] = {0};
                            tmp[0] = hex[5];
                            device.firmware_major = strtol(tmp, nullptr, 16);
                            tmp[0] = hex[6];
                            device.firmware_minor = strtol(tmp, nullptr, 16);
                            tmp[0] = hex[7];
                            device.firmware_patch = strtol(tmp, nullptr, 16);

                            // Hardware version (first char, decimal)
                            tmp[0] = hex[0];
                            tmp[1] = '\0';
                            device.hardware_version = strtol(tmp, nullptr, 10);

                            // Model string (substring 1-3)
                            char model_buf[4] = { hex[1], hex[2], hex[3], '\0' };
                            device.model = identifier_string_to_model(device.device_group, model_buf, device.hardware_version, device.firmware_version);
                        }
                        else
                        {
                            if(device.device_group == 0 || device.device_group == 1)
                            {
                                // Old indoor station models
                                switch(telegram_data.raw)
                                {
                                    // TTC-XX
                                    case 0x08000040:
                                        device.model = MODEL_IS_TTCXX;
                                        break;

                                    // TTS-XX
                                    case 0x02010040:
                                        device.model = MODEL_IS_TTSXX;
                                        break;

                                    // ISH 1030
                                    case 0x08000048:
                                    case 0x08080048:
                                        device.model = MODEL_IS_ISH1030;
                                        break;

                                    default:
                                        break;
                                }
                            }
                            else if(device.device_group == 4)
                            {
                                // Old controller models
                                switch(telegram_data.raw)
                                {
                                    case 0x877F5804:
                                        device.model = MODEL_CTRL_BVS20;
                                        break;

                                    default:
                                        break;
                                }
                            }
                            else
                            {
                                // Old models of other groups
                                // Not implemented
                            }
                        }

                        if (device.model != MODEL_NONE)
                        {
                            // Add missing information
                            device.memory_size = getModelData(device.model).memory_size;

                            ESP_LOGD(TAG, "Identified Hardware: %s (v%i) | Firmware: %i.%i.%i",
                                    model_to_string(device.model),
                                    device.hardware_version,
                                    device.firmware_major,
                                    device.firmware_minor,
                                    device.firmware_patch);

                            
                            // Update Model
                            if (device.model != this->model_)
                            {
                                this->set_model(device.model);
                            }
                            this->identify_complete_callback_.call(device);
                        }
                        else
                        {
                            ESP_LOGE(TAG, "Unable to identify Hardware! Unknown model. Data received: %s", telegram_data.hex);
                            this->identify_unknown_callback_.call();
                        }

                        // Do not proceed
                        return true;
                    }
                }
                else
                {
                    #ifdef USE_BINARY_SENSOR
                        // Fire Binary Sensors
                        for (auto &listener : listeners_)
                        {
                            bool allow_publish = (telegram_data.type == (listener->type_.value_or(TELEGRAM_TYPE_UNKNOWN))) &&
                                (telegram_data.address == listener->address_.value_or(0) || listener->address_.value_or(0) == 255) &&
                                (telegram_data.payload == listener->payload_.value_or(0) || listener->payload_.value_or(0) == 255) &&
                                (telegram_data.serial_number == this->serial_number_);

                            // Trigger listener binary sensor if match found
                            if (allow_publish)
                            {
                                listener->turn_on(&listener->timer_, listener->auto_off_);
                            }
                        }
                    #endif
                }
            }

            return true;
        }

        void TCBusDeviceComponent::publish_settings()
        {
            if(this->device_group_ == DEVICE_GROUP_INDOOR_STATION)
            {
                ESP_LOGD(TAG, "Ringtone volume: %i", get_setting(SETTING_VOLUME_RINGTONE));
                ESP_LOGD(TAG, "Handset volume (Door Call): %i", get_setting(SETTING_VOLUME_HANDSET_DOOR_CALL));
                ESP_LOGD(TAG, "Handset volume (Internal Call): %i", get_setting(SETTING_VOLUME_HANDSET_INTERNAL_CALL));

                ESP_LOGD(TAG, "Entrance Door Call Ringtone: %i", get_setting(SETTING_RINGTONE_ENTRANCE_DOOR_CALL));
                ESP_LOGD(TAG, "Second Entrance Door Call Ringtone: %i", get_setting(SETTING_RINGTONE_SECOND_ENTRANCE_DOOR_CALL));
                ESP_LOGD(TAG, "Floor Call Ringtone: %i", get_setting(SETTING_RINGTONE_FLOOR_CALL));
                ESP_LOGD(TAG, "Internal Call Ringtone: %i", get_setting(SETTING_RINGTONE_INTERNAL_CALL));

                ESP_LOGD(TAG, "Address Divider (AS): %i", get_setting(SETTING_AS_ADDRESS_DIVIDER));
                ESP_LOGD(TAG, "Address Divider (VAS): %i", get_setting(SETTING_VAS_ADDRESS_DIVIDER));

                #ifdef USE_SELECT
                    if (this->ringtone_entrance_door_call_select_)
                    {
                        this->ringtone_entrance_door_call_select_->publish_state(int_to_ringtone(get_setting(SETTING_RINGTONE_ENTRANCE_DOOR_CALL)));
                    }
                    if (this->ringtone_second_entrance_door_call_select_)
                    {
                        this->ringtone_second_entrance_door_call_select_->publish_state(int_to_ringtone(get_setting(SETTING_RINGTONE_SECOND_ENTRANCE_DOOR_CALL)));
                    }
                    if (this->ringtone_floor_call_select_)
                    {
                        this->ringtone_floor_call_select_->publish_state(int_to_ringtone(get_setting(SETTING_RINGTONE_FLOOR_CALL)));
                    }
                    if (this->ringtone_internal_call_select_)
                    {
                        this->ringtone_internal_call_select_->publish_state(int_to_ringtone(get_setting(SETTING_RINGTONE_INTERNAL_CALL)));
                    }
                #endif

                #ifdef USE_NUMBER
                    if (this->volume_handset_door_call_number_)
                    {
                        this->volume_handset_door_call_number_->publish_state(get_setting(SETTING_VOLUME_HANDSET_DOOR_CALL));
                    }
                    if (this->volume_handset_internal_call_number_)
                    {
                        this->volume_handset_internal_call_number_->publish_state(get_setting(SETTING_VOLUME_HANDSET_INTERNAL_CALL));
                    }
                    if (this->volume_ringtone_number_)
                    {
                        this->volume_ringtone_number_->publish_state(get_setting(SETTING_VOLUME_RINGTONE));
                    }
                #endif
            }
            else if(this->device_group_ == DEVICE_GROUP_OUTDOOR_STATION)
            {
                ESP_LOGD(TAG, "Address: %i", get_setting(SETTING_AS_ADDRESS));
                ESP_LOGD(TAG, "Address Lock: %s", YESNO(get_setting(SETTING_AS_ADDRESS_LOCK)));

                ESP_LOGD(TAG, "Talking requires door readiness: %s", YESNO(get_setting(SETTING_TALKING_REQUIRES_DOOR_READINESS)));
                
                uint8_t door_opener_dur = get_setting(SETTING_DOOR_OPENER_DURATION);
                if(door_opener_dur == 0)
                {
                    ESP_LOGD(TAG, "Door Opener Duration: %s Unlimited");
                }
                else
                {
                    ESP_LOGD(TAG, "Door Opener Duration: %i sec.", door_opener_dur * 8);
                }

                uint8_t calling_dur = get_setting(SETTING_CALLING_DURATION);
                if(calling_dur == 0)
                {
                    ESP_LOGD(TAG, "Calling Duration: Unlimited");
                }
                else
                {
                    ESP_LOGD(TAG, "Calling Duration: %i sec.", calling_dur * 8);
                }

                uint8_t door_readiness_dur = get_setting(SETTING_DOOR_READINESS_DURATION);
                if(door_readiness_dur == 0)
                {
                    ESP_LOGD(TAG, "Door Readiness Duration: Unlimited");
                }
                else
                {
                    ESP_LOGD(TAG, "Door Readiness Duration: %i sec.", door_readiness_dur * 8);
                }
            }
            else
            {
                // Other devices
                ESP_LOGW(TAG, "Can't publish settings for this device group - not implemented yet");
            }
        }

        #ifdef USE_BINARY_SENSOR
            void TCBusDeviceComponent::register_listener(TCBusDeviceListener *listener)
            {
                this->listeners_.push_back(listener);
            }
        #endif

        void TCBusDeviceComponent::send_telegram(TelegramType type, uint8_t address, uint32_t payload, uint32_t wait_duration)
        {
            // Use 32-bit protocol
            if(type == TELEGRAM_TYPE_OPEN_DOOR && this->force_long_door_opener_)
            {
                ESP_LOGV(TAG, "Detected 32-bit door protocol override, change telegram telegram to OPEN_DOOR_LONG.");
                type = TELEGRAM_TYPE_OPEN_DOOR_LONG;
            }

            this->tc_bus_->send_telegram(type, address, payload, this->serial_number_, wait_duration);
        }

        void TCBusDeviceComponent::request_version()
        {
            if(running_flow_ || this->identify_model_flow_)
            {
                ESP_LOGE(TAG, "Another flow is already running");
                return;
            }

            if(this->serial_number_ == 0)
            {
                ESP_LOGE(TAG, "Device model cannot be identified without a serial number!");
                return;
            }

            ESP_LOGD(TAG, "Set running_flow_");
            running_flow_ = true;

            this->cancel_timeout("wait_for_identification_other");

            ESP_LOGD(TAG, "Set identify_model_flow_");
            this->identify_model_flow_ = true;

            if(this->device_group_ == DEVICE_GROUP_INDOOR_STATION)
            {
                // Indoor Stations

                // First try with group 0
                ESP_LOGD(TAG, "Identifying device model (Group %i) using serial number: %i...", 0, this->serial_number_);
                send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, 0, 400); // group 0
                send_telegram(TELEGRAM_TYPE_REQUEST_VERSION, 0, 0, 400);

                this->set_timeout("wait_for_identification_group_0", 1000, [this]()
                {
                    // Didn't receive identify result of group 0
                    // Second try with group 1
                    ESP_LOGD(TAG, "Identifying device model (Group %i) using serial number: %i...", 1, this->serial_number_);
                    send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, 1, 400); // group 1
                    send_telegram(TELEGRAM_TYPE_REQUEST_VERSION, 0, 0, 400);

                    this->set_timeout("wait_for_identification_group_1", 1000, [this]()
                    {
                        // Didn't receive identify result of group 1
                        // Failed
                        ESP_LOGD(TAG, "Reset identify_model_flow_");
                        this->identify_model_flow_ = false;

                        ESP_LOGD(TAG, "Reset running_flow_");
                        running_flow_ = false;

                        this->identify_timeout_callback_.call();
                        ESP_LOGE(TAG, "Identification response not received in time. The device model may not support identification.");
                    });
                });
            }
            else
            {
                // Other Devices

                // Use device group if not 0 and 1
                ESP_LOGD(TAG, "Identifying device model (group %i) using serial number: %i...", (uint8_t)this->device_group_, this->serial_number_);
                send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, (uint8_t)this->device_group_, 400);
                send_telegram(TELEGRAM_TYPE_REQUEST_VERSION, 0, 0, 400);

                this->set_timeout("wait_for_identification_other", 1000, [this]() {
                    // Failed
                    ESP_LOGD(TAG, "Reset identify_model_flow_");
                    this->identify_model_flow_ = false;

                    ESP_LOGD(TAG, "Reset running_flow_");
                    running_flow_ = false;

                    ESP_LOGE(TAG, "Identification response not received in time. The device model may not support identification.");
                });
            }
        }

        void TCBusDeviceComponent::read_memory()
        {
            if(running_flow_ || this->read_memory_flow_)
            {
                ESP_LOGE(TAG, "Another flow is already running");
                return;
            }

            if (this->serial_number_ == 0)
            {
                ESP_LOGE(TAG, "Unable to read device memory without a serial number!");
                return;
            }

            if (this->model_ == MODEL_NONE)
            {
                ESP_LOGE(TAG, "Unable to read device memory without a model!");
                return;
            }

            if (this->model_data_.memory_size == 0)
            {
                ESP_LOGE(TAG, "This model %s (Serial: %i) does not support reading memory!", model_to_string(this->model_), this->serial_number_);
                // Call timeout callback for unsupported models
                this->read_memory_timeout_callback_.call();
                return;
            }
            else
            {
                ESP_LOGD(TAG, "Reading EEPROM for model %s (Serial: %i)...", model_to_string(this->model_), this->serial_number_);
            }

            ESP_LOGD(TAG, "Set running_flow_");
            running_flow_ = true;

            send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, this->model_data_.device_group);
            send_telegram(TELEGRAM_TYPE_SELECT_MEMORY_PAGE, 0, 0);

            memory_buffer_.clear();
  
            ESP_LOGD(TAG, "Set read_memory_flow_");
            this->read_memory_flow_ = true;

            reading_memory_count_ = 0;
            reading_memory_max_ = (this->model_data_.memory_size / 4);

            this->set_timeout("wait_for_memory_reading", reading_memory_max_ * 600, [this]()
            {
                memory_buffer_.clear();
                reading_memory_count_ = 0;
                reading_memory_max_ = 0;

                ESP_LOGD(TAG, "Reset read_memory_flow_");
                this->read_memory_flow_ = false;

                ESP_LOGD(TAG, "Reset running_flow_");
                running_flow_ = false;

                this->read_memory_timeout_callback_.call();
                ESP_LOGE(TAG, "Memory block not received in time. Reading canceled!");
            });

            read_memory_block();
        }

        void TCBusDeviceComponent::read_memory_update(uint8_t index)
        {
            if(running_flow_ || this->read_memory_update_flow_)
            {
                ESP_LOGE(TAG, "Another flow is already running");
                return;
            }

            if (this->serial_number_ == 0)
            {
                ESP_LOGE(TAG, "Unable to read device memory without a serial number!");
                return;
            }

            if (this->model_ == MODEL_NONE)
            {
                ESP_LOGE(TAG, "Unable to read device memory without a model!");
                return;
            }

            if (this->model_data_.memory_size == 0)
            {
                ESP_LOGE(TAG, "This model %s (Serial: %i) does not support reading memory!", model_to_string(this->model_), this->serial_number_);
                // Call timeout callback for unsupported models
                this->read_memory_timeout_callback_.call();
                return;
            }
            else
            {
                ESP_LOGD(TAG, "Reading EEPROM for model %s (Serial: %i)...", model_to_string(this->model_), this->serial_number_);
            }

            ESP_LOGD(TAG, "Set running_flow_");
            running_flow_ = true;

            send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, this->model_data_.device_group);
            send_telegram(TELEGRAM_TYPE_SELECT_MEMORY_PAGE, 0, 0);
  
            ESP_LOGD(TAG, "Set read_memory_flow_");
            this->read_memory_update_flow_ = true;

            reading_memory_count_ = (index / 4);

            this->set_timeout("wait_for_memory_reading", 600, [this]()
            {
                reading_memory_count_ = 0;
                reading_memory_max_ = 0;

                ESP_LOGD(TAG, "Reset read_memory_update_flow_");
                this->read_memory_update_flow_ = false;

                ESP_LOGD(TAG, "Reset running_flow_");
                running_flow_ = false;
            });

            read_memory_block();
        }

        void TCBusDeviceComponent::read_memory_block()
        {
            ESP_LOGD(TAG, "Read 4 memory blocks, from %i to %i.", (reading_memory_count_ * 4), (reading_memory_count_ * 4) + 4);
            send_telegram(TELEGRAM_TYPE_READ_MEMORY_BLOCK, reading_memory_count_, 0, 300);
        }

        bool TCBusDeviceComponent::supports_setting(SettingType type)
        {
            if (memory_buffer_.size() == 0 || this->model_ == MODEL_NONE)
            {
                return false;
            }

            // Get Setting Cell Data by Model
            SettingCellData cellData = getSettingCellData(type, this->model_);
            if (cellData.index != 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        uint8_t TCBusDeviceComponent::get_setting(SettingType type)
        {
            if (memory_buffer_.size() == 0)
            {
                return 0;
            }

            if (this->model_ == MODEL_NONE)
            {
                ESP_LOGV(TAG, "Model is not specified (NONE). Returning 0.");
                return 0;
            }

            // Get Setting Cell Data by Model
            SettingCellData cellData = getSettingCellData(type, this->model_);
            if (cellData.index != 0)
            {
                uint8_t shift = cellData.start_bit - cellData.length + 1;
                uint8_t mask = (1 << cellData.length) - 1;
                uint8_t value = (memory_buffer_[cellData.index] >> shift) & mask;
                return value;
            }
            else
            {
                ESP_LOGV(TAG, "The setting '%s' is not available for model '%s'.", setting_type_to_string(type), model_to_string(this->model_));
                return 0;
            }
        }

        bool TCBusDeviceComponent::update_setting(SettingType type, uint8_t new_value)
        {
            if (this->memory_buffer_.size() == 0)
            {
                ESP_LOGE(TAG, "Memory buffer is empty. Please read memory before proceeding!");
                return false;
            }

            if (this->serial_number_ == 0)
            {
                ESP_LOGE(TAG, "Unable to update device setting without a serial number!");
                return false;
            }

            if (this->model_ == MODEL_NONE)
            {
                ESP_LOGE(TAG, "Unable to update device setting without a model!");
                return false;
            }

            // Get Setting Cell Data by Model
            SettingCellData cellData = getSettingCellData(type, this->model_);
            if (cellData.index == 0)
            {
                ESP_LOGE(TAG, "Cannot write because setting %s is not available for model %s!", setting_type_to_string(type), model_to_string(this->model_));
                return false;
            }

            ESP_LOGD(TAG, "Writing setting %s (%X) to EEPROM of %s (%i)...", setting_type_to_string(type), new_value, model_to_string(this->model_), this->serial_number_);
            
            // Apply new data
            uint8_t shift = cellData.start_bit - cellData.length + 1;
            uint8_t mask = (1 << cellData.length) - 1;
            uint8_t current_byte = memory_buffer_[cellData.index];
            current_byte &= ~(mask << shift);
            current_byte |= ((new_value & mask) << shift);
            memory_buffer_[cellData.index] = current_byte;

            // Prepare Transmission
            // Select device group
            send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, this->model_data_.device_group);

            // Select memory page %i of serial number %i
            send_telegram(TELEGRAM_TYPE_SELECT_MEMORY_PAGE, 0, 0);

            // Transfer new settings value to memory
            uint16_t new_values = (memory_buffer_[cellData.index] << 8) | memory_buffer_[cellData.index + 1];
            send_telegram(TELEGRAM_TYPE_WRITE_MEMORY, cellData.index, new_values);

            // Reset
            send_telegram(TELEGRAM_TYPE_RESET);

            return true;
        }

        bool TCBusDeviceComponent::write_memory()
        {
            if (this->memory_buffer_.size() == 0)
            {
                ESP_LOGE(TAG, "Memory buffer is empty! Please read memory first before proceeding.");
                return false;
            }

            if (this->serial_number_ == 0)
            {
                ESP_LOGE(TAG, "Unable to write memory without a serial number!");
                return false;
            }

            if (this->model_ == MODEL_NONE)
            {
                ESP_LOGE(TAG, "Unable to write memory without a model!");
                return false;
            }

            ESP_LOGD(TAG, "Write memory buffer to EEPROM of %s (%i)...", model_to_string(this->model_), this->serial_number_);

            // Prepare Transmission
            send_telegram(TELEGRAM_TYPE_SELECT_DEVICE_GROUP, 0, this->model_data_.device_group);

            send_telegram(TELEGRAM_TYPE_SELECT_MEMORY_PAGE, 0, 0);

            // Transmit Memory
            for (size_t address = 0; address < memory_buffer_.size(); address += 2)
            {
                uint16_t new_value = (memory_buffer_[address] << 8) | memory_buffer_[address + 1];
                send_telegram(TELEGRAM_TYPE_WRITE_MEMORY, address, new_value);
            }

            // Reset
            send_telegram(TELEGRAM_TYPE_RESET);

            return true;
        }

    } // namespace tc_bus
} // namespace esphome