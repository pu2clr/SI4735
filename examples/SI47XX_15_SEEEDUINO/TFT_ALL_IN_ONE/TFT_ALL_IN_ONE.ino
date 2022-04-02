/*
  UNDER CONSTRUCTION ...

  It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
  regular  comercial  stations.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.

  Features:   AM; SSB; LW/MW/SW; external mute circuit control; AGC; Attenuation gain control;
              SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  Seeeduino XIAO Wire up with TFT3577 , encoder/pushbutton and SI4735-D60

  | Device name               | Device Pin / Description      |  Seeeduino XIAO  |
  | ------------------------- | ----------------------------- | ---------------- |
  |    TFT3577                |                               |                  |
  |                           | DC                            |  D4              | 
  |                           | RES                           |  D5              |
  |                           | SDA                           |  D10 (MOSI)      |
  |                           | SCL                           |  D8 (SCK)        | 
  |                           | VCC                           |  3.3V            |
  |                           | BL                            |  3.3V            |
  |                           | GND                           |  GND             | 
  |     SS473X                |                               |                  |
  |                           | RESET (pin 15)                |  D3              |
  |                           | SDIO (pin 18)                 |  D4/A4           |
  |                           | SCLK (pin 17)                 |  D4/A5           |
  |                           | (*1)SEN (pin 16)              |  +Vcc or GND     |
  |    Encoder                |                               |                  |
  |                           | A                             |  D1              |
  |                           | B                             |  D2              |
  |                           | PUSH BUTTON (encoder)         |  D0              |

  (*1) If you are using the SI4732-A10, check the corresponding pin numbers.
  (*1) The PU2CLR SI4735 Arduino Library has resources to detect the I2C bus address automatically.
       It seems the original project connect the SEN pin to the +Vcc. By using this sketch, you do
       not need to worry about this setting.

  ATTENTION: Read the file user_manual.txt

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  LCD16x2 (3.3V version) and ESP32 wireup: https://www.circuitschools.com/interfacing-16x2-lcd-module-with-esp32-with-and-without-i2c/
  LCD custom character: https://maxpromer.github.io/LCD-Character-Creator/

  By PU2CLR, Ricardo, May  2021.
*/
#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ST7735.h> // Include Adafruit_ST7735 library to drive the display
#include <SPI.h>             //Include SPI control library
#include <FlashAsEEPROM_SAMD.h> // Install this library from Github: https://github.com/khoih-prog/FlashStorage_SAMD#why-do-we-need-this-flashstorage_samd-library
#include <SI4735.h>
#include <LiquidCrystal.h>
#include "Rotary.h"
#include "patch_init.h" // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see patch_init.h

// Define pins :

static const int TFT_CS = 3;
static const int TFT_RST = 5;
static const int TFT_DC = 4;

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 3

// Enconder PINs
#define ENCODER_PIN_A 1
#define ENCODER_PIN_B 2


// Buttons controllers
#define ENCODER_PUSH_BUTTON 0 //
#define DUMMY_BUTTON 15

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 3000
#define ELAPSED_COMMAND 2000 // time to turn off the last command controlled by encoder. Time to goes back to the FVO control
#define ELAPSED_CLICK 1500   // time to check the double click commands
#define DEFAULT_VOLUME 35    // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

#define EEPROM_SIZE 512

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

// EEPROM - Stroring control variables
const uint8_t app_id = 37; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

bool itIsTimeToSave = false;

bool bfoOn = false;
bool ssbLoaded = false;

int8_t agcIdx = 0;
int8_t disableAgc = 0;
int8_t agcNdx = 0;
int8_t softMuteMaxAttIdx = 4;
int8_t avcIdx; // min 12 and max 90
uint8_t countClick = 0;

uint8_t seekDirection = 1;

bool cmdBand = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;
bool cmdMenu = false;
bool cmdRds = false;
bool cmdSoftMuteMaxAtt = false;
bool cmdAvc = false;

bool fmRDS = false;

int16_t currentBFO = 0;
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();
long elapsedClick = millis();
volatile int encoderCount = 0;
uint16_t currentFrequency;
uint16_t previousFrequency = 0;

const uint8_t currentBFOStep = 10;

const char *menu[] = {"Volume", "FM RDS", "Step", "Mode", "BFO", "BW", "AGC/Att", "SoftMute", "AVC", "Seek Up", "Seek Down"};
int8_t menuIdx = 0;
const int lastMenu = 10;
int8_t currentMenuCmd = -1;

typedef struct
{
  uint8_t idx;      // SI473X device bandwidth index
  const char *desc; // bandwidth description
} Bandwidth;

int8_t bwIdxSSB = 4;
const int8_t maxSsbBw = 5;
Bandwidth bandwidthSSB[] = {
    {4, "0.5"},
    {5, "1.0"},
    {0, "1.2"},
    {1, "2.2"},
    {2, "3.0"},
    {3, "4.0"}};

int8_t bwIdxAM = 4;
const int8_t maxAmBw = 6;
Bandwidth bandwidthAM[] = {
    {4, "1.0"},
    {5, "1.8"},
    {3, "2.0"},
    {6, "2.5"},
    {2, "3.0"},
    {1, "4.0"},
    {0, "6.0"}};

int8_t bwIdxFM = 0;
const int8_t maxFmBw = 4;

Bandwidth bandwidthFM[] = {
    {0, "AUT"}, // Automatic - default
    {1, "110"}, // Force wide (110 kHz) channel filter.
    {2, " 84"},
    {3, " 60"},
    {4, " 40"}};

int tabAmStep[] = {1,    // 0
                   5,    // 1
                   9,    // 2
                   10,   // 3
                   50,   // 4
                   100}; // 5

const int lastAmStep = (sizeof tabAmStep / sizeof(int)) - 1;
int idxAmStep = 3;

int tabFmStep[] = {5, 10, 20};
const int lastFmStep = (sizeof tabFmStep / sizeof(int)) - 1;
int idxFmStep = 1;

uint16_t currentStepIdx = 1;

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

/**
 *  Band data structure
 */
typedef struct
{
  const char *bandName;  // Band description
  uint8_t bandType;      // Band type (FM, MW or SW)
  uint16_t minimumFreq;  // Minimum frequency of the band
  uint16_t maximumFreq;  // maximum frequency of the band
  uint16_t currentFreq;  // Default frequency or current frequency
  int8_t currentStepIdx; // Idex of tabStepAM:  Defeult frequency step (See tabStepAM)
  int8_t bandwidthIdx;   // Index of the table bandwidthFM, bandwidthAM or bandwidthSSB;
  uint8_t disableAgc;
  int8_t agcIdx;
  int8_t agcNdx;
  int8_t avcIdx;
} Band;

/*
   Band table
   YOU CAN CONFIGURE YOUR OWN BAND PLAN. Be guided by the comments.
   To add a new band, all you have to do is insert a new line in the table below. No extra code will be needed.
   You can remove a band by deleting a line if you do not want a given band.
   Also, you can change the parameters of the band.
   ATTENTION: You have to RESET the eeprom after adding or removing a line of this table.
              Turn your receiver on with the encoder push button pressed at first time to RESET the eeprom content.
*/
Band band[] = {
    {"VHF", FM_BAND_TYPE, 6400, 10800, 10390, 1, 0, 1, 0, 0, 0},
    {"MW1", MW_BAND_TYPE, 150, 1720, 810, 3, 4, 0, 0, 0, 32},
    {"MW2", MW_BAND_TYPE, 531, 1701, 783, 2, 4, 0, 0, 0, 32},
    {"MW3", MW_BAND_TYPE, 1700, 3500, 2500, 1, 4, 1, 0, 0, 32},
    {"80M", MW_BAND_TYPE, 3500, 4000, 3700, 0, 4, 1, 0, 0, 32},
    {"SW1", SW_BAND_TYPE, 4000, 5500, 4885, 1, 4, 1, 0, 0, 32},
    {"SW2", SW_BAND_TYPE, 5500, 6500, 6000, 1, 4, 1, 0, 0, 32},
    {"40M", SW_BAND_TYPE, 6500, 7300, 7100, 0, 4, 1, 0, 0, 40},
    {"SW3", SW_BAND_TYPE, 7200, 8000, 7200, 1, 4, 1, 0, 0, 40},
    {"SW4", SW_BAND_TYPE, 9000, 11000, 9500, 1, 4, 1, 0, 0, 40},
    {"SW5", SW_BAND_TYPE, 11100, 13000, 11900, 1, 4, 1, 0, 0, 40},
    {"SW6", SW_BAND_TYPE, 13000, 14000, 13500, 1, 4, 1, 0, 0, 40},
    {"20M", SW_BAND_TYPE, 14000, 15000, 14200, 0, 4, 1, 0, 0, 42},
    {"SW7", SW_BAND_TYPE, 15000, 17000, 15300, 1, 4, 1, 0, 0, 42},
    {"SW8", SW_BAND_TYPE, 17000, 18000, 17500, 1, 4, 1, 0, 0, 42},
    {"15M", SW_BAND_TYPE, 20000, 21400, 21100, 0, 4, 1, 0, 0, 44},
    {"SW9", SW_BAND_TYPE, 21400, 22800, 21500, 1, 4, 1, 0, 0, 44},
    {"CB ", SW_BAND_TYPE, 26000, 28000, 27500, 0, 4, 1, 0, 0, 44},
    {"10M", SW_BAND_TYPE, 28000, 30000, 28400, 0, 4, 1, 0, 0, 44},
    {"ALL", SW_BAND_TYPE, 150, 30000, 15000, 0, 4, 1, 0, 0, 48} // All band. LW, MW and SW (from 150kHz to 30MHz)
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;

uint8_t rssi = 0;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

SI4735 rx;

// Create display:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void setup() {

  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  
  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    EEPROM.write(eeprom_address, 0);
    EEPROM.commit();
    // print(0, 0, NULL, 2, "EEPROM RESETED");
    delay(3000);
    tft.fillScreen(ST77XX_BLACK);
  }

  showTemplate();


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
 * Show cmd on display. It means you are setting up something.
 */
void showCommandStatus(char *currentCmd)
{
  // lcd.setCursor(5, 0);
  // lcd.print(currentCmd);
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
char *convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator)
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

/*
   The Seeduino uses a Flash EEPROM emulator library. See FlashStorage_SAMD Arduino Library on: https://github.com/khoih-prog/FlashStorage_SAMD

   writes the conrrent receiver information into the eeprom.
   The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
*/
void saveAllReceiverInformation()
{
  int addr_offset;

  EEPROM.update(eeprom_address, app_id);             // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);        // Stores the current band
  EEPROM.update(eeprom_address + 3, fmRDS);
  EEPROM.update(eeprom_address + 4, currentMode); // Stores the current Mode (FM / AM / SSB)
  EEPROM.update(eeprom_address + 5, currentBFO >> 8);
  EEPROM.update(eeprom_address + 6, currentBFO & 0XFF);

  addr_offset = 7;
  band[bandIdx].currentFreq = currentFrequency;

  for (int i = 0; i <= lastBand; i++)
  {
    EEPROM.update(addr_offset++, (band[i].currentFreq >> 8));   // stores the current Frequency HIGH byte for the band
    EEPROM.update(addr_offset++, (band[i].currentFreq & 0xFF)); // stores the current Frequency LOW byte for the band
    EEPROM.update(addr_offset++, band[i].currentStepIdx);       // Stores current step of the band
    EEPROM.update(addr_offset++, band[i].bandwidthIdx);         // table index (direct position) of bandwidth
  }
  EEPROM.commit();
}

/**
 * reads the last receiver status from eeprom.
 */
void readAllReceiverInformation()
{
  uint8_t volume;
  int addr_offset;
  int bwIdx;

  volume = EEPROM.read(eeprom_address + 1); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);
  fmRDS = EEPROM.read(eeprom_address + 3);
  currentMode = EEPROM.read(eeprom_address + 4);
  currentBFO = EEPROM.read(eeprom_address + 5) << 8;
  currentBFO |= EEPROM.read(eeprom_address + 6);

  addr_offset = 7;
  for (int i = 0; i <= lastBand; i++)
  {
    band[i].currentFreq = EEPROM.read(addr_offset++) << 8;
    band[i].currentFreq |= EEPROM.read(addr_offset++);
    band[i].currentStepIdx = EEPROM.read(addr_offset++);
    band[i].bandwidthIdx = EEPROM.read(addr_offset++);
  }

  currentFrequency = band[bandIdx].currentFreq;

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentStepIdx = idxFmStep = band[bandIdx].currentStepIdx;
    rx.setFrequencyStep(tabFmStep[currentStepIdx]);
  }
  else
  {
    currentStepIdx = idxAmStep = band[bandIdx].currentStepIdx;
    rx.setFrequencyStep(tabAmStep[currentStepIdx]);
  }

  bwIdx = band[bandIdx].bandwidthIdx;

  if (currentMode == LSB || currentMode == USB)
  {
    loadSSB();
    bwIdxSSB = (bwIdx > 5) ? 5 : bwIdx;
    rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bandwidthSSB[bwIdxSSB].idx == 0 || bandwidthSSB[bwIdxSSB].idx == 4 || bandwidthSSB[bwIdxSSB].idx == 5)
      rx.setSSBSidebandCutoffFilter(0);
    else
      rx.setSSBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    bwIdxAM = bwIdx;
    rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
  }
  else
  {
    bwIdxFM = bwIdx;
    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);
  }

  delay(50);
  rx.setVolume(volume);
}

/*
 * To store any change into the EEPROM, it is needed at least STORE_TIME  milliseconds of inactivity.
 */
void resetEepromDelay()
{
  elapsedCommand = storeTime = millis();
  itIsTimeToSave = true;
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
  cmdMenu = false;
  cmdSoftMuteMaxAtt = false;
  cmdRds = false;
  cmdAvc = false;
  countClick = 0;

  showCommandStatus((char *)"VFO ");
}

/**
 * Reads encoder via interrupt
 * Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
 * if you do not add ICACHE_RAM_ATTR declaration, the system will reboot during attachInterrupt call.
 * With ICACHE_RAM_ATTR macro you put the function on the RAM.
 */
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
    encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
}

void loadSSB()
{
  rx.setI2CFastModeCustom(400000); // You can try rx.setI2CFastModeCustom(700000); or greater value
  rx.loadPatch(ssb_patch_content, size_content, bandwidthSSB[bwIdxSSB].idx);
  rx.setI2CFastModeCustom(100000);
  ssbLoaded = true;
}

void loop() {

}