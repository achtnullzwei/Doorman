#pragma once
#include <esphome/core/defines.h>
#ifdef USE_BINARY_SENSOR
#include <map>
#include <esphome/core/application.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include "esphome/components/homekit_base/const.h"
#include "automation.h"

namespace esphome
{
  namespace homekit
  {
    class BinarySensorEntity
    {
    private:
      static constexpr const char* TAG = "BinarySensorEntity";
      
      binary_sensor::BinarySensor* binarySensorPtr;

      void on_binary_sensor_update(binary_sensor::BinarySensor* obj, bool new_state) {
        ESP_LOGD(TAG, "%s value: %s", obj->get_name().c_str(), ONOFF(new_state));

        hap_acc_t* acc = hap_acc_get_by_aid(hap_get_unique_aid(std::to_string(obj->get_object_id_hash()).c_str()));
        if (!acc) return;

        hap_serv_t* hs = hap_serv_get_next(hap_acc_get_first_serv(acc));
        if (!hs) return;
          
        hap_char_t* on_char = hap_serv_get_first_char(hs);
        if (!on_char) return;

        ESP_LOGD(TAG, "HAP CURRENT VALUE: %s", ONOFF(hap_char_get_val(on_char)->u));

        hap_val_t state;
        state.u = new_state ? 1 : 0;
        hap_char_update_val(on_char, &state);



        hap_char_t* active_char = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_ON);
        if (!active_char) return;

        hap_val_t state2;
        state2.b = new_state;
        hap_char_update_val(active_char, &state2);
      }

      static int binary_sensor_read(hap_char_t* hc, hap_status_t* status_code, void* serv_priv, void* read_priv) {
        if (serv_priv) {
          binary_sensor::BinarySensor* binarySensorPtr = (binary_sensor::BinarySensor*)serv_priv;
          ESP_LOGD(TAG, "Read called for Accessory %s (%s)", std::to_string(binarySensorPtr->get_object_id_hash()).c_str(), binarySensorPtr->get_name().c_str());
          
          hap_val_t sensorValue;
          sensorValue.u = binarySensorPtr->state ? 1 : 0;
          hap_char_update_val(hc, &sensorValue);
          
          return HAP_SUCCESS;
        }
        return HAP_FAIL;
      }

      static int acc_identify(hap_acc_t* ha) {
        ESP_LOGI("homekit", "Accessory identified");
        return HAP_SUCCESS;
      }

      std::vector<HKIdentifyTrigger *> triggers_identify_;

    public:
      BinarySensorEntity(binary_sensor::BinarySensor* binarySensorPtr) : binarySensorPtr(binarySensorPtr) {}

      void set_meta(std::map<AInfo, const char*> info) {
        std::map<AInfo, const char*> merged_info;
        merged_info.merge(info);
        merged_info.merge(this->accessory_info);
        this->accessory_info.swap(merged_info);
      }

      std::map<AInfo, const char*> accessory_info = {
        {NAME, NULL},
        {MODEL, "Binary Sensor"},
        {SN, NULL},
        {MANUFACTURER, "ESPHome"},
        {FW_REV, "0.1"}
      };

      void register_on_identify_trigger(HKIdentifyTrigger* trig) {
          triggers_identify_.push_back(trig);
      }

      void setup() {
        hap_serv_t* service = nullptr;

        std::string device_class = binarySensorPtr->get_device_class();
        if (std::equal(device_class.begin(), device_class.end(), strdup("motion"))) {
          service = hap_serv_motion_sensor_create(binarySensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("door"))) {
          service = hap_serv_contact_sensor_create(binarySensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("window"))) {
          service = hap_serv_contact_sensor_create(binarySensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("occupancy"))) {
          service = hap_serv_occupancy_sensor_create(binarySensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("carbon_monoxide"))) {
          service = hap_serv_carbon_dioxide_sensor_create(binarySensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("smoke"))) {
          service = hap_serv_smoke_sensor_create(binarySensorPtr->state);
        } else {
          service = hap_serv_contact_sensor_create(binarySensorPtr->state);
        }

        if (service) {
          hap_acc_cfg_t acc_cfg = {
              .model = strdup(accessory_info[MODEL]),
              .manufacturer = strdup(accessory_info[MANUFACTURER]),
              .fw_rev = strdup(accessory_info[FW_REV]),
              .hw_rev = NULL,
              .pv = strdup("1.1.0"),
              .cid = HAP_CID_SENSOR,
              .identify_routine = acc_identify,
          };
          
          std::string accessory_name = binarySensorPtr->get_name();
          acc_cfg.name = strdup(accessory_info[NAME] ? accessory_info[NAME] : accessory_name.c_str());
          acc_cfg.serial_num = strdup(accessory_info[SN] ? accessory_info[SN] : std::to_string(binarySensorPtr->get_object_id_hash()).c_str());

          hap_acc_t* accessory = hap_acc_create(&acc_cfg);

          ESP_LOGD(TAG, "ID HASH: %lu", binarySensorPtr->get_object_id_hash());
          hap_serv_set_priv(service, binarySensorPtr);

          /* Set the read callback for the service */
          hap_serv_set_read_cb(service, binary_sensor_read);

          /* Add the Sensor Service to the Accessory Object */
          hap_acc_add_serv(accessory, service);

          /* Add the Accessory to the HomeKit Database */
          hap_add_bridged_accessory(accessory, hap_get_unique_aid(std::to_string(binarySensorPtr->get_object_id_hash()).c_str()));

          if (!binarySensorPtr->is_internal())
            binarySensorPtr->add_on_state_callback([this](bool b) { this->on_binary_sensor_update(binarySensorPtr, b); });

          ESP_LOGI(TAG, "Binary Sensor '%s' linked to HomeKit", accessory_name.c_str());
        }
      }
    };
  }
}
#endif