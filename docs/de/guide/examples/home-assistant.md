# Home Assistant Beispiele

Entdecke praktische Beispiele zur Integration und Automatisierung von Doorman mit Home Assistant, von einfachen Setups bis hin zu fortgeschrittenen Konfigurationen.

## Bus Commands senden
Mit Home Assistant kannst du Aktionen nutzen, um Commands über den Bus zu senden.
Benutze entweder `command` für hexadezimale Befehle oder `type`, `address`, `payload` und `serial_number` um Befehle über den Command Builder zu senden.

::: tip
Denk an das führende `0x` beim Senden eines Befehls mit der `command` Eigenschaft. Wenn du es weglässt, musst du den HEX-Befehl zuerst in eine Dezimalzahl umwandeln.
:::

::: code-group
```yaml [Command Builder]
service: esphome.doorman_s3_send_tc_command
data:
  type: open_door
  address: 0
  payload: 0
  serial_number: 0
```
```yaml [HEX Commands]
service: esphome.doorman_s3_send_tc_command_raw
data:
  command: 0x3a001100
```
:::

## Auf ESPHome-Ereignisse hören
Der Doorman sendet `esphome.doorman`-Ereignisse an Home Assistant, jedes Mal wenn ein Befehl empfangen wird.

Jedes Ereignis sieht so aus:
```yaml
event_type: esphome.doorman
data:
  device_id: 373c62d6788cf81d322763235513310e
  command: "00001100"
  type: "open_door"
  address: "0"
  payload: "0"
  serial_number: "0"
origin: LOCAL
time_fired: "2024-08-12T12:34:13.718317+00:00"
context:
  id: 01J5399Y2PP2XS2VRYKBT3H3AV
  parent_id: null
  user_id: null
```

Beispiel für eine Automation:
::: code-group
```yaml [Command Builder]
alias: Bei Doorman TC Türöffnungsbefehl auslösen
description: ""
trigger:
  - platform: event
    event_type: esphome.doorman
    event_data:
      type: "open_door"
condition: []
action: []
mode: single
```
```yaml [HEX Commands]
alias: Bei Doorman TC Türöffnungsbefehl auslösen
description: ""
trigger:
  - platform: event
    event_type: esphome.doorman
    event_data:
      command: "00001100"
condition: []
action: []
mode: single
```
:::