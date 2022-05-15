/*

  ATTENTION: Under Construction....

  arduino-cli command line:
  arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso ./SI473X_03_ESP32_LCD16x2_ALL_IN_ONE
  

  It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
  regular  comercial  stations.

  The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735-D60 or Si4732-A10  and  the
  "PU2CLR SI4735 Arduino Library". It is not the purpose of this prototype  to provide you a beautiful interface. You can do it better.

  Features:   AM; SSB; LW/MW/SW; external mute circuit control; AGC; Attenuation gain control;
              SSB filter; CW; AM filter; 1, 5, 10, 50 and 500kHz step on AM and 10Hhz sep on SSB

  ESP32 Wire up with LCD, encoder/pushbutton and SI4735-D60

  | Device name               | Device Pin / Description      |  ESP32        |
  | ----------------          | ----------------------------- | ------------- |
  |    LCD 16x2 or 20x4       |                               |               |
  |                           | D4                            |  GPIO18       |
  |                           | D5                            |  GPIO17       |
  |                           | D6                            |  GPIO16       |
  |                           | D7                            |  GPIO15       |
  |                           | RS                            |  GPIO19       |
  |                           | E/ENA                         |  GPIO23       |
  |                           | RW & VSS & K (16)             |  GND          |
  |                           | A (15) & VDD                  |  +Vcc         |
  |                           | VO (see 20K tripot connection)|  ------------ |
  |     SS473X                |                               |               |
  |                           | RESET (pin 15)                |  GPIO12       |
  |                           | SDIO (pin 18)                 |  GPIO21       |
  |                           | SCLK (pin 17)                 |  GPIO22       |
  |                           | (*1)SEN (pin 16)              |  +Vcc or GND  |
  |    Encoder                |                               |               |
  |                           | A                             |  CPIO13       |
  |                           | B                             |  GPIO14       |
  |                           | PUSH BUTTON (encoder)         |  GPIO27       |
  |    Band Pass Filter       |                               |               |
  |                           | S0                            | GPIO25        |
  |                           | S1                            | GPIO26        |
  |    External Audio Mute    |                               |               |
  |                           | External Audio Mute  Optional | GPIO35        |


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

#include <SI4735.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
// #include <LiquidCrystal_I2C.h>
#include "Rotary.h"
#include <patch_init.h> // SSB patch for whole SSBRX initialization string
#include "AutoBPF.h"    // Small Arduino Library to control the BPF device. See https://github.com/pu2clr/auto_bpf_arduino

const uint16_t size_content = sizeof ssb_patch_content; // see patch_init.h

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 13
#define ENCODER_PIN_B 14

// Band Pass Filter device
#define BPF_S0 25      // GPIO25
#define BPF_S1 26      // GPIO26

// Optional External Audio Mute
#define AUDIO_MUTE 35  // GPIO26

// LCD 16x02 or LCD20x4 PINs
#define LCD_D7 15
#define LCD_D6 16
#define LCD_D5 17
#define LCD_D4 18
#define LCD_RS 19
#define LCD_E  23

// Buttons controllers
#define ENCODER_PUSH_BUTTON 27 // 
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

#define EEPROM_SIZE        512

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
int8_t avcIdx;   // min 12 and max 90 
uint8_t countClick = 0;

uint8_t seekDirection = 1;

bool cmdBand = false;
bool cmdVolume = false;
bool cmdAgc = false;
bool cmdBandwidth = false;
bool cmdStep = false;
bool cmdMode = false;
bool cmdMenu = false;
bool cmdRds  =  false;
bool cmdSoftMuteMaxAtt = false;
bool cmdAvc = false;
bool cmdBPF = false;

bool fmRDS = false;

int16_t currentBFO = 0;
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();
long elapsedClick = millis();
volatile int encoderCount = 0;
uint16_t currentFrequency;
uint16_t previousFrequency = 0;

uint8_t currentBPF;


const uint8_t currentBFOStep = 10;

const char * menu[] = {"Volume", "FM RDS", "Step", "Mode", "BFO", "BW", "AGC/Att", "SoftMute", "AVC", "Seek Up", "Seek Down", "BPF"};
int8_t menuIdx = 0;
const int lastMenu = 11;
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
  uint8_t disableAgc;
  int8_t agcIdx;
  int8_t agcNdx;
  int8_t avcIdx; 
  uint8_t filter;         // Band Pass Filter used for the band (0-3) 
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
    {"VHF", FM_BAND_TYPE, 6400, 10800, 10390, 1, 0, 1, 0, 0, 0, 3}, // Filter is not applicable on this band
    {"MW1", MW_BAND_TYPE, 150, 1720, 810, 3, 4, 0, 0, 0, 32, 3},    // Filter is not applicable on this band
    {"MW2", MW_BAND_TYPE, 531, 1701, 783, 2, 4, 0, 0, 0, 32, 3},    // Filter is not applicable on this band
    {"MW3", MW_BAND_TYPE, 1700, 3500, 2500, 1, 4, 1, 0, 0, 32, 0},  // Filter number 0 (S0 = 0; S1 = 0)
    {"80M", MW_BAND_TYPE, 3500, 4000, 3700, 0, 4, 1, 0, 0, 32, 0},
    {"SW1", SW_BAND_TYPE, 4000, 5500, 4885, 1, 4, 1, 0, 0, 32, 0},
    {"SW2", SW_BAND_TYPE, 5500, 6500, 6000, 1, 4, 1, 0, 0, 32, 1}, // Filter number 1 (S0 = 1; S1 = 0)
    {"40M", SW_BAND_TYPE, 6500, 7300, 7100, 0, 4, 1, 0, 0, 40, 1},
    {"SW3", SW_BAND_TYPE, 7200, 8000, 7200, 1, 4, 1, 0, 0, 40, 1},
    {"SW4", SW_BAND_TYPE, 9000, 11000, 9500, 1, 4, 1, 0, 0, 40, 2}, // Filter number 2 (S0 = 0; S1 = 1)
    {"SW5", SW_BAND_TYPE, 11100, 13000, 11900, 1, 4, 1, 0, 0, 40, 2},
    {"SW6", SW_BAND_TYPE, 13000, 14000, 13500, 1, 4, 1, 0, 0, 40, 2},
    {"20M", SW_BAND_TYPE, 14000, 15000, 14200, 0, 4, 1, 0, 0, 42, 2},
    {"SW7", SW_BAND_TYPE, 15000, 17000, 15300, 1, 4, 1, 0, 0, 42, 2},
    {"SW8", SW_BAND_TYPE, 17000, 18000, 17500, 1, 4, 1, 0, 0, 42, 3}, // Filter number 3 (S0 = 1; S1 = 1)
    {"15M", SW_BAND_TYPE, 20000, 21400, 21100, 0, 4, 1, 0, 0, 44, 3},
    {"SW9", SW_BAND_TYPE, 21400, 22800, 21500, 1, 4, 1, 0, 0, 44, 3},
    {"CB ", SW_BAND_TYPE, 26000, 28000, 27500, 0, 4, 1, 0, 0, 44, 3},
    {"10M", SW_BAND_TYPE, 28000, 30000, 28400, 0, 4, 1, 0, 0, 44, 3},
    {"ALL", SW_BAND_TYPE, 150, 30000, 15000, 0, 4, 1, 0, 0, 48, 0} // Filter is not applicable on this band
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;


uint8_t rssi = 0;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// LiquidCrystal_I2C lcd(0x27, 20, 4);

AutoBPF bpf; // Declare the Auto bandpass filter class.

SI4735 rx;

            
void setup()
{

  // Serial.begin(115200);
  // while(!Serial);

  // Encoder pins
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  bpf.setup(BPF_S0, BPF_S1);
  lcd.begin(16, 2);


  // Splash - Remove or Change the splash message
  lcd.setCursor(0, 0);
  lcd.print("PU2CLR-SI47XX");
  lcd.setCursor(0, 1);
  lcd.print("Arduino Library");
  delay(1500);

  // End splash

  EEPROM.begin(EEPROM_SIZE);

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    EEPROM.write(eeprom_address, 0);
    lcd.setCursor(0,0);
    lcd.print("EEPROM RESETED");
    itIsTimeToSave =  true; // forces save the current status as soon as possible. 
    storeTime = 0; 
    delay(3000); 
    lcd.clear();
  }

  // controlling encoder via interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  // rx.setMaxDelayPowerUp(500);
  rx.setI2CFastModeCustom(100000);
  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error

  rx.setAudioMuteMcuPin(AUDIO_MUTE); // avoiding pop in the speaker

  rx.setup(RESET_PIN, MW_BAND_TYPE);
  // Comment the line above and uncomment the three lines below if you are using external ref clock (active crystal or signal generator)
  // rx.setRefClock(32768);
  // rx.setRefClockPrescaler(1);   // will work with 32768  
  // rx.setup(RESET_PIN, 0, MW_BAND_TYPE, SI473X_ANALOG_AUDIO, XOSCEN_RCLK);

  
  delay(300);

  // Checking the EEPROM content
  if (EEPROM.read(eeprom_address) == app_id)
  {
    readAllReceiverInformation();
  } else 
    rx.setVolume(volume);

  useBand();
  showStatus();
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

  addr_offset = 7;
  band[bandIdx].currentFreq = currentFrequency;

  for (int i = 0; i <= lastBand; i++)
  {
    EEPROM.write(addr_offset++, (band[i].currentFreq >> 8));   // stores the current Frequency HIGH byte for the band
    EEPROM.write(addr_offset++, (band[i].currentFreq & 0xFF)); // stores the current Frequency LOW byte for the band
    EEPROM.write(addr_offset++, band[i].currentStepIdx);       // Stores current step of the band
    EEPROM.write(addr_offset++, band[i].bandwidthIdx);         // table index (direct position) of bandwidth
    EEPROM.write(addr_offset++, band[i].disableAgc );
    EEPROM.write(addr_offset++, band[i].agcIdx);
    EEPROM.write(addr_offset++, band[i].agcNdx);
    EEPROM.write(addr_offset++, band[i].avcIdx);
  }

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
    band[i].disableAgc = EEPROM.read(addr_offset++);
    band[i].agcIdx = EEPROM.read(addr_offset++);
    band[i].agcNdx = EEPROM.read(addr_offset++);
    band[i].avcIdx = EEPROM.read(addr_offset++);
    
    // char str[100];
    // sprintf(str,"Pos %2.2d | disableAgc %2.2d  | agcIdx %2.2d | agcNdx %2.2d | avcIdx %2.2d", i, band[i].disableAgc, band[i].agcIdx, band[i].agcNdx, band[i].avcIdx );
    // Serial.println(str);
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
  cmdAvc =  false; 
  cmdBPF = false;
  countClick = 0;

  showCommandStatus((char *) "VFO ");
}

/**
 * Reads encoder via interrupt
 * Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
 * if you do not add ICACHE_RAM_ATTR declaration, the system will reboot during attachInterrupt call. 
 * With ICACHE_RAM_ATTR macro you put the function on the RAM.
 */
void  rotaryEncoder()
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
  char tmp[15];
  char bufferDisplay[15];
  char *unit;
  int col = 4;
  sprintf(tmp, "%5.5u", currentFrequency);
  bufferDisplay[0] = (tmp[0] == '0') ? ' ' : tmp[0];
  bufferDisplay[1] = tmp[1];
  if (rx.isCurrentTuneFM())
  {
    bufferDisplay[2] = tmp[2];
    bufferDisplay[3] = '.';
    bufferDisplay[4] = tmp[3];
    if ( fmRDS ) { 
      col = 0;
      unit = (char *)" ";
    } else {
      unit = (char *)"MHz";
    }
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
    unit = (char *)"kHz";
  }
  bufferDisplay[5] = '\0';
  strcat(bufferDisplay, unit);
  lcd.setCursor(col, 1);
  lcd.print(bufferDisplay);
  showMode();
}

/**
 * Shows the current mode
 */
void showMode()
{
  char *bandMode;

   
  if (currentFrequency < 520)
    bandMode = (char *)"LW  ";
  else
    bandMode = (char *)bandModeDesc[currentMode];
  lcd.setCursor(0, 0);
  lcd.print(bandMode);

  if ( currentMode == FM && fmRDS ) return;
  
  lcd.setCursor(0, 1);
  lcd.print(band[bandIdx].bandName);
}

/**
 * Shows some basic information on display
 */
void showStatus()
{
  lcd.clear();
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

  sprintf(bandwidth, "BW: %s", bw);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(bandwidth);
}

/**
 *   Shows the current RSSI and SNR status
 */
void showRSSI()
{
  int rssiAux = 0;
  char sMeter[7];

  if (currentMode == FM)
  {
    lcd.setCursor(14, 0);
    lcd.print((rx.getCurrentPilot()) ? "ST" : "MO");
    lcd.setCursor(10, 0);
    if ( fmRDS ) {
      lcd.print("RDS");
      return;
    }
    else 
      lcd.print("   ");
  }

    
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
  else
    rssiAux = 9;

  sprintf(sMeter, "S%1.1u%c", rssiAux, (rssi >= 60) ? '+' : '.');
  lcd.setCursor(13, 1);
  lcd.print(sMeter);
}

/**
 *    Shows the current AGC and Attenuation status
 */
void showAgcAtt()
{
  char sAgc[15];
  lcd.clear();
  rx.getAutomaticGainControl();
  if ( !disableAgc /*agcNdx == 0 && agcIdx == 0 */ )
    strcpy(sAgc, "AGC ON");
  else
    sprintf(sAgc, "ATT: %2.2d", agcNdx);

  lcd.setCursor(0, 0);
  lcd.print(sAgc);
}

/**
 *   Shows the current step
 */
void showStep()
{
  char sStep[15];

  sprintf(sStep, "Stp:%4d", (currentMode == FM)? (tabFmStep[currentStepIdx] *10) : tabAmStep[currentStepIdx] );
  lcd.setCursor(0, 0);
  lcd.print(sStep);
}

/**
 *  Shows the current BFO value
 */
void showBFO()
{
  char bfo[15];
  if (currentBFO > 0)
    sprintf(bfo, "BFO:+%4.4d", currentBFO);
  else
    sprintf(bfo, "BFO:%4.4d", currentBFO);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(bfo);
  elapsedCommand = millis();
}

/*
 *  Shows the volume level on LCD
 */
void showVolume()
{
  char volAux[12];
  sprintf(volAux, "VOLUME: %2u", rx.getVolume());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(volAux);
}

/**
 * Show Soft Mute 
 */
void showSoftMute()
{
  char sMute[18];
  sprintf(sMute, "Soft Mute: %2d", softMuteMaxAttIdx);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sMute);
}

/**
 * Shows Soft Mute 
 */
void showAvc()
{
  char sAvc[18];
  sprintf(sAvc, "AVC: %2d", avcIdx);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sAvc);
}

/**
 * Shows the current Band Pass Filter
 */
void showBPF() {
  char sBPF[5];
  sprintf(sBPF, "BPF: %2d", currentBPF);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sBPF);
}

/**
 * Shows RDS ON or OFF
 */
void showRdsSetup() 
{
  char sRdsStatus[10];
  sprintf(sRdsStatus, "RDS: %s", (fmRDS)? "ON ": "OFF");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sRdsStatus);  

}

/***************  
 *   RDS
 *   
 */
 
char *stationName;
char bufferStatioName[20];
char localTime[10];
uint16_t localYear, localMonth, localDay, localHour, localMinute; 

void clearRDS() {
   stationName = (char *) "           ";
   showRDSStation();
}

void showRDSStation()
{
    int col = 7;
    for (int i = 0; i < 8; i++ ) {
      if (stationName[i] != bufferStatioName[i] ) {
        lcd.setCursor(col + i, 1);
        lcd.print(stationName[i]); 
        bufferStatioName[i] = stationName[i];
      }
    }
    
    delay(100);
}

void showRDSTime()
{
  lcd.setCursor(8,1);
  lcd.print(localTime);
  delay(200);
}

/*
 * Checks the station name is available
 */
void checkRDS()
{
  rx.getRdsStatus();
  if (rx.getRdsReceived())
  {
    if (rx.getRdsSync() && rx.getRdsSyncFound() && !rx.getRdsSyncLost() && !rx.getGroupLost() )
    {
      stationName = rx.getRdsText0A();
      if (stationName != NULL )
      {
        showRDSStation();
      }
      if ( rx.getRdsDateTime(&localYear, &localMonth, &localDay, &localHour, &localMinute) ) {
        sprintf(localTime,"%02u:%02u", localHour, localMinute);
        showRDSTime();
        delay(500);
        clearRDS();
      }
    }
  }
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
    rx.setRdsConfig(1, 2, 2, 2, 2);
    rx.setFifoCount(1);
    
    bfoOn = ssbLoaded = false;
    bwIdxFM = band[bandIdx].bandwidthIdx;
    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);    
  }
  else
  {

    disableAgc = band[bandIdx].disableAgc;
    agcIdx = band[bandIdx].agcIdx;
    agcNdx = band[bandIdx].agcNdx;
    avcIdx = band[bandIdx].avcIdx;

    // char str[100];
    // sprintf(str,"Pos %2.2d | disableAgc %2.2d  | agcIdx %2.2d | agcNdx %2.2d | avcIdx %2.2d", bandIdx, disableAgc, agcIdx, agcNdx, avcIdx );
    // Serial.println(str);

    
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor((band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);
    if (ssbLoaded)
    {
      rx.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabAmStep[band[bandIdx].currentStepIdx], currentMode);
      rx.setSSBAutomaticVolumeControl(1);
      rx.setSsbSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Disable Soft Mute for SSB
      bwIdxSSB = band[bandIdx].bandwidthIdx;
      rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
      delay(500);
      rx.setSsbAgcOverrite(disableAgc, agcNdx);
    }
    else
    {
      currentMode = AM;
      rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabAmStep[band[bandIdx].currentStepIdx]);
      bfoOn = false;
      bwIdxAM = band[bandIdx].bandwidthIdx;
      rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
      rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Soft Mute for AM or SSB
      rx.setAutomaticGainControl(disableAgc, agcNdx);

    }
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    rx.setSeekAmSpacing(5); // Max 10kHz for spacing
    rx.setAvcAmMaxGain(avcIdx);
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStepIdx = band[bandIdx].currentStepIdx;

  // Select the right filter for the band
  bpf.setFilter(band[bandIdx].filter);
  currentBPF = bpf.getCurrentFilter();

  rssi = 0;
  showStatus();
  showCommandStatus((char *) "Band");
}


void loadSSB() {
   rx.setI2CFastModeCustom(400000); // You can try rx.setI2CFastModeCustom(700000); or greater value
   rx.loadPatch(ssb_patch_content, size_content, bandwidthSSB[bwIdxSSB].idx);
   rx.setI2CFastModeCustom(100000);  
   ssbLoaded =  true; 
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
void showCommandStatus(char * currentCmd)
{
  lcd.setCursor(5, 0);
  lcd.print(currentCmd);
}

/**
 * Show menu options
 */
void showMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.setCursor(0, 1);
  lcd.print(menu[menuIdx]);
  showCommandStatus( (char *) "Menu");
}

/**
 *  AGC and attenuattion setup
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
  if ( currentMode == AM ) 
     rx.setAutomaticGainControl(disableAgc, agcNdx); // if agcNdx = 0, no attenuation
  else
    rx.setSsbAgcOverrite(disableAgc, agcNdx, 0B1111111);

  band[bandIdx].disableAgc = disableAgc;
  band[bandIdx].agcIdx = agcIdx;
  band[bandIdx].agcNdx = agcNdx;

  showAgcAtt();
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

  lcd.clear();
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  showStatus();
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
 * Turns RDS ON or OFF
 */
void doRdsSetup(int8_t v)
{
  fmRDS = (v == 1)? true:false;
  showRdsSetup();
  elapsedCommand = millis();
}

/**
 * Switch the current BPF
 */
void doBPF(int8_t v)
{
  int8_t aux = currentBPF;
  aux = (v == 1) ? (currentBPF + 1) : (currentBPF-1);
  if (aux > 3)
    currentBPF = 0;
  else if ( aux < 0)
    currentBPF = 3;
  else 
    currentBPF = (uint8_t) aux; 

  bpf.setFilter(currentBPF);   
  showBPF(); 
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
 * Return true if the current status is Menu command
 */
bool isMenuMode() {
  return (cmdMenu | cmdStep | cmdBandwidth | cmdAgc | cmdVolume | cmdSoftMuteMaxAtt | cmdMode | cmdRds | cmdAvc | cmdBPF);
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
    case 1: 
      cmdRds = true;
      showRdsSetup();
      break;
    case 2:                 // STEP
      cmdStep = true;
      showStep();
      break;
    case 3:                 // MODE
      cmdMode = true;
      lcd.clear();
      showMode();
      break;
    case 4:
        bfoOn = true;
        if ((currentMode == LSB || currentMode == USB)) {
          showBFO();
        }
      // showFrequency();
      break;      
    case 5:                 // BW
      cmdBandwidth = true;
      showBandwidth();
      break;
    case 6:                 // AGC/ATT
      cmdAgc = true;
      showAgcAtt();
      break;
    case 7: 
      cmdSoftMuteMaxAtt = true;
      showSoftMute();  
      break;
    case 8: 
      cmdAvc =  true; 
      showAvc();
      break;  
    case 9:
      seekDirection = 1;
      doSeek();
      break;  
    case 10:
      seekDirection = 0;
      doSeek();
      break; 
    case 11: 
      showBPF();
      break;     
    default:
      showStatus();
      break;
  }
  currentMenuCmd = -1;
  elapsedCommand = millis();
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
    else if (cmdBandwidth)
      doBandwidth(encoderCount);
    else if (cmdVolume)
      doVolume(encoderCount);
    else if (cmdSoftMuteMaxAtt)
      doSoftMute(encoderCount);
    else if (cmdAvc)
      doAvc(encoderCount);
    else if (cmdBand)
      setBand(encoderCount);
    else if (cmdRds ) 
      doRdsSetup(encoderCount);  
    else if (cmdBPF)
      doBPF(encoderCount);
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
          showStatus();
          showCommandStatus((char *)"VFO ");
        }
        else if ( bfoOn ) {
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

  // Disable commands control
  if ((millis() - elapsedCommand) > ELAPSED_COMMAND)
  {
    if ((currentMode == LSB || currentMode == USB) )
    {
      bfoOn = false;
      // showBFO();
      showStatus();
    } else if (isMenuMode()) 
      showStatus();
    disableCommands();
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

  if (currentMode == FM && fmRDS && !isMenuMode() )
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
