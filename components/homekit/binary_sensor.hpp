#pragma once
#include <esphome/core/defines.h>
#ifdef USE_BINARY_SENSOR
#include <map>
#include "const.h"
#include <esphome/core/application.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
namespace esphome
{
  namespace homekit
  {
    class BinarySensorEntity
    {
    private:
      static constexpr const char* TAG = "BinarySensorEntity";
      
      std::map<AInfo, const char*> accessory_info = {{NAME, NULL}, {MODEL, "HAP-BINARY-SENSOR"}, {SN, NULL}, {MANUFACTURER, "rednblkx"}, {FW_REV, "0.1"}};
      
      binary_sensor::BinarySensor* sensorPtr;

      void on_binary_sensor_update(binary_sensor::BinarySensor* obj, bool b) {
        ESP_LOGD(TAG, "%s value: %s", obj->get_name().c_str(), ONOFF(b));

        hap_acc_t* acc = hap_acc_get_by_aid(hap_get_unique_aid(std::to_string(obj->get_object_id_hash()).c_str()));

        if (acc) {
          hap_serv_t* hs = hap_serv_get_next(hap_acc_get_first_serv(acc));
          if (hs) {
            hap_char_t* on_char = hap_serv_get_first_char(hs);
            ESP_LOGD(TAG, "HAP CURRENT VALUE: %s", ONOFF(hap_char_get_val(on_char)->b));
            hap_val_t state;
            state.u = b ? 1 : 0;
            hap_char_update_val(on_char, &state);
            hap_char_notify(on_char, &state);
          }
        }
        return;
      }

      static int binary_sensor_read(hap_char_t* hc, hap_status_t* status_code, void* serv_priv, void* read_priv) {
        if (serv_priv) {
          binary_sensor::BinarySensor* sensorPtr = (binary_sensor::BinarySensor*)serv_priv;
          ESP_LOGD(TAG, "Read called for Accessory %s (%s)", std::to_string(sensorPtr->get_object_id_hash()).c_str(), sensorPtr->get_name().c_str());
          
          hap_val_t sensorValue;
          sensorValue.u = sensorPtr->state ? 1 : 0;
          hap_char_update_val(hc, &sensorValue);
          
          return HAP_SUCCESS;
        }
        return HAP_FAIL;
      }

      static int acc_identify(hap_acc_t* ha) {
        ESP_LOGI(TAG, "Accessory identified");
        return HAP_SUCCESS;
      }

    public:
      BinarySensorEntity(binary_sensor::BinarySensor* sensorPtr) : sensorPtr(sensorPtr) {}

      void setInfo(std::map<AInfo, const char*> info) {
        std::map<AInfo, const char*> merged_info;
        merged_info.merge(info);
        merged_info.merge(this->accessory_info);
        this->accessory_info.swap(merged_info);
      }

      void setup() {
        hap_serv_t* service = nullptr;

        std::string device_class = sensorPtr->get_device_class();
        if (std::equal(device_class.begin(), device_class.end(), strdup("motion"))) {
          service = hap_serv_motion_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("door"))) {
          service = hap_serv_contact_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("window"))) {
          service = hap_serv_contact_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("occupancy"))) {
          service = hap_serv_occupancy_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("carbon_monoxide"))) {
          service = hap_serv_carbon_dioxide_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("smoke"))) {
          service = hap_serv_smoke_sensor_create(sensorPtr->state);
        }

        if (service) {
          hap_acc_cfg_t acc_cfg = {
              .model = strdup(accessory_info[MODEL]),
              .manufacturer = strdup(accessory_info[MANUFACTURER]),
              .fw_rev = strdup(accessory_info[FW_REV]),
              .hw_rev = NULL,
              .pv = strdup("1.1.0"),
              .cid = HAP_CID_BRIDGE,
              .identify_routine = acc_identify,
          };
          
          hap_acc_t* accessory = nullptr;
          std::string accessory_name = sensorPtr->get_name();

          if (accessory_info[NAME] == NULL) {
            acc_cfg.name = strdup(accessory_name.c_str());
          }
          else {
            acc_cfg.name = strdup(accessory_info[NAME]);
          }
          if (accessory_info[SN] == NULL) {
            acc_cfg.serial_num = strdup(std::to_string(sensorPtr->get_object_id_hash()).c_str());
          }
          else {
            acc_cfg.serial_num = strdup(accessory_info[SN]);
          }

          accessory = hap_acc_create(&acc_cfg);
          ESP_LOGD(TAG, "ID HASH: %lu", sensorPtr->get_object_id_hash());
          hap_serv_set_priv(service, sensorPtr);

          /* Set the read callback for the service */
          hap_serv_set_read_cb(service, binary_sensor_read);

          /* Add the Sensor Service to the Accessory Object */
          hap_acc_add_serv(accessory, service);


          /* Add the Accessory to the HomeKit Database */
          hap_add_bridged_accessory(accessory, hap_get_unique_aid(std::to_string(sensorPtr->get_object_id_hash()).c_str()));

          if (!sensorPtr->is_internal())
            sensorPtr->add_on_state_callback([this](bool b) { this->on_binary_sensor_update(sensorPtr, b); });

          ESP_LOGI(TAG, "Binary Sensor '%s' linked to HomeKit", accessory_name.c_str());
        }
      }
    };
  }
}
#endif