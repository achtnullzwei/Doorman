# TC:BUS Device Component <Badge type="tip" text="tc_bus_device" />
This component extends the [TC:BUS](./esphome-component) base component, making it easier to communicate with individual devices on the bus. It lets you identify devices, access their memory, and read or modify their settings - take a look at the [supported models and settings](#model-setting-availability).

## Configuration
The `tc_bus_device` component offers the following configuration options:

| Option                    | Description                                                                                                                                   | Required | Default       |
|---------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------|----------|---------------|
| `id`                      | Unique ID for the component.                                                                                                                  | Yes      |               |
| `type`                    | Device Group of the TC:BUS Device. E.g. Indoor Station, Outdoor Station.                                                                      | Yes      |               |
| `auto_configuration`      | When enabled, the component automatically identifies the device using the serial number and reads device memory based on the device model.    | No       | false         |
| `on_read_memory_complete` | Defines actions to be triggered when the memory reading is complete. Returns a `std::vector<uint8_t>` buffer as the `x` variable.             | No       |               |
| `on_read_memory_timeout`  | Defines actions to be triggered when the memory reading times out.                                                                            | No       |               |
| `on_identify_complete`    | Defines actions to be triggered when the identification of the indoor station is complete. Returns a `ModelData` object as the `x` variable.  | No       |               |
| `on_identify_unknown`     | Defines actions to be triggered when the identification of the indoor station completes with unknown model.                                   | No       |               |
| `on_identify_timeout`     | Defines actions to be triggered when the identification of the indoor station times out.                                                      | No       |               |


## Number Inputs
The `tc_bus_device` Number Input platform offers the following configuration options:

| Option                         | Description                                                                                                   | Required | Default       |
|--------------------------------|---------------------------------------------------------------------------------------------------------------|----------|---------------|
| `tc_bus_device_id` | ID of the related `tc_bus_device` instance.                                                             | Yes      |               |
| `serial_number`                | Indoor Station Serial Number Input to set the serial number of the predefined indoor station.                 | No       |               |
| `volume_handset_door_call`     | Door Call Handset Volume Number Input to set the handset volume for door calls of your indoor station.        | No       |               |
| `volume_handset_internal_call` | Internal Call Handset Volume Number Input to set the handset volume for internal calls of your indoor station.| No       |               |
| `volume_ringtone`              | Ringtone Volume Number Input to set the ringtone volume of your indoor station.                               | No       |               |


## Select Inputs
The `tc_bus_device` Select platform offers the following configuration options:

| Option                               | Description                                                                                                    | Required | Default       |
|--------------------------------------|----------------------------------------------------------------------------------------------------------------|----------|---------------|
| `tc_bus_device_id`                   | ID of the related `tc_bus_device` instance.                                                                    | Yes      |               |
| `model`                              | Model Select to set the model of your device (used to read and write settings). Take a look at the [supported models and settings](#model-setting-availability).| No       | `None`        |
| `ringtone_entrance_door_call`        | Entrance Door Call Ringtone Select to set the entrance door call ringtone of your indoor station.              | No       |               |
| `ringtone_second_entrance_door_call` | Second Entrance Door Call Ringtone Select to set the second entrance door call ringtone of your indoor station.| No       |               |
| `ringtone_floor_call`                | Floor Call Ringtone Select to set the floor call ringtone of your indoor station.                              | No       |               |
| `ringtone_internal_call`             | Internal Call Ringtone Select to set the internal call ringtone of your indoor station.                        | No       |               |


## Switches
The `tc_bus_device` Switch platform offers the following configuration options:

| Option                               | Description                                                                                                    | Required | Default       |
|--------------------------------------|----------------------------------------------------------------------------------------------------------------|----------|---------------|
| `tc_bus_device_id`                   | ID of the related `tc_bus_device` instance.                                                                    | Yes      |               |
| `force_long_door_opener`             | This enforces execution of the long door opener telegram and mandates inclusion of a serial number in the short door opener telegram. | No       | |

## Buttons
The `tc_bus_device` Button platform offers the following configuration options:

| Option                | Description                                                                                       | Required | Default       |
|-----------------------|---------------------------------------------------------------------------------------------------|----------|---------------|
| `tc_bus_device_id`    | ID of the related `tc_bus_device` instance.                                                       | Yes      |               |
| `identify_device`     | This starts the identification process to determine the device model by using it's serial number. | No       | |
| `read_memory`         | This reads the device memory if supported into a memory buffer. Take a look at the [supported models and settings](#model-setting-availability). | No       | |

## Binary Sensors
The `tc_bus_device` Binary Sensor detects binary states such as doorbell presses. It can be configured to trigger based on a predefined telegram.

| Option           | Description                                                                                              | Required | Default       |
|------------------|----------------------------------------------------------------------------------------------------------|----------|---------------|
| `id`             | Unique ID for the binary sensor component.                                                               | Yes      |               |
| `tc_bus_device_id` | ID of the related `tc_bus_device` instance.                                                            | Yes      |               |
| `icon`           | Icon to represent the sensor in the UI.                                                                  | No       | `mdi:doorbell`|
| `name`           | Name of the binary sensor.                                                                               | No       | `Doorbell`    |
| `auto_off`       | Time period after which the sensor automatically turns off, useful for momentary signals like doorbell presses.  | No       | `3s`          |
| `type`           | Telegram type that will trigger the binary sensor, used alongside `address` and `payload`.               | Yes       | `unknown`     |
| `address`        | 8-bit address that serves as a condition to trigger the binary sensor. If you set it to `255`, it will catch all addresses. | No       | `0`           |
| `payload`        | 32-bit payload that serves as a condition to trigger the binary sensor.                                  | No       | `0`           |

## Callbacks
### Read Memory Complete <Badge type="tip" text="on_read_memory_complete" />
This callback allows you to work with the memory buffer, accessible as the `x` variable.

```yaml
on_read_memory_complete:
  - logger.log: "Completed memory reading!"
  - lambda: |-
      std::string hexString = str_upper_case(format_hex(x));
      ESP_LOGI("tc_bus", "Memory Dump: %s", hexString.c_str());
```

### Read Memory Timeout <Badge type="tip" text="on_read_memory_timeout" />
This callback allows you to detect a failed memory reading. Most probably when a model doesn't support the related telegrams.

```yaml
on_read_memory_timeout:
  - logger.log: "Failed to read Memory"
```

### Device Identification Complete <Badge type="tip" text="on_identify_complete" />
This callback allows you to utilize the [ModelData](#model-data) struct, accessible as the `x` variable.

```yaml
on_identify_complete:
  - logger.log: "Completed identification!"
  - lambda: |-
      std::string hexString = str_upper_case(format_hex(x));
      ESP_LOGI("tc_bus", "Memory Dump: %s", hexString.c_str());
```

### Device Identification Complete (Unknown) <Badge type="tip" text="on_identify_unknown" />
This callback allows you to detect an unknown model identification of the device. Most probably when a model is too old and doesn't support this process or is not implemented yet.

```yaml
on_identify_unknown:
  - logger.log: "Failed to identify device - unknown model!"
```

### Device Identification Timeout <Badge type="tip" text="on_identify_timeout" />
This callback allows you to detect a failed identification of the device. Most probably when a model is too old doesn't support this process.

```yaml
on_identify_timeout:
  - logger.log: "Failed to identify device!"
```

## Actions
### Read Memory <Badge type="tip" text="tc_bus_device.read_memory" />
This action allows you to read the memory of any supported device on the bus.

```yaml
on_...:
  - tc_bus_device.read_memory:
      id: my_tc_bus_indoor_station_device
```

### Identify devices <Badge type="tip" text="tc_bus_device.identify" />
This action allows you to automatically detect the model of a supported device on the bus.

::: tip Note
Automatic identification is not supported by all devices. Currently, only indoor stations are fully supported in the identification process. However, you will still receive a response with the device's identification data, even for unsupported models. Feel free to open an Issue with the identification response data in order to implement it.
:::

```yaml
on_...:
  - tc_bus_device.identify:
      id: my_tc_bus_indoor_station_device
```

### Update Settings <Badge type="tip" text="tc_bus_device.update_setting" />
This action allows you to change the supported settings of any supported device on the bus.
Take a look at the [supported models and settings](#model-setting-availability).

```yaml
on_...:
  - tc_bus_device.update_setting:
      id: my_tc_bus_indoor_station_device
      type: volume_ringtone
      value: 7
```

### Update Doorbell Buttons <Badge type="tip" text="tc_bus_device.update_doorbell_button" />
This action allows you to configure the physical doorbell buttons of an outdoor station device.

::: warning EXPERIMENTAL
This feature is experimental, use with caution.
:::

#### Supported actions:

- `none` — No action is assigned.
- `door_call` — Triggers a door call.
- `light` — Controls the light function.
- `control_function` — Executes a custom control function.

If no action is specified, the default value is `none`.
If no payload is provided, it remains unset.

For matrix systems, you can use the `button_col` parameter. For other systems, this parameter is not required.

```yaml
on_...:
  - tc_bus_device.update_doorbell_button:
      id: my_tc_bus_outdoor_station_device
      button_row: 1
      button_col: 1
      primary_action: door_call
      primary_payload: 798906
      secondary_action: control_function
      secondary_payload: 8
```

### Sending Telegrams <Badge type="tip" text="tc_bus_device.send" />
You can send device related telegrams on the bus using this action.

```yaml
on_...:
  - tc_bus_device.send:
      id: my_tc_bus_indoor_station_device
      type: open_door
      address: 0
      payload: 0
```

## Example YAML Configuration
This is an example configuration for the component in ESPHome:

```yaml
external_components:
  - source: github://azoninc/doorman@master
    components: [ tc_bus, tc_bus_device ]

## RMT configuration
remote_receiver:
  pin:
    number: GPIO9
    mode: INPUT
  filter: 1500us
  idle: 7000us

remote_transmitter:
  pin:
    number: GPIO8
    mode: OUTPUT
  carrier_duty_percent: 100%

# TC:BUS configuration
tc_bus:

# TC:BUS Device configuration
tc_bus_device:
  - id: my_tc_bus_indoor_station_device
    type: indoor_station
    on_read_memory_complete:
      - lambda: |-
          std::string hexString = str_upper_case(format_hex(x));
          ESP_LOGI("tc_bus", "Memory reading completed. Data: %s", hexString.c_str());
    on_read_memory_timeout:
      - logger.log:
          format: "Memory reading timed out! No memory block received in time."
          level: ERROR
    on_identify_complete:
      - logger.log:
          format: "Identified Hardware: %s (v%i) | Firmware: %i.%i.%i"
          args: [ 'model_to_string(x.model)', 'x.hardware_version', 'x.firmware_major', 'x.firmware_minor', 'x.firmware_patch' ]
          level: INFO
    on_identify_unknown:
      - logger.log: "Setup: Unable to identify the Indoor Station, unknown model."
    on_identify_timeout:
      - logger.log:
          format: "Failed to identify the Indoor Station. Please select it manually."
          level: ERROR

number:
  - platform: tc_bus_device
    tc_bus_device_id: my_tc_bus_indoor_station_device
    serial_number:
      name: "Serial Number"
    volume_ringtone:
      name: "Volume: Ringtone"
    volume_handset_door_call:
      name: "Volume: Handset Door Call"
    volume_handset_internal_call:
      name: "Volume: Handset Internal Call"

select:
  - platform: tc_bus_device
    tc_bus_device_id: my_tc_bus_indoor_station_device
    model:
      name: "Model"
    ringtone_entrance_door_call:
      name: "Ringtone: Entrance Door Call"
    ringtone_second_entrance_door_call:
      name: "Ringtone: Second Entrance Door Call"
    ringtone_floor_call:
      name: "Ringtone: Floor Call"
    ringtone_internal_call:
      name: "Ringtone: Internal Call"

switch:
  - platform: tc_bus_device
    tc_bus_device_id: my_tc_bus_indoor_station_device
    force_long_door_opener:
      name: "Enforce long Door Opener Telegram"

button:
  - platform: tc_bus_device
    tc_bus_device_id: my_tc_bus_indoor_station_device
    identify_device:
      name: "Identify Device"
    read_memory:
      name: "Read Memory"

binary_sensor:
  - platform: tc_bus_device
    tc_bus_device_id: my_tc_bus_indoor_station_device
    id: doorbell_sensor
    name: "Outdoor Station Doorbell (parser)"
    icon: "mdi:doorbell"
    type: door_call
```

## Advanced Configuration

### Accessing device settings
If you need to access the supported settings in the memory buffer you can use the `get_setting` and `update_setting` methods of the `tc_bus_device` instance. Take a look at the [setting types](#setting-types).

Example (read and write setting):
```yaml
button:
  - platform: template
    name: "Read Handset volume via lambda"
    on_press:
      - lambda: |-
          ESP_LOGD("TAG", "Handset volume: %i", id(my_tc_bus_indoor_station_device)->get_setting(SETTING_VOLUME_HANDSET_DOOR_CALL));

  - platform: template
    name: "Set Handset volume via lambda"
    on_press:
      - lambda: |-
          id(my_tc_bus_indoor_station_device)->update_setting(SETTING_VOLUME_HANDSET_DOOR_CALL, 7);

  - platform: template
    name: "Set Handset volume via action"
    on_press:
      - tc_bus_device.update_setting:
          id: my_tc_bus_indoor_station_device
          type: volume_handset_door_call
          value: 7
```

### Accessing outdoor station doorbell button configuration
If you need to access the doorbell button configuration of an outdoor station in the memory buffer you can use the `get_doorbell_button` and `update_doorbell_button` methods of the `tc_bus_device` instance. Take a look at the [doorbell button actions](#doorbell-button-actions).

Example (read and write doorbell button actions):
```yaml
button:
  - platform: template
    name: "Read Actions of Doorbell Button 1 via lambda"
    on_press:
      - lambda: |-
          DoorbellButtonAction button = id(my_tc_bus_outdoor_station_device)->get_doorbell_button(1);
          ESP_LOGD("TAG", "Primary Action: %x", button.primary_action);
          ESP_LOGD("TAG", "Primary Payload: %x", button.primary_payload);
          ESP_LOGD("TAG", "Secondary Action: %x", button.secondary_action);
          ESP_LOGD("TAG", "Secondary Payload: %x", button.secondary_payload);

  - platform: template
    name: "Set Doorbell Button 1 via lambda"
    on_press:
      - lambda: |-
          DoorbellButtonAction button;
          button.primary_action = DOORBELL_BUTTON_ACTION_DOOR_CALL;
          button.primary_payload = 123456;

          id(my_tc_bus_outdoor_station_device)->update_doorbell_button(1, button);

  - platform: template
    name: "Set Doorbell Button 1 via action"
    on_press:
      - tc_bus_device.update_doorbell_button:
          id: my_tc_bus_outdoor_station_device
          button_row: 1
          primary_action: door_call
          primary_payload: 123456

  - platform: template
    name: "Set Matrix Button 1;5 via action"
    on_press:
      - tc_bus_device.update_doorbell_button:
          id: my_tc_bus_outdoor_station_device
          button_row: 1
          button_col: 5
          primary_action: door_call
          primary_payload: 123456

  - platform: template
    name: "Reset Doorbell Button 1 via action"
    on_press:
      - tc_bus_device.update_doorbell_button:
          id: my_tc_bus_outdoor_station_device
          button_row: 1
```

## Doorbell Button Data
The `DoorbellButtonConfig` struct is used internally.

```c++
struct DoorbellButtonConfig
 {
    DoorbellButtonAction primary_action = DOORBELL_BUTTON_ACTION_NONE;
    uint32_t primary_payload = DOORBELL_BUTTON_UNASSIGNED;
    DoorbellButtonAction secondary_action = DOORBELL_BUTTON_ACTION_NONE;
    uint32_t secondary_payload = DOORBELL_BUTTON_UNASSIGNED;
};
```

## Model Data
The `ModelData` struct is used internally in the identification process.

```c++
struct ModelData {
    Model model = MODEL_NONE;

    uint32_t firmware_version = 0;
    uint8_t firmware_major = 0;
    uint8_t firmware_minor = 0;
    uint8_t firmware_patch = 0;
    uint8_t hardware_version = 0; 
    
    uint8_t device_group = 0;
    uint8_t memory_size = 0;
    uint32_t capabilities = 0;
};
```

## Doorbell Button Actions
Here are the available doorbell button actions you can use to update the doorbell button configuration of your outdoor station:

- none <Badge type="tip" text="DOORBELL_BUTTON_ACTION_NONE" />
- light <Badge type="tip" text="DOORBELL_BUTTON_ACTION_LIGHT" />
- door_call <Badge type="tip" text="DOORBELL_BUTTON_ACTION_DOOR_CALL" />
- control_function <Badge type="tip" text="DOORBELL_BUTTON_ACTION_CONTROL_FUNCTION" />

## Setting Types
Here are the available setting types you can use to update the settings of your bus devices.

### Indoor Station
- ringtone_floor_call <Badge type="tip" text="SETTING_RINGTONE_FLOOR_CALL" />
- ringtone_entrance_door_call <Badge type="tip" text="SETTING_RINGTONE_ENTRANCE_DOOR_CALL" />
- ringtone_second_entrance_door_call <Badge type="tip" text="SETTING_RINGTONE_SECOND_ENTRANCE_DOOR_CALL" />
- ringtone_internal_call <Badge type="tip" text="SETTING_RINGTONE_INTERNAL_CALL" />
- volume_ringtone <Badge type="tip" text="SETTING_VOLUME_RINGTONE" />
- volume_handset_door_call <Badge type="tip" text="SETTING_VOLUME_HANDSET_DOOR_CALL" />
- volume_handset_internal_call <Badge type="tip" text="SETTING_VOLUME_HANDSET_INTERNAL_CALL" />
- volume_handset_internal_call <Badge type="tip" text="SETTING_AS_ADDRESS_DIVIDER" />
- volume_handset_internal_call <Badge type="tip" text="SETTING_VAS_ADDRESS_DIVIDER" />

### Outdoor Station
- as_address <Badge type="tip" text="SETTING_AS_ADDRESS" />
- as_address_lock <Badge type="tip" text="SETTING_AS_ADDRESS_LOCK" />
- button_rows <Badge type="tip" text="SETTING_BUTTON_ROWS" />
- talking_requires_door_readiness <Badge type="tip" text="SETTING_TALKING_REQUIRES_DOOR_READINESS" />
- door_opener_duration <Badge type="tip" text="SETTING_DOOR_OPENER_DURATION" />
- door_readiness_duration <Badge type="tip" text="SETTING_DOOR_READINESS_DURATION" />
- calling_duration <Badge type="tip" text="SETTING_CALLING_DURATION" />
- has_code_lock <Badge type="tip" text="SETTING_HAS_CODE_LOCK" />

## Model Setting Availability
In general, **all listed models** are supported.
However, some support configuration directly via Doorman, while others don't.

### Indoor Stations
Below is a list of available settings for specific indoor station models:

| Model                        | Available settings |
|------------------------------|--------------------|
| TCS ISH1030 / Koch TTS25     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_internal_call` |
| TCS TTC-XX                   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_internal_call` |
| TCS TTS-XX                   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_internal_call` |
| TCS ISH3030 / Koch TCH50 / Scantron Lux2 | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISH3130 / Koch TCH50P / Scantron LuxPlus | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISH3230 / Koch TCH50 GFA | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISH3340                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call` |
| TCS ISW3030 / Koch TC50 / Scantron Stilux | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISW3230 / Koch TC50 GFA  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISW3340                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISW3130 / Koch TC50P     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IVH3222 / Koch VTCH50 / Scantron VLux | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call` |
| TCS IVH4222 / Koch VTCH50/2D | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call` |
| TCS ISW3330 / Koch TC50 BW   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS ISW5010 / Koch TC60      | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS ISW5020                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS ISW5030                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS ISW5031                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS ISW5033                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS IVW511x / Koch VTC60/2D / Scantron VIVO | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS IVW521x | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS ISW6010                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS ISW6031                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS IVW6511                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS ISW7030 / Koch TC70      | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW7510 / Koch VTC70     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS ISH7030 / Koch TCH70     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVH7510 / Koch VTCH70    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS ISWM7000                 | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider` |
| TCS IVWM7000                 | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `volume_handset_internal_call`, `as_address_divider`, `vas_address_divider` |
| TCS ISW4100 / Koch TC31      | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `as_address_divider` |
| TCS IMM2100 / Koch TCE31     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `as_address_divider` |
| TCS IVW2210 / Koch Ecoos     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW2211 / Koch Ecoos     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW2212 / Koch Ecoos / Scantron SLIM60T | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS VTC42V2                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| TCS TC40V2                   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS VTC40                    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| TCS TC40                     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS TC2000                   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_internal_call` |
| TCS TC20P                    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call` |
| TCS TC20F                    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call` |
| TCS IMM1000 / Koch TCH30     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IMM1100 / Koch TCHE30    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IMM1300 / Koch VTCH30    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IMM1310 / Koch VTCHE30   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IMM1110 / Koch TCHEE30   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IMM1500                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| TCS IVW2220 / Koch Sky       | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW2221 / Koch Sky R1.00 | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW3011 / Koch Skyline Plus | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS IVW3012 / Koch Skyline/Aldup | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS VMH / Koch VMH | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| TCS VML / Koch VML | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| TCS VMF / Koch VMF | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| Jung TKIS                    | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider` |
| Jung TKISV                   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `volume_handset_door_call`, `as_address_divider`, `vas_address_divider` |
| TCS CAIXXXX / Koch CAIXXXX   | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider` |
| TCS CAI2000 / Koch Carus     | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider`, `vas_address_divider` |
| TCS ISW42X0                  | `ringtone_floor_call`, `ringtone_entrance_door_call`, `ringtone_second_entrance_door_call`, `ringtone_internal_call`, `volume_ringtone`, `as_address_divider` |
| TCS ISH3022                  | None |
| TCS IVW9010 | None |
| TCS IVW9011 / Koch VTP10 | None |
| TCS IVW9110 | None |
| TCS IVW9030 / Scantron SLIM50T | None |
| TCS IVE70   | None |

### Outdoor Stations
Below is a list of available settings for specific outdoor station models:

| Model     | Available settings |
|-----------|--------------------|
| TCS PAKV2    | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows` |
| TCS PAKV3    | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows` |
| TCS PUK      | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows` |
| TCS PUK-DSP  | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows` |
| TCS PES      | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows` |
| TCS PDS0X    | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows`, `has_code_lock` |
| TCS PDS0X/04 | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness`, `button_rows`, `has_code_lock` |
| TCS TCU2     | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness` |
| TCS TCU3     | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness` |
| TCS TCU4     | `as_address`, `as_address_lock`, `door_opener_duration`, `door_readiness_duration`, `calling_duration`, `talking_requires_door_readiness` |

### Controller
Below is a list of available settings for specific controller models:

| Model                        | Available settings |
|------------------------------|--------------------|
| TCS BVS20                    | None |
| TCS BVS30                    | None |
| TCS VBVS30                   | None |
| TCS NBV3210                  | None |
| TCS NBV2600                  | None |