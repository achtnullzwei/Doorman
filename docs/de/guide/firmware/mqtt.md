## MQTT
Nachdem du die Firmware mit MQTT-Integration geflasht und deinen Doorman mit dem WLAN verbunden hast, **blinkt die Status-LED abwechselnd rot und blau**, weil der MQTT-Broker noch nicht eingerichtet ist.

Öffne jetzt einfach die Web-Oberfläche und gehe zum Bereich `MQTT Configuration`.
Dort gibst du deine Broker-Zugangsdaten ein:

![broker-credentials](../images/mqtt-details.png)

Sobald du die Einstellungen speicherst, versucht die Firmware sich mit deinem MQTT-Broker zu verbinden.
Klappt das, leuchtet die Status-LED **für 3 Sekunden durchgehend blau** als Bestätigung.

### Topics
Bei Verwendung der MQTT-Firmware werden verschiedene Topics an deinen Broker gesendet. So funktioniert die Struktur der Topics und Steuerung.

#### Topic Struktur
Jede Entität veröffentlicht ihren Status auf einem Topic im folgenden Format:
```
<TOPIC_PREFIX>/<COMPONENT_TYPE>/<COMPONENT_NAME>/state
```

Du kannst bestimmte Entitäten steuern, indem du einen Befehl an ein Topic mit folgendem Format sendest:
::: code-group
``` [Topic]
<TOPIC_PREFIX>/<COMPONENT_TYPE>/<COMPONENT_NAME>/command 
```
``` [Payload]
ON oder OFF oder was sonst unterstützt wird
```
:::

#### Beispiel
Um die [Ring-To-Open](../automation/ring-to-open.md) Automatisierung zu aktivieren oder zu deaktivieren, sende ON oder OFF als Payload an dieses Topic:
::: code-group
``` [Topic]
doorman-s3/switch/rto__entrance_door/command
```
``` [Payload]
ON
```
:::

### Spezielle Topics
Es gibt spezielle Topics, die erweiterte Befehle ermöglichen.

#### Senden eines Commands (Hexadezimal)
Hier ist ein Beispiel um hexadezimale Commands (uint32) an den Bus zu senden:
::: code-group
``` [Topic]
doorman-s3/send_raw_command
```
```json [Payload]
{
    "command": 0x1C30BA80
}
```
```json [Advanced Payload]
{
    "command": 0x1C30BA80,
    "is_long": false
}
```
:::

#### Senden eines Commands (Command Builder)
Hier ist ein Beispiel um Commands via Command Builder an den Bus zu senden:
::: code-group
``` [Topic]
doorman-s3/send_command
```
```json [Payload]
{
    "type": "open_door",
    "address": 0,
    "payload": 0,
    "serial_number": 123456
}
```
:::