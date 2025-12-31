# MQTT Integration

Diese Seite bietet alles, was du brauchst – von der Konfiguration bis hin zu praktischen Beispielen mit MQTT-Topics und Payloads.

## Einrichtung
<!--@include: ./instructions/mqtt.md-->

## Topics
Bei Verwendung der MQTT-Firmware werden verschiedene Topics an deinen Broker gesendet. So funktioniert die Struktur der Topics und Steuerung.

### Topic Struktur
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

### Beispiel
Um die [Ring-To-Open](../features/ring-to-open.md) Automatisierung zu aktivieren oder zu deaktivieren, sende `ON` oder `OFF` als Payload an dieses Topic:
::: code-group
``` [Topic]
doorman-s3/switch/ring_to_open/command
```
``` [Payload]
ON
```
:::

## Senden eines Bus Telegramms (Hexadezimal)
Hier ist ein Beispiel um hexadezimale Telegramme (16-bit/32-bit) an den Bus zu senden:
::: code-group
``` [Topic]
doorman-s3/send_tc_telegram/raw
```
```text [16-Bit Text Payload]
1100
```
```text [32-Bit Text Payload]
1C30BA80
```
:::

## Senden eines Bus Telegramms (Telegram Builder)
Hier ist ein Beispiel um Telegramme via Telegram Builder an den Bus zu senden:
::: code-group
``` [Topic]
doorman-s3/send_tc_telegram
```
```json [JSON Payload]
{
    "type": "open_door",
    "address": 0,
    "payload": 0,
    "serial_number": 123456
}
```
:::

## Senden eines Innenstation Telegramms (Telegram Builder)
Hier ist ein Beispiel um Innenstation Telegramme via Telegram Builder an den Bus zu senden:
::: code-group
``` [Topic]
doorman-s3/send_tc_is_telegram
```
```json [JSON Payload]
{
    "type": "open_door",
    "address": 0,
    "payload": 0
}
```
:::

## WLAN neu konfigurieren
Hier ist ein Beispiel um die WLAN Zugangsdaten neu zu konfigurieren:
::: code-group
``` [Topic]
doorman-s3/reconfigure_wifi
```
```json [JSON Payload]
{
    "ssid": "Your new WiFi SSID",
    "password": "YourNewPassword!"
}
```
:::

## Empfang von geparsten Telegrammen
Alle Telegramme werden auf einem speziellen Topic veröffentlicht. Die Nachrichten enthalten die geparsten Telegrammdaten.

::: code-group
``` [Topic]
doorman-s3/last_telegram
```
```json [Payload]
{
    "telegram": "1100",
    "type": "open_door",
    "address": "0",
    "payload": "0",
    "serial_number": "0"
}
```
:::