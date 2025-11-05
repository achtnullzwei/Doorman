---
editLink: false
lastUpdated: false
---

<script setup lang="ts">
import type { DefaultTheme } from 'vitepress/theme'
import { VPButton } from 'vitepress/theme'

import dayjs from 'dayjs';
import relativeTime from 'dayjs/plugin/relativeTime';
dayjs.extend(relativeTime);

import 'dayjs/locale/de';
dayjs.locale('de');

import IconFluentEmojiRocket from '~icons/fluent-emoji/rocket'
import IconNotoPackage from '~icons/noto/package'
import IconTwemojiFlagGermany from '~icons/twemoji/flag-germany'
import IconTwemojiFlagSwitzerland from '~icons/twemoji/flag-switzerland';
import IconTwemojiFlagEuropeanUnion from '~icons/twemoji/flag-european-union'
import EmojioneV1GlobeShowingAmericas from '~icons/emojione-v1/globe-showing-americas';
import IconLogosPaypal from '~icons/logos/paypal';
import IconSimpleIconsSepa from '~icons/simple-icons/sepa';
</script>

<script lang="ts">
import axios from 'axios'

const api = axios.create({
   withCredentials: true,
   baseURL: 'https://order.doorman.azon.ai'
})

const allCountries = [
    { value: 'AT', label: 'Österreich' },
    { value: 'BE', label: 'Belgien' },
    { value: 'BG', label: 'Bulgarien' },
    { value: 'HR', label: 'Kroatien' },
    { value: 'CY', label: 'Zypern' },
    { value: 'CZ', label: 'Tschechische Republik' },
    { value: 'DK', label: 'Dänemark' },
    { value: 'EE', label: 'Estland' },
    { value: 'FI', label: 'Finland' },
    { value: 'FR', label: 'Frankreich' },
    { value: 'DE', label: 'Deutschland' },
    { value: 'GR', label: 'Griechenland' },
    { value: 'HU', label: 'Ungarn' },
    { value: 'IE', label: 'Irland' },
    { value: 'IT', label: 'Italien' },
    { value: 'LV', label: 'Lettland' },
    { value: 'LT', label: 'Litauen' },
    { value: 'LU', label: 'Luxemburg' },
    { value: 'MT', label: 'Malta' },
    { value: 'NL', label: 'Niederlande' },
    { value: 'PL', label: 'Polen' },
    { value: 'PT', label: 'Portugal' },
    { value: 'RO', label: 'Rumänien' },
    { value: 'SK', label: 'Slovakei' },
    { value: 'SI', label: 'Slovenien' },
    { value: 'SE', label: 'Schweden' },
    { value: 'ES', label: 'Spanien' },
    { value: 'CH', label: 'Schweiz' },
    { value: 'RS', label: 'Serbien' },
    { value: 'CN', label: 'China' },
];

export default {
    data() {
        const standardTrackingDetails = 'Wähle diese Option, wenn du auf Sendungsverfolgung verzichten kannst. Verlorene Sendungen können nicht erstattet oder ersetzt werden.';
        const trackedTrackingDetails = 'Empfohlen für eine zuverlässige Lieferung mit Sendungsverfolgung. Verlorene Sendungen können untersucht oder reklamiert werden.';

        return {
            errors: {
                fullname: false,
                name: false,
                email: false,
                street: false,
                zip: false,
                city: false,
                country: false,
                model: false,
            },
            user: null,
            status: {},
            orderHash: '',
            processing: false,
            form: {
                fullname: '',
                name: '',
                email: '',
                discord: '',
                street: '',
                zip: '',
                city: '',
                country: 'DE',
                address_extra: '',
                product: 'pcb',
                amount: 1,
                shipping_region: 'DE',
                shipping_method: 'standard',
                payment_option: 'paypal',
                message: '',
                model: ''
            },
            max_items: 4,
            payment_options: [
                {
                    key: 'paypal',
                    name: 'PayPal',
                    icon: IconLogosPaypal,
                    details: 'Bitte wähle bei PayPal <code>Freunde & Familie</code>, um zusätzliche Gebühren zu vermeiden. Wenn du eine andere Option nutzt, musst du die Gebühren selbst tragen.',
                },
                {
                    key: 'sepa',
                    name: 'SEPA-Überweisung',
                    icon: IconSimpleIconsSepa,
                    details: 'Wenn PayPal für dich nicht geeignet ist (z. B. wegen Gebühren), kannst du auch per SEPA-Überweisung bezahlen.',
                },
            ],
            products: [
                {
                    key: 'pcb',
                    name: 'Doorman-S3',
                    image: '/pcb.png',
                    details: 'Nur die Platine – ideal, wenn du sie in eine bestehende Wandbox oder deine Innenstation einbauen kannst.',
                    price: 35
                },
                {
                    key: 'bundle',
                    name: 'Doorman-S3 Bundle',
                    image: '/enclosure.png',
                    details: 'Beinhaltet Platine und Gehäuse – perfekt, wenn du deinen Doorman sichtbar anbringen möchtest.',
                    price: 37.5
                }
            ],
            shipping_regions: [
                {
                    key: 'DE',
                    name: 'Deutschland',
                    details: '',
                    icon: IconTwemojiFlagGermany,
                    options: [
                        {
                            key: 'standard',
                            name: 'Standardversand',
                            icon: '',
                            details: standardTrackingDetails,
                            price: 1.8
                        },
                        {
                            key: 'tracking',
                            name: 'Sendungsverfolgung',
                            icon: '',
                            details: trackedTrackingDetails,
                            price: 6.19
                        }
                    ],
                    countries: [
                        'DE'
                    ],
                    defaultCountry: 'DE'
                },
                {
                    key: 'CH',
                    name: 'Schweiz',
                    details: '',
                    icon: IconTwemojiFlagSwitzerland,
                    options: [
                        {
                            key: 'standard',
                            name: 'Standardversand',
                            icon: '',
                            details: standardTrackingDetails,
                            price: 9
                        },
                        {
                            key: 'tracking',
                            name: 'Sendungsverfolgung',
                            icon: '',
                            details: trackedTrackingDetails,
                            price: 27
                        }
                    ],
                    countries: [
                        'CH'
                    ],
                    defaultCountry: 'CH'
                },
                {
                    key: 'EU',
                    name: 'Europäische Union',
                    details: '',
                    icon: IconTwemojiFlagEuropeanUnion,
                    options: [
                        {
                            key: 'standard',
                            name: 'Standardversand',
                            icon: '',
                            details: standardTrackingDetails,
                            price: 7
                        },
                        {
                            key: 'tracking',
                            name: 'Sendungsverfolgung',
                            icon: '',
                            details: trackedTrackingDetails,
                            price: 14.5
                        }
                    ],
                    countries: [
                        'AT','BE','BG','HR','CY','CZ','DK','EE','FI','FR','GR','HU','IE','IT','LV','LT','LU','MT','NL','PL','PT','RO','SK','SI','SE','ES'
                    ],
                    defaultCountry: 'AT'
                },
                {
                    key: 'INT',
                    name: 'Other',
                    details: '',
                    icon: EmojioneV1GlobeShowingAmericas,
                    options: [
                        {
                            key: 'standard',
                            name: 'Standard Shipping',
                            icon: '',
                            details: standardTrackingDetails,
                            price: 15
                        },
                        {
                            key: 'tracking',
                            name: 'Tracked Shipping',
                            icon: '',
                            details: trackedTrackingDetails,
                            price: 30
                        }
                    ],
                    countries: [
                        'RS','CN'
                    ],
                    defaultCountry: 'RS'
                }
            ],
            step: 1,
            modalOpen: false,
            result_title: '',
            result_text: '',
            available_units: -1,
            available_timestamp: 0
        }
    },
    created() {
        api.get('/status', { 
            withCredentials: true 
        })
        .then(res => {
            this.status = res.data;
        })
        .catch(() => {
            this.status = { status: 'error' };
        });

        api.get('/user', { 
            withCredentials: true 
        })
        .then(response => {
            this.user = response.data;
        })
        .catch(() => {
            this.user = null;
        });

        api.get('/product_data', { 
            withCredentials: true 
        })
        .then(res => {
            this.available_units = res.data.available_units;
            this.available_timestamp = res.data.available_timestamp;
            
            // merge into products
            if (res.data.products) {
                this.products = this.products.map(p => {
                    const override = res.data.products.find(x => { return x.key == p.key });
                    return override ? { ...p, price: override.price } : p;
                });
            }

            if (res.data.shipping_regions) {
                this.shipping_regions = this.shipping_regions.map(r => {
                    const override = res.data.shipping_regions.find(x => x.key === r.key);
                    if (!override) return r;

                    // merge options inside region
                    const mergedOptions = r.options.map(opt => {
                        const optOverride = override.options?.find(o => o.key === opt.key);
                        return optOverride ? { ...opt, ...optOverride } : opt;
                    });

                    return { ...r, ...override, options: mergedOptions };
                });
            }
        })
        .catch(() => {
            
        })
    },
    watch: {
        'form.shipping_region'(new_value) {
            const destination = this.shipping_regions.find(d => d.key === new_value);
            if (!destination) return [];
            this.form.country = destination.defaultCountry;
        },
        'form.fullname'(val) {
            this.errors.fullname = !val;
        },
        'form.name'(val) {
            this.errors.name = !val;
        },
        'form.email'(val) {
            // Simple email regex validation
            const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
            this.errors.email = !emailRegex.test(val);
        },
        'form.street'(val) {
            this.errors.street = !val;
        },
        'form.zip'(val) {
            this.errors.zip = !val;
        },
        'form.city'(val) {
            this.errors.city = !val;
        },
        'form.country'(val) {
            this.errors.country = !val;
        },
        'form.model'(val) {
            this.errors.model = !val;
        }
    },
    computed: {
        availability_class() {
            if(this.available_units > 5) {
                return 'tip';
            } else if(this.available_units > 0) {
                return 'warning';
            }
            return 'danger';
        },
        availability_text() {
            if(this.available_units > 0) {
                return this.available_units + ' verfügbar';
            }
            return 'Derzeit nicht verfügbar';
        },
        availability_time_text() {
            const now = dayjs().unix();
            if(this.available_timestamp > now) {
                return 'Kein Grund zur Sorge - neue Doormans sind unterwegs und werden ungefähr <b>' + dayjs.unix(this.available_timestamp).fromNow() + '</b> verfügbar sein.';
            } else if(this.available_timestamp < now) {
                if(this.available_units == 0) {
                    return 'Es scheint derzeit eine Verzögerung zu geben – die neuen Doormans hätten bereits <b>' + dayjs.unix(this.available_timestamp).fromNow() + '</b> verfügbar sein sollen.';
                }
            }
            return '';
        },
        available_shipping_options() {
            return this.shipping_regions.find(dest => dest.key === this.form.shipping_region)?.options || [];
        },
        countryOptions() {
            const destination = this.shipping_regions.find(d => d.key === this.form.shipping_region);
            if (!destination) return [];
            return allCountries.filter(c => destination.countries.includes(c.value));
        },
        total_price() {
            const product = this.products.find(p => p.key === this.form.product);
            const destination = this.shipping_regions.find(d => d.key === this.form.shipping_region);
            const shipping = destination?.options.find(o => o.key === this.form.shipping_method);

            // Calculate total
            const productPrice = (product ? product.price : 0) * this.form.amount;
            const shippingPrice = shipping ? shipping.price : 0;

            return (productPrice + shippingPrice);
        },
        last_update() {
            return dayjs.unix(this.status.updated_timestamp).fromNow();
        }
    },
    methods: {
        openManagement() {
            window.location = '/order-management';
        },
        showModal(title, text) {
            this.modalOpen = true;
            this.result_title = title;
            this.result_text = text;
        },
        validate() {
            const { fullname, name, email, street, zip, city, country, model } = this.form;

            // Reset errors
            for (const key in this.errors) this.errors[key] = false;

            let valid = true;

            if (!fullname) { this.errors.fullname = true; valid = false; }
            if (!name) { this.errors.name = true; valid = false; }
            if (!email) { this.errors.email = true; valid = false; }

            if (!street) { this.errors.street = true; valid = false; }
            if (street.split(' ').length < 2) { this.errors.street = true; valid = false; }
            if (/\d/.test(street) == false) { this.errors.street = true; valid = false; }

            if (!zip) { this.errors.zip = true; valid = false; }
            if (!city) { this.errors.city = true; valid = false; }
            if (!country) { this.errors.country = true; valid = false; }
            if (!model) { this.errors.model = true; valid = false; }

            return valid;
        },
        nextStep() {
            if (this.validate()) {
                this.step++;
            } else {
                this.showModal("Entschuldigung!", "Bitte fülle alle erforderlichen Felder aus, um fortzufahren.");
            }
        },
        previousStep() {
            this.step--;
        },
        async submit() {
            this.processing = true;

            api.post('/submit', this.form, { 
                withCredentials: true 
            })
            .then(response => {
                this.processing = false;
                this.showModal("Erhalten!", "Vielen Dank. Ich melde mich so schnell wie möglich bei dir.");
                this.status = response.data;
            })
            .catch(error => {
                this.processing = false;
                this.showModal("Entschuldigung!", this.getErrorMessage(error, 'Etwas ist schiefgelaufen. Bitte versuche es später erneut.'));
            });
        },
        async closeOrder() {
            if (!confirm('Bist du dir sicher dass du die Bestellung abschließen möchtest?')) return;
            api.post('/close', {}, { 
                withCredentials: true 
            })
            .then(response => {
                this.status = response.data;
            })
            .catch(error => {
                this.showModal("Entschuldigung!", this.getErrorMessage(error, 'Etwas ist schiefgelaufen. Bitte versuche es später erneut.'));
            });
        },
        async updateOrderStatus() {
            if(!this.status) {
                alert('Die Bestelldetails stehen nicht zur Verfügung!');
                return;
            }

            if (!confirm('Bist du dir sicher dass du den Bestellstatus aktualisieren möchtest?')) return;

            if(this.status.shipping_method == 'tracking' && this.status.status == 'pending_shipment') {
                const trackingUrl = prompt('Trackingdaten angeben:');
                if (!trackingUrl) {
                    alert('Keine Trackingdaten angegeben! Wird abgebrochen!');
                    return;
                }
                this.setNextStep(this.status.id, trackingUrl);
            } else {
                this.setNextStep(this.status.id, '');
            }
        },
        async setNextStep(id, tracking) {
            api.post('/orders/' + id + '/next', { tracking }, { 
                withCredentials: true 
            })
            .then(response => {
                this.showModal("Aktualisiert!", "Der Bestellstatus wurde erfolgreich aktualisiert!");
                this.refreshOrderStatus();
            })
            .catch(error => {
                this.showModal("Entschuldigung!", this.getErrorMessage(error, 'Der Bestellstatus konnte nicht aktualisiert werden!'));
            });
        },
        async resetOrder() {
            api.post('/reset', {}, { 
                withCredentials: true 
            })
            .then(response => {
                this.status = response.data;
            })
            .catch(error => {
                this.showModal("Entschuldigung!", this.getErrorMessage(error, 'Etwas ist schiefgelaufen. Bitte versuche es später erneut.'));
            });
        },
        async refreshOrderStatus() {
            try {
                const { data: statusData } = await api.get('/status', { withCredentials: true });
                this.status = statusData;
            } catch (error) {

            }
        },
        async checkOrder() {
            if (!this.orderHash) return;

            try {
                await api.get(`/order/${this.orderHash}/details`, { withCredentials: true });

                const { data: statusData } = await api.get('/status', { withCredentials: true });
                this.status = statusData;

                if (this.status.status === 'none') {
                    this.showModal("Entschuldigung!", "Diese Bestellung existiert nicht! Bitte überprüfe die Bestellnummer.");
                }
            } catch (error) {
                this.showModal("Entschuldigung!", this.getErrorMessage(error, 'Etwas ist schiefgelaufen. Bitte versuche es später erneut.'));
            }
        },
        getErrorMessage(error, defaultMessage = "Etwas ist schiefgelaufen. Bitte versuche es später erneut.") {
            if (!error) return defaultMessage;

            if (error.response) {
                return error.response.data?.error || error.response.data?.message || `Server error (${error.response.status})`;
            } else if (error.request) {
                return "No response from server. Please check your connection.";
            } else {
                return error.message || defaultMessage;
            }
        },
    }
}
</script>

<style scoped>
form {
    width: 100%
}

.form-element {
    margin-bottom: 1rem
}

.top {
    display: inline-block;
    width: 48%
}

.top label {
    display: block
}

.email-container {
    float: right
}

textarea,input[type=text],input[type=email],select {
    background-color: var(--vp-c-bg);
    width: 100%;
    font-size: 14px;
    border-radius: 8px;
    border: 1px solid var(--vp-c-divider);
    padding: 10px 12px;
    transition: color .25s,border-color .25s,background-color .25s;
    height: 40px;
    font-family: inherit;
    margin-top: .5rem
}

textarea:hover,input[type=text]:hover,input[type=email]:hover,select:hover {
    border: 1px solid var(--vp-button-brand-active-bg);
    transition: color .1s,border-color .1s,background-color .1s
}

textarea {
    height: 100px
}

.link-wrap {
    display: grid;
    grid-template-columns: 1fr 1fr
}

.submit {
    align-items: center;
    justify-content: center;
    display: flex;
    gap: 10px;
}

.estimate-list {
    list-style: none;
    padding: 0;
    margin: 1rem 0 1.5rem 0;
}
.estimate-list li {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: .5rem;
}

.invalid {
  border: 1px solid var(--vp-c-red-2) !important;
}

.amount-control {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  margin-top: 10px;
  padding-top: 15px;
  border-top: solid 1px var(--vp-c-default-soft);
}

.amount-control button {
    height: 25px !important;
    line-height: 25px !important;
}

.amount-control span {
  width: 64px;
  text-align: center;
  font-weight: 600;
  font-size: 14px;
}
</style>


# Hol dir deinen Doorman <Badge v-if="status.status !== 'error' && available_units >= 0" :type="availability_class" :text="availability_text" @click="openManagement" />

Du suchst eine einfache Lösung? Ich hab einen einsatzbereiten Doorman-S3 für dich, schon mit der [Doorman-Firmware](guide/firmware/installation) geflasht. Den kannst du direkt in dein Home Assistant Setup einbinden – ganz ohne Stress.

:::tip BITTE BEACHTE
Dies ist **kein offizielles oder zertifiziertes Produkt**. Es basiert auf Reverse Engineering und wird **ohne Garantie auf Kompatibilität, Sicherheit oder Funktionalität** bereitgestellt.
:::

Um eine Anfrage zu stellen, fülle einfach das untenstehende Formular aus. **Dies ist kein Kauf oder Checkout** — nur eine unverbindliche Anfrage, damit ich das Interesse einschätzen und Bestellungen privat koordinieren kann.

Sobald ich deine Nachricht erhalte, melde ich mich schnellstmöglich bei dir. Die Status Updates sind automatisiert, **bitte prüfe auch deinen Spam Ordner.**

<div v-if="status.status == 'none' && available_units === 0" class="warning custom-block">
    <p class="custom-block-title">ACHTUNG</p>
    <p>Alle Doormans sind gerade ausverkauft. Du kannst deine Anfrage trotzdem schonmal senden, dann reserviere ich dir einen.</p>
    <p v-html="availability_time_text"></p>
</div>

<hr />

<ContactModal :show="modalOpen" @close="modalOpen = false">
    <template #header>
        <h3>{{ result_title }}</h3>
    </template>
    <template #body>
        {{ result_text }}
    </template>
</ContactModal>

<div v-if="status.status == 'error'" class="danger custom-block">
    <p class="custom-block-title">SORRY</p>
    <p>Das Anfrageformular ist derzeit nicht verfügbar. Wahrscheinlich arbeite ich bereits an einer Lösung – bitte versuche es später erneut oder kontaktiere mich direkt in der Zwischenzeit.</p>
</div>
<div v-else-if="status.status == 'pending_review'" class="warning custom-block">
    <p class="custom-block-title">ÜBERPRÜFUNG AUSSTEHEND</p>
    <p>Ich habe deine Bestellung erhalten und werde diese mit dir prüfen, um die Kompatibilität abzuklären. Du wirst benachrichtigt, sobald die Überprüfung abgeschlossen ist.</p>
</div>
<div v-else-if="status.status == 'pending_payment'" class="warning custom-block">
    <p class="custom-block-title">ZAHLUNG AUSSTEHEND</p>
    <p>Deine Bestellung ist bereit für den nächsten Schritt. Bitte führe die Zahlung gemäß den bereitgestellten Anweisungen durch, damit ich mit der Bearbeitung des Versands fortfahren kann.</p>
</div>
<div v-else-if="status.status == 'pending_shipment'" class="tip custom-block">
    <p class="custom-block-title">VERSAND WIRD VORBEREITET</p>
    <p>
        Deine Bestellung wird für den Versand vorbereitet und wird bald versendet. Sobald sie verschickt wurde, erhälst du ein Update mit den Versanddetails. Vielen Dank für deine Geduld!
    </p>
    <p v-if="user">
        <VPButton text="Mark as shipped" @click="updateOrderStatus" />
    </p>
</div>
<div v-else-if="status.status == 'shipped'" class="tip custom-block">
    <p class="custom-block-title">BESTELLUNG VERSCHICKT</p>
    <p>
        Gute Nachrichten! Deine Bestellung wurde {{last_update}} versandt und sollte normalerweise innerhalb einer Woche eintreffen.
        <br>
        Bitte beachte, dass der Zoll gelegentlich zu leichten Verzögerungen führen kann.
        <br><br>Sobald die Lieferung erfolgt ist, informiere mich bitte hier. Vielen Dank!
    </p>
    <p v-if="user">
        <VPButton text="Bestellung abschließen" @click="closeOrder" />
    </p>
    <p v-else>
        <VPButton text="Ich habe meinen Doorman erhalten" @click="closeOrder" />
    </p>
</div>
<div v-else-if="status.status == 'closed'" class="tip custom-block">
    <p class="custom-block-title">DANKE</p>
    <p>
        Deine Bestellung wurde erfolgreich abgeschlossen. Ich hoffe, du hast viel Freude mit deinem Doorman 😊<br>
    </p>
    <p>
        <b>Schau dir doch gleich Mal den <a href="/de/guide/getting-started">Quickstart Guide</a> an.</b>
    </p>
    <p>
        Wenn du eine neue Bestellung aufgeben möchten, klicke bitte auf den Button unten.
    </p>
    <p>
        <VPButton text="Neue Bestellung" @click="resetOrder" />
    </p>
</div>
<div v-else-if="status.status == 'cancelled'" class="danger custom-block">
    <p class="custom-block-title">BESTELLUNG STORNIERT</p>
    <p>
        Deine Bestellung wurde storniert!<br>
        Wenn du eine neue Bestellung aufgeben möchten, klicke bitte auf den Button unten.
    </p>
    <p>
        <VPButton text="Neue Bestellung" @click="resetOrder" />
    </p>
</div>
<form v-else-if="status.status == 'none'" @submit.prevent="submit">
    <div v-if="step == 1">
        <h5 class="firmware_title_row">Schon bestellt?</h5>
        <div style="display: flex; gap: 15px;justify-content: space-between; align-items: center;">
            <input type="text" name="order_hash" id="order_hash" maxlength="8" v-model="orderHash" placeholder="Bestellnummer" style="text-transform: uppercase; margin: 0px;" />
            <VPButton type="button" text="Status prüfen" :disabled="orderHash.length != 8" @click="checkOrder" />
        </div>
        <br>
        <hr />
        <h5 class="firmware_title_row">Wähle dein Doorman-Paket</h5>
        <div class="firmware_option_row" :class="{ half: products.length <= 2 }">
            <label class="firmware_option" v-for="product in products" :key="product.key">
                <input type="radio" class="reset_default" v-model="form.product" :value="product.key">
                <span class="checkmark">
                    <div class="image" v-if="product.image">
                        <img :src="product.image" />
                    </div>
                    <div class="title">{{ product.name }} <Badge type="tip">{{ product.price.toFixed(2) }} €</Badge></div>
                    <div class="details" v-html="product.details"></div>
                    <div class="amount-control" v-if="form.product == product.key">
                        <VPButton theme="alt" type="button" text="-" @click="form.amount = Math.max(1, form.amount - 1)" />
                        <span class="font-semibold w-8 text-center">{{ form.amount }} {{ form.amount === 1 ? 'Stück' : 'Stücke' }}</span>
                        <VPButton theme="alt" type="button" text="+" @click="form.amount = Math.min(max_items, form.amount + 1)" />
                    </div>
                </span>
            </label>
        </div>
        <h5 class="firmware_title_row">Erzähl mir Etwas über deine Anlage</h5>
        <div class="form-element">
            <label for="model">Modell der Innenstation</label>
            <input type="text" name="model" id="model" placeholder="z.B. TCS ISH3030" maxlength="30" v-model="form.model" :class="{ 'invalid': errors.model }" required />
        </div>
        <h5 class="firmware_title_row">Wohin soll's geschickt werden?</h5>
        <div class="firmware_option_row" :class="{ half: shipping_regions.length <= 2 }">
            <label class="firmware_option" v-for="destination in shipping_regions" :key="destination.key">
                <input type="radio" class="reset_default" v-model="form.shipping_region" :value="destination.key">
                <span class="checkmark">
                    <div class="icon" v-if="destination.icon"><component :is="destination.icon" /></div>
                    <div class="title">{{ destination.name }}</div>
                    <div class="details" v-html="destination.details"></div>
                </span>
            </label>
        </div>
        <h5 class="firmware_title_row">Wie soll's verschickt werden?</h5>
        <div class="firmware_option_row" :class="{ half: available_shipping_options.length <= 2 }">
            <label class="firmware_option" v-for="option in available_shipping_options" :key="option.key">
                <input type="radio" class="reset_default" v-model="form.shipping_method" :value="option.key">
                <span class="checkmark">
                    <div class="icon" v-if="option.icon"><component :is="option.icon" /></div>
                    <div class="title">{{ option.name }} <Badge type="tip">{{ option.price.toFixed(2) }} €</Badge></div>
                    <div class="details" v-html="option.details"></div>
                </span>
            </label>
        </div>
        <h5 class="firmware_title_row">Deine Lieferadresse</h5>
        <div class="form-element">   
            <label for="name">Name</label>
            <input type="text" name="name" id="name" placeholder="Max Mustermann" v-model="form.name" :class="{ 'invalid': errors.name }" required />
        </div>
        <div class="form-element">
            <label for="address_extra">Zusätzliche Adressinfos <Badge type="info">Optional</Badge></label>
            <input type="text" name="address_extra" id="address_extra" placeholder="" v-model="form.address_extra" />
        </div>
        <div class="form-element">
            <label for="street">Straße & Hausnummer</label>
            <input type="text" name="street" id="street" placeholder="Musterstraße 1" v-model="form.street" :class="{ 'invalid': errors.street }" required />
        </div>
        <div class="form-element top name-container">
            <label for="zip">Postleitzahl</label>
            <input type="text" name="zip" id="zip" placeholder="97080" v-model="form.zip" :class="{ 'invalid': errors.zip }" required />
        </div>
        <div class="form-element top email-container">
            <label for="city">Ort / Stadt</label>
            <input type="text" name="city" id="city" placeholder="Würzburg" v-model="form.city" :class="{ 'invalid': errors.city }" required />
        </div>
        <div class="form-element">
            <label for="city">Land</label>
            <select name="country" class="form-control" id="country" v-model="form.country" required>
                <option v-for="country in countryOptions" :key="country.value" :value="country.value" :label="country.label">{{ country.label }}</option>
            </select>
        </div>
        <h5 class="firmware_title_row">Wie kann ich dich erreichen?</h5>
        <div class="form-element">
            <label for="fullname">Name</label>
            <input type="text" name="fullname" id="fullname" placeholder="Max" v-model="form.fullname" :class="{ 'invalid': errors.fullname }" required />
        </div>
        <div class="form-element top name-container">
            <label for="email">E-Mail</label>
            <input type="email" name="email" id="email" placeholder="max@mustermann.net" v-model="form.email" :class="{ 'invalid': errors.email }" required />
        </div>
        <div class="form-element top email-container">
            <label for="discord">Discord-Tag <Badge type="info">Optional</Badge></label>
            <input type="text" name="discord" id="discord" placeholder="azonflo" v-model="form.discord" />
        </div>
        <h5 class="firmware_title_row">Noch was, das du mir sagen willst?</h5>
        <div class="form-element">
            <label for="discord">Notizen <Badge type="info">Optional</Badge></label>
            <textarea id="message" name="message" v-model="form.message" placeholder="Besondere Anforderungen oder sonstige Fragen?"></textarea>
        </div>
        <div class="submit">
            <VPButton type="button" text="Weiter" @click="nextStep" />
        </div>
    </div>
    <div v-else-if="step == 2">
        <h5 class="firmware_title_row">Wie willst du bezahlen?</h5>
        <div class="firmware_option_row" :class="{ half: payment_options.length <= 2 }">
            <label class="firmware_option" v-for="option in payment_options" :key="option.key">
                <input type="radio" class="reset_default" v-model="form.payment_option" :value="option.key">
                <span class="checkmark">
                    <div class="icon" v-if="option.icon"><component :is="option.icon" /></div>
                    <div class="title">{{ option.name }}</div>
                    <div class="details" v-html="option.details"></div>
                </span>
            </label>
        </div>
        <h5 class="firmware_title_row">Bestellübersicht</h5>
        <ul class="estimate-list">
            <li>
                <span>
                    {{ form.amount }} &times; {{
                        (products.find(p => p.key === form.product)?.name || '—')
                    }}
                </span>
                <span>
                    <Badge type="tip">
                        {{
                            ((products.find(p => p.key === form.product)?.price || 0) * form.amount).toFixed(2)
                        }} €
                    </Badge>
                </span>
            </li>
            <li>
                <span>
                    1 &times; {{
                        (available_shipping_options.find(o => o.key === form.shipping_method)?.name || '—')
                    }} ({{
                        (shipping_regions.find(o => o.key === form.shipping_region)?.name || '—')
                    }})
                </span>
                <span>
                    <Badge type="tip">
                        {{
                            available_shipping_options.find(o => o.key === form.shipping_method)?.price?.toFixed(2)
                        }} €
                    </Badge>
                </span>
            </li>
            <hr>
            <li>
                <span><strong>Voraussichtlicher Gesamtpreis: </strong></span>
                <span>
                    <Badge type="warning">{{ total_price.toFixed(2) }} €</Badge>
                </span>
            </li>
        </ul>
        <div class="warning custom-block" style="padding-top: 8px">
            Das ist nur ein unverbindlicher Kostenvoranschlag für das ausgewählte Produkt und Versand. Nach Absenden deiner Anfrage bekommst du eine Bestätigung und Zahlungsinfos per E-Mail.
        </div>
        <div class="submit">
            <VPButton type="button" text="Zurück" @click="previousStep" />
            <VPButton type="submit" text="Anfrage absenden" :disabled="processing" />
        </div>
    </div>
</form>