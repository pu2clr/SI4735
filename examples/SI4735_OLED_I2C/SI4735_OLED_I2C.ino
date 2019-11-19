/*
  SS4735 Arduino Library example with LCD 20x4 I2C.
  Rotary Encoder: This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch.
 

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
#define AM_FM_BUTTON 5      // Next Band
#define SEEK_BUTTON_UP 8    // Seek Up
#define SEEK_BUTTON_DOWN 9  // Seek Down
#define VOL_UP 7            // Volume Volume Up
#define VOL_DOWN 6          // Volume Down
// Seek Function

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();


// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
unsigned currentFrequency;
unsigned previousFrequency;
unsigned lastAmFrequency = 810;     // Starts AM on 810KHz;
unsigned lastFmFrequency = 10390;   // Starts FM on 103,9MHz
byte rssi = 0;
byte stereo = 1;
byte volume = 0;


// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

SSD1306AsciiAvrI2c display;

SI4735 si4735;

void setup()
{

  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  pinMode(AM_FM_BUTTON, INPUT);
  pinMode(SEEK_BUTTON_UP, INPUT);
  pinMode(SEEK_BUTTON_DOWN, INPUT);
  pinMode(VOL_UP, INPUT);
  pinMode(VOL_DOWN, INPUT);

  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  delay(500);

  // Splash - Change it for your introduction text.
  display.set1X();
  display.setCursor(0,0);
  display.print("Si4735 Arduino Library");
  delay(500);
  display.setCursor(30,3);
  display.print("By PU2CLR");
  delay(3000);
  display.clear();
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800,  lastFmFrequency, 10);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);

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
void showStatus()
{
  String unit, freqDisplay;
  String bandMode;

  if ( si4735.isCurrentTuneFM())
  {
    bandMode = String("FM");
    unit = "MHz";
    // Formatting the frequency to show on display
    freqDisplay = String((currentFrequency / 100.0), 1);
    freqDisplay.replace(".", ",");
  }
  else
  {
    bandMode = String("AM");
    unit = "KHz";
    freqDisplay = String(currentFrequency);
  }

  display.set1X();
  display.setCursor(0, 0);
  display.print(String(bandMode));

  display.setCursor(98,0);
  display.print(unit);

  display.set2X();
  display.setCursor(26, 1);
  display.print("        ");  
  display.setCursor(26, 1);
  display.print(freqDisplay);

  
  // Show AGC Information
  si4735.getAutomaticGainControl();
  display.set1X();
  display.setCursor(5,4);
  display.print((si4735.isAgcEnabled())?"AGC ON" : "AGC OFF" );
  display.setCursor(5,5);
  display.print("G.:");
  display.setCursor( 27,5);
  display.print(si4735.getAgcGainIndex());

}


/* *******************************
   Shows RSSI status
*/
void showRSSI() {
  int blk;

  display.set1X();
  display.setCursor(70, 6);
  display.print("S:");
  display.print(rssi);
  display.print(" dBuV");
}


/* *****************************
   Shows Stereo or Mono status
*/
void showStereo() {

  display.set1X();
  display.setCursor(5, 6);
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


/*
   Main
*/
void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0  ) {

    if (encoderCount == 1 )
      si4735.frequencyUp();
    else
      si4735.frequencyDown();

    encoderCount = 0;
  }

  // Check button commands
  if (digitalRead(AM_FM_BUTTON) | digitalRead(SEEK_BUTTON_UP) |  digitalRead(SEEK_BUTTON_DOWN) | digitalRead(VOL_UP) | digitalRead(VOL_DOWN) )
  {
    // check if some button is pressed
    if (digitalRead(AM_FM_BUTTON) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME) {
      // Switch AM to FM and vice-versa
      if  (si4735.isCurrentTuneFM() ) {
        lastFmFrequency = currentFrequency;
        si4735.setAM(570, 1710,  lastAmFrequency, 10);
      }
      else {
        lastAmFrequency = currentFrequency;
        si4735.setFM(8600, 10800,  lastFmFrequency, 10);
      }
      // Uncoment the line below if you want to disable AGC
      // si4735.setAutomaticGainControl(1,0);      
    }
    else if (digitalRead(SEEK_BUTTON_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.seekStationUp();
    else if (digitalRead(SEEK_BUTTON_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.seekStationDown();
    else if (digitalRead(VOL_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.volumeUp();
    else if (digitalRead(VOL_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.volumeDown();

    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  currentFrequency = si4735.getFrequency();
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showStatus();
  }

  // Show RSSI status only if this condition has changed
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

  // Show volume level only if this condition has changed
  if ( si4735.getCurrentVolume() != volume ) {
    volume = si4735.getCurrentVolume();
    showVolume();
  }

  delay(5);
}
