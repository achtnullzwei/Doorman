# Nuki Bridge Firmware <Badge type="tip" text="ESP-IDF Framework" />

This configuration inherits all features from the Stock Firmware and adds a [Nuki Bridge](https://github.com/uriyacovy/ESPHome_nuki_lock) component via BLE. Note that it does not support Improv via Bluetooth LE, as the BLE stack is used by the Nuki Bridge component.

## Install or Update Firmware

There are several ways to update the firmware.

::: warning
Be aware that some **updates may introduce breaking changes** — always review the changelogs carefully to ensure your system functions reliably in critical situations.
:::

#### Web Serial <Badge type="tip" text="Latest release build, no customization" />
Connect your **Doorman-S3** via USB-C, then use the configuration assistant below to configure and install the latest firmware directly via Web Serial. This setup ensures seamless integration with the Home Assistant API and makes future community-driven updates easy to apply.

<FirmwareConfig baseUrl="../../firmware/release/" variantSelection="nuki-bridge" />

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
<!--@include: ../../../../firmware/examples/esp32-s3.ha.nuki-bridge.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32-s3.mqtt.nuki-bridge.master.example.yaml-->
```
:::

::: details ESP32-S3 (Quad PSRAM) / Doorman-S3 (1.4)
::: code-group
```yaml [Home Assistant]
<!--@include: ../../../../firmware/examples/esp32-s3-quad.ha.nuki-bridge.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32-s3-quad.mqtt.nuki-bridge.master.example.yaml-->
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
<!--@include: ../../../../firmware/examples/esp32.ha.nuki-bridge.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp32.mqtt.nuki-bridge.master.example.yaml-->
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
<!--@include: ../../../../firmware/examples/esp8266.ha.nuki-bridge.master.example.yaml-->
```
```yaml [MQTT]
<!--@include: ../../../../firmware/examples/esp8266.mqtt.nuki-bridge.master.example.yaml-->
```
:::

## Pair your Nuki Smart Lock

::: warning
If your Smart Lock is already paired, this process will unpair it!
:::

Depending on the PCB revision, either press the `FLASH`- or `PRG`-button on the Doorman-S3 PCB for 5 seconds until the status LED starts flashing purple, or enable the Nuki Pairing Mode switch in Home Assistant / ESPHome Webinterface. Then, press the button on your Smart Lock for 5 seconds until the light ring turns on.

After a successful pairing, the status LED will remain steadily on for 3 seconds.

## Unpair your Nuki Smart Lock
You can unpair your device using either the `Nuki Unpair Device` button in Home Assistant / ESPHome Webinterface or the physical `FLASH`- or `PRG`-button on the Doorman-S3 PCB.

::: info Physical Button
If your Smart Lock is already paired with Doorman, press the `FLASH`- or `PRG`-button on the Doorman-S3 PCB for 5 seconds until the RGB status LED starts flashing purple. Your Smart Lock will then be unpaired. Note that the pairing mode will time out after 30 seconds.
:::

<!--@include: ./additions.md-->

<!--@include: ./mqtt.md-->