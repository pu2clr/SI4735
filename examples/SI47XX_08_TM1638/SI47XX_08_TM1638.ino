/*
  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a TM1638 7 segments display and controls

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.


  Features:   AM; SSB; LW/MW/SW; two super band (from 150kHz to 30 MHz); external mute circuit control; Seek (Automatic tuning)
              AGC; Attenuation gain control; SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  Wire up on Arduino UNO, Pro mini and SI4735-D60
  | Device name               | Device Pin / Description      |  Arduino Pin  |
  | ----------------          | ----------------------------- | ------------  |
  |    TM1638                 |                               |               |
  |                           | STB                           |    4          |
  |                           | CLK                           |    7          |
  |                           | DIO                           |    8          |
  |                           | VCC                           |    3.3V       |
  |                           | GND                           |    GND        |
  |     SI4735                |                               |               |
  |                           | RESET (pin 15)                |     12        |
  |                           | SDIO (pin 18)                 |     A4        |
  |                           | SCLK (pin 17)                 |     A5        |
  |                           | SEN (pin 16)                  |    GND        |
  |    Encoder                |                               |               |
  |                           | A                             |       2       |
  |                           | B                             |       3       |
  |                           | Encoder button                |      A0       |

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Jim Reagan's schematic: https://github.com/JimReagans/Si4735-radio-PCB-s-and-bandpass-filter
  By PU2CLR, Ricardo; and W09CHL, Jim Reagan;  Sep  2020.
*/

#include <TM1638lite.h>
#include <SI4735.h>

#include "Rotary.h"

#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define TM1638_STB   4
#define TM1638_CLK   7
#define TM1638_DIO   8

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
#define ENCODER_PUSH_BUTTON 14 // Used to select the enconder control (BFO or VFO)

// TM1638 - Buttons controllers
#define BAND_BUTTON 1       // S1 Band switch button
#define MODE_SWITCH 2       // S2 FM/AM/SSB
#define BANDWIDTH_BUTTON 4  // S3 Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define SEEK_BUTTON 8       // S4 Seek
#define AGC_SWITCH  16      // S5 Switch AGC ON/OF
#define STEP_SWITCH 32      // S6 Increment or decrement frequency step (1, 5 or 10 kHz)
#define AUDIO_VOLUME 64     // S7 Volume Control
#define SOFT_MUTE 128       // S8 External AUDIO MUTE circuit control

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 150
#define ELAPSED_COMMAND 1500 // time to turn off the last command controlled by encoder
#define DEFAULT_VOLUME 50    // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

bool bfoOn = false;
bool ssbLoaded = false;

// AGC and attenuation control
int8_t agcIdx = 0;
uint8_t disableAgc = 0;
int8_t agcNdx = 0;

bool cmdBand = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;
bool cmdSoftMuteMaxAtt = false;

int currentBFO = 0;
uint8_t seekDirection = 1; // Tells the SEEK direction (botton or upper limit)
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
bool commandDisabledByUser = false;
uint8_t currentBFOStep = 10;

typedef struct
{
  uint8_t idx;      // SI473X device bandwidth index
  const char *desc; // bandwidth description
} Bandwidth;

int8_t bwIdxSSB = 4;
Bandwidth bandwidthSSB[] = {
    {4, "0.5"}, // 0
    {5, "1.0"}, // 1
    {0, "1.2"}, // 2
    {1, "2.2"}, // 3
    {2, "3.0"}, // 4
    {3, "4.0"}  // 5
};

int8_t bwIdxAM = 4;
Bandwidth bandwidthAM[] = {{4, "1.0"},
  {5, "1.8"},
  {3, "2.0"},
  {6, "2.5"},
  {2, "3.0"},
  {1, "4.0"},
  {0, "6.0"}
};

uint8_t currentMode = FM;
uint16_t currentStep = 1;

/**
 *  Band data structure
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
 *  Band table. Actually, except FM (VHF), the other bands cover the entire LW / MW and SW spectrum.
 *  Only the default frequency and step is changed. You can change this setup.
 */
Band band[] = {
    {"FM  ", FM_BAND_TYPE, 6400, 10800, 10390, 10},
    {"AM  ", MW_BAND_TYPE, 150, 1720, 810, 10},
    {"180 ", SW_BAND_TYPE, 1700, 3000, 2500, 5},
    {"H80 ", SW_BAND_TYPE, 3500, 4000, 3600, 1},
    {"60  ", SW_BAND_TYPE, 4000, 5500, 4885, 5},
    {"49  ", SW_BAND_TYPE, 5500, 7000, 6100, 5},
    {"H40 ", SW_BAND_TYPE, 7000, 7200, 7100, 1},
    {"41  ", SW_BAND_TYPE, 7200, 8000, 7205, 5},
    {"31  ", SW_BAND_TYPE, 8000, 10500, 9600, 5},
    {"25  ", SW_BAND_TYPE, 10500, 13000, 11940, 5},
    {"22  ", SW_BAND_TYPE, 13000, 14000, 13600, 5},
    {"H20 ", SW_BAND_TYPE, 14000, 15000, 14200, 1},
    {"19  ", SW_BAND_TYPE, 15000, 17000, 15300, 5},
    {"16  ", SW_BAND_TYPE, 17000, 19000, 17600, 5},
    {"H15 ", SW_BAND_TYPE, 20000, 22000, 21525, 1},
    {"13  ", SW_BAND_TYPE, 21000, 22000, 21525, 5},
    {"CB  ", SW_BAND_TYPE, 26000, 28000, 27500, 1},
    {"H10 ", SW_BAND_TYPE, 28000, 30000, 28400, 1},
    {"ALL1", SW_BAND_TYPE, 150, 30000, 21100, 1},
    {"ALL2", SW_BAND_TYPE, 150, 30000, 28400, 1}};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
int idxStep = 0;

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;
int8_t softMuteMaxAttIdx = 8;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
TM1638lite tm(TM1638_STB, TM1638_CLK, TM1638_DIO);
SI4735 rx;

void setup()
{
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  tm.reset();
  showSplash();
  // uncomment the line below if you have external audio mute circuit
  // rx.setAudioMuteMcuPin(AUDIO_VOLUME);
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);
  // rx.setI2CFastMode(); // Set I2C bus speed.
  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error
  rx.setup(RESET_PIN, 0, 1, SI473X_ANALOG_AUDIO); // Starts FM mode and ANALOG audio mode.
  delay(400);
  useBand();
  rx.setVolume(volume);
  showStatus();
}

/**
 * Set all command flags to false. When all flags are disabled (false), the encoder controls the frequency
 */
void disableCommands()
{
  cmdBand = false;
  bfoOn = false;
  cmdVolume = false;
  cmdAgc = false;
  cmdBandwidth = false;
  cmdStep = false;
  cmdMode = false;
  cmdSoftMuteMaxAtt = false;
  commandDisabledByUser = false;
  tm.setLED(1, 0);  // Turn off the command LED indicator
}

/**
 * Checks if the system is in command condition
 */
inline bool isCommand() {
  return (cmdBand | cmdSoftMuteMaxAtt | cmdMode | cmdStep | cmdBandwidth | cmdAgc | cmdVolume | bfoOn);
}

/**
 * Shows the static content on  display
 */
void showSplash()
{
  const char *s7= "-SI4735-";
  for (int i = 0; i < 8; i++) {
    tm.setLED(i, 1);
    delay(200);
    tm.displayASCII(i, s7[i]);
    tm.setLED(i, 0);
    delay(200);
  }
  delay(1000);
  tm.reset();
}

/**
 * Clear the first three 7 seg. display 
 */
void clearStatusDisplay() {
  tm.displayASCII(0, ' ');
  tm.displayASCII(1, ' ');
  tm.displayASCII(2, ' ');
}

/**
 * Reads encoder via interrupt. It uses Rotary.h and Rotary.cpp
 */
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
    encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
}

/**
 * Shows frequency information on Display
 */
void showFrequency()
{
  char bufferDisplay[15];
  char tmp[15];
  // It is better than use dtostrf or String to save space.
  sprintf(tmp, "%5.5u", currentFrequency);
  bufferDisplay[0] = (tmp[0] == '0') ? ' ' : tmp[0];
  bufferDisplay[1] = tmp[1];
  if (rx.isCurrentTuneFM())
  {
    bufferDisplay[2] = tmp[2];
    bufferDisplay[3] = '.';
    bufferDisplay[4] = tmp[3];
  }
  else
  {
    if (currentFrequency < 1000)
    {
      bufferDisplay[1] = ' ';
      bufferDisplay[2] = tmp[2];
      bufferDisplay[3] = tmp[3];
      bufferDisplay[4] = tmp[4];
    }
    else
    {
      bufferDisplay[2] = tmp[2];
      bufferDisplay[3] = tmp[3];
      bufferDisplay[4] = tmp[4];
    }
  }
  bufferDisplay[5] = '\0';
  for (int i = 3; i < 8; i++ )
     tm.displayASCII(i,bufferDisplay[i-3]);
}

/**
 * Shows the current mode (Actually it turns on the FM, LSB, USB or AM LED)
 */
void showMode() {
    for (int i = 4; i < 8; i++ ) {
       tm.setLED (i, (i - 4) == currentMode);
    }
}

/**
 * This function is called by the seek function process.
 */
void showFrequencySeek(uint16_t freq)
{
  currentFrequency = freq;
  showFrequency();
}

/**
 * Show some basic information on display
 */
void showStatus()
{
  tm.reset();
  showFrequency();
  showMode();
}

/**
 *  Shows the current Bandwidth status
 */
void showBandwidth()
{
  char bufferDisplay[15];
  // Bandwidth
  if (currentMode == LSB || currentMode == USB || currentMode == AM)
  {
    char *bw;

    if (currentMode == AM)
      bw = (char *)bandwidthAM[bwIdxAM].desc;
    else
      bw = (char *)bandwidthSSB[bwIdxSSB].desc;
    sprintf(bufferDisplay, "BW %s", bw);
  }
  else
  {
    bufferDisplay[0] = '\0';
  }
  tm.displayText(bufferDisplay);
}

/**
 *   Shows the current RSSI and SNR status
 */
void showRSSI()
{
  uint8_t rssiAux;
  if (isCommand() ) return; // do not show the RSSI during command status
  if (currentMode == FM)
  {
    tm.setLED (0, rx.getCurrentPilot()); // Indicates Stereo or Mono 
  }
  // It needs to be calibrated. You can do it better.
  // RSSI: 0 to 127 dBuV
  if (rssi < 2)
    rssiAux = 4;
  else if (rssi < 4)
    rssiAux = 5;
  else if (rssi < 12)
    rssiAux = 6;
  else if (rssi < 25)
    rssiAux = 7;
  else if (rssi < 50)
    rssiAux = 8;
  else if (rssi >= 50)
    rssiAux = 9;

  clearStatusDisplay();

  tm.displayASCII(0, 'S');
  tm.displayHex(1, rssiAux);
}

/**
 * Shows the current AGC and Attenuation status
 */
void showAgcAtt()
{
  char sAgc[10];
  rx.getAutomaticGainControl();
  if (agcNdx == 0 && agcIdx == 0)
    strcpy(sAgc, "AGC");
  else
    sprintf(sAgc, "ATT %2d", agcNdx);

  tm.displayText(sAgc);
}

/**
 * Shows the current step
 */
void showStep()
{
  char sStep[15];
  sprintf(sStep, "STEP %3d", currentStep);
  tm.displayText(sStep);
}

/**
 *  Shows the current audio volume level 
 */
void showVolume()
{
  char volAux[12];
  sprintf(volAux, "Vol %2u", rx.getVolume());
  tm.displayText(volAux);
}

/**
 * Shows the current Soft Mute Attenuation
 */
void showSoftMute()
{
  char sMute[15];
  sprintf(sMute, "SM %2d", softMuteMaxAttIdx);
  tm.displayText(sMute);
}

/**
 * Shows the current BFO value
 */
void showBFO()
{
  char bufferDisplay[15];
  sprintf(bufferDisplay, "BFO %4d", currentBFO);
  tm.displayText(bufferDisplay);
  elapsedCommand = millis();
}

/** 
 * Show the current band name on display
 */
void showBand() {
  tm.displayText(band[bandIdx].bandName);
}

/**
 * Show cmd on display. It means you are setting up something.  
 */
inline void showCommandStatus(uint8_t v)
{
  tm.setLED(1,v);
}

/**
 * Sets Band up (1) or down (!1)
 */
void setBand(int8_t up_down)
{
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;
  if (up_down == 1)
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;
  useBand();
  showBand();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *   This function loads the contents of the ssb_patch_content array into the CI (Si4735). 
 *   See also loadPatch implementation in the SI4735 Arduino Library (SI4735.h/SI4735.cpp)
 */
void loadSSB()
{
  tm.displayText("SSB...");
  rx.reset();
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  rx.setI2CFastMode(); // Recommended
  // rx.setI2CFastModeCustom(500000); // It is a test and may crash.
  rx.downloadPatch(ssb_patch_content, size_content);
  rx.setI2CStandardMode(); // goes back to default (100kHz)
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  rx.setSSBConfig(bandwidthSSB[bwIdxSSB].idx, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
}

/**
 * Switch the radio to current band
 */
void useBand()
{
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    rx.setTuneFrequencyAntennaCapacitor(0);
    rx.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    rx.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    bfoOn = ssbLoaded = false;
  }
  else
  {
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor((band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);

    if (ssbLoaded)
    {
      rx.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      rx.setSSBAutomaticVolumeControl(1);
    }
    else
    {
      currentMode = AM;
      rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      bfoOn = false;
    }
    rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Disable Soft Mute for AM or SSB
    rx.setAutomaticGainControl(disableAgc, agcNdx);
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);               // Consider the range all defined current band
    rx.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : band[bandIdx].currentStep); // Max 10kHz for spacing
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  idxStep = getStepIndex(currentStep);
  rssi = 0;
  showStatus();
  showCommandStatus(1);
}

/**
 *   Switches the Bandwidth
 */
void doBandwidth(int8_t v)
{
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB = (v == 1) ? bwIdxSSB + 1 : bwIdxSSB - 1;

    if (bwIdxSSB > 5)
      bwIdxSSB = 0;
    else if (bwIdxSSB < 0)
      bwIdxSSB = 5;

    rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bandwidthSSB[bwIdxSSB].idx == 0 || bandwidthSSB[bwIdxSSB].idx == 4 || bandwidthSSB[bwIdxSSB].idx == 5)
      rx.setSSBSidebandCutoffFilter(0);
    else
      rx.setSSBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    bwIdxAM = (v == 1) ? bwIdxAM + 1 : bwIdxAM - 1;

    if (bwIdxAM > 6)
      bwIdxAM = 0;
    else if (bwIdxAM < 0)
      bwIdxAM = 6;

    rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
  }
  showBandwidth();
  elapsedCommand = millis();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}

/**
 * Deal with AGC and attenuattion
*/
void doAgc(int8_t v)
{
  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;
  if (agcIdx < 0)
    agcIdx = 37;
  else if (agcIdx > 37)
    agcIdx = 0;

  disableAgc = (agcIdx > 0); // if true, disable AGC; esle, AGC is enable

  if (agcIdx > 1)
    agcNdx = agcIdx - 1;
  else
    agcNdx = 0;

  rx.setAutomaticGainControl(disableAgc, agcNdx); // if agcNdx = 0, no attenuation
  showAgcAtt();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *   Gets the current step index.
 */
int getStepIndex(int st)
{
  for (int i = 0; i < lastStep; i++)
  {
    if (st == tabStep[i])
      return i;
  }
  return 0;
}

/**
 * Switches the current step
 */
void doStep(int8_t v)
{
  idxStep = (v == 1) ? idxStep + 1 : idxStep - 1;
  if (idxStep > lastStep)
    idxStep = 0;
  else if (idxStep < 0)
    idxStep = lastStep;

  currentStep = tabStep[idxStep];
  rx.setFrequencyStep(currentStep);
  band[bandIdx].currentStep = currentStep;
  rx.setSeekAmSpacing((currentStep > 10) ? 10 : currentStep); // Max 10kHz for spacing
  showStep();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 * Switches to the AM, LSB or USB modes
*/
void doMode(int8_t v)
{
  if (currentMode != FM)
  {
    if (v == 1)
    { // clockwise
      if (currentMode == AM)
      {
        // If you were in AM mode, it is necessary to load SSB patch (avery time)
        loadSSB();
        currentMode = LSB;
      }
      else if (currentMode == LSB)
        currentMode = USB;
      else if (currentMode == USB)
      {
        currentMode = AM;
        bfoOn = ssbLoaded = false;
      }
    }
    else
    { // and counterclockwise
      if (currentMode == AM)
      {
        // If you were in AM mode, it is necessary to load SSB patch (avery time)
        loadSSB();
        currentMode = USB;
      }
      else if (currentMode == USB)
        currentMode = LSB;
      else if (currentMode == LSB)
      {
        currentMode = AM;
        bfoOn = ssbLoaded = false;
      }
    }
    // Nothing to do if you are in FM mode
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    useBand();
  }
  elapsedCommand = millis();
}

/**
 * Find a station. The direction is based on the last encoder move clockwise or counterclockwise
 */
void doSeek()
{
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  currentFrequency = rx.getFrequency();
}

/**
 * Sets the audio volume
 */
void doVolume( int8_t v ) {
  if ( v == 1)
    rx.volumeUp();
  else
    rx.volumeDown();

  showVolume();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 * Configures the Softmute parameter
 */
void doSoftMute( int8_t v )  {
  softMuteMaxAttIdx = (v == 1) ? softMuteMaxAttIdx + 1 : softMuteMaxAttIdx - 1;
  if (softMuteMaxAttIdx > 32)
    softMuteMaxAttIdx = 0;
  else if (softMuteMaxAttIdx < 0)
    softMuteMaxAttIdx = 32;

  rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx);
  showSoftMute();
  elapsedCommand = millis();
}

/**
 * Prepares the system to process the desired current command
 */
void prepareCommand(bool *b, void (*showFunc)())
{
  bool tmp = *b;
  disableCommands(); // Disable previous command if active.
  *b = !tmp;
  commandDisabledByUser = (*b) ? false : true;
  showCommandStatus(cmdBand);
  if ( showFunc != NULL)
    showFunc();

  delay(MIN_ELAPSED_TIME);
  elapsedCommand = millis();
}

void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn & (currentMode == LSB || currentMode == USB))
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      rx.setSSBBfo(currentBFO);
      showBFO();
    }
    else if (cmdMode)
      doMode(encoderCount);
    else if (cmdStep)
      doStep(encoderCount);
    else if (cmdAgc)
      doAgc(encoderCount);
    else if (cmdBandwidth)
      doBandwidth(encoderCount);
    else if (cmdVolume)
      doVolume(encoderCount);      
    else if (cmdBand)
      setBand(encoderCount);
    else if (cmdSoftMuteMaxAtt)
      doSoftMute(encoderCount);      
    else
    {
      if ( (seekDirection = (encoderCount == 1)) )
        rx.frequencyUp();
      else
        rx.frequencyDown();
      currentFrequency = rx.getFrequency();
      showFrequency();
    }
    encoderCount = 0;
  }
  else // checks actions from buttons
  {
    uint8_t tm_button = tm.readButtons();
    delay(50);
    if (tm_button == BANDWIDTH_BUTTON)
      prepareCommand(&cmdBandwidth, showBandwidth);
    else if (tm_button == AUDIO_VOLUME)
      prepareCommand(&cmdVolume, showVolume);
    else if (tm_button == BAND_BUTTON)
      prepareCommand(&cmdBand, showBand);
    else if (tm_button == AGC_SWITCH )
      prepareCommand(&cmdAgc, showAgcAtt);
    else if (tm_button ==  STEP_SWITCH)
      prepareCommand(&cmdStep,showStep);
    else if (tm_button == MODE_SWITCH)
      prepareCommand(&cmdMode, NULL);
    else if ( tm_button == SOFT_MUTE) 
      prepareCommand(&cmdSoftMuteMaxAtt, showSoftMute);
    else if (tm_button == SEEK_BUTTON)
      doSeek();
    else if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
    {
      bfoOn = !bfoOn;
      commandDisabledByUser = (bfoOn) ? false : true;
      if ((currentMode == LSB || currentMode == USB))
         showBFO();
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
  }
  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 6)
  {
    rx.getCurrentReceivedSignalQuality();
    int aux = rx.getCurrentRSSI();
    if (rssi != aux)
    {
      rssi = aux;
      showRSSI();
    }
    elapsedRSSI = millis();
  }
  // Disable commands control
  if (  ( ((millis() - elapsedCommand) > ELAPSED_COMMAND) && isCommand() ) || commandDisabledByUser)
  {
    if ((currentMode == LSB || currentMode == USB))
      bfoOn = false;
    showFrequency();
    disableCommands();
    clearStatusDisplay();
    elapsedCommand = millis();
  }
  delay(1);
}
