#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome::hardware
{
    class HardwareTextSensor : public text_sensor::TextSensor, public Component 
    {
      public:
        void set_product(text_sensor::TextSensor *product) { product_ = product; }
        void set_revision(text_sensor::TextSensor *revision) { revision_ = revision; }

        float get_setup_priority() const override { return setup_priority::HARDWARE; }

        void setup() override;
        void dump_config() override;

      protected:
        text_sensor::TextSensor *product_{nullptr};
        text_sensor::TextSensor *revision_{nullptr};
    };
}