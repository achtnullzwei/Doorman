# Hardware Component <Badge type="tip" text="hardware" />
The Hardware component is a custom ESPHome text sensor hub designed for boards I manufacture.
During production, each board's PCB hardware revision and product type are permanently written to the ESP32's EFuse memory. This enables the firmware to automatically detect the correct hardware version at runtime without manual configuration.

The component reads these EFuse values and exposes them as text sensors, providing visibility into the physical hardware characteristics of each device.

## How does it work?
Each board has its PCB revision and variant permanently encoded in `EFUSE_BLK3`:

| Byte | Meaning       |
| ---- | ------------- |
| 0    | Major version |
| 1    | Minor version |
| 2    | Patch version |
| 3    | Product type  |

During startup, this component:

1. Reads 32 bits from `EFUSE_BLK3`
2. Parses the hardware revision into major.minor.patch format and extracts the product type
3. Exposes the results as text sensors

Example log output:
```log
[I][hardware.text_sensor:034]: Product: Doorman S3
[I][hardware.text_sensor:035]: Revision: 1.6.0
```

## Example YAML Configuration
This is an example configuration for the component in ESPHome:

```yaml
external_components:
  - source: github://azoninc/doorman@master
    components: [ hardware ]

text_sensor:
  - platform: hardware
    product:
      id: doorman_variant
      name: Product
    revision:
      id: doorman_hardware_version
      name: Hardware Revision
```