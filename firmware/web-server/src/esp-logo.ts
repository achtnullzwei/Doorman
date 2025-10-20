import { LitElement, html } from "lit";
import { customElement } from "lit/decorators.js";

import logoUrl from "./logo.png?inline";

@customElement("esp-logo")
export default class EspLogo extends LitElement {
  createRenderRoot() {
    return this;
  }

  render() {
    return html`<img src="${logoUrl}" alt="ESPHome Logo" style="width: 40px; height: 40px;" />`;
  }
}