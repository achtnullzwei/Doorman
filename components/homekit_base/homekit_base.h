#pragma once
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <hap.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include "const.h"

#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif

namespace esphome
{
namespace homekit {

  class HomeKitBaseComponent : public Component
  {
  #ifdef USE_BUTTON
    SUB_BUTTON(reset)
  #endif

  private:
    static constexpr const char* TAG = "homekit_base";

  public:
    float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_task_stack_size(uint32_t task_stack_size){ this->task_stack_size_ = task_stack_size; }
    void set_setup_code(const char *setup_code) { this->setup_code_ = setup_code; }
    void set_setup_id(const char *setup_id) { this->setup_id_ = setup_id; }
    void set_task_stack_size(const char *setup_id) { this->setup_id_ = setup_id; }
    void set_meta(std::map<AInfo, const char*> info) {
      std::map<AInfo, const char*> merged_info;
      merged_info.merge(info);
      merged_info.merge(this->accessory_info_);
      this->accessory_info_.swap(merged_info);
    }

    void factory_reset();
    bool is_paired();
    bool is_connected();

    static void static_hap_event_handler(hap_event_t event, void *data);
    static int static_identify_handler(hap_acc_t *ha);

    void add_pairing_started_callback(std::function<void()> &&callback);
    CallbackManager<void()> pairing_started_callback_{};

    void add_pairing_aborted_callback(std::function<void()> &&callback);
    CallbackManager<void()> pairing_aborted_callback_{};

    void add_pairing_timeout_callback(std::function<void()> &&callback);
    CallbackManager<void()> pairing_timeout_callback_{};

    void add_pairing_completed_callback(std::function<void(std::string)> &&callback);
    CallbackManager<void(std::string)> pairing_completed_callback_{};

    void add_identify_callback(std::function<void()> &&callback);
    CallbackManager<void()> identify_callback_{};

    void add_controller_connected_callback(std::function<void(std::string)> &&callback);
    CallbackManager<void(std::string)> controller_connected_callback_{};

    void add_controller_disconnected_callback(std::function<void(std::string)> &&callback);
    CallbackManager<void(std::string)> controller_disconnected_callback_{};

  protected:
    bool is_ready_ = false;
    uint8_t connection_count_ = 0;
    uint32_t task_stack_size_ = 4096;
    const char *setup_code_ = "159-35-728";
    const char *setup_id_ = "DMS3";
    std::map<AInfo, const char*> accessory_info_ = {{NAME, "ESPH Bridge"}, {MODEL, "HAP-BRIDGE"}, {SN, "16161616"}, {MANUFACTURER, "rednblkx"}, {FW_REV, "0.1"}};
  };

  static HomeKitBaseComponent *global_homekit_base;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace homekit
}  // namespace esphome