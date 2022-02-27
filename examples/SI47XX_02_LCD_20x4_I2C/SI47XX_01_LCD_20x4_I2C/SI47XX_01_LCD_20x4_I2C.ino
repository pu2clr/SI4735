/*
  It is a simple  AM and FM radio.
  SI4735 Arduino Library example with LCD 20x4 I2C.
  It is a example that shows how to set up a LCD and SI4735 on the same I2C bus.
  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
  together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C).
  Look for LiquidCrystal I2C on Manager Libraries.


  ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:
  This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal
  pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit.
  If this occurs in your project, change the circuit to use external pull-up on pin 13.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.

  This sketch has been successfully tested on Pro Mini 3.3V.

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/schematic_basic_oled_SI47XX_02_ALL_IN_ONE_OLED.png

  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include <LiquidCrystal_I2C.h> // Version 1.1.4 by Frank de Brabander. Download this library on https://github.com/marcoschwartz/LiquidCrystal_I2C
#include "Rotary.h"

#define AM_FUNCTION 1
#define FM_FUNCTION 0

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define AM_FM_BUTTON 4      // AM/FM SWITCH
#define SEEK_BUTTON_UP 5    // Next Station
#define SEEK_BUTTON_DOWN 6 // Previous Station
#define VOL_UP 7           // Volume Volume Up
#define VOL_DOWN 8         // Volume Down

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();


// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = 0;


// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
LiquidCrystal_I2C lcd(0x27, 20, 4);  // please check the address of your I2C device
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

  lcd.init();

  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("SI4735");
  lcd.setCursor(2, 1);
  lcd.print("Arduino Library");
  lcd.setCursor(0, 3);
  lcd.print("By PU2CLR - Ricardo");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FM and AM (MW & SW)");
  lcd.setCursor(4, 2);
  lcd.print("Tuning Test");

  lcd.clear();

  
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  delay(500);

  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800,  10390, 10);

  delay(500);

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
void showStatus() {
  // Clear just the frequency information space.
  lcd.setCursor(7,0); 
  lcd.print("        ");
  
  lcd.setCursor(0, 0);
  if (si4735.isCurrentTuneFM() ) {
    lcd.print("FM ");
    lcd.setCursor(7, 0);
    lcd.print(String(currentFrequency / 100.0, 2));
    lcd.setCursor(17, 0);
    lcd.print("MHz ");
    showStereo();
  } else {
    lcd.print("AM ");
    lcd.setCursor(8, 0);
    lcd.print(currentFrequency);
    lcd.setCursor(17, 0);
    lcd.print("kHz");
    lcd.setCursor(0, 3);
    lcd.print("       ");
  }
}


/* *******************************
 * Shows RSSI status
 */
void showRSSI() {
  int blk;
  lcd.setCursor(8, 3);
  lcd.print("S:");

  blk = rssi / 10 + 1;

  for (int i = 0; i < 10; i++ ) {
    lcd.setCursor(10 + i, 3);
    lcd.print( (i < blk ) ? ">" : " ");
  }
}


/* *****************************
 * Shows Stereo or Mono status
 */
void showStereo() {

  lcd.setCursor(0, 3);
  lcd.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO  ");

}

/* ***************************
 * Shows the volume level on LCD
 */
void showVolume() {
  lcd.setCursor(8,2);
  lcd.print("VOLUME    ");
  lcd.setCursor(15,2);
  lcd.print(volume);
}


/*
 * Main
 */
void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0  ) {

    if (encoderCount == 1 )
      si4735.frequencyUp();
    else
      si4735.frequencyDown();

    currentFrequency = si4735.getFrequency();
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME )
  {
    // check if some button is pressed
    if (digitalRead(AM_FM_BUTTON) == LOW ) {
       // Switch AM to FM and vice-versa  
       if  (si4735.isCurrentTuneFM() ) 
          si4735.setAM(570, 1710,  810, 10);
       else   
       si4735.setFM(8600, 10800,  10390, 10);
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
  
    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showStatus();
  }

  // Show RSSI status only if this condition has changed
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

  // Show volume level only if this condition has changed
  if ( si4735.getCurrentVolume() != volume ) {
    volume = si4735.getCurrentVolume(); 
    showVolume();
  }
  
  delay(15);
}
