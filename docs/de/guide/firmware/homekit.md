# HomeKit Integration

Diese Seite bietet dir alle Informationen, die du brauchst, um dein Doorman-Gerät mit Apple HomeKit einzurichten und zu steuern.

## Einschränkungen
Die HomeKit-Integration unterstützt derzeit nur die wichtigsten und grundlegendsten Funktionen deines Doormans. Erweiterte Einstellungen und weniger häufig genutzte Funktionen stehen über HomeKit nicht zur Verfügung und müssen weiterhin über Home Assistant oder die Weboberfläche gesteuert werden.

### Unterstützte Entitäten
- **Schloss:** Entrance Door
- **Schloss:** Second Entrance Door
- **Schalter:** Ring To Open
- **Schalter:** Ring To Open: Entrance Door
- **Schalter:** Ring To Open: Second Entrance Door

## Koppeln
Nachdem du die Firmware mit HomeKit-Unterstützung geflasht und Doorman mit deinem WLAN verbunden hast, folge den untenstehenden Schritten, um die Verbindung mit der Home-App auf deinem iPhone oder iPad herzustellen.

<div style="display: flex; flex-direction: row; flex-wrap: wrap; justify-content: space-between; gap: 20px;align-items: center;" markdown>
   <div style="min-width:260px;flex: 1 1 260px;" markdown>

   1. Öffne die Home-App auf deinem iPhone oder iPad. Tippe auf das "+"-Symbol und wähle "Zubehör hinzufügen".

   2. Scanne den HomeKit-Setup-QR-Code auf der rechten Seite.  
      Falls du den Code nicht scannen kannst, wähle "Ich habe keinen Code oder kann ihn nicht scannen" und gib anschließend den Setup-Code manuell ein: `291-21-998`
   
   3. Folge den Anweisungen auf dem Bildschirm, um die Kopplung abzuschließen.  
      Nach erfolgreicher Verbindung kannst du Doorman über die Home-App oder mit Siri steuern.

   </div>
   <div style="min-width: 100px;max-width:150px;flex: 1 1 100px;" markdown>
      <img style="" src="/homekit-qrcode.svg">
   </div>
</div>

## Kopplung zurücksetzen
Du kannst alle aktuell mit deinem Doorman verbundenen Geräte entkoppeln, indem du den Button `Reset HomeKit Pairing` in der Home Assistant- oder Doorman-Weboberfläche benutzt.