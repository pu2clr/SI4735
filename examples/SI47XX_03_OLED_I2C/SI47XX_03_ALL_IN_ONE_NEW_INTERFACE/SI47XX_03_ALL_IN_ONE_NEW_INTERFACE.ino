/*
  Under construction... 

  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with an regular OLED/I2C
  You have to install the Tiny4kOLED library to used this sketch 

 
  It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
  regular  comercial  stations. If  you  are  using  the  same  circuit  used  on  examples with OLED and LCD,
  you have to change some buttons wire up. This TFT device takes five pins from Arduino.
  For this reason, it is necessary change the pins of some buttons.
  Fortunately, you can use the ATmega328 analog pins as digital pins.

  The libraries Adafruit_GFX and Adafruit_ST7735 take a lot of memory space from Arduino. 
  You have few space to improve your prototype with standard Arduino Pro Mini.
  However, you can use some approaches:  
  1. Shrink or remove the boot loader from Arduino Pro Mini;
  2. The Arduino Nano e Uno has smaller bootloader than the Arduino Pro Mini
  3. Port this sketch to a bigger board like Arduino Mega or DUE. 


  Features:   AM; SSB; LW/MW/SW; two super band (from 150Khz to 30 MHz); external mute circuit control; Seek (Automatic tuning)
              AGC; Attenuation gain control; SSB filter; CW; AM filter; 1, 5, 10, 50 and 500KHz step on AM and 10Hhz sep on SSB

  Wire up on Arduino UNO, Pro mini
  | Device name               | Device Pin / Description      |  Arduino Pin  |
  | ----------------          | ----------------------------- | ------------  |
  | Display PLED              |                               |               |
  |                           | SDA                           |     A4        |
  |                           | CLK                           |     A5        |
  |     Si4735                |                               |               |
  |                           | RESET (pin 15)                |     12        |
  |                           | SDIO (pin 18)                 |     A4        |
  |                           | SCLK (pin 17)                 |     A5        |
  |     Buttons               |                               |               |
  |                           | (*1)Switch MODE (AM/LSB/AM)   |      4        |
  |                           | (*1)Banddwith                 |      5        |
  |                           | (*1)BAND                      |      6        |
  |                           | (*2)SEEK                      |      7        |
  |                           | (*1)AGC/Attenuation           |     14 / A0   |
  |                           | (*1)STEP                      |     15 / A1   | 
  |                           | VFO/VFO Switch (Encoder)      |     16 / A2   |
  |    Encoder                |                               |               |
  |                           | A                             |       2       |
  |                           | B                             |       3       |

  (*1) You have to press the push button and after, rotate the encoder to select the parameter.
       After you activate a command by pressing a push button, it will keep active for 2,5 seconds. 
  (*2) The SEEK direction is based on the last movement of the encoder. If the last movement of 
       the encoder was clockwise, the SEEK will be towards the upper limit. If the last movement of 
       the encoder was counterclockwise, the SEEK direction will be towards the lower limit.  
  (*3) You might need to switch from 8 to 9  depending of your ST7735 device     

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By PU2CLR, Ricardo; and Jim Reagan W09CHL,  Sep  2020.
*/

#include <SI4735.h>
#include <Tiny4kOLED.h>
#include "Rotary.h"

// Test it with patch_init.h or patch_full.h. Do not try load both.
#include "patch_init.h" // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h


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
#define BANDWIDTH_BUTTON 5    // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz
#define BAND_BUTTON 6         // Band switch button
#define SEEK_BUTTON 7         // Previous band
#define AGC_SWITCH 14         // Pin A0 - Switch AGC ON/OF
#define STEP_SWITCH 15        // Pin A1 - Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
#define BFO_SWITCH 16         // Pin A2 - Used to select the enconder control (BFO or VFO)
#define AUDIO_MUTE 1          // External AUDIO MUTE circuit control

#define MIN_ELAPSED_TIME 250
#define MIN_ELAPSED_RSSI_TIME 150
#define ELAPSED_COMMAND 2500  // time to turn off the last command controlled by encoder
#define DEFAULT_VOLUME 40     // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1
#define CLEAR_BUFFER(x)  (x[0] = '\0');

bool bfoOn = false;
bool ssbLoaded = false;

// AGC and attenuation control
int8_t agcIdx = 0;
uint8_t disableAgc = 0;
uint8_t agcNdx = 0;

bool cmdBand = false;
bool cmdBfo = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;

int currentBFO = 0;
uint8_t seekDirection = 1;   // Tells the SEEK direction (botton or upper limit)

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;

uint8_t currentBFOStep = 10;

uint8_t bwIdxSSB = 2;
const char * bandwitdthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char * bandwitdthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char * bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

uint16_t currentStep = 1;

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
    {"SW1", SW_BAND_TYPE, 150, 30000, 7100, 1}, // Here and below: 150KHz to 30MHz
    {"SW2", SW_BAND_TYPE, 150, 30000, 9600, 5},
    {"SW3", SW_BAND_TYPE, 150, 30000, 11940, 5},
    {"SW4", SW_BAND_TYPE, 150, 30000, 13600, 5},
    {"SW5", SW_BAND_TYPE, 150, 30000, 14200, 1},
    {"SW5", SW_BAND_TYPE, 150, 30000, 15300, 5},
    {"SW6", SW_BAND_TYPE, 150, 30000, 17600, 5},
    {"SW7", SW_BAND_TYPE, 150, 30000, 21100, 1},
    {"SW8", SW_BAND_TYPE, 150, 30000, 22525, 5},
    {"SW9", SW_BAND_TYPE, 150, 30000, 28400, 1}};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;


int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
int idxStep = 0;


uint8_t rssi = 0;
uint8_t snr = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);


SI4735 rx;

void setup()
{
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(BANDWIDTH_BUTTON, INPUT_PULLUP);
  pinMode(BAND_BUTTON, INPUT_PULLUP);
  pinMode(SEEK_BUTTON, INPUT_PULLUP);
  pinMode(BFO_SWITCH, INPUT_PULLUP);
  pinMode(AGC_SWITCH, INPUT_PULLUP);
  pinMode(STEP_SWITCH, INPUT_PULLUP);
  pinMode(MODE_SWITCH, INPUT_PULLUP);

  // uncomment the line below if you have external audio mute circuit
  // rx.setAudioMuteMcuPin(AUDIO_MUTE);


  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT6X8);
  
  // Splash - Change it for your introduction text.
  oled.setCursor(41, 0);
  oled.print("SI4735");
  oled.setCursor(21, 1);
  oled.print("Arduino Library");
  delay(500);
  oled.setCursor(7, 2);
  oled.print("All in One Radio");
  delay(500);
  oled.setCursor(9, 3);
  oled.print("By PU2CLR & W0CHL");
  delay(5000);
  
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  // rx.setup(RESET_PIN, 1); // Starts FM mode and ANALOG audio mode
  // rx.setup(RESET_PIN, -1, 1, SI473X_ANALOG_AUDIO); // Starts FM mode and ANALOG audio mode.
  rx.setup(RESET_PIN, -1, 1, SI473X_ANALOG_DIGITAL_AUDIO); // Starts FM mode and ANALOG and DIGITAL audio mode.

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  rx.setVolume(volume);
  showStatus();
}


/**
 *  Set all command flags to false
 *  When all flags are disabled (false), the encoder controls the frequency
 */
void disableCommands() {
  cmdBand = false;
  cmdBfo = false;
  bfoOn = false;
  cmdVolume = false;
  cmdAgc = false;
  cmdBandwidth = false;
  cmdStep = false;
  cmdMode = false;
}


/**
 *   Reads encoder via interrupt
 *   Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
 */
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();

  if (encoderStatus)
    encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
}

/**
 *  Shows frequency information on Display
 */
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
  else if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
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

  if ( !bfoOn )
    freqDisplay = String((float)currentFrequency / divider, decimals);
  else
    freqDisplay = ">" + String((float)currentFrequency / divider, decimals) + "<";

  oled.setCursor(39, 0);
  oled.print("        ");
  oled.setCursor(39, 0);
  oled.print(freqDisplay);

  if (currentFrequency < 520 )
    bandMode = "LW  ";
  else
    bandMode = bandModeDesc[currentMode];

  oled.setCursor(1, 0);
  oled.print(bandMode);

  oled.setCursor(95, 0);
  oled.print(unit);
}


/**
 *  This function is called by the seek function process.
 */
void showFrequencySeek(uint16_t freq)
{
  currentFrequency = freq;
  showFrequency();
}

/**
 *   Show some basic information on display
 */
void showStatus()
{
  oled.clear();
  showFrequency();
  showStep();
  showBandwitdth();
  showAgcAtt();
  showRSSI();
  showVolume();
}

/**
 * Shows the current Bandwitdth status
 */
void showBandwitdth() {

    char bufferDisplay[15];
    
    // Bandwidth
    if (currentMode == LSB || currentMode == USB || currentMode == AM) {
      char * bw;

      if (currentMode == AM) 
        bw = (char *) bandwitdthAM[bwIdxAM];
      else 
        bw = (char *) bandwitdthSSB[bwIdxSSB];
      sprintf(bufferDisplay, "BW: %s KHz", bw);
    } 
    else {
      bufferDisplay[0] = '\0';
    }

   oled.setCursor(1, 3);
   oled.print("           ");
   oled.setCursor(1, 3);
   oled.print(bufferDisplay);
}

/**
 *  Shows the current RSSI and SNR status
 */
void showRSSI()
{
  int bars = ((rssi / 10.0) / 2.0) + 1;

  oled.setCursor(81, 3);
  oled.print("       ");
  oled.setCursor(81, 3);
  oled.print("S:");
  if ( bars > 5 )  {
    bars = 5;
  }
  for (int i = 0; i < bars; i++)
    oled.print(">");

  if ( currentMode == FM) {
    oled.setCursor(1, 3);
    oled.print((rx.getCurrentPilot()) ? "STEREO   " : "MONO     ");
  }
}

 /**
  *  Shows the current AGC and Attenuation status
  */
 void showAgcAtt() {
      char sAgc[15];
      rx.getAutomaticGainControl();
      if (agcNdx == 0 && agcIdx == 0)
        strcpy(sAgc, "AGC ON");
      else
      sprintf(sAgc, "ATT: %2d", agcNdx);

      // Show AGC Information
      rx.getAutomaticGainControl();
      oled.setCursor(1, 1);
      oled.print(sAgc);      
 }


/**
 *  Shows the current step
 */
void showStep() {
  oled.setCursor(80, 1);
  oled.print("        ");
  oled.setCursor(80, 1);
  oled.print("St: ");
  oled.print(currentStep);
}

/**
 * Shows the current BFO value
 */
void showBFO()
{
  String bfo;

  if (currentBFO > 0)
    bfo = "+" + String(currentBFO);
  else
    bfo = String(currentBFO);

  oled.setCursor(1, 2);
  oled.print("         ");
  oled.setCursor(1, 2);
  oled.print("BFO:");
  oled.print(bfo);
  oled.print("Hz ");

  oled.setCursor(81, 2);
  oled.print("       ");
  oled.setCursor(81, 2);
  oled.print("St: ");
  oled.print(currentBFOStep);
  showFrequency();
}


/*
   Shows the volume level on LCD
*/
void showVolume()
{
  /* TO DO
  oled.setCursor(61, 3);
  oled.print("  ");
  oled.setCursor(61, 3);
  oled.print(rx.getCurrentVolume());
  */
}


/**
 *  Sets Band up (1) or down (!1)
 */
void setBand(int8_t up_down) {
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
 *  This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
 *  SSB mode.
 *  See also loadPatch implementation in the SI4735 Arduino Library (SI4735.h/SI4735.cpp) 
 */
void loadSSB()
{

  oled.setCursor(1, 2);
  oled.print("Loading SSB");
  
  rx.reset();
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  rx.setI2CFastMode(); // Recommended
  // rx.setI2CFastModeCustom(500000); // It is a test and may crash.
  rx.downloadPatch(ssb_patch_content, size_content);
  rx.setI2CStandardMode(); // goes back to default (100KHz)

  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  rx.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
  showStatus();
}

/**
 *  Switch the radio to current band
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
    rx.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : band[bandIdx].currentStep); // Max 10KHz for spacing
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  rssi = 0;
  showStatus();
}


/**
 *  Switches the Bandwidth
 */
void doBandwidth(int8_t v) {
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB = ( v == 1) ? bwIdxSSB + 1 : bwIdxSSB - 1;

    if (bwIdxSSB > 5)
      bwIdxSSB = 0;
    else if ( bwIdxSSB < 0 )
      bwIdxSSB = 5;

    rx.setSSBAudioBandwidth(bwIdxSSB);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
      rx.setSBBSidebandCutoffFilter(0);
    else
      rx.setSBBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    bwIdxAM = ( v == 1) ? bwIdxAM + 1 : bwIdxAM - 1;

    if (bwIdxAM > 6)
      bwIdxAM = 0;
    else if ( bwIdxAM < 0)
      bwIdxAM = 6;

    rx.setBandwidth(bwIdxAM, 1);
  }
  showBandwitdth();
  elapsedCommand = millis();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.

}

/**
 *  Deal with AGC and attenuattion
 */
void doAgc(int8_t v) {

  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;
  if (agcIdx < 0 )
    agcIdx = 35;
  else if ( agcIdx > 35)
    agcIdx = 0;

  disableAgc = (agcIdx > 0);
  agcNdx = agcIdx;

  rx.setAutomaticGainControl(disableAgc, agcNdx);
  showAgcAtt();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *  Gets the current step index.
 */
int getStepIndex(int st) {
  for (int i = 0; i < lastStep; i++) {
    if ( st == tabStep[i] ) return i;
  }
  return 0;
}

/**
 *  Switches the current step
 */
void doStep(int8_t v) {
  idxStep = ( v == 1 ) ? idxStep + 1 : idxStep - 1;
  if ( idxStep > lastStep)
     idxStep = 0;
  else if ( idxStep < 0 )
     idxStep = lastStep;

  currentStep = tabStep[idxStep];

  rx.setFrequencyStep(currentStep);
  band[bandIdx].currentStep = currentStep;
  rx.setSeekAmSpacing((currentStep > 10) ? 10 : currentStep); // Max 10KHz for spacing
  showStep();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *  Switches to the AM, LSB or USB modes
 */
void doMode(int8_t v) {
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

  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *  Find a station. The direction is based on the last encoder move clockwise or counterclockwise
 */
void doSeek() {
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  currentFrequency = rx.getFrequency();
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
      elapsedCommand = millis();
    }
    else if ( cmdMode ) 
      doMode(encoderCount);
    else if (cmdStep)
      doStep(encoderCount);
    else if (cmdAgc )
      doAgc(encoderCount);
    else if (cmdBandwidth)
      doBandwidth(encoderCount);
    else if (cmdBand)
      setBand(encoderCount);
    else
    {
      if (encoderCount == 1) {
        rx.frequencyUp();
        seekDirection = 1;
      }
      else {
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
    if (digitalRead(BANDWIDTH_BUTTON) == LOW) {
      cmdBandwidth = !cmdBandwidth;
      elapsedCommand = millis();
    }
    else if (digitalRead(BAND_BUTTON) == LOW) {
      cmdBand = !cmdBand;
      elapsedCommand = millis();
    }
    else if (digitalRead(SEEK_BUTTON) == LOW) {
      doSeek();
    }
    else if (digitalRead(BFO_SWITCH) == LOW) {
      bfoOn = !bfoOn;
      cmdBfo = false;
      elapsedCommand = millis();
      delay(MIN_ELAPSED_TIME);
    }
    else if (digitalRead(AGC_SWITCH) == LOW) {
        cmdAgc = !cmdAgc;
        elapsedCommand = millis();
    }
    else if (digitalRead(STEP_SWITCH) == LOW) {
        cmdStep = !cmdStep;
        elapsedCommand = millis();
    }
    else if (digitalRead(MODE_SWITCH) == LOW) {
        cmdMode = !cmdMode;
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
  if ( (millis() - elapsedCommand) > ELAPSED_COMMAND ) {
    if ( cmdBfo ) {
      bfoOn = cmdBfo = false;
      showFrequency();
    }
    disableCommands();
    elapsedCommand = millis();
  }

  // delay(5);
}
