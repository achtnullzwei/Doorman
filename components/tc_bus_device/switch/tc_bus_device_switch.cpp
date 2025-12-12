#include "tc_bus_device_switch.h"

namespace esphome
{
    namespace tc_bus
    {
        void ForceLongDoorOpenerProtocolSwitch::write_state(bool value)
        {
            this->publish_state(value);
            this->parent_->set_force_long_door_opener_protocol(value);
            this->parent_->save_preferences();
        }
    }  // namespace tc_bus
}  // namespace esphome