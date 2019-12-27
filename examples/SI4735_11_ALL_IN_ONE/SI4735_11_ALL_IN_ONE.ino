/*
  
  Attention: under construction.....
  
  SI4735 all in one with SSB Support 
  
  This sketch was tested on Arduino Pro Mini 3.3V
  This sketch uses LiquidCrystal/LCD with I2C and Encoder.

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included 
  together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C). 
  Look for LiquidCrystal I2C on Manager Libraries.   

  This sketch will download a SSB patch to your SI4735 device (patch_content.h). It will take about 8KB of the Arduino memory.

  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
  There is little information available about patching the SI4735. The following information is the understanding of the author of
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by
  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
  Given this, it is at your own risk to continue with the procedures suggested here.
  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.

  Features of this sketch:

  1) Only SSB (LSB and USB);
  2) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
  3) 22 commercial and ham radio bands pre configured;
  4) BFO Control; and
  5) Frequency step switch (1, 5 and 10KHz);

  Main Parts:
  Encoder with push button;
  Seven bush buttons;
  OLED Display with I2C protocol;
  Arduino Pro mini 3.3V;

  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include <LiquidCrystal_I2C.h>
#include "Rotary.h"

// Test it with patch_init.h or patch_full.h. Do not try load both.
#include "patch_init.h" // SSB patch for whole SSBRX initialization string
// #include "patch_full.h"    // SSB patch for whole SSBRX full download

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define AVC_SWITCH 4       // Switch SSB Automatic Volume Control ON/OFF
#define BANDWIDTH_BUTTON 5 // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz
#define VOL_UP 6           // Volume Up
#define VOL_DOWN 7         // Volume Down
#define BAND_BUTTON_UP 8   // Next band
#define BAND_BUTTON_DOWN 9 // Previous band
#define AGC_SWITCH 11      // Switch AGC ON/OF
#define STEP_SWITCH 10     // Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
#define BFO_SWITCH 13      // Used to select the enconder control (BFO or VFO)
// Seek Function

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150

#define FM 0
#define LSB 1
#define USB 2
#define AM 3

bool bfoOn = false;
bool disableAgc = true;
bool avc_en = true;

int currentBFO = 0;
int previousBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentStep = 1;
uint8_t currentBFOStep = 50;

uint8_t bandwidthIdx = 2;
char *bandwitdth[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

char *bandModeDesc[] = {"FM", "LSB", "USB", "AM"};

/*
 * Band data structure
 */
typedef struct
{
  uint8_t bandType;      // Band type (FM, MW or SW)
  uint16_t minimumFreq;  // Minimum frequency of the band
  uint16_t maximumFreq;  // maximum frequency of the band
  uint16_t currentFreq;  // Default frequency or current frequency
  uint16_t currentStep;  // Defeult step (increment and decrement)
  uint8_t modeOperation; // Default Operation mode (AM, FM, SSB)
} Band;

/*
 * Band tables
 */
Band band[] = {
    {FM_BAND_TYPE, 8400, 10800, 10390, 10, FM},
    {MW_BAND_TYPE, 570, 1720, 810, 10, AM},
    {SW_BAND_TYPE, 1800, 3500, 1900, 1, LSB}, // 160 meters
    {SW_BAND_TYPE, 3500, 4000, 3700, 1, LSB}, // 80 meters
    {SW_BAND_TYPE, 4500, 5300, 4800, 5, AM},
    {SW_BAND_TYPE, 5600, 6300, 6000, 5, AM},
    {SW_BAND_TYPE, 7000, 7300, 7100, 1, LSB}, // 40 meters
    {SW_BAND_TYPE, 7200, 7800, 7350, 5, AM},
    {SW_BAND_TYPE, 9300, 10000, 9600, 5, AM},
    {SW_BAND_TYPE, 10000, 10500, 10050, 1, USB}, // 30 meters
    {SW_BAND_TYPE, 11200, 12500, 11940, 5, AM},
    {SW_BAND_TYPE, 13400, 13900, 13600, 5, AM},
    {SW_BAND_TYPE, 14000, 14500, 14200, 1, USB}, // 20 meters
    {SW_BAND_TYPE, 15000, 15900, 15300, 5, AM},
    {SW_BAND_TYPE, 17200, 17900, 17600, 5, AM},
    {SW_BAND_TYPE, 18000, 18300, 18100, 1, USB}, // 17 meters
    {SW_BAND_TYPE, 21000, 21400, 21200, 1, USB}, // 15 mters
    {SW_BAND_TYPE, 21500, 21900, 21525, 5, AM},
    {SW_BAND_TYPE, 24890, 25000, 24940, 1, USB}, // 12 meters
    {SW_BAND_TYPE, 25670, 26200, 25800, 5, AM},
    {SW_BAND_TYPE, 26200, 27900, 27500, 1, USB},  // CB band (11 meters)
    {SW_BAND_TYPE, 28000, 30000, 28400, 1, USB}}; // 10 meters

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = 50;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
LiquidCrystal_I2C display(0x27, 20, 4); // please check the address of your I2C device
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
  pinMode(BFO_SWITCH, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(AVC_SWITCH, INPUT_PULLUP);

  display.init();

  delay(500);

  // Splash - Change it for your introduction text.
  display.backlight();
  display.setCursor(7, 0);
  display.print("SI4735");
  display.setCursor(2, 1);
  display.print("Arduino Library");
  delay(500);
  display.setCursor(1, 2);
  display.print("All in One Radio");
  delay(500);
  display.setCursor(4, 3);
  display.print("By PU2CLR");
  delay(2000);
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, MW_BAND_TYPE);

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();

  currentFrequency = previousFrequency = si4735.getFrequency();

  si4735.setVolume(volume);
  display.clear();
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

void showFrequency()
{
  String freqDisplay;
  String unit;
  String bandMode;
  int divider = 1;
  int decimals = 3;
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    divider = 100;
    decimals = 1;
    unit = "MHz";
  }
  else if (band[bandIdx].bandType == MW_BAND_TYPE)
  {
    divider = 1;
    decimals = 0;
    unit = "KHz";
  }
  else
  {
    divider = 1000;
    decimals = 3;
    unit = "KHz";
  }

  freqDisplay = String((float)currentFrequency / divider, decimals);

  display.setCursor(7, 0);
  display.print("        ");
  display.setCursor(7, 0);
  display.print(freqDisplay);

  bandMode = bandModeDesc[band[bandIdx].modeOperation];

  display.setCursor(0, 0);
  display.print(bandMode);

  display.setCursor(16, 0);
  display.print(unit);
}

void showStatus()
{

  showFrequency();

  // Show AGC Information
  si4735.getAutomaticGainControl();
  display.setCursor(0, 1);
  display.print((si4735.isAgcEnabled()) ? "AGC ON " : "AGC OFF");

  display.setCursor(12, 1);
  display.print("          ");
  display.setCursor(12, 1);
  display.print("St: ");
  display.print(currentStep);
  display.print("KHz");

  display.setCursor(0, 3);
  display.print("           ");
  display.setCursor(0, 3);
  display.print("BW:");
  display.print(String(bandwitdth[bandwidthIdx]));
  display.print("KHz");

  showBFO();
  showRSSI();
}

/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
  int bars = ((rssi / 10.0) / 2.0) + 1;

  display.setCursor(12, 3);
  display.print("        ");
  display.setCursor(12, 3);
  display.print("S:");
  for (int i = 0; i < bars; i++)
    display.print(">");
}

/* ***************************
   Shows the volume level on LCD
*/
void showVolume()
{
  /*
    display.setCursor(10, 2);
    display.print("          ");
    display.setCursor(10, 2);
    display.print("V:");
    display.print(volume);
  */
}

void showBFO()
{

  String bfo;

  if (currentBFO > 0)
    bfo = "+" + String(currentBFO);
  else
    bfo = String(currentBFO);

  display.setCursor(0, 2);
  display.print("         ");
  display.setCursor(0, 2);
  display.print("BFO:");
  display.print(bfo);
  display.print("Hz ");

  display.setCursor(12, 2);
  display.print("        ");
  display.setCursor(12, 2);
  display.print("St:");
  display.print(currentBFOStep);
}

void bandUp()
{
  display.clear();
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
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

void bandDown()
{
  display.clear();
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
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
 * This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
 * SSB mode.
 */
void loadSSB()
{
  delay(100);
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  delay(100);
  si4735.patchPowerUp();
  delay(100);
  si4735.downloadPatch(ssb_patch_content, size_content);
  delay(100);
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  si4735.setSSBConfig(bandwidthIdx, 1, 0, 1, 0, 1);
  delay(100);
  showStatus();
}

/*
 * Band
 */
void useBand()
{

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
  }
  else if (band[bandIdx].bandType == MW_BAND_TYPE)
  {
    si4735.setTuneFrequencyAntennaCapacitor(0); 
    si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    delay(250);
    si4735.setVolume(volume);
  }
  else if (band[bandIdx].bandType == SW_BAND_TYPE)
  {
    if (band[bandIdx].modeOperation != AM)
    {
      // switch to SSB mode. Load the patch
      si4735.setup(RESET_PIN, MW_BAND_TYPE);
      delay(100);
      loadSSB();
      delay(250);
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, band[bandIdx].modeOperation);
      delay(250);
    }
    else
    {
      si4735.reset(); 
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      delay(250);
      volume = 50;
      si4735.setVolume(volume);
    }
  }
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  currentFrequency = si4735.getFrequency();
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

    if (bfoOn)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
    }
    else
    {
      if (encoderCount == 1)
        si4735.frequencyUp();
      else
        si4735.frequencyDown();

      // Show the current frequency only if it has changed
      currentFrequency = si4735.getFrequency();
      showFrequency();
    }
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
    {
      bandwidthIdx++;
      if (bandwidthIdx > 5)
        bandwidthIdx = 0;
      si4735.setSSBAudioBandwidth(bandwidthIdx);
      // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
      if (bandwidthIdx == 0 || bandwidthIdx == 4 || bandwidthIdx == 5)
        si4735.setSBBSidebandCutoffFilter(0);
      else
        si4735.setSBBSidebandCutoffFilter(1);
      showStatus();
    }
    else if (digitalRead(BAND_BUTTON_UP) == LOW)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
      bandDown();
    else if (digitalRead(VOL_UP) == LOW) {
      si4735.volumeUp();
      volume = si4735.getVolume();
    }
    else if (digitalRead(VOL_DOWN) == LOW) {
      si4735.volumeDown();
      volume = si4735.getVolume();     
    }
    else if (digitalRead(BFO_SWITCH) == LOW)
    {
      bfoOn = !bfoOn;
      if (bfoOn)
        showBFO();
      else
        showStatus();
      delay(200); // waits a little more for releasing the button.
    }
    else if (digitalRead(AGC_SWITCH) == LOW)
    {
      disableAgc = !disableAgc;
      // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
      si4735.setAutomaticGainControl(disableAgc, 1);
      showStatus();
    }
    else if (digitalRead(STEP_SWITCH) == LOW)
    {
      if (bfoOn)
      {
        currentBFOStep = (currentBFOStep == 50) ? 10 : 50;
        showBFO();
      }
      else
      {
        if (currentStep == 1)
          currentStep = 5;
        else if (currentStep == 5)
          currentStep = 10;
        else
          currentStep = 1;
        si4735.setFrequencyStep(currentStep);
        band[bandIdx].currentStep = currentStep;
        showStatus();
      }
    }
    else if (digitalRead(AVC_SWITCH) == LOW)
    {
      avc_en = !avc_en;
      si4735.setSSBAutomaticVolumeControl(avc_en);
    }
    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  if ((millis() - elapsedFrequency) > MIN_ELAPSED_RSSI_TIME * 4)
  {
    currentFrequency = si4735.getFrequency();
    if (currentFrequency != previousFrequency)
    {
      previousFrequency = currentFrequency;
      showFrequency();
    }
    elapsedFrequency = millis();
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 6)
  {
    si4735.getCurrentReceivedSignalQuality();
    if (rssi != si4735.getCurrentRSSI())
    {
      rssi = si4735.getCurrentRSSI();
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  // Show volume level only if this condition has changed
  if (si4735.getCurrentVolume() != volume)
  {
    volume = si4735.getCurrentVolume();
    showVolume();
  }

  if (currentBFO != previousBFO)
  {
    previousBFO = currentBFO;
    si4735.setSSBBfo(currentBFO);
    showBFO();
  }

  delay(50);
}
