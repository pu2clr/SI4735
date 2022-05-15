/*
  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT from MICROYUM (2" - 176 x 220) - ILI9225.
  The Arduino library used to control that display device is TFT_22_ILI9225 (seeL https://github.com/Nkawu/TFT_22_ILI9225).
  Please, install it before start working  with this sketch. 

  It is a complete radio capable to tune FM/RDS; LW, MW, SW on AM and SSB mode and also receive the
  regular comercial stations. If you are using the same circuit used on examples with OLED and LCD,
  you have to change some buttons wire up. This TFT device takes five pins from Arduino.
  For this reason, it is necessary change the pins of some buttons.
  Fortunately, you can use the ATmega328 analog pins as digital pins.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.


  Features:   FM/RDS; AM; SSB; LW/MW/SW; two super band (from 150kHz to 30 MHz); external mute circuit control; Seek (Automatic tuning)
              AGC; Attenuation gain control; SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  It is important to know the SSB support works on SI4735-D60 and SI4732-A10 devices. 

  Wire up on Arduino UNO, Pro mini
  | Device name               | Device Pin / Description  |  Arduino Pin  |
  | ----------------          | --------------------      | ------------  |
  | Display TFT               |                           |               |
  |                           | RST (RESET)               |      8        |
  |                           | RS or DC                  |      9        |
  |                           | CS or SS                  |     10        |
  |                           | SDI / SDA / MOSI          |     11        |
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
#include "TFT_22_ILI9225.h" //   // See https://github.com/Nkawu/TFT_22_ILI9225/wiki
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
#define AUDIO_MUTE 1       // External AUDIO MUTE circuit control

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
bool ssbLoaded = false;
bool fmStereo = true;


// AGC and attenuation control
uint8_t agcIdx = 0;
uint8_t disableAgc = 0;
uint8_t agcNdx = 0;


int currentBFO = 0;
uint8_t seekDirection = 1;


long elapsedRSSI = millis();
long elapsedButton = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency = 0;

uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char * bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char * bandwidthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char * bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

uint16_t currentStep = 1;

char bufferDisplay[100]; // Useful to handle string
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
  {"AM ", MW_BAND_TYPE, 520, 1720, 810, 10},
  {"SW1", SW_BAND_TYPE, 150, 30000, 7100, 1}, // ALL SW1 (from 150kHz to 30MHz)
  {"SW2", SW_BAND_TYPE, 150, 30000, 14200, 1} // ALL SW2 (from 150kHz to 30MHz)
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

const char * const text_message  = "DIY: github.com/pu2clr/SI4735";

uint8_t rssi = 0;
uint8_t snr = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
SI4735 si4735;

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


  // Comment the line below if you do not have external audio mute circuit
  si4735.setAudioMuteMcuPin(AUDIO_MUTE);

  // Use this initializer if using a 1.8" TFT screen:
  tft.begin();
  tft.setOrientation(1);
  tft.clear();
  showTemplate();

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  // si4735.setup(RESET_PIN, 1); // Starts FM mode and ANALOG audio mode
  // si4735.setup(RESET_PIN, 0, 1, SI473X_ANALOG_AUDIO); // Starts FM mode and ANALOG audio mode.
  si4735.setup(RESET_PIN, 0, 1, SI473X_ANALOG_DIGITAL_AUDIO); // Starts FM mode and ANALOG and DIGITAL audio mode.

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  si4735.setVolume(volume);
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

  tft.drawText(5, 130, text_message, COLOR_YELLOW);
}


/*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
void printValue(byte col, byte line, char *oldValue, char *newValue, uint16_t color, uint8_t space) {
  byte c = col;
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

  // Save the current content to be tested next time
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

/*
   Shows frequency information on Display
*/
void showFrequency()
{
  uint16_t color;
  char tmp[15];
  
  // It is better than use dtostrf or String to save space.   
  
  sprintf(tmp,"%5.5u", currentFrequency);

  if (si4735.isCurrentTuneFM())
  {
    bufferDisplay[0] = tmp[0];
    bufferDisplay[1] = tmp[1];
    bufferDisplay[2] = tmp[2];
    bufferDisplay[3] = '.';
    bufferDisplay[4] = tmp[3];
    bufferDisplay[5] = '\0';
  }
  else
  {
    if ( currentFrequency  < 1000 ) {
       bufferDisplay[0] = tmp[2] ;
       bufferDisplay[1] = tmp[3];
       bufferDisplay[2] = tmp[4];
       bufferDisplay[3] = '\0';
    } else {
      bufferDisplay[0] = tmp[0] ;
      bufferDisplay[1] = tmp[1];
      bufferDisplay[2] = '.';
      bufferDisplay[3] = tmp[2];
      bufferDisplay[4] = tmp[3];
      bufferDisplay[5] = tmp[4];
      bufferDisplay[6] = '\0';
    }
  }

  color = (bfoOn) ? COLOR_CYAN : COLOR_YELLOW;
  tft.setFont(Trebuchet_MS16x21);
  printValue(10, 10, bufferFreq, bufferDisplay, color, 20);
}


// Will be used by seekStationProgress function. 
// This Si4735 library method calls the function below during seek process informing the current seek frequency.    
void showFrequencySeek(uint16_t freq)
{
  currentFrequency = freq;
  showFrequency();
}

/*
    Show some basic information on display
*/
void showStatus()
{
  si4735.getStatus();
  si4735.getCurrentReceivedSignalQuality();
  // SRN
  currentFrequency = si4735.getFrequency();
  showFrequency();

  tft.setFont(Terminal6x8);
  printValue(155, 10, bufferStepVFO, bufferDisplay, COLOR_BLACK, 7);

  if (si4735.isCurrentTuneFM())
  {
    tft.drawText(155, 30, "MHz", COLOR_RED);
    tft.drawText(124, 45, bufferBW, COLOR_BLACK);
    CLEAR_BUFFER(bufferBW)
  }
  else
  {
    tft.fillRectangle(153, 3, 216, 20, COLOR_BLACK);  // Clear Step field
    sprintf(bufferDisplay, "Stp: %3d", currentStep);
    printValue(153, 10, bufferStepVFO, bufferDisplay, COLOR_YELLOW, 6);
    tft.drawText(153, 30, "kHz", COLOR_RED);
  }

  if (band[bandIdx].bandType == SW_BAND_TYPE)
    sprintf(bufferDisplay, "%s %s", band[bandIdx].bandName, bandModeDesc[currentMode]);
  else
    sprintf(bufferDisplay, "%s", band[bandIdx].bandName);
  printValue(4, 60, bufferBand, bufferDisplay, COLOR_CYAN, 6);

  // AGC
  si4735.getAutomaticGainControl();
  sprintf(bufferDisplay, "%s %2d", (si4735.isAgcEnabled()) ? "AGC" : "ATT", agcNdx);
  printValue(65, 60, bufferAGC, bufferDisplay, COLOR_CYAN, 6);
  showFilter();
}


void showFilter() {
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
    sprintf(bufferDisplay, "BW: %s kHz", bw);
    printValue(124, 45, bufferBW, bufferDisplay, COLOR_CYAN, 6);
  }
}

char *rdsMsg;
char *stationName;
char *rdsTime;
char bufferStatioName[40];
char bufferRdsMsg[40];
char bufferRdsTime[32];

long stationNameElapsed = millis();

void showRDSMsg() {
  rdsMsg[35] = bufferRdsMsg[35] = '\0';
  if (strcmp(bufferRdsMsg, rdsMsg) == 0) return;
  printValue(5, 90, bufferRdsMsg, rdsMsg, COLOR_GREEN, 6);
  delay(250);
}

/**
 * TODO: process RDS Dynamic PS or Scrolling PS
 */
void showRDSStation() {
  if (strncmp(bufferStatioName, stationName,3) == 0 ) return;
  printValue(5, 110, bufferStatioName, stationName, COLOR_GREEN, 6);
  // for( int i = 0; i < 8; i++ ) stationName[i] = '\0';
}

void showRDSTime() {
  if (strcmp(bufferRdsTime, rdsTime) == 0 ) return;
  printValue(100, 110, bufferRdsTime, rdsTime, COLOR_GREEN, 6);
  delay(100);
}

void checkRDS() {
  tft.setFont(Terminal6x8);
  si4735.getRdsStatus();
  if (si4735.getRdsReceived()) {
    if (si4735.getRdsSync() && si4735.getRdsSyncFound() ) {
      rdsMsg = si4735.getRdsText2A();
      stationName = si4735.getRdsText0A();
      rdsTime = si4735.getRdsTime();
      if ( rdsMsg != NULL )   showRDSMsg();
      
      if ( (millis() - stationNameElapsed) > 2000 ) {
        if ( stationName != NULL && si4735.getRdsNewBlockA() )   showRDSStation();
        stationNameElapsed = millis();
      }
      
      if ( rdsTime != NULL ) showRDSTime();
    }
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

  tft.setFont(Terminal6x8);
  if (currentMode == FM)
  {
    sprintf(bufferDisplay, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
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
  tft.drawText(124, 65, "Stp.:", color);
}

void clearBFO() {
  tft.fillRectangle(124, 52, 218, 79, COLOR_BLACK); // Clear All BFO area
  CLEAR_BUFFER(bufferBFO);
  CLEAR_BUFFER(bufferStepBFO);
}

void showBFO()
{
  tft.setFont(Terminal6x8);

  sprintf(bufferDisplay, "%+4d", currentBFO);
  printValue(160, 55, bufferBFO, bufferDisplay, COLOR_CYAN, 7);

  sprintf(bufferDisplay, "%4d", currentBFOStep);
  printValue(160, 65, bufferStepBFO, bufferDisplay, COLOR_CYAN, 7);
}


/**
 * Sets Band up (1) or down (!1)
 */
void setBand(uint8_t up_down) {
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;      
  if ( up_down == 1) 
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;
  useBand();
}


/*
   This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
   SSB mode.
*/
void loadSSB()
{
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  si4735.patchPowerUp();
  delay(50);
  si4735.setI2CFastMode(); // Recommended
  // si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
  si4735.downloadPatch(ssb_patch_content, size_content);
  si4735.setI2CStandardMode(); // goes back to default (100kHz)

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
}

/*
   Switch the radio to current band
*/
void useBand()
{
  showBFOTemplate(COLOR_BLACK);
  tft.fillRectangle(3, 90,  tft.maxX() - 5, 120, COLOR_BLACK);

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    si4735.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    bfoOn = ssbLoaded = false;
    si4735.setRdsConfig(1, 2, 2, 2, 2);
  }
  else
  {
    // set the tuning capacitor for SW or MW/LW
    si4735.setTuneFrequencyAntennaCapacitor( (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);

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
    si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    si4735.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : band[bandIdx].currentStep); // Max 10kHz for spacing
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  rssi = 0;
  clearBFO();
  tft.fillRectangle(153, 3, 216, 20, COLOR_BLACK);  // Clear Step field
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
      if (encoderCount == 1) {
        si4735.frequencyUp();
        seekDirection = 1;
      }
      else {
        si4735.frequencyDown();
        seekDirection = 0;
      }
      // Show the current frequency only if it has changed
      currentFrequency = si4735.getFrequency();
    }
    showFrequency();
    encoderCount = 0;
  } else {
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
        showStatus();
        // delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
      }
      else if (digitalRead(BAND_BUTTON_UP) == LOW)
        setBand(1);
      else if (digitalRead(BAND_BUTTON_DOWN) == LOW)
        setBand(-1);
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
        } else {
          si4735.seekStationProgress(showFrequencySeek, seekDirection);
          currentFrequency = si4735.getFrequency();
        }
        // delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
        showFrequency();
      }
      else if (digitalRead(AGC_SWITCH) == LOW)
      {
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
          agcNdx = 20;    // Increases the attenuation AM/SSB AGC Index  = 30
          agcIdx = 4;          
        } else if (agcIdx == 4)
        {
          disableAgc = 1; // Turns AGC OFF
          agcNdx = 30;    // Increases the attenuation AM/SSB AGC Index  = 30
          agcIdx = 0;
        }
        // Sets AGC on/off and gain
        si4735.setAutomaticGainControl(disableAgc, agcNdx);
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
        else if (currentMode == FM ) {
          band[bandIdx].currentStep = (band[bandIdx].currentStep == 10) ? 100 : 10;
          si4735.setFrequencyStep(band[bandIdx].currentStep);
        }
        else
        {
          if (currentStep == 1)
            currentStep = 5;
          else if (currentStep == 5)
            currentStep = 10;
          else if (currentStep == 10)
            currentStep = 50;
          else if ( currentStep == 50 &&  bandIdx > 1 )  // If band index is not VHF(FM), LW (AM) and MW(AM) you can use 500kHz Step.
            currentStep = 500;
          else
            currentStep = 1;
          si4735.setFrequencyStep(currentStep);
          band[bandIdx].currentStep = currentStep;
          si4735.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : currentStep); // Max 10kHz for spacing
          showStatus();
        }
        // delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
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
      delay(MIN_ELAPSED_TIME);
    }
  }


  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 6)
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

  if ( currentMode == FM) {
    if ( currentFrequency != previousFrequency ) {
      tft.fillRectangle(3, 90,  tft.maxX() - 5, 120, COLOR_BLACK);
      bufferStatioName[0] = bufferRdsMsg[0] = rdsTime[0] =  bufferRdsTime[0] = rdsMsg[0] = stationName[0] = '\0';
      showRDSMsg();
      showRDSStation();
      previousFrequency = currentFrequency;
    }
    checkRDS();
  }

  delay(10);
}
