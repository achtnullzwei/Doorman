#pragma once

#include "tc_bus.h"

#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome
{
    namespace tc_bus
    {
        // Actions
        template<typename... Ts>
        class TCBusSendAction : public Action<Ts...>, public Parented<TCBusComponent>
        {
            public:
                TEMPLATABLE_VALUE(uint32_t, telegram)
                TEMPLATABLE_VALUE(bool, is_long)
                TEMPLATABLE_VALUE(TelegramType, type)
                TEMPLATABLE_VALUE(uint8_t, address)
                TEMPLATABLE_VALUE(uint32_t, payload)
                TEMPLATABLE_VALUE(uint32_t, serial_number)

                void play(const Ts&... x) override
                {
                    if(this->telegram_.value(x...) == 0)
                    {
                        this->parent_->send_telegram(this->type_.value(x...), this->address_.value(x...), this->payload_.value(x...), this->serial_number_.value(x...));
                    }
                    else
                    {
                        if(this->is_long_.value(x...) == false)
                        {
                            this->parent_->send_telegram(this->telegram_.value(x...));
                        }
                        else
                        {
                            this->parent_->send_telegram(this->telegram_.value(x...), true);
                        }
                    }
                }
        };

        template<typename... Ts>
        class TCBusProgrammingModeAction : public Action<Ts...>, public Parented<TCBusComponent>
        {
            public:
                TEMPLATABLE_VALUE(bool, programming_mode)

                void play(const Ts&... x) override
                {
                    this->parent_->set_programming_mode(this->programming_mode_.value(x...));
                }
        };

        // Callbacks
        class ReceivedTelegramTrigger : public Trigger<TelegramData> {
            public:
                explicit ReceivedTelegramTrigger(TCBusComponent *parent) {
                    parent->add_received_telegram_callback([this](const TelegramData &value) { this->trigger(value); });
                }
        };
    }  // namespace tc_bus
}  // namespace esphome