# Ring To Open <Badge type="warning" text="RTO / Party Mode" />

Die Doorman-Firmware bietet eine **Ring To Open**-Automation, die früher als **Party Mode** bekannt war.

### Was macht es?
Wie der Name schon sagt, öffnet sich die Eingangstür automatisch, wenn jemand die Türklingel betätigt. Diese Funktion ist besonders nützlich während einer Party, da Gäste einfach durch Klingeln das Gebäude betreten können.

### Wie benutze ich es?
Du kannst die **Ring to Open**-Funktion für jede Tür individuell aktivieren oder deaktivieren.
Dafür stehen dir separate Schalter wie `RTO: Entrance Door`, `RTO: Second Entrance Door` und `RTO: Apartment Door` zur Verfügung.

Zusätzlich gibt es einen **zentralen Schalter** namens `Ring to Open`, der das für mehrere Türen gleichzeitig steuern kann.
Welche Türen davon betroffen sind, legst du über die Einstellung `RTO: Central Toggle – Door Selection` fest.

#### Allgemeine Einstellungen:
::: info Confirmation
Wenn du diese Option aktivierst, klingelt deine Innenstation jedes Mal, wenn du die Ring-to-Open-Automation manuell (z. B. per Funktionstaste) aktivierst.
:::

::: info Display Status
Du kannst festlegen, ob die Status-LED blinkt, während Ring to Open für eine der Türen aktiv ist.
:::

::: info Central Toggle - Trigger
Lege fest, wodurch der zentrale `Ring to Open`-Schalter umgeschaltet wird.
Zum Beispiel kannst du einstellen, dass die Funktionstaste deiner Innenstation den zentralen Schalter aktiviert oder deaktiviert.
:::

::: info Central Toggle - Door Selection
Bestimme, für welche Türen der zentrale `Ring-to-Open`-Schalter die Automation ein- oder ausschaltet.
:::

#### Einstellungen pro Tür:
::: info Delay
Diese Einstellung erlaubt es dir, eine Verzögerung für den Türöffner zu konfigurieren. Wenn du die Verzögerung auf 60 Sekunden einstellst, wird bei jeder Ausführung der Automation zufällig eine Verzögerung zwischen 5 und 15 Sekunden ausgewählt.
:::

::: info Timeout
Du kannst festlegen, ob Ring to Open nach einer bestimmten Zeit (zwischen 5 und 60 Minuten), nach einem einzigen Klingeln oder gar nicht automatisch deaktiviert wird.
:::

::: info Pattern Condition
Wähle aus, bei welchem Klingel-Muster die Automation ausgelöst werden soll.
:::