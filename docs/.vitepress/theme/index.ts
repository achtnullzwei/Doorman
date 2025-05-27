import DefaultTheme from "vitepress/theme";
import Layout from './Layout.vue'

import { enhanceAppWithTabs } from 'vitepress-plugin-tabs/client'
import 'virtual:group-icons.css'

import '@mdi/font/css/materialdesignicons.css';

import 'uno.css'
import "./custom.css";

import 'improv-wifi-sdk/dist/web/launch-button.js'
import 'improv-wifi-serial-sdk/dist/web/serial-launch-button.js'

import 'esp-web-tools/dist/web/install-button.js'

export default {
    extends: DefaultTheme,
    // override the Layout with a wrapper component that
    // injects the slots
    Layout: Layout,
    enhanceApp({ app }) {
        enhanceAppWithTabs(app);
    },
    setup() {

    }
}