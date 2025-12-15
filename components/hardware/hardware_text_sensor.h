#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome::hardware
{
    class HardwareTextSensor : public text_sensor::TextSensor, public Component 
    {
      public:
        void set_model(text_sensor::TextSensor *model) { model_ = model; }
        void set_revision(text_sensor::TextSensor *revision) { revision_ = revision; }

        float get_setup_priority() const override { return setup_priority::BUS; }

        void setup() override;
        void dump_config() override;

      protected:
        text_sensor::TextSensor *model_{nullptr};
        text_sensor::TextSensor *revision_{nullptr};
    };
}