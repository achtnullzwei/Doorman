# Doorman Hardware Component <Badge type="tip" text="doorman_hardware" />
The Doorman Hardware component is a custom ESPHome text sensor designed specifically for Doorman S3 boards shipped by me.
Each board has its PCB hardware revision burned into the ESP32's EFuse memory, allowing firmware to automatically identify the correct hardware version at runtime.

This component reads that EFuse value and publishes it as a text sensor.

## How does it work?
Each shipped Doorman S3 board has its PCB revision encoded in EFuse Block 3 using three bytes:

| Byte | Meaning       |
| ---- | ------------- |
| 0    | Major version |
| 1    | Minor version |
| 2    | Patch version |

During startup, this component:

1. Reads 24 bits from EFUSE_BLK3
2. Parses the values into major.minor.patch
3. Publishes the result as a text sensor value

Example log output:
```log
[I][doorman_hardware.text_sensor:034]: Detected Doorman Hardware: Revision 1.5.0
```

## Example YAML Configuration
This is an example configuration for the component in ESPHome:

```yaml
external_components:
  - source: github://azoninc/doorman@master
    components: [ doorman_hardware ]

text_sensor:
  - platform: doorman_hardware
    id: doorman_hardware_version
    name: Doorman PCB Revision
```