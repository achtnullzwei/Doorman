<script setup>
import { inBrowser } from 'vitepress'
</script>
<script>
export default {
  data() {
    return {
        baseUrl: '../../firmware/release/',
        platform: '',
        integration: '',
        variant: '',
        platform_options: [
            {
                key: 'esp32-s3',
                name: 'ESP32-S3 (Octal)',
                icon: '',
                iconColor: '',
                details: 'Best choice for the <b>Doorman-S3</b> and any ESP32-S3 board with 8MB (octal) PSRAM.',
            },
            {
                key: 'esp32-s3-quad',
                name: 'ESP32-S3 (Quad)',
                icon: '',
                iconColor: '',
                details: 'For <b>Doorman-S3 rev 1.4</b> and ESP32-S3 boards with 4MB (quad) PSRAM.',
            },
            {
                key: 'esp32',
                name: 'ESP32',
                icon: '',
                iconColor: '',
                details: 'For standard ESP32 boards without PSRAM or with other custom configurations.',
            },
        ],
        integration_options: [
            {
                key: 'ha',
                name: 'Home Assistant',
                icon: '<img src="/home-assistant.svg" />',
                iconColor: '',
                details: 'I use Home Assistant and want the easiest way to integrate Doorman with my smart home.',
            },
            {
                key: 'mqtt',
                name: 'MQTT',
                icon: '<img src="/mqtt-ver-transp.svg" />',
                iconColor: '#606',
                details: 'I want Doorman to connect directly to my MQTT broker to use it with other platforms.',
            },
            {
                key: 'custom',
                name: 'Standalone',
                icon: '✨',
                iconColor: '',
                details: 'I don\'t use a smart home system — I just want Doorman to work on its own over Wi-Fi.',
            },
        ],
        variant_options: [
        {
            key: 'stock',
            name: 'Standard',
            icon: '',
            iconColor: '',
            details: 'I only need the basic features to control my intercom — nothing extra.',
        },
        {
            key: 'nuki-bridge',
            name: 'Nuki Bridge',
            icon: '',
            iconColor: '',
            details: 'I also want to control my Nuki Smart Lock and use Ring to Open with the apartment doorbell.',
        },
        ],
    }
  },
  computed: {
    fw_string() {
      return [this.platform, this.integration, this.variant].join('.');
    },
    manifest_file() {
      return this.baseUrl + this.fw_string + '/manifest.json';
    },
    valid_manifest() {
        return this.platform && this.integration && this.variant;
    },
    notes() {
        if(this.integration == 'mqtt') {
            return 'The MQTT Integration requires further setup! Please take a look at the <code>MQTT</code> section below.';
        }
        return '';
    },
    web_serial_available() {
        if(inBrowser) {
            return navigator.serial != undefined;
        }
        return false;
    }
  },
}
</script>

# Stock Firmware <Badge type="tip" text="ESP-IDF Framework" />

This is the preferred firmware as it works out of the box with the Doorman S3 and supports Improv via Bluetooth LE.

## Install or Update Firmware

There are several ways to update the firmware.

::: warning
Be aware that some **updates may introduce breaking changes** — always review the changelogs carefully to ensure your system functions reliably in critical situations.
:::

#### Web Serial <Badge type="tip" text="Latest release build, no customization" />
Connect your **Doorman-S3** via USB-C, then use the configuration assistant below to configure and install the latest firmware directly via Web Serial. This setup ensures seamless integration with the Home Assistant API and makes future community-driven updates easy to apply.

<div v-if="web_serial_available">
    <h5 class="title_row"><i class="mdi mdi-hammer-wrench"></i> Which hardware do you use?</h5>
    <div class="firmware_option_row">
        <label class="firmware_option" v-for="fw_platform in platform_options" :key="fw_platform.key">
            <input type="radio" class="reset_default" v-model="platform" :value="fw_platform.key">
            <span class="checkmark">
                <div class="icon" v-if="fw_platform.icon" v-html="fw_platform.icon"></div>
                <div class="title" v-html="fw_platform.name"></div>
                <div class="details" v-html="fw_platform.details"></div>
            </span>
        </label>
    </div>
    <div v-if="platform != ''">
        <h5 class="title_row"><i class="mdi mdi-home-automation"></i> Do use have any smart home system?</h5>
        <div class="firmware_option_row">
            <label class="firmware_option" v-for="fw_integration in integration_options" :key="fw_integration.key">
                <input type="radio" class="reset_default" v-model="integration" :value="fw_integration.key">
                <span class="checkmark">
                    <div class="icon" :style="{ 'background-color': fw_integration.iconColor }" v-if="fw_integration.icon" v-html="fw_integration.icon"></div>
                    <div class="title" v-html="fw_integration.name"></div>
                    <div class="details" v-html="fw_integration.details"></div>
                </span>
            </label>
        </div>
    </div>
    <div v-if="notes" class="important custom-block">
        <p class="custom-block-title">WAIT A MINUTE</p>
        <p v-html="notes"></p>
    </div>
    <div v-if="integration != ''">
        <h5 class="title_row"><i class="mdi mdi-package-variant-plus"></i> What about some extras?</h5>
        <div class="firmware_option_row">
            <label class="firmware_option" v-for="fw_variant in variant_options" :key="fw_variant.key">
                <input type="radio" class="reset_default" v-model="variant" :value="fw_variant.key">
                <span class="checkmark">
                    <div class="icon" v-if="fw_variant.icon" v-html="fw_variant.icon"></div>
                    <div class="title" v-html="fw_variant.name"></div>
                    <div class="details" v-html="fw_variant.details"></div>
                </span>
            </label>
        </div>
    </div>
    <div v-if="valid_manifest">
        <h5 class="title_row"><i class="mdi mdi-auto-fix"></i> Let's summon the firmware spirits!</h5>
        <esp-web-install-button :manifest="manifest_file">
            <button slot="activate">
                <div class="custom-layout">
                    <a class="btn">Install or Update Firmware</a>
                </div>
            </button>
            <div slot="unsupported">
                <div class="danger custom-block">
                    <p class="custom-block-title">OH SNAP!</p>
                    <p>Your Browser doesn't support Web Serial :(</p>
                </div>
            </div>
            <div slot="not-allowed">
                <div class="danger custom-block">
                    <p class="custom-block-title">OH SNAP!</p>
                    <p>You are not allowed to use this on HTTP :(</p>
                </div>
            </div>
        </esp-web-install-button>
    </div>
</div>
<div v-else class="danger custom-block">
    <p class="custom-block-title">BROWSER INCOMPATIBLE</p>
    <p>Since your browser doesn't support Web Serial, the configuration assistant isn't available. Please use a Chromium-based browser instead.</p>
</div>

#### HTTP OTA Updates <Badge type="tip" text="Latest release build, no customization" />
By integrating Doorman with Home Assistant, you unlock the convenience of automated firmware builds and seamless updates to the latest releases. The Update will appear directly in your Update list, allowing you to initiate an OTA update with just a click of the Update button — simple, efficient, and hassle-free.

#### ESPHome CLI / Dashboard <Badge type="warning" text="Full customization" />
For full customization and complete control over the firmware, you can adopt it into your ESPHome Dashboard or utilize the ESPHome CLI by executing:  
`esphome run <yamlfile.yaml>`

If you prefer using the MQTT-only firmware, simply apply the provided MQTT example YAML tailored for your board below.

##### Board Configuration Options: 
::: details ESP32-S3 (Octal PSRAM) / Doorman-S3 (1.5)
::: code-group
```yaml [Home Assistant]
<!--@include: ../../../../firmware/examples/esp32-s3.ha.stock.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32-s3.mqtt.stock.master.example.yaml-->
```
:::

::: details ESP32-S3 (Quad PSRAM) / Doorman-S3 (1.4)
::: code-group
```yaml [Home Assistant]
<!--@include: ../../../../firmware/examples/esp32-s3-quad.ha.stock.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32-s3-quad.mqtt.stock.master.example.yaml-->
```
:::

::: details Generic ESP32
#### GPIO configuration
- **TC:BUS RX:** GPIO 22
- **TC:BUS TX:** GPIO 23
- **Status LED:** GPIO 2
- **RGB Status LED:** GPIO 4
- **Relay:** GPIO 21
- **External button:** GPIO 20
- **ADC Input:** GPIO 36

::: code-group
```yaml [Home Assistant]
<!--@include: ../../../../firmware/examples/esp32.ha.stock.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32.mqtt.stock.master.example.yaml-->
```
:::

::: details Generic ESP8266
#### GPIO configuration
- **TC:BUS RX:** GPIO 12
- **TC:BUS TX:** GPIO 13
- **Status LED:** GPIO 2
- **RGB Status LED:** Dummy
- **Relay:** GPIO 14
- **External button:** GPIO 15
- **ADC Input:** A0

::: code-group
```yaml [Home Assistant]
<!--@include: ../../../../firmware/examples/esp8266.ha.stock.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp8266.mqtt.stock.master.example.yaml-->
```
:::

<!--@include: ./additions.md-->

<!--@include: ./mqtt.md-->

<style>
.title_row
{
    margin-top: 20px;
    margin-bottom: 20px;
}

.firmware_option_row
{
    display: flex;
    flex-wrap: wrap;
    margin-bottom: 8px;
    gap: 10px;
}

.firmware_option_row .firmware_option
{
    flex-grow: 1;
    flex-basis: 0;
}

.firmware_option_row .firmware_option input
{
    position: absolute;
    opacity: 0;
    cursor: pointer;
}

.firmware_option_row .firmware_option:hover input ~ .checkmark
{
    box-shadow: 0px 0px 0px 2px var(--vp-button-brand-bg) inset;
    color: var(--vp-c-text-1);
}

.firmware_option_row .firmware_option input:checked ~ .checkmark
{
    font-weight: 500;
    box-shadow: inset 0px 0px 0px 2px var(--vp-button-brand-bg);
    color: var(--vp-c-text-1);
}

.firmware_option_row .firmware_option .checkmark
{
    display: flex;
    flex-direction: column;
    padding: 24px;
    border: 1px solid var(--vp-c-bg-soft);
    border-radius: 12px;
    height: 100%;
    background-color: var(--vp-c-bg-soft);
    transition: border-color 0.25s, background-color 0.25s;
    cursor: pointer;
    color: var(--vp-c-text-2);
    min-width: 210px;
}

.firmware_option_row .firmware_option .checkmark .icon
{
    display: flex;
    justify-content: center;
    align-items: center;
    margin-bottom: 20px;
    border-radius: 6px;
    background-color: var(--vp-c-default-soft);
    width: 48px;
    height: 48px;
    font-size: 24px;
    transition: background-color 0.25s;
    padding: 10px;
}

.firmware_option_row .firmware_option .checkmark .title
{
    line-height: 24px;
    font-size: 16px;
    font-weight: 600;
}

.firmware_option_row .firmware_option .checkmark .details
{
    flex-grow: 1;
    padding-top: 8px;
    line-height: 24px;
    font-size: 14px;
    font-weight: 500;
    color: var(--vp-c-text-2);
}
</style>