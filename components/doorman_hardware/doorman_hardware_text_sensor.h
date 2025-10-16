#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome
{
  namespace doorman_hardware_text_sensor
  {
    class DoormanHardwareTextSensor : public text_sensor::TextSensor, public Component
    {
      public:
        void setup() override;
        void dump_config() override;

        char hardware_version_str_[32] = "Generic";
    };
  }  // namespace doorman_hardware_text_sensor
}  // namespace esphome