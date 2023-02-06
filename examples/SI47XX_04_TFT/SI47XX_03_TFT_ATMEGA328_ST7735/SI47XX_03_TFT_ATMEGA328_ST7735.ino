/*
  ATTENTION: Use the MiniCore board manager with "LTO Enabled" and "No bootloader" options selected.
             See https://github.com/MCUdude/MiniCore


  Install the libraries Adafruit_GFX, Adafruit_ST7735 and SI4735

  You can use the arduino-cli instead Arduino IDE: Using the shell, from example folder, 
  run: arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  -u -p </dev/XXXX>  ./SI47XX_04_TFT/SI47XX_03_TFT_ATMEGA328_ST7735 
  
  Where /dev/XXXX is the port of your computer (COM1, COM2 ... if you are using Windows OS)
  Run arduino-cli board list to know the port connected to the board.


  This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT ST7735 1.8"

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735  and  the
  "PU2CLR SI4735 Arduino Library" working with the TFT ST7735 display. It is not the purpose of this prototype
  to provide you a beautiful interface. To be honest, I think you can do it better than me.

  This sketch does not use sprintf or any function to deal with value formatting.  It helped to save Arduino memory.

  It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
  regular  comercial  stations. If  you  are  using  the  same  circuit  used  on  examples with OLED and LCD,
  you have to change some buttons wire up. This TFT device takes five pins from Arduino.
  For this reason, it is necessary change the pins of some buttons.
  Fortunately, you can use the ATmega328 analog pins as digital pins.

  The libraries Adafruit_GFX and Adafruit_ST7735 and SSB patch take a lot of memory space from Arduino.
  You have few space to improve your prototype with standard Arduino Pro Mini.
  However, you can use some approaches:
  1. Shrink or remove the boot loader from Arduino Pro Mini;
  2. The Arduino Nano and Uno have smaller bootloader than the Arduino Pro Mini
  3. Port this sketch to a bigger board like Arduino Mega or DUE.


  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.


  Features:   AM; SSB; LW/MW/SW; two super band (from 150kHz to 30 MHz); external mute circuit control; Seek (Automatic tuning)
              AGC; Attenuation gain control; SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  Wire up on Arduino UNO, Pro mini and SI4735-D60

  | Device name               | Device Pin / Description      |  Arduino Pin  |
  | ----------------          | ----------------------------- | ------------  |
  | Display TFT               |                               |               |
  |                           | RST (RESET)                   |     (*3) 8    |
  |                           | RS or DC or A0                |     (*3) 9    |
  |                           | CS or SS                      |     10        |
  |                           | SDI or SDA or MOSI            |     11        |
  |                           | CLK                           |     13        |
  |                           | BL                            |    +VCC       |
  |     Si4735                |                               |               |
  |                           | (*4) RESET (pin 15)           |     12        |
  |                           | (*4) SDIO (pin 18)            |     A4        |
  |                           | (*4) SCLK (pin 17)            |     A5        |
  |                           | (*5) SEN (pin 16)             |    GND        |
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
  (*4) - If you are using the SI4732-A10, check the corresponding pin numbers.
  (*5) - If you are using the SI4735-D60, connect the SEN pin to the ground;
         If you are using the SI4732-A10, connect the SEN pin to the +Vcc.

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Jim Reagan's schematic: https://github.com/JimReagans/Si4735-radio-PCB-s-and-bandpass-filter

  By PU2CLR, Ricardo; and W09CHL, Jim Reagan;  Sep  2020.

*/

#include <SI4735.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Fonts/FreeMonoBold18pt7b.h>  

#include <SPI.h>
#include "Rotary.h"

#include <patch_ssb_compressed.h> // Compressed SSB patch version (saving almost 1KB)
const uint16_t size_content = sizeof ssb_patch_content; // See ssb_patch_content.h
const uint16_t cmd_0x15_size = sizeof cmd_0x15;         // Array of lines where the 0x15 command occurs in the patch content.

// TFT ST7735 based device pin setup
#define TFT_RST 8  // You might need to switch from 8 to 9 depending of your ST7735 device
#define TFT_DC 9   // You might need to switch from 9 to 8 depending of your ST7735 device
// #define TFT_DC 8 
// #define TFT_RST 9  
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
#define MODE_SWITCH 4         // Switch MODE (Am/LSB/USB)
#define BANDWIDTH_BUTTON 5    // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz
#define BAND_BUTTON 6         // Band switch button
#define SEEK_BUTTON 7         // Previous band
#define AGC_SWITCH 14         // Pin A0 - Switch AGC ON/OF
#define STEP_SWITCH 15        // Pin A1 - Used to select the increment or decrement frequency step (1, 5 or 10 kHz)
#define BFO_SWITCH 16         // Pin A2 - Used to select the enconder control (BFO or VFO)
#define AUDIO_MUTE 1          // External AUDIO MUTE circuit control

#define MIN_ELAPSED_TIME 300
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
int8_t agcNdx = 0;

bool cmdBand = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;
bool commandDisabledByUser = false;

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
                              {3, "4.0"}}; // 5

int8_t bwIdxAM = 4;
Bandwidth bandwidthAM[] = {{4, "1.0"},
                             {5, "1.8"},
                             {3, "2.0"},
                             {6, "2.5"},
                             {2, "3.0"},
                             {1, "4.0"},
                             {0, "6.0"}};


const char * bandModeDesc[] = {"   ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

uint16_t currentStep = 1;

char bufferDisplay[15]; // Useful to handle string
char bufferFreq[15];
char bufferBFO[15];
char bufferStepVFO[10];
char bufferBW[15];
char bufferAGC[10];
char bufferBand[15];
char bufferStereo[10];
char bufferUnt[5];
char bufferRssi[8];

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
    {"SW8", SW_BAND_TYPE, 150, 30000, 28400, 1}};

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
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
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

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  showTemplate();

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  rx.getDeviceI2CAddress(RESET_PIN);

  // rx.setup(RESET_PIN, 1); // Starts FM mode and ANALOG audio mode
  rx.setup(RESET_PIN, 0, 1, SI473X_ANALOG_AUDIO); // Starts FM mode and ANALOG audio mode.
 
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
  bfoOn = false;
  cmdVolume = false;
  cmdAgc = false;
  cmdBandwidth = false;
  cmdStep = false;
  cmdMode = false;
  commandDisabledByUser = false;
}

/**
 * Checks if the system is in command condition
 */
inline bool isCommand()
{
  return (cmdBand | cmdMode | cmdStep | cmdBandwidth | cmdAgc | cmdVolume | bfoOn);
}

/**
 *  Shows the static content on  display
 */
void showTemplate()
{
  int maxX1 = tft.width() - 2;
  int maxY1 = tft.height() - 5;
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(2, 2, maxX1, maxY1, ST77XX_YELLOW);
  tft.drawLine(2, 40, maxX1, 40, ST77XX_YELLOW);
  tft.drawLine(2, 60, maxX1, 60, ST77XX_YELLOW);
}

/**
 * Converts a number to a char string and places leading zeros. 
 * It is useful to mitigate memory space used by sprintf or generic similar function 
 * 
 * value  - value to be converted
 * strValue - the value will be receive the value converted
 * len -  final string size (in bytes) 
 * dot - the decimal or tousand separator position
 * separator -  symbol "." or "," 
 */
char* convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator)
{
  char d;
  for (int i = (len - 1); i >= 0; i--)
  {
    d = value % 10;
    value = value / 10;
    strValue[i] = d + 48;
  }
  strValue[len] = '\0';
  if (dot > 0)
  {
    for (int i = len; i >= dot; i--)
    {
      strValue[i + 1] = strValue[i];
    }
    strValue[dot] = separator;
  }

  if (strValue[0] == '0')
  {
    strValue[0] = ' ';
    if (strValue[1] == '0')
      strValue[1] = ' ';
  }
  return strValue;
}


/**
 * Prevents blinking during the frequency display.
 * Erases the old digits if it has changed and print the new digit values.
 */
void printValue(int col, int line, char *oldValue, char *newValue, uint8_t space, uint16_t color, uint8_t txtSize)
{
  int c = col;
  char *pOld;
  char *pNew;

  tft.setTextSize(txtSize);
  pOld = oldValue;
  pNew = newValue;

  // prints just changed digits
  while (*pOld && *pNew)
  {
    if (*pOld != *pNew)
    {
      // Erases olde value
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(c, line);
      tft.print(*pOld);
      // Writes new value
      tft.setTextColor(color);
      tft.setCursor(c, line);
      tft.print(*pNew);
    }
    pOld++;
    pNew++;
    c += space;
  }

  // Is there anything else to erase?
  tft.setTextColor(ST77XX_BLACK);
  while (*pOld)
  {
    tft.setCursor(c, line);
    tft.print(*pOld);
    pOld++;
    c += space;
  }

  // Is there anything else to print?
  tft.setTextColor(color);
  while (*pNew)
  {
    tft.setCursor(c, line);
    tft.print(*pNew);
    pNew++;
    c += space;
  }
  // Save the current content to be tested next time
  strcpy(oldValue, newValue);
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
  uint16_t color;

  if (rx.isCurrentTuneFM())
  {
    convertToChar(currentFrequency, bufferDisplay, 5, 3, ',');
    color = ST7735_CYAN;
  }
  else
  {
    convertToChar(currentFrequency, bufferDisplay, 5, 0, '.');
    color = (bfoOn && (currentMode == LSB || currentMode == USB)) ? ST7735_WHITE : ST77XX_YELLOW;
  }
  bufferDisplay[5] = '\0';
  
  tft.setFont(&FreeMonoBold18pt7b);
  printValue(22, 32, bufferFreq, bufferDisplay, 22, color, 1);
  tft.setFont(NULL);
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
  char *unt;
  int maxX1 = tft.width() - 5;

  tft.fillRect(3, 3, maxX1, 36, ST77XX_BLACK);
  tft.fillRect(3, 61, maxX1, 60, ST77XX_BLACK);
  CLEAR_BUFFER(bufferFreq);
  CLEAR_BUFFER(bufferUnt);
  CLEAR_BUFFER(bufferBand);
  CLEAR_BUFFER(bufferAGC);
  CLEAR_BUFFER(bufferBW);
  CLEAR_BUFFER(bufferStepVFO);
  CLEAR_BUFFER(bufferStereo);
  CLEAR_BUFFER(bufferRssi);
  showFrequency();
  if (rx.isCurrentTuneFM()) {
    unt = (char *) "MHz";
  } else
  {
    unt = (char *) "kHz";
    showStep();
    showAgcAtt();
    if (ssbLoaded)  showBFO();
  }
  printValue(140, 5, bufferUnt, unt, 6, ST77XX_GREEN,1);
  strcpy(bufferDisplay, band[bandIdx].bandName);
  strcat(bufferDisplay, " ");
  strcat(bufferDisplay, bandModeDesc[currentMode]);

  printValue(5, 65, bufferBand, bufferDisplay, 6, ST77XX_CYAN, 1);
  showBandwidth();
}

/**
 * Shows the current Bandwidth status
 */
void showBandwidth() {
    // Bandwidth
    if (currentMode == LSB || currentMode == USB || currentMode == AM) {
      char * bw;
      if (currentMode == AM) 
        bw = (char *) bandwidthAM[bwIdxAM].desc;
      else 
        bw = (char *) bandwidthSSB[bwIdxSSB].desc;
      strcpy(bufferDisplay,"BW: ");
      strcat(bufferDisplay,bw);
      strcat(bufferDisplay,"kHz");  
    } 
    else {
      bufferDisplay[0] = '\0';
    }
    printValue(5, 110, bufferBW, bufferDisplay, 6, ST77XX_GREEN,1);
}

/**
 *  Shows the current RSSI and SNR status
 */
void showRSSI()
{
    int rssiLevel;
    uint8_t rssiAux;
    int snrLevel;
    char sSt[10];
    char sRssi[10];
    int maxAux = tft.width() - 10;

    if (currentMode == FM)
    {
      strcpy(sSt,(rx.getCurrentPilot()) ? "ST" : "MO");
      printValue(4, 4, bufferStereo, sSt, 6, ST77XX_GREEN, 1);
    }

    // It needs to be calibrated. You can do it better. 
    // RSSI: 0 to 127 dBuV
    if (rssi < 2) 
       rssiAux = 4;
    else if ( rssi < 4)
       rssiAux = 5;
    else if ( rssi < 12 ) 
       rssiAux = 6;
    else if (rssi < 25) 
       rssiAux = 7;
    else if ( rssi < 50 )
       rssiAux = 8;
    else if ( rssi >= 50 )
       rssiAux = 9;

    sRssi[0] = 'S';
    sRssi[1] = rssiAux + 48; 
    sRssi[2] = (rssiAux == 9)? '+': ' ';  
    sRssi[3] = '\0'; 
              
    rssiLevel = map(rssiAux, 0, 10, 0, maxAux);
    snrLevel = map(snr, 0, 127, 0, maxAux);

    tft.fillRect(5, 42,  maxAux, 6, ST77XX_BLACK);
    tft.fillRect(5, 42, rssiLevel, 6, ST77XX_ORANGE);
    printValue(5, 31, bufferRssi, sRssi, 6, ST77XX_GREEN, 1);

    tft.fillRect(5, 51, maxAux, 6, ST77XX_BLACK);
    tft.fillRect(5, 51, snrLevel, 6, ST77XX_WHITE);
}

/**
 *  Shows the current AGC and Attenuation status
 */
void showAgcAtt() {
    char sAgc[10];
    char tmp[4];
    rx.getAutomaticGainControl();
    if (agcNdx == 0 && agcIdx == 0)
      strcpy(sAgc, "AGC ON");
    else {
      convertToChar(agcNdx, tmp, 2, 0, '.'); 
      strcpy(sAgc,"ATT: ");
      strcat(sAgc,tmp);  
    }
    tft.setFont(NULL);
    printValue(110, 110, bufferAGC, sAgc, 6, ST77XX_GREEN, 1);
}

/**
 *  Shows the current step
 */
void showStep() {
  char sStep[15];
  char tmp[10];
  convertToChar(currentStep, tmp, 4, 0, '.'); 
  strcpy(sStep,"Stp:");
  strcat(sStep,tmp);
  printValue(110, 65, bufferStepVFO, sStep, 6, ST77XX_GREEN, 1);
}

/**
 * Shows the current BFO value
 */
void showBFO()
{
    char tmp[6];
    uint16_t auxBfo;
    auxBfo = currentBFO;
    if (currentBFO < 0 ) {
        auxBfo = ~currentBFO + 1; // converts to absolute value (ABS) using binary operator
        bufferDisplay[0] = '-';
    }
    else if (currentBFO > 0 )  
      bufferDisplay[0] = '+';
    else 
      bufferDisplay[0] = ' ';      

     convertToChar(auxBfo, tmp, 4, 0, '.'); 
     strcpy(&bufferDisplay[1], tmp);
    // sprintf(bufferDisplay, "%+4d", currentBFO);
    printValue(120, 30, bufferBFO, bufferDisplay, 6, ST77XX_CYAN,1);
    // showFrequency();
    elapsedCommand = millis();
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
  rx.reset();
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  // rx.setI2CFastMode(); // Recommended
  rx.setI2CFastModeCustom(500000); // It is a test and may crash.
  rx.downloadCompressedPatch(ssb_patch_content, size_content, cmd_0x15, cmd_0x15_size);
  
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
  idxStep = getStepIndex(currentStep);
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

}

/**
 *  Deal with AGC and attenuattion
 */
void doAgc(int8_t v) {
  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;
  if (agcIdx < 0 )
    agcIdx = 37;
  else if ( agcIdx > 37)
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
  rx.setSeekAmSpacing((currentStep > 10) ? 10 : currentStep); // Max 10kHz for spacing
  showStep();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 *  Switches to the AM, LSB or USB modes
 */
void doMode(int8_t v) {
  bufferBFO[0] =  bufferFreq[0] = '\0';

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

/**
 * Prepares the system to process the desired current command
 */
void prepareCommand(bool *b)
{
  bool tmp = *b;
  disableCommands(); // Disable previous command if is activated.
  *b = !tmp;
  commandDisabledByUser = (*b) ? false : true;
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
    else if (cmdBand)
      setBand(encoderCount);
    else
    {
      if (seekDirection = (encoderCount == 1) )
        rx.frequencyUp();
      else
        rx.frequencyDown();
      // Show the current frequency only if it has changed
      currentFrequency = rx.getFrequency();
    }
    showFrequency();
    encoderCount = 0;
  }
  else
  {
    if (digitalRead(BANDWIDTH_BUTTON) == LOW)
      prepareCommand(&cmdBandwidth);
    else if (digitalRead(BAND_BUTTON) == LOW)
      prepareCommand(&cmdBand);
    else if (digitalRead(SEEK_BUTTON) == LOW)
      doSeek();
    else if (digitalRead(AGC_SWITCH) == LOW)
      prepareCommand(&cmdAgc);
    else if (digitalRead(STEP_SWITCH) == LOW)
      prepareCommand(&cmdStep);
    else if (digitalRead(MODE_SWITCH) == LOW)
      prepareCommand(&cmdMode);
    else if (digitalRead(BFO_SWITCH) == LOW)
    {
      bfoOn = !bfoOn;
      if ((currentMode == LSB || currentMode == USB))
        showBFO();

      CLEAR_BUFFER(bufferFreq);
      showFrequency();
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
  if ( ( ((millis() - elapsedCommand) > ELAPSED_COMMAND ) && isCommand()) || commandDisabledByUser)
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
  delay(1);
}
