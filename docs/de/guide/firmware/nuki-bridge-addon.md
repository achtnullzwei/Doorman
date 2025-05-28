# Nuki Bridge Erweiterung

Diese Erweiterung fügt eine [Nuki Bridge](https://github.com/uriyacovy/ESPHome_nuki_lock)-Komponente über BLE hinzu. Beachte, dass Improv über Bluetooth LE nicht gleichzeitig unterstützt wird, da der BLE-Stack von der Nuki Bridge-Komponente verwendet wird.

::: tip HINWEIS
Mit der offiziellen Nuki-App kannst du dein Smart Lock zwar steuern, jedoch nicht über die emulierte Nuki Bridge dieses Add-ons. Um dein Schloss aus der Ferne über diese Bridge zu steuern, musst du stattdessen die Home Assistant App verwenden.
:::

## Nuki Smart Lock koppeln
::: warning WARNUNG
Wenn dein Smart Lock bereits gekoppelt ist, wird es durch diesen Vorgang entkoppelt!
:::

Drücke je nach PCB-Revision entweder die FLASH- oder PRG-Taste auf der Doorman-S3 Platine für 5 Sekunden, bis die Status-LED lila zu blinken beginnt, oder aktiviere den Nuki Pairing Mode-Schalter in Home Assistant / ESPHome Weboberfläche. Drücke dann die Taste an deinem Smart Lock für 5 Sekunden, bis der Leuchtring aufleuchtet.

Nach erfolgreicher Kopplung bleibt die Status-LED für 3 Sekunden konstant an und schaltet sich dann aus.

## Nuki Smart Lock entkoppeln

Du kannst dein Smart Lock entweder über den `Nuki Unpair Device`-Button in Home Assistant / ESPHome Weboberfläche oder die physische `FLASH`- oder `PRG`-Taste auf der Doorman-S3 Platine entkoppeln.

::: info Physische Taste
Falls dein Smart Lock bereits mit Doorman gekoppelt ist, drücke die `FLASH`- oder `PRG`-Taste auf der Doorman-S3 Platine für 5 Sekunden, bis die RGB-Status-LED lila zu blinken beginnt. Dein Smart Lock wird dann entkoppelt. Beachte, dass der Pairing-Modus nach 30 Sekunden abläuft.
:::