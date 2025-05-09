# Stock Firmware <Badge type="tip" text="ESP-IDF Framework" />

This is the preferred firmware as it works out of the box with the Doorman S3 and supports Improv via Bluetooth LE.

## Install or Update Firmware

There are several ways to update the firmware.

::: warning
Be aware that some **updates may introduce breaking changes** — always review the changelogs carefully to ensure your system functions reliably in critical situations.
:::

#### Web Serial <Badge type="tip" text="Latest release build, no customization" />
Connect your **Doorman-S3** via USB-C, then use the configuration assistant below to configure and install the latest firmware directly via Web Serial. This setup ensures seamless integration with the Home Assistant API and makes future community-driven updates easy to apply.

<FirmwareConfig baseUrl="../../firmware/release/" />

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