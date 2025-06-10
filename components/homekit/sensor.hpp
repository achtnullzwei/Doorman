#pragma once
#include <esphome/core/defines.h>
#ifdef USE_SENSOR
#include <map>
#include <esphome/core/application.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include "esphome/components/homekit_bridge/const.h"
#include "automation.h"

namespace esphome
{
  namespace homekit
  {
    class SensorEntity
    {
      static std::unordered_map<hap_acc_t*, SensorEntity*> acc_instance_map;

    private:
      static constexpr const char* TAG = "SensorEntity";
      sensor::Sensor* sensorPtr;

      void on_sensor_update(sensor::Sensor* obj, float v) {
        ESP_LOGD(TAG, "%s value: %.2f", obj->get_name().c_str(), v);

        hap_acc_t* acc = hap_acc_get_by_aid(hap_get_unique_aid(std::to_string(obj->get_object_id_hash()).c_str()));
        if (!acc) return;

        hap_serv_t* hs = hap_serv_get_next(hap_acc_get_first_serv(acc));
        if (!hs) return;
          
        hap_char_t* on_char = hap_serv_get_first_char(hs);
        if (!on_char) return;

        ESP_LOGD(TAG, "HAP CURRENT VALUE: %.2f", hap_char_get_val(on_char)->f);

        hap_val_t state;
        state.f = v;
        hap_char_update_val(on_char, &state);
      }

      static int sensor_read(hap_char_t* hc, hap_status_t* status_code, void* serv_priv, void* read_priv) {
        if (serv_priv) {
          sensor::Sensor* sensorPtr = (sensor::Sensor*)serv_priv;
          ESP_LOGD(TAG, "Read called for Accessory %s (%s)", std::to_string(sensorPtr->get_object_id_hash()).c_str(), sensorPtr->get_name().c_str());
          hap_val_t sensorValue;
          sensorValue.f = sensorPtr->get_state();
          hap_char_update_val(hc, &sensorValue);
          return HAP_SUCCESS;
        }
        return HAP_FAIL;
      }

      static int acc_identify(hap_acc_t* ha) {
        auto it = acc_instance_map.find(ha);
        if (it != acc_instance_map.end()) {
            it->second->on_identify();
        }
        return HAP_SUCCESS;
      }

      void on_identify() {
        ESP_LOGD(TAG, "Accessory identified");
        for (auto* trig : triggers_identify_) {
          if (trig) trig->trigger();
        }
      }

      std::vector<HKIdentifyTrigger *> triggers_identify_;

    public:
      SensorEntity(sensor::Sensor* sensorPtr) : sensorPtr(sensorPtr) {}

      void set_meta(std::map<AInfo, const char*> info) {
        std::map<AInfo, const char*> merged_info;
        merged_info.merge(info);
        merged_info.merge(this->accessory_info);
        this->accessory_info.swap(merged_info);
      }

      void register_on_identify_trigger(HKIdentifyTrigger* trig) {
          triggers_identify_.push_back(trig);
      }

      std::map<AInfo, const char*> accessory_info = {
        {NAME, NULL},
        {MODEL, "Sensor"},
        {SN, NULL},
        {MANUFACTURER, "ESPHome"},
        {FW_REV, "0.1"}
      };

      void setup() {
        hap_serv_t* service = nullptr;

        std::string device_class = sensorPtr->get_device_class();
        if (std::equal(device_class.begin(), device_class.end(), strdup("temperature"))) {
          service = hap_serv_temperature_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("humidity"))) {
          service = hap_serv_humidity_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("illuminance"))) {
          service = hap_serv_light_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("aqi"))) {
          service = hap_serv_air_quality_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("carbon_dioxide"))) {
          service = hap_serv_carbon_dioxide_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("carbon_monoxide"))) {
          service = hap_serv_carbon_monoxide_sensor_create(sensorPtr->state);
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("pm10"))) {
          service = hap_serv_create(HAP_SERV_UUID_AIR_QUALITY_SENSOR);
          hap_serv_add_char(service, hap_char_pm_10_density_create(sensorPtr->state));
        }
        else if (std::equal(device_class.begin(), device_class.end(), strdup("pm25"))) {
          service = hap_serv_create(HAP_SERV_UUID_AIR_QUALITY_SENSOR);
          hap_serv_add_char(service, hap_char_pm_2_5_density_create(sensorPtr->state));
        }

        if (service) {
          hap_acc_cfg_t acc_cfg = {
              .model = strdup(accessory_info[MODEL]),
              .manufacturer = strdup(accessory_info[MANUFACTURER]),
              .fw_rev = strdup(accessory_info[FW_REV]),
              .hw_rev = NULL,
              .pv = strdup("1.1.0"),
              .cid = HAP_CID_SENSOR,
              .identify_routine = acc_identify,
          };

          std::string accessory_name = sensorPtr->get_name();
          acc_cfg.name = strdup(accessory_info[NAME] ? accessory_info[NAME] : accessory_name.c_str());
          acc_cfg.serial_num = strdup(accessory_info[SN] ? accessory_info[SN] : std::to_string(sensorPtr->get_object_id_hash()).c_str());

          hap_acc_t* accessory = hap_acc_create(&acc_cfg);
          acc_instance_map[accessory] = this;

          ESP_LOGD(TAG, "ID HASH: %lu", sensorPtr->get_object_id_hash());

          hap_serv_set_priv(service, sensorPtr);

          /* Set the read callback for the service */
          hap_serv_set_read_cb(service, sensor_read);

          /* Add the Sensor Service to the Accessory Object */
          hap_acc_add_serv(accessory, service);

          /* Add the Accessory to the HomeKit Database */
          hap_add_bridged_accessory(accessory, hap_get_unique_aid(std::to_string(sensorPtr->get_object_id_hash()).c_str()));
          if (!sensorPtr->is_internal())
            sensorPtr->add_on_state_callback([this](float v) { this->on_sensor_update(sensorPtr, v); });
          ESP_LOGI(TAG, "Sensor '%s' linked to HomeKit", accessory_name.c_str());
        }
      }
    };

    inline std::unordered_map<hap_acc_t*, SensorEntity*> SensorEntity::acc_instance_map;
  }
}
#endif