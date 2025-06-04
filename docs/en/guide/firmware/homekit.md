# HomeKit Integration

This page provides everything you need to set up and control your Doorman device with Apple HomeKit.

## Limitations
The HomeKit integration currently supports only the most essential and basic functions of your Doorman. Advanced settings and less commonly used features are not available through HomeKit and must still be managed via Home Assistant or the web interface.

### Supported Entites
- **Lock:** Entrance Doorbell
- **Lock:** Second Entrance Doorbell
- **Switch:** Ring To Open
- **Switch:** Ring To Open: Entrance Door
- **Switch:** Ring To Open: Second Entrance Door

## Pairing
After flashing the firmware with HomeKit integration and connecting Doorman to your Wi-Fi network, follow the steps below to pair it with the Home app on your iPhone or iPad.

<div style="display: flex; flex-direction: row; flex-wrap: wrap; justify-content: space-between; gap: 20px;align-items: center;" markdown>
   <div style="min-width:260px;flex: 1 1 260px;" markdown>

   1. To pair Doorman via HomeKit, open the Home app on your iPhone or iPad. Tap the "+" icon and select "Add Accessory".

   2. Scan the HomeKit setup QR code on the right side.  
      If you can't scan the code, select "I Don’t Have a Code or Cannot Scan" and then choose "Enter Code Manually" to type in the setup code: `291-21-998`
   
   3. Follow the on-screen instructions to complete pairing.  
    Once connected, you can control Doorman through the Home app or with Siri.

   </div>
   <div style="min-width: 100px;max-width:150px;flex: 1 1 100px;" markdown>
      <img style="" src="/homekit-qrcode.svg">
   </div>
</div>

## Reset Pairing
You can unpair all devices currently connected to your Doorman by using the `Reset HomeKit Pairing` button in the Home Assistant or Doorman web interface.