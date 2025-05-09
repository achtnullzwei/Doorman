<script>
export default {
  data() {
    return {
        baseUrl: '../../../firmware/release/',
        platform: '',
        integration: '',
        variant: 'nuki-bridge',
        platform_options: [
            {
                key: 'esp32-s3',
                name: 'ESP32-S3 (Octal)',
                icon: '',
                iconColor: '',
                details: 'Empfohlen für den <b>Doorman-S3</b> und alle ESP32-S3-Boards mit 8MB (Octal) PSRAM.',
            },
            {
                key: 'esp32-s3-quad',
                name: 'ESP32-S3 (Quad)',
                icon: '',
                iconColor: '',
                details: 'Für <b>Doorman-S3 rev 1.4</b> und ESP32-S3-Boards mit 4MB (Quad) PSRAM.',
            },
            {
                key: 'esp32',
                name: 'ESP32',
                icon: '',
                iconColor: '',
                details: 'Für klassische ESP32-Boards ohne PSRAM oder mit benutzerdefinierten Konfigurationen.',
            },
        ],
        integration_options: [
            {
                key: 'ha',
                name: 'Home Assistant',
                icon: '<img src="/home-assistant.svg" />',
                iconColor: '',
                details: 'Ich nutze Home Assistant und will Doorman möglichst einfach in mein Smart Home einbinden.',
            },
            {
                key: 'mqtt',
                name: 'MQTT',
                icon: '<img src="/mqtt-ver-transp.svg" />',
                iconColor: '#606',
                details: 'Ich möchte Doorman direkt mit meinem MQTT-Broker verbinden und auf anderen Plattformen nutzen.',
            },
            {
                key: 'custom',
                name: 'Standalone',
                icon: '✨',
                iconColor: '',
                details: 'Ich nutze kein Smart Home – Doorman soll einfach eigenständig über WLAN funktionieren.',
            },
        ],
        variant_options: [
            {
                key: 'stock',
                name: 'Standard',
                icon: '',
                iconColor: '',
                details: 'Ich brauche nur die Grundfunktionen, um meine Gegensprechanlage zu steuern – mehr nicht.',
            },
            {
                key: 'nuki-bridge',
                name: 'Nuki Bridge',
                icon: '',
                iconColor: '',
                details: 'Ich will zusätzlich mein Nuki Smart Lock steuern und Ring To Open auch über die Wohnungsklingel nutzen.',
            },
        ],
    }
  },
  computed: {
    fw_string() {
      return [this.platform, this.integration, this.variant].join('.');
    },
    manifest_file() {
      return this.baseUrl + this.fw_string + '/manifest.json';
    },
    valid_manifest() {
        return this.platform && this.integration && this.variant;
    },
    notes() {
        if(this.integration == 'mqtt') {
            return 'Die MQTT-Integration benötigt nach dem installieren noch etwas Liebe! Schau dir dazu bitte den Abschnitt <code>MQTT</code> weiter unten an.';
        }
        return '';
    },
    web_serial_available() {
        return navigator.serial != undefined;
    }
  },
}
</script>

# Nuki Bridge Firmware <Badge type="tip" text="ESP-IDF Framework" />

Diese Konfiguration übernimmt alle Funktionen der Stock Firmware und fügt eine [Nuki Bridge](https://github.com/uriyacovy/ESPHome_nuki_lock)-Komponente über BLE hinzu. Beachte, dass Improv über Bluetooth LE nicht unterstützt wird, da der BLE-Stack von der Nuki Bridge-Komponente verwendet wird.

## Firmware installieren oder aktualisieren

Es gibt verschiedene Möglichkeiten, die Firmware zu aktualisieren.

::: warning WARNUNG
Manche **Updates können Breaking Changes mit sich bringen** — lies dir die Changelogs immer genau durch, damit dein System auch in wichtigen Situationen zuverlässig funktioniert.  
:::

#### Web Serial <Badge type="tip" text="Neueste Release-Version, keine Anpassungen" />
Verbinde deinen **Doorman-S3** per USB-C und nutze dann den Konfigurationsassistent unten, um die neueste Firmware direkt über Web Serial zu installieren oder zu aktualisieren. So kannst du das Gerät nahtlos mit der Home Assistant API nutzen und zukünftig einfach Community-Updates einspielen.

<div v-if="web_serial_available">
    <h5 class="title_row"><i class="mdi mdi-hammer-wrench"></i> Welche Hardware nutzt du?</h5>
    <div class="firmware_option_row">
        <label class="firmware_option" v-for="fw_platform in platform_options" :key="fw_platform.key">
            <input type="radio" class="reset_default" v-model="platform" :value="fw_platform.key">
            <span class="checkmark">
                <div class="icon" v-if="fw_platform.icon" v-html="fw_platform.icon"></div>
                <div class="title" v-html="fw_platform.name"></div>
                <div class="details" v-html="fw_platform.details"></div>
            </span>
        </label>
    </div>
    <div v-if="platform != ''">
        <h5 class="title_row"><i class="mdi mdi-home-automation"></i> Nutzt du irgendein Smart Home System?</h5>
        <div class="firmware_option_row">
            <label class="firmware_option" v-for="fw_integration in integration_options" :key="fw_integration.key">
                <input type="radio" class="reset_default" v-model="integration" :value="fw_integration.key">
                <span class="checkmark">
                    <div class="icon" :style="{ 'background-color': fw_integration.iconColor }" v-if="fw_integration.icon" v-html="fw_integration.icon"></div>
                    <div class="title" v-html="fw_integration.name"></div>
                    <div class="details" v-html="fw_integration.details"></div>
                </span>
            </label>
        </div>
    </div>
    <div v-if="notes" class="important custom-block">
        <p class="custom-block-title">WARTE KURZ</p>
        <p v-html="notes"></p>
    </div>
    <div v-if="integration != ''">
        <h5 class="title_row"><i class="mdi mdi-package-variant-plus"></i> Darf's vielleicht etwas mehr sein?</h5>
        <div class="firmware_option_row">
            <label class="firmware_option" v-for="fw_variant in variant_options" :key="fw_variant.key">
                <input type="radio" class="reset_default" v-model="variant" :value="fw_variant.key">
                <span class="checkmark">
                    <div class="icon" v-if="fw_variant.icon" v-html="fw_variant.icon"></div>
                    <div class="title" v-html="fw_variant.name"></div>
                    <div class="details" v-html="fw_variant.details"></div>
                </span>
            </label>
        </div>
    </div>
    <div v-if="valid_manifest">
        <h5 class="title_row"><i class="mdi mdi-auto-fix"></i> Jetzt wird geflasht – und zwar mit Stil!</h5>
        <esp-web-install-button :manifest="manifest_file">
            <button slot="activate">
                <div class="custom-layout">
                    <a class="btn">Firmware installieren oder updaten</a>
                </div>
            </button>
            <div slot="unsupported">
                <div class="danger custom-block">
                    <p class="custom-block-title">OH NEIN!</p>
                    <p>Dein Browser unterstützt leider kein Web Serial :(</p>
                </div>
            </div>
            <div slot="not-allowed">
                <div class="danger custom-block">
                    <p class="custom-block-title">OH NEIN!</p>
                    <p>Das funktioniert mit HTTP nicht :(</p>
                </div>
            </div>
        </esp-web-install-button>
    </div>
</div>
<div v-else class="danger custom-block">
    <p class="custom-block-title">COMPUTER SAGT NEIN</p>
    <p>
        Da dein Browser kein Web Serial unterstützt, ist der Konfigurations Assistent leider nicht verfügbar! Bitte verwende einen Chromium-basierten Browser.
    </p>
</div>

#### HTTP OTA-Updates <Badge type="tip" text="Neueste Release-Version, keine Anpassungen" />  
Wenn du Doorman in Home Assistant hinzugefügt hast, kannst du von automatisierten Firmware-Builds und einfachen Updates profitieren. Das Update erscheint direkt in deiner Update-Liste – ein Klick auf den Update-Button genügt, und das OTA-Update startet sofort. Einfach, effizient und stressfrei.

#### ESPHome CLI / Dashboard <Badge type="warning" text="Vollständige Anpassung" />  
Für maximale Kontrolle und individuelle Anpassungen kannst du die Firmware in dein ESPHome Dashboard übernehmen oder das ESPHome CLI nutzen, indem du folgenden Befehl ausführst:  
`esphome run <yamlfile.yaml>`  

Falls du lieber die MQTT-Only-Firmware verwenden möchtest, nutze einfach das passende MQTT-Beispiel-YAML für dein Board unten.

##### Board Konfigurationsoptionen:

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
#### GPIO Konfiguration
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
#### GPIO Konfiguration
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

<!--@include: ./additions.md-->

<!--@include: ./mqtt.md-->

<style>
.title_row
{
    margin-top: 20px;
    margin-bottom: 20px;
}

.firmware_option_row
{
    display: flex;
    flex-wrap: wrap;
    margin-bottom: 8px;
    gap: 10px;
}

.firmware_option_row .firmware_option
{
    flex-grow: 1;
    flex-basis: 0;
}

.firmware_option_row .firmware_option input
{
    position: absolute;
    opacity: 0;
    cursor: pointer;
}

.firmware_option_row .firmware_option:hover input ~ .checkmark
{
    box-shadow: 0px 0px 0px 2px var(--vp-button-brand-bg) inset;
    color: var(--vp-c-text-1);
}

.firmware_option_row .firmware_option input:checked ~ .checkmark
{
    font-weight: 500;
    box-shadow: inset 0px 0px 0px 2px var(--vp-button-brand-bg);
    color: var(--vp-c-text-1);
}

.firmware_option_row .firmware_option .checkmark
{
    display: flex;
    flex-direction: column;
    padding: 24px;
    border: 1px solid var(--vp-c-bg-soft);
    border-radius: 12px;
    height: 100%;
    background-color: var(--vp-c-bg-soft);
    transition: border-color 0.25s, background-color 0.25s;
    cursor: pointer;
    color: var(--vp-c-text-2);
    min-width: 210px;
}

.firmware_option_row .firmware_option .checkmark .icon
{
    display: flex;
    justify-content: center;
    align-items: center;
    margin-bottom: 20px;
    border-radius: 6px;
    background-color: var(--vp-c-default-soft);
    width: 48px;
    height: 48px;
    font-size: 24px;
    transition: background-color 0.25s;
    padding: 10px;
}

.firmware_option_row .firmware_option .checkmark .title
{
    line-height: 24px;
    font-size: 16px;
    font-weight: 600;
}

.firmware_option_row .firmware_option .checkmark .details
{
    flex-grow: 1;
    padding-top: 8px;
    line-height: 24px;
    font-size: 14px;
    font-weight: 500;
    color: var(--vp-c-text-2);
}
</style>