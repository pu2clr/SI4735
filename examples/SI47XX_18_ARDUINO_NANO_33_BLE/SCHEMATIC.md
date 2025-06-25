# Schematic and Wiring Diagrams

## Arduino Nano 33 BLE + SI4735 Basic Connection

```
                    Arduino Nano 33 BLE
                    ┌─────────────────────┐
                    │                     │
                    │                  A5 │──────────────┐
                    │                  A4 │──────────┐   │
                    │                  D12│──────┐   │   │
                    │                 3.3V│──┐   │   │   │
                    │                  GND│──│───│───│───│───┐
                    │                     │  │   │   │   │   │
                    └─────────────────────┘  │   │   │   │   │
                                             │   │   │   │   │
                                             │   │   │   │   │
                    SI4735 Module            │   │   │   │   │
                    ┌─────────────────────┐  │   │   │   │   │
                    │                     │  │   │   │   │   │
                    │               VCC   │──┘   │   │   │   │
                    │               GND   │──────┘   │   │   │
                    │      RESET (15)     │──────────┘   │   │
                    │      SDIO  (18)     │──────────────┘   │
                    │      SCLK  (17)     │──────────────────┘
                    │      SEN   (16)     │──────────────────────┐
                    │                     │                      │
                    └─────────────────────┘                      │
                                                                 │
                                                                GND
```

## Connection Table

| Arduino Nano 33 BLE Pin | SI4735 Pin | Wire Color Suggestion | Description |
|-------------------------|------------|----------------------|-------------|
| 3.3V | VCC | Red | Power supply |
| GND | GND | Black | Ground |
| A4 (SDA) | SDIO (18) | Blue | I2C Data |
| A5 (SCL) | SCLK (17) | Yellow | I2C Clock |
| D12 | RESET (15) | Green | Reset control |
| GND | SEN (16) | Black | I2C address (0x11) |

## Optional Audio Output Circuit

```
                    Arduino Nano 33 BLE
                    ┌─────────────────────┐
                    │                     │
                    │                  D3 │──────────────┐
                    │                  D4 │──────────┐   │
                    │                     │          │   │
                    └─────────────────────┘          │   │
                                                     │   │
                                                     │   │
                    Audio Amplifier (e.g., LM386)   │   │
                    ┌─────────────────────┐          │   │
                    │                     │          │   │
                    │    Volume Control   │──────────┘   │
                    │    Mute Control     │──────────────┘
                    │                     │
                    │    Audio Output     │──────────────► To Speaker
                    └─────────────────────┘
```

## Notes

1. **Voltage Compatibility**: The Arduino Nano 33 BLE operates at 3.3V, making it directly compatible with the SI4735 without level shifters.

2. **I2C Pull-ups**: The Arduino Nano 33 BLE has internal pull-up resistors on the I2C lines, so external pull-ups are typically not required.

3. **Power Supply**: Ensure adequate power supply for both the Arduino and SI4735. The SI4735 can draw up to 60mA during operation.

4. **Antenna**: Connect appropriate antennas to the SI4735 module for FM and AM reception.

5. **Layout**: Keep the wiring as short as possible to minimize interference, especially for the I2C lines.

6. **Ground**: Ensure solid ground connections for stable operation.

## BLE Range Considerations

- **Indoor Range**: Typically 10-30 meters depending on obstacles
- **Outdoor Range**: Up to 100 meters in clear line of sight
- **Interference**: Wi-Fi, other Bluetooth devices, and metal objects can reduce range
- **Antenna**: The Nano 33 BLE has an integrated antenna - orientation affects range

## Troubleshooting

### Common Issues:
1. **No I2C Communication**: Check SDA/SCL connections and pull-up resistors
2. **Reset Problems**: Ensure RESET pin is properly connected and not floating
3. **Power Issues**: Verify 3.3V power supply and ground connections
4. **BLE Connection**: Check that the device is advertising and within range

### Testing Steps:
1. Use Serial Monitor to verify SI4735 initialization
2. Use I2C scanner sketch to detect SI4735 address
3. Test BLE advertising with smartphone scanner app
4. Verify audio output with headphones or small speaker
