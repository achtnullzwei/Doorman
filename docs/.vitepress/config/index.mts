import { defineConfig } from 'vitepress'
import { tabsMarkdownPlugin } from 'vitepress-plugin-tabs'
import { groupIconMdPlugin, groupIconVitePlugin, localIconLoader } from 'vitepress-plugin-group-icons'

import { shared } from './shared.mts'
import { en } from './en.mts'
import { de } from './de.mts'

export default defineConfig({
  ...shared,
  locales: {
    root: { label: 'English', ...en },
    de: { label: 'Deutsch', ...de },
  },
  markdown: {
    config(md) {
      md.use(tabsMarkdownPlugin)
      md.use(groupIconMdPlugin)
    }
  },
  vite: {
    plugins: [
      groupIconVitePlugin({
        customIcon: {
          'homekit': localIconLoader(import.meta.url, '../../public/homekit-white.svg'),
          'home assistant': localIconLoader(import.meta.url, '../../public/home-assistant.svg'),
          'mqtt': localIconLoader(import.meta.url, '../../public/mqtt-icon-transparent.svg'),
        },
      })
    ],
  }
})