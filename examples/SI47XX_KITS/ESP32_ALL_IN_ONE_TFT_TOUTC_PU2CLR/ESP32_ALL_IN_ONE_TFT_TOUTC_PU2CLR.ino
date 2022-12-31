/*
  
  This sketch works with GERT BAAK circuit and Thiago's KIT based on SI4732 and THT touch 2.8" based on ILI9341

  ATTENTION: You migh need to calibrate the touch. 

  ABOUT SSB PATCH:  
 
  First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr. 
  Vadim Afonkin on his Dropbox repository. It is important to note that the author of this library does not encourage anyone to use the SSB patches 
  content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library.

  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
  There is little information available about patching the SI4735 or Si4732 devices. The following information is the understanding of the author of
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
  Patches to the SI473X are distributed in binary form and have to be transferred to the internal RAM of the device by
  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.


  The table below was copied from Gert's sketch and ca help you to connect your ESP32 to SI473X, TFT and other devices. 

  |------------|------------------|------------|------------|------------|
  |Display 2.8 |      ESP32       |   Si4735   |  Encoder   |  Beeper    |
  |  ILI9341   |                  |            |            |            |        Encoder        1,2,3
  |------------|------------------|------------|------------|------------|        Encoder switch 4,5
  |   Vcc      |     3V3     | 01 |    Vcc     |            |            |        pin 33 with 18K to 3.3 volt and 18K to ground.
  |   GND      |     GND     | 02 |    GND     |     2,4    |            |        pin 32 (Beeper) via 2K to base V1  BC547
  |   CS       |     15      | 03 |            |            |            |        Collector via beeper to 5v
  |   Reset    |      4      | 04 |            |            |            |        Emmitor to ground
  |   D/C      |      2      | 05 |            |            |            |
  |   SDI      |     23      | 06 |            |            |            |        Encoder        1,2,3
  |   SCK      |     18      | 07 |            |            |            |        Encoder switch 4,5
  |   LED Coll.|     14 2K   | 08 |            |            |            |        Display LED
  |   SDO      |             | 09 |            |            |            |        Emmitor  V2 BC557 to 3.3 V
  |   T_CLK    |     18      | 10 |            |            |            |        Base with 2K to pin 14 (Display_Led)
  |   T_CS     |      5      | 11 |            |            |            |        Collector to led pin display
  |   T_DIN    |     23      | 12 |            |            |            |
  |   T_DO     |     19      | 13 |            |            |            |
  |   T_IRQ    |     34      | 14 |            |            |            |
  |            |     12      |    |   Reset    |            |            |
  |            |     21      |    |    SDA     |            |            |
  |            |     22      |    |    SCL     |            |            |
  |            |     16      |    |            |      1     |            |
  |            |     17      |    |            |      3     |            |
  |            |     33      |    |            |      5     |            |
  |            |     32 2K   |    |            |            |     In     |
  |            |     27 Mute |    |see schematics                        |
  |------------|-------------|----|------------|------------|------------|


  Gert's shecmatic: https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino?fbclid=IwAR3TQd2j4HxAFvpcGkbXiPuDly8m2OnGclTDiqthnkbqqe2fN1McP2m3WSI
  Thiago's project: https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_KITS/THIAGO_LIMA

  SI473X Arduino Library documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, May 2021
  
*/

#include <SI4735.h>
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
#include "DSEG7_Classic_Mini_Regular_48.h"
#include "DSEG7_Classic_Mini_Regular_38.h"
#include "Rotary.h"
#include "EEPROM.h"

// #include <patch_init.h> // SSB patch for whole SSBRX initialization string
// #include <patch_init.h>  // SSB patch full - It is not clear. No difference found if compared with patch_init
#include "patch_3rd.h" // 3rd patch. Taken from DEGEN DE1103 receiver according to the source.

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define DISPLAY_LED 14   // Pin used to control to turn the display on or off
#define DISPLAY_ON   0   
#define DISPLAY_OFF  1

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define RESET_PIN 12           // Mega2560 digital Pin used to RESET
#define ENCODER_PUSH_BUTTON 33 // Used to switch BFO and VFO or other function
#define AUDIO_MUTE_CIRCUIT 27  // If you have an external mute circuit, use this pin to connect it.

// Enconder PINs (interrupt pins used on DUE. All Digital DUE Pins can be used as interrupt)
#define ENCODER_PIN_A 17
#define ENCODER_PIN_B 16

#define AM_FUNCTION 1
#define FM_FUNCTION 0

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define MIN_ELAPSED_TIME 250
#define MIN_ELAPSED_RSSI_TIME 200
#define ELAPSED_COMMAND 2000      // time to turn off the last command
#define ELAPSED_DISPLAY 60000 * 3 // Set the Display off after 3 minuts of inactivitiy 
#define DEFAULT_VOLUME 45         // change it for your favorite sound volume

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4
#define SSB 1

#define RSSI_DISPLAY_COL_OFFSET 0
#define RSSI_DISPLAY_LIN_OFFSET 100

#define KEYBOARD_LIN_OFFSET 50
#define STATUS_DISPLAY_COL_OFFSET 5
#define STATUS_DISPLAY_LIN_OFFSET 85

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define EEPROM_SIZE        512

// EEPROM - Stroring control variables
const uint8_t app_id = 47; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

long displayTime = millis();
bool displayOn = true;

// Commands controls
bool cmdBFO = false;
bool cmdAudioMute = false;
bool cmdMuteRate = false;
bool cmdVolume = false; // if true, the encoder will control the volume.
bool cmdAgcAtt = false;
bool cmdFilter = false;
bool cmdStep = false;
bool cmdBand = false;
bool cmdModeOrRDS =  false;
bool cmdSoftMuteMaxAtt = false;
bool cmdFmDE = false;

bool ssbLoaded = false;
bool fmStereo = true;
bool fmRDS = false;
bool touch = false;

bool itIsTimeToSave = false;

// AGC and attenuation control
int8_t agcIdx = 0;
uint8_t disableAgc = 0;
int8_t agcNdx = 0;
uint16_t antennaIdx = 0;
int8_t softMuteMaxAttIdx = 8;
int16_t slopIdx = 1;
int16_t muteRateIdx = 64;

int16_t currentBFO = 0;
int previousBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

uint8_t rssi = 0;

// Encoder control variables
volatile int encoderCount = 0;

typedef struct
{
  const char *bandName;
  uint8_t bandType;         // Band type (FM, MW or SW)
  uint16_t minimumFreq;     // Minimum frequency of the band
  uint16_t maximumFreq;     // maximum frequency of the band
  uint16_t currentFreq;     // Default frequency or current frequency
  int8_t currentStepIdx;  // Idex of tabStepAM:  Defeult frequency step (See tabStepAM)
  int8_t bandwidthIdx;    //  Index of the table bandwidthFM, bandwidthAM or bandwidthSSB;

} Band;

/*
   Band table
*/
Band band[] = {
    {"FM1 ", FM_BAND_TYPE, 6400,  8400,  7000, 1, 0}, // FM from 64 to 84 MHz
    {"FM2 ", FM_BAND_TYPE, 8400, 10800, 10390, 1, 0},
    {"LW  ", LW_BAND_TYPE, 100, 510, 300,  0, 4},
    {"AM1 ", MW_BAND_TYPE, 520, 1720, 810, 3, 4},
    {"AM2 ", MW_BAND_TYPE, 531, 1701, 783, 2, 4},
    {"160m", SW_BAND_TYPE, 1800, 3500, 1900, 0, 4}, // 160 meters
    {"80m ", SW_BAND_TYPE, 3500, 4500, 3700, 0, 4}, // 80 meters
    {"60m ", SW_BAND_TYPE, 4500, 5500, 4885, 1, 4},
    {"49m ", SW_BAND_TYPE, 5600, 6300, 6100, 1, 4},
    {"40m ", SW_BAND_TYPE, 6800, 7200, 7100, 0, 4}, // 40 meters
    {"41m ", SW_BAND_TYPE, 7200, 7900, 7205, 1, 4}, // 41 meters
    {"31m ", SW_BAND_TYPE, 9200, 10000, 9600, 1, 4},
    {"30m ", SW_BAND_TYPE, 10000, 11000, 10100, 0, 4}, // 30 meters
    {"25m ", SW_BAND_TYPE, 11200, 12500, 11940, 1, 4},
    {"22m ", SW_BAND_TYPE, 13400, 13900, 13600, 1, 4},
    {"20m ", SW_BAND_TYPE, 14000, 14500, 14200, 0, 4}, // 20 meters
    {"19m ", SW_BAND_TYPE, 15000, 15900, 15300, 1, 4},
    {"18m ", SW_BAND_TYPE, 17200, 17900, 17600, 1, 4},
    {"17m ", SW_BAND_TYPE, 18000, 18300, 18100, 0, 4}, // 17 meters
    {"15m ", SW_BAND_TYPE, 21000, 21499, 21200, 0, 4}, // 15 mters
    {"13m ", SW_BAND_TYPE, 21500, 21900, 21525, 1, 4}, // 15 mters
    {"12m ", SW_BAND_TYPE, 24890, 26200, 24940, 0, 4}, // 12 meters
    {"CB  ", SW_BAND_TYPE, 26200, 27900, 27500, 0, 4}, // CB band (11 meters)
    {"10m ", SW_BAND_TYPE, 28000, 30000, 28400, 0, 4},
    {"All ", SW_BAND_TYPE, 100, 30000, 15000, 0, 4} // All HF in one band
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 1;

int tabStepAM[] = {1, 5, 9, 10, 50, 100, 500, 1000};
const int lastStepAM = (sizeof tabStepAM / sizeof(int)) - 1;


int tabStepFM[] = {5, 10, 20};
const int lastStepFM = (sizeof tabStepFM / sizeof(int)) - 1;

int idxStepAM = 0;
int idxStepFM = 0;


uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
uint16_t currentStepIdx = 1;
uint8_t currentBFOStep = 25;
uint8_t seekDirection = 1;
// uint8_t currentBandwidthIdx;

// Datatype to deal with bandwidth on AM and SSB in numerical order.
typedef struct
{
  uint8_t idx;      // SI473X device bandwidth index value
  const char *desc; // bandwidth description
} Bandwidth;

int8_t bwIdxSSB = 4;
const int maxFilterSSB = 5;
Bandwidth bandwidthSSB[] = {{4, "0.5"},  //  4 = 0.5kHz
                              {5, "1.0"},  //
                              {0, "1.2"},  //
                              {1, "2.2"},  //
                              {2, "3.0"},  //
                              {3, "4.0"}}; // 3 = 4kHz

int8_t bwIdxAM = 4;
const int maxFilterAM = 6;
Bandwidth bandwidthAM[] = {{4, "1.0"}, // 4 = 1kHz
                             {5, "1.8"},
                             {3, "2.0"},
                             {6, "2.5"},
                             {2, "3.0"},
                             {1, "4.0"},
                             {0, "6.0"}}; // 0 = 6kHz

int8_t bwIdxFM = 0;
const int maxFilterFM = 4;
Bandwidth bandwidthFM[] = {{0, "AUT"}, // Automatic
                             {1, "110"}, // Force wide (110 kHz) channel filter.
                             {2, " 84"},
                             {3, " 60"},
                             {4, " 40"}};

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

char buffer[255];
char bufferFreq[10];
char bufferStereo[10];
char bufferBFO[15];
char bufferUnit[10];

char bufferMode[15];
char bufferBandName[15];

Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
SI4735 rx;

TFT_eSPI_Button buttonBand,   
                buttonVolumeLevel, 
                buttonSeek,        
                buttonStep, 
                buttonAudioMute, 
                buttonModeOrRDS, 
                buttonFilter, 
                buttonAGC, 
                buttonSoftMute;

uint16_t pixel_x, pixel_y; //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
  bool pressed = tft.getTouch(&pixel_x, &pixel_y);
  return pressed;
}

void showBandwidth(bool drawAfter = false);
void showAgcAtt(bool drawAfter = false);
void showStep(bool drawAfter = false);
void showSoftMute(bool drawAfter = false);
void showBFOorRDS(bool drawAfter = false);
void showVolume(bool drawAfter = false);
void showAudioMute(bool drwaAfter = false);


void setup(void)
{
  // if you are using the Gerts or Thiago project, the two lines below turn on the display 
  pinMode(DISPLAY_LED, OUTPUT);
  digitalWrite(DISPLAY_LED, DISPLAY_ON);

  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);

  rx.setAudioMuteMcuPin(AUDIO_MUTE_CIRCUIT);

  // Initialise the TFT screen
  tft.init();
  
  // tft.setRotation(0); //PORTRAIT
  tft.setRotation(2);

  // Calibration code for touchscreen : for 2.8 inch & Rotation = 2
  uint16_t calData[5] = {258, 3566, 413, 3512, 2};
  tft.setTouch(calData);

  tft.fillScreen(BLACK);

  // tft.setFreeFont(&FreeSans12pt7b);
  showText(60, 0, 3, NULL, GREEN, "SI4735");
  showText(60, 30, 3, NULL, YELLOW, "Arduino");
  showText(60, 60, 3, NULL, YELLOW, "Library");
  tft.setFreeFont(NULL);
  tft.setTextSize(1);
  showText(60, 100, 3, NULL, WHITE, "PU2CLR");
  showText(60, 140, 3, NULL, WHITE, "RICARDO");
  showText(70, 180, 3, NULL, WHITE, "V1.0.3");
  showText(30, 250, 1, NULL, WHITE, "https://pu2clr.github.io/SI4735/");
  
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if (si4735Addr == 0)
  {
    tft.fillScreen(BLACK);
    showText(0, 160, 2, NULL, RED, "Si473X not");
    showText(0, 240, 2, NULL, RED, "detected!!");
    while (1)
      ;
  }
  else
  {
    sprintf(buffer, "The Si473X I2C address is 0x%x ", si4735Addr);
    showText(65, 440, 1, NULL, RED, buffer);
  }
  delay(3000);

  tft.fillScreen(BLACK);

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    showText(0, 160, 2, NULL, RED, "EEPROM ERROR");
    while(1); 
  }

  // If you want to reset the eeprom, keep the VOLUME_UP button pressed during statup
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    EEPROM.write(eeprom_address, 0);
    EEPROM.commit();
    showText(0, 160, 2, NULL, RED, "EEPROM RESETED");
    delay(2000);
    tft.fillScreen(BLACK);
  }

  // Atach Encoder pins interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  // rx.setup(RESET_PIN, 1);
  rx.setup(RESET_PIN, 0, POWER_UP_AM, SI473X_ANALOG_AUDIO, XOSCEN_CRYSTAL);
  rx.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM/SSB mode (between 12 and 90dB)

  // Set up the radio for the current band (see index table variable bandIdx )
  delay(300);

  // Checking the EEPROM content
  if (EEPROM.read(eeprom_address) == app_id)
  {
    readAllReceiverInformation();
  }

  showTemplate();
  
  useBand();
  currentFrequency = previousFrequency = rx.getFrequency();
  tft.setFreeFont(NULL); // default font
  showStatus();
}

/*
   Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt

*/
void rotaryEncoder()
{ // rotary encoder events
  uint8_t encoderStatus = encoder.process();
  if (encoderStatus)
  {
    if (encoderStatus == DIR_CW)
    {
      encoderCount = 1;
    }
    else
    {
      encoderCount = -1;
    }
  }
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
  EEPROM.write(eeprom_address + 4, currentMode);        // Stores the current Mode (FM / AM / SSB)
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
    EEPROM.write(addr_offset++, band[i].bandwidthIdx);        // table index (direct position) of bandwidth
    EEPROM.commit();
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

  EEPROM.end();

  previousFrequency = currentFrequency = band[bandIdx].currentFreq;

  if (band[bandIdx].bandType == FM_BAND_TYPE) {
    currentStepIdx = idxStepFM = band[bandIdx].currentStepIdx;
    rx.setFrequencyStep(tabStepFM[currentStepIdx]);
  }
  else { 
    currentStepIdx = idxStepAM = band[bandIdx].currentStepIdx;
    rx.setFrequencyStep(tabStepAM[currentStepIdx]);
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
  storeTime = displayTime = millis();
  itIsTimeToSave = true;
  // Turns the display on only it is off
  if (!displayOn) {   
    digitalWrite(DISPLAY_LED, DISPLAY_ON);
   }
   displayOn = true;
}



  /*
   Shows a text on a given position; with a given size and font, and with a given color

   @param int x column
   @param int y line
   @param int sz font size
   @param const GFXfont *f font type
   @param uint16_t color
   @param char * msg message
*/
  void showText(int x, int y, int sz, const GFXfont *f, uint16_t color, const char *msg)
  {
    tft.setFreeFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(msg);
  }

  /*
    Prevents blinking during the frequency display.
    Erases the old char/digit value if it has changed and print the new one.
*/
  void printText(int col, int line, int sizeText, char *oldValue, const char *newValue, uint16_t color, uint8_t space)
  {
    int c = col;
    char *pOld;
    char *pNew;

    pOld = oldValue;
    pNew = (char *)newValue;

    tft.setTextSize(sizeText);

    // prints just changed digits
    while (*pOld && *pNew)
    {
      if (*pOld != *pNew)
      {
        tft.setTextColor(BLACK);
        tft.setCursor(c, line);
        tft.print(*pOld);
        tft.setTextColor(color);
        tft.setCursor(c, line);
        tft.print(*pNew);
      }
      pOld++;
      pNew++;
      c += space;
    }

    // Is there anything else to erase?
    tft.setTextColor(BLACK);
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

  void setDrawButtons(bool value)
  {
    buttonBand.drawButton(value);
    buttonVolumeLevel.drawButton(value);
    buttonAudioMute.drawButton(value);
    buttonSeek.drawButton(value);
    buttonStep.drawButton(value);
    buttonModeOrRDS.drawButton(value);
    buttonFilter.drawButton(value);
    buttonAGC.drawButton(value);
    buttonSoftMute.drawButton(value);
  }

  /**
 * Initiates an instance of a given button
 * 
 */
  void setButton(TFT_eSPI_Button * button, int16_t col, int16_t lin, int16_t width, int16_t high, char *label, bool drawAfter)
  {
    tft.setFreeFont(NULL);
    button->initButton(&tft, col, lin, width, high, WHITE, CYAN, BLACK, (char *)label, 1);
    button->drawButton(drawAfter);
  }

  void setButtonsFM()
  {
    showBFOorRDS(true);
    showBandwidth(true);
    showStep(true);
  }

  void setButtonsAM() {
    setButton(&buttonModeOrRDS, 120, KEYBOARD_LIN_OFFSET + 185, 70, 49, (char *)"Mode", true);
    showStep(true);
  }


/**
 * Shows the main screen 
 */
void showTemplate()
{
    int w = tft.width();
    // Area used to show the frequency
    tft.drawRect(0, 0, w, 75, WHITE); // Frequency and status area
    tft.drawRect(0, KEYBOARD_LIN_OFFSET + 100, w, KEYBOARD_LIN_OFFSET + 120, CYAN); // keyboard area
    tft.setFreeFont(NULL);

    setButton(&buttonBand, 45, KEYBOARD_LIN_OFFSET + 130, 70, 49, (char *)"Band", true);
    setButton(&buttonVolumeLevel, 120, KEYBOARD_LIN_OFFSET + 130, 70, 49, (char *)"Vol", true);
    setButton(&buttonAudioMute, 195, KEYBOARD_LIN_OFFSET + 130, 70, 49, (char *)"Mute", true);

    setButton(&buttonSeek, 45, KEYBOARD_LIN_OFFSET + 185, 70, 49, (char *)"Seek", true);
    setButton(&buttonModeOrRDS, 120, KEYBOARD_LIN_OFFSET + 185, 70, 49, (char *)"Mode", true);
    setButton(&buttonStep, 195, KEYBOARD_LIN_OFFSET + 185, 70, 49, (char *)"Step", true);

    setButton(&buttonAGC, 45, KEYBOARD_LIN_OFFSET + 240, 70, 49, (char *)"AGC On", true);
    setButton(&buttonSoftMute, 120, KEYBOARD_LIN_OFFSET + 240, 70, 49, (char *)"SoftM", true);
    setButton(&buttonFilter, 195, KEYBOARD_LIN_OFFSET + 240, 70, 49, (char *)"BW", true);

    // Exibe os botões (teclado touch)
    setDrawButtons(true);
    tft.setFreeFont(NULL);
}

  /*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
  void showFrequencyValue(int col, int line, char *oldValue, char *newValue, uint16_t color, uint8_t space, uint8_t textSize)
  {
    int c = col;
    char *pOld;
    char *pNew;

    pOld = oldValue;
    pNew = newValue;
    // prints just changed digits
    while (*pOld && *pNew)
    {
      if (*pOld != *pNew)
      {
        tft.drawChar(c, line, *pOld, BLACK, BLACK, textSize);
        tft.drawChar(c, line, *pNew, color, BLACK, textSize);
      }
      pOld++;
      pNew++;
      c += space;
    }
    // Is there anything else to erase?
    while (*pOld)
    {
      tft.drawChar(c, line, *pOld, BLACK, BLACK, textSize);
      pOld++;
      c += space;
    }
    // Is there anything else to print?
    while (*pNew)
    {
      tft.drawChar(c, line, *pNew, color, BLACK, textSize);
      pNew++;
      c += space;
    }
    strcpy(oldValue, newValue);
  }

  /**
 * Shows the current frequency
 * 
 */
  void showFrequency()
  {
    uint16_t color;
    uint8_t space; 
    uint8_t lin, col;

    char sFreq[15];

    tft.setTextSize(1);
    if (rx.isCurrentTuneFM())
    {
      sprintf(sFreq, "%5d", currentFrequency);
    }
    else
    {
      sprintf(sFreq, "%5d", currentFrequency);
      tft.drawChar(180, 55, '.', BLACK, BLACK, 1);
    }

    color = YELLOW;   
    if (ssbLoaded) { 
        tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38); 
        space = 30;
        col = 50;
        lin = 64;
        if ( cmdBFO ) 
           color = CYAN;  
    }
    else {
       tft.setFreeFont(&DSEG7_Classic_Mini_Regular_48);
       space = 38;
       col = 40;
       lin = 58;
    }

    showFrequencyValue(col, lin, bufferFreq, sFreq, color, space, 1);

    if (currentMode == LSB || currentMode == USB)
    {
      showBFOorRDS(true);
    }
    tft.setFreeFont(NULL); // default font
  }

  /**
 * Shows the frequency during seek process
 * 
 */
  void showFrequencySeek(uint16_t freq)
  {
    currentFrequency = freq;
    showFrequency();
  }

  /**
 * Checks the stop seeking criterias.  
 * Returns true if the user press the touch or rotates the encoder. 
 */
  bool checkStopSeeking()
  {
    // Checks the touch and encoder
    return (bool)encoderCount || Touch_getXY(); // returns true if the user rotates the encoder or touches on screen
  }

  /**
 * Clears status area
 * 
 */
  void clearStatusArea()
  {
    tft.fillRect(STATUS_DISPLAY_COL_OFFSET, STATUS_DISPLAY_LIN_OFFSET, tft.width() - 8, 36, BLACK); // Clear all status area
  }

  /**
 * Clears frequency area
 * 
 */
  void clearFrequencyArea()
  {
    tft.fillRect(2, 2, tft.width() - 4, 70, BLACK);
  }

  int getStepIndex(int st)
  {
    for (int i = 0; i < lastStepAM; i++)
    {
      if (st == tabStepAM[i])
        return i;
    }
    return 0;
  }

  /**
 * Clears buffer control variable
 * 
 */
  void clearBuffer()
  {
    bufferMode[0] = '\0';
    bufferBandName[0] = '\0';
    bufferMode[0] = '\0';
    bufferBFO[0] = '\0';
    bufferFreq[0] = '\0';
    bufferUnit[0] = '\0';
  }

  /**
 * Soows status
 * 
 */
  void showStatus()
  {
    clearBuffer();
    clearFrequencyArea();
    clearStatusArea();

    DrawSmeter();

    rx.getStatus();
    rx.getCurrentReceivedSignalQuality();

    rx.getFrequency();
    showFrequency();

    showVolume(true);
    tft.setFreeFont(NULL); // default font
    printText(5, 5, 2, bufferBandName, band[bandIdx].bandName, CYAN, 11);

    if (band[bandIdx].bandType == SW_BAND_TYPE)
    {
      printText(5, 30, 2, bufferMode, bandModeDesc[currentMode], RED, 11);
    }
    else
    {
      printText(5, 30, 2, bufferMode, "    ", BLACK, 11);
    }

    if (rx.isCurrentTuneFM())
    {
      printText(5, 55, 2, bufferUnit, "MHz", WHITE, 12);
      setButtonsFM();
      return;
    }

    printText(5, 55, 2, bufferUnit, "kHz", WHITE, 12);

    setButtonsAM();
    showBandwidth(true);
    showAgcAtt(true);
    showStep(true);
    showSoftMute(true);
    showAudioMute(true);
    
    // setDrawButtons(true);
  }

  /**
 * SHow bandwidth on AM or SSB mode
 * 
 */
  void showBandwidth(bool drawAfter)
  {
    char bw[20];

    if (currentMode == LSB || currentMode == USB)
    {
      sprintf(bw, "BW:%s", bandwidthSSB[bwIdxSSB].desc);
      showBFOorRDS(true);
    }
    else if (currentMode == AM)
    {
      sprintf(bw, "BW:%s", bandwidthAM[bwIdxAM].desc);
    }
    else
    {
      sprintf(bw, "BW:%s", bandwidthFM[bwIdxFM].desc);
    }
    setButton(&buttonFilter, 195, KEYBOARD_LIN_OFFSET + 240, 70, 49, bw, drawAfter);
  }

  /**
 * Shows AGC and Attenuation status
 * 
 */
  void showAgcAtt(bool drawAfter)
  {
    char sAgc[15];

    // rx.getAutomaticGainControl();
    if (agcNdx == 0 && agcIdx == 0)
      strcpy(sAgc, "AGC ON");
    else
    {
      sprintf(sAgc, "ATT: %2d", agcNdx);
    }
    setButton(&buttonAGC, 45, KEYBOARD_LIN_OFFSET + 240, 70, 49, sAgc, drawAfter);
  }

  /**
 * Draws the Smeter 
 * This function is based on Mr. Gert Baak's sketch available on his github repository 
 * See: https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino 
 */
  void DrawSmeter()
  {
    tft.setTextSize(1);
    tft.setTextColor(WHITE, BLACK);
    for (int i = 0; i < 10; i++)
    {
      tft.fillRect(RSSI_DISPLAY_COL_OFFSET + 13 + (i * 12), RSSI_DISPLAY_LIN_OFFSET + 24, 4, 8, YELLOW);
      tft.setCursor((RSSI_DISPLAY_COL_OFFSET + 12 + (i * 12)), RSSI_DISPLAY_LIN_OFFSET + 13);
      tft.print(i);
    }
    for (int i = 1; i < 7; i++)
    {
      tft.fillRect((RSSI_DISPLAY_COL_OFFSET + 121 + (i * 16)), RSSI_DISPLAY_LIN_OFFSET + 24, 4, 8, RED);
      tft.setCursor((RSSI_DISPLAY_COL_OFFSET + 115 + (i * 16)), RSSI_DISPLAY_LIN_OFFSET + 13);
      if ((i == 2) or (i == 4) or (i == 6))
      {
        tft.print("+");
        tft.print(i * 10);
      }
    }
    tft.fillRect(RSSI_DISPLAY_COL_OFFSET + 13, RSSI_DISPLAY_LIN_OFFSET + 32, 112, 4, YELLOW);
    tft.fillRect(RSSI_DISPLAY_COL_OFFSET + 125, RSSI_DISPLAY_LIN_OFFSET + 32, 100, 4, RED);
  }

  /**
 * Shows RSSI level
 * This function shows the RSSI level based on the function DrawSmeter above. 
 */
  void showRSSI()
  {
    int spoint;
    if (currentMode != FM)
    {
      //dBuV to S point conversion HF
      if (rssi <= 1)
        spoint = 12; // S0
      if (rssi <= 2)
        spoint = 24; // S1
      if (rssi <= 3)
        spoint = 36; // S2
      if (rssi <= 4)
        spoint = 48; // S3
      if ((rssi > 4) and (rssi <= 10))
        spoint = 48 + (rssi - 4) * 2; // S4
      if ((rssi > 10) and (rssi <= 16))
        spoint = 60 + (rssi - 10) * 2; // S5
      if ((rssi > 16) and (rssi <= 22))
        spoint = 72 + (rssi - 16) * 2; // S6
      if ((rssi > 22) and (rssi <= 28))
        spoint = 84 + (rssi - 22) * 2; // S7
      if ((rssi > 28) and (rssi <= 34))
        spoint = 96 + (rssi - 28) * 2; // S8
      if ((rssi > 34) and (rssi <= 44))
        spoint = 108 + (rssi - 34) * 2; // S9
      if ((rssi > 44) and (rssi <= 54))
        spoint = 124 + (rssi - 44) * 2; // S9 +10
      if ((rssi > 54) and (rssi <= 64))
        spoint = 140 + (rssi - 54) * 2; // S9 +20
      if ((rssi > 64) and (rssi <= 74))
        spoint = 156 + (rssi - 64) * 2; // S9 +30
      if ((rssi > 74) and (rssi <= 84))
        spoint = 172 + (rssi - 74) * 2; // S9 +40
      if ((rssi > 84) and (rssi <= 94))
        spoint = 188 + (rssi - 84) * 2; // S9 +50
      if (rssi > 94)
        spoint = 204; // S9 +60
      if (rssi > 95)
        spoint = 208; //>S9 +60
    }
    else
    {
      //dBuV to S point conversion FM
      if (rssi < 1)
        spoint = 36;
      if ((rssi > 1) and (rssi <= 2))
        spoint = 60; // S6
      if ((rssi > 2) and (rssi <= 8))
        spoint = 84 + (rssi - 2) * 2; // S7
      if ((rssi > 8) and (rssi <= 14))
        spoint = 96 + (rssi - 8) * 2; // S8
      if ((rssi > 14) and (rssi <= 24))
        spoint = 108 + (rssi - 14) * 2; // S9
      if ((rssi > 24) and (rssi <= 34))
        spoint = 124 + (rssi - 24) * 2; // S9 +10
      if ((rssi > 34) and (rssi <= 44))
        spoint = 140 + (rssi - 34) * 2; // S9 +20
      if ((rssi > 44) and (rssi <= 54))
        spoint = 156 + (rssi - 44) * 2; // S9 +30
      if ((rssi > 54) and (rssi <= 64))
        spoint = 172 + (rssi - 54) * 2; // S9 +40
      if ((rssi > 64) and (rssi <= 74))
        spoint = 188 + (rssi - 64) * 2; // S9 +50
      if (rssi > 74)
        spoint = 204; // S9 +60
      if (rssi > 76)
        spoint = 208; //>S9 +60
    }
    tft.fillRect(RSSI_DISPLAY_COL_OFFSET + 13, RSSI_DISPLAY_LIN_OFFSET + 38, (2 + spoint), 6, RED);
    tft.fillRect(RSSI_DISPLAY_COL_OFFSET + 15 + spoint, RSSI_DISPLAY_LIN_OFFSET + 38, 212 - (2 + spoint), 6, GREEN);
  }

  void showStep(bool drawAfter)
  {
    char sStep[15];
    if ( currentMode == FM ) { 
      sprintf(sStep, "Stp:%4d", tabStepFM[currentStepIdx] * 10); 
    } else {
      sprintf(sStep, "Stp:%4d", tabStepAM[currentStepIdx]); 
    }
    setButton(&buttonStep, 195, KEYBOARD_LIN_OFFSET + 185, 70, 49, sStep, drawAfter);
  }

  void showSoftMute(bool drawAfter)
  {
    char sMute[15];

    sprintf(sMute, "SoftM: %2d", softMuteMaxAttIdx);
    setButton(&buttonSoftMute, 120, KEYBOARD_LIN_OFFSET + 240, 70, 49, sMute, drawAfter);
  }

 /**
  * Shows the BFO or FM RDS status 
  */
  void showBFOorRDS(bool drawAfter)
  {
    if ( currentMode == FM ) {
      sprintf(buffer, "%s", (fmRDS) ? "RDS On" : "RDS Off");
    } else {
      if ( ssbLoaded ) {
        tft.setFreeFont(NULL); // default font
        sprintf(buffer, "%c%d", (int) (currentBFO >= 0) ? '+' : '-', (int) abs(currentBFO));
        printText(175, 5, 2, bufferBFO, buffer, YELLOW, 11);
        sprintf(buffer,"%s", bandModeDesc[currentMode]);
      } else {
        sprintf(buffer,"%s", "Mode");  
      }
    }
    setButton(&buttonModeOrRDS, 120, KEYBOARD_LIN_OFFSET + 185, 70, 49, buffer, drawAfter);
  }

  /**
 * Shows the current volume level
 */
  void showVolume(bool drawAfter)
  {
    char sVolume[15];
    sprintf(sVolume, "Vol: %2.2d", rx.getVolume());
    setButton(&buttonVolumeLevel, 120, KEYBOARD_LIN_OFFSET + 130, 70, 49, sVolume, drawAfter);
  }


void showAudioMute(bool drwaAfter) {
     setButton(&buttonAudioMute, 195, KEYBOARD_LIN_OFFSET + 130, 70, 49, (char *)"Mute", drwaAfter); 
}

/***************  
 *   RDS
 *   
 */
 
char *stationName;
char bufferStatioName[20];

void clearRDS() {
   stationName = (char *) "           ";
   showRDSStation();
}

void showRDSStation()
{
    printText(STATUS_DISPLAY_COL_OFFSET + 5, STATUS_DISPLAY_LIN_OFFSET, 2, bufferStatioName, stationName, GREEN, 14);
    delay(100);
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
    }
  }
}

/*
   Goes to the next band (see Band table)
*/
  void bandUp()
  {
    // save the current frequency for the band
    band[bandIdx].currentFreq = currentFrequency;
    // band[bandIdx].currentStepIdx = currentStepIdx;
    // band[bandIdx].bandwidthIdx = currentBandwidthIdx

    if (bandIdx < lastBand)
    {
      bandIdx++;
    }
    else
    {
      bandIdx = 0;
    }

    useBand();
  }

  /*
   Goes to the previous band (see Band table)
*/
  void bandDown()
  {
    // save the current frequency for the band
    band[bandIdx].currentFreq = currentFrequency;
    // band[bandIdx].currentStepIdx = currentStepIdx;
    // band[bandIdx].bandwidthIdx = currentBandwidthIdx
    
    if (bandIdx > 0)
    {
      bandIdx--;
    }
    else
    {
      bandIdx = lastBand;
    }
    useBand();
  }

  /*
   This function loads the contents of the ssb_patch_content array into the CI (Si4735) and starts the radio on
   SSB mode.
*/
  void loadSSB()
  {
    // rx.reset();
    rx.queryLibraryId(); // It also calls power down. So it is necessary.
    rx.patchPowerUp();
    delay(50);
    // rx.setI2CFastMode(); // Recommended
    rx.setI2CFastModeCustom(500000); // It is a test and may crash.
    rx.downloadPatch(ssb_patch_content, size_content);
    rx.setI2CStandardMode(); // goes back to default (100kHz)

    // delay(50);
    // Parameters
    // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
    // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
    // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
    // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
    // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
    // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
    rx.setSSBConfig(bandwidthSSB[bwIdxSSB].idx, 1, 0, 0, 0, 1);
    rx.setFifoCount(1);
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
      idxStepFM = currentStepIdx = band[bandIdx].currentStepIdx;
      rx.setTuneFrequencyAntennaCapacitor(0);
      rx.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabStepFM[currentStepIdx]);
      // rx.setFMDeEmphasis(1); // 1 = 50 μs. Used in Europe, Australia, Japan;
      rx.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
      // Define here the best criteria to find a FM station during the seeking process
      rx.setSeekFmSpacing(1); // Using space 1 (not documentend). The manual says: 5, 10 or 20. They mean 50, 100 or 200 kHz.
      // rx.setSeekAmRssiThreshold(0);
      // rx.setSeekFmSNRThreshold(3);
      cmdBFO = ssbLoaded = false;
      rx.setRdsConfig(1, 2, 2, 2, 2);
      rx.setFifoCount(1);
      bwIdxFM = band[bandIdx].bandwidthIdx;
      rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);
    }
    else
    {
      if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      {
        antennaIdx = 0;
        rx.setTuneFrequencyAntennaCapacitor(antennaIdx);
      }
      else
      {
        antennaIdx = 1;
        rx.setTuneFrequencyAntennaCapacitor(antennaIdx);
      }

      if (ssbLoaded)
      {
        rx.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabStepAM[band[bandIdx].currentStepIdx], currentMode);
        rx.setSSBAutomaticVolumeControl(1);
        rx.setSsbSoftMuteMaxAttenuation(softMuteMaxAttIdx); // Disable Soft Mute for SSB
        bwIdxSSB = band[bandIdx].bandwidthIdx;
        rx.setSSBAudioBandwidth(bandwidthSSB[bwIdxSSB].idx);
        // showBFOorRDS();
      }
      else
      {
        currentMode = AM;
        rx.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, tabStepAM[band[bandIdx].currentStepIdx]);
        rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx); // // Disable Soft Mute for AM
        cmdBFO = false;
        bwIdxAM = band[bandIdx].bandwidthIdx;
        rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
      }
      idxStepAM = currentStepIdx = band[bandIdx].currentStepIdx;
      rx.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);               // Consider the range all defined current band
      rx.setSeekAmSpacing(5); 
    }
    delay(100);

    // Sets AGC or attenuation control
    rx.setAutomaticGainControl(disableAgc, agcNdx);
    // rx.setAMFrontEndAgcControl(10,12); // Try to improve sensitivity

    currentFrequency = band[bandIdx].currentFreq;

    showStatus();
    resetEepromDelay();
  }

  void switchAgc(int8_t v)
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

    // Sets AGC on/off and gain
    rx.setAutomaticGainControl(disableAgc, agcNdx);
    showAgcAtt();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  void switchFilter(uint8_t v)
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

      band[bandIdx].bandwidthIdx = bwIdxSSB;
    }
    else if (currentMode == AM)
    {
      bwIdxAM = (v == 1) ? bwIdxAM + 1 : bwIdxAM - 1;

      if (bwIdxAM > maxFilterAM)
        bwIdxAM = 0;
      else if (bwIdxAM < 0)
        bwIdxAM = maxFilterAM;

      rx.setBandwidth(bandwidthAM[bwIdxAM].idx, 1);
      band[bandIdx].bandwidthIdx = bwIdxAM;
      
    } else {
    bwIdxFM = (v == 1) ? bwIdxFM + 1 : bwIdxFM - 1;
    if (bwIdxFM > 4)
      bwIdxFM = 0;
    else if (bwIdxFM < 0)
      bwIdxFM = 4;

    rx.setFmBandwidth(bandwidthFM[bwIdxFM].idx);
    band[bandIdx].bandwidthIdx = bwIdxFM;
  }
  showBandwidth();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}

void switchSync(int8_t v) {
  if (currentMode != FM)
  {
    currentBFO = 0;
    if (!ssbLoaded)
    {
      loadSSB();
    }
    currentMode = (currentMode == LSB)? USB:LSB;
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStepIdx = currentStepIdx;
    useBand();
    rx.setSSBDspAfc(0);
    rx.setSSBAvcDivider(3);
  }
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}


void switchStep(int8_t v)
{

  // This command should work only for SSB mode
  if (cmdBFO && (currentMode == LSB || currentMode == USB))
  {
    currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
    showBFOorRDS();
  }
  else
  {
    if ( currentMode == FM ) {
      idxStepFM = (v == 1) ? idxStepFM + 1 : idxStepFM - 1;
      if (idxStepFM > lastStepFM)
        idxStepFM = 0;
      else if (idxStepFM < 0)
        idxStepFM = lastStepFM;
        
      currentStepIdx = idxStepFM;
      rx.setFrequencyStep(tabStepFM[currentStepIdx]);
      
    } else {
      idxStepAM = (v == 1) ? idxStepAM + 1 : idxStepAM - 1;
      if (idxStepAM > lastStepAM)
        idxStepAM = 0;
      else if (idxStepAM < 0)
        idxStepAM = lastStepAM;

      currentStepIdx = idxStepAM;
      rx.setFrequencyStep(tabStepAM[currentStepIdx]);
      rx.setSeekAmSpacing(5); // Max 10kHz for spacing
    }
    band[bandIdx].currentStepIdx = currentStepIdx;
    showStep();
  }
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}

void switchSoftMute(int8_t v)
{
  softMuteMaxAttIdx = (v == 1) ? softMuteMaxAttIdx + 1 : softMuteMaxAttIdx - 1;
  if (softMuteMaxAttIdx > 32)
    softMuteMaxAttIdx = 0;
  else if (softMuteMaxAttIdx < 0)
    softMuteMaxAttIdx = 32;

  if ( currentMode == FM ) 
    rx.setFmSoftMuteMaxAttenuation(softMuteMaxAttIdx);
  else 
    rx.setAmSoftMuteMaxAttenuation(softMuteMaxAttIdx);
    
  showSoftMute();
}

/**
 * @brief Proccess the volume level
 * 
 * @param v 1 = Up; !1 = down
 */
void doVolume(int8_t v)
{
  if (v == 1)
    rx.volumeUp();
  else
    rx.volumeDown();
  showVolume();
}

void doBFO()
{
  bufferFreq[0] = '\0';
  cmdBFO = !cmdBFO;
  if (cmdBFO)
  {
    showBFOorRDS();
  }
  showStatus();
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
        ssbLoaded = false;
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
        ssbLoaded = false;
      }
    } 
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStepIdx = currentStepIdx;
    useBand();
    cmdModeOrRDS = false;
  } 
}

/**
 * @brief Checks the touch
 * 
 */
void checkTouch()
{
  bool down = Touch_getXY();
  buttonBand.press(down && buttonBand.contains(pixel_x, pixel_y));
  buttonVolumeLevel.press(down && buttonVolumeLevel.contains(pixel_x, pixel_y));
  buttonSeek.press(down && buttonSeek.contains(pixel_x, pixel_y));
  buttonStep.press(down && buttonStep.contains(pixel_x, pixel_y));
  buttonAudioMute.press(down && buttonAudioMute.contains(pixel_x, pixel_y));
  buttonModeOrRDS.press(down && buttonModeOrRDS.contains(pixel_x, pixel_y));
  buttonFilter.press(down && buttonFilter.contains(pixel_x, pixel_y));
  buttonAGC.press(down && buttonAGC.contains(pixel_x, pixel_y));
  buttonSoftMute.press(down && buttonSoftMute.contains(pixel_x, pixel_y));
  if ( down )
    resetEepromDelay();
}

/**
 * disble command buttons and keep the current status of the last command button pressed
 */
void disableCommand(bool *b, bool value, void (*showFunction)( bool act))
{
  cmdBFO = false;
  cmdAudioMute = false;
  cmdMuteRate = false;
  cmdVolume = false; // if true, the encoder will control the volume.
  cmdAgcAtt = false;
  cmdFilter = false;
  cmdStep = false;
  cmdBand = false;
  cmdModeOrRDS = false;
  cmdSoftMuteMaxAtt = false;

  
  
  if (b != NULL)  { // rescues the last status of the last command only the parameter is not null
     setDrawButtons(true);
    *b = value;
  }
    
  if (showFunction != NULL) //  show the desired status only if it is necessary.
    showFunction(!value);

  delay(MIN_ELAPSED_TIME);
}

/* two buttons are quite simple
*/
void loop(void)
{

  if (encoderCount != 0) // Check and process the encoder
  {
    if (cmdBFO)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      rx.setSSBBfo(currentBFO);
      showBFOorRDS(true);
    }
    else if (cmdVolume)
      doVolume(encoderCount);
    else if (cmdAgcAtt)
      switchAgc(encoderCount);
    else if (cmdFilter)
      switchFilter(encoderCount);
    else if (cmdStep)
      switchStep(encoderCount);
    else if (cmdSoftMuteMaxAtt)
      switchSoftMute(encoderCount); 
    else if (cmdModeOrRDS) 
      doMode(encoderCount);       
    else if (cmdBand)
    {
      if (encoderCount == 1)
        bandUp();
      else
        bandDown();
    }
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
      currentFrequency = rx.getFrequency();      // Queries the Si473X device.
      showFrequency();
    }
    encoderCount = 0;
    resetEepromDelay();
  }
  else // Check and process touch
  {
    checkTouch();

    if (buttonBand.justPressed()) {
      cmdBand = !cmdBand;
      disableCommand(&cmdBand, cmdBand, NULL); 
      buttonBand.drawButton(!cmdBand);     
    }
      else if (buttonVolumeLevel.justPressed()) // Volume
    {
      cmdVolume = ! cmdVolume;
      disableCommand(&cmdVolume, cmdVolume, showVolume);
      rx.setAudioMute(cmdAudioMute);
    }
    else if (buttonAudioMute.justPressed()) // Mute
    {
      cmdAudioMute = !cmdAudioMute;
      disableCommand(&cmdAudioMute, cmdAudioMute, showAudioMute);
      rx.setAudioMute(cmdAudioMute);
    }
    else if (buttonSeek.justPressed()) // SEEK DOWN
    {
      if (currentMode == FM || currentMode == AM ) {
        // Jumps up or down one space
        if (seekDirection)
          rx.frequencyUp();
        else
          rx.frequencyDown();

        rx.seekStationProgress(showFrequencySeek, checkStopSeeking, seekDirection);
        delay(30);
        if (currentMode == FM)
        {
          float f = round(rx.getFrequency() / 10.0);
          currentFrequency = (uint16_t)f * 10; // adjusts band space from 1 (10kHz) to 10 (100 kHz)
          rx.setFrequency(currentFrequency);
        }
        else
        {
          currentFrequency = rx.getFrequency(); //
        }
        showFrequency();
        resetEepromDelay();
      }
    }
    else if (buttonSoftMute.justPressed()) // Soft Mute
    {
      cmdSoftMuteMaxAtt = !cmdSoftMuteMaxAtt;
      disableCommand(&cmdSoftMuteMaxAtt, cmdSoftMuteMaxAtt, showSoftMute);
    }
    else if (buttonModeOrRDS.justPressed()) // Switch to AM mode
    {
      if (currentMode == FM ) {
        fmRDS = !fmRDS;
        resetEepromDelay();
        clearRDS();
      }
      else
        cmdModeOrRDS = !cmdModeOrRDS;

      disableCommand(&cmdModeOrRDS, cmdModeOrRDS, showBFOorRDS);
      buttonModeOrRDS.drawButton(!cmdModeOrRDS);
    }
    else if (buttonAGC.justPressed()) // AGC and Attenuation control
    {
      cmdAgcAtt = !cmdAgcAtt;
      disableCommand(&cmdAgcAtt, cmdAgcAtt, showAgcAtt);
    }
    else if (buttonFilter.justPressed()) // FILTER
    {
      cmdFilter = !cmdFilter;
      disableCommand(&cmdFilter, cmdFilter, showBandwidth);
    }
    else if (buttonStep.justPressed()) // STEP
    {
      cmdStep = !cmdStep;
      disableCommand(&cmdStep, cmdStep, showStep);
    }
  }

  // ENCODER PUSH BUTTON
  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    if (currentMode == LSB || currentMode == USB)
    {
      doBFO();
    }
    else
    {
      cmdBand = !cmdBand;
      disableCommand(&cmdBand, cmdBand, NULL);
      buttonBand.drawButton(!cmdBand);
    }
    delay(300);
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 12)
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

  if (currentMode == FM && fmRDS )
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

  if (itIsTimeToSave)
  {
    if ((millis() - storeTime) > STORE_TIME)
    {
      saveAllReceiverInformation();
      storeTime = millis();
      itIsTimeToSave = false;
    }
  }

  // Turns the display off if it is On and the time greater than ELAPSED_DISPLAY
  if ( displayOn && (millis() -  displayTime) > ELAPSED_DISPLAY ) {
    displayTime = millis();
    displayOn =  false;
    digitalWrite(DISPLAY_LED, DISPLAY_OFF);
  }

  delay(3);
}
