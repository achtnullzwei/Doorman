#pragma once

#include "esphome/components/switch/switch.h"
#include "../tc_bus_device.h"

namespace esphome
{
  namespace tc_bus
  {
    class ForceLongDoorOpenerProtocolSwitch final : public switch_::Switch, public Parented<TCBusDeviceComponent> {
      public:
        ForceLongDoorOpenerProtocolSwitch() = default;

      protected:
        void write_state(bool value) override;
    };

    class RingtoneMuteSwitch final : public switch_::Switch, public Parented<TCBusDeviceComponent> {
      public:
        RingtoneMuteSwitch() = default;

      protected:
        void write_state(bool value) override;
    };
  }  // namespace tc_bus
}  // namespace esphome