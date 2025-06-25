/*
   SI4735 Arduino Library with Bluetooth Low Energy Control
   Arduino Nano 33 BLE Example
   
   This advanced example demonstrates wireless control of the SI4735 radio receiver
   using Bluetooth Low Energy (BLE). The Arduino Nano 33 BLE's built-in nRF52840
   provides native BLE capability, allowing remote control via smartphone apps.

   Features:
   - FM, AM, and SW reception
   - Wireless BLE control interface
   - Custom BLE service for radio control
   - Real-time status updates via BLE
   - Compatible with generic BLE scanner apps
   - Serial Monitor for debugging

   BLE Service Structure:
   - Service UUID: 19B10000-E8F2-537E-4F6C-D104768A1214
   - Frequency Characteristic (Read/Write): 19B10001-E8F2-537E-4F6C-D104768A1214
   - Volume Characteristic (Read/Write): 19B10002-E8F2-537E-4F6C-D104768A1214
   - Mode Characteristic (Read/Write): 19B10003-E8F2-537E-4F6C-D104768A1214
   - Status Characteristic (Read/Notify): 19B10004-E8F2-537E-4F6C-D104768A1214

   Pin Connections (same as basic example):
   ┌─────────────────────────────────────────────────────────────┐
   │ SI4735 Pin  │ Arduino Nano 33 BLE Pin │ Description         │
   │─────────────│─────────────────────────│─────────────────────│
   │ VCC         │ 3.3V                    │ Power supply        │
   │ GND         │ GND                     │ Ground              │
   │ SDIO (18)   │ A4 (SDA)               │ I2C Data line       │
   │ SCLK (17)   │ A5 (SCL)               │ I2C Clock line      │
   │ RESET (15)  │ D12                     │ Reset control       │
   │ SEN (16)    │ GND                     │ I2C addr = 0x11     │
   └─────────────────────────────────────────────────────────────┘

   Usage:
   1. Upload this sketch to Arduino Nano 33 BLE
   2. Open Serial Monitor to see BLE advertising
   3. Use a BLE scanner app (like "BLE Scanner" or "nRF Connect")
   4. Connect to "SI4735 Radio" device
   5. Write values to characteristics to control the radio

   BLE Control Values:
   - Mode: 0=FM, 1=AM, 2=SW
   - Frequency: Write frequency in kHz (e.g., 103900 for 103.9 MHz FM)
   - Volume: 0-63
   - Status: JSON format with current radio status

   Required Libraries:
   - SI4735 by PU2CLR
   - ArduinoBLE (install via Library Manager)

   Author: Ricardo Lima Caratti (PU2CLR)
   Date: June 2025
   
   References:
   - SI4735 Library: https://github.com/pu2clr/SI4735  
   - ArduinoBLE: https://github.com/arduino-libraries/ArduinoBLE
*/

#include <SI4735.h>
#include <ArduinoBLE.h>

// Pin definitions
#define RESET_PIN 12

// Radio modes
#define MODE_FM 0
#define MODE_AM 1  
#define MODE_SW 2

// BLE Service and Characteristics UUIDs
#define SERVICE_UUID "19B10000-E8F2-537E-4F6C-D104768A1214"
#define FREQUENCY_UUID "19B10001-E8F2-537E-4F6C-D104768A1214"
#define VOLUME_UUID "19B10002-E8F2-537E-4F6C-D104768A1214" 
#define MODE_UUID "19B10003-E8F2-537E-4F6C-D104768A1214"
#define STATUS_UUID "19B10004-E8F2-537E-4F6C-D104768A1214"

// Global variables
SI4735 rx;
uint16_t currentFrequency;
uint8_t currentVolume;
uint8_t currentMode = MODE_FM;
unsigned long lastStatusUpdate = 0;
const unsigned long STATUS_UPDATE_INTERVAL = 2000; // 2 seconds

// BLE Service and Characteristics
BLEService radioService(SERVICE_UUID);
BLEUnsignedLongCharacteristic frequencyChar(FREQUENCY_UUID, BLERead | BLEWrite);
BLEByteCharacteristic volumeChar(VOLUME_UUID, BLERead | BLEWrite);
BLEByteCharacteristic modeChar(MODE_UUID, BLERead | BLEWrite);
BLEStringCharacteristic statusChar(STATUS_UUID, BLERead | BLENotify, 200);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port
  }

  Serial.println("======================================================");
  Serial.println("SI4735 BLE Radio Controller - Arduino Nano 33 BLE");
  Serial.println("PU2CLR SI4735 Arduino Library with BLE Control");
  Serial.println("======================================================");

  // Initialize SI4735
  if (!initializeRadio()) {
    Serial.println("Failed to initialize radio. Halting...");
    while (1) delay(1000);
  }

  // Initialize BLE
  if (!initializeBLE()) {
    Serial.println("Failed to initialize BLE. Halting...");
    while (1) delay(1000);
  }

  Serial.println("System ready!");
  Serial.println("Connect with a BLE scanner app to control the radio.");
  Serial.println("Device name: SI4735 Radio");
  Serial.println();
}

void loop() {
  // Poll for BLE events
  BLE.poll();

  // Handle BLE characteristic updates
  handleBLEUpdates();
  
  // Send periodic status updates
  if (millis() - lastStatusUpdate > STATUS_UPDATE_INTERVAL) {
    updateStatus();
    lastStatusUpdate = millis();
  }

  delay(100);
}

bool initializeRadio() {
  Serial.println("Initializing SI4735...");
  
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  // Look for SI4735
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if (si4735Addr == 0) {
    Serial.println("SI4735 not found!");
    return false;
  }

  Serial.print("SI4735 found at address: 0x");
  Serial.println(si4735Addr, HEX);

  // Initialize in FM mode
  rx.setup(RESET_PIN, 0); // 0 = FM
  rx.setFM(8800, 10800, 10390, 10); // 88-108 MHz, start at 103.9, 100kHz steps
  
  currentFrequency = rx.getFrequency();
  currentVolume = 35;
  rx.setVolume(currentVolume);
  currentMode = MODE_FM;

  Serial.println("SI4735 initialized successfully!");
  return true;
}

bool initializeBLE() {
  Serial.println("Initializing BLE...");

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    return false;
  }

  // Set BLE device name and service
  BLE.setLocalName("SI4735 Radio");
  BLE.setAdvertisedService(radioService);

  // Add characteristics to service
  radioService.addCharacteristic(frequencyChar);
  radioService.addCharacteristic(volumeChar);
  radioService.addCharacteristic(modeChar);
  radioService.addCharacteristic(statusChar);

  // Add service
  BLE.addService(radioService);

  // Set initial characteristic values
  frequencyChar.writeValue(currentFrequency);
  volumeChar.writeValue(currentVolume);
  modeChar.writeValue(currentMode);
  
  // Set event handlers
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  
  frequencyChar.setEventHandler(BLEWritten, frequencyCharacteristicWritten);
  volumeChar.setEventHandler(BLEWritten, volumeCharacteristicWritten);
  modeChar.setEventHandler(BLEWritten, modeCharacteristicWritten);

  // Start advertising
  BLE.advertise();

  Serial.println("BLE initialized successfully!");
  Serial.print("BLE device address: ");
  Serial.println(BLE.address());
  Serial.println("Advertising as 'SI4735 Radio'...");
  
  return true;
}

void handleBLEUpdates() {
  // This function is called from the main loop to handle any pending BLE updates
  // The actual handling is done in the event handlers
}

void frequencyCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  uint32_t newFreq = frequencyChar.value();
  Serial.print("BLE: New frequency requested: ");
  Serial.println(newFreq);
  
  // Validate and set frequency based on current mode
  if (currentMode == MODE_FM) {
    if (newFreq >= 8800 && newFreq <= 10800) {
      rx.setFrequency(newFreq);
      currentFrequency = newFreq;
      Serial.print("FM frequency set to: ");
      Serial.print(newFreq / 100.0, 1);
      Serial.println(" MHz");
    }
  } else {
    if (newFreq >= 150 && newFreq <= 30000) {
      rx.setFrequency(newFreq);
      currentFrequency = newFreq;
      Serial.print("AM frequency set to: ");
      if (newFreq < 1000) {
        Serial.print(newFreq);
        Serial.println(" kHz");
      } else {
        Serial.print(newFreq / 1000.0, 3);
        Serial.println(" MHz");
      }
    }
  }
}

void volumeCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t newVolume = volumeChar.value();
  Serial.print("BLE: New volume requested: ");
  Serial.println(newVolume);
  
  if (newVolume <= 63) {
    rx.setVolume(newVolume);
    currentVolume = newVolume;
    Serial.print("Volume set to: ");
    Serial.println(newVolume);
  }
}

void modeCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t newMode = modeChar.value();
  Serial.print("BLE: New mode requested: ");
  Serial.println(newMode);
  
  switch (newMode) {
    case MODE_FM:
      rx.setFM(8800, 10800, 10390, 10);
      currentMode = MODE_FM;
      Serial.println("Switched to FM mode");
      break;
      
    case MODE_AM:
      rx.setAM(520, 1750, 810, 10);
      currentMode = MODE_AM;
      Serial.println("Switched to AM mode");
      break;
      
    case MODE_SW:
      rx.setAM(150, 30000, 7200, 5);
      currentMode = MODE_SW;
      Serial.println("Switched to SW mode");
      break;
      
    default:
      Serial.println("Invalid mode requested");
      return;
  }
  
  currentFrequency = rx.getFrequency();
  frequencyChar.writeValue(currentFrequency);
}

void updateStatus() {
  currentFrequency = rx.getFrequency();
  rx.getCurrentReceivedSignalQuality();
  
  // Create JSON status string
  String status = "{";
  status += "\"frequency\":" + String(currentFrequency) + ",";
  status += "\"volume\":" + String(currentVolume) + ",";
  status += "\"mode\":" + String(currentMode) + ",";
  status += "\"rssi\":" + String(rx.getCurrentRSSI()) + ",";
  status += "\"snr\":" + String(rx.getCurrentSNR()) + ",";
  
  if (currentMode == MODE_FM) {
    status += "\"stereo\":" + String(rx.getCurrentPilot() ? 1 : 0) + ",";
    status += "\"modeText\":\"FM\"";
  } else {
    status += "\"stereo\":0,";
    status += "\"modeText\":\"" + String(currentMode == MODE_AM ? "AM" : "SW") + "\"";
  }
  
  status += "}";
  
  statusChar.writeValue(status);
  
  // Also print to serial for debugging
  Serial.print("Status update: ");
  Serial.println(status);
}

void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
  
  // Send initial status
  updateStatus();
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}
