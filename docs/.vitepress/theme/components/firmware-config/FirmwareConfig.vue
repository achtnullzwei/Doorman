<script>
export default {
  props: ['baseUrl', 'variantSelection'],
  data() {
    return {
        platform: '',
        integration: '',
        variant: this.variantSelection,

        platform_options: [
            {
                key: 'esp32-s3',
                name: 'ESP32-S3 (Octal)',
                icon: '',
                iconColor: '',
                details: 'Best choice for the <b>Doorman-S3</b> and any ESP32-S3 board with 8MB (octal) PSRAM.',
            },
            {
                key: 'esp32-s3-quad',
                name: 'ESP32-S3 (Quad)',
                icon: '',
                iconColor: '',
                details: 'For <b>Doorman-S3 rev 1.4</b> and ESP32-S3 boards with 4MB (quad) PSRAM.',
            },
            {
                key: 'esp32',
                name: 'ESP32',
                icon: '',
                iconColor: '',
                details: 'For standard ESP32 boards without PSRAM or with other custom configurations.',
            },
        ],
        integration_options: [
            {
                key: 'ha',
                name: 'Home Assistant',
                icon: '<img src="/home-assistant.svg" />',
                iconColor: '',
                details: 'I use Home Assistant and want the easiest way to integrate Doorman with my smart home.',
            },
            {
                key: 'mqtt',
                name: 'MQTT',
                icon: '<img src="/mqtt-ver-transp.svg" />',
                iconColor: '#606',
                details: 'I want Doorman to connect directly to my MQTT broker, without using Home Assistant or other platforms.',
            },
            {
                key: 'custom',
                name: 'Standalone',
                icon: '⚡',
                iconColor: '',
                details: 'I don\'t use a smart home system — I just want Doorman to work on its own over Wi-Fi.',
            },
        ],

        variant_options: [
        {
            key: 'stock',
            name: 'Standard',
            icon: '',
            iconColor: '',
            details: 'I only need the basic features to control my intercom — nothing extra.',
        },
        {
            key: 'nuki-bridge',
            name: 'Nuki Bridge',
            icon: '',
            iconColor: '',
            details: 'I also want to control my Nuki Smart Lock and use Ring to Open with the apartment doorbell.',
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
        return this.platform != '' && this.integration != '' && this.variant != '';
    },
    notes() {
        if(this.integration == 'mqtt') {
            return 'The MQTT Integration requires further setup! You need to setup your Broker details after flashing the firmware via the web interface.';
        }
        return '';
    }
  },
}
</script>

<template>
    <div class="custom-layout">
        <div>
            <div class="title_row">1️⃣ Which platform do you use?</div>
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
        </div>

        <div v-if="platform != ''">
            <div class="title_row">2️⃣ How should Doorman connect to your setup?</div>
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

        <div v-if="integration != ''">
            <div class="title_row">3️⃣ What about some extras?</div>
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
            <div class="title_row">✨ Let's do this</div>
            <div v-if="notes" class="tip custom-block">
                <p class="custom-block-title">NOTE</p>
                <p>{{ notes }}</p>
            </div>
            <esp-web-install-button :manifest="manifest_file">
                <button slot="activate">
                    <div class="custom-layout">
                        <a class="btn">Install or Update Firmware ({{ fw_string }})</a>
                    </div>
                </button>
                <div slot="unsupported">
                    <div class="danger custom-block">
                        <p class="custom-block-title">OH SNAP!</p>
                        <p>Your Browser doesn't support Web Serial :(</p>
                    </div>
                </div>
                <div slot="not-allowed">
                    <div class="danger custom-block">
                        <p class="custom-block-title">OH SNAP!</p>
                        <p>You are not allowed to use this on HTTP :(</p>
                    </div>
                </div>
            </esp-web-install-button>
        </div>
        
    </div>
</template>
  
<script setup>
  
</script>

<style>
.title_row
{
    margin-top: 20px;
    margin-bottom: 8px;
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