import DefaultTheme from "vitepress/theme";
import Layout from './Layout.vue'

import { enhanceAppWithTabs } from 'vitepress-plugin-tabs/client'
import 'virtual:group-icons.css'

import 'uno.css'
import "./custom.css";

export default {
    extends: DefaultTheme,
    // override the Layout with a wrapper component that
    // injects the slots
    Layout: Layout,
    enhanceApp({ app }) {
        enhanceAppWithTabs(app)
    }
}