/*
  UNDER CONSTRUCTION... 
 
  IMPORTANT:
  This sketch is an EXPERIMENT with dual converter designed by Plamen Panteleev (LZ1PPL).

  Features: 
                1) Dual converter (from 150kHz to 170MHz)
                2) The receiver current status is stored into Arduino EEPROM;


  See user_manual.txt before operating the receiver. 

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  LZ1PPL website: https://www.lz1ppl.com/en/2021/04/22/si4735-all-mode-reciver/?fbclid=IwAR10n3x89ayj332m2X3x65AAR6bMVARHZ5VdDtkEzusT_qmUE-F6u9_QIaI

  By PU2CLR, Ricardo, Jul  2021.
*/

#include <SI4735.h>
#include <si5351.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DSEG7_Classic_Regular_16.h"
#include "Rotary.h"

// #include <patch_init.h> // SSB patch for whole SSBRX initialization string
#include <patch_ssb_compressed.h> // Compressed SSB patch version (saving almost 1KB)

const uint16_t size_content = sizeof ssb_patch_content; // See ssb_patch_content.h
const uint16_t cmd_0x15_size = sizeof cmd_0x15;         // Array of lines where the 0x15 command occurs in the patch content.


#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define RESET_PIN PB12

// Enconder PINs
#define ENCODER_PIN_A PA4
#define ENCODER_PIN_B PA5

// Buttons controllers
#define ENCODER_PUSH_BUTTON PA3 

#define MIN_ELAPSED_TIME 300
#define MIN_ELAPSED_RSSI_TIME 500
#define ELAPSED_COMMAND 2000 // time to turn off the last command controlled by encoder. Time to goes back to the FVO control
#define ELAPSED_CLICK 1500   // time to check the double click commands
#define DEFAULT_VOLUME 15    // change it for your favorite sound volume

// SI5351 PARAMETERS
#define SI5351_CALIBRATION_OFFSET  39000  // SI5351 calibration factor 
#define IF_AMI_OFFSET 10700               // AM Offset - Set the IF offset you are using. For example: +1070000000LLU (increase 10.7MHz) or -1070000000LLU (decrease 10.7Mhz); +45500000LU (455kHz) or -45500000LU (-455kHz)
#define IF_FMI_OFFSET 6570                // FM Offset - On FM 6570 means 65,70 MHz
#define FREQUENCY_UP 1
#define FREQUENCY_DOWN -1      
#define MFREQ 1000000ULL                 // For example: 65.700.000 * 100ULL

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1

#define EEPROM_SIZE        512

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

// EEPROM - Stroring control variables
const uint8_t app_id = 31; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

bool itIsTimeToSave = false;

bool bfoOn = false;
bool ssbLoaded = false;

int8_t agcIdx = 0;
uint8_t disableAgc = 0;
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


bool fmRDS = false;

int16_t currentBFO = 0;
long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedCommand = millis();
long elapsedClick = millis();
volatile int encoderCount = 0;
uint32_t currentFrequency;
uint16_t previousFrequency = 0;
int32_t currentIF = IF_AMI_OFFSET;
int currentStep = 0;
const uint8_t currentBFOStep = 10;

const char * menu[] = {"Volume", "FM RDS", "Step", "Mode", "BFO", "BW", "AGC/Att", "SoftMute", "AVC", "Seek Up", "Seek Down"};
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
  uint32_t minimumFreq;   // Minimum frequency of the band
  uint32_t maximumFreq;   // maximum frequency of the band
  uint32_t currentFreq;   // Default frequency or current frequency
  int8_t currentStepIdx;  // Idex of tabStepAM:  Defeult frequency step (See tabStepAM)
  int8_t bandwidthIdx;    // Index of the table bandwidthFM, bandwidthAM or bandwidthSSB;
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
    {"FM ", FM_BAND_TYPE, 6600, 10800, 10650, 1, 0, 1, 0, 0, 0},
    {"MW1", MW_BAND_TYPE, 150, 1720, 810, 3, 4, 0, 0, 0, 32},
    {"80M", MW_BAND_TYPE, 1700, 4000, 3700, 0, 4, 1, 0, 0, 32},
    {"SW1", SW_BAND_TYPE, 4000, 6500, 6000, 1, 4, 1, 0, 0, 32},
    {"40M", SW_BAND_TYPE, 6500, 7300, 7100, 0, 4, 1, 0, 0, 40},
    {"SW2", SW_BAND_TYPE, 7200, 14000, 7200, 1, 4, 1, 0, 0, 40},
    {"20M", SW_BAND_TYPE, 14000, 15000, 14200, 0, 4, 1, 0, 0, 42},
    {"SW7", SW_BAND_TYPE, 15000, 18000, 15300, 1, 4, 1, 0, 0, 42},
    {"15M", SW_BAND_TYPE, 20000, 21400, 21100, 0, 4, 1, 0, 0, 44},
    {"SW9", SW_BAND_TYPE, 21400, 22800, 21500, 1, 4, 1, 0, 0, 44},
    {"CB ", SW_BAND_TYPE, 26000, 28000, 27500, 0, 4, 1, 0, 0, 44},
    {"10M", SW_BAND_TYPE, 28000, 30000, 28400, 0, 4, 1, 0, 0, 44},
    {"6M ", SW_BAND_TYPE, 50000, 55000, 50110, 0, 4, 1, 0, 0, 44},
    {"AIR", SW_BAND_TYPE, 118000, 137000, 127700, 0, 4, 1, 0, 0, 48},    // All band from 150kHz to 170MHz on AM and SSB modes)
    {"AL1", SW_BAND_TYPE, 150, 170000, 28400, 0, 4, 1, 0, 0, 48},    // All band from 150kHz to 170MHz on AM and SSB modes)
    {"AL2", FM_BAND_TYPE, 150, 170000, 145000, 0, 4, 1, 0, 0, 48}     // All band from 150kHz to 170MHz on FM  mode)
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int tabStep[] = {1, 5, 10, 50, 100, 500, 1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;

// Useful to convert long frequency to sequence of bytes 
typedef union { 
    uint32_t frequency;
    uint8_t  raw[4];
} LongFrequency; 


uint8_t rssi = 0;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

SI4735 rx;
Si5351 vfo(0x62); 


void setup()
{
  // Encoder pins
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // Starts Splash
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  display.display();
  display.setTextColor(SSD1306_WHITE);

  // Splash - Remove or change it for your introduction text.
  display.clearDisplay();
  print(0, 0, NULL, 1, "PU2CLR");
  print(0, 15, NULL, 1, "Plamen's Kit");
  display.display();
  delay(2000);
  display.clearDisplay();
  print(0, 0, NULL, 1, "SI5351/SI4732");
  print(0, 15, NULL, 1, "Dual Conversion.");
  display.display();
  // Ends Splash

  // EEPROM.begin();
 
  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    EEPROM.update(eeprom_address, 0);
    display.setCursor(0,0);
    display.print("EEPROM RESETED");
    delay(2000);
    display.clearDisplay();
  }

  vfo.init(SI5351_CRYSTAL_LOAD_10PF, 27000000, SI5351_CALIBRATION_OFFSET);
  vfo.output_enable(SI5351_CLK0, 1);                  //1 - Enable / 0 - Disable CLK
  vfo.drive_strength(SI5351_CLK0, SI5351_DRIVE_4MA);  //Output current 2MA, 4MA, 6MA or 8MA
 
  // vfo.update_status();
  
  delay(100);

  // controlling encoder via interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  rx.setI2CFastModeCustom(100000);
  
  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error
  
  rx.setup(RESET_PIN, MW_BAND_TYPE);
  // Comment the line above and uncomment the three lines below if you are using external ref clock (active crystal or signal generator)
  // rx.setRefClock(32768);
  // rx.setRefClockPrescaler(1);   // will work with 32768  
  // rx.setup(RESET_PIN, 0, MW_BAND_TYPE, SI473X_ANALOG_AUDIO, XOSCEN_RCLK);

  
  delay(300);
  rx.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM/SSB mode (you can use values between 12 and 90dB).


  // Checking the EEPROM content
  if (EEPROM.read(eeprom_address) == app_id)
  {
    // rx.setVolume(volume);
    // currentFrequency = band[bandIdx].currentFreq;
    readAllReceiverInformation();
  } else {
    rx.setVolume(volume);
    currentFrequency = band[bandIdx].currentFreq;
  }
  
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
  LongFrequency freqAux;

  // EEPROM.begin();

  EEPROM.update(eeprom_address, app_id);                 // stores the app id;
  EEPROM.update(eeprom_address + 1, rx.getVolume()); // stores the current Volume
  EEPROM.update(eeprom_address + 2, bandIdx);            // Stores the current band
  EEPROM.update(eeprom_address + 3, fmRDS);
  EEPROM.update(eeprom_address + 4, currentMode); // Stores the current Mode (FM / AM / SSB)
  EEPROM.update(eeprom_address + 5, currentBFO >> 8);
  EEPROM.update(eeprom_address + 6, currentBFO & 0XFF);

  addr_offset = 7;
  band[bandIdx].currentFreq = currentFrequency;

  for (int i = 0; i <= lastBand; i++)
  {
    freqAux.frequency = band[i].currentFreq;
    EEPROM.update(addr_offset++, freqAux.raw[0]);    // stores the current Frequency HIGH byte for the band
    EEPROM.update(addr_offset++, freqAux.raw[1]);    // 
    EEPROM.update(addr_offset++, freqAux.raw[2]);    // 
    EEPROM.update(addr_offset++, freqAux.raw[3]);    // stores the current Frequency LOW byte for the band

    EEPROM.update(addr_offset++, band[i].currentStepIdx);       // Stores current step of the band
    EEPROM.update(addr_offset++, band[i].bandwidthIdx);         // table index (direct position) of bandwidth
    EEPROM.update(addr_offset++, band[i].disableAgc );
    EEPROM.update(addr_offset++, band[i].agcIdx);
    EEPROM.update(addr_offset++, band[i].agcNdx);
    EEPROM.update(addr_offset++, band[i].avcIdx);
  }

  // EEPROM.end();
}

/**
 * reads the last receiver status from eeprom. 
 */
void readAllReceiverInformation()
{
  uint8_t volume;
  int addr_offset;
  int bwIdx;
  LongFrequency freqAux;

  volume = EEPROM.read(eeprom_address + 1); // Gets the stored volume;
  bandIdx = EEPROM.read(eeprom_address + 2);
  fmRDS = EEPROM.read(eeprom_address + 3);
  currentMode = EEPROM.read(eeprom_address + 4);
  currentBFO = EEPROM.read(eeprom_address + 5) << 8;
  currentBFO |= EEPROM.read(eeprom_address + 6);

  addr_offset = 7;
  for (int i = 0; i <= lastBand; i++)
  {
    freqAux.raw[0] = EEPROM.read(addr_offset++);
    freqAux.raw[1] = EEPROM.read(addr_offset++);
    freqAux.raw[2] = EEPROM.read(addr_offset++);
    freqAux.raw[3] = EEPROM.read(addr_offset++);
    band[i].currentFreq = freqAux. frequency;
    band[i].currentStepIdx = EEPROM.read(addr_offset++);
    band[i].bandwidthIdx = EEPROM.read(addr_offset++);
    band[i].disableAgc = EEPROM.read(addr_offset++);
    band[i].agcIdx = EEPROM.read(addr_offset++);
    band[i].agcNdx = EEPROM.read(addr_offset++);
    band[i].avcIdx = EEPROM.read(addr_offset++);
  }


  currentFrequency = band[bandIdx].currentFreq;

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentStepIdx = idxFmStep = band[bandIdx].currentStepIdx;
    currentStep = tabFmStep[currentStepIdx];

  }
  else
  {
    currentStepIdx = idxAmStep = band[bandIdx].currentStepIdx;
    currentStep = tabAmStep[currentStepIdx];
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
 * Prints a given content on display 
 */
void print(uint8_t col, uint8_t lin, const GFXfont *font, uint8_t textSize, const char *msg)
{
  display.setFont(font);
  display.setTextSize(textSize);
  display.setCursor(col, lin);
  display.print(msg);
}

void printParam(const char *msg)
{
  display.fillRect(0, 10, 128, 10, SSD1306_BLACK);
  print(0, 10, NULL, 1, msg);
  display.display();
}


/**
 * Shows frequency information on Display
 */
void showFrequency()
{
  char tmp[15];
  char bufferDisplay[15];
  char *unit;
  sprintf(tmp, "%5.5ul", currentFrequency);
  bufferDisplay[0] = (tmp[0] == '0') ? ' ' : tmp[0];
  bufferDisplay[1] = tmp[1];
  if (rx.isCurrentTuneFM())
  {
    bufferDisplay[2] = tmp[2];
    bufferDisplay[3] = '.';
    bufferDisplay[4] = tmp[3];
    bufferDisplay[5] = tmp[4];
    unit = (char *)"MHz";
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
  // strcat(bufferDisplay, unit);
  // display.setTextSize(2);
  display.setFont(&DSEG7_Classic_Regular_16);
  display.clearDisplay();
  display.setCursor(10, 25);
  display.print(bufferDisplay);
  display.setCursor(90, 15);
  display.setFont(NULL);
  display.setTextSize(1);
  display.print(unit);
  display.display();

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

  display.setTextSize(1);
  // display.clearDisplay();
  display.setCursor(0, 0);
  display.print(bandMode);
  display.setCursor(90, 0);
  display.print(band[bandIdx].bandName);
  display.display();
}

/**
 * Shows some basic information on display
 */
void showStatus()
{
  display.clearDisplay();
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
  printParam(bandwidth);
}

/**
 *   Shows the current RSSI and SNR status
 */
void showRSSI()
{
  char sMeter[10];
  sprintf(sMeter, "S:%d ", rssi);

  display.fillRect(0, 25, 128, 10, SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(90, 25);
  display.print(sMeter);
  if (currentMode == FM)
  {
    display.setCursor(0, 25);
    display.print((rx.getCurrentPilot()) ? "ST" : "MO");
  }

  display.display();
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
 *   Shows the current step
 */
void showStep()
{
  char sStep[15];
  if (currentMode == FM)
  {
    sprintf(sStep, "Stp:%4d", tabFmStep[currentStepIdx] * 10);
  }
  else
  {
    sprintf(sStep, "Stp:%4d", tabAmStep[currentStepIdx]);
  }
  printParam(sStep);
}

/**
 *  Shows the current BFO value
 */
void showBFO()
{
  char bfo[18];
  
  if (currentBFO > 0)
    sprintf(bfo, "BFO: +%4.4d", currentBFO);
  else
    sprintf(bfo, "BFO: %4.4d", currentBFO);

  printParam(bfo);
}

/*
 *  Shows the volume level on LCD
 */
void showVolume()
{
  char volAux[12];
  sprintf(volAux, "VOLUME: %2u", rx.getVolume());
  printParam(volAux);
}

/**
 * Show Soft Mute 
 */
void showSoftMute()
{
  char sMute[18];
  sprintf(sMute, "Soft Mute: %2d", softMuteMaxAttIdx);
  printParam(sMute);
}


/**
 * Show Soft Mute 
 */
void showAvc()
{
  char sAvc[10];
  sprintf(sAvc, "AVC: %2d", avcIdx);
  printParam(sAvc);
}



/**
 * Shows RDS ON or OFF
 */
void showRdsSetup() 
{
  char sRdsSetup[10];
  sprintf(sRdsSetup, "RDS: %s", (fmRDS) ? "ON " : "OFF");
  printParam(sRdsSetup);
}

/***************  
 *   RDS
 *   
 */

char *stationName;
char bufferStatioName[20];

void clearRDS() {
  /*
   stationName = (char *) "           ";
   showRDSStation();
   */
}

void showRDSStation()
{
  /*
    int col = 8;
    for (int i = 0; i < 8; i++ ) {
      if (stationName[i] != bufferStatioName[i] ) {
        lcd.setCursor(col + i, 1);
        lcd.print(stationName[i]); 
        bufferStatioName[i] = stationName[i];
      }
    }
    delay(100);
  */
}

/*
 * Checks the station name is available
 */
void checkRDS()
{
  /*
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
    }
  }
  */
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
    currentIF = IF_FMI_OFFSET;
    currentStep = tabFmStep[band[bandIdx].currentStepIdx];

    rx.setTuneFrequencyAntennaCapacitor(0);
    rx.setFM(currentIF - 10, currentIF + 10, currentIF, tabFmStep[band[bandIdx].currentStepIdx]);

    // rx.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    // rx.setRdsConfig(1, 2, 2, 2, 2);
    // rx.setFifoCount(1);
    
    bfoOn = ssbLoaded = false;
    bwIdxFM = band[bandIdx].bandwidthIdx;
    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx); 
    delay(100);
    rx.setFrequency(currentIF);   
  }
  else
  {
    currentStep = tabAmStep[band[bandIdx].currentStepIdx];
    disableAgc = band[bandIdx].disableAgc;
    agcIdx = band[bandIdx].agcIdx;
    agcNdx = band[bandIdx].agcNdx;
    avcIdx = band[bandIdx].avcIdx;
    
    // set the tuning capacitor for SW or MW/LW
    rx.setTuneFrequencyAntennaCapacitor((band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) ? 0 : 1);
    if (ssbLoaded)
    {
      rx.setSSB(IF_AMI_OFFSET - 10, IF_AMI_OFFSET + 10, IF_AMI_OFFSET, tabAmStep[band[bandIdx].currentStepIdx], currentMode);
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
      rx.setAM(IF_AMI_OFFSET - 10, IF_AMI_OFFSET + 10, IF_AMI_OFFSET, tabAmStep[band[bandIdx].currentStepIdx]);
      bfoOn = false;
      bwIdxAM = band[bandIdx].bandwidthIdx;
      rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
      rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Soft Mute for AM or SSB
      rx.setAutomaticGainControl(disableAgc, agcNdx);
    }
    rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq); // Consider the range all defined current band
    rx.setSeekAmSpacing(5); // Max 10kHz for spacing
    rx.setAvcAmMaxGain(avcIdx);
    delay(100);
    rx.setFrequency(IF_AMI_OFFSET);
    currentIF = IF_AMI_OFFSET / 10; // Adjusts AM IF to work with SI5351
  }
  delay(500);
  currentFrequency = band[bandIdx].currentFreq;
  currentStepIdx = band[bandIdx].currentStepIdx;

  vfo.set_freq( (currentFrequency - currentIF) * MFREQ, SI5351_CLK0);

  rssi = 0;
  showStatus();
  showCommandStatus((char *) "Band");
}


void loadSSB() {
  // rx.setI2CFastModeCustom(700000); // It is working. Faster, but I'm not sure if it is safe.
  rx.setI2CFastModeCustom(400000);
  rx.queryLibraryId(); // Is it really necessary here? I will check it.
  rx.patchPowerUp();
  delay(50);
  rx.downloadCompressedPatch(ssb_patch_content, size_content, cmd_0x15, cmd_0x15_size);
  rx.setSSBConfig(bandwidthSSB[bwIdxSSB].idx, 1, 0, 1, 0, 1);
  rx.setI2CStandardMode();
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
void showCommandStatus(char * currentCmd)
{
  display.fillRect(40, 0, 50, 8, SSD1306_BLACK);
  display.setCursor(40, 0);
  display.print(currentCmd);
  display.display();
}

/**
 * Show menu options
 */
void showMenu() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(menu[menuIdx]);
  display.display();
  showCommandStatus((char *)"Menu");
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
      currentStep = tabFmStep[currentStepIdx];
      
    } else {
      idxAmStep = (v == 1) ? idxAmStep + 1 : idxAmStep - 1;
      if (idxAmStep > lastAmStep)
        idxAmStep = 0;
      else if (idxAmStep < 0)
        idxAmStep = lastAmStep;

      currentStepIdx = idxAmStep;
      currentStep = tabAmStep[currentStepIdx];
      // rx.setSeekAmSpacing(5); // Max 10kHz for spacing
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
  /*
  if ((currentMode == LSB || currentMode == USB)) return; // It does not work for SSB mode

  display.clearDisplay();
  rx.seekStationProgress(showFrequencySeek, seekDirection);
  showStatus();
  currentFrequency = rx.getFrequency();
  */
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
  return (cmdMenu | cmdStep | cmdBandwidth | cmdAgc | cmdVolume | cmdSoftMuteMaxAtt | cmdMode | cmdRds | cmdAvc);
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
      display.clearDisplay();
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
    default:
      showStatus();
      break;
  }
  currentMenuCmd = -1;
  elapsedCommand = millis();
}


void setVfoFrequency(int direction) {
  
    currentFrequency += currentStep * direction;
    band[bandIdx].currentFreq = currentFrequency;
    if (currentFrequency > band[bandIdx].maximumFreq) 
      currentFrequency = band[bandIdx].minimumFreq; 
    else if (currentFrequency < band[bandIdx].minimumFreq) 
      currentFrequency = band[bandIdx].maximumFreq;

    vfo.set_freq( (currentFrequency - currentIF) * MFREQ, SI5351_CLK0);
    
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
    else
    {
      if (encoderCount == 1)
      {
        setVfoFrequency(1);
      }
      else
      {
        setVfoFrequency(-1);
      }
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
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 6)
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

  delay(2);
}
