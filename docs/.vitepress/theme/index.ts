import DefaultTheme from "vitepress/theme";
import Layout from './Layout.vue'

import ContactModal from './components/ContactModal.vue'

import { enhanceAppWithTabs } from 'vitepress-plugin-tabs/client'
import 'virtual:group-icons.css'


import PrimeVue from 'primevue/config';
import Aura from '@primeuix/themes/aura';


import IconField from 'primevue/iconfield';
import InputIcon from 'primevue/inputicon';
import DataTable from 'primevue/datatable';
import Column from 'primevue/column';
import ColumnGroup from 'primevue/columngroup';   // optional
import Row from 'primevue/row';                   // optional


import "./styles.css";

import Vue3Datatable from '@bhplugin/vue3-datatable'
import '@bhplugin/vue3-datatable/dist/style.css'

export default {
    extends: DefaultTheme,
    // override the Layout with a wrapper component that
    // injects the slots
    Layout: Layout,
    enhanceApp({ app }) {
        app.component('ContactModal', ContactModal)

        app.use(PrimeVue, {
            theme: {
                preset: Aura
            }
        });

        app.component('Vue3Datatable', Vue3Datatable)

        app.component('IconField', IconField)
        app.component('InputIcon', InputIcon)
        app.component('DataTable', DataTable)
        app.component('Column', Column)
        app.component('ColumnGroup', ColumnGroup)
        app.component('Row', Row)

        enhanceAppWithTabs(app);

        if (typeof window !== 'undefined')
        {
            import('improv-wifi-sdk/dist/web/launch-button.js');
            import('improv-wifi-serial-sdk/dist/web/serial-launch-button.js');
            import('esp-web-tools/dist/web/install-button.js');
        }
    },
    setup() {

    }
}