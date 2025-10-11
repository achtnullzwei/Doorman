# TC:BUS Serial Component <Badge type="tip" text="tc_bus_serial" />
This component extends the [TC:BUS](./esphome-component) base component, enabling direct interaction over a serial connection.

## Configuration
This component does not offer any configuration options.

## Example YAML Configuration
This is an example configuration for the component in ESPHome:

```yaml
external_components:
  - source: github://azoninc/doorman@master
    components: [ tc_bus, tc_bus_serial ]

## RMT configuration
remote_receiver:
  pin:
    number: GPIO9
    mode: INPUT
  filter: 1500us
  idle: 7000us

remote_transmitter:
  pin:
    number: GPIO8
    mode: OUTPUT
  carrier_duty_percent: 100%

# TC:BUS configuration
tc_bus:

# TC:BUS Serial configuration
tc_bus_serial:
```

## Example Application

This Node.js example listens for a door call telegram and automatically opens the door when one is received.

::: code-group

```js [app.js]
const { SerialPort } = require('serialport');
const { RegexParser } = require('@serialport/parser-regex');

// Port-Name and Baudrate
const portName = 'COM11';
const baudRate = 9600;

// Initialize serial port
const port = new SerialPort({
  path: portName,
  baudRate: baudRate,
});

// Parser for serial messages
// Detect telegram between 0x01 and 0x04
const parser = port.pipe(
    new RegexParser({ regex: /\x01(.*?)\x04/ })
);

parser.on('data', (data) => {
    let type = data.substring(0, 1);
    let telegram = data.substring(1);
    
    if(type == '$') {
        // Telegrams received by the tc_bus base component
        console.log('Received telegram from TC:BUS: "' + telegram + '"');

        // Call from outdoor station 0 to serial number C30BA
        if(telegram == '0C30BA80') {
            // Open door
            sendTelegram("1100");
        }
    } else {
        // Telegrams sent by the tc_bus base component
        console.log('Sent telegram to TC:BUS: "' + telegram + '"');
    }
});

function sendTelegram(telegram) {
    port.write(Buffer.from([0x01]));
    port.write(telegram);
    port.write(Buffer.from([0x04]));

    console.log('Sent telegram to TC:BUS: "' + telegram + '"');
}

port.on('error', (err) => {
  console.error('Error:', err.message);
});

port.on('open', () => {
  console.log(`Opened Serial Port: ${portName}`);
});
```
:::