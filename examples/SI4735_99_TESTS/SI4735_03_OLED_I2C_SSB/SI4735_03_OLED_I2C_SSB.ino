/*
 * This sketch applies patches on Si4735 devices.
 * In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
 *
 * There is little information available about patching the SI4735. The following information is the understanding
 * of the author.
 * 
 * A SI4735 patches are executed internally (run by internal MCU) of the device. Usually, patches are  used to fixes bugs
 * or add improvements and  new features of the firmware installed in the internal ROM of the device.
 * Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device
 * by the host MCU (in this case Arduino).
 * Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
 * Consequently, the content of the patch has to be transferred again to the device each time after turn on the
 * system or reset the device.
 * 
 * ATTENTION:
 * The author of this sketch does not guarantee that this procedure will work in your development environment.
 * Given this, it is at your own risk to continue with the procedures suggested here.
 * This sketch works with the I2C communication protocol and is designed to apply a SSB extension PATCH to CI
 * SI4735-D60 every time you switch to SSB mode. Once again, the author disclaims any liability for any damage 
 * this procedure may cause to your SI4735 or other devices that you are using. 
 * 
 * By Ricardo Lima Caratti, Nov 2019
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

// Button controllers
#define VOL_UP 6              // Volume Volume Up
#define VOL_DOWN 7            // Volume Down
#define BANDWIDTH_BUTTON 5    // Next Band
#define BAND_BUTTON_UP 8      // Band Up
#define BAND_BUTTON_DOWN 9    // Band Down
#define MODE_SWITCH_BUTTON 10 // Switch AM/SSB
#define FM_SWITCH_BUTTON 11   // Switch FM / (AM/SSB)

#define MIN_ELAPSED_TIME 100

#define MODE_AM 0
#define MODE_LSB 1
#define MODE_USB 2


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
  unsigned minimumFreq; // Minimum frequency to the band
  unsigned maximumFreq; // Maximum frequency to the band
  unsigned currentFreq; // Used to store the current band before you switch to another band 
  unsigned currentStep; // Used to store the corrent step 
  unsigned defaultMode; // 
} Band;

// SW band table
Band band[] = {
  {4600, 5200, 4850, 5},
  {5700, 6200, 6000, 5},
  {7000, 7500, 7100, 5},
  {9300, 10000, 9600, 5},
  {11400, 12200, 11940, 5},
  {13500, 13900, 13600, 5},
  {14000} 14400, 14200, 1}, // 20 meters
  {15000, 15800, 15400, 5},
  {17400, 17900, 17600, 5},
  {21400, 21800, 21500, 5},
  {27000, 27500, 27220, 1},
  {28000, 28500, 28400, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 2; // 41M


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

  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(60);

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

/*
 * Patches to the SI4735 are transferred to the internal RAM of the device.
 * All content of the patch will get lost when you switch to another mode like AM or FM. 
 * So, the content of the patch has to be transferred again to the device each time you select
 * SSB mode.
 */
void loadSSB() {
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  si4735.patchPowerUp();
  si4735.downloadPatch(ssb_patch_content_full, size_content_full);
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

    encoderCount = 0;
  }

  // Check button commands
  if (digitalRead(BANDWIDTH_BUTTON) | digitalRead(BAND_BUTTON_UP) | digitalRead(BAND_BUTTON_DOWN) | digitalRead(VOL_UP) | digitalRead(VOL_DOWN))
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

  delay(50);
}
