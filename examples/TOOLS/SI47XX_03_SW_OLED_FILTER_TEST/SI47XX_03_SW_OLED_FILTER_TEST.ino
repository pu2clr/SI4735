/*
  SS4735 Arduino Library example with OLED I2C.
  Bandwidth filter test (11 bands SW Receiver with OLED)
  This example is a 11 bands SW receiver based on SI4735.
  It shows the selection of the bandwidth of the channel filter for AM reception (in this case SW).
  The choices are: 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz).

  This sketch has been successfully tested on:
    Arduino Pro Mini 3.3V;
    Arduino UNO (voltage converter);
    Arduino Micro (voltage converter);
    Arduino Yún (voltage converter); and
    Arduino Mega 2560 (voltage converter).

  This sketch DOES NOT COMPILE on Arduino DUE and ESP32 due to OLED libraries used here.

  Rotary Encoder: This sketch uses the Rotary Encoder Class implementation from Ben Buxton.
  The source code is included together with this sketch.


  ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:
  This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal
  pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit. 
  If this occurs in your project, change the circuit to use external pull-up on pin 13.     


  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/schematic_basic_oled_SI47XX_02_ALL_IN_ONE_OLED.png

  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "Rotary.h"

#define AM_FUNCTION 1

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define BANDWIDTH_BUTTON 5 // Bandwidth 
#define VOL_UP 6           // Volume Volume Up
#define VOL_DOWN 7         // Volume Down
#define BAND_BUTTON_UP 8   // Next Band (See band table)
#define BAND_BUTTON_DOWN 9 // Previous Band (See band table)

// Seek Function

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();
long elapsedStatus = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;

uint8_t bandwidthIdx = 1;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


typedef struct {
  uint16_t   minimumFreq;
  uint16_t   maximumFreq;
  uint16_t   currentFreq;
  uint8_t    currentStep;
} Band;


Band band[] = {{4600, 5200, 4850, 5},
  {5700, 6200, 6000, 5},
  {7000, 7500, 7200, 5},
  {9300, 10000, 9600, 5},
  {11400, 12200, 11940, 5},
  {13500, 13900, 13600, 5},
  {15000, 15800, 15400, 5},
  {17400, 17900, 17600, 5},
  {21400, 21800, 21500, 5},
  {27000, 27500, 27220, 1},
  {28000, 28500, 28400, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 2; // 41M


uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = 0;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

SSD1306AsciiAvrI2c display;

SI4735 si4735;

void setup()
{

  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  pinMode(BANDWIDTH_BUTTON, INPUT_PULLUP);
  pinMode(BAND_BUTTON_UP, INPUT_PULLUP);
  pinMode(BAND_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(VOL_UP, INPUT_PULLUP);
  pinMode(VOL_DOWN, INPUT_PULLUP);

  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  delay(500);

  // Splash - Change it for your introduction text.
  display.set1X();
  display.setCursor(0, 0);
  display.print("Si4735 Arduino Library");
  delay(500);
  display.setCursor(30, 3);
  display.print("By PU2CLR");
  delay(3000);
  display.clear();
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, AM_FUNCTION);

  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.

  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
  delay(200);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(50);

  showStatus();
}

// Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
  {
    if (encoderStatus == DIR_CW)
    {
      encoderCount = 1;
    }
    else
    {
      encoderCount = -1;
    }
  }
}

// Show current frequency
void showStatus()
{
  String unit, freqDisplay;
  String bandMode;

  bandMode = String("AM");
  unit = "kHz";
  freqDisplay = String(currentFrequency);

  display.set1X();
  display.setCursor(0, 0);
  display.print(String(bandMode));

  display.setCursor(98, 0);
  display.print(unit);

  display.set2X();
  display.setCursor(26, 1);
  display.print("        ");
  display.setCursor(26, 1);
  display.print(freqDisplay);

  si4735.getAutomaticGainControl();

  // Show AGC Information
  display.set1X();
  display.setCursor(0, 4);
  display.print((si4735.isAgcEnabled()) ? "AGC ON" : "AGC OFF");
  display.setCursor(0, 5);
  display.print("G.:");
  display.print(si4735.getAgcGainIndex());

  display.setCursor(0, 7);
  display.print("            ");
  display.setCursor(0, 7);
  display.print("BW: ");
  display.print(String(bandwidth[bandwidthIdx]));
  display.print(" kHz");
}

/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
  int blk;

  display.set1X();
  display.setCursor(70, 7);
  display.print("S:");
  display.print(rssi);
  display.print(" dBuV");
}

/* ***************************
   Shows the volume level on LCD
*/
void showVolume()
{
  display.set1X();
  display.setCursor(70, 5);
  display.print("          ");
  display.setCursor(70, 5);
  display.print("V:");
  display.print(volume);
}


void bandUp() {

  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if ( currentFreqIdx < lastBand ) {
    currentFreqIdx++;
  } else {
    currentFreqIdx = 0;
  }

  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);

}

void bandDown() {
  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if ( currentFreqIdx > 0 ) {
    currentFreqIdx--;
  } else {
    currentFreqIdx = lastBand;
  }
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
}

void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0)
  {

    if (encoderCount == 1)
      si4735.frequencyUp();
    else
      si4735.frequencyDown();
    delay(15);
    currentFrequency = si4735.getFrequency();
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME )
  {

    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == LOW )
    {
      bandwidthIdx++;
      if (bandwidthIdx > 6)  bandwidthIdx = 0;

      si4735.setBandwidth(bandwidthIdx, 0);
      showStatus();
    }
    else if (digitalRead(BAND_BUTTON_UP) == LOW ) {
      bandUp();
      delay(15);
      currentFrequency = si4735.getFrequency();
    }
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW ) {
      bandDown();
      delay(15);
      currentFrequency = si4735.getFrequency();
    }
    else if (digitalRead(VOL_UP) == LOW )
      si4735.volumeUp();
    else if (digitalRead(VOL_DOWN) == LOW )
      si4735.volumeDown();

    delay(35);
    elapsedButton = millis();
  }


  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
  }

  if ( (millis() - elapsedStatus) > (4 * MIN_ELAPSED_TIME) ) {
    // Show RSSI status only if this condition has changed
    si4735.getCurrentReceivedSignalQuality();
    if (rssi != si4735.getCurrentRSSI())
    {
      rssi = si4735.getCurrentRSSI();
      showRSSI();
    }

    // Show volume level only if this condition has changed
    if (si4735.getCurrentVolume() != volume)
    {
      volume = si4735.getCurrentVolume();
      showVolume();
    }
    elapsedStatus = millis();
  }

  delay(15);
}
