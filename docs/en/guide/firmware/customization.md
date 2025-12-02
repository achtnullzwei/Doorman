# Customization

Explore various examples to customize and extend the Doorman firmware. From adding custom sensors to tweaking automation settings, these examples help you tailor the device to your specific needs. Follow the guides to unlock full flexibility and make the most of your setup.

## Create a simple TC Telegram Binary Sensor
You can easily add custom binary sensors for any TC Telegram, in addition to the preconfigured ones. This allows you to listen for and respond to any other bus telegram, expanding the functionality of your system.

::: code-group
```yaml [Telegram Builder]
<!--@include: minimal.example.yaml-->

binary_sensor: # [!code ++] [!code focus]
  - platform: tc_bus # [!code ++] [!code focus]
    name: "Custom Telegram" # [!code ++] [!code focus]
    type: open_door # [!code ++] [!code focus]
    address: 0 # [!code ++] [!code focus]
    web_server: # [!code ++] [!code focus]
      sorting_group_id: sorting_group_listeners # [!code ++] [!code focus]
```
```yaml [32-Bit Telegrams]
<!--@include: minimal.example.yaml-->

binary_sensor: # [!code ++] [!code focus]
  - platform: tc_bus # [!code ++] [!code focus]
    name: "Custom Telegram" # [!code ++] [!code focus]
    telegram: 0x00001100 # [!code ++] [!code focus]
    web_server: # [!code ++] [!code focus]
      sorting_group_id: sorting_group_listeners # [!code ++] [!code focus]
```
:::

## Control the internal RGB Status LED
To control the onboard RGB LED with a button (for example), just reference the light entity with the internal id: `doorman_rgb_status_led`.
```yaml
<!--@include: minimal.example.yaml-->

button: # [!code ++] [!code focus]
  - platform: template # [!code ++] [!code focus]
    name: "Turn on Status RGB LED to red" # [!code ++] [!code focus]
    on_press: # [!code ++] [!code focus]
      - light.turn_on: # [!code ++] [!code focus]
          id: doorman_rgb_status_led # [!code ++] [!code focus]
          red: 100% # [!code ++] [!code focus]
          green: 0% # [!code ++] [!code focus]
          blue: 0% # [!code ++] [!code focus]
```

## Use the External Hardware Button
If you want to use the external button to trigger automations, extend either the binary sensor or the event entity (which already provides several event types) with your own custom actions.

::: code-group
```yaml [Binary Sensor]
<!--@include: minimal.example.yaml-->

binary_sensor: # [!code ++] [!code focus]
  - id: !extend doorman_external_button # [!code ++] [!code focus]
    on_press: # [!code ++] [!code focus]
      - logger.log: "External button pressed!" # [!code ++] [!code focus]
```
```yaml [Event]
<!--@include: minimal.example.yaml-->

event: # [!code ++] [!code focus]
  - id: !extend doorman_external_button_event # [!code ++] [!code focus]
    on_event: # [!code ++] [!code focus]
      - lambda: |- # [!code ++] [!code focus]
          ESP_LOGD("main", "External Button Event %s triggered.", event_type.c_str()); # [!code ++] [!code focus]
```
:::

## Use I²C Sensors
If you want to add sensors via the I²C bus, you can use the two available GPIO pins and add the I²C component to your configuration. You can then attach your sensors to these two I²C GPIO pins.

::: code-group
```yaml [Doorman up to rev. 1.5]
<!--@include: minimal.example.yaml-->

i2c: # [!code ++] [!code focus]
  sda: GPIO40 # [!code ++] [!code focus]
  scl: GPIO48 # [!code ++] [!code focus]
  scan: true # [!code ++] [!code focus]
  id: i2c_bus # [!code ++] [!code focus]
```
```yaml [Doorman rev. 1.6 and newer]
<!--@include: minimal.example.yaml-->

i2c: # [!code ++] [!code focus]
  sda: GPIO40 # [!code ++] [!code focus]
  scl: GPIO41 # [!code ++] [!code focus]
  scan: true # [!code ++] [!code focus]
  id: i2c_bus # [!code ++] [!code focus]
```
:::

:::warning REVISION 1.6+
Beginning with Doorman revision 1.6, the external button must be removed from your configuration YAML.
:::

## Create Your Own Doorbell Pattern
If you want to create a custom doorbell pattern, you can easily extend the existing doorbell entities. For more information about patterns, refer to the [ESPHome Docs](https://esphome.io/components/binary_sensor/index.html#on-multi-click).

```yaml
<!--@include: minimal.example.yaml-->

# Extend the doorbell_pattern event entity // [!code ++] [!code focus]
# Add a new special event type // [!code ++] [!code focus]
event: # [!code ++] [!code focus]
  - id: !extend apartment_doorbell_pattern # [!code ++] [!code focus]
    event_types: # [!code ++] [!code focus]
      - "special" # [!code ++] [!code focus]

# Extend the Ring-To-Open Pattern Conditions // [!code ++] [!code focus]
# You need to extend each Door Pattern Condition where you want to use the pattern // [!code ++] [!code focus]
select: # [!code ++] [!code focus]
  - id: !extend rto_apartment_door_pattern_condition # [!code ++] [!code focus]
    options: # [!code ++] [!code focus]
      - "special" # [!code ++] [!code focus]

# Extend the apartment_doorbell / entrance_doorbell entity // [!code ++] [!code focus]
# and add your new special pattern // [!code ++] [!code focus]
binary_sensor: # [!code ++] [!code focus]
  - id: !extend apartment_doorbell # [!code ++] [!code focus]
    on_multi_click: # [!code ++] [!code focus]
      # Special Pattern // [!code ++] [!code focus]
      - timing: # [!code ++] [!code focus]
          # Press twice quickly with at most half a second in between. // [!code ++] [!code focus]
          - ON for at most 0.3s # [!code ++] [!code focus]
          - OFF for at most 0.5s # [!code ++] [!code focus]
          - ON for at most 0.3s # [!code ++] [!code focus]
          - OFF for 1s to 1.5s # [!code ++] [!code focus]
          # Press once slowly with at most 1.5 seconds in between. // [!code ++] [!code focus]
          - ON for at most 0.3s # [!code ++] [!code focus]
          - OFF for 1s to 1.5s # [!code ++] [!code focus]
          # Press once // [!code ++] [!code focus]
          - ON for at most 0.3s # [!code ++] [!code focus]
          - OFF for at least 2s # [!code ++] [!code focus]
        then: # [!code ++] [!code focus]
          - logger.log: "Special pattern detected!" # [!code ++] [!code focus]
          - event.trigger: # [!code ++] [!code focus]
              id: apartment_doorbell_pattern # [!code ++] [!code focus]
              # Use the previously defined new event type here // [!code ++] [!code focus]
              event_type: special # [!code ++] [!code focus]
```

## Light On When Entrance Doorbell Rings
You can easily set up an automation to turn on the light whenever someone rings the entrance doorbell. This can be useful for enhancing visibility or simply making it easier to see who is at the door. Simply configure the automation to trigger the light based on the doorbell event.

::: code-group
```yaml [Basic]
<!--@include: minimal.example.yaml-->

binary_sensor: # [!code ++] [!code focus]
  - id: !extend entrance_doorbell # [!code ++] [!code focus]
    on_press: # [!code ++] [!code focus]
      - tc_bus.send: # [!code ++] [!code focus]
          type: "light" # [!code ++] [!code focus]
```
```yaml [Based on sun's elevation]
<!--@include: minimal.example.yaml-->

# Import the Home Assistant sun elevation sensor // [!code ++] [!code focus]
sensor: # [!code ++] [!code focus]
  - platform: homeassistant # [!code ++] [!code focus]
    id: sun_elevation # [!code ++] [!code focus]
    entity_id: sun.sun # [!code ++] [!code focus]
    attribute: elevation # [!code ++] [!code focus]

# Extend the entrance doorbell sensor // [!code ++] [!code focus]
binary_sensor: # [!code ++] [!code focus]
  - id: !extend entrance_doorbell # [!code ++] [!code focus]
    on_press: # [!code ++] [!code focus]
      # Sun elevation is <= 0 (dark) // [!code ++] [!code focus]
      - if: # [!code ++] [!code focus]
          condition: # [!code ++] [!code focus]
            sensor.in_range: # [!code ++] [!code focus]
              id: sun_elevation # [!code ++] [!code focus]
              below: 1 # [!code ++] [!code focus]
          then: # [!code ++] [!code focus]
            # Turn on the light // [!code ++] [!code focus]
            - tc_bus.send: # [!code ++] [!code focus]
                type: "light" # [!code ++] [!code focus]
```
:::

## Mix and Match
Want to customize the firmware and use only the features you need? Good news!  
The Doorman firmware is modular, organized into packages that you can easily mix and match.

Don't need a feature, like the indoor station settings? Simply comment it out in your configuration.
The order of the packages is important because some depend on others.

Check out the example YAML configuration below for guidance:

:::details Example configuration
```yaml 
# Doorman S3 DIY Firmware

# You can change a few options here.
substitutions:
  name: "doorman-s3"
  friendly_name: "Doorman S3"
  
  # Required for "External Components"
  branch: "master"

packages:
  # Device type
  # Either esp32, esp32-s3 or esp32-s3-quad
  host: github://azoninc/doorman/firmware/hosts/esp32-s3.yaml@master
  # host: github://azoninc/doorman/firmware/hosts/esp32-s3-quad.yaml@master
  # host: github://azoninc/doorman/firmware/hosts/esp32.yaml@master

  # External Components (REQUIRED)
  external_components: github://azoninc/doorman/firmware/components/external-components.yaml@master

  # RGB Status LED (REQUIRED)
  rgb_status_led: github://azoninc/doorman/firmware/components/rgb-status-led.yaml@master
  rgb_status_led_effects: github://azoninc/doorman/firmware/components/rgb-status-led.effects.yaml@master

  # Base Doorman Firmware (REQUIRED)
  base: github://azoninc/doorman/firmware/base.yaml@master

  # Improv BLE
  # Incompatible with Nuki Bridge
  bluedroid_ble: github://azoninc/doorman/firmware/components/bluedroid-ble.yaml@master

  # OTA via Webserver & Device Builder
  ota_update: github://azoninc/doorman/firmware/components/ota-update.esphome.yaml@master

  # Smart Home Integration
  # Either homeassistant, mqtt, homekit or custom
  # Use custom, if you just want to use it via wifi (http requests)
  api: github://azoninc/doorman/firmware/components/api.homeassistant.yaml@master
  # api: github://azoninc/doorman/firmware/components/api.mqtt.yaml@master
  # api: github://azoninc/doorman/firmware/components/api.homekit.yaml@master
  # api: github://azoninc/doorman/firmware/components/api.custom.yaml@master
  
  # TC:BUS debug tools
  debug_utilities: github://azoninc/doorman/firmware/components/debug-utilities.yaml@master

  # ESPHome debug sensors
  debug_component: github://azoninc/doorman/firmware/components/debug-component.yaml@master
  debug_component_psram: github://azoninc/doorman/firmware/components/debug-component.psram.yaml@master

  # Pattern events
  pattern_events: github://azoninc/doorman/firmware/components/pattern-events.yaml@master

  # Ring To Open automation
  ring_to_open: github://azoninc/doorman/firmware/components/ring-to-open.yaml@master

  # TC:BUS Device Settings (Indoor Station)
  # Ringtones, Volume
  indoor_station_settings: github://azoninc/doorman/firmware/components/indoor-station-settings.yaml@master

  # Nuki Bridge
  # Incompatible with Improv BLE (bluedroid-ble)
  # addon_nuki_bridge: !include github://azoninc/doorman/firmware/components/nuki-bridge.yaml@master

  # Interactive Setup Mode
  interactive_setup: github://azoninc/doorman/firmware/components/interactive-setup.yaml@master

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
```
:::