/*

  The Atmega8 has the same Atmega328 (Arduino Nano, Uno and Pro Mini) pinout.
  However, it has memory limitation.



  ATmega8, SI4735-D60 and encoder wire up. 

  | Device name               | Device Pin / Description      |  Atmega8/Pin  |
  | ----------------          | ----------------------------- | ------------  |
  |    LCD 16x2 or 20x4       |                               |               |
  |                           | D4                            |     D7        |
  |                           | D5                            |     D6        |
  |                           | D6                            |     D5        |
  |                           | D7                            |     D4        |
  |                           | RS                            |     D12       |
  |                           | E/ENA                         |     D13       |
  |                           | RW & VSS & K (16)             |    GND        |
  |                           | A (15) & VDD                  |    +Vcc       |
  |                           | VO (see 20K tripot connection)|   ---------   |
  |     SS473X                |                               |               |
  |                           | RESET (pin 15)                |      9        |
  |                           | SDIO (pin 18)                 |     A4        |
  |                           | SCLK (pin 17)                 |     A5        |
  |                           | (*1)SEN (pin 16)              |  +Vcc or GND  |
  |    Encoder                |                               |               |
  |                           | A                             |       2       |
  |                           | B                             |       3       |
  |                           | PUSH BUTTON (encoder)         |     A0/14     |

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Dez 2021.
*/

#include <SI4735.h>
#include <LiquidCrystal.h>
#include "Rotary.h"

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
#define RESET_PIN 9
#define ENCODER_PUSH_BUTTON 14 // Pin A0/14


#define AM_FM_BUTTON 1      // AM/FM SWITCH
#define FM_FUNCTION 0

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 4
#define LCD_D6 5
#define LCD_D5 6
#define LCD_D4 7
#define LCD_RS 12
#define LCD_E 13

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define FM 0
#define AM 3
#define LW 4

uint16_t currentFrequency;

volatile int encoderCount = 0;
char lastEncoderMove = 0;
uint8_t currentStep = 1;
uint8_t currentMode = FM;

long lastQuery = millis();

/*
   Band data structure
*/
typedef struct
{
  uint8_t bandType;     // Band type (FM, MW or SW)
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint16_t currentStep; // Defeult step (increment and decrement)
} Band;


/*
   Band table
*/
Band band[] = {
  {FM_BAND_TYPE, 8400, 10800, 10390, 10},
  {LW_BAND_TYPE, 100, 510, 300, 1},
  {MW_BAND_TYPE, 520, 1720, 810, 10},
  {SW_BAND_TYPE, 1800, 3500, 1900, 1}, // 160 meters
  {SW_BAND_TYPE, 3500, 4500, 3700, 1}, // 80 meters
  {SW_BAND_TYPE, 4500, 5500, 4850, 5},
  {SW_BAND_TYPE, 5600, 6300, 6000, 5},
  {SW_BAND_TYPE, 6800, 7800, 7200, 5}, // 40 meters
  {SW_BAND_TYPE, 9200, 10000, 9600, 5},
  {SW_BAND_TYPE, 10000, 11000, 10100, 1}, // 30 meters
  {SW_BAND_TYPE, 11200, 12500, 11940, 5},
  {SW_BAND_TYPE, 13400, 13900, 13600, 5},
  {SW_BAND_TYPE, 14000, 14500, 14200, 1}, // 20 meters
  {SW_BAND_TYPE, 15000, 15900, 15300, 5},
  {SW_BAND_TYPE, 17200, 17900, 17600, 5},
  {SW_BAND_TYPE, 18000, 18300, 18100, 1},  // 17 meters
  {SW_BAND_TYPE, 21000, 21900, 21200, 1},  // 15 mters
  {SW_BAND_TYPE, 24890, 26200, 24940, 1},  // 12 meters
  {SW_BAND_TYPE, 26200, 27900, 27500, 1},  // CB band (11 meters)
  {SW_BAND_TYPE, 28000, 30000, 28400, 1}
}; // 10 meters

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

SI4735 rx;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

void setup()
{
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  lcd.begin(16, 2);
  // Splash - Remove or Change the splash message
  lcd.setCursor(0, 0);
  lcd.print("SI4735-ATMEGA8");
  lcd.setCursor(0, 1);
  lcd.print("BY PU2CLR");

  // controlling encoder via interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  rx.setup(RESET_PIN, FM_FUNCTION);
  rx.setVolume(45);
  useBand();
}


/**
   Reads encoder via interrupt
   Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
   if you do not add ICACHE_RAM_ATTR declaration, the system will reboot during attachInterrupt call.
   With ICACHE_RAM_ATTR macro you put the function on the RAM.
*/
void  rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
    encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
}

/*
    Shows the currend frequency
*/
void showStatus() {

}

/*
   Show the current Signal Quality
*/
void showSignalQuality() {
  rx.getCurrentReceivedSignalQuality();
  lcd.setCursor(0, 1);

  if ( rx.isCurrentTuneFM() )
    lcd.print((rx.getCurrentPilot()) ? "ST" : "MO");
  else
    lcd.println("MW");

  lcd.setCursor(5, 1);
  lcd.print("S:");
  lcd.setCursor(7, 1);
  lcd.print("         ");
  lcd.setCursor(7, 1);
  lcd.print(rx.getCurrentRSSI());
  lcd.print("dBuV");
}


/**
    Shows frequency information on Display
*/
void showFrequency()
{
  char bufferDisplay[9];

  lcd.setCursor(0, 0);
  if (rx.isCurrentTuneFM())
  {
    rx.convertToChar(currentFrequency, bufferDisplay, 5, 3, ',');
    lcd.print("FM ");
    lcd.setCursor(13, 0);
    lcd.print("MHz");
  }
  else
  {
    rx.convertToChar(currentFrequency, bufferDisplay, 5, 0, '.');
    lcd.print("AM ");
    lcd.setCursor(13, 0);
    lcd.print("kHz");
  }
  lcd.setCursor(5, 0);
  lcd.print(currentFrequency);

}

/*
   Goes to the next band (see Band table)
*/
void bandUp()
{
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;

  if (bandIdx < lastBand)
  {
    bandIdx++;
  }
  else
  {
    bandIdx = 0;
  }
  useBand();
}

/*
   Goes to the previous band (see Band table)
*/
void bandDown()
{
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;
  if (bandIdx > 0)
  {
    bandIdx--;
  }
  else
  {
    bandIdx = lastBand;
  }
  useBand();
}

/*
   Switch the radio to current band.
   The bandIdx variable points to the current band. 
   This function change to the band referenced by bandIdx (see table band).
*/
void useBand()
{
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    rx.setTuneFrequencyAntennaCapacitor(0);
    rx.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      rx.setTuneFrequencyAntennaCapacitor(0);
    else 

      rx.setTuneFrequencyAntennaCapacitor(1);
      currentMode = AM;
      rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      rx.setAutomaticGainControl(1, 0);
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  showStatus();
}


void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if ( (lastEncoderMove = encoderCount) == 1)
      rx.frequencyUp();
    else
      rx.frequencyDown();

    // Show the current frequency only if it has changed
    currentFrequency = rx.getFrequency();
    showFrequency();
    encoderCount = 0;
  }
  // Check encoder push button
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW) {
    if ( lastEncoderMove == 1) { 
      // go to next band 
      bandUp();
    } else {
      // go to previous band
      bandDown();
    }
  }
  // TO DO
  //  Step control
  //  showStatus
  //  Add buttons
  //  Seek function
     
  delay(5);
}
