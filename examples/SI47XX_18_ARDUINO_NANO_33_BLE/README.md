# SI4735 Arduino Library - Arduino Nano 33 BLE Examples

This folder contains examples specifically designed for the Arduino Nano 33 BLE (nRF52840) microcontroller board.

## Arduino Nano 33 BLE Features

The Arduino Nano 33 BLE is a compact board based on the Nordic nRF52840 microcontroller, featuring:
- ARM Cortex-M4 32-bit processor running at 64 MHz
- 256 KB RAM and 1 MB Flash memory
- Built-in Bluetooth Low Energy (BLE) connectivity
- 3.3V operating voltage (SI4735 compatible)
- Native I2C support on pins A4 (SDA) and A5 (SCL)
- Compact Nano form factor

## Pin Connections

### Basic SI4735 Connection
| SI4735 Pin | Arduino Nano 33 BLE Pin | Description |
|------------|-------------------------|-------------|
| VCC        | 3.3V                   | Power supply |
| GND        | GND                    | Ground |
| SDIO       | A4 (SDA)               | I2C Data |
| SCLK       | A5 (SCL)               | I2C Clock |
| RESET      | D12                    | Reset pin |
| SEN        | GND                    | I2C address selection |

### Optional Audio Output
| Audio Component | Arduino Pin | Description |
|----------------|-------------|-------------|
| Audio Amplifier | D3 (PWM)   | Volume control via PWM |
| Mute Control    | D4         | Audio mute control |

## Examples

### SI47XX_01_NANO33BLE_SERIAL_MONITOR
Basic SI4735 control via Serial Monitor. No external components required except the SI4735 module.

### SI47XX_02_NANO33BLE_BLE_CONTROL  
Advanced example using Bluetooth Low Energy to control the radio remotely via smartphone app.

## Important Notes

1. **Voltage Compatibility**: The Arduino Nano 33 BLE operates at 3.3V, making it directly compatible with the SI4735 without level shifters.

2. **I2C Pins**: Use pins A4 (SDA) and A5 (SCL) for I2C communication. These are the standard I2C pins for Arduino boards.

3. **BLE Capability**: The built-in BLE can be used to create wireless radio control applications.

4. **Power Consumption**: The nRF52840 is designed for low power applications, making it suitable for battery-powered radio projects.

5. **Programming**: Use the Arduino IDE with the Arduino Mbed OS Nano Boards package installed.

## Installation

1. Install the Arduino Mbed OS Nano Boards package in Arduino IDE
2. Install the PU2CLR SI4735 library
3. For BLE examples, install the ArduinoBLE library

## Schematic

```
Arduino Nano 33 BLE         SI4735 Module
┌─────────────────┐         ┌─────────────────┐
│              A5 │────────►│ SCLK (17)       │
│              A4 │◄───────►│ SDIO (18)       │
│             D12 │────────►│ RESET (15)      │
│            3.3V │────────►│ VCC             │
│             GND │────────►│ GND             │
│                 │         │ SEN (16)        │◄─── GND
└─────────────────┘         └─────────────────┘
```

## Support

For questions and support, visit: https://github.com/pu2clr/SI4735
