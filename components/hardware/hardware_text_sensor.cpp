#include "esphome/core/log.h"
#include "hardware_text_sensor.h"

#if defined(USE_ESP32)
#include "soc/efuse_reg.h"
#include "soc/efuse_periph.h"
#include "esp_efuse.h"
#include "esp_efuse_table.h"
#endif

namespace esphome::hardware
{
    static const char *TAG = "hardware.text_sensor";

    void HardwareTextSensor::setup()
    {
        ESP_LOGCONFIG(TAG, "Running setup");
        ESP_LOGI(TAG, "Checking Hardware...");

        #ifdef USE_ESP32
        uint8_t revision[3] = {0};
        uint8_t model = 0;
        uint8_t model_alt = 0;

        esp_err_t rev_err = esp_efuse_read_block(EFUSE_BLK3, revision, 0, 24);
        esp_err_t model_err = esp_efuse_read_block(EFUSE_BLK3, &model, 24, 8);
        // Alternative location
        esp_err_t model_alt_err = esp_efuse_read_block(EFUSE_BLK4, &model_alt, 0, 8);

        if (rev_err == ESP_OK && model_err == ESP_OK && model_alt_err == ESP_OK && revision[0] > 0)
        {
            // No model set, fix it
            if(model == 0 && model_alt == 0)
            {
                model = 1; // Doorman S3

                ESP_LOGI(TAG, "Writing model: %d at bit offset 0", model);
                esp_err_t write_err = esp_efuse_write_block(EFUSE_BLK4, &model, 0, 8);
                
                if (write_err == ESP_OK) {
                    ESP_LOGI(TAG, "Successfully written model");
                    
                    // Verify
                    uint8_t verify = 0;
                    esp_efuse_read_block(EFUSE_BLK4, &verify, 0, 8);
                    ESP_LOGI(TAG, "Verified value: %d", verify);
                } else {
                    ESP_LOGE(TAG, "Write failed: %s", esp_err_to_name(write_err));
                }
            }

            // Use alternative model
            if(model_alt != 0)
            {
                model = model_alt;
            }

            // Product Name
            if (this->model_ != nullptr)
            {
                constexpr const char* MODEL_NAMES[] = {
                    "Unset",
                    "Doorman S3"
                };
                constexpr size_t NUM_MODELS = sizeof(MODEL_NAMES) / sizeof(MODEL_NAMES[0]);
                const char* model_name = (model < NUM_MODELS) ? MODEL_NAMES[model] : "Unknown";
                
                ESP_LOGI(TAG, "Model: %s", model_name);

                this->model_->publish_state(model_name);
            }

            // Revision
            if (this->revision_ != nullptr)
            {
                char revision_str[32];
                snprintf(revision_str, sizeof(revision_str), "%u.%u.%u", revision[0], revision[1], revision[2]);
                this->revision_->publish_state(revision_str);
                ESP_LOGI(TAG, "Revision: %s", revision_str);
            }
        }
        else
        {
            ESP_LOGW(TAG, "No hardware revision available");
            
            if (this->model_ != nullptr)
            {
                this->model_->publish_state("Unknown");
            }
            
            if (this->revision_ != nullptr)
            {
                this->revision_->publish_state("0.0.0");
            }
        }
        #else
        ESP_LOGE(TAG, "Unsuppported platform!");

        if (this->model_ != nullptr)
        {
            this->model_->publish_state("Unsupported");
        }

        if (this->revision_ != nullptr)
        {
            this->revision_->publish_state("0.0.0");
        }
        #endif
    }

    void HardwareTextSensor::dump_config()
    {
        ESP_LOGCONFIG(TAG, "Hardware:");

        if (this->model_ != nullptr)
        {
            LOG_TEXT_SENSOR("  ", "Model", this->model_);
        }

        if (this->revision_ != nullptr)
        {
            LOG_TEXT_SENSOR("  ", "Revision", this->revision_);
        }
    }
}