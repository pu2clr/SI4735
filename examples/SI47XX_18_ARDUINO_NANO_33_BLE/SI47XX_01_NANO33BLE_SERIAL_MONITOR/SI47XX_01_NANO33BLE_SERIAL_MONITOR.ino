/*
   SI4735 Arduino Library Example for Arduino Nano 33 BLE
   
   This sketch demonstrates how to use the SI4735 library with the Arduino Nano 33 BLE.
   The Arduino Nano 33 BLE features a Nordic nRF52840 microcontroller with built-in
   Bluetooth Low Energy capability and native 3.3V operation, making it ideal for
   modern radio receiver projects.

   Features:
   - FM, AM, and SW reception (1.7MHz to 30MHz)
   - Serial Monitor control interface
   - Native 3.3V operation (no level shifters needed)
   - Compact form factor
   - Ready for BLE expansion

   ATTENTION: Please avoid using a computer connected to the mains during testing.
              This may cause interference with the radio receiver.

   Pin Connections:
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

   Serial Monitor Commands:
   - F: Switch to FM mode
   - A: Switch to AM mode  
   - 1: Switch to All Band mode (SW)
   - U/D: Frequency up/down
   - S/s: Seek up/down
   - +/-: Volume up/down
   - 0: Show current status
   - B: Change bandwidth filter
   - 4-8: Change frequency step
   - ?: Show help

   Author: Ricardo Lima Caratti (PU2CLR)
   Date: June 2025
   
   References:
   - SI4735 Library: https://github.com/pu2clr/SI4735
   - Arduino Nano 33 BLE: https://docs.arduino.cc/hardware/nano-33-ble
   - Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png
*/

#include <SI4735.h>

// Pin definitions for Arduino Nano 33 BLE
#define RESET_PIN 12        // Digital pin 12 for SI4735 reset
#define SDA_PIN A4          // I2C Data (built-in Wire library uses A4)
#define SCL_PIN A5          // I2C Clock (built-in Wire library uses A5)

// Function definitions
#define AM_FUNCTION 1
#define FM_FUNCTION 0

// Global variables
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

SI4735 rx;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }

  // Initialize reset pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  
  Serial.println("======================================================");
  Serial.println("SI4735 Radio Receiver - Arduino Nano 33 BLE");
  Serial.println("PU2CLR SI4735 Arduino Library");
  Serial.println("======================================================");
  Serial.println();
  
  // Display board information
  Serial.println("Board: Arduino Nano 33 BLE (nRF52840)");
  Serial.println("Microcontroller: Nordic nRF52840 (ARM Cortex-M4)");
  Serial.println("Operating Voltage: 3.3V");
  Serial.println("I2C Pins: A4 (SDA), A5 (SCL)");
  Serial.println();

  showHelp();

  // Look for the Si47XX I2C bus address
  Serial.print("Searching for SI4735 on I2C bus... ");
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  
  if (si4735Addr == 0) {
    Serial.println("FAILED!");
    Serial.println();
    Serial.println("ERROR: SI4735 not found!");
    Serial.println("Please check:");
    Serial.println("1. Wiring connections");
    Serial.println("2. SI4735 module power supply");
    Serial.println("3. I2C pull-up resistors (if needed)");
    Serial.flush();
    while (1) {
      delay(1000); // Halt execution
    }
  } else {
    Serial.println("SUCCESS!");
    Serial.print("SI4735 I2C address: 0x");
    Serial.println(si4735Addr, HEX);
  }

  Serial.println();
  Serial.println("Initializing SI4735...");
  
  delay(500);
  
  // Initialize the SI4735
  rx.setup(RESET_PIN, FM_FUNCTION);
  
  // Set up FM band (84-108 MHz, start at 103.9 MHz, 100kHz steps)
  rx.setFM(8400, 10800, 10390, 10);
  
  delay(500);
  
  // Get initial frequency and set volume
  currentFrequency = previousFrequency = rx.getFrequency();
  rx.setVolume(35); // Set moderate volume level
  
  Serial.println("SI4735 initialized successfully!");
  Serial.println();
  
  showStatus();
}

void loop() {
  if (Serial.available() > 0) {
    char key = Serial.read();
    
    switch (key) {
      case 'F':
      case 'f':
        rx.setFM(8400, 10800, 10390, 10);
        Serial.println("FM mode selected");
        showStatus();
        break;
        
      case 'A':
      case 'a':
        rx.setAM(520, 1750, 810, 10);
        Serial.println("AM mode selected");
        showStatus();
        break;
        
      case '1':
        rx.setAM(150, 30000, 7200, 5);
        Serial.println("All Band mode selected (SW)");
        showStatus();
        break;
        
      case 'U':
      case 'u':
        rx.frequencyUp();
        showStatus();
        break;
        
      case 'D':
      case 'd':
        rx.frequencyDown();
        showStatus();
        break;
        
      case 'S':
        rx.seekStationUp();
        showStatus();
        break;
        
      case 's':
        rx.seekStationDown();
        showStatus();
        break;
        
      case '+':
        rx.volumeUp();
        showStatus();
        break;
        
      case '-':
        rx.volumeDown();
        showStatus();
        break;
        
      case '0':
        showStatus();
        break;
        
      case 'B':
      case 'b':
        if (rx.isCurrentTuneFM()) {
          Serial.println("Bandwidth control not available in FM mode");
        } else {
          bandwidthIdx++;
          if (bandwidthIdx > 6) bandwidthIdx = 0;
          rx.setBandwidth(bandwidthIdx, 1);
          Serial.print("Bandwidth set to: ");
          Serial.print(bandwidth[bandwidthIdx]);
          Serial.println(" kHz");
          showStatus();
        }
        break;
        
      case '4':
        rx.setFrequencyStep(1);
        Serial.println("Frequency step: 1 kHz");
        break;
        
      case '5':
        rx.setFrequencyStep(5);
        Serial.println("Frequency step: 5 kHz");
        break;
        
      case '6':
        rx.setFrequencyStep(10);
        Serial.println("Frequency step: 10 kHz");
        break;
        
      case '7':
        rx.setFrequencyStep(100);
        Serial.println("Frequency step: 100 kHz");
        break;
        
      case '8':
        rx.setFrequencyStep(1000);
        Serial.println("Frequency step: 1000 kHz");
        break;
        
      case '?':
        showHelp();
        break;
        
      default:
        break;
    }
  }
  
  delay(100);
}

void showHelp() {
  Serial.println("======================================================");
  Serial.println("               CONTROL COMMANDS");
  Serial.println("======================================================");
  Serial.println("F or f ........ Switch to FM mode");
  Serial.println("A or a ........ Switch to AM mode");
  Serial.println("1 ............. Switch to All Band mode (SW)");
  Serial.println("U or u ........ Frequency UP");
  Serial.println("D or d ........ Frequency DOWN");
  Serial.println("S ............. Seek station UP");
  Serial.println("s ............. Seek station DOWN");
  Serial.println("+ ............. Volume UP");
  Serial.println("- ............. Volume DOWN");
  Serial.println("0 ............. Show current status");
  Serial.println("B or b ........ Change bandwidth filter (AM/SW only)");
  Serial.println("4 ............. Set frequency step to 1 kHz");
  Serial.println("5 ............. Set frequency step to 5 kHz");
  Serial.println("6 ............. Set frequency step to 10 kHz");
  Serial.println("7 ............. Set frequency step to 100 kHz");
  Serial.println("8 ............. Set frequency step to 1000 kHz");
  Serial.println("? ............. Show this help");
  Serial.println("======================================================");
  Serial.println();
}

void showStatus() {
  currentFrequency = rx.getFrequency();
  
  Serial.println("------------------------------------------------------");
  
  if (rx.isCurrentTuneFM()) {
    Serial.print("FM: ");
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print(" MHz");
    
    rx.getCurrentReceivedSignalQuality();
    Serial.print(" | Signal: ");
    Serial.print(rx.getCurrentRSSI());
    Serial.print(" dBμV");
    Serial.print(" | SNR: ");
    Serial.print(rx.getCurrentSNR());
    Serial.print(" dB");
    Serial.print(" | ");
    Serial.print((rx.getCurrentPilot()) ? "STEREO" : "MONO");
    
  } else {
    Serial.print("AM: ");
    if (currentFrequency < 1000) {
      Serial.print(currentFrequency);
      Serial.print(" kHz");
    } else {
      Serial.print(String(currentFrequency / 1000.0, 3));
      Serial.print(" MHz");
    }
    
    rx.getCurrentReceivedSignalQuality();
    Serial.print(" | Signal: ");
    Serial.print(rx.getCurrentRSSI());
    Serial.print(" dBμV");
    Serial.print(" | SNR: ");
    Serial.print(rx.getCurrentSNR());
    Serial.print(" dB");
    Serial.print(" | BW: ");
    Serial.print(bandwidth[bandwidthIdx]);
    Serial.print(" kHz");
  }
  
  Serial.print(" | Volume: ");
  Serial.print(rx.getCurrentVolume());
  Serial.println();
  Serial.println("------------------------------------------------------");
  Serial.println();
}
