/*
  This sketch runs on ESP32 device LilyGO T-Embed panel.
  
  The interface design of the sketch used here was written by Volos (https://github.com/VolosR/TEmbedFMRadio). 
  The Volos' project is a receiver based on the TEA5767 device and I have adapted it to the SI473X device.
  This sketch implements the main functions of the SI4732/SI4735. It is  a  complete  radio  capable  to  
  tune  on  AM  and  SSB  modes from 150 to 30.000 kHz  and also FM mode from 64 to 108 MHz.

  Please, read the file user_manual.txt or README.md files for more details. 

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.

  ESP32 and components wire up.

  | Device name            | Device Pin / Description     |  ESP32    |
  | -----------------------| -----------------------------| ----------|
  | LilyGo Painel I2C      |                              |           |
  |                        | SDA/SDIO                     |  GPIO 18  |
  |                        | SCL/SCLK                     |  GPIO  8  |
  | Encoder  LilyGo Painel |                              |           |
  |                        | A                            |  GPIO  2  |
  |                        | B                            |  GPIO  1  |
  |                        | PUSH BUTTON (encoder)        |  GPIO  0  |
  |                        |                              |           |  
  | Battery Indicator      |                              |  GPIO  4  |
  | LCD LED                |                              |  GPIO 15  |  
  | Power ON               |                              |  GPIO 46  |
  |                        |                              |           |
  | SI453X MCU RESET PIN   |                              |           |
  |  See table below       | ESP32 PIN USED FOR RESETTING |  GPIO 16  |  


  ESP32 and SI4735-D60 or SI4732-A10 wire up

  | Si4735  | SI4732   | DESC.  | ESP32    (GPIO)    |
  |---------| -------- |--------|--------------------|
  | pin 15  |  pin 9   | RESET  |   12 (GPIO12)      |
  | pin 18  |  pin 12  | SDIO   |   21 (SDA / GPI21) |
  | pin 17  |  pin 11  | SCLK   |   22 (SCL / GPI22) |

  If you are using the SI4732-A10, check the corresponding pin numbers.

  References: 
  About LilyGO T-Embed and SI473X: https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_KITS/Lilygo_t_embed
  About PU2CLR SI4735 Arduino Library: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By PU2CLR, Ricardo, Dec  2022.
*/

#include <Wire.h>
#include "EEPROM.h"
#include <SI4735.h>
#include <FastLED.h>
#include <TFT_eSPI.h>
#include <Battery18650Stats.h> // Install it from: https://github.com/danilopinotti/Battery18650Stats

#include "Rotary.h"
#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see patch_init.h

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN   16      // GPIO16

// Enconder PINs
#define ENCODER_PIN_A  1           // GPIO2 
#define ENCODER_PIN_B  2           // GPIO1

// I2C bus pin on ESP32
#define ESP32_I2C_SDA 18
#define ESP32_I2C_SCL 8

#define PIN_BAT_VOLT   4
#define PIN_POWER_ON  46
#define PIN_LCD_BL    15 

// Buttons controllers
#define ENCODER_PUSH_BUTTON 0     // GPIO0

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 200
#define ELAPSED_COMMAND 2000  // time to turn off the last command controlled by encoder. Time to goes back to the FVO control
#define ELAPSED_CLICK 1500    // time to check the double click commands
#define DEFAULT_VOLUME 60    // change it for your favorite sound volume
#define ELAPSED_TIME_BATERRY 60000

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

#define EEPROM_SIZE        512

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

#define NUM_LEDS 7
#define DATA_PIN 42
#define CLOCK_PIN 45

#define color1 0xC638
#define color2 0xC638

Battery18650Stats battery(PIN_BAT_VOLT);

// EEPROM - Stroring control variables
const uint8_t app_id = 47; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

bool itIsTimeToSave = false;

bool bfoOn = false;
bool ssbLoaded = false;

int8_t agcIdx = 0;
uint8_t disableAgc = 0;
int8_t agcNdx = 0;
int8_t softMuteMaxAttIdx = 4;
uint8_t countClick = 0;

uint8_t seekDirection = 1;

bool cmdBand = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;
bool cmdMenu = false;
bool cmdSoftMuteMaxAtt = false;
bool cmdAvc = false;      

bool fmRDS = false;

int16_t currentBFO = 0;
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedBattery = millis();
long elapsedClick = millis();
long elapsedCommand = millis();
volatile int encoderCount = 0;
uint16_t currentFrequency;

const uint8_t currentBFOStep = 10;

const char *menu[] = {"Volume", "Step", "Mode", "BFO", "BW", "AGC/Att", "AVC", "SoftMute", "Seek"};
int8_t menuIdx = 0;
const int lastMenu = 8;
int8_t currentMenuCmd = -1;

int8_t avcIdx = 38;

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
  {3, "4.0"}
};

int8_t bwIdxAM = 4;
const int8_t maxAmBw = 6;
Bandwidth bandwidthAM[] = {
  {4, "1.0"},
  {5, "1.8"},
  {3, "2.0"},
  {6, "2.5"},
  {2, "3.0"},
  {1, "4.0"},
  {0, "6.0"}
};

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
  const char *bandName;   // Band description
  uint8_t bandType;       // Band type (FM, MW or SW)
  uint16_t minimumFreq;   // Minimum frequency of the band
  uint16_t maximumFreq;   // maximum frequency of the band
  uint16_t currentFreq;   // Default frequency or current frequency
  int8_t currentStepIdx;  // Idex of tabStepAM:  Defeult frequency step (See tabStepAM)
  int8_t bandwidthIdx;    // Index of the table bandwidthFM, bandwidthAM or bandwidthSSB;
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
    {"VHF", FM_BAND_TYPE, 6400, 10800, 10270, 2, 0},
    {"MW1", MW_BAND_TYPE, 150, 1720, 810, 3, 4},
    {"MW2", MW_BAND_TYPE, 531, 1701, 783, 2, 4},
    {"MW2", MW_BAND_TYPE, 1700, 3500, 2500, 1, 4},
    {"80M", MW_BAND_TYPE, 3500, 4000, 3700, 0, 4},
    {"SW1", SW_BAND_TYPE, 4000, 5500, 4885, 1, 4},
    {"SW2", SW_BAND_TYPE, 5500, 6500, 6000, 1, 4},
    {"40M", SW_BAND_TYPE, 6500, 7300, 7100, 0, 4},
    {"SW3", SW_BAND_TYPE, 7200, 8000, 7200, 1, 4},
    {"SW4", SW_BAND_TYPE, 9000, 11000, 9500, 1, 4},
    {"SW5", SW_BAND_TYPE, 11100, 13000, 11900, 1, 4},
    {"SW6", SW_BAND_TYPE, 13000, 14000, 13500, 1, 4},
    {"20M", SW_BAND_TYPE, 14000, 15000, 14200, 0, 4},
    {"SW7", SW_BAND_TYPE, 15000, 17000, 15300, 1, 4},
    {"SW8", SW_BAND_TYPE, 17000, 18000, 17500, 1, 4},
    {"15M", SW_BAND_TYPE, 20000, 21400, 21100, 0, 4},
    {"SW9", SW_BAND_TYPE, 21400, 22800, 21500, 1, 4},
    {"CB ", SW_BAND_TYPE, 26000, 28000, 27500, 0, 4},
    {"10M", SW_BAND_TYPE, 28000, 30000, 28400, 0, 4},
    {"ALL", SW_BAND_TYPE, 150, 30000, 15000, 0, 4} // All band. LW, MW and SW (from 150kHz to 30MHz)
};                                             

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
uint8_t rssi = 0;
uint8_t volume = DEFAULT_VOLUME;

CRGB leds[NUM_LEDS];

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

SI4735 rx;

void setup()
{
  // Encoder pins
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // The line below may be necessary to setup I2C pins on ESP32
  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);

  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  tft.begin();
  tft.writecommand(0x11);
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  turnDisplay(true);

  spr.createSprite(320, 170);
  spr.setTextDatum(4);
  spr.setSwapBytes(true);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.setTextColor(color1, TFT_BLACK);

  // Splash - Remove or change it for your own introduction.
  splash(); 

  EEPROM.begin(EEPROM_SIZE);

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    EEPROM.write(eeprom_address, 0);
    EEPROM.commit();
    print(20, 20, &Orbitron_Light_24, "EEPROM RESETED");
    delay(3000);
  }

  // ICACHE_RAM_ATTR void rotaryEncoder(); see rotaryEncoder implementation below.
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error 
  rx.setup(RESET_PIN, MW_BAND_TYPE);
  
  delay(300);

  // Checking the EEPROM content
  if (EEPROM.read(eeprom_address) == app_id)
  {
    readAllReceiverInformation();
  } else 
    rx.setVolume(volume);

  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Green;
  leds[5] = CRGB::Green;
  leds[6] = CRGB::Green;
  FastLED.show();

  // rx.setMaxDelaySetFrequency(20); // Sets the Max Delay after Set Frequency 

  useBand();
  showStatus();
}

// Turn the device and display on (true) or off (false)
void turnDisplay(bool v) {
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, v);   
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, v);
}

/**
 * Shows the first screen with some message.
 * Remove all code of this function if you do not wish it.
 */
void splash() {
  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.drawString(" PU2CLR SI4735", 140, 12);
  spr.drawString("Arduino Library", 140, 60);
  spr.pushSprite(0, 0);
  delay(700);
}

/**
 * Prints a given content on display 
 */
void print(uint8_t col, uint8_t lin, const GFXfont *font, const char *msg) {
  spr.setFreeFont(font);
  spr.drawString(msg, col, lin);
  spr.pushSprite(0, 0);
}

void printParam(const char *msg) {
  spr.fillRect(10, 120, 220, 40, TFT_BLACK);
  print(80,130,&Orbitron_Light_32, msg);
}

/*
   writes the conrrent receiver information into the eeprom.
   The EEPROM.update avoid write the same data in the same memory position. It will save unnecessary recording.
*/
void saveAllReceiverInformation()
{
  int addr_offset;

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.write(eeprom_address, app_id);                 // stores the app id;
  EEPROM.write(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.write(eeprom_address + 2, bandIdx);            // Stores the current band
  EEPROM.write(eeprom_address + 3, fmRDS);
  EEPROM.write(eeprom_address + 4, currentMode); // Stores the current Mode (FM / AM / SSB)
  EEPROM.write(eeprom_address + 5, currentBFO >> 8);
  EEPROM.write(eeprom_address + 6, currentBFO & 0XFF);
  EEPROM.commit();

  addr_offset = 7;
  band[bandIdx].currentFreq = currentFrequency;

  for (int i = 0; i <= lastBand; i++)
  {
    EEPROM.write(addr_offset++, (band[i].currentFreq >> 8));   // stores the current Frequency HIGH byte for the band
    EEPROM.write(addr_offset++, (band[i].currentFreq & 0xFF)); // stores the current Frequency LOW byte for the band
    EEPROM.write(addr_offset++, band[i].currentStepIdx);       // Stores current step of the band
    EEPROM.write(addr_offset++, band[i].bandwidthIdx);         // table index (direct position) of bandwidth
    EEPROM.commit();
  }
  // Saves AVC and AGC/Att status
  EEPROM.write(addr_offset++,avcIdx);
  EEPROM.write(addr_offset++,agcIdx);
  EEPROM.write(addr_offset++,agcNdx);
  EEPROM.end();
}

/**
 * reads the last receiver status from eeprom. 
 */
void readAllReceiverInformation()
{
  uint8_t volume;
  int addr_offset;
  int bwIdx;
  EEPROM.begin(EEPROM_SIZE);

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
  // Rescues the previous  AVC and AGC/Att status
  avcIdx = EEPROM.read(addr_offset++);
  agcIdx = EEPROM.read(addr_offset++);
  agcNdx = EEPROM.read(addr_offset++);
  EEPROM.end();
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
  cmdAvc = false;   
  countClick = 0;
  showCommandStatus((char *) "VFO ");
}

/**
 * Reads encoder via interrupt
 * Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
 * if you do not add ICACHE_RAM_ATTR declaration, the system will reboot during attachInterrupt call. 
 * With ICACHE_RAM_ATTR macro you put the function on the RAM.
 */
ICACHE_RAM_ATTR void  rotaryEncoder()
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
  float freq, value;  
  int decimals; 

  // defines the frequency display format depending on the used band
  if (rx.isCurrentTuneFM())
  {
      freq = currentFrequency / 100.0;
      decimals = 1;
  }
  else
  {
    if ( currentFrequency  < 1000 ) { // LW or MW
      freq = (float) currentFrequency;
      decimals = 0;
    } else {
      freq = (float) currentFrequency / 1000.0;
      decimals = 3;
    }
  }

  value = freq * 10;

  spr.fillSprite(TFT_BLACK); // clear screen
  spr.drawFloat(freq, decimals, 150, 64, 7); // shows the frequency

  // Builds the frequency ruler
  spr.fillTriangle(156, 104, 160, 114, 164, 104, TFT_RED); 
  spr.drawLine(160, 114, 160, 170, TFT_RED);
 
  int temp = value - 20;
  for (int i = 0; i < 40; i++)
  {
    if ((temp % 10) == 0)
    {
      spr.drawLine(i * 8, 170, i * 8, 140, color1);
      spr.drawLine((i * 8) + 1, 170, (i * 8) + 1, 140, color1);
      spr.drawFloat(temp / 10.0, 1, i * 8, 130, 2);
    }
    else if ((temp % 5) == 0 && (temp % 10) != 0)
    {
      spr.drawLine(i * 8, 170, i * 8, 150, color1);
      spr.drawLine((i * 8) + 1, 170, (i * 8) + 1, 150, color1);
    }
    else
    {
      spr.drawLine(i * 8, 170, i * 8, 160, color1);
    }
    temp = temp + 1;
  }

  showBattery();

  spr.pushSprite(0, 0);

  showRSSI();
  showMode();
}

/**
 * Shows the current mode
 */
void showMode() {
  char * bandMode;
  if (currentFrequency < 520)
    bandMode = (char *) "LW  ";
  else
    bandMode = (char *) bandModeDesc[currentMode];

   spr.setTextColor(TFT_YELLOW, TFT_BLACK);   
   spr.setFreeFont(&FreeMono9pt7b);
   spr.drawString(bandMode, 25, 20);
   spr.drawString(band[bandIdx].bandName, 25, 35); 
   spr.setTextColor(TFT_WHITE, TFT_BLACK);  
   spr.pushSprite(0, 0);
}

void showBattery() {

  uint16_t colorBattery;
  int value = map(battery.getBatteryChargeLevel(true),0,100, 0,18);
  // int value = map(battery.getBatteryVolts() * 10,32,42,0,18);

  if (value < 5) 
     colorBattery = TFT_RED;
  else if (value < 11) 
     colorBattery = TFT_YELLOW;
  else 
     colorBattery = TFT_GREEN;      
  
  spr.drawRect(290, 6, 20, 9, TFT_WHITE);
  spr.fillRect(291, 7, 18, 7, TFT_BLACK);
  spr.fillRect(291, 7, value, 7, colorBattery);
  spr.fillRect(310, 8, 2, 5, TFT_WHITE);
  spr.pushSprite(0, 0);
  
}

/**
 * Shows some basic information on display
 */
void showStatus()
{
  showFrequency();
  showRSSI();


}

/**
 *  Shows the current Bandwidth status
 */
void showBandwidth()
{
  char *bw;
  char bandwidth[20];
  if (currentMode == LSB || currentMode == USB)
  {
    bw = (char *)bandwidthSSB[bwIdxSSB].desc;
    showBFO();
  }
  else if (currentMode == AM)
  {
    bw = (char *)bandwidthAM[bwIdxAM].desc;
  }
  else
  {
    bw = (char *)bandwidthFM[bwIdxFM].desc;
  }
  sprintf(bandwidth,"%s", bw);
  printParam(bandwidth);
}

/*
 * Concert rssi to VU
*/
int getStrength(uint8_t rssi)
{
  if ((rssi >= 0) and (rssi <= 1))
    return 1; // S0
  if ((rssi > 1) and (rssi <= 1))
    return 2; // S1
  if ((rssi > 2) and (rssi <= 3))
    return  3; // S2
  if ((rssi > 3) and (rssi <= 4))
    return  4; // S3
  if ((rssi > 4) and (rssi <= 10))
    return  5; // S4
  if ((rssi > 10) and (rssi <= 16))
    return 6; // S5
  if ((rssi > 16) and (rssi <= 22))
    return 7; // S6
  if ((rssi > 22) and (rssi <= 28))
    return  8; // S7
  if ((rssi > 28) and (rssi <= 34))
    return 9; // S8
  if ((rssi > 34) and (rssi <= 44))
    return 10; // S9
  if ((rssi > 44) and (rssi <= 54))
    return 11; // S9 +10
  if ((rssi > 54) and (rssi <= 64))
    return 12; // S9 +20
  if ((rssi > 64) and (rssi <= 74))
    return 13; // S9 +30
  if ((rssi > 74) and (rssi <= 84))
    return 14; // S9 +40
  if ((rssi > 84) and (rssi <= 94))
    return 15; // S9 +50
  if (rssi > 94)
    return 16; // S9 +60
  if (rssi > 95)
    return 17; //>S9 +60

  return 0;
}

/**
 *   Shows the current RSSI and SNR status
 */
void showRSSI()
{
  spr.fillRect(240,20,76,88,TFT_BLACK); // Clear the indicator areas 
  
  for (int i = 0; i < getStrength(rssi); i++)
  {
    if (i < 9)
      spr.fillRect(244 + (i * 4), 80 - i, 2, 4 + i, TFT_GREEN);
    else
      spr.fillRect(244 + (i * 4), 80 - i, 2, 4 + i, TFT_RED);
  }

  if (currentMode == FM)
  {
       spr.drawRoundRect(240, 20, 76, 22, 4, TFT_WHITE);
      if (rx.getCurrentPilot())
         spr.drawString("Stereo", 275, 31, 2);
      else
        spr.drawString("Mono", 275, 31, 2);
  }  
  spr.pushSprite(0, 0);
}

/**
 *    Shows the current AGC and Attenuation status
 */
void showAgcAtt()
{
  char sAgc[15];
  // lcd.clear();
  rx.getAutomaticGainControl();
  if (agcNdx == 0 && agcIdx == 0)
    strcpy(sAgc, "AGC ON");
  else
    sprintf(sAgc, "ATT: %2.2d", agcNdx);

  printParam(sAgc);  
}

/**
   Shows current Automatic Volume Control
*/
void showAvc() {
  char sAvc[10];
  if ( currentMode != FM ) {
    sprintf(sAvc, "%2.2d", avcIdx);    
    printParam(sAvc);  
  }
}

/**
 *   Shows the current step
 */
void showStep()
{
    char sStep[15];
    sprintf(sStep, "%4d", (currentMode == FM )? ( tabFmStep[currentStepIdx] * 10) : tabAmStep[currentStepIdx]);
    printParam(sStep);
}

/**
 *  Shows the current BFO value
 */
void showBFO()
{
  char bfo[18];
  
  if (currentBFO > 0)
    sprintf(bfo, "+%4.4d", currentBFO);
  else
    sprintf(bfo, " %4.4d", currentBFO);

  printParam(bfo);
  elapsedCommand = millis();
}

/*
 *  Shows the volume level on LCD
 */
void showVolume()
{
  char volAux[12];
  sprintf(volAux, "%2u", rx.getVolume());
  printParam(volAux);
}

/**
 * Show Soft Mute 
 */
void showSoftMute()
{
  char sMute[18];
  sprintf(sMute, "%2d", softMuteMaxAttIdx);
  printParam(sMute);
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
    rx.setAutomaticGainControl( (disableAgc = (agcIdx > 0)), agcNdx);
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    rx.setSeekAmSpacing(5); // Max 10kHz for spacing
    rx.setAvcAmMaxGain(avcIdx);
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStepIdx = band[bandIdx].currentStepIdx;
  rssi = 0;
  showStatus();
  showCommandStatus((char *) "Band");
}

void loadSSB() {
  rx.setI2CFastModeCustom(400000); // You can try rx.setI2CFastModeCustom(700000); or greater value
  rx.loadPatch(ssb_patch_content, size_content, bandwidthSSB[bwIdxSSB].idx);
  rx.setI2CFastModeCustom(100000);
  ssbLoaded = true; 
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
      
    } else {
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
 * Show cmd on display. It means you are setting up something.  
 */
void showCommandStatus(char * currentCmd )
{
  spr.setFreeFont(&Orbitron_Light_24);
  spr.fillRect(110, 0, 90, 25, TFT_BLACK);
  spr.drawString(currentCmd, 160, 12);
  spr.pushSprite(0, 0);
}

/**
 * Show menu options
 */
void showMenu() {
  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.setFreeFont(&Orbitron_Light_32);
  spr.drawString(menu[menuIdx], 100, 50);
  showCommandStatus( (char *) "Menu");
  spr.pushSprite(0, 0);
}

/**
 *  AGC and attenuattion setup
 */
void doAgc(int8_t v) {
  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;
  if (agcIdx < 0 )
    agcIdx = 35;
  else if ( agcIdx > 35)
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
   sets the Automatic Volume Control
*/
void doAvc(int8_t v)
{
  if ( currentMode != FM ) {
    avcIdx = (v == 1) ? avcIdx + 2 : avcIdx - 2;
    if (avcIdx > 90)
      avcIdx = 12;
    else if (avcIdx < 12)
      avcIdx = 90;

    rx.setAvcAmMaxGain(avcIdx);
    showAvc();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

/**
 * Switches the current step
 */
void doStep(int8_t v)
{
    if ( currentMode == FM ) {
      idxFmStep = (v == 1) ? idxFmStep + 1 : idxFmStep - 1;
      if (idxFmStep > lastFmStep)
        idxFmStep = 0;
      else if (idxFmStep < 0)
        idxFmStep = lastFmStep;
        
      currentStepIdx = idxFmStep;
      rx.setFrequencyStep(tabFmStep[currentStepIdx]);
      
    } else {
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
 * Switches to the AM, LSB or USB modes
 */
void doMode(int8_t v)
{
  if (currentMode != FM)
  {
    if (v == 1)  { // clockwise
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
    } else { // and counterclockwise
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
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
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
  if ((currentMode == LSB || currentMode == USB)) return; // It does not work for SSB mode
  
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  currentFrequency = rx.getFrequency();
  
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
 *  Menu options selection
 */
void doMenu( int8_t v) {
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
 * Starts the MENU action process
 */
void doCurrentMenuCmd() {
  disableCommands();
  switch (currentMenuCmd) {
     case 0:                 // VOLUME
      cmdVolume = true;
      showVolume();
      break;
    case 1:                 // STEP
      cmdStep = true;
      showStep();
      break;
    case 2:                 // MODE
      cmdMode = true;
      showMode();
      break;
    case 3:
        bfoOn = true;
        if ((currentMode == LSB || currentMode == USB)) {
          showBFO();
        }
      // showFrequency();
      break;      
    case 4:                 // BW
      cmdBandwidth = true;
      showBandwidth();
      break;
    case 5:                 // AGC/ATT
      cmdAgc = true;
      showAgcAtt();
      break;
    case 6:
       cmdAvc =  true;
       showAvc();
       break;
    case 7: 
      cmdSoftMuteMaxAtt = true;
      showSoftMute();  
      break;
    case 8:
      doSeek();
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
bool isMenuMode() {
  return (cmdMenu | cmdStep | cmdBandwidth | cmdAgc | cmdAvc | cmdVolume | cmdSoftMuteMaxAtt | cmdMode);
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
      showBFO();
    }
    else if (cmdMenu)
      doMenu(encoderCount);
    else if (cmdMode)
      doMode(encoderCount);
    else if (cmdStep)
      doStep(encoderCount);
    else if (cmdAgc)
      doAgc(encoderCount);
    else if (cmdAvc)
      doAvc(encoderCount);
    else if (cmdBandwidth)
      doBandwidth(encoderCount);
    else if (cmdVolume)
      doVolume(encoderCount);
    else if (cmdSoftMuteMaxAtt)
      doSoftMute(encoderCount);
    else if (cmdBand)
      setBand(encoderCount);
    else
    {
      if (encoderCount == 1)
      {
        seekDirection = 1;
        rx.frequencyUp();
      }
      else
      {
        seekDirection = 0;
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
          showStatus();
          showCommandStatus((char *)"VFO ");
        }
        else if (bfoOn) {
          bfoOn = false;
          showStatus();
        }
        else
        {
          cmdBand = !cmdBand;
          showCommandStatus((char *)"Band");
        }
      }
      else
      { // GO to MENU if more than one click in less than 1/2 seconds.
        cmdMenu = !cmdMenu;
        if (cmdMenu)
          showMenu();
      }
      delay(MIN_ELAPSED_TIME);
      elapsedCommand = millis();
    }
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > (MIN_ELAPSED_RSSI_TIME * 6))
  {
    rx.getCurrentReceivedSignalQuality();
    int aux = rx.getCurrentRSSI();
    if (rssi != aux && !isMenuMode())
    {
      rssi = aux;
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  // Disable commands control
  if ((millis() - elapsedCommand) > ELAPSED_COMMAND)
  {
    if ((currentMode == LSB || currentMode == USB) )
      bfoOn = false;
    elapsedCommand = millis();
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

  if ((millis() - elapsedBattery) > ELAPSED_TIME_BATERRY )
  {
      elapsedBattery = millis();
      showBattery();
  }

}
