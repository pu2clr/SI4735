/*
  This example tests two display (a OLED and a LCD 20x4) sharing the same I2C bus with SI4735 device.
  It is a 12 bands SW radio.

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
  together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C).
  Look for LiquidCrystal I2C on Manager Libraries.
  You also need to install Adafruit_GFX  (https://github.com/adafruit/Adafruit-GFX-Library) and
  Adafruit_SH1106 (https://github.com/wonho-maker/Adafruit_SH1106).


  ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:
  This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal
  pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit.
  If this occurs in your project, change the circuit to use external pull-up on pin 13.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.


  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_internal_pullup_i2c.png

  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include <LiquidCrystal_I2C.h> // Version 1.1.4 by Frank de Brabander. Download this library on https://github.com/marcoschwartz/LiquidCrystal_I2C
#include <Adafruit_GFX.h>    // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SH1106.h> // https://github.com/wonho-maker/Adafruit_SH1106
#include "Rotary.h"
#include "smeter.h"


#define AM_FUNCTION 1

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.


#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define BANDWIDTH_BUTTON 5 // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define VOL_UP 6           // Volume Up
#define VOL_DOWN 7         // Volume Down
#define BAND_BUTTON_UP 8   // Next band
#define BAND_BUTTON_DOWN 9 // Previous band
#define AGC_SWITCH 11      // Switch AGC ON/OF
#define STEP_SWITCH 10     // Used to select the increment or decrement frequency step (1, 5 or 10 kHz)

// Seek Function

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 120
#define LSB 1
#define USB 2

bool disableAgc = true;
bool agc_en = true;


long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentStep = 5;

uint8_t bandwidthIdx = 2;
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
  {24890, 25000, 24940, 1},
  {26000, 27700, 27300, 1},
  {28000, 28500, 28400, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 2;

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = 0;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
LiquidCrystal_I2C lcd(0x27, 20, 4); // please check the address of your I2C device
Adafruit_SH1106 display(RST_PIN);
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
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);


  display.begin(SH1106_SWITCHCAPVCC, 0x3C);                // needed for SH1106 display
  display.clearDisplay();                                  // clears display from any library info displayed
  display.display();


  lcd.init();

  delay(500);

  // Splash - Change it for your introduction text.
  lcd.backlight();
  lcd.setCursor(7, 0);
  lcd.print("SI4735");
  lcd.setCursor(2, 1);
  lcd.print("Arduino Library");
  delay(500);
  lcd.setCursor(4, 2);
  lcd.print("S-METER TEST");
  delay(500);
  lcd.setCursor(5, 3);
  lcd.print("By PU2CLR");
  delay(2000);
  lcd.clear();
  // end Splash
  
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, AM_FUNCTION);
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
  delay(100);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  // Enable AGC and set it for Minimum attenuation
  si4735.setAutomaticGainControl(0, 0);

  showStatus();
  showSmeter(rssi);
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

void showFrequency() {

  String freqDisplay;
  freqDisplay = String((float) currentFrequency / 1000, 3);

  lcd.setCursor(7, 0);
  lcd.print("        ");
  lcd.setCursor(7, 0);
  lcd.print(freqDisplay);

}

void showStatus()
{
  lcd.setCursor(0, 0);
  lcd.print(String("SW"));

  lcd.setCursor(16, 0);
  lcd.print("kHz");

  // Show AGC Information
  si4735.getAutomaticGainControl();
  lcd.setCursor(0, 1);
  lcd.print((si4735.isAgcEnabled()) ? "AGC ON " : "AGC OFF");

  lcd.setCursor(12, 1);
  lcd.print("        ");
  lcd.setCursor(12, 1);
  lcd.print("St:");
  lcd.print(currentStep);
  lcd.print("kHz");

  lcd.setCursor(0, 3);
  lcd.print("           ");
  lcd.setCursor(0, 3);
  lcd.print("BW:");
  lcd.print(String(bandwidth[bandwidthIdx]));
  lcd.print("kHz");

  showSmeter(rssi);
  showFrequency();

}


/* ***************************
   Shows the volume level on LCD
*/
void showVolume()
{
  lcd.setCursor(14, 3);
  lcd.print("     ");  
  lcd.setCursor(14, 3);
  lcd.print("V:");
  lcd.print(volume);
}


void bandUp()
{
  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if (currentFreqIdx < lastBand)
  {
    currentFreqIdx++;
  }
  else
  {
    currentFreqIdx = 0;
  }

  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
  si4735.setAutomaticGainControl(disableAgc, 0 /* Minimum attenuation */);
  showStatus();
}

void bandDown()
{
  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if (currentFreqIdx > 0)
  {
    currentFreqIdx--;
  }
  else
  {
    currentFreqIdx = lastBand;
  }
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
  si4735.setAutomaticGainControl(disableAgc, 0 /* Minimum attenuation */);
  showStatus();
}




/*
   OLED Analog S-Meter
   Draws the s meter and displays the value.
*/
void showSmeter(uint16_t signalLevel)
{
  static uint8_t buffer[20]; 
  static uint16_t sample; 
  static uint8_t idx = 0;
  static bool isFull = false;
  const int hMeter = 65; // horizontal center for needle animation
  const int vMeter = 85; // vertical center for needle animation (outside of dislay limits)
  const int rMeter = 80;

  // makes needle movement smoother
  if ( idx < 20 && !isFull ) {
    buffer[idx] = signalLevel;
    idx++;
    isFull = true;
    return;
  }
  if ( idx >= 20 ) idx = 0;
  buffer[idx] = signalLevel;
  // always get the average of the last 20 readings
  for (int i = sample = 0; i < 20; i++)
    sample += buffer[idx];

  // Draw the S Meter with the average value
  signalLevel = map((sample / 20), 0, 127, 0, 1023);
  float smeterValue = (signalLevel) * 330 / 1024; // convert the signal value to arrow information

  smeterValue = smeterValue - 34;                           // shifts needle to zero position
  display.clearDisplay();                                   // refresh display for next step
  display.drawBitmap(0, 0, S_Meter, 128, 64, WHITE);        // draws background
  int a1 = (hMeter + (sin(smeterValue / 57.296) * rMeter)); // meter needle horizontal coordinate
  int a2 = (vMeter - (cos(smeterValue / 57.296) * rMeter)); // meter needle vertical coordinate
  display.drawLine(a1, a2, hMeter, vMeter, WHITE);          // draws needle
  display.display();
  idx = 0;
  sample = 0; 
}




/*
   Main
*/
void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (encoderCount == 1)
      si4735.frequencyUp();
    else
      si4735.frequencyDown();

    // Show the current frequency only if it has changed
    currentFrequency = si4735.getFrequency();

    encoderCount = 0;
  }

  // Check button commands
  if ( (millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
    {
      bandwidthIdx++;
      if (bandwidthIdx > 6)
        bandwidthIdx = 0;

      si4735.setBandwidth(bandwidthIdx, 0);
      showStatus();
    }
    else if (digitalRead(BAND_BUTTON_UP) == LOW)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
      bandDown();
    else if (digitalRead(VOL_UP) == LOW)
      si4735.volumeUp();
    else if (digitalRead(VOL_DOWN) == LOW)
      si4735.volumeDown();
    else if ( digitalRead(AGC_SWITCH) == LOW) {
      disableAgc = !disableAgc;
      // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
      si4735.setAutomaticGainControl(disableAgc, 0 /* Minimum attenuation */);
      showStatus();
    } else if ( digitalRead(STEP_SWITCH) == LOW) {
      if (currentStep == 1)
        currentStep = 5;
      else if ( currentStep == 5)
        currentStep = 10;
      else
        currentStep = 1;
      si4735.setFrequencyStep(currentStep);
      band[currentFreqIdx].currentStep = currentStep;
      showStatus();
    } else if ( digitalRead(AGC_SWITCH) == LOW ) {
      agc_en = !agc_en;
      si4735.setAutomaticGainControl(!agc_en,0);
    }
    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  if ( (millis() - elapsedFrequency) > MIN_ELAPSED_RSSI_TIME * 2) {
    currentFrequency = si4735.getFrequency();
    if (currentFrequency != previousFrequency)
    {
      previousFrequency = currentFrequency;
      showFrequency();
    }
    elapsedFrequency = millis();
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME ) {
    si4735.getCurrentReceivedSignalQuality();
    if (rssi != si4735.getCurrentRSSI())
    {
      rssi = si4735.getCurrentRSSI();
      showSmeter(rssi);
    }
    elapsedRSSI = millis();
  }

  // Show volume level only if this condition has changed
  if (si4735.getCurrentVolume() != volume)
  {
    volume = si4735.getCurrentVolume();
    showVolume();
  }


  delay(50);
}
