/*
  Under construction... 
  
  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT ST7735 based device.
  The Arduino library used to control that display device is TFT_ST7735 from Bodmer:  https://github.com/Bodmer/TFT_ST7735
  Please, install that before start working  with this sketch.

  It is a complete radio capable to tune FM/RDS; LW, MW, SW on AM and SSB mode and also receive the
  regular comercial stations. If you are using the same circuit used on examples with OLED and LCD,
  you have to change some buttons wire up. This TFT device takes five pins from Arduino.
  For this reason, it is necessary change the pins of some buttons.
  Fortunately, you can use the ATmega328 analog pins as digital pins.

  Features:   FM/RDS; AM; SSB; LW/MW/SW; two super band (from 150kHz to 30 MHz); external mute circuit control; Seek (Automatic tuning)
              AGC; Attenuation gain control; SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  It is important to know the SSB support works on SI4735-D60 and SI4732-A10 devices.

  Wire up on Arduino UNO, Pro mini and SI4735-D60

  | Device name               | Device Pin / Description  |  Arduino Pin  |
  | ----------------          | --------------------      | ------------  |
  | Display TFT               |                           |               |
  |                           | RST (RESET)               |      8        |
  |                           | RS or DC                  |      9        |
  |                           | CS or SS                  |     10        |
  |                           | SDI / SDA / MOSI          |     11        |
  |                           | CLK                       |     13        |
  |     Si4735                |                           |               |
  |                           | (*1) RESET (pin 15)       |     12        |
  |                           | (*1) SDIO (pin 18)        |     A4        |
  |                           | (*1) SCLK (pin 17)        |     A5        |
  |                           | (*2) SEN (pin 16)         |    GND        |  
  |     Buttons               |                           |               |
  |                           | Switch MODE (AM/LSB/AM)   |      4        |
  |                           | Banddwith                 |      5        |
  |                           | Band Switch               |      6        |
  |                           | Seek Station              |      7        |
  |                           | AGC ON/OF                 |     14 / A0   |
  |                           | Frequency Step            |     15 / A1   |
  |                           | VFO/VFO Switch            |     16 / A2   |
  |    Encoder                |                           |               |
  |                           | A                         |       2       |
  |                           | B                         |       3       |

  (*1) If you are using the SI4732-A10, check the corresponding pin numbers.  
  (*2) If you are using the SI4735-D60, connect the SEN pin to the ground; 
       If you are using the SI4732-A10, connect the SEN pin to the +Vcc. 

  User manual

  1. BAND, MODE, AGC/Attenuation, banddwith and STEP comands

  Press the desired command push button and after, rotate the encoder to select the option you want. For example:
  To switch the band, press the band button and then rotate the encoder clockwise or counterclockwise.
  The display will show you the current band. If you are on AM mode, you can use also the encoder push
  button to select the band (on SSB mode the encoder push button is used to switch the VFO and BFO).

  To switch the mode (AM, LSB or USB), press Mode push button and then rotate the encoder.

  The same idea you can use to AGC/Attenuation, STEP and Banddwith.

  2. SEEK COMMAND

  The seek button should be used to find a station. The seek direction is based on the last encoder movement.
  If clockwise, the seek will go up; if counterclockwise, the seek will go down.


  3. VFO/VFO Switch

  To control the VFO and BFO, used the encoder push button. The display will show you if you are using VFO or BFO.
  It is importante to say the this action work only on SSB mode.

  Tip: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time.
       If you do that, you might alternate the command status (enable and disable) randomly.

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/


  By PU2CLR, Ricardo,  Oct  2020.
*/

#include <SI4735.h>
#include <SPI.h>
#include "./TFT_ST7735/TFT_ST7735.h"

#include "Rotary.h"

#include <patch_ssb_compressed.h> // Compressed SSB patch version (saving almost 1KB)

const uint16_t size_content = sizeof ssb_patch_content; // See ssb_patch_content.h
const uint16_t cmd_0x15_size = sizeof cmd_0x15;         // Array of lines where the 0x15 command occurs in the patch content.const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h


#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons controllers
#define MODE_SWITCH 4         // Switch MODE (Am/LSB/USB)
#define BANDWIDTH_BUTTON 5    // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define BAND_BUTTON 6         // Band switch button
#define SEEK_BUTTON 7         // Previous band
#define AGC_SWITCH 14         // Pin A0 - Switch AGC ON/OF
#define STEP_SWITCH 15        // Pin A1 - Used to select the increment or decrement frequency step (1, 5 or 10 kHz)
#define ENCODER_PUSH_BUTTON 16         // Pin A2 - Used to select the enconder control (BFO or VFO)
#define AUDIO_MUTE 1          // External AUDIO MUTE circuit control

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 150
#define ELAPSED_COMMAND 3000 // time to turn off the last command controlled by encoder
#define DEFAULT_VOLUME 50    // change it for your favorite sound volume

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

int currentBFO = 0;
uint8_t seekDirection = 1;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency = 0;

const uint8_t currentBFOStep = 20;


const char * bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;


typedef struct
{
  uint8_t idx;      // SI473X device bandwidth index
  const char *desc; // bandwidth description
} Bandwidth;

int8_t bwIdxSSB = 4;
Bandwidth bandwidthSSB[] = {{4, "0.5"}, // 0
  {5, "1.0"}, // 1
  {0, "1.2"}, // 2
  {1, "2.2"}, // 3
  {2, "3.0"}, // 4
  {3, "4.0"}
}; // 5

int8_t bwIdxAM = 4;
Bandwidth bandwidthAM[] = {{4, "1.0"},
  {5, "1.8"},
  {3, "2.0"},
  {6, "2.5"},
  {2, "3.0"},
  {1, "4.0"},
  {0, "6.0"}
};


int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
int idxStep = 0;

uint16_t currentStep = 1;

char bufferDisplay[10]; // Useful to handle string
char bufferFreq[10];
char bufferBFO[5];
char bufferStepVFO[5];
char bufferBW[5];
char bufferAGC[5];
char bufferBand[5];
char bufferStereo[3];

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
   Actually, except FM (VHF), the other bands cover the entire LW / MW and SW spectrum.
   Only the default frequency and step is changed. You can change this setup.
*/
Band band[] = {
  {"FM ", FM_BAND_TYPE, 6400, 10800, 10390, 10},
  {"MW ", MW_BAND_TYPE, 150, 1720, 810, 10},
  {"SW1", SW_BAND_TYPE, 150, 30000, 7100, 1}, // Here and below: 150kHz to 30MHz
  {"SW2", SW_BAND_TYPE, 150, 30000, 9600, 5},
  {"SW3", SW_BAND_TYPE, 150, 30000, 11940, 5},
  {"SW4", SW_BAND_TYPE, 150, 30000, 13600, 5},
  {"SW5", SW_BAND_TYPE, 150, 30000, 14200, 1},
  {"SW5", SW_BAND_TYPE, 150, 30000, 15300, 5},
  {"SW6", SW_BAND_TYPE, 150, 30000, 17600, 5},
  {"SW7", SW_BAND_TYPE, 150, 30000, 21100, 1},
  {"SW8", SW_BAND_TYPE, 150, 30000, 28400, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

uint8_t rssi = 0;
uint8_t snr = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
TFT_ST7735 tft = TFT_ST7735();
SI4735 rx;

void setup()
{
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BANDWIDTH_BUTTON, INPUT_PULLUP);
  pinMode(BAND_BUTTON, INPUT_PULLUP);
  pinMode(SEEK_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(MODE_SWITCH, INPUT_PULLUP);


  // Comment the line below if you do not have external audio mute circuit
  rx.setAudioMuteMcuPin(AUDIO_MUTE);

  // Use this initializer if using a 1.8" TFT screen:
  tft.begin();
  tft.setRotation(3); 
  tft.fillScreen(TFT_BLACK); //  Clear Screen
  
  showTemplate();

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  // rx.setup(RESET_PIN, 1); // Starts FM mode and ANALOG audio mode
  rx.setup(RESET_PIN, -1, 1, SI473X_ANALOG_AUDIO); // Starts FM mode and ANALOG audio mode.
  // rx.setup(RESET_PIN, -1, 1, SI473X_ANALOG_DIGITAL_AUDIO); // Starts FM mode and ANALOG and DIGITAL audio mode.

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  rx.setVolume(volume);
  showStatus();
}

/**
    Set all command flags to false
    When all flags are disabled (false), the encoder controls the frequency
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
}



/*
   Shows the static content on  display
*/
void showTemplate()
{

}


/*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
void printValue(byte col, byte line, char *oldValue, char *newValue, uint16_t color, int font, uint8_t space) {
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
      tft.drawChar(c, line, *pOld, TFT_BLACK, TFT_BLACK, font);
      tft.drawChar(c, line, *pNew, color,TFT_BLACK,font);
    }
    pOld++;
    pNew++;
    c += space;
  }

  // Is there anything else to erase?
  while (*pOld)
  {
    tft.drawChar(c, line, *pOld, TFT_BLACK, TFT_BLACK, font);
    pOld++;
    c += space;
  }

  // Is there anything else to print?
  while (*pNew)
  {
    tft.drawChar(c, line, *pNew, color, TFT_BLACK, font);
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

  // sprintf(tmp, "%5.5u", currentFrequency);

  if (rx.isCurrentTuneFM())
  {
  }
  else
  {

  }

  color = (bfoOn && (currentMode == LSB || currentMode == USB)) ?  TFT_CYAN : TFT_YELLOW;
  printValue(10, 10, bufferFreq, bufferDisplay, color, 1, 20);
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


}

/**
    Shows the current AGC and Attenuation status
*/
void showAgcAtt()
{


}


/**
    Shows the current step
*/
void showStep() {

}


/**
   Shows the current Bandwidth status
*/
void showBandwidth() {

}


char *stationName;
char bufferStatioName[10];

long stationNameElapsed = millis();

/**
   TODO: process RDS Dynamic PS or Scrolling PS
*/
void showRDSStation() {

}


void checkRDS() {
  rx.getRdsStatus();
  if (rx.getRdsReceived()) {
    if (rx.getRdsSync() && rx.getRdsSyncFound() ) {
      stationName = rx.getRdsText0A();
      if ( (millis() - stationNameElapsed) ) {
        if ( stationName != NULL && rx.getRdsNewBlockA() )   showRDSStation();
        stationNameElapsed = millis();
      }
    }
  }
}

/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
 
}

void showBFOTemplate(uint16_t color)
{
 
}

void clearBFO() {

}

void showBFO()
{

}


/**
   Sets Band up (1) or down (!1)
*/
void setBand(uint8_t up_down) {
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStep = currentStep;
  if ( up_down == 1)
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;
  useBand();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}


/**
    This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
    SSB mode.
    See also loadPatch implementation in the SI4735 Arduino Library (SI4735.h/SI4735.cpp)
*/
void loadSSB()
{
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  // rx.setI2CFastMode(); // Recommended
  // rx.setI2CFastModeCustom(600000); // It is a test and may crash.
  rx.downloadCompressedPatch(ssb_patch_content, size_content, cmd_0x15, cmd_0x15_size);
  // rx.setI2CStandardMode(); // goes back to default (100kHz)

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

/*
   Switch the radio to current band
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
    rx.setRdsConfig(1, 2, 2, 2, 2);
  }
  else
  {
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor( (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);

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
    rx.setAmSoftMuteMaxAttenuation(0); // Disable Soft Mute for AM or SSB
    rx.setAutomaticGainControl(disableAgc, agcNdx);
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    rx.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : band[bandIdx].currentStep); // Max 10kHz for spacing
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  // idxStep = getStepIndex(currentStep);
  rssi = 0;
  clearBFO();
  showStatus();
}

/**
    Deal with AGC and attenuattion
*/
void doAgc(int8_t v)
{
  /*
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
  */
}


/**
    Switches the current step
*/
void doStep(int8_t v) {
  /*
  idxStep = ( v == 1 ) ? idxStep + 1 : idxStep - 1;
  if ( idxStep > lastStep)
    idxStep = 0;
  else if ( idxStep < 0 )
    idxStep = lastStep;

  currentStep = tabStep[idxStep];

  rx.setFrequencyStep(currentStep);
  band[bandIdx].currentStep = currentStep;
  rx.setSeekAmSpacing((currentStep > 10) ? 10 : currentStep); // Max 10kHz for spacing
  showStep();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
  */
}


/**
    Find a station. The direction is based on the last encoder move clockwise or counterclockwise
*/
void doSeek() {
  /*
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  currentFrequency = rx.getFrequency();
  */
}


/**
    Switches to the AM, LSB or USB modes
*/
void doMode(int8_t v)
{
  
  bufferBFO[0] = bufferFreq[0] - '\0';
  if (currentMode != FM)
  {
    if (v == 1)  { // clockwise
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
    } else { // and counterclockwise
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

}

/**
    Switches the Bandwidth
*/
void doBandwidth(int8_t v) {
  /*
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB = ( v == 1) ? bwIdxSSB + 1 : bwIdxSSB - 1;

    if (bwIdxSSB > 5)
      bwIdxSSB = 0;
    else if ( bwIdxSSB < 0 )
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
    bwIdxAM = ( v == 1) ? bwIdxAM + 1 : bwIdxAM - 1;

    if (bwIdxAM > 6)
      bwIdxAM = 0;
    else if ( bwIdxAM < 0)
      bwIdxAM = 6;

    rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
  }
  showBandwidth();
  elapsedCommand = millis();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  */
}

void doBFO()
{
  /*
  bfoOn = !bfoOn;
  if ((currentMode == LSB || currentMode == USB))
    showBFO();
  CLEAR_BUFFER(bufferFreq);
  showFrequency();
  delay(MIN_ELAPSED_TIME);
  elapsedCommand = millis();
  */
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
    else if (cmdBand)
      setBand(encoderCount);
    else
    {
      if (encoderCount == 1)
      {
        rx.frequencyUp();
        seekDirection = 1;
      }
      else
      {
        rx.frequencyDown();
        seekDirection = 0;
      }
      // Show the current frequency only if it has changed
      currentFrequency = rx.getFrequency();
    }
    showFrequency();
    encoderCount = 0;
  }
  else
  {
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
    {
      cmdBandwidth = !cmdBandwidth;
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
    else if (digitalRead(BAND_BUTTON) == LOW)
    {
      cmdBand = !cmdBand;
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
    else if (digitalRead(SEEK_BUTTON) == LOW)
    {
      doSeek();
    }
    else if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
    {
      if (currentMode == LSB || currentMode == USB)
        doBFO();
      else
      {
        cmdBand = !cmdBand;
        elapsedCommand = millis();
      }
      delay(300);
    }
    else if (digitalRead(AGC_SWITCH) == LOW)
    {
      cmdAgc = !cmdAgc;
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
    else if (digitalRead(STEP_SWITCH) == LOW)
    {
      cmdStep = !cmdStep;
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
    else if (digitalRead(MODE_SWITCH) == LOW)
    {
      cmdMode = !cmdMode;
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
      snr = rx.getCurrentSNR();
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  // Disable commands control
  if ((millis() - elapsedCommand) > ELAPSED_COMMAND)
  {
    if ((currentMode == LSB || currentMode == USB)) {
      bfoOn = false;
      showBFO();
      CLEAR_BUFFER(bufferFreq);
      showFrequency();
    }
    disableCommands();
    elapsedCommand = millis();
  }

  if ( currentMode == FM) {
    if ( currentFrequency != previousFrequency ) {
      bufferStatioName[0] = stationName[0] = '\0';
      showRDSStation();
      previousFrequency = currentFrequency;
    }
    checkRDS();
  }

  delay(1);
}
