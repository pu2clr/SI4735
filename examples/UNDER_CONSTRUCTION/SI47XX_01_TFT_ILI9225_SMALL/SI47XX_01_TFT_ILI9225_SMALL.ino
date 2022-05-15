/*

  Under construction... 
  Shrinking Arduino memory consumption  by removing sprintf and dtostrf functions. 
  

  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT from MICROYUM (2" - 176 x 220) - ILI9225.
  The Arduino library used to control that display device is TFT_22_ILI9225 (seeL https://github.com/Nkawu/TFT_22_ILI9225).
  Please, install it before start working  with this sketch.

  It is also a complete radio capable to tune LW, MW, SW on AM and SSB mode and also receive the
  regular comercial stations. The SSB support works on SI4735-D60 and SI4732-A10 devices.
  If you are using the same circuit used on examples with OLED and LCD,
  you have to change some buttons wire up. This TFT device takes five pins from Arduino.
  For this reason, it is necessary change the pins of some buttons.
  Fortunately, you can use the ATmega328 analog pins as digital pins.

  ABOUT SSB PATCH:
  This sketch will download a SSB patch to your SI4735-D60 or SI4732-A10 devices (patch_init.h). It will take about 8KB of the Arduino memory.

  First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr.
  Vadim Afonkin on his Dropbox repository. It is important to note that the author of this library does not encourage anyone to use the SSB patches
  content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library.

  Read more about SSB patch documentation on https://pu2clr.github.io/SI4735/

  Wire up on Arduino UNO, Pro mini

  | Device name               | Device Pin / Description  |  Arduino Pin  |
  | ----------------          | --------------------      | ------------  |
  | Display TFT               |                           |               |
  |                           | RST (RESET)               |      8        |
  |                           | RS  or DC                 |      9        |
  |                           | CS  or SS                 |     10        |
  |                           | SDI                       |     11        |
  |                           | CLK                       |     13        |
  |     Si4735                |                           |               |
  |                           | RESET (pin 15)            |     12        |
  |                           | SDIO (pin 18)             |     A4        |
  |                           | SCLK (pin 17)             |     A5        |
  |     Buttons               |                           |               |
  |                           | Switch MODE (AM/LSB/AM)   |      4        |
  |                           | Banddwith                 |      5        |
  |                           | Next band                 |      6        |
  |                           | Previous band             |      7        |
  |                           | AGC ON/OF                 |     14 / A0   |
  |                           | Frequency Step            |     15 / A1   |
  |                           | VFO/VFO Switch            |     16 / A2   |
  |    Encoder                |                           |               |
  |                           | A                         |       2       |
  |                           | B                         |       3       |

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By PU2CLR, Ricardo,  Feb  2020.
*/

#include <SI4735.h>

#include <SPI.h>
#include <TFT_22_ILI9225.h> // See https://github.com/Nkawu/TFT_22_ILI9225/wiki
#include "Rotary.h"

// Test it with patch_init.h or patch_full.h. Do not try load both.
#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

// TFT MICROYUM or ILI9225 based device pin setup
#define TFT_RST 8
#define TFT_RS 9
#define TFT_CS 10  // SS
#define TFT_SDI 11 // MOSI
#define TFT_CLK 13 // SCK
#define TFT_LED 0  // 0 if wired to +3.3V directly
#define TFT_BRIGHTNESS 200

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons controllers
#define MODE_SWITCH 4      // Switch MODE (Am/LSB/USB)
#define BANDWIDTH_BUTTON 5 // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define BAND_BUTTON_UP 6   // Next band
#define BAND_BUTTON_DOWN 7 // Previous band
#define AGC_SWITCH 14      // Pin A0 - Switch AGC ON/OF
#define STEP_SWITCH 15     // Pin A1 - Used to select the increment or decrement frequency step (1, 5 or 10 kHz)
#define BFO_SWITCH 16      // Pin A2 - Used to select the enconder control (BFO or VFO)

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150
#define DEFAULT_VOLUME 50 // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

#define CLEAR_BUFFER(x)  (x[0] = '\0');

bool bfoOn = false;
bool disableAgc = true;
bool ssbLoaded = false;
bool fmStereo = true;

int currentBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;

uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char * bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char * bandwidthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char * bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

uint16_t currentStep = 1;


char bufferDisplay[40]; // Useful to handle string
char bufferFreq[15];
char bufferBFO[15];
char bufferStepVFO[15];
char bufferStepBFO[15];
char bufferBW[15];
char bufferAGC[15];
char bufferBand[15];
char bufferStereo[15];

/*
   Band data structure
*/
typedef struct
{
  const char *bandName; // Band description
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
  {"FM ", FM_BAND_TYPE, 6400, 10800, 10390, 10},
  {"LW ", LW_BAND_TYPE, 100, 510, 300, 1},
  {"AM ", MW_BAND_TYPE, 520, 1720, 810, 10},
  {"80m", SW_BAND_TYPE, 3000, 4500, 3700, 1}, // 80 meters
  {"60m", SW_BAND_TYPE, 4500, 6300, 6000, 5}, //
  {"41m", SW_BAND_TYPE, 6800, 7800, 7100, 5}, // 40 meters
  {"31m", SW_BAND_TYPE, 9200, 10000, 9600, 5},
  {"25m", SW_BAND_TYPE, 11200, 12500, 11940, 5},
  {"22m", SW_BAND_TYPE, 13400, 13900, 13600, 5},
  {"20m", SW_BAND_TYPE, 14000, 14500, 14200, 1}, // 20 meters
  {"19m", SW_BAND_TYPE, 15000, 15900, 15300, 5},
  {"17m", SW_BAND_TYPE, 18000, 18300, 18100, 1}, // 17 meters
  {"15m", SW_BAND_TYPE, 21000, 21900, 21200, 1}, // 15 mters
  {"CB ", SW_BAND_TYPE, 26200, 27900, 27500, 1}, // CB band (11 meters)
  {"10m", SW_BAND_TYPE, 28000, 30000, 28400, 1},
  {"All", SW_BAND_TYPE, 100, 30000, 14200, 1}, // ALL SW (From 1.7 to 30 MHZ)
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;


const char * const text_arduino_library = "PU2CLR-SI4735 Arduino Library";
const char * const text_example  = "https://github.com/pu2clr/SI4735";
const char * const text_message  = "DIY - You can make it better.";


uint8_t rssi = 0;
uint8_t snr = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
SI4735 rx;

void setup()
{
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BANDWIDTH_BUTTON, INPUT_PULLUP);
  pinMode(BAND_BUTTON_UP, INPUT_PULLUP);
  pinMode(BAND_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BFO_SWITCH, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(MODE_SWITCH, INPUT_PULLUP);

  // Use this initializer if using a 1.8" TFT screen:
  tft.begin();
  tft.setOrientation(1);
  tft.clear();
  showTemplate();

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  rx.setup(RESET_PIN, 1);

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  rx.setVolume(volume);
  showStatus();
}

/*
   Shows the static content on  display
*/
void showTemplate()
{
  int maxY1 = tft.maxY() - 1;
  int maxX1 = tft.maxX() - 1;

  tft.setFont(Terminal6x8);

  tft.drawRectangle(0, 0, maxX1, maxY1, COLOR_WHITE);
  tft.drawRectangle(2, 2, maxX1 - 2, 40, COLOR_YELLOW);

  tft.drawLine(150, 0, 150, 40, COLOR_YELLOW);

  tft.drawLine(0, 80, maxX1, 80, COLOR_YELLOW); //
  tft.drawLine(60, 40, 60, 80, COLOR_YELLOW);            // Mode Block
  tft.drawLine(120, 40, 120, 80, COLOR_YELLOW);          // Band name

  tft.drawText(5, 150, "SNR.:", COLOR_RED);
  tft.drawText(5, 163, "RSSI:", COLOR_RED);

  tft.drawLine(0, 145, maxX1, 145, COLOR_YELLOW);

  tft.drawRectangle(45, 150,  maxX1 - 2, 156, COLOR_YELLOW);
  tft.drawRectangle(45, 163,  maxX1 - 2, 169, COLOR_YELLOW);

  tft.drawText(5, 90, text_arduino_library, COLOR_YELLOW);
  tft.drawText(5, 110, text_example, COLOR_YELLOW);
  tft.drawText(5, 130, text_message, COLOR_YELLOW);
}



/*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
void printValue(int col, int line, char *oldValue, char *newValue, uint16_t color, uint8_t space) {
  int c = col;
  char * pOld;
  char * pNew;

  pOld = oldValue;
  pNew = newValue;

  // prints just changed digits
  while (*pOld && *pNew)
  {
    if (*pOld != *pNew)
    {
      tft.drawChar(c, line, *pOld, COLOR_BLACK);
      tft.drawChar(c, line, *pNew, color);
    }
    pOld++;
    pNew++;
    c += space;
  }

  // Is there anything else to erase?
  while (*pOld)
  {
    tft.drawChar(c, line, *pOld, COLOR_BLACK);
    pOld++;
    c += space;
  }

  // Is there anything else to print?
  while (*pNew)
  {
    tft.drawChar(c, line, *pNew, color);
    pNew++;
    c += space;
  }

  // Save the current content to be checked next time
  strcpy(oldValue, newValue);
}

/*
    Reads encoder via interrupt
    Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
*/
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();

  if (encoderStatus)
    encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
}


/**
  Converts a number to a char string and places leading zeros.
  It is useful to mitigate memory space used by sprintf or generic similar function
*/
void convertToChar(uint16_t value, char *strValue, uint8_t len)
{
  char d;
  for (int i = (len - 1); i >= 0; i--)
  {
    d = value % 10;
    value = value / 10;
    strValue[i] = d + 48;
   }
  strValue[len] = '\0';
}



/*
   Shows frequency information on Display
*/
void showFrequency()
{
  uint16_t color;
  char tmp[15];
  char sf[15];

  tft.setFont(Trebuchet_MS16x21);
  
  convertToChar(currentFrequency, tmp, 5);

  sf[5] = sf[6] = '\0';

  sf[0] = tmp[0];
  sf[1] = tmp[1];
  if (rx.isCurrentTuneFM())
  {
    sf[2] = tmp[2];
    sf[3] = '.';
    sf[4] = tmp[3];
  }
  else
  {
      sf[2] = '.';      
      sf[3] = tmp[2];
      sf[4] = tmp[3];
      sf[5] = tmp[4];
  }
  color = (bfoOn) ? COLOR_CYAN : COLOR_YELLOW;
  printValue(10, 10, bufferFreq, sf, color, 20);
}

/*
    Show some basic information on display
*/
void showStatus()
{
  char tmp[6];
  rx.getStatus();
  rx.getCurrentReceivedSignalQuality();
  // SRN
  currentFrequency = rx.getFrequency();

  showFrequency();

  tft.setFont(Terminal6x8);
  printValue(155, 10, bufferStepVFO, bufferDisplay, COLOR_BLACK, 7);

  if (rx.isCurrentTuneFM())
  {
    tft.drawText(155, 30, "MHz", COLOR_RED);
    // showBFOTemplate(COLOR_BLACK);
    // tft.drawText(124, 45, bufferBW, COLOR_BLACK);
  }
  else
  {
    convertToChar(currentStep, tmp, 3);
    strcpy(bufferDisplay, "Stp:");
    strcat(bufferDisplay, tmp);
    printValue(155, 10, bufferStepVFO, bufferDisplay, COLOR_YELLOW, 6);
    tft.drawText(155, 30, "kHz", COLOR_RED);
  }

  strcpy(bufferDisplay, band[bandIdx].bandName);
  if (band[bandIdx].bandType == SW_BAND_TYPE)
  {
    strcat(bufferDisplay, " ");
    strcat(bufferDisplay, bandModeDesc[currentMode]);
  }

  printValue(4, 60, bufferBand, bufferDisplay, COLOR_CYAN, 6);

  // AGC
  rx.getAutomaticGainControl();
  strcpy(bufferDisplay, "AGC ");
  strcat(bufferDisplay, (rx.isAgcEnabled()) ? "ON  " : "OFF");
  printValue(65, 60, bufferAGC, bufferDisplay, COLOR_CYAN, 6);

  showFilter();
}


void showFilter() {

  char tmp[5];
  // Bandwidth
  if (currentMode == LSB || currentMode == USB || currentMode == AM) {
    char * bw;

    tft.drawText(150, 60, bufferStereo, COLOR_BLACK); // Erase Stereo/Mono information

    if (currentMode == AM) {
      bw = (char *) bandwidthAM[bwIdxAM];
    }
    else {
      bw = (char *) bandwidthSSB[bwIdxSSB];
      showBFOTemplate(COLOR_CYAN);
      showBFO();
    }
    strcpy(bufferDisplay, "BW:");
    strcat(bufferDisplay, bw);
    strcat(bufferDisplay, "kHz");
    printValue(124, 45, bufferBW, bufferDisplay, COLOR_CYAN, 6);
  }

}



/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
  int rssiLevel;
  int snrLevel;
  int maxAux = tft.maxX();

  return;  
  tft.setFont(Terminal6x8);
  if (currentMode == FM)
  {
    strcpy(bufferDisplay, (rx.getCurrentPilot()) ? "STEREO" : "MONO");
    printValue(150, 60, bufferStereo, bufferDisplay, COLOR_CYAN, 7);
  }

  // Check it
  // RSSI: 0 to 127 dBuV
  rssiLevel = 47 + map(rssi, 0, 127, 0, ( maxAux  - 43) );
  // SNR.: 0 to 127 dB
  snrLevel = 47 + map(snr, 0, 127, 0, ( maxAux  - 43) );

  tft.fillRectangle(46, 151,  maxAux - 3, 155, COLOR_BLACK);
  tft.fillRectangle(46, 164, maxAux - 3, 168, COLOR_BLACK);

  tft.fillRectangle(46, 151,  rssiLevel, 155, COLOR_LIGHTCYAN);
  tft.fillRectangle(46, 164, snrLevel, 168, COLOR_LIGHTCYAN);

}

void showBFOTemplate(uint16_t color)
{
  tft.setFont(Terminal6x8);

  tft.drawText(150, 60, bufferStereo, COLOR_BLACK);

  tft.drawText(124, 55, "BFO.:", color);
  tft.drawText(124, 65, "Stp:", color);
}

void clearBFO() {
  tft.fillRectangle(124, 52, 218, 73, COLOR_BLACK); // Clear All BFO area
  CLEAR_BUFFER(bufferBFO);
  CLEAR_BUFFER(bufferStepBFO);
}

void showBFO()
{
  uint16_t bfoAux;

  tft.setFont(Terminal6x8);
  bfoAux = currentBFO;
  if (currentBFO < 0 ) {
    bfoAux = ~currentBFO + 1;
    bufferDisplay[0] = '-';
  } else if (currentBFO > 0)
    bufferDisplay[0] = '+';
  else
    bufferDisplay[0] = ' ';

  convertToChar(bfoAux, &bufferDisplay[1], 4);
  printValue(160, 55, bufferBFO, bufferDisplay, COLOR_CYAN, 7);

  convertToChar(currentBFOStep, bufferDisplay, 4);
  printValue(160, 65, bufferStepBFO, bufferDisplay, COLOR_CYAN, 7);
}

/*
   Goes to the next band (see Band table)
*/
void bandUp()
{
  // save the current frequency for the band
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;

  bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;

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

  bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;

  useBand();
}

/*
   This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
   SSB mode.
*/
void loadSSB()
{
  rx.reset();
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  rx.setI2CFastMode(); // Recommended
  // rx.setI2CFastModeCustom(500000); //  It is a test and may crash.
  rx.downloadPatch(ssb_patch_content, size_content);
  rx.setI2CStandardMode(); // goes back to default (100kHz)

  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  rx.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
}

/*
   Switch the radio to current band
*/
void useBand()
{
  showBFOTemplate(COLOR_BLACK);

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    rx.setTuneFrequencyAntennaCapacitor(0);
    rx.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    bfoOn = ssbLoaded = false;
  }
  else
  {
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor( (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);

    if (ssbLoaded)
    {
      rx.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      rx.setSSBAutomaticVolumeControl(1);
      rx.setSsbSoftMuteMaxAttenuation(0); // Disable Soft Mute for SSB
    }
    else
    {
      currentMode = AM;
      rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      rx.setAutomaticGainControl(1, 0);
      rx.setAmSoftMuteMaxAttenuation(0); // // Disable Soft Mute for AM
      bfoOn = false;
    }
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  rssi = 0;
  clearBFO();
  tft.fillRectangle(155, 3, 216, 20, COLOR_BLACK);  // Clear Step field
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
      rx.setSSBBfo(currentBFO);
      showBFO();
    }
    else
    {
      if (encoderCount == 1)
        rx.frequencyUp();
      else
        rx.frequencyDown();
      // Show the current frequency only if it has changed
      currentFrequency = rx.getFrequency();
    }
    showFrequency();
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
    {
      if (currentMode == LSB || currentMode == USB)
      {
        bwIdxSSB++;
        if (bwIdxSSB > 5)
          bwIdxSSB = 0;
        rx.setSSBAudioBandwidth(bwIdxSSB);
        // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
        if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
          rx.setSSBSidebandCutoffFilter(0);
        else
          rx.setSSBSidebandCutoffFilter(1);
      }
      else if (currentMode == AM)
      {
        bwIdxAM++;
        if (bwIdxAM > 6)
          bwIdxAM = 0;
        rx.setBandwidth(bwIdxAM, 1);
      }
      showStatus();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(BAND_BUTTON_UP) == LOW)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
      bandDown();
    else if (digitalRead(BFO_SWITCH) == LOW)
    {
      if (currentMode == LSB || currentMode == USB)
      {
        bfoOn = !bfoOn;
        if (bfoOn)
        {
          showBFOTemplate(COLOR_CYAN);
          showStatus();
          showBFO();
        }
        else
        {
          showBFOTemplate(COLOR_BLACK);
          clearBFO();
        }
        CLEAR_BUFFER(bufferFreq);
      } 
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
      showFrequency();
    }
    else if (digitalRead(AGC_SWITCH) == LOW)
    {
      disableAgc = !disableAgc;
      // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
      rx.setAutomaticGainControl(disableAgc, 1);
      showStatus();
    }
    else if (digitalRead(STEP_SWITCH) == LOW)
    {
      // This command should work only for SSB mode
      if (bfoOn && (currentMode == LSB || currentMode == USB))
      {
        currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
        showBFO();
      }
      else
      {
        if (currentStep == 1)
          currentStep = 5;
        else if (currentStep == 5)
          currentStep = 10;
        else if (currentStep == 10)
          currentStep = 50;
        else if ( currentStep == 50 &&  bandIdx == lastBand)  // If band index is All, you can use 500kHz Step.
          currentStep = 500;
        else
          currentStep = 1;
        rx.setFrequencyStep(currentStep);
        band[bandIdx].currentStep = currentStep;
        showStatus();
      }
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(MODE_SWITCH) == LOW)
    {
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
          bfoOn = ssbLoaded = false;
        }
        // Nothing to do if you are in FM mode
        band[bandIdx].currentFreq = currentFrequency;
        band[bandIdx].currentStep = currentStep;
        useBand();
      }
    }
    elapsedButton = millis();
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 6)
  {
    rx.getCurrentReceivedSignalQuality();
    int aux = rx.getCurrentRSSI();
    if (rssi != aux)
    {
      rssi = aux;
      snr = rx.getCurrentSNR();
      showRSSI();
    }
    elapsedRSSI = millis();
  }
  delay(10);
}
