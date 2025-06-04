#pragma once
#include <esphome/core/defines.h>
#ifdef USE_BUTTON
#include <map>
#include <esphome/core/application.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include "esphome/components/homekit_base/const.h"

namespace esphome
{
  namespace homekit
  {
    class ButtonEntity
    {
    private:
      static constexpr const char* TAG = "ButtonEntity";
      
      button::Button* buttonPtr;

      void on_button_update(button::Button* obj) {
        ESP_LOGD(TAG, "%s value: single", obj->get_name().c_str());

        hap_acc_t* acc = hap_acc_get_by_aid(hap_get_unique_aid(std::to_string(obj->get_object_id_hash()).c_str()));
        if (!acc) return;

        hap_serv_t* hs = hap_serv_get_next(hap_acc_get_first_serv(acc));
        if (!hs) return;
          
        hap_char_t* event_char = hap_serv_get_first_char(hs);
        if (!event_char) return;

        hap_val_t val = {.u = 0};
        val.u = 0; // SINGLE_PRESS

        int ret = hap_char_update_val(event_char, &val);
        ESP_LOGI(TAG, "HomeKit Stateless Switch Button sent: %i, result: %d", val.u, ret);
      }

      static int acc_identify(hap_acc_t* ha) {
        ESP_LOGI("homekit", "Accessory identified");
        return HAP_SUCCESS;
      }

      std::vector<HKIdentifyTrigger *> triggers_identify_;

    public:
      ButtonEntity(button::Button* buttonPtr) : buttonPtr(buttonPtr) {}

      void set_meta(std::map<AInfo, const char*> info) {
        std::map<AInfo, const char*> merged_info;
        merged_info.merge(info);
        merged_info.merge(this->accessory_info);
        this->accessory_info.swap(merged_info);
      }

      std::map<AInfo, const char*> accessory_info = {
        {NAME, NULL},
        {MODEL, "Button"},
        {SN, NULL},
        {MANUFACTURER, "ESPHome"},
        {FW_REV, "0.1"}
      };

      void register_on_identify_trigger(HKIdentifyTrigger* trig) {
          triggers_identify_.push_back(trig);
      }

      void setup() {
        hap_serv_t* service = hap_serv_stateless_programmable_switch_create(0);

        if (service) {
          const uint8_t _validVals[] = {0};
          hap_char_add_valid_vals(hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_PROGRAMMABLE_SWITCH_EVENT), _validVals, 1);

          hap_acc_cfg_t acc_cfg = {
              .model = strdup(accessory_info[MODEL]),
              .manufacturer = strdup(accessory_info[MANUFACTURER]),
              .fw_rev = strdup(accessory_info[FW_REV]),
              .hw_rev = NULL,
              .pv = strdup("1.1.0"),
              .cid = HAP_CID_PROGRAMMABLE_SWITCH,
              .identify_routine = acc_identify,
          };
          
          std::string accessory_name = buttonPtr->get_name();
          acc_cfg.name = strdup(accessory_info[NAME] ? accessory_info[NAME] : accessory_name.c_str());
          acc_cfg.serial_num = strdup(accessory_info[SN] ? accessory_info[SN] : std::to_string(buttonPtr->get_object_id_hash()).c_str());

          hap_acc_t* accessory = hap_acc_create(&acc_cfg);

          ESP_LOGD(TAG, "ID HASH: %lu", buttonPtr->get_object_id_hash());

          hap_serv_set_priv(service, buttonPtr);
          hap_acc_add_serv(accessory, service);

          /* Add the Accessory to the HomeKit Database */
          hap_add_bridged_accessory(accessory, hap_get_unique_aid(std::to_string(buttonPtr->get_object_id_hash()).c_str()));

          if (!buttonPtr->is_internal()) {
            buttonPtr->add_on_press_callback([this]() {
              this->on_button_update(buttonPtr);
            });
          }

          ESP_LOGI(TAG, "Button '%s' linked to HomeKit", accessory_name.c_str());
        }
      }
    };
  }
}
#endif