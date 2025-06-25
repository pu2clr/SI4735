# SI4735 Bluetooth Low Energy Control Example

This example demonstrates how to control the SI4735 radio receiver wirelessly using Bluetooth Low Energy (BLE) with the Arduino Nano 33 BLE.

## Features

- **Wireless Control**: Control your radio remotely via smartphone or tablet
- **Real-time Updates**: Get live status updates including frequency, signal strength, and stereo detection
- **Multiple Bands**: Switch between FM, AM, and SW modes via BLE
- **Custom BLE Service**: Dedicated radio control service with multiple characteristics
- **JSON Status**: Structured status information for easy parsing by client applications

## BLE Service Structure

### Service UUID
`19B10000-E8F2-537E-4F6C-D104768A1214`

### Characteristics

| Characteristic | UUID | Type | Description |
|----------------|------|------|-------------|
| Frequency | `19B10001-E8F2-537E-4F6C-D104768A1214` | Read/Write (UInt32) | Current frequency in kHz |
| Volume | `19B10002-E8F2-537E-4F6C-D104768A1214` | Read/Write (Byte) | Volume level (0-63) |
| Mode | `19B10003-E8F2-537E-4F6C-D104768A1214` | Read/Write (Byte) | Radio mode (0=FM, 1=AM, 2=SW) |
| Status | `19B10004-E8F2-537E-4F6C-D104768A1214` | Read/Notify (String) | JSON status information |

## Usage Instructions

### 1. Hardware Setup
Connect the SI4735 module to your Arduino Nano 33 BLE as shown in the main README.md file.

### 2. Software Setup
1. Install required libraries:
   - PU2CLR SI4735 library
   - ArduinoBLE library (via Arduino Library Manager)

2. Upload the sketch to your Arduino Nano 33 BLE

3. Open Serial Monitor to see initialization messages

### 3. BLE Connection
1. The device will advertise as "SI4735 Radio"
2. Use a BLE scanner app such as:
   - **Android**: "BLE Scanner" by Bluepixel Technologies
   - **iOS**: "BLE Scanner 4.0" by Kai Wegner
   - **Cross-platform**: "nRF Connect" by Nordic Semiconductor

3. Connect to the "SI4735 Radio" device
4. Navigate to the radio service UUID

### 4. Control Commands

#### Change Radio Mode
Write to Mode characteristic:
- `0` - FM mode (88-108 MHz)
- `1` - AM mode (520-1750 kHz) 
- `2` - SW mode (1.5-30 MHz)

#### Set Frequency
Write to Frequency characteristic (value in kHz):
- FM: `103900` for 103.9 MHz
- AM: `810` for 810 kHz
- SW: `7200` for 7.2 MHz

#### Adjust Volume
Write to Volume characteristic:
- Range: `0` to `63`
- `0` = minimum volume, `63` = maximum volume

#### Get Status
Read or subscribe to Status characteristic to get JSON data:
```json
{
  "frequency": 103900,
  "volume": 35,
  "mode": 0,
  "rssi": 45,
  "snr": 12,
  "stereo": 1,
  "modeText": "FM"
}
```

## Example Client Code

### Python Example (using bleak library)
```python
import asyncio
from bleak import BleakClient, BleakScanner

SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214"
FREQUENCY_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"
VOLUME_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214"
MODE_UUID = "19B10003-E8F2-537E-4F6C-D104768A1214"
STATUS_UUID = "19B10004-E8F2-537E-4F6C-D104768A1214"

async def control_radio():
    # Find and connect to SI4735 Radio
    devices = await BleakScanner.discover()
    radio_device = None
    
    for device in devices:
        if device.name == "SI4735 Radio":
            radio_device = device
            break
    
    if not radio_device:
        print("SI4735 Radio not found")
        return
    
    async with BleakClient(radio_device) as client:
        # Set FM mode
        await client.write_gatt_char(MODE_UUID, bytes([0]))
        
        # Set frequency to 103.9 MHz (103900 kHz)
        freq_bytes = (103900).to_bytes(4, byteorder='little')
        await client.write_gatt_char(FREQUENCY_UUID, freq_bytes)
        
        # Set volume to 40
        await client.write_gatt_char(VOLUME_UUID, bytes([40]))
        
        # Read status
        status = await client.read_gatt_char(STATUS_UUID)
        print(f"Status: {status.decode()}")

# Run the example
asyncio.run(control_radio())
```

## Troubleshooting

### BLE Connection Issues
- Ensure the Arduino Nano 33 BLE is properly powered
- Check that the device is advertising (look for "SI4735 Radio" in BLE scanner)
- Try restarting both the Arduino and the client device
- Make sure no other devices are connected to the radio

### Radio Issues
- Verify SI4735 wiring connections
- Check power supply (3.3V)
- Ensure I2C pull-up resistors are present if needed
- Monitor Serial output for error messages

### Performance Tips
- Keep BLE client within reasonable range (typically 10-30 meters)
- Avoid excessive characteristic writes (the radio needs time to process changes)
- Subscribe to status notifications instead of constantly polling

## Advanced Features

You can extend this example by:
- Adding preset station management
- Implementing RDS data transmission
- Creating a custom mobile app interface
- Adding audio streaming capabilities
- Integrating with home automation systems

## Support

For questions and support:
- Visit: https://github.com/pu2clr/SI4735
- Arduino BLE documentation: https://docs.arduino.cc/tutorials/nano-33-ble/bluetooth
