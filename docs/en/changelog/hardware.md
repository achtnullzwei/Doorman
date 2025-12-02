# PCB Revision History & Changelog
Welcome to the complete history of all PCB updates! This page highlights the new features, improvements, and changes made in each revision.

## 1.6.0 <Badge type="tip" text="Current" />
- Replace Terminal Pin 5 with 3.3V
- Replace Terminal Pin 6 with GPIO40
- Replace Terminal Pin 7 with GPIO41
- Remove BTN resistor  from GPIO41 (use internal ESP resistor)
- Extend ground plane around power supply section
- Change Voltage Divider to 1M + 147K (allow up to 28V Bus signals)
- Move diode after the resistor of BUSPWR jumper
- Improved Schematics

## 1.5.0
- Power supply redesign based on the reference design
- New method to power the Doorman S3 via TC:BUS using a jumper cap
- Connected GPIO10 to the BUS line, thus it's not needed to change the firmware to work with the voltage anymore
- Removed copper around the wifi antenna to improve signal strength
- New ESPHome Logo

## 1.4.0
- Added a diode to prevent multiple power sources damage the circuit

## 1.3.0
Initial release

## 1.2.0
No changelog available / Development Board

## 1.1.0
No changelog available / Development Board

## 1.0.0
No changelog available / Development Board