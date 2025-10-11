#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"

#include "esphome/components/tc_bus/tc_bus.h"
#include "esphome/components/tc_bus/protocol.h"

#include <vector>

#ifdef USE_ESP32
#include <driver/uart.h>
#if defined(USE_ESP32_VARIANT_ESP32C3) || defined(USE_ESP32_VARIANT_ESP32C6) || defined(USE_ESP32_VARIANT_ESP32S3) || \
    defined(USE_ESP32_VARIANT_ESP32H2)
#include <driver/usb_serial_jtag.h>
#include <hal/usb_serial_jtag_ll.h>
#endif
#if defined(USE_ESP32_VARIANT_ESP32S2) || defined(USE_ESP32_VARIANT_ESP32S3)
#include <esp_private/usb_console.h>
#endif
#elif defined(USE_ARDUINO)
#include <HardwareSerial.h>
#endif

namespace esphome
{
  namespace tc_bus_serial
  {
    class TCBusSerialComponent : public Component, public tc_bus::TCBusRemoteListener {
    public:
      void setup() override;
      void loop() override;
      void dump_config() override;
      bool on_receive(tc_bus::TelegramData telegram_data, bool received) override;
      void set_base_component(tc_bus::TCBusComponent *base_component) { this->base_component_ = base_component; }

    protected:
      tc_bus::TCBusComponent *base_component_{nullptr};

      bool parse_byte_(uint8_t byte);
      optional<uint8_t> read_byte_();
      void write_data_(std::vector<uint8_t> &data, bool linebreak = false);

    #ifdef USE_ESP32
      uart_port_t uart_num_;
    #elif defined(USE_ARDUINO)
      Stream *hw_serial_{nullptr};
    #endif

      std::vector<uint8_t> rx_buffer_;
      uint32_t last_read_byte_{0};
    };

  }  // namespace tc_bus_serial
}  // namespace esphome