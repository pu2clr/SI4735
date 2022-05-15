/*
  UNDER CONSTRUCTION.... It is not working so far
  

  It is important to know that this example is just a Proof of Concept. Is does not intend to be a real application.

  This example explores the use of images and an on  encoder to conttoll the menu.

  ATTENTION:
  This sketch uses a Flash EEPROM emulator library.
  See FlashStorage_SAMD Arduino Library on: https://github.com/khoih-prog/FlashStorage_SAMD
  It is importante to know that the flash memory has a limited amount of write cycles. Typical flash memories
  can perform about 10000 writes cycles to the same flash block before starting losing the ability to retain data.
  To try to save cycles, this sketch implements some advices:
   * The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
   * Avoid call write() and commit() to store the EEPROM data in flash as possible as you can.
   * Use write() and commit() functions with care: Every call writes the complete emulated-EEPROM data to flash.
   * Take care when you call these function in a loop or you will lose your flash soon.

  ATTENTION: The author of this sketch is not responsible for damages caused to your devices.


  It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
  regular  comercial  stations.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.

  Features:   AM; SSB; LW/MW/SW; external mute circuit control; AGC; Attenuation gain control;
              SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  Seeeduino XIAO Wire up with TFT3577 , encoder/pushbutton and SI4735-D60

  | Device name               | Device Pin / Description      |  Pi Pico RP2040  |
  | ------------------------- | ----------------------------- | ---------------- |
  |    TFT3577                |                               |                  |
  |                           | DC                            |  GPIO14          |
  |                           | RES                           |  GPIO15          |
  |                           | SDA                           |  GPIO11 (MOSI)   |
  |                           | SCL                           |  GPIO10 (SCK)    |
  |                           | VCC                           |  3.3V            |
  |                           | BL                            |  3.3V            |
  |                           | GND                           |  GND             |
  |     SS473X                |                               |                  |
  |                           | RESET (pin 15)                |  GPIO3           |
  |                           | SDIO (pin 18)                 |  GPIO0           |
  |                           | SCLK (pin 17)                 |  GPIO1           |
  |                           | (*1)SEN (pin 16)              |  +Vcc or GND     |
  |    Encoder                |                               |                  |
  |                           | A                             |  GPIO19          |
  |                           | B                             |  GPIO20          |
  |                           | PUSH BUTTON (encoder)         |  GPIO21          |

  The Seeeduino pin 9 is free. You can use a push button to improve the user interface.

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

// Dealing with font type
#include "./fonts/DSEG14_Classic_Mini_Regular_40.h"
#include "./fonts/DSEG7_Classic_Regular_16.h"
#include "./fonts/Serif_plain_15.h"
#include "./fonts/Serif_bold_15.h"

// Dealing with images. This approach consumes a lot of memory resources.
#include "./images/world2.h"
// #include "./images/signallevel.h"

// Install this library from Github: https://github.com/khoih-prog/FlashStorage_SAMD
// #include <FlashAsEEPROM_SAMD.h> // You have to install the ptch too. See section "For Seeeduino SAMD boards" https://github.com/khoih-prog/FlashStorage_SAMD#3-for-seeeduino-samd-boards

#include <SI4735.h>
#include "Rotary.h"
#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see patch_init.h

// Define pins :

static const int TFT_CS = 13;
static const int TFT_RST = 15;
static const int TFT_DC = 14;

#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_SCK 10


#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 3

// Enconder PINs
#define ENCODER_PIN_A 19
#define ENCODER_PIN_B 20

// Buttons controllers
#define ENCODER_PUSH_BUTTON 21 //
#define DUMMY_BUTTON 15

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 3000
#define ELAPSED_CLICK 1800 // time to check the double click commands
#define DEFAULT_VOLUME 42  // change it for your favorite sound volume

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
int8_t avcIdx = 42; // min 12 and max 90
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
bool menuSelection = false;

bool scanScop = false;

int16_t currentBFO = 0;
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();
long elapsedClick = millis();
volatile int encoderCount = 0;
uint16_t currentFrequency;
uint16_t previousFrequency = 0;

const uint8_t currentBFOStep = 10;

/**
 * Declaration of the functions responsible for the presentation of parameters
 * The references for these functions will be used by the menu functions.
 */
void showVolume();
void showRdsSetup();
void showStep();
void showModeMenu();
void showFrequencySSB();
void showBandwidth();
void showAgcAtt();
void showSoftMute();
void showAvc();
void showDummy(){};

/**
 * Buffers to store the current parameter values
 *
 */
char oldBand[10];
char oldFreq[10];
char oldUnit[10];
char oldMode[10];
char oldModeMenu[10];
char oldAux[10];
char oldStep[10];
char oldVolume[10];
char oldRds[10];
char oldBFO[10];
char oldBandwidth[10];
char oldAgcAtt[10];
char oldSoftMute[10];
char oldAvc[10];
char oldDummy[10];
char oldTime[10];

typedef struct
{
  uint8_t itemId;
  const char *itemName;     // Menu item description
  uint8_t colItem;    // Column that the item name will be shown
  uint8_t colContent; // Column that the item value will be shown
  uint8_t linItem;    // Line that the item will be shown
} Menu;

Menu menu[] = {{0, "Volume", 4, 60, 10},
               {1, "FM RDS", 4, 60, 20},
               {2, "Step", 4, 60, 30},
               {3, "Mode", 4, 60, 40},
               {4, "BFO", 4, 60, 50},
               {5, "Bandwidth", 4, 60, 60},
               {6, "AGC/Att", 4, 60, 70},
               {7, "SoftMute", 4, 60, 80},
               {8, "AVC", 4, 60, 90},
               {9, "Seek Up", 4, 60, 100},
               {10, "Seek Down", 4, 60, 110},
               {11, "Scan", 110, 110, 10},
               {12, "Exit", 110, 110, 20}};

int8_t menuIdx = 0;
const int lastMenu = 12;
// const int lastMenu = (sizeof menu / sizeof(Menu)) - 1;
int8_t currentMenuCmd = -1;

/** Finish Menu definition **/

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
    {"VHF", FM_BAND_TYPE, 8410, 10810, 10390, 1, 0, 1, 0, 0, 0},
    {"MW1", MW_BAND_TYPE, 520, 1720, 810, 3, 4, 0, 0, 0, 32},
    {"MW2", MW_BAND_TYPE, 531, 1701, 783, 2, 4, 0, 0, 0, 32},
    {"MW3", MW_BAND_TYPE, 1700, 3500, 2500, 1, 4, 1, 0, 0, 32},
    {"80M", MW_BAND_TYPE, 3500, 4000, 3700, 0, 4, 1, 0, 0, 32},
    {"SW1", SW_BAND_TYPE, 4500, 5200, 4885, 1, 4, 1, 0, 0, 32},
    {"SW2", SW_BAND_TYPE, 5700, 6300, 6000, 1, 4, 1, 0, 0, 32},
    {"40M", SW_BAND_TYPE, 7000, 7200, 7100, 0, 4, 1, 0, 0, 40},
    {"SW3", SW_BAND_TYPE, 7200, 7900, 7200, 1, 4, 1, 0, 0, 40},
    {"SW4", SW_BAND_TYPE, 9200, 10000, 9500, 1, 4, 1, 0, 0, 40},
    {"SW5", SW_BAND_TYPE, 11300, 12300, 11900, 1, 4, 1, 0, 0, 40},
    {"SW6", SW_BAND_TYPE, 13400, 13900, 13500, 1, 4, 1, 0, 0, 40},
    {"20M", SW_BAND_TYPE, 14000, 15000, 14200, 0, 4, 1, 0, 0, 42},
    {"SW7", SW_BAND_TYPE, 15000, 16000, 15300, 1, 4, 1, 0, 0, 42},
    {"SW8", SW_BAND_TYPE, 17300, 17900, 17500, 1, 4, 1, 0, 0, 42},
    {"15M", SW_BAND_TYPE, 20000, 21400, 21100, 0, 4, 1, 0, 0, 44},
    {"SW9", SW_BAND_TYPE, 21450, 21900, 21500, 1, 4, 1, 0, 0, 44},
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

// RDS
char *stationName;
char bufferStatioName[20];
char localTime[10];
uint16_t localYear, localMonth, localDay, localHour, localMinute;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

SI4735 rx;

// Create display:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Adjust here the size of the TFT display you are using.
const int tftHight = 128;
const int tftWidth = 160;

void inline clearScreen()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRGBBitmap(0, 0, mapa1, 168, 120);
}

void inline clearBuffer()
{
  oldFreq[0] = oldUnit[0] = oldMode[0] = oldBand[0] = oldAux[0] = oldDummy[0] = '\0';
  oldStep[0] = oldVolume[0] = oldSoftMute[0] = oldBandwidth[0] = oldAgcAtt[0] = '\0';
  oldAvc[0] = oldTime[0] = oldRds[0] = oldBFO[0] = '\0';
}

void setup()
{
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  delay(50);
  

  clearScreen();



  splash();

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    /*
    EEPROM.write(eeprom_address, 0);
    EEPROM.commit();
    */
    // print(0, 0, NULL, 2, "EEPROM RESETED");
    delay(3000);
    clearScreen();
  
  }

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

 
  // rx.setI2CFastModeCustom(100000);
  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error

  rx.setup(RESET_PIN, MW_BAND_TYPE);
  // Comment the line above and uncomment the three lines below if you are using external ref clock (active crystal or signal generator)
  // rx.setRefClock(32768);
  // rx.setRefClockPrescaler(1);   // will work with 32768
  // rx.setup(RESET_PIN, 0, MW_BAND_TYPE, SI473X_ANALOG_AUDIO, XOSCEN_RCLK);

  delay(300);

  // Checking the EEPROM content
  /*
  if (EEPROM.read(eeprom_address) == app_id)
  {
    readAllReceiverInformation();
  }
  else
    rx.setVolume(volume);
  */

  rx.setVolume(volume);

  useBand();
  showStatus();
  showCommandStatus((char *)" VFO ");
}

/*
** Remove the call of this function for faster statup
*/
void splash()
{
  tft.drawRGBBitmap(0, 0, mapa1, 168, 120); // , (uint16_t)ST77XX_BLUE, (uint16_t)ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(40, 10);
  tft.print("PU2CLR-SI4735");
  tft.setCursor(38, 40);
  tft.print("Arduino Library");
  tft.setCursor(27, 60);
  tft.print("You can do it better");
  // delay(4000);
  // tft.drawRGBBitmap(0, 0, signalLevel, 168, 120);
  delay(3000);
  clearScreen();
  showTemplate();
}

/**
 *  Shows the static content on  display
 */
void showTemplate()
{
  printValue(3, 82, oldAux, (char *) "88888", 31, ST77XX_BLUE, ST77XX_BLUE, 1, &DSEG14_Classic_Mini_Regular_40);
}

/**
 * Prevents blinking during the frequency display.
 * Erases the old digits if it has changed and print the new digit values.
 */
void printValue(int col, int line, char *oldValue, char *newValue, uint8_t space, uint16_t color, uint16_t bgColor, uint8_t txtSize, const GFXfont *font)
{
  int c = col;
  char *pOld;
  char *pNew;

  tft.setFont(font);

  tft.setTextSize(txtSize);
  pOld = oldValue;
  pNew = newValue;

  // prints just changed digits
  while (*pOld && *pNew)
  {
    if (*pOld != *pNew)
    {
      // Erases olde value
      tft.setTextColor(bgColor);
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
  tft.setTextColor(bgColor);
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
 * Shows some basic information on display
 */
void showStatus()
{
  showTemplate();
  showFrequency();
  showRSSI();
  showBand();
  showMode();
  showUnit();
}

/*
** Shows the current band name
*/
void showBand()
{
  printValue(7, 20, oldBand, (char *)band[bandIdx].bandName, 17, ST77XX_GREEN, ST77XX_BLUE, 1, &Serif_plain_15);
}

/**
 * Shows a single character
 */
void showChar(uint8_t col, uint8_t lin, char content, uint16_t color, const GFXfont *font)
{
  tft.setFont(font);
  tft.setTextColor(color);
  tft.setCursor(col, lin);
  tft.print(content);
}

/*
 ** Shows the current frequency Unit
 */
void showUnit()
{
  static char oldDot[4];
  char *p;

  showChar(95, 83, ',', ST77XX_BLACK, &Serif_bold_15);
  showChar(64, 83, '.', ST77XX_BLACK, &Serif_bold_15);
  if (rx.isCurrentTuneFM())
  {
    p = (char *)"MHZ";
    showChar(95, 83, ',', ST77XX_YELLOW, &Serif_bold_15);
  }
  else
  {
    p = (char *)"kHz";
    if (currentFrequency > 1800)
      showChar(64, 83, '.', ST77XX_YELLOW, &Serif_bold_15);
  }
  printValue(115, 110, oldUnit, p, 13, ST77XX_GREEN, ST77XX_BLUE, 1, &Serif_plain_15);
}

/**
 * Shows frequency information on Display
 */
void showFrequency()
{

  char freqAux[10];
  rx.convertToChar(currentFrequency, freqAux, 5, 0, '.');
  // TO DO: Number format
  if (scanScop)
  {
    printValue(25, 30, oldFreq, freqAux, 20, ST77XX_YELLOW, ST77XX_BLACK, 1, &DSEG7_Classic_Regular_16);
    showPlot();
  }
  else
  {
    printValue(3, 82, oldFreq, freqAux, 31, ST77XX_YELLOW, ST77XX_BLUE, 1, &DSEG14_Classic_Mini_Regular_40);
  }
}

/**
 * Shows SSB frequency information.
 */
void showFrequencySSB()
{
  char tmp[6];
  uint16_t auxBfo;
  auxBfo = currentBFO;
  char newBFO[10];

  if (currentBFO < 0)
  {
    auxBfo = ~currentBFO + 1; // converts to absolute value (ABS) using binary operator
    newBFO[0] = '-';
  }
  else if (currentBFO > 0)
    newBFO[0] = '+';
  else
    newBFO[0] = ' ';

  rx.convertToChar(auxBfo, tmp, 4, 0, '.');
  strcpy(&newBFO[1], tmp);
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldBFO, newBFO, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
  elapsedCommand = millis();
}

/**
 * https://github.com/G6EJD/ESP8266-Analogue-Meter/blob/master/ESP8266_METER_DISPLAY_ILI9340_V4.ino
 *
 */
void showSmeter(uint16_t signalLevel)
{
  static uint8_t buffer[20];
  static uint16_t sample;
  static uint8_t idx = 0;
  static bool isFull = false;
  const int hMeter = 65; // horizontal center for needle animation
  const int vMeter = 85; // vertical center for needle animation (outside of dislay limits)
  const int rMeter = 80;

  // makes needle movement smoother
  if (idx < 20 && !isFull)
  {
    buffer[idx] = signalLevel;
    idx++;
    isFull = true;
    return;
  }
  if (idx >= 20)
    idx = 0;
  buffer[idx] = signalLevel;
  // always get the average of the last 20 readings
  for (int i = sample = 0; i < 20; i++)
    sample += buffer[idx];

  // Draw the S Meter with the average value
  signalLevel = map((sample / 20), 0, 127, 0, 1023);
  float smeterValue = (signalLevel)*330 / 1024; // convert the signal value to arrow information

  smeterValue = smeterValue - 34; // shifts needle to zero position
  // display.clearDisplay();                                   // refresh display for next step
  // display.drawBitmap(0, 0, S_Meter, 128, 64, WHITE);        // draws background
  int a1 = (hMeter + (sin(smeterValue / 57.296) * rMeter)); // meter needle horizontal coordinate
  int a2 = (vMeter - (cos(smeterValue / 57.296) * rMeter)); // meter needle vertical coordinate
  // display.drawLine(a1, a2, hMeter, vMeter, WHITE);          // draws needle
  // display.display();
  tft.drawLine(a1, a2, hMeter, vMeter, ST7735_GREEN);
  idx = 0;
  sample = 0;
}

/**
 * Shows RSSI signal level
 */
void showRSSI()
{
  // TO DO: Shows Antenna ICON and Signal level
}

/**
 * Show cmd on display. It means you are setting up something.
 */
void showCommandStatus(char *currentCmd)
{
  static char oldStatus[7];
  printValue(60, 0, oldStatus, currentCmd, 11, ST77XX_RED, ST7735_BLACK, 1, NULL);
}

/*
   The Seeduino uses a Flash EEPROM emulator library. See FlashStorage_SAMD Arduino Library on: https://github.com/khoih-prog/FlashStorage_SAMD
   writes the conrrent receiver information into the eeprom/Flash Memory.
   The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
   ATTENTION: avoid call write() and commit() to store the EEPROM data in flash as possible as you can.
              Use these function with care: Every call writes the complete emulated-EEPROM data to flash.
              This will reduce the remaining flash-write-cycles.
              Take care when you call these function in a loop or you will lose your flash soon.

*/
void saveAllReceiverInformation()
{
  /*
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
  */
}

/**
 * reads the last receiver status from eeprom.
 */
void readAllReceiverInformation()
{
  /*
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
  */
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
  menuSelection = false;
  scanScop = false;
  showCommandStatus((char *)" VFO ");
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

/**
 * upload SSB patch into the SI4735-D60 or SI4732-A10 devices
 */
void loadSSB()
{
  rx.setI2CFastModeCustom(400000); // You can try rx.setI2CFastModeCustom(700000); or greater value
  rx.loadPatch(ssb_patch_content, size_content, bandwidthSSB[bwIdxSSB].idx);
  rx.setI2CFastModeCustom(100000);
  ssbLoaded = true;
}

/**
 *   Sets Band up (1) or down (!1)
 */
void setBand(int8_t up_down)
{
  band[bandIdx].currentFreq = currentFrequency;
  band[bandIdx].currentStepIdx = currentStepIdx;
  if (up_down == 1)
    bandIdx = (bandIdx < lastBand) ? (bandIdx + 1) : 0;
  else
    bandIdx = (bandIdx > 0) ? (bandIdx - 1) : lastBand;
  useBand();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
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
    rx.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabFmStep[band[bandIdx].currentStepIdx]);
    rx.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    bfoOn = ssbLoaded = false;
    bwIdxFM = band[bandIdx].bandwidthIdx;
    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);
    rx.setRdsConfig(1, 2, 2, 2, 2);
    rx.setFifoCount(1);
  }
  else
  {
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor((band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);
    if (ssbLoaded)
    {
      rx.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabAmStep[band[bandIdx].currentStepIdx], currentMode);
      rx.setSSBAutomaticVolumeControl(1);
      rx.setSsbSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Disable Soft Mute for SSB
      bwIdxSSB = band[bandIdx].bandwidthIdx;
      rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
    }
    else
    {
      currentMode = AM;
      rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabAmStep[band[bandIdx].currentStepIdx]);
      bfoOn = false;
      bwIdxAM = band[bandIdx].bandwidthIdx;
      rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
      rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Soft Mute for AM or SSB
    }
    rx.setAutomaticGainControl(disableAgc, agcNdx);
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    rx.setSeekAmSpacing(5);                                                   // Max 10kHz for spacing
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStepIdx = band[bandIdx].currentStepIdx;
  rssi = 0;
}

/**
 * Shows the current Bandwidth
 */
void showBandwidth()
{
  char *bw;
  if (currentMode == LSB || currentMode == USB)
  {
    bw = (char *)bandwidthSSB[bwIdxSSB].desc;
    showFrequencySSB();
  }
  else if (currentMode == AM)
  {
    bw = (char *)bandwidthAM[bwIdxAM].desc;
  }
  else
  {
    bw = (char *)bandwidthFM[bwIdxFM].desc;
  }
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldBandwidth, bw, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 *  Switches the Bandwidth
 */
void doBandwidth(int8_t v)
{
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB = (v == 1) ? bwIdxSSB + 1 : bwIdxSSB - 1;

    if (bwIdxSSB > maxSsbBw)
      bwIdxSSB = 0;
    else if (bwIdxSSB < 0)
      bwIdxSSB = maxSsbBw;

    rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bandwidthSSB[bwIdxSSB].idx == 0 || bandwidthSSB[bwIdxSSB].idx == 4 || bandwidthSSB[bwIdxSSB].idx == 5)
      rx.setSSBSidebandCutoffFilter(0);
    else
      rx.setSSBSidebandCutoffFilter(1);

    band[bandIdx].bandwidthIdx = bwIdxSSB;
  }
  else if (currentMode == AM)
  {
    bwIdxAM = (v == 1) ? bwIdxAM + 1 : bwIdxAM - 1;

    if (bwIdxAM > maxAmBw)
      bwIdxAM = 0;
    else if (bwIdxAM < 0)
      bwIdxAM = maxAmBw;

    rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
    band[bandIdx].bandwidthIdx = bwIdxAM;
  }
  else
  {
    bwIdxFM = (v == 1) ? bwIdxFM + 1 : bwIdxFM - 1;
    if (bwIdxFM > maxFmBw)
      bwIdxFM = 0;
    else if (bwIdxFM < 0)
      bwIdxFM = maxFmBw;

    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);
    band[bandIdx].bandwidthIdx = bwIdxFM;
  }
  showBandwidth();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}

/**
 * Show menu options
 */
void showMenu()
{
  uint16_t color;
  clearBuffer();
  if (!menuSelection)
  {
    clearScreen();
    showCommandStatus((char *)"MENU ");
    menuSelection = true;
  }

  tft.setFont(NULL);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_ORANGE);

  for (int i = 0; i <= lastMenu; i++)
  {
    tft.setCursor(menu[i].colItem, menu[i].linItem);
    tft.setTextColor((menuIdx == i) ? ST77XX_ORANGE : ST7735_CYAN);
    tft.print(menu[i].itemName);
    tft.setCursor(menu[i].colContent, menu[i].linItem);
  }
}

/**
 *    Shows the current AGC and Attenuation status
 */
void showAgcAtt()
{
  char sAgc[15];
  rx.getAutomaticGainControl();
  if (agcNdx == 0 && agcIdx == 0)
    strcpy(sAgc, "AGC ON");
  else
    rx.convertToChar(agcNdx, sAgc, 2, 0, '.');

  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldAgcAtt, sAgc, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 *  AGC and attenuattion setup
 */
void doAgc(int8_t v)
{
  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;
  if (agcIdx < 0)
    agcIdx = 35;
  else if (agcIdx > 35)
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
 *   Shows the current step
 */
void showStep()
{
  char sStep[15];
  rx.convertToChar(((currentMode == FM) ? (tabFmStep[currentStepIdx] * 10) : tabAmStep[currentStepIdx]), sStep, 4, 0, '.');
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldStep, sStep, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 * Switches the current step
 */
void doStep(int8_t v)
{
  if (currentMode == FM)
  {
    idxFmStep = (v == 1) ? idxFmStep + 1 : idxFmStep - 1;
    if (idxFmStep > lastFmStep)
      idxFmStep = 0;
    else if (idxFmStep < 0)
      idxFmStep = lastFmStep;

    currentStepIdx = idxFmStep;
    rx.setFrequencyStep(tabFmStep[currentStepIdx]);
  }
  else
  {
    idxAmStep = (v == 1) ? idxAmStep + 1 : idxAmStep - 1;
    if (idxAmStep > lastAmStep)
      idxAmStep = 0;
    else if (idxAmStep < 0)
      idxAmStep = lastAmStep;

    currentStepIdx = idxAmStep;
    rx.setFrequencyStep(tabAmStep[currentStepIdx]);
    rx.setSeekAmSpacing(5); // Max 10kHz for spacing
  }
  band[bandIdx].currentStepIdx = currentStepIdx;
  showStep();
  elapsedCommand = millis();
}

/**
 * Shows the current mode on Display
 */
void showMode()
{
  char *p;
  if (currentFrequency < 520)
    p = (char *)"LW  ";
  else
    p = (char *)bandModeDesc[currentMode];
  printValue(4, 115, oldMode, p, 20, ST77XX_GREEN, ST77XX_BLUE, 1, &Serif_plain_15);
}

/**
 * Shows the selected mode during the menu action
 */
void showModeMenu()
{
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldModeMenu, (char *)bandModeDesc[currentMode], 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 * Switches to the AM, LSB or USB modes
 */
void doMode(int8_t v)
{
  showModeMenu();
  if (currentMode != FM)
  {
    if (v == 1)
    { // clockwise
      if (currentMode == AM)
      {
        // If you were in AM mode, it is necessary to load SSB patch (avery time)
        loadSSB();
        ssbLoaded = true;
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
        ssbLoaded = true;
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
    band[bandIdx].currentStepIdx = currentStepIdx;
    useBand();
  }
  showModeMenu();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/*
 *  Shows the volume level on LCD
 */
void showVolume()
{
  char volAux[4];
  rx.convertToChar(rx.getCurrentVolume(), volAux, 2, 0, '.');
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldVolume, volAux, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 * Sets the audio volume
 */
void doVolume(int8_t v)
{
  int8_t vol = rx.getVolume() + ((v == 1) ? 2 : -2);
  if (vol > 63)
    vol = 63;
  rx.setVolume(vol);
  showVolume();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
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
 *  Find a station. The direction is based on the last encoder move clockwise or counterclockwise
 */
void doSeek()
{
  if ((currentMode == LSB || currentMode == USB))
    return; // It does not work for SSB mode

  rx.seekStationProgress(showFrequencySeek, seekDirection);
  currentFrequency = rx.getFrequency();
}

/**
 * Show Soft Mute
 */
void showSoftMute()
{
  char sMute[5];
  rx.convertToChar(softMuteMaxAttIdx, sMute, 2, 0, '.');
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldSoftMute, sMute, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 * Sets the Soft Mute Parameter
 */
void doSoftMute(int8_t v)
{
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
 * Show Soft Mute
 */
void showAvc()
{
  char avc[4];
  rx.convertToChar(avcIdx, avc, 2, 0, '.');
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldAvc, avc, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

/**
 * Sets the Max gain for Automatic Volume Control.
 */
void doAvc(int8_t v)
{
  avcIdx = (v == 1) ? avcIdx + 2 : avcIdx - 2;
  if (avcIdx > 90)
    avcIdx = 12;
  else if (avcIdx < 12)
    avcIdx = 90;

  rx.setAvcAmMaxGain(avcIdx);

  band[bandIdx].avcIdx = avcIdx;

  showAvc();
  elapsedCommand = millis();
}

/**
 * Shows RDS ON or OFF
 */
void showRdsSetup()
{
  char *p;

  p = (fmRDS) ? (char *)"ON " : (char *)"OFF";
  printValue(menu[menuIdx].colContent, menu[menuIdx].linItem, oldRds, p, 10, ST7735_YELLOW, ST7735_BLUE, 1, NULL);
}

void showRDSTime()
{
  printValue(105, 120, oldTime, localTime, 11, ST7735_YELLOW, ST7735_BLACK, 1, NULL);
  delay(200);
}
void showRDSStation()
{
  printValue(5, 120, bufferStatioName, stationName, 11, ST7735_YELLOW, ST7735_BLACK, 1, NULL);
  delay(100);
}

void clearRDS()
{
  stationName = (char *)"           ";
  showRDSStation();
}

/*
 * Checks the station name is available
 */
void checkRDS()
{
  rx.getRdsStatus();
  if (rx.getRdsReceived())
  {
    if (rx.getRdsSync() && rx.getRdsSyncFound() && !rx.getRdsSyncLost() && !rx.getGroupLost())
    {
      stationName = rx.getRdsText0A();
      if (stationName != NULL)
      {
        showRDSStation();
      }
      if (rx.getRdsDateTime(&localYear, &localMonth, &localDay, &localHour, &localMinute))
      {
        sprintf(localTime, "%02u:%02u", localHour, localMinute);
        showRDSTime();
        delay(500);
        clearRDS();
      }
    }
  }
}

/**
 * Turns RDS ON or OFF
 */
void doRdsSetup(int8_t v)
{
  fmRDS = (v == 1) ? true : false;
  showRdsSetup();
  elapsedCommand = millis();
}

void showPlot()
{
  static float oldPos = 0.0;
  int step;

  if (band[bandIdx].bandType == FM_BAND_TYPE)
    step = tabFmStep[band[bandIdx].currentStepIdx];
  else
    step = tabAmStep[band[bandIdx].currentStepIdx];

  float incRate = (float)tftWidth / ((band[bandIdx].maximumFreq - band[bandIdx].minimumFreq) / (float)step);
  float pos = ((float)(currentFrequency - band[bandIdx].minimumFreq) / (float)step) * incRate;
  tft.drawLine((int)oldPos, 55, (int)oldPos, 65, ST7735_BLACK);
  tft.drawLine((int)pos, 55, (int)pos, 65, ST7735_YELLOW);
  tft.fillCircle((int)oldPos, 55, 2, ST7735_BLACK);
  tft.fillCircle((int)pos, 55, 2, ST7735_YELLOW);

  oldPos = pos;
}

void doGrid()
{
  // UNDER CONSTRUCTION...
  tft.drawRect(0, 0, (tftWidth - 2), (tftHight - 2) / 3, ST77XX_YELLOW);

  oldFreq[0] = '\0';
  showChar(78, 28, ',', ST7735_BLACK, NULL);
  showChar(60, 28, '.', ST7735_BLACK, &Serif_bold_15);
  if (rx.isCurrentTuneFM())
  {
    showChar(78, 28, ',', ST7735_YELLOW, NULL);
  }
  else
  {
    if (currentFrequency > 1800)
      showChar(60, 28, '.', ST77XX_YELLOW, &Serif_bold_15);
  }

}
void doScan()
{
  // UNDER CONSTRUCTION....

  uint16_t freq_tmp;
  float incRate;
  float pos = 1.0;
  uint16_t step;
  char oldAux[10];

  scanScop = true;

  rx.setAudioMute(true);
  tft.fillScreen(ST77XX_BLACK);
  doGrid();
  freq_tmp = currentFrequency;
  if (band[bandIdx].bandType == FM_BAND_TYPE)
    step = tabFmStep[band[bandIdx].currentStepIdx];
  else
    step = tabAmStep[band[bandIdx].currentStepIdx];

  // Adjusts the amount of channels (current bandwidth / steps) to the width of the display in pixels
  // The increment rate (incdRate) comprises the number of channels that correspond to a pixel (a value <= 1).
  incRate = (float)tftWidth / ((band[bandIdx].maximumFreq - band[bandIdx].minimumFreq) / (float)step);

  for (uint16_t i = band[bandIdx].minimumFreq; i < band[bandIdx].maximumFreq; i += step)
  {
    int x, y;
    rx.getCurrentReceivedSignalQuality(1);
    rx.setFrequency(i);
    currentFrequency = rx.getFrequency();
    showFrequency();
    delay(10);
    // Use map function to adjust the RSSI
    x = map(rx.getCurrentRSSI(), 0, tftHight, 128, 0);
    y = (int)pos + 1; // Adjusts column position 
    tft.drawLine(y, tftHight - 10, y, x - 10, ST7735_WHITE);
    pos += incRate;
  }
  rx.setFrequency(freq_tmp); // Goes back to the frequency before scanning.
  currentFrequency = rx.getFrequency();
  rx.setAudioMute(false);
  showFrequency();
}

/**
 *  Menu options selection
 */
void doMenu(int8_t v)
{
  menuIdx = (v == 1) ? menuIdx + 1 : menuIdx - 1;
  if (menuIdx > lastMenu)
    menuIdx = 0;
  else if (menuIdx < 0)
    menuIdx = lastMenu;

  showMenu();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 * Finishes  menu processing
 */
void doExitMenu()
{
  disableCommands();
  clearBuffer();
  clearScreen();
  showStatus();
  showCommandStatus((char *)" VFO ");
}

/**
 * Starts the MENU action process
 */
void doCurrentMenuCmd()
{
  disableCommands();
  switch (currentMenuCmd)
  {
  case 0: // VOLUME
    cmdVolume = true;
    showVolume();
    break;
  case 1:
    cmdRds = true;
    showRdsSetup();
    break;
  case 2: // STEP
    cmdStep = true;
    showStep();
    break;
  case 3: // MODE
    cmdMode = true;
    showModeMenu();
    break;
  case 4:
    bfoOn = true;
    if ((currentMode == LSB || currentMode == USB))
    {
      showFrequencySSB();
    }
    // showFrequency();
    break;
  case 5: // BW
    cmdBandwidth = true;
    showBandwidth();
    break;
  case 6: // AGC/ATT
    cmdAgc = true;
    showAgcAtt();
    break;
  case 7:
    cmdSoftMuteMaxAtt = true;
    showSoftMute();
    break;
  case 8:
    cmdAvc = true;
    showAvc();
    break;
  case 9:
  case 10:
    seekDirection = 10 - currentMenuCmd; // 1 Up; 0 Down
    clearScreen();
    clearBuffer();
    showStatus();
    doSeek();
    break;
  case 11:
    doScan();
    break;
  case 12:
    doExitMenu();
    break;
  default:
    showStatus();
    break;
  }
  currentMenuCmd = -1;
  elapsedCommand = millis();
}

/**
 * Return true if the current status is Menu command
 */
bool isMenuMode()
{
  return (cmdMenu | cmdStep | cmdBandwidth | cmdAgc | cmdVolume | cmdSoftMuteMaxAtt | cmdMode | cmdRds | cmdAvc);
}

/**
 * Main loop
 */
void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn & (currentMode == LSB || currentMode == USB))
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      rx.setSSBBfo(currentBFO);
      showFrequencySSB();
    }
    else if (cmdMenu)
      doMenu(encoderCount);
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
    else if (cmdSoftMuteMaxAtt)
      doSoftMute(encoderCount);
    else if (cmdAvc)
      doAvc(encoderCount);
    else if (cmdBand)
    {
      setBand(encoderCount);
      showCommandStatus((char *)" BAND ");
      showStatus();
    }
    else if (cmdRds)
      doRdsSetup(encoderCount);
    else
    {
      if (encoderCount == 1)
      {
        rx.frequencyUp();
      }
      else
      {
        rx.frequencyDown();
      }
      // Show the current frequency only if it has changed
      currentFrequency = rx.getFrequency();
      showFrequency();
    }
    encoderCount = 0;
    resetEepromDelay();
  }
  else
  {
    if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
    {
      countClick++;
      if (cmdMenu)
      {
        currentMenuCmd = menuIdx;
        doCurrentMenuCmd();
      }
      else if (countClick == 1)
      { // If just one click, you can select the band by rotating the encoder
        if (isMenuMode())
        {
          disableCommands();
          cmdMenu = true;
          showMenu();
        }
        else if (bfoOn)
        {
          disableCommands();
          cmdMenu = true;
          bfoOn = false;
          showMenu();
        }
        else
        {
          cmdBand = !cmdBand;
          if (!cmdBand)
            showCommandStatus((char *)" VFO ");
          else
            showCommandStatus((char *)" BAND ");
        }
      }
      else
      { // GO to MENU if more than one click in less than 1/2 seconds.
        if (countClick > 2)
        {
          doExitMenu();
        }
        else
        {
          cmdMenu = !cmdMenu;
          if (cmdMenu)
            showMenu();
        }
      }
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME)
  {
    rx.getCurrentReceivedSignalQuality(1);
    int aux = rx.getCurrentRSSI();
    if (rssi != aux && !isMenuMode())
    {
      rssi = aux;
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  if ((millis() - elapsedClick) > ELAPSED_CLICK)
  {
    countClick = 0;
    elapsedClick = millis();
  }

  // Show the current frequency only if it has changed
  if (itIsTimeToSave)
  {
    if ((millis() - storeTime) > STORE_TIME)
    {
      saveAllReceiverInformation();
      storeTime = millis();
      itIsTimeToSave = false;
    }
  }

  if (currentMode == FM && fmRDS && !isMenuMode())
  {
    if (currentFrequency != previousFrequency)
    {
      clearRDS();
      previousFrequency = currentFrequency;
    }
    else
    {
      checkRDS();
    }
  }
  delay(5);
}
