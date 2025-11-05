#pragma once

#include "util.h"
#include "tc_bus_device.h"

#include "esphome/components/tc_bus/tc_bus.h"
#include "esphome/components/tc_bus/protocol.h"

#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace tc_bus
    {
        // Actions
        template<typename... Ts>
        class TCBusDeviceSendAction : public Action<Ts...>, public Parented<TCBusDeviceComponent>
        {
            public:
                TEMPLATABLE_VALUE(TelegramType, type)
                TEMPLATABLE_VALUE(uint8_t, address)
                TEMPLATABLE_VALUE(uint32_t, payload)

                void play(const Ts&... x) override
                {
                    this->parent_->send_telegram(this->type_.value(x...), this->address_.value(x...), this->payload_.value(x...));
                }
        };

        template<typename... Ts>
        class TCBusDeviceUpdateSettingAction : public Action<Ts...>, public Parented<TCBusDeviceComponent>
        {
            public:
                TEMPLATABLE_VALUE(SettingType, type)
                TEMPLATABLE_VALUE(uint8_t, value)

                void play(const Ts&... x) override
                {
                    this->parent_->update_setting(this->type_.value(x...), this->value_.value(x...));
                }
        };

        template<typename... Ts>
        class TCBusDeviceUpdateDoorbellButtonAction : public Action<Ts...>, public Parented<TCBusDeviceComponent>
        {
            public:
                TEMPLATABLE_VALUE(uint8_t, button_row)
                TEMPLATABLE_VALUE(uint8_t, button_col)
                TEMPLATABLE_VALUE(DoorbellButtonAction, primary_action)
                TEMPLATABLE_VALUE(uint32_t, primary_payload)
                TEMPLATABLE_VALUE(DoorbellButtonAction, secondary_action)
                TEMPLATABLE_VALUE(uint32_t, secondary_payload)

                void play(const Ts&... x) override
                {
                    DoorbellButtonConfig button;

                    if (this->primary_action_.has_value())
                    {
                        button.primary_action = this->primary_action_.value(x...);
                    }

                    if (this->primary_payload_.has_value())
                    {
                        button.primary_payload = this->primary_payload_.value(x...);
                    }

                    if (this->secondary_action_.has_value())
                    {
                        button.secondary_action = this->secondary_action_.value(x...);
                    }

                    if (this->secondary_payload_.has_value())
                    {
                        button.secondary_payload = this->secondary_payload_.value(x...);
                    }

                    this->parent_->update_doorbell_button(this->button_row_.value(x...), this->button_col_.value(x...), button);
                }
        };

        template<typename... Ts>
        class TCBusDeviceReadMemoryAction : public Action<Ts...>, public Parented<TCBusDeviceComponent>
        {
            public:
                void play(const Ts&... x) override
                {
                    this->parent_->read_memory();
                }
        };

        template<typename... Ts>
        class TCBusDeviceIdentifyAction : public Action<Ts...>, public Parented<TCBusDeviceComponent>
        {
            public:
                void play(const Ts&... x) override
                {
                    this->parent_->request_version();
                }
        };

        // Callbacks
        class ReadMemoryCompleteTrigger : public Trigger<std::vector<uint8_t>> {
            public:
                explicit ReadMemoryCompleteTrigger(TCBusDeviceComponent *parent) {
                    parent->add_read_memory_complete_callback([this](const std::vector<uint8_t> &value) { this->trigger(value); });
                }
        };

        class ReadMemoryTimeoutTrigger : public Trigger<> {
            public:
                explicit ReadMemoryTimeoutTrigger(TCBusDeviceComponent *parent) {
                    parent->add_read_memory_timeout_callback([this]() { this->trigger(); });
                }
        };

        class IdentifyTimeoutTrigger : public Trigger<> {
            public:
                explicit IdentifyTimeoutTrigger(TCBusDeviceComponent *parent) {
                    parent->add_identify_timeout_callback([this]() { this->trigger(); });
                }
        };
        class IdentifyUnknownTrigger : public Trigger<> {
            public:
                explicit IdentifyUnknownTrigger(TCBusDeviceComponent *parent) {
                    parent->add_identify_timeout_callback([this]() { this->trigger(); });
                }
        };
        class IdentifyCompleteTrigger : public Trigger<ModelData> {
            public:
                explicit IdentifyCompleteTrigger(TCBusDeviceComponent *parent) {
                    parent->add_identify_complete_callback([this](const ModelData &value) { this->trigger(value); });
                }
        };
    }  // namespace tc_bus
}  // namespace esphome