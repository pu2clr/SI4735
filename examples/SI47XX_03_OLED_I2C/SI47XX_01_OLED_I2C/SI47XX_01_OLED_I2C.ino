/*
  SI4735 Arduino Library example with OLED  I2C.
  It is AM and FM radio.
  Rotary Encoder: This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch.

  ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:
  This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on.
  If you use the internal pull-up resistor of the pin 13, you might experiment problem due to the drop voltage
  caused by the LED circuit. If this occurs in your project you can do:
  1. use the pin 14. This pin is the A0 (Analog). But you have to refer it by 14 to use it as a digital pin (just change 13 by 14 on the sketch examples);
  2. change the circuit and sketch to use external pull-up on pin 13;
  3. remove the LED or resitor connected to the led from the board (caution).

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.


  This sketch has been successfully tested on Pro Mini 3.3V.

  SI473X bases device  and Arduino Atmega328 based device wire up.

  | Device name               | Device Pin / Description  |  Arduino Pin  |
  | ----------------          | --------------------      | ------------  |
  | Display OLED              |                           |               |
  |                           | SDA                       |     A4        |
  |                           | CLK                       |     A5        |
  |     Si4735                |                           |               |
  |                           | RESET (pin 15)            |     12        |
  |                           | SDIO (pin 18)             |     A4        |
  |                           | SCLK (pin 17)             |     A5        |
  |     Buttons               |                           |               |
  |                           | AM/FM                     |      4        |
  |                           | Seek Up                   |      6        |
  |                           | Seek Down                 |      7        |
  |                           | Volume Up                 |      8        |
  |                           | Volume Down               |      9        |
  |    Encoder                |                           |               |
  |                           | A                         |      2        |
  |                           | B                         |      3        |


  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/schematic_basic_oled_SI47XX_02_ALL_IN_ONE_OLED.png


  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "Rotary.h"

#define AM_FUNCTION 1
#define FM_FUNCTION 0

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define AM_FM_BUTTON 4      // AM/FM SWITCH
#define SEEK_BUTTON_UP 6    // Seek Up
#define SEEK_BUTTON_DOWN 7  // Seek Down
#define VOL_UP 8            // Volume Volume Up
#define VOL_DOWN 9          // Volume Down
// Seek Function

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();
long elapsedRSSI =  millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint16_t lastAmFrequency = 810;     // Starts AM on 810kHz;
uint16_t lastFmFrequency = 10390;   // Starts FM on 103,9MHz
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
  pinMode(AM_FM_BUTTON, INPUT_PULLUP);
  pinMode(SEEK_BUTTON_UP, INPUT_PULLUP);
  pinMode(SEEK_BUTTON_DOWN, INPUT_PULLUP);
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

  si4735.getDeviceI2CAddress(RESET_PIN); // Looks for the Si47XX I2C bus address and set it.
  
  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800,  lastFmFrequency, 10);
  delay(200);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(35);
  showStatus();
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

// Show current frequency
void showFrequency() {
  String freqDisplay;

  if ( si4735.isCurrentTuneFM())
  {
    // Formatting the frequency to show on display
    freqDisplay = String((currentFrequency / 100.0), 1);
    freqDisplay.replace(".", ",");
  }
  else
  {
    freqDisplay = String(currentFrequency);
  }

  display.set2X();
  display.setCursor(26, 1);
  display.print("        ");
  display.setCursor(26, 1);
  display.print(freqDisplay);
  display.set1X();
}

void showStatus()
{
  display.set1X();
  display.setCursor(0, 0);
  display.print( (si4735.isCurrentTuneFM()) ? "FM" : "AM" );
  display.setCursor(98, 0);
  display.print((si4735.isCurrentTuneFM()) ? "MHz" : "kHz");

  // Show AGC Information
  si4735.getAutomaticGainControl();
  display.set1X();
  display.setCursor(5, 4);
  display.print((si4735.isAgcEnabled()) ? "AGC ON" : "AGC OFF" );
  display.setCursor(5, 5);
  display.print("G.:");
  display.setCursor( 27, 5);
  display.print(si4735.getAgcGainIndex());

  showFrequency();
  showRSSI();
  showStereo();
  showVolume();
}

/* *******************************
   Shows RSSI status
*/
void showRSSI() {
  int blk = rssi / 10;
  display.set1X();
  display.setCursor(70, 6);
  display.print("S:");
  display.print(rssi);
  display.print(" dBuV");
  display.setCursor(70, 7);
  for (uint8_t i = 0; i < 10; i++)  {
    if ( i < blk )
      display.print("#");
    else
      display.print(" ");
  }
}

/* *****************************
   Shows Stereo or Mono status
*/
void showStereo() {
  display.set1X();
  display.setCursor(5, 7);
  display.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO  ");
}

/* ***************************
   Shows the volume level on LCD
*/
void showVolume() {
  display.set1X();
  display.setCursor(70, 5);
  display.print("          ");
  display.setCursor(70, 5);
  display.print("V:");
  display.print(volume);
}

void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0  ) {

    if (encoderCount == 1 )
      si4735.frequencyUp();
    else
      si4735.frequencyDown();

    // Show the current frequency only if it has changed
    currentFrequency = si4735.getFrequency();
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME )
  {
    // check if some button is pressed
    if (digitalRead(AM_FM_BUTTON) == LOW ) {
      // Switch AM to FM and vice-versa
      if  (si4735.isCurrentTuneFM() ) {
        lastFmFrequency = currentFrequency;
        si4735.setAM(570, 1710,  lastAmFrequency, 10);
        delay(15);
        currentFrequency = si4735.getFrequency();
      }
      else {
        lastAmFrequency = currentFrequency;
        si4735.setFM(8600, 10800,  lastFmFrequency, 10);
        delay(15);
        currentFrequency = si4735.getFrequency();
      }
      // Uncoment the line below if you want to disable AGC
      // si4735.setAutomaticGainControl(1,0);
      display.clear();
      showStatus();
    }
    else if (digitalRead(SEEK_BUTTON_UP) == LOW ) {
      si4735.seekStationUp();
      currentFrequency = si4735.getFrequency();
    }
    else if (digitalRead(SEEK_BUTTON_DOWN) == LOW ) {
      si4735.seekStationDown();
      currentFrequency = si4735.getFrequency();
    }
    else if (digitalRead(VOL_UP) == LOW )
      si4735.volumeUp();
    else if (digitalRead(VOL_DOWN) == LOW )
      si4735.volumeDown();

    delay(35);
    elapsedButton = millis();
  }

  // Checks if currentFrequency changed. 
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showFrequency();
  }

  // Show RSSI status only if this condition has changed
  if ( (millis() - elapsedRSSI) > MIN_ELAPSED_TIME * 8) {
    si4735.getCurrentReceivedSignalQuality();
    if ( rssi != si4735.getCurrentRSSI() ) {
      rssi = si4735.getCurrentRSSI();
      showRSSI();
    }

    // Show stereo status only if this condition has changed
    if (si4735.isCurrentTuneFM() ) {
      // Show stereo status only if this condition has changed
      if ( stereo != si4735.getCurrentPilot() ) {
        stereo = si4735.getCurrentPilot();
        showStereo();
      }
    }
    elapsedRSSI =  millis();
  }

  // Show volume level only if this condition has changed
  if ( si4735.getCurrentVolume() != volume ) {
    volume = si4735.getCurrentVolume();
    showVolume();
  }

  delay(15);
}
