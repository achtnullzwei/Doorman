import { defineConfig } from 'vitepress'
import { tabsMarkdownPlugin } from 'vitepress-plugin-tabs'
import { groupIconMdPlugin, groupIconVitePlugin } from 'vitepress-plugin-group-icons'

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
      groupIconVitePlugin()
    ],
  }
})