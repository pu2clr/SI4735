/*
  SI4735 all in one with SSB Support
  It is important to know the SSB support works on SI4735-D60 and SI4732-A10 devices. 
  This sketch has been successfully tested on STM32F103 Bluepill
  The table below shows the Si4735 and STM32F103C8 pin connections

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.



  STM32F1 and components wire up. 
  
  | Device name   | Device Pin / Description |  STM32F1|
  | --------------| -------------------------| --------|
  | OLED          |                          |         |
  |               | SDA/SDIO                 |  B7     | 
  |               | SCL/SCLK                 |  B6     | 
  | Encoder       |                          |         |
  |               | A                        |  PA0    |
  |               | B                        |  PA1    |
  |               | PUSH BUTTON (BFO/VFO)    |  PA15   |
  | Buttons       |                          |         |
  |   MODE_SWITCH | Switch MODE (Am/LSB/USB) |  PA2    |  
  |   BANDWIDTH   | Bandwidth                |  PA3    |  
  |   VOL         | Volume Up                |  PA4    |
  |   VOL_DOWN    | Volume Down              |  PA5    |
  |   BAND_UP     | Next Band                |  PA6    | 
  |   BAND_DOWN   | Previous band            |  PA7    | 
  |   AGC_SWITCH  | Switch AGC               |  PA8    | 
  |   STEP_SWITCH | Step Switch              |  PA11   | 




  STM32F1 3.1 and SI4735-D60 or SI4732-A10 wire up

  | Si4735  | SI4732   | DESC.  | ESP32    | 
  |---------| -------- |--------|----------|
  | pin 15  |  pin 9   | RESET  |   PA12   |  
  | pin 18  |  pin 12  | SDIO   |   B7     |
  | pin 17  |  pin 11  | SCLK   |   B6     |
  
 



  This sketch uses I2C OLED/I2C, buttons and  Encoder.

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
  together with this sketch) and Library Adafruit libraries to control the OLED.

  ABOUT SSB PATCH:
  This sketch will download a SSB patch to your SI4735-D60 or SI4732-A10 devices (patch_init.h or patch_full.h). It will take about 8KB or 15KB of the Arduino memory.

  First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr.
  Vadim Afonkin on his Dropbox repository. It is important to note that the author of this library does not encourage anyone to use the SSB patches
  content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library.

  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
  There is little information available about patching the SI4735. The following information is the understanding of the author of
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
  Patches to the SI4735 are binary format stored in some place (eeprom, sd-card) and have to be transferred to the internal RAM of the device by
  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
  Given this, it is at your own risk to continue with the procedures suggested here.
  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.

  Features of this sketch:

  1) FM, AM (MW and SW) and SSB (LSB and USB);
  2) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4kHz;
  3) 22 commercial and ham radio bands pre configured;
  4) BFO Control; and
  5) Frequency step switch (1, 5 and 10kHz);

  Main Parts:
  Encoder with push button;
  Seven bush buttons;
  OLED Display with I2C device;
  STM32F103 Bluepill
  SI4735-D60 circuit (see documentation)

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Rotary.h"

#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    4 // Reset pin # (or -1 if sharing Arduino reset pin)


// Si4735 and receiver constants
#define RESET_PIN PA12
// Enconder PINs
#define ENCODER_PIN_A PA0
#define ENCODER_PIN_B PA1

// Buttons controllers
#define MODE_SWITCH PA2      // Switch MODE (Am/LSB/USB)
#define BANDWIDTH_BUTTON PA3 // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define VOL_UP PA4           // Volume Up
#define VOL_DOWN PA5         // Volume Down
#define BAND_BUTTON_UP PA6   // Next band
#define BAND_BUTTON_DOWN PA7 // Previous band
#define AGC_SWITCH PA8       // Switch AGC ON/OF
#define STEP_SWITCH PA11     // Used to select the increment or decrement frequency step (1, 5 or 10 kHz)
#define BFO_SWITCH PA15      // Used to select the enconder control (BFO or VFO)

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150

#define DEFAULT_VOLUME 45 // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

bool bfoOn = false;
bool ssbLoaded = false;
bool fmStereo = true;

// AGC and attenuation control
uint8_t agcIdx = 0;
uint8_t disableAgc = 0;
uint8_t agcNdx = 0;

char displayBuffer[20];
char oldFreq[20];
char oldMode[10];
char oldUnit[10];
char oldStep[10];
char oldBandwitdt[20];
char oldExtraSignalInfo[15];
char oldRssi[20];
char oldSnr[20];
char oldBfo[15];
char oldStepBfo[10];
char oldVolume[10];


int currentBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentStep = 1;
uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char *bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char *bandwidthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

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
  {FM_BAND_TYPE, 8400, 10800, 10570, 10},
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

uint8_t rssi = 0;
uint8_t snr = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SI4735 si4735;



inline void clearBuffer(char *p) {
  p[0] = '\0';
}


void resetBuffer() {
  clearBuffer(displayBuffer);
  clearBuffer(oldFreq);
  clearBuffer(oldMode);
  clearBuffer(oldUnit);
  clearBuffer(oldStep);
  clearBuffer(oldBandwitdt);
  clearBuffer(oldExtraSignalInfo);
  clearBuffer(oldRssi);
  clearBuffer(oldSnr);
  clearBuffer(oldBfo);
  clearBuffer(oldStepBfo);
  clearBuffer(oldVolume);
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

/*
    Writes just the changed information on Display
    Prevents blinking on display and also noise.
    Erases the old digits if it has changed and print the new digit values.
*/
void printValue(int col, int line, char *oldValue, char *newValue, int space, int textSize ) {
  int c = col;
  char * pOld;
  char * pNew;

  pOld = oldValue;
  pNew = newValue;

  oled.setTextSize(textSize);

  // prints just changed digits
  while (*pOld && *pNew)
  {
    if (*pOld != *pNew)
    {
      oled.setTextColor(SSD1306_BLACK);
      oled.setCursor(c, line);
      oled.write(*pOld);
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(c, line);
      oled.write(*pNew);
    }
    pOld++;
    pNew++;
    c += space;
  }
  // Is there anything else to erase?
  oled.setTextColor(SSD1306_BLACK);
  while (*pOld)
  {
    oled.setCursor(c, line);
    oled.write(*pOld);
    pOld++;
    c += space;
  }
  // Is there anything else to print?
  oled.setTextColor(SSD1306_WHITE);
  while (*pNew)
  {
    oled.setCursor(c, line);
    oled.write(*pNew);
    pNew++;
    c += space;
  }

  // Save the current content to be tested next time
  strcpy(oldValue, newValue);
}

/*
   Shows the volume level on LCD
*/
void showVolume()
{
  char sVolume[10];
  sprintf(sVolume, "V%2.2u", si4735.getCurrentVolume());
  printValue(105, 30, oldVolume, sVolume, 6, 1);
  oled.display();
}

/*
   Shows the BFO current status.
   Must be called only on SSB mode (LSB or USB)
*/
void showBFO()
{

  char bfo[15];
  char stepBfo[10];
  char flag;

  if (currentBFO > 0)
    flag = '+';
  else if (currentBFO < 0)
    flag = '-';
  else
    flag = ' ';

  sprintf(bfo, "BFO: %c%4.4uHz", flag, abs(currentBFO));
  printValue(0, 43, oldBfo, bfo, 6, 1);

  sprintf(stepBfo, "%3.3u", currentBFOStep);
  printValue(105, 43, oldStepBfo, stepBfo, 6, 1);

  oled.display();

}


void showFrequency()
{
  char freq[15];
  char tmp[15];
  char * unit;
  char * bandMode;

  byte textSize;

  sprintf(tmp, "%5u", currentFrequency);

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    freq[0] = tmp[0];
    freq[1] = tmp[1];
    freq[2] = tmp[2];
    freq[3] = '.';
    freq[4] = tmp[3];
    freq[5] = tmp[4];
    freq[6] = '\0';
    unit = "MHz";
  }
  else
  {
    freq[0] = ' ';
    freq[1] = tmp[0];
    freq[2] = tmp[1];
    freq[3] = tmp[2];
    freq[4] = tmp[3];
    freq[5] = tmp[4];
    freq[6] = '\0';
    unit = "kHz";
  }

  textSize = ( !bfoOn ) ? 2 : 1;

  printValue(23, 0, oldFreq, freq, 12, textSize);

  if (currentFrequency < 520 )
    bandMode = "LW  ";
  else
    bandMode = (char *) bandModeDesc[currentMode];

  printValue(0, 0, oldMode, bandMode, 7, 1);
  printValue(105, 0, oldUnit, unit, 7, 1);

  oled.display();
}

/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
  char sRssi[20];
  char sSnr[20];
  char extraSignalInfo[15];

  if ( currentMode == FM) {
    sprintf(extraSignalInfo, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    printValue(0, 20, oldExtraSignalInfo, extraSignalInfo, 7, 1);
  } else {
     // AGC and Attenuation
     si4735.getAutomaticGainControl();
     if ( si4735.isAgcEnabled() ) {
      strcpy(extraSignalInfo, "AGC ON");
     } else {
      sprintf(extraSignalInfo, "ATT %2d", agcNdx);      
     }
     printValue(0, 20, oldExtraSignalInfo, extraSignalInfo, 7, 1);
  }

  sprintf(sRssi, "RSSI:%idBuV", rssi);
  printValue(0, 56, oldRssi, sRssi, 6, 1);

  sprintf(sSnr, "SNR:%idB", snr);
  printValue(78, 56, oldSnr, sSnr, 6, 1);

  oled.display();
}



/*
    Show some basic information on display
*/
void showStatus()
{

  char step[10];
  char bandwitdt[20];


  oled.clearDisplay();
  resetBuffer();

  oled.drawLine(0,17,130,17, SSD1306_WHITE ); 
  oled.drawLine(0,52,130,52, SSD1306_WHITE );
  
  showFrequency();

  sprintf(step, "%3.3u", currentStep);
  printValue(105, 20, oldStep, step, 7, 1);

  if (currentMode == LSB || currentMode == USB)
  {
    sprintf(bandwitdt, "BW:%skHz", bandwidthSSB[bwIdxSSB]);
    printValue(0, 30, oldBandwitdt, bandwitdt, 7, 1);
    showBFO();
  }
  else if (currentMode == AM)
  {
    sprintf(bandwitdt, "BW:%skHz", bandwidthAM[bwIdxAM]);
    printValue(0, 30, oldBandwitdt, bandwitdt, 7, 1);
  }

  showVolume();
  showRSSI();

  oled.display();
}

/*
   Switch the radio to current band.
   The bandIdx variable points to the current band.
   This function change to the band referenced by bandIdx (see table band).
*/
void useBand()
{
  // cleanBfoRdsInfo();
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    bfoOn = ssbLoaded = false;

  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      si4735.setTuneFrequencyAntennaCapacitor(0);
    else
      si4735.setTuneFrequencyAntennaCapacitor(1);

    if (ssbLoaded)
    {
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      si4735.setSSBAutomaticVolumeControl(1);
    }
    else
    {
      currentMode = AM;
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      bfoOn = false;
    }
    si4735.setAmSoftMuteMaxAttenuation(0); // Disable Soft Mute for AM or SSB
    si4735.setAutomaticGainControl(disableAgc, agcNdx);
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  resetBuffer();
  showStatus();
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
   This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
   SSB mode.
*/
void loadSSB()
{
  si4735.reset();
  si4735.queryLibraryId(); // Is it really necessary here?  Just powerDown() maigh work!
  si4735.patchPowerUp();
  delay(50);
  // You might wnat to improve the I2C bus speed.
  si4735.downloadPatch(ssb_patch_content, size_content);
  // cleanBfoRdsInfo();
  // delay(50);
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  si4735.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
  showStatus();
}



/*
 * Button Mode Switch FM, AM, LSB and USB
 */
void modeSwitchButton() {
  if (currentMode != FM)
  {
    if (currentMode == AM)
    {
      // If you were in AM mode, it is necessary to load SSB patch (avery time)
      loadSSB();
      currentMode = LSB;
    }
    else if (currentMode == LSB)
    {
      currentMode = USB;
    }
    else if (currentMode == USB)
    {
      currentMode = AM;
      ssbLoaded = false;
      bfoOn = false;
    }
    // Nothing to do if you are in FM mode
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    useBand();
  }
}

/*
 * Button bandwidth on AM and SSB modes
 */
void bandwidthButton() {
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB++;
    if (bwIdxSSB > 5)
      bwIdxSSB = 0;
    si4735.setSSBAudioBandwidth(bwIdxSSB);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
      si4735.setSSBSidebandCutoffFilter(0);
    else
      si4735.setSSBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    bwIdxAM++;
    if (bwIdxAM > 6)
      bwIdxAM = 0;
    si4735.setBandwidth(bwIdxAM, 1);
  }
  resetBuffer();
  showStatus();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}

/*
 * Button BFO switch. Makes the encoder control the VFO or BFO
 */
void bfoSwitchButton() {
  if (currentMode == LSB || currentMode == USB)
  {
    bfoOn = !bfoOn;
    if (bfoOn)
      showBFO();
    showStatus();
  }
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  showFrequency();

}

/*
 * Button Step. kHz if you are controlling the VFO. If you are controlling the BFO the step changed will be the BFO in Hz
 */
void stepButton() {

  // This command should work only for SSB mode
  if (bfoOn && (currentMode == LSB || currentMode == USB))
  {
    currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
    showBFO();
  }
  else
  {
    if (currentMode != FM)
    {
      if (currentStep == 1)
        currentStep = 5;
      else if (currentStep == 5)
        currentStep = 10;
      else if (currentStep == 10)
        currentStep = 50;
      else
        currentStep = 1;
    }
    else
    {
      if (currentStep == 10)
        currentStep = 100;
      else
        currentStep = 10;
    }
    si4735.setFrequencyStep(currentStep);
    band[bandIdx].currentStep = currentStep;
    showStatus();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

}

/*
 * Button - AGC and attenuation 
 */
void attenuationButton() {

      if (agcIdx == 0)
      {
        disableAgc = 0; // Turns AGC ON
        agcNdx = 0;
        agcIdx = 1;
      } else if (agcIdx == 1)
      {
        disableAgc = 1; // Turns AGC OFF
        agcNdx = 0;     // Sets minimum attenuation
        agcIdx = 2;
      } else if (agcIdx == 2)
      {
        disableAgc = 1; // Turns AGC OFF
        agcNdx = 10;    // Increases the attenuation AM/SSB AGC Index  = 10
        agcIdx = 3;
      } else if (agcIdx == 3)
      {
        disableAgc = 1; // Turns AGC OFF
        agcNdx = 15;    // Increases the attenuation AM/SSB AGC Index  = 30
        agcIdx = 4;
      } else if (agcIdx == 4) {
        disableAgc = 1; // Turns AGC OFF
        agcNdx = 25;    // Increases the attenuation AM/SSB AGC Index  = 30
        agcIdx = 5;        
      } else if (agcIdx == 5) {
        disableAgc = 1; // Turns AGC OFF
        agcNdx = 35;    // Increases the attenuation AM/SSB AGC Index  = 30
        agcIdx = 0;        
      }      
      // Sets AGC on/off and gain
      si4735.setAutomaticGainControl(disableAgc, agcNdx);
      showStatus();
}


/*
 * Button - Volume control
 */
void volumeButton ( byte d) {

  if ( d == 1 )  
    si4735.volumeUp();
  else
    si4735.volumeDown();

  volume = si4735.getVolume();
  showVolume();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.

}


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
  pinMode(MODE_SWITCH, INPUT_PULLUP);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  // Splash - Change it for your introduction text.
  oled.setTextSize(1); // Draw 2X-scale text
  oled.setCursor(40, 0);
  oled.print("SI4735");
  oled.setCursor(20, 10);
  oled.print("Arduino Library");
  oled.display();
  delay(500);
  oled.setCursor(15, 20);
  oled.print("All in One Radio");
  oled.display();
  delay(500);
  oled.setCursor(30, 35);
  oled.print("SMT32 - OLED");
  oled.setCursor(10, 50);
  oled.print("V1.1.6 - By PU2CLR");

  oled.display();
  delay(2000);
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);


  si4735.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error

  // si4735.setI2CFastMode();               // Recommended (400 kHz)

  si4735.setup(RESET_PIN, MW_BAND_TYPE);

  delay(300);
  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();

  currentFrequency = si4735.getFrequency();

  si4735.setVolume(volume);

  showStatus();
}


void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      si4735.setSSBBfo(currentBFO);
      showBFO();
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
      bandwidthButton();
    else if (digitalRead(BAND_BUTTON_UP) == LOW)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
      bandDown();
    else if (digitalRead(VOL_UP) == LOW)
      volumeButton(1);
    else if (digitalRead(VOL_DOWN) == LOW)
      volumeButton(-1);
    else if (digitalRead(BFO_SWITCH) == LOW)
      bfoSwitchButton();
    else if (digitalRead(AGC_SWITCH) == LOW)
      attenuationButton(); 
    else if (digitalRead(STEP_SWITCH) == LOW)
      stepButton();  
    else if (digitalRead(MODE_SWITCH) == LOW)
      modeSwitchButton();
    elapsedButton = millis();
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 10)
  {
    si4735.getCurrentReceivedSignalQuality();
    int aux = si4735.getCurrentRSSI();
    if (rssi != aux)
    {
      rssi = aux;
      snr = si4735.getCurrentSNR();
      showRSSI();
    }
    elapsedRSSI = millis();
  }
  delay(10);
}
