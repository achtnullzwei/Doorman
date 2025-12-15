#include "tc_bus_device_switch.h"

namespace esphome::tc_bus
{
    void ForceLongDoorOpenerProtocolSwitch::write_state(bool value)
    {
        this->publish_state(value);
        this->parent_->set_force_long_door_opener_protocol(value);
        this->parent_->save_preferences();
    }

    void RingtoneMuteSwitch::write_state(bool value)
    {
        this->publish_state(value);
        this->parent_->update_setting(SETTING_RINGTONE_MUTE, value);
    }
}