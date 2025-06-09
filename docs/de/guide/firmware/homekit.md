# HomeKit Integration

Diese Seite bietet dir alle Informationen, die du brauchst, um dein Doorman-Gerät mit Apple HomeKit einzurichten und zu steuern.

## Einschränkungen
Die HomeKit-Integration unterstützt derzeit nur die wichtigsten und grundlegendsten Funktionen deines Doormans. Erweiterte Einstellungen und weniger häufig genutzte Funktionen stehen über HomeKit nicht zur Verfügung und müssen weiterhin über Home Assistant oder die Weboberfläche gesteuert werden.

### Unterstützte Entitäten
- **Ereignis:** Entrance Doorbell
- **Ereignis:** Second Entrance Doorbell
- **Ereignis:** Apartment Doorbell
- **Schloss:** Entrance Door
- **Schloss:** Second Entrance Door
- **Schalter:** Ring To Open
- **Schalter:** Ring To Open: Entrance Door
- **Schalter:** Ring To Open: Second Entrance Door

## Koppeln
<!--@include: ./instructions/homekit.md-->

## Kopplung zurücksetzen
Klicke in der Doorman-Weboberfläche im Bereich `HomeKit Configuration` auf den Button `Reset HomeKit Pairing`, um alle aktuell mit deinem Doorman verbundenen Controller zu entkoppeln.