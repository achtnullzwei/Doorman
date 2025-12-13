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
        uint8_t product = 0;

        esp_err_t rev_err = esp_efuse_read_block(EFUSE_BLK3, revision, 0, 24);
        esp_err_t prod_err = esp_efuse_read_block(EFUSE_BLK3, &product, 24, 8);

        if (rev_err == ESP_OK && prod_err == ESP_OK && revision[0] > 0)
        {
            // Product Name
            if (this->product_ != nullptr)
            {
                constexpr const char* PRODUCT_NAMES[] = {
                    "Doorman S3" // 0
                };
                constexpr size_t NUM_PRODUCTS = sizeof(PRODUCT_NAMES) / sizeof(PRODUCT_NAMES[0]);
                const char* product_name = (product < NUM_PRODUCTS) ? PRODUCT_NAMES[product] : "Unknown";
                
                ESP_LOGI(TAG, "Product: %s", product_name);

                this->product_->publish_state(product_name);
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
            
            if (this->product_ != nullptr)
            {
                this->product_->publish_state("Unknown");
            }
            
            if (this->revision_ != nullptr)
            {
                this->revision_->publish_state("0.0.0");
            }
        }
        #else
        ESP_LOGE(TAG, "Unsuppported platform!");

        if (this->product_ != nullptr)
        {
            this->product_->publish_state("Unsupported");
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

        if (this->product_ != nullptr)
        {
            LOG_TEXT_SENSOR("  ", "Product", this->product_);
        }

        if (this->revision_ != nullptr)
        {
            LOG_TEXT_SENSOR("  ", "Revision", this->revision_);
        }
    }
}