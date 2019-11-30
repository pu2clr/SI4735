/*
  SS4735 Arduino Library example with OLED I2C.
  Rotary Encoder: This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch.


  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "Rotary.h"
#include "patch_content.h"


#define AM_FUNCTION 1

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define BANDWIDTH_BUTTON 5 // Next Band
#define BAND_BUTTON_UP 8   // Seek Up
#define BAND_BUTTON_DOWN 9 // Seek Down
#define VOL_UP 6           // Volume Volume Up
#define VOL_DOWN 7         // Volume Down
#define BFO_SWITCH  13
// Seek Function

#define MIN_ELAPSED_TIME 100
#define LSB 1
#define USB 2

bool bfoOn = false;

int currentBFO = 0;
int previousBFO = 0;

const int size_content_full = sizeof ssb_patch_content_full;

long elapsedButton = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
unsigned currentFrequency;
unsigned previousFrequency;

byte bandwidthIdx = 1;
char *bandwitdth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


typedef struct {
  unsigned   minimumFreq;
  unsigned   maximumFreq;
  unsigned   currentFreq;
  unsigned   currentStep;
  byte       currentSSB; 
} Band;


Band band[] = {
  {3500, 4000, 3700, 1,LSB},
  {7000, 7500, 7100, 1,LSB},
  {14000, 14300, 14200, 1,USB},
  {210000, 21400, 21200, 1,USB},
  {27000, 27500, 27220, 1,USB},
  {28000, 28500, 28400, 1, USB}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 1;


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

  pinMode(BANDWIDTH_BUTTON, INPUT);
  pinMode(BAND_BUTTON_UP, INPUT);
  pinMode(BAND_BUTTON_DOWN, INPUT);
  pinMode(VOL_UP, INPUT);
  pinMode(VOL_DOWN, INPUT);
  pinMode(BFO_SWITCH, INPUT);

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

  loadSSB();

  delay(500);
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep, band[currentFreqIdx].currentSSB);
  delay(500);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(40);
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

  bandMode = String("SSB");
  unit = "KHz";
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
  display.print(String(bandwitdth[bandwidthIdx]));
  display.print(" KHz");
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


void showBFO() {

  display.setCursor(70, 4);
  display.print("         ");
  display.setCursor(70, 4);
  display.print("BFO: ");
  display.print(currentBFO);
  
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
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep, band[currentFreqIdx].currentSSB);

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
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep,  band[currentFreqIdx].currentSSB);
}


void loadSSB()
{
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  delay(500);
  si4735.patchPowerUp();
  delay(500);
  si4735.downloadPatch(ssb_patch_content_full, size_content_full);
  delay(500);
  si4735.setSsbConfig(3, 1, 0, 1, 0, 1);
  showStatus();
}

/*
   Main
*/
void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0)
  {

    if (bfoOn) {
      currentBFO = (encoderCount == 1) ?  (currentBFO + 50) : (currentBFO - 50);
    } else {
      if (encoderCount == 1)
        si4735.frequencyUp();
      else
        si4735.frequencyDown();
    }
    encoderCount = 0;
  }

  // Check button commands
  if (digitalRead(BANDWIDTH_BUTTON) | digitalRead(BAND_BUTTON_UP) | digitalRead(BAND_BUTTON_DOWN) | digitalRead(VOL_UP) | digitalRead(VOL_DOWN) | digitalRead(BFO_SWITCH))
  {

    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
    {
      bandwidthIdx++;
      if (bandwidthIdx > 6)  bandwidthIdx = 0;

      si4735.setBandwidth(bandwidthIdx, 0);
      showStatus();
    }
    else if (digitalRead(BAND_BUTTON_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      bandDown();
    else if (digitalRead(VOL_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.volumeUp();
    else if (digitalRead(VOL_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      si4735.volumeDown();
      else if (digitalRead(BFO_SWITCH) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME) {
        bfoOn = !bfoOn;
      }
    elapsedButton = millis();
  }


  // Show the current frequency only if it has changed
  currentFrequency = si4735.getFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
  }

  // Show RSSI status only if this condition has changed
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

  if (currentBFO != previousBFO ) {
    previousBFO = currentBFO;
    si4735.setSsbBfo(currentBFO);
    showBFO();
  }
  

  delay(50);
}
