/*
    Is is an example to show how to save data into the internal Arduino EEPROM.
    The Arduino used for this example is the Pro Mini 8MHz 3.3V.
    The receiver setup is a FM receiver.

    This sketch implements the circuit https://pu2clr.github.io/SI4735/extras/schematic#storing-data-into-the-internal-eeprom-before-shutdowning.
    It saves the Frequency and Volume values into the internal Arduino EEPROM after turning the system off.

    The table below shows the Si4735 and Arduino Pro Mini pin connections

    | Si4735 pin        |  Arduino Pin |
    | ----------------- | ------------ |
    | RESET (pin 15)    |     12       |
    | SDIO (pin 18)     |     A4       |
    | CLK (pin 17)      |     A5       |
    | TURN OFF DETECTOR |   A3 / 16    |

  Schematic:
  Prototype documentation : https://pu2clr.github.io/SI4735/extras/schematic#storing-data-into-the-internal-eeprom-before-shutdowning
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Nov 2021.
*/

#include <SI4735.h>
#include <EEPROM.h>
#include <Tiny4kOLED.h>
#include "Rotary.h"


#define RESET_PIN 12
#define SHUTDOWN_DETECTOR 16 // A2 - Arduino pin 16 configured as digital
#define FM_FUNCTION 0

#define VOLUME_DOWN 4
#define VOLUME_UP   5
#define ENCODER_A   2
#define ENCODER_B   3

const uint8_t app_id =  35; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;

uint16_t currentFrequency = 0;
uint16_t previousFrequency = 0;
uint16_t defaultFrequency = 10390;  // Default frequency value is 103.90 MHz
uint8_t volume;
uint8_t freqByteLow;
uint8_t freqByteHigh;

volatile int encoderCount = 0;
Rotary encoder = Rotary(ENCODER_A, ENCODER_B);
SI4735 rx;

void setup()
{
  pinMode(SHUTDOWN_DETECTOR, INPUT); // If HIGH power supply detected; else, no power supply detected.

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(VOLUME_DOWN, INPUT_PULLUP);
  pinMode(VOLUME_UP, INPUT_PULLUP);

  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);

  showSplash();
  cleanDisplay();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if (si4735Addr == 0)
  {
    oled.print("SI473X not found");
    while (1);
  } else
    oled.print("SI473X found");
  delay(2000);
  // If you want to reset the eeprom, keep the ENCODER_PUSH_BUTTON (BFO_SWITCH) button pressed during statup
  if (digitalRead(VOLUME_UP) == LOW)
  {
    cleanDisplay();
    EEPROM.write(eeprom_address, 0);
    oled.print("EEPROM RESETED");
    delay(2000);
  }
  cleanDisplay();
  // Checking the EEPROM content
  if (EEPROM.read(eeprom_address) == app_id)
  {
    // There are useful data stored to rescue
    volume = EEPROM.read(eeprom_address + 1); // Gets the stored volume;
    freqByteHigh = EEPROM.read(eeprom_address + 2); // Gets the frequency high byte
    freqByteLow = EEPROM.read(eeprom_address + 3);  // Gets the frequency low  byte
    currentFrequency = (freqByteHigh << 8) | freqByteLow; // Converts the stored frequency to SI473X frequency.
    oled.print("Data restored");
    oled.setCursor(0, 2);
    oled.print("Using last setup");
  }
  else
  {
    oled.print("No data found!");
    oled.setCursor(0, 2);
    oled.print("Using default!");
    volume = 40; // default volume 
    currentFrequency = defaultFrequency; // 103.9MHz
  }
  delay(2000);
  oled.clear();

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), rotaryEncoder, CHANGE);

  rx.setup(RESET_PIN, FM_FUNCTION);
  rx.setFM(8400, 10800, currentFrequency, 10);
  delay(300);
  rx.setVolume(volume);
  currentFrequency = previousFrequency = rx.getFrequency();
  showStatus();
}


void showSplash() {
  oled.setCursor(0, 0);
  oled.print("DEALING     WITH");
  oled.setCursor(0, 2);
  oled.print("EEPROM-By PU2CLR");
  delay(2000);  
}

inline void cleanDisplay() {
  oled.clear();
  oled.setCursor(0, 0);  
}

// Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus) {
    if ( encoderStatus == DIR_CW ) {
      encoderCount = 1;
    }
    else {
      encoderCount = -1;
    }
  }
}

/**
   Shows current frequency and volume
*/
void showStatus() {
  oled.setCursor(0, 0);
  oled.print("FM ");
  oled.setCursor(38, 0);
  oled.print("      ");
  oled.setCursor(38, 0);
  oled.print(currentFrequency / 100.0);
  oled.setCursor(95, 0);
  oled.print("MHz");
  oled.setCursor(0, 2);
  oled.print("VOL: ");
  oled.print(rx.getVolume());
}

/**
    Saves the current volume and frequency into the internal EEPROM
*/
void writeReceiverData() {
  EEPROM.write(eeprom_address, app_id); // stores the app id;
  EEPROM.write(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.write(eeprom_address + 2, (currentFrequency >> 8) );   // stores the current Frequency HIGH byte
  EEPROM.write(eeprom_address + 3, (currentFrequency & 0xFF));  // stores the current Frequency LOW byte
}

void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0  ) {

    if (encoderCount == 1 )
      rx.frequencyUp();
    else
      rx.frequencyDown();

    currentFrequency = rx.getFrequency();
    encoderCount = 0;
  } else if (digitalRead(VOLUME_UP) == LOW ) {
    rx.volumeUp();
    delay(50);
    showStatus();
  }
  else if (digitalRead(VOLUME_DOWN) == LOW ) {
    rx.volumeDown();
    delay(50);
    showStatus();
  }

  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
  }

  // Checks the shutdown status
  if (digitalRead(SHUTDOWN_DETECTOR) == LOW ) {
    writeReceiverData();
    while (1); // Stop working
  }

}
