//  Version for ESP32 and ili9341 displays -Full RDS services - new S-meter- by Antonino RUSSO IU4ALH. 13.01.2023
//  V4.1    Jan-22-2022 Battery Indicator.
//  V4.0    Jan-16-2022 New version based on Gert Baak's V3.4 sketch.

//  * New user interface
//  * "Press and hold" performing different actions
//  * Battery icon
//  * Bug fixes

//  V3      Jun-06-2021 Initial Release.

//  * First Release. Alpha version.

//  ToDo:
//        * Store current frequency with Long Press in the Preset Menu.
//        * Other function with long press and long long press
//        * Implement the "FREQ" Menu (insane at this screen size, but possible, I believe)

//  This sketch was made by [Ralph Xavier] (https://github.com/ralphxavier/SI4735)

//  The sketch and documentation available here are based on Gert's sketch.
//  I would like to thank to [Gert Baak](https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino) 
//  for his excelent job. You are great!

//  This sketch also uses the [PU2CLR SI4735 Library for Arduino](https://github.com/pu2clr/SI4735). Thanks for the very nice work.

//  This sketch uses a Rotary Encoder and [TTGO T-Display](http://www.lilygo.cn/prod_view.aspx?TypeId=50044&Id=1126).
//  The radio is fully controlled by the Rotary Encoder.

//  IMPORTANT:

//      This release implements different behaviors for different "press and hold" durations on its single button.

//      There are 3 different button "press and hold" durations: Short Press Duration (SP), Long Press Duration (LP) 
//      and Long Long Press Duration (LLP)

//          - SP:  Short Press - less than 500ms.
//          - LP:  Long Press - between 500ms and 1500ms.
//          - LLP: Long Long Press - 1500ms or more.
      
//      At the main screen (Frequency Display), the duration of the button press and hold performs 3 different actions:
//          - SP:  Volume controle
//          - LP:  Opens First Layer Menu
//          - LLP: Do a reset.

//      At the others screen/Menu, the duration of the button press performs only one action:
//          - SP:  Select
//          - LP:  Do Nothing
//          - LLP: Do Nothing.


//  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
//  together with this sketch).
//  Also a schematic drawing is available.

//  ABOUT SSB PATCH:
//  First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr.
//  Vadim Afonkin on his Dropbox repository. It is important to note that the author of the SI473x library does not encourage anyone to use the SSB patches
//  content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library.
//  This sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 8KB of the Arduino memory.
//  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
//  There is little information available about patching the SI4735. The following information is the understanding of the author of
//  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
//  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
//  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by
//  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
//  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

//  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
//  Given this, it is at your own risk to continue with the procedures suggested here.
//  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
//  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices, like an ATS-100 that you are using.
//  There is NO GUARANTEE that this sketch will work with the Chinese ATS-100 KIT sold on AliExpress, eBay etc.
//  The author of this sketch and Arduino Library does not know the seller of this kit and does not have a commercial relationship with any commercial product that uses the Arduino Library.
//  It is important you understand that there is no guarantee that this sketch will work correctly in your current product.
//  SO, DO NOT TRY IT IF YOU DON'T KNOW WHAT ARE YOU DOING. YOU MUST BE ABLE TO GO BACK TO THE PREVIOUS VERSION IF THIS SKETCH DOES NOT WORK FOR YOU.

//  Library TFT_eSPI you may download from here : https://github.com/Bodmer/TFT_eSPI
//  Library Rotary is provided with the program
//  Library SI4735 you may download from here   : https://github.com/pu2clr/SI4735
//
//  *********************************
//  **   Display connections etc.  **
//  *********************************
//  |-------------|------------|------------|------------|
//  |    TTGO     |   Si4735   |  Encoder   |   Audio    |
//  |  T-Display  |            |            | Amplifier  |        
//  |-------------|------------|------------|------------|        
//  |     3V3     |    Vcc     |            |    Vcc     |        
//  |     GND     |    GND     |     2,4    |    GND     |        Encoder        1,2,3        
//  |      2      |            |     5      |            |        Encoder switch 4,5
//  |     25      |   Reset    |            |            |
//  |     21      |    SDA     |            |            |
//  |     22      |    SCL     |            |            |
//  |     33      |            |      1     |            |
//  |     32      |            |      3     |            |
//  |             |    LOut    |            |    LIn     |
//  |             |    ROut    |            |    RIn     |
//  |     27 Mute |            |            |    Mute    |
//  |-------------|------------|------------|------------|
//
// ==================Display========================
#define IhaveTDisplayTFT
// =================================================
// =================================================

// ==================Oscillator=====================
#define IhaveCrystal
// =================================================

#include <Battery18650Stats.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <SI4735.h>
#include "EEPROM.h"
#include "Rotary.h"

#include "DSEG7_Classic_Mini_Regular_38.h"
#include "TFT_Colors.h"
#include "Button.h"
#include "Battery.h"


//#include "patch_init.h"    // SSB patch for whole SSBRX initialization string
#include "patch_full.h"    // SSB patch for whole SSBRX full download

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define ESP32_I2C_SDA    21  // I2C bus pin on ESP32
#define ESP32_I2C_SCL    22  // I2C bus pin on ESP32
#define RESET_PIN        12
#define ENCODER_PIN_A    17  // http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html
#define ENCODER_PIN_B    16
#define ENCODER_SWITCH    33
#define BEEPER           18  // To be deleted. My scketch do not have beep
#define displayon         1
#define displayoff        0
#define beepOn            1
#define beepOff           0
#define AUDIO_MUTE       27
#define VBAT_MON         34
#define MIN_USB_VOLTAGE  4.9
#define CONV_FACTOR      1.8
#define READS            20

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3


#define MIN_ELAPSED_TIME             100
#define MIN_ELAPSED_RSSI_TIME        150
#define MIN_ELAPSED_AudMut_TIME        0  // Noise surpression SSB in mSec. 0 mSec = off
#define MIN_ELAPSED_DISPL_TIME      1000
#define MIN_ELAPSED_RDS_TIME           5
#define DEFAULT_VOLUME                45   // change it for your favorite start sound volume
#define MIN_ELAPSED_VOLbut_TIME     1000
#define CLK_Xtal                    SI5351wire_CLK0

#define FM          0
#define LSB         1
#define USB         2
#define AM          3
#define CW          4
#define TFT_GREY 0x5AEB

bool bfoOn          = false;
bool ssbLoaded      = false;
bool FirstLayer     = true;
bool FirstTime      = true;
bool SecondLayer    = false;
bool ThirdLayer     = false;
bool ForthLayer     = false;
bool HamBand        = false;
bool Modebut        = false;
bool FREQbut        = false;
bool Decipoint      = false;
bool STEPbut        = false;
bool encsw          = false;
bool BroadBand;
bool BandWidth;
bool MISCbut        = false;
bool PRESbut        = false;
bool VOLbut         = false;
bool AudioMut       = false;
bool DISplay        = false;
bool Mutestat       = false;
bool AGCgainbut     = false;
bool writingEeprom  = false;

bool pressed;
bool presStat     = false;
bool audioMuteOn  = true;
bool audioMuteOff = false;
bool RDS          = true; // RDS on  or  off
bool SEEK         = false;
bool bright       = false;
bool CWShift      = false;
bool fstShift     = false;
bool calibratSI5351 = false;


int tcount = 0; // scroll

int currentBFO;
int currentBFOmanu;
int previousBFO     = 0;
int previousBFOmanu = 0;
int nrbox           = 0;
int OldRSSI;
int NewRSSI;
int NewSNR;
int encBut;
int AGCgain         = 0;
int PrevRSSI        = 0;
int strongup        = 0;

long elapsedRSSI        = millis();
long elapsedAudMut      = millis();
long elapsedRDS         = millis();
long stationNameElapsed = millis();
long DisplayOnTime      = millis();
long VOLbutOnTime       = millis();

volatile int encoderCount  = 0;
volatile int encoderButton = 0;

int previousBatteryLevel = -1;
int currentBatteryLevel = 1;
bool batteryCharging = false;

uint16_t previousFrequency;
uint8_t currentBFOStep     = 10;
int currentPRES            =  0;
int previousPRES           =  0;
uint8_t currentPRESStep    =  1;

uint8_t currentAGCgain     =  1;
uint8_t previousAGCgain    =  1;
uint8_t currentAGCgainStep =  1;
uint8_t MaxAGCgain;
uint8_t MaxAGCgainFM       = 26;
uint8_t MaxAGCgainAM       = 37;
uint8_t MinAGCgain         =  1;


uint8_t currentVOL         =  0;
uint8_t previousVOL        =  0;
uint8_t currentVOLStep     =  1;
uint8_t MaxVOL             = 63;
uint8_t MinVOL             = 20;

uint8_t currentAGCAtt      =  0;
uint8_t bwIdxSSB;
uint8_t bwIdxAM;
uint8_t bwIdxFM;
uint8_t ssIdxMW;
uint8_t ssIdxAM;
uint8_t ssIdxFM;
uint8_t bandIdx;
uint8_t currentMode        = FM;
uint8_t previousMode       =  0;
uint16_t x = 0, y = 0; // To store the touch coordinates
uint8_t encoderStatus;
uint16_t freqstep;
uint8_t freqstepnr         = 0; //1kHz
int freqDec                = 0;

const int LedFreq          = 5000;
const int LedResol         = 8;
const int LedChannelforTFT = 0;
uint16_t currentBrightness;
uint16_t previousBrightness;
uint16_t MaxBrightness     = 20;
uint16_t MinBrightness     = 250;
uint8_t stepsizesynth      = 10;

float Displayfreq          = 0;
float currentFrequency     = 0;
float dpfrq                = 0;
float fact                 = 1;
float RSSIfact             = 3;

String BWtext;
String Modtext;
String RDSbuttext;
String AGCgainbuttext;

//===============================================================================
const char *bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};
int selectedBWSSB = 0;
const char *bandwidthAM[]  = {"6.0", "4.0", "3.0", "2.0", "1.0", "1.8", "2.5"};
int selectedBWAM = 0;
const char *bandwidthFM[]  = {"AUT","110","84","60","40"};
int selectedBWFM = 0;
const char *stepsize[]     = {"1","5","9","10"};
int selectedStep = 0;
const char *stepsizeFM[]   = {"100","10"};
int selectedStepFM = 0;
 
const char *Keypathtext[]  = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "Send", "Clear"};

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM ", "CW"};
int selectedMode = 0;

char buffer[64]; // Useful to handle string
char buffer1[64];

char *stationName;
char bufferStatioName[40];

char *rdsMsg;
char bufferRdsMsg[100];   //  100

char *rdsTime;
char bufferRdsTime[32];  //  32

unsigned long FreqSI5351 = 3276800;
unsigned long calibratvalSI5351;
//=======================================================   Buttons First and Third Layer   ==========================
typedef struct // Buttons first layer
{
  const char *ButtonNam;
  uint8_t     ButtonNum;       // Button location at display from 0 to 11. To move around buttons freely at first layer.
  const char *ButtonNam1;
  uint8_t     ButtonNum1;      // Button location at display from 0 to 11. To move around buttons freely at third layer.
  uint16_t    XButos;          // Xoffset
  long        YButos;          // Yoffset
} Button;
int ytotoffset = 0;
int borderThickness = 0;       // 0 - No border

//  Button table
int Xbutst  =   37;               // X Start location Buttons
int Ybutst  =  90 + 105;  // Y Start location Buttons

int Xsmtr   =   -20;
int Ysmtr   =  83 + ytotoffset;  // S meter 85

int XVolInd =   0;
//int YVolInd = 135 + ytotoffset;  // Volume indicator
int YVolInd = 125 + ytotoffset;  // Volume indicator

int XFreqDispl  =   0;
int YFreqDispl  =   0 + ytotoffset;  // display

int Xbutsiz =  80;  //size of buttons first & third layer
int Ybutsiz =  46;

int Xbatsiz = 15;  // size of battery icon
int Ybatsiz = 29;

#ifdef IhaveTDisplayTFT
uint16_t Xbut0  =  0 * Xbutsiz ; uint16_t Ybut0  =  0 * Ybutsiz; // location calqualation for 12 first layer buttons
uint16_t Xbut1  =  1 * Xbutsiz ; uint16_t Ybut1  =  0 * Ybutsiz;
uint16_t Xbut2  =  2 * Xbutsiz ; uint16_t Ybut2  =  0 * Ybutsiz;
uint16_t Xbut3  =  3 * Xbutsiz ; uint16_t Ybut3  =  0 * Ybutsiz;
uint16_t Xbut4  =  4 * Xbutsiz ; uint16_t Ybut4  =  0 * Ybutsiz;
uint16_t Xbut5  =  5 * Xbutsiz ; uint16_t Ybut5  =  0 * Ybutsiz;
uint16_t Xbut6  =  6 * Xbutsiz ; uint16_t Ybut6  =  0 * Ybutsiz;
uint16_t Xbut7  =  7 * Xbutsiz ; uint16_t Ybut7  =  0 * Ybutsiz;
uint16_t Xbut8  =  8 * Xbutsiz ; uint16_t Ybut8  =  0 * Ybutsiz;
uint16_t Xbut9  =  9 * Xbutsiz ; uint16_t Ybut9  =  0 * Ybutsiz;
uint16_t Xbut10 = 10 * Xbutsiz ; uint16_t Ybut10 =  0 * Ybutsiz;
uint16_t Xbut11 = 11 * Xbutsiz ; uint16_t Ybut11 =  0 * Ybutsiz;
uint16_t Xbut12 = 12 * Xbutsiz ; uint16_t Ybut12 =  0 * Ybutsiz;
uint16_t Xbut13 = 13 * Xbutsiz ; uint16_t Ybut13 =  0 * Ybutsiz;
uint16_t Xbut14 = 14 * Xbutsiz ; uint16_t Ybut14 =  0 * Ybutsiz;
uint16_t Xbut15 = 15 * Xbutsiz ; uint16_t Ybut15 =  0 * Ybutsiz;
uint16_t Xbut16 = 16 * Xbutsiz ; uint16_t Ybut16 =  0 * Ybutsiz;
uint16_t Xbut17 = 17 * Xbutsiz ; uint16_t Ybut17 =  0 * Ybutsiz;
#endif

#ifdef IhaveTDisplayTFT                                                 
#define HAM       0
#define BROAD     1
#define FREQ      2
#define BFO       3
#define AGC       4
#define BANDW     5
#define STEP      6
#define VOL       7
#define MUTE      8
#define MODE      9
#define PRESET   10
#define SEEKUP   11
#define SEEKDN   12
#define STATUS   13
#define RDSbut   14
#define AGCset   15
#define Displbut 16
#define ChipType 17
#define NEXT     18  // Delete this definition
#define PREV     19  // Delete this definition


Button bt[] = {                                                 
  { "HAM"   ,  0 , "", 0 , Xbut0  , Ybut0  }, //     ...--|----|----|----|----|----|----|----|----|----|--...
  { "BROAD" ,  1 , "", 1 , Xbut1  , Ybut1  }, //     ...  | 15 | 16 |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  ...
  { "FREQ"  ,  2 , "", 2 , Xbut2  , Ybut2  }, //     ...--|----|----|----|----|----|----|----|----|----|--...
  { "BFO"   ,  3 , "", 3 , Xbut3  , Ybut3  },
  { "AGC"   ,  4 , "", 4 , Xbut4  , Ybut4  }, 
  { "BANDW" ,  5 , "", 5 , Xbut5  , Ybut5  },
  { "STEP"  ,  6 , "", 6 , Xbut6  , Ybut6  },
  { "VOL"   ,  7 , "", 7 , Xbut7  , Ybut7  },
  { "MUTE"  ,  8 , "", 8 , Xbut8  , Ybut8  },
  { "MODE"  ,  9 , "", 9 , Xbut9  , Ybut9  },
  { "PRESET", 10 , "",10 , Xbut10 , Ybut10 },
  { "SEEKUP", 11 , "",11 , Xbut11 , Ybut11 },
  { "SEEKDN", 12 , "",12 , Xbut12 , Ybut12 },
  { "STATUS", 13 , "",13 , Xbut13 , Ybut13 },
  { "RDS"   , 14 , "",14 , Xbut14 , Ybut14 },
  { "AGCset", 15 , "",15 , Xbut15 , Ybut15 },
  { "Displ" , 16 , "",16 , Xbut16 , Ybut16 },
  { "Chip"  , 17 , "",17 , Xbut17 , Ybut17 }
};
int selectedMenu = 0;
#endif


// You may freely move around the button (blue) position on the display to your flavour by changing the position in ButtonNum and ButtonNum1
// You have to stay in the First or Third Layer
//======================================================= End  Buttons First  and Third Layer   ======================

//======================================================= Tunings Steps     ===============================
typedef struct // Tuning steps
{
  uint8_t stepFreq;
  double stepFreqFM;
  uint16_t Xstepos;          //Xoffset
  uint16_t Xstepsr;          //X size rectang
  uint16_t Ystepos;          //Yoffset
  uint16_t Ystepsr;          //Y size rectang
  uint16_t Ystepnr;          //Y next rectang
} Step;

//  Tuning steps table

#ifdef IhaveTDisplayTFT
uint16_t Xfstep =  0;
uint16_t Yfstep = 0; // 88;
#endif


Step sp[] = {
  { 1 , 10   ,Xfstep, 100, Yfstep, 30,  0},
  { 5 ,  1   ,Xfstep, 100, Yfstep, 30, 30},
  { 9 ,  0   ,Xfstep, 100, Yfstep, 30, 60},
  { 10,  0   ,Xfstep, 100, Yfstep, 30, 90}
};
//======================================================= End Tunings Steps     ===============================

//======================================================= Modulation Types     ================================
typedef struct // MODULATION
{
  uint16_t Modenum;
  uint16_t Xmodos;          //Xoffset
  uint16_t Xmodsr;          //X size rectang
  uint16_t Ymodos;          //Yoffset
  uint16_t Ymodsr;          //Y size rectang
  uint16_t Ymodnr;          //Y next rectang
} Mode;

//  Modulation table

#ifdef IhaveTDisplayTFT
uint16_t Xfmod = 0;
uint16_t Yfmod = 0; // 88;
#endif


Mode md[] = {
  { 0  , Xfmod, 100, Yfmod, 30,  0},
  { 1  , Xfmod, 100, Yfmod, 30, 30},
  { 2  , Xfmod, 100, Yfmod, 30, 60},
  { 3  , Xfmod, 100, Yfmod, 30, 90},
  { 4  , Xfmod, 100, Yfmod, 30,120}
};
//======================================================= End Modulation Types     ============================

//======================================================= Keypath     =========================================
typedef struct // Keypath
{
  uint16_t KeypNum;
  uint16_t Xkeypos;          //Xoffset
  uint16_t Xkeypsr;          //X size rectang
  uint16_t Xkeypnr;          //Y next rectang
  uint16_t Ykeypos;          //Yoffset
  uint16_t Ykeypsr;          //X size rectang
  uint16_t Ykeypnr;          //Y next rectang
} Keypath;

//  Keypath table

#ifdef IhaveTDisplayTFT
uint16_t Xpath = 35;
uint16_t Ypath = 47;

Keypath kp[] = {
  {  0 , Xpath,  60 ,   0 , Ypath , 60 ,   0},
  {  1 , Xpath,  60 ,  60 , Ypath , 60 ,   0},
  {  2 , Xpath,  60 , 120 , Ypath , 60 ,   0},
  {  3 , Xpath,  60 ,   0 , Ypath , 60 ,  60},
  {  4 , Xpath,  60 ,  60 , Ypath , 60 ,  60},
  {  5 , Xpath,  60 , 120 , Ypath , 60 ,  60},
  {  6 , Xpath,  60 ,   0 , Ypath , 60 , 120},
  {  7 , Xpath,  60 ,  60 , Ypath , 60 , 120},
  {  8 , Xpath,  60 , 120 , Ypath , 60 , 120},
  {  9 , Xpath,  60 ,   0 , Ypath , 60 , 180},
  { 10 , Xpath,  60 ,  60 , Ypath , 60 , 180},
  { 11 , Xpath,  60 , 120 , Ypath , 60 , 180},
};
#endif

//======================================================= End Keypath     =====================================

//======================================================= Bandwidth AM, SSB, FM     ===============================
typedef struct // Bandwidth AM & SSB & FM
{
  uint16_t BandWidthAM;
  uint16_t BandWidthSSB;
  uint16_t BandWidthFM;
  uint16_t Xos;          //Xoffset
  uint16_t Xsr;          //X size rectang
  uint16_t Yos;          //Yoffset
  uint16_t Ysr;          //X size rectang
  uint16_t Ynr;          //Y next rectang
} Bandwidth;

//  Bandwidth table
#ifdef IhaveTDisplayTFT
uint16_t XfBW = 0;
uint16_t YfBW = 0; // 88;
#endif

Bandwidth bw[] = {                                 //  AM    SSB   FM
  { 4 , 4 , 0 , XfBW, 100, YfBW, 30,   0},         //  1.0   0.5   Aut
  { 5 , 5 , 1 , XfBW, 100, YfBW, 30,  30},         //  1.8   1.0   110
  { 3 , 0 , 2 , XfBW, 100, YfBW, 30,  60},         //  2.0   1.2    84
  { 6 , 1 , 3 , XfBW, 100, YfBW, 30,  90},         //  2.5   2.2    60
  { 2 , 2 , 4 , XfBW, 100, YfBW, 30, 120},         //  3.0   3.0    40
  { 1 , 3 , 0 , XfBW, 100, YfBW, 30, 150},         //  4.0   4.0   ---
  { 0 , 0 , 0 , XfBW, 100, YfBW, 30, 180}          //  6.0   ---   ---
};
//======================================================= End Bandwidth AM & FM & SSB ===========================

//=======================================================    Broad Band Definitions   ==========================
typedef struct // Broad-Band switch
{
  uint16_t BbandNum;          // bandIdx
  uint16_t Xbbandos;          //Xoffset
  uint16_t Xbbandsr;          //X size rectang
  uint16_t Xbbandnr;          //X next rectang
  uint16_t Ybbandos;          //Yoffset
  uint16_t Ybbandsr;          //X size rectang
  uint16_t Ybbandnr;          //Y next rectang
} BBandnumber;

//  Bandnumber table for the broad-bands

#ifdef IhaveTDisplayTFT
uint16_t Xfbband = 0;
uint16_t Yfbband = 0; // 88;
#endif


BBandnumber bb[] = {
  {  0 , Xfbband, 80 ,   0 , Yfbband , 30 ,   0}, // 0
  {  1 , Xfbband, 80 ,   0 , Yfbband , 30 ,  30}, // 1
  {  2 , Xfbband, 80 ,   0 , Yfbband , 30 ,  60}, // 2
  {  6 , Xfbband, 80 ,   0 , Yfbband , 30 ,  90}, // 3
  {  7 , Xfbband, 80 ,   0 , Yfbband , 30 , 120}, // 4
  {  9 , Xfbband, 80 ,   0 , Yfbband , 30 , 150}, // 5
  { 11 , Xfbband, 80 ,   0 , Yfbband , 30 , 180}, // 6
  { 13 , Xfbband, 80 ,  80 , Yfbband , 30 ,   0}, // 7
  { 14 , Xfbband, 80 ,  80 , Yfbband , 30 ,  30}, // 8
  { 16 , Xfbband, 80 ,  80 , Yfbband , 30 ,  60}, // 9
  { 17 , Xfbband, 80 ,  80 , Yfbband , 30 ,  90}, //10
  { 19 , Xfbband, 80 ,  80 , Yfbband , 30 , 120}, //11
  { 21 , Xfbband, 80 ,  80 , Yfbband , 30 , 150}, //12
  { 22 , Xfbband, 80 ,  80 , Yfbband , 30 , 180}, //13
  { 24 , Xfbband, 80 , 160 , Yfbband , 30 ,   0}, //14
  { 26 , Xfbband, 80 , 160 , Yfbband , 30 ,  30}, //15
  { 27 , Xfbband, 80 , 160 , Yfbband , 30 ,  60}, //16
  { 29 , Xfbband, 80 , 160 , Yfbband , 30 ,  90}  //17
};
int selectedBroad = 0;
//======================================================= End Broad Band Definitions     ======================

//======================================================= Ham Band Definitions     ============================
typedef struct // Ham Band switch
{
  uint16_t BandNum;          // bandIdx
  uint16_t HamBandTxt;
  uint16_t Xbandos;          //Xoffset
  uint16_t Xbandsr;          //X size rectang
  uint16_t Xbandnr;          //X next rectang
  uint16_t Ybandos;          //Yoffset
  uint16_t Ybandsr;          //Y size rectang
  uint16_t Ybandnr;          //Y next rectang
} Bandnumber;

//  Bandnumber table for the hambands

#ifdef IhaveTDisplayTFT
uint16_t Xfband = 0;
uint16_t Yfband = 0; // 88;
#endif

Bandnumber bn[] = {
  {  3 ,  0 , Xfband, 110 ,   0 , Yfband , 30 ,   0},
  {  4 ,  1 , Xfband, 110 ,   0 , Yfband , 30 ,  30},
  {  5 ,  2 , Xfband, 110 ,   0 , Yfband , 30 ,  60},
  {  8 ,  3 , Xfband, 110 ,   0 , Yfband , 30 ,  90},
  { 10 ,  4 , Xfband, 110 ,   0 , Yfband , 30 , 120},
  { 12 ,  5 , Xfband, 110 ,   0 , Yfband , 30 , 150},
  { 15 ,  6 , Xfband, 110 , 110 , Yfband , 30 ,   0},
  { 18 ,  7 , Xfband, 110 , 110 , Yfband , 30 ,  30},
  { 20 ,  8 , Xfband, 110 , 110 , Yfband , 30 ,  60},
  { 23 ,  9 , Xfband, 110 , 110 , Yfband , 30 ,  90},
  { 25 , 10 , Xfband, 110 , 110 , Yfband , 30 , 120},
  { 28 , 11 , Xfband, 110 , 110 , Yfband , 30 , 150}
};
int selectedHam = 0;
//======================================================= End Ham Band Definitions     ========================

//======================================================= THE Band Definitions     ============================
typedef struct // Band data
{
  const char *bandName; // Bandname
  uint8_t  bandType;    // Band type (FM, MW or SW)
  uint16_t prefmod;     // Pref. modulation
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint8_t  currentStep; // Default step (increment and decrement)
  int          lastBFO; // Last BFO per band
  int      lastmanuBFO; // Last Manual BFO per band using X-Tal

} Band;

//   Band table
Band band[] = {
  {   "FM", FM_BAND_TYPE,  FM,  8750, 10800, 10680, 10, 0, 0}, //  FM          0         
  {   "LW", LW_BAND_TYPE,  AM,   130,   279,   198,  9, 0, 0}, //  LW          1
  {   "MW", MW_BAND_TYPE,  AM,   522,  1701,  1395,  9, 0, 0}, //  MW          2
  {"BACON", LW_BAND_TYPE,  AM,   280,   470,   284,  1, 0, 0}, // Ham  600M    3
  { "630M", SW_BAND_TYPE, LSB,   472,   479,   475,  1, 0, 0}, // Ham  630M    4
  { "160M", SW_BAND_TYPE, LSB,  1800,  1910,  1899,  1, 0, 0}, // Ham  160M    5
  { "120M", SW_BAND_TYPE,  AM,  2300,  2495,  2400,  5, 0, 0}, //      120M    6
  {  "90M", SW_BAND_TYPE,  AM,  3200,  3400,  3300,  5, 0, 0}, //       90M    7
  {  "80M", SW_BAND_TYPE, LSB,  3500,  3800,  3630,  1, 0, 0}, // Ham   80M    8
  {  "75M", SW_BAND_TYPE,  AM,  3900,  4000,  3950,  5, 0, 0}, //       75M    9
  {  "60M", SW_BAND_TYPE, USB,  5330,  5410,  5375,  1, 0, 0}, // Ham   60M   10
  {  "49M", SW_BAND_TYPE,  AM,  5900,  6200,  6000,  5, 0, 0}, //       49M   11
  {  "40M", SW_BAND_TYPE, LSB,  7000,  7300,  7100,  1, 0, 0}, // Ham   40M   12
  {  "41M", SW_BAND_TYPE,  AM,  7200,  7450,  7210,  5, 0, 0}, //       41M   13
  {  "31M", SW_BAND_TYPE,  AM,  9400,  9900,  9600,  5, 0, 0}, //       31M   14
  {  "30M", SW_BAND_TYPE, USB, 10100, 10150, 10125,  1, 0, 0}, // Ham   30M   15
  {  "25M", SW_BAND_TYPE,  AM, 11600, 12100, 11700,  5, 0, 0}, //       25M   16
  {  "22M", SW_BAND_TYPE,  AM, 13570, 13870, 13700,  5, 0, 0}, //       22M   17
  {  "20M", SW_BAND_TYPE, USB, 14000, 14350, 14250,  1, 0, 0}, // Ham   20M   18
  {  "19M", SW_BAND_TYPE,  AM, 15100, 15830, 15700,  5, 0, 0}, //       19M   19
  {  "17M", SW_BAND_TYPE, USB, 18068, 18168, 18100,  1, 0, 0}, // Ham   17M   20
  {  "16M", SW_BAND_TYPE,  AM, 17480, 17900, 17600,  5, 0, 0}, //       16M   21
  {  "15M", SW_BAND_TYPE,  AM, 18900, 19020, 18950,  5, 0, 0}, //       15M   22
  {  "15M", SW_BAND_TYPE, USB, 21000, 21450, 21350,  1, 0, 0}, // Ham   15M   23
  {  "13M", SW_BAND_TYPE,  AM, 21450, 21850, 21500,  5, 0, 0}, //       13M   24
  {  "12M", SW_BAND_TYPE, USB, 24890, 24990, 24940,  1, 0, 0}, // Ham   12M   25
  {  "11M", SW_BAND_TYPE,  AM, 25670, 26100, 25800,  5, 0, 0}, //       11M   26
  {   "CB", SW_BAND_TYPE,  AM, 26200, 27990, 27200,  1, 0, 0}, // CB band     27
  {  "10M", SW_BAND_TYPE, USB, 28000, 30000, 28500,  1, 0, 0}, // Ham   10M   28
  {   "SW", SW_BAND_TYPE,  AM,  1730, 30000, 15500,  5, 0, 0}  // Whole SW    29
};
//======================================================= End THE Band Definitions     ========================

//======================================================= FM Presets     ======================================
typedef struct // Preset data
{
  float      presetIdx;
  const char *PresetName;
} FM_Preset ;

FM_Preset preset[] = {
  8900  , "Radio 2",      // 00 Radio 2
  8950  , "Radio 1",    // 01 Radio 1
  9050  , "RadioMaria",    // 02 Radio Maria
  9100  , "R. Bruno", // 03 Radio Bruno
  9120  , "Lattemiele",    // 04 Lattemiele
  9140  , "*R.M.C.*",      // 05 RMC
  9390  , "Radio 3",  // 06 Radio 3
  9410  , "*R.D.S.*",   // 07 R.D.S.
  9650  , "R. Delta",       // 08 Radio Delta
  9680  , "R. Stella",      // 09 Radio Stella
  9920  , "Birikina",       // 10 Radio Birikina
  10230  , "R. Italia",   // 11 R. Italia
  10250 , "RTL 102.5",  // 12 RTL 102.5
  10370 , "R 101",    // 13 R. 101
  10390 , "R. Padova",  // 14 R. Padova
  10420 , "FERRARA", // 15 R. Ferrara
  10450 , "*R.S.L.*",   // 16 R.s.Luchino
  10680 , "Radio 24",   // 17 Radio 24
  10790 , "R. 105"  // 18 R. 105
};
//======================================================= END FM Presets     ======================================

//======================================================= Tuning Digit selection ====================
typedef struct // Tuning digit
{
  uint8_t  digit;
  uint16_t Xdignumos;          //Xoffset
  uint16_t Xdignumsr;          //X size rectang
  uint16_t Ydignumos;          //Yoffset
  uint16_t Ydignumsr;          //Y size rectang
  uint16_t Xdignumnr;          //X next rectang
} DigNum;

uint8_t Xdignum = 139;
uint8_t Ydignum = 25;

//  Tuning digit table

DigNum dn[] = {
  { 0 ,Xdignum, 21, Ydignum, 35,  0},
  { 1 ,Xdignum, 21, Ydignum, 35, 30},
  { 2 ,Xdignum, 21, Ydignum, 35, 59}

};
//======================================================= End Tuning Digit selection     ===============================

const int lastButton = (sizeof bt / sizeof(Button));
const int lastBand   = (sizeof band / sizeof(Band));
const int lastHam    = (sizeof bn / sizeof(Bandnumber));
const int lastBroad  = (sizeof bb / sizeof(BBandnumber));
const int lastMod    = (sizeof md / sizeof(Mode));
const int lastBW     = (sizeof bw / sizeof(Bandwidth));
const int lastStep   = (sizeof sp / sizeof(Step));
const int lastStepFM = (sizeof stepsizeFM / sizeof(stepsizeFM[0]));
const int lastdignum = (sizeof dn / sizeof(DigNum)) - 1;
const int lastKPath  = (sizeof kp / sizeof(Keypath)) - 1;
const int lastPreset = (sizeof preset / sizeof (FM_Preset)) - 1;
const int lastBWSSB  = (sizeof bandwidthSSB / sizeof (bandwidthSSB[0]));
const int lastBWAM   = (sizeof bandwidthAM / sizeof (bandwidthAM[0]));
const int lastBWFM   = (sizeof bandwidthFM / sizeof (bandwidthFM[0]));


#define offsetEEPROM       0x20
#define EEPROM_SIZE        265

struct StoreStruct {
  byte     chkDigit;
  byte     bandIdx;
  uint16_t Freq;
  uint8_t  currentMode;
  uint8_t  bwIdxSSB;
  uint8_t  bwIdxAM;
  uint8_t  bwIdxFM;
  uint8_t  ssIdxMW;
  uint8_t  ssIdxAM;
  uint8_t  ssIdxFM;
  int      currentBFO;
  int      currentBFOmanu;
  uint8_t  currentAGCAtt;
  uint8_t  currentVOL;
  uint8_t  currentBFOStep;
  uint8_t  RDS;
  unsigned long FreqSI5351;
  uint16_t currentBrightness;
  uint8_t  currentAGCgain;
  unsigned long calibratvalSI5351;
  int  BFOLW;
  int  BFOMW;
  int  BFO600M;
  int  BFO630M;
  int  BFO160M;
  int  BFO120M;
  int  BFO90M;
  int  BFO80M;
  int  BFO75M;
  int  BFO60M;
  int  BFO49M;
  int  BFO40M;
  int  BFO41M;
  int  BFO31M;
  int  BFO30M;
  int  BFO25M;
  int  BFO22M;
  int  BFO20M;
  int  BFO19M;
  int  BFO17M;
  int  BFO16M;
  int  BFO15M;
  int  BFO15H;
  int  BFO13M;
  int  BFO12M;
  int  BFO11M;
  int  BFOCB;
  int  BFO10M;
  int  BFOSW; 
};

StoreStruct storage = {
      '@',  //First time check
        0,  //bandIdx
     8930,  //Freq
        0,  //mode
        1,  //bwIdxSSB
        1,  //bwIdxAM
        0,  //bwIdxFM
        9,  //ssIdxMW
        5,  //ssIdxAM
       10,  //ssIdxFM  
        0,  //currentBFO
        0,  //currentBFOmanu
        2,  //currentAGCAtt
       45,  //currentVOL
       10,  //currentBFOStep
        1,  //RDS
  3276800,  //FreqSI5351
      220,  //currentBrightness
        1,  //currentAGCgain
        0,  //calibratvalSI5351
        0,  //BFOLW;
        0,  //BFOMW
        0,  //BFO600M
        0,  //BFO630M
        0,  //BFO160M
        0,  //BFO120M
        0,  //BFO90M
        0,  //BFO80M
        0,  //BFO75M
        0,  //BFO60M
        0,  //BFO49M
        0,  //BFO40M
        0,  //BFO41M
        0,  //BFO31M
        0,  //BFO30M
        0,  //BFO25M
        0,  //BFO22M
        0,  //BFO20M
        0,  //BFO19M
        0,  //BFO17M
        0,  //BFO16M
        0,  //BFO15M
        0,  //BFO15H
        0,  //BFO13M
        0,  //BFO12M
        0,  //BFO11M
        0,  //BFOCB
        0,  //BFO10M
        0   //BFOSW
};

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

// Battery18650Stats battery(VBAT_MON);
Battery18650Stats battery(VBAT_MON,CONV_FACTOR, READS);
TFT_eSPI tft    = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

TFT_eSprite showrdsmsg = TFT_eSprite(&tft);	// scroll

TFT_eSprite freqScale = TFT_eSprite(&tft);	// Frequency Scale


SI4735 si4735;

//=======================================================================================
void IRAM_ATTR RotaryEncFreq() {
//=======================================================================================
  // rotary encoder events
  if (!writingEeprom) {
    encoderStatus = encoder.process();

    if (encoderStatus)
    {
      if (encoderStatus == DIR_CW)// Direction clockwise
      {
        encoderCount = 1;
      }
      else
      {
        encoderCount = -1;
      }
    }
  }
}

/**
 * Cleans the EEPROM
*/
void resetEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  for (int k = 0; k < EEPROM_SIZE; k++) {
      EEPROM.write(k, 0);
  }
  EEPROM.end();
}

// Performe a Soft Reset
void(* resetFunc) (void) = 0;



//=======================================================================================
void setup() {
  //=======================================================================================
  Serial.begin(115200);

  Serial.println(F("Initializing...."));

  pinMode(BEEPER, OUTPUT);
  pinMode(ENCODER_SWITCH, INPUT_PULLUP);

  tft.init();
//  tft.writecommand(TFT_DISPOFF);
  tft.fillScreen(TFT_BLACK);

  //Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL); //I2C for SI4735
  
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  // Beep(1, 200);

  //tft.setRotation(0); // Rotate 0
  //tft.setRotation(1); // Rotate 90
  //tft.setRotation(2); // Rotate 180
  //tft.setRotation(3); // Rotate 270

#ifdef IhaveTDisplayTFT
  tft.setRotation(1);
#endif


  ledcSetup(LedChannelforTFT, LedFreq, LedResol);
//  ledcAttachPin(TFT_BL, LedChannelforTFT);
  tft.fillScreen(TFT_BLACK);
  ledcWrite(LedChannelforTFT, 250);
//  tft.writecommand(TFT_DISPON);
  DISplay = true;

//==== Create a sprite for scroll rds messages ====
  showrdsmsg.setSwapBytes(true);
  showrdsmsg.setColorDepth(8);
  showrdsmsg.createSprite(1250, 20); // Sprite wider than the display plus the text to allow text to scroll from the right. //960
  showrdsmsg.fillSprite(TFT_BLACK);
  showrdsmsg.setScrollRect(0, 200, 1250, 20, TFT_BLACK); // Sprite wider than the display plus the text to allow text to scroll from the right.// 960									 
//=======end scroll rds messages========


  for (int i = 0; i < 10; i++) {
      if (digitalRead(ENCODER_SWITCH) == LOW) {
        resetEEPROM(); // Cleans the EEPROM.
        tft.fillScreen(TFT_BLACK);
        delay(500);
        tft.setTextSize(1);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.println(F("EEPROM ERASED!")); 
        Serial.println(F("EEPROM ERASED!")); 
        //while(1);  // Stops the System. It is needed to turn it off. 
        tft.println(F("Restart in 4 seconds")); 
        Serial.println(F("Restart in 4 seconds")); 
        delay(4000);
        resetFunc();
      }
      delay(100);
  } 


  if (!EEPROM.begin(EEPROM_SIZE))
  {
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(F("failed to initialise EEPROM"));
    Serial.println(F("failed to initialise EEPROM"));
    while (1);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(F("EEPROM initialization successful"));
    Serial.println(F("EEPROM initialization successful"));
  }



  if (EEPROM.read(offsetEEPROM) != storage.chkDigit) {
    Serial.println(F("Writing defaults...."));
    saveConfig();
  }
  loadConfig();
  printConfig();

  //Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL); //I2C for SI4735

  // Encoder pins
  pinMode(ENCODER_PIN_A , INPUT_PULLUP);
  pinMode(ENCODER_PIN_B , INPUT_PULLUP);
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), RotaryEncFreq, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), RotaryEncFreq, CHANGE);
  si4735.setAudioMuteMcuPin(AUDIO_MUTE);

  tft.fillScreen(TFT_BLACK);
  // delay(500);
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  Serial.println("     SI4735/32 Radio");
  Serial.println("Version 4.1.1 Jan-15-2023");
  tft.setCursor(0, 0);
  tft.println("   SI4735/32 Radio");
  // tft.setCursor(0, 40);
  tft.println("  V4.1.1 Jan-15-2023");
  tft.setCursor(0, 50);
  tft.println("  Sketch:   RXavier");
  tft.println("  Based on: PE0MGB");
  tft.println("  Library:  PU2CLR");
  tft.println("  Mod ili9341 by: IU4ALH");
  tft.setCursor(0, 140);
  // delay(1000);
  if ( si4735Addr == 0 ) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Si4735 not detected");
    Serial.println("Si4735 not detected");
    while (1);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("  Si473X addr :  ");
    tft.println(si4735Addr, HEX);
  }

  delay(1500);

  if (si4735Addr == 17)
  {
    si4735.setDeviceI2CAddress(0);
  }
  else
  {
    si4735.setDeviceI2CAddress(1);
  }

  // Setup the radio from last setup in EEPROM
  bandIdx                   =  storage.bandIdx;
  band[bandIdx].currentFreq =  storage.Freq;
  currentMode               =  storage.currentMode;
  bwIdxSSB                  =  storage.bwIdxSSB; // band width
  bwIdxAM                   =  storage.bwIdxAM;
  bwIdxFM                   =  storage.bwIdxFM;
  ssIdxMW                   =  storage.ssIdxMW; // step size
  ssIdxAM                   =  storage.ssIdxAM;
  ssIdxFM                   =  storage.ssIdxFM;
  currentBFO                =  storage.currentBFO;
  currentBFOmanu            =  storage.currentBFOmanu;
  currentAGCAtt             =  storage.currentAGCAtt;
  currentVOL                =  storage.currentVOL;
  currentBFOStep            =  storage.currentBFOStep;
  RDS                       =  storage.RDS;
  FreqSI5351                =  storage.FreqSI5351;
  currentBrightness         =  storage.currentBrightness;
  currentAGCgain            =  storage.currentAGCgain;
  calibratvalSI5351         =  storage.calibratvalSI5351;
  band[ 1].lastmanuBFO      =  storage.BFOLW;
  band[ 2].lastmanuBFO      =  storage.BFOMW;
  band[ 3].lastmanuBFO      =  storage.BFO600M;
  band[ 4].lastmanuBFO      =  storage.BFO630M;
  band[ 5].lastmanuBFO      =  storage.BFO160M;
  band[ 6].lastmanuBFO      =  storage.BFO120M;
  band[ 7].lastmanuBFO      =  storage.BFO90M;
  band[ 8].lastmanuBFO      =  storage.BFO80M;
  band[ 9].lastmanuBFO      =  storage.BFO75M;
  band[10].lastmanuBFO      =  storage.BFO60M;
  band[11].lastmanuBFO      =  storage.BFO49M;
  band[12].lastmanuBFO      =  storage.BFO40M;
  band[13].lastmanuBFO      =  storage.BFO41M;
  band[14].lastmanuBFO      =  storage.BFO31M;
  band[15].lastmanuBFO      =  storage.BFO30M;
  band[16].lastmanuBFO      =  storage.BFO25M;
  band[17].lastmanuBFO      =  storage.BFO22M;
  band[18].lastmanuBFO      =  storage.BFO20M;
  band[19].lastmanuBFO      =  storage.BFO19M;
  band[20].lastmanuBFO      =  storage.BFO17M;
  band[21].lastmanuBFO      =  storage.BFO16M;
  band[22].lastmanuBFO      =  storage.BFO15M;
  band[23].lastmanuBFO      =  storage.BFO15H;
  band[24].lastmanuBFO      =  storage.BFO13M;
  band[25].lastmanuBFO      =  storage.BFO12M;
  band[26].lastmanuBFO      =  storage.BFO11M;
  band[27].lastmanuBFO      =  storage.BFOCB;
  band[28].lastmanuBFO      =  storage.BFO10M;
  band[29].lastmanuBFO      =  storage.BFOSW; 

#ifdef IhaveCrystal
 if (bandIdx == 0) si4735.setup(RESET_PIN, FM_BAND_TYPE); //Start in FM
 else si4735.setup(RESET_PIN, 1);
 if (bandIdx != 0) si4735.setAM(); // Start in AM
#endif

  ledcWrite(LedChannelforTFT, currentBrightness);
  freqstep = 1000;//hz
  previousBFO = -1;
  band[bandIdx].lastBFO  = currentBFO;
  freqDec = currentBFO;
  band[bandIdx].prefmod = currentMode;
  si4735.setVolume(currentVOL);
  previousVOL = currentVOL;
  previousAGCgain = currentAGCgain;
  BandSet();
  currentFrequency = previousFrequency = band[bandIdx].currentFreq;
  Beep(2, 200);
  encBut = 600;
  x = y = 0;
  DrawFila();
  si4735.setSeekFmSpacing(10);
  si4735.setSeekFmLimits(band[0].minimumFreq, band[0].maximumFreq);
  si4735.setSeekAmRssiThreshold(50);
  si4735.setSeekAmSrnThreshold(20);
  si4735.setSeekFmRssiThreshold(5);
  si4735.setSeekFmSrnThreshold(5);
  xTaskCreate(SaveInEeprom, "SaveInEeprom", 2048, NULL, 1, NULL);
  // xTaskCreate(batteryMonitor, "batteryMonitor", 2048, NULL, 1, NULL);
  delay(10);
}// end setup
//=======================================================================================
//=======================================================================================


//=======================================================================================
void SaveInEeprom (void* arg)  {
//=======================================================================================
  while (1) {
    storage.bandIdx           = bandIdx;
    storage.Freq              = band[bandIdx].currentFreq;
    storage.currentMode       = currentMode;
    storage.bwIdxSSB          = bwIdxSSB;
    storage.bwIdxAM           = bwIdxAM;
    storage.bwIdxFM           = bwIdxFM;
    storage.ssIdxMW           = ssIdxMW;
    storage.ssIdxAM           = ssIdxAM;
    storage.ssIdxFM           = ssIdxFM;
    storage.currentBFO        = currentBFO;
    storage.currentBFOmanu    = currentBFOmanu;
    storage.currentAGCAtt     = currentAGCAtt;
    storage.currentVOL        = currentVOL;
    storage.currentBFOStep    = currentBFOStep;
    storage.RDS               = RDS;
    storage.FreqSI5351        = FreqSI5351;
    storage.currentBrightness = currentBrightness;
    storage.currentAGCgain    = currentAGCgain;
    storage.calibratvalSI5351 = calibratvalSI5351;   
    storage.BFOLW = band[1].lastmanuBFO;
    storage.BFOMW = band[2].lastmanuBFO;
    storage.BFO600M = band[3].lastmanuBFO;
    storage.BFO630M = band[4].lastmanuBFO;
    storage.BFO160M = band[5].lastmanuBFO;
    storage.BFO120M = band[6].lastmanuBFO;
    storage.BFO90M = band[7].lastmanuBFO;
    storage.BFO80M = band[8].lastmanuBFO;
    storage.BFO75M = band[9].lastmanuBFO;
    storage.BFO60M = band[10].lastmanuBFO;
    storage.BFO49M = band[11].lastmanuBFO;
    storage.BFO40M = band[12].lastmanuBFO;
    storage.BFO41M = band[13].lastmanuBFO;
    storage.BFO31M = band[14].lastmanuBFO;
    storage.BFO30M = band[15].lastmanuBFO;
    storage.BFO25M = band[16].lastmanuBFO;
    storage.BFO22M = band[17].lastmanuBFO;
    storage.BFO20M = band[18].lastmanuBFO;
    storage.BFO19M = band[19].lastmanuBFO;
    storage.BFO17M = band[20].lastmanuBFO;
    storage.BFO16M = band[21].lastmanuBFO;
    storage.BFO15M = band[22].lastmanuBFO;
    storage.BFO15H = band[23].lastmanuBFO;
    storage.BFO13M = band[24].lastmanuBFO;
    storage.BFO12M = band[25].lastmanuBFO;
    storage.BFO11M = band[26].lastmanuBFO;
    storage.BFOCB  = band[27].lastmanuBFO;
    storage.BFO10M = band[28].lastmanuBFO;
    storage.BFOSW  = band[29].lastmanuBFO;   
    
    for (unsigned int t = 0; t < sizeof(storage); t++) {
      delay(1);
      if (EEPROM.read(offsetEEPROM + t) != *((char*)&storage + t)) {
        delay(1);
        EEPROM.write(offsetEEPROM + t, *((char*)&storage + t));
      }
    }

    writingEeprom = true;
    EEPROM.commit();
    writingEeprom = false;
    vTaskDelay(5000 / portTICK_RATE_MS);
  }

}

//=======================================================================================
void saveConfig() {
//=======================================================================================
  delay(10);
  for (unsigned int t = 0; t < sizeof(storage); t++) {
    if (EEPROM.read(offsetEEPROM + t) != *((char*)&storage + t)) {
      EEPROM.write(offsetEEPROM + t, *((char*)&storage + t));
    }
  }
  EEPROM.commit();
}

//=======================================================================================
void loadConfig() {
//=======================================================================================
  if (EEPROM.read(offsetEEPROM + 0) == storage.chkDigit) {
    for (unsigned int t = 0; t < sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(offsetEEPROM + t);
    Serial.println("Load config done");
  }
}

//=======================================================================================
void printConfig() {
//=======================================================================================
  Serial.print("Storage = ");
  Serial.println(sizeof(storage));
  if (EEPROM.read(offsetEEPROM) == storage.chkDigit) {
    for (unsigned int t = 0; t < sizeof(storage); t++)
      Serial.write(EEPROM.read(offsetEEPROM + t));
      Serial.println();
  }
}

//=======================================================================================
void BandSet()  {
//=======================================================================================
  if (bandIdx == 0) currentMode = FM;// only mod FM in FM band
  if ((currentMode == AM) or (currentMode == FM)) {
    ssbLoaded = false;
  }
  if ((currentMode == LSB) or  (currentMode == USB)) 
  {
    if (ssbLoaded == false) {
      // showWarningMessage("Loading patch. Wait");
      loadSSB();
    } 
  }
  useBand();
  setBandWidth();
}

//=======================================================================================
void useBand()  {
//=======================================================================================
  if ((band[bandIdx].bandType == MW_BAND_TYPE) || (band[bandIdx].bandType == LW_BAND_TYPE)) {
    band[bandIdx].currentStep = ssIdxMW;
  }    
  if (band[bandIdx].bandType == SW_BAND_TYPE) {   
    band[bandIdx].currentStep = ssIdxAM;
  }
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    bfoOn = false;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    delay(100);
    band[bandIdx].currentStep = ssIdxFM;
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    si4735.setFMDeEmphasis(1);
    si4735.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    ssbLoaded = false;
    si4735.RdsInit();
    si4735.setRdsConfig(1, 2, 2, 2, 2);
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
      si4735.setTuneFrequencyAntennaCapacitor(0);
    }else{
      si4735.setTuneFrequencyAntennaCapacitor(1);
    }
    if (ssbLoaded)
    {
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      si4735.setSSBAutomaticVolumeControl(1);
      //si4735.setSsbSoftMuteMaxAttenuation(0); // Disable Soft Mute for SSB
      //si4735.setSSBDspAfc(0);
      //si4735.setSSBAvcDivider(3);
      //si4735.setSsbSoftMuteMaxAttenuation(8); // Disable Soft Mute for SSB
      //si4735.setSBBSidebandCutoffFilter(0);
      si4735.setSSBBfo(currentBFO);
      int temp = 1; // SSB ONLY 1KHz stepsize
      si4735.setFrequencyStep(temp);
      band[bandIdx].currentStep = temp;
    }
    else
    {
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      //si4735.setAutomaticGainControl(1, 0);
      //si4735.setAmSoftMuteMaxAttenuation(0); // // Disable Soft Mute for AM
      bfoOn = false;
    }
  }
  delay(100);

}// end useband

//=======================================================================================
void setBandWidth()  {
//=======================================================================================
  if (currentMode == LSB || currentMode == USB)
  {
    si4735.setSSBAudioBandwidth(bwIdxSSB);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
      si4735.setSBBSidebandCutoffFilter(0);
    else
      si4735.setSBBSidebandCutoffFilter(1);
  }
  if (currentMode == AM)
  {
    si4735.setBandwidth(bwIdxAM, 0);
  }
  if (currentMode == FM)
  {
    si4735.setFmBandwidth(bwIdxFM);
  }
}

//=======================================================================================
void loadSSB()  {
//=======================================================================================
  si4735.reset();
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  si4735.patchPowerUp();
  delay(50);
  si4735.setI2CFastMode(); // Recommended
  //si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
  si4735.downloadPatch(ssb_patch_content, size_content);
  si4735.setI2CStandardMode(); // goes back to default (100KHz)

  // delay(50);
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  si4735.setSSBConfig(bwIdxSSB, 1, 0, 1, 0, 1);
  delay(25);
  ssbLoaded = true;
}


//=======================================================================================
void Freqcalq(int keyval)  {
//=======================================================================================
  if (Decipoint) {
    dpfrq = dpfrq + keyval / fact;
  }
  else Displayfreq = (Displayfreq + keyval) * 10;
  fact = fact * 10;
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(50, 0);
  if (Decipoint) {
    tft.print((Displayfreq / 10) + dpfrq, 4);
    tft.print(" Mhz");
  }
  else {
    tft.print((Displayfreq / 10) + dpfrq, 0);
    if (((Displayfreq / 10) + dpfrq) <= 30000) tft.print(" KHz");
    else tft.print(" Hz");
  }
}


//=======================================================================================
void Smeter() {
//=======================================================================================
int spoint;
  if (currentMode != FM) {
    //dBuV to S point conversion HF
    if ((rssi >= 0) and (rssi <=  1)) spoint =  12;                    // S0
    if ((rssi >  1) and (rssi <=  2)) spoint =  24;                    // S1
    if ((rssi >  2) and (rssi <=  3)) spoint =  36;                    // S2
    if ((rssi >  3) and (rssi <=  4)) spoint =  48;                    // S3
    if ((rssi >  4) and (rssi <= 10)) spoint =  48 + (rssi - 4) * 2;   // S4
    if ((rssi > 10) and (rssi <= 16)) spoint =  60 + (rssi - 10) * 2;  // S5
    if ((rssi > 16) and (rssi <= 22)) spoint =  72 + (rssi - 16) * 2;  // S6
    if ((rssi > 22) and (rssi <= 28)) spoint =  84 + (rssi - 22) * 2;  // S7
    if ((rssi > 28) and (rssi <= 34)) spoint =  96 + (rssi - 28) * 2;  // S8
    if ((rssi > 34) and (rssi <= 44)) spoint = 108 + (rssi - 34) * 2;  // S9
    if ((rssi > 44) and (rssi <= 54)) spoint = 124 + (rssi - 44) * 2;  // S9 +10
    if ((rssi > 54) and (rssi <= 64)) spoint = 140 + (rssi - 54) * 2;  // S9 +20
    if ((rssi > 64) and (rssi <= 74)) spoint = 156 + (rssi - 64) * 2;  // S9 +30
    if ((rssi > 74) and (rssi <= 84)) spoint = 172 + (rssi - 74) * 2;  // S9 +40
    if ((rssi > 84) and (rssi <= 94)) spoint = 188 + (rssi - 84) * 2;  // S9 +50
    if  (rssi > 94)                   spoint = 204;                    // S9 +60
    if  (rssi > 95)                   spoint = 208;                    //>S9 +60
  }
  else
  {
    //dBuV to S point conversion FM
    if  (rssi <  1) spoint = 36;
    if ((rssi >  1) and (rssi <=  2)) spoint =  60;                    // S6
    if ((rssi >  2) and (rssi <=  8)) spoint =  84 + (rssi - 2) * 2;   // S7
    if ((rssi >  8) and (rssi <= 14)) spoint =  96 + (rssi - 8) * 2;   // S8
    if ((rssi > 14) and (rssi <= 24)) spoint = 108 + (rssi - 14) * 2;  // S9
    if ((rssi > 24) and (rssi <= 34)) spoint = 124 + (rssi - 24) * 2;  // S9 +10
    if ((rssi > 34) and (rssi <= 44)) spoint = 140 + (rssi - 34) * 2;  // S9 +20
    if ((rssi > 44) and (rssi <= 54)) spoint = 156 + (rssi - 44) * 2;  // S9 +30
    if ((rssi > 54) and (rssi <= 64)) spoint = 172 + (rssi - 54) * 2;  // S9 +40
    if ((rssi > 64) and (rssi <= 74)) spoint = 188 + (rssi - 64) * 2;  // S9 +50
    if  (rssi > 74)                   spoint = 204;                    // S9 +60
    if  (rssi > 76)                   spoint = 208;                    //>S9 +60
  }


    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(BC_DATUM);
    tft.setCursor(2, Ysmtr+42);
    tft.print("S|");

    tft.setCursor(2, Ysmtr+125);
    tft.print("<-");
    tft.setCursor(290, Ysmtr+125);
    tft.print("->");


    tft.fillRect(30, Ysmtr + 113 , 250, 40, TFT_BLACK); 
    tft.setCursor(50, Ysmtr+125);
    tft.print("Push Button to set");
    
  
  tft.fillRect(Xsmtr + 45, Ysmtr + 50 , (3 + spoint), 5, TFT_GREEN); 

  if (spoint > 45) {
    tft.fillRect(Xsmtr + 93, Ysmtr + 48 , (3 + spoint - 40), 7, TFT_ORANGE); 
  }
  if (spoint > 105) {
    tft.fillRect(Xsmtr + 155, Ysmtr + 45 , (3 + spoint - 60), 11, TFT_RED); 
  }

  for (int i = 0; i < 19; i++) 
    tft.fillRect(Xsmtr + 32 + (i * 12), Ysmtr + 45, 3, 13, TFT_BLACK); 
  tft.fillRect(Xsmtr + 37 + spoint, Ysmtr + 45 , 226 - (2 + spoint), 11, TFT_BLACK);
}


//=======================================================================================
void VolumeIndicator(int vol) {
//=======================================================================================
  // vol = map(vol, 20, 63, 0, 212);
  vol = map(vol, 20, 63, 0, 203 - 2);
  // tft.fillRect(XVolInd + 24, YVolInd + 55 , (2 + vol), 6, TFT_YELLOW);
  // tft.fillRect(XVolInd + 26 + vol, YVolInd + 55 , 203 - (2 + vol), 6, TFT_BLUE);

  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillRect(XFreqDispl + 253, YFreqDispl + 112, 44, 18, TFT_BLACK);
  tft.drawString(String ("Vol."), XFreqDispl + 240, YFreqDispl + 130); //////volume
  tft.drawString(String(map(currentVOL, 20, 63, 0, 100)), XFreqDispl + 280, YFreqDispl + 130); //////volume

}

//=======================================================================================
void brightnessIndicator(int vol) {
//=======================================================================================
  // vol = map(vol, 250, 20, 0, 212);
  vol = map(vol, 250, 20, 0, 203 - 2);
  // tft.fillRect(XVolInd + 24, YVolInd + 55 , (2 + vol), 6, TFT_RED);
  // tft.fillRect(XVolInd + 26 + vol, YVolInd + 55 , 203 - (2 + vol), 6, TFT_GREEN);
}

//=======================================================================================
void loop() {
//=======================================================================================
  if ((FirstLayer == true) or (ThirdLayer == true)) VolumeIndicator(si4735.getVolume());

  // Pressed will be set true if button is pressed
  
  while (((pressed == false) and (encoderCount == 0) and (encoderButton == 0) and (digitalRead(ENCODER_SWITCH) == HIGH)) or (writingEeprom)) {  // wait loop
    pressed = false;
    showtimeRSSI();
    DisplayRDS();
 //   Dispoff();
 //   MuteAud();
    batteryMonitor();
  }
  encoderCheck();        // Check if the encoder has moved.
  encoderButtonCheck();  // Check if encoderbutton is pressed
  
  if (pressed) {
    pressed = false;
    PRESbut = false; // Preset stopped after other button is pressed

    DisplayOnTime = millis();
    if (DISplay == false) {
      // Beep(1, 0);
      delay(400);
      //digitalWrite(TFT_BL, displayon);
 //     tft.writecommand(TFT_DISPON);
      ledcWrite(LedChannelforTFT, currentBrightness);
      DISplay = true;
    }
    if (FirstLayer) { //==================================================

      /*if (( x > 139) and (x < 219) and (y > 25) and (y < 60)) {    // digit selection
        for (int n = 0 ; n <=lastdignum ; n++) { 
          if ((x > (dn[n].Xdignumos) + (dn[n].Xdignumnr)) and (x < ((dn[n].Xdignumos) + (dn[n].Xdignumsr) + (dn[n].Xdignumnr))) and (y > (dn[n].Ydignumos) and (y < ((dn[n].Ydignumos) + (dn[n].Ydignumsr) )))) {
            Beep(1, 0);  
            delay(400);
            if ((currentMode == LSB || currentMode == USB || currentMode == CW) and (bfoOn == false)){
              freqstepnr = n;
              if (freqstepnr == 0)  freqstep = 1000;
              if (freqstepnr == 1)  freqstep = 100;
              if (freqstepnr == 2)  freqstep = 10;    
              FreqDispl();
            }
            if ((currentMode == LSB || currentMode == USB || currentMode == CW) and (bfoOn == true)){
              if (n ==1) stepsizesynth = 10;
              if (n ==2) stepsizesynth = 1;
              FreqDispl();
            }
          }
        }
      }*/

      //Check which button is pressed in First Layer.

        int n = selectedMenu;

          // Beep(1, 0);
          // delay(400);

          if ((VOLbut == true) and (n != VOL)) {
            VOLbut = false;
            DrawDispl ();
          }

          if ((Mutestat == true) and (n != MUTE)) {
            Mutestat = false;
            drawMUTE();
          }

          if ((bfoOn == true) and ((n == VOL) or (bright == true))) {
            bfoOn = false;
          }


          if (n == HAM) {
            // delay(400);  // HamBand button
            HamBand = true;
            HamBandlist();
            if (!pressed) {
              HamBand = false;
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            } else {
              FirstLayer = false;
              SecondLayer = true;
            }
          }

          if (n == BFO) {           //==============================  // BFO button
            // delay(400);

            if (currentMode == LSB || currentMode == USB || currentMode == CW)  {
              if (bfoOn == false) {
                bfoOn = true;
              } else {
                bfoOn = false;
              }
              DrawDispl ();
              
            } else {
              // Beep(4, 100);
              showWarningMessage(" Valid only in SSB");
              delay(200);
            } 
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
          }

          if (n == FREQ) {          //============================  // Frequency input
            // delay(400);
            FREQbut = true;
            // Decipoint = false;
            // Displayfreq = 0;
            // dpfrq = 0;
            // drawKeyPath();

            showWarningMessage("Not implemented Yet");
            delay(200);

            FirstLayer = false;
            SecondLayer = true;

          }

          if (n == AGC) {           //============================//AGC switch
            si4735.getAutomaticGainControl();
            AGCgain = 0;
            if  (si4735.isAgcEnabled()) {
              si4735.setAutomaticGainControl(1, 0);     //    disabled
            } else {
              AGCgainbut = false;
              si4735.setAutomaticGainControl(0, 0);      //   enabled
            }
            drawAGC();
            DrawDispl ();
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
          }

          if (n == MODE) {    //============================= MODE
            if (currentMode != FM)  {
              // delay(400);// Mode
              Modebut = true;
              Modelist();
              if (!pressed) {
                Modebut = false;
                DrawSmeter();
                DrawVolumeIndicator();
                OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
              } else {
                FirstLayer = false;
                SecondLayer = true;
              }
            } else {
              // Beep(4, 100);
              showWarningMessage(" Not valid in FM");
              delay(200);

              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }
          }


          if (n == BANDW) {        //=========================BANDWIDTH
            // delay(400);
            BandWidth = true;
            BWList();
            if (!pressed) {
              BandWidth = false;
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            } else {
              FirstLayer = false;
              SecondLayer = true;
            }
          }


          if (n == STEP) {            //========================= STEPS for tune and bfo
            // delay(400);
            if (bfoOn) setStep();
            if (currentMode == AM || currentMode == FM){
              STEPbut = true;
              Steplist();
              if (!pressed) {
                STEPbut = false;
                DrawSmeter();
                DrawVolumeIndicator();
                OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
              } else {
                FirstLayer = false;
                SecondLayer = true;
              }
            } 
            if ((currentMode == LSB || currentMode == USB) and (bfoOn == false)) {
              // Beep(4, 100);
              showWarningMessage(" Not valid in SSB");
              delay(200);
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }          
          }

          if (n == BROAD)  {
            // delay(400);
            BroadBand = true;
            BroadBandlist();
            if (!pressed) {
              BroadBand = false;
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            } else {
              FirstLayer = false;
              SecondLayer = true;
            }
          }

          if (n == PRESET) {
            // delay(400);
            PRESbut = true;
            FirstLayer = false;
            SecondLayer = true;
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
          }

          if (n == VOL) {
            // delay(400);
            if (VOLbut == false) {
              VOLbut = true;
              currentVOL = si4735.getVolume();
              previousVOL = currentVOL;
              FirstLayer = false;
              SecondLayer = true;
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }
            else {
              VOLbut = false;
              FirstLayer = true;
              SecondLayer = false;
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }
            FreqDispl();
          }

          if (n == MUTE) {
            // delay(200);
            if (Mutestat == false)  {
              Mutestat = true;
            }
            else  {
              Mutestat = false;
            }
            drawMUTE();
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
          }

          if (n == SEEKUP) {
            // delay(200);
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            VolumeIndicator(si4735.getVolume());
            SEEK = true;
            if ((currentMode != LSB) and (currentMode != USB))   {
              if (currentMode != FM) {     // No FM
                if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     //9 KHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
                }
                else {
                  bandIdx = 29;// all sw
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     // 5 KHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
                }
              }
              si4735.seekStationProgress(SeekFreq, checkStopSeeking,  SEEK_UP); // 1 is up
              delay(300);
              currentFrequency = si4735.getFrequency();
              band[bandIdx].currentFreq = currentFrequency ;
              if (currentFrequency != previousFrequency)
              {
                previousFrequency = currentFrequency;
                DrawDispl();
                delay(300);
              }
            } else {
              showWarningMessage(" Not valid in SSB");
              delay(200);
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }
            SEEK = false;
          }

          if (n == SEEKDN) {
            // delay(200);
            DrawSmeter();
            DrawVolumeIndicator();
            OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            VolumeIndicator(si4735.getVolume());
            SEEK = true;
            if ((currentMode != LSB) and (currentMode != USB))   {
              if (currentMode != FM) {     // No FM
                if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     //9 KHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
                } else {
                  bandIdx = 29;// all sw
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     // 5 KHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
                }
              }

              si4735.seekStationProgress(SeekFreq,checkStopSeeking,  SEEK_DOWN);
              delay(300);
              currentFrequency = si4735.getFrequency();
              band[bandIdx].currentFreq = currentFrequency ;
              if (currentFrequency != previousFrequency)
              {
                previousFrequency = currentFrequency;
                DrawDispl();
                delay(300);
              }
            } else {
              showWarningMessage(" Not valid in SSB");
              delay(200);
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }
            SEEK = false;
          }

          if (n == STATUS) {
            // delay(200);
            subrstatus();
            DrawFila();
          }

          if (n == RDSbut) {
            // delay(200);
            if (RDS) RDS = false;
            else RDS = true;

            DrawFila();
          }

          if (n == AGCset) {
            // delay(200);
            if (AGCgainbut) AGCgainbut = false;
            else {
              bfoOn = false; // only AGC function at the rotory encoder
              AGCgainbut = true;
              si4735.getAutomaticGainControl();
              previousAGCgain = 37; // force to setup AGC gain
            }
            FreqDispl();
            DrawFila();
          }

          if (n == Displbut) {
            // delay(200);
            if (bright == false)  {
              bright = true;
              if (bfoOn == true) {
                bfoOn = false;
                DrawFila();
              }
              previousBrightness = currentBrightness;
              DrawFila();
            }
            else {
              bright = false;
              DrawFila();
            }
          }

          if (n == ChipType) {
            // delay(200);
            showFirmwareInformation();
            DrawFila();
          }

    } // end FirstLayer

    if (SecondLayer) {  //===============================================================
      int n;
      pressed = false;
      if (Modebut == true) {
        if ( selectedMode == FM ) {
          Modebut = false;
          showWarningMessage("Invalid operation");
          delay(200);
          DrawFila();
        } else {
          n = selectedMode;
          currentMode = n;
          if ((CWShift == true) and (previousMode == USB)  ) {  
            currentBFO = currentBFO - 700;
            band[bandIdx].lastBFO = currentBFO;
            freqDec = currentBFO;
            CWShift = false;
          }
          if ((currentMode !=  previousMode) and (currentMode == CW) and (CWShift == false)) {
            currentMode = USB;
            CWShift = true;
            currentBFO = currentBFO + 700;
            band[bandIdx].lastBFO = currentBFO;
            freqDec = currentBFO;
          }
          Modebut = false;
          previousMode = currentMode;
          band[bandIdx].prefmod = currentMode;
          BandSet();
          DrawFila();
        }
      }

      if (BandWidth == true) {

        if ( currentMode == AM) {
          n = selectedBWAM;
          bwIdxAM = bw[n].BandWidthAM;
          BandWidth = false;
          BandSet();
          DrawFila();
        }
        if ((currentMode == LSB) or  (currentMode == USB) or (currentMode == CW)) {
          n = selectedBWSSB;
          bwIdxSSB = bw[n].BandWidthSSB;
          BandWidth = false;
          BandSet();
          DrawFila();
        }
        
        if (currentMode == FM) {
          n = selectedBWFM;
          bwIdxFM = bw[n].BandWidthFM;
          BandWidth = false;
          BandSet();
          DrawFila();
        }
      }
        
      if (STEPbut == true) {

        STEPbut = false;
        if (((band[bandIdx].bandType == MW_BAND_TYPE) or (band[bandIdx].bandType == LW_BAND_TYPE)) and ( currentMode == AM)) {           

          n = selectedStep;

          ssIdxMW = sp[n].stepFreq;
          si4735.setFrequencyStep(ssIdxMW);
          band[bandIdx].currentStep = ssIdxMW;
        }
        
        if ((band[bandIdx].bandType == SW_BAND_TYPE) and( currentMode == AM)) {           

          n = selectedStep;

          ssIdxAM = sp[n].stepFreq;
          si4735.setFrequencyStep(ssIdxAM);
          band[bandIdx].currentStep = ssIdxAM;
        }
  
        if (currentMode == FM){

          n = selectedStepFM;

          ssIdxFM = sp[n].stepFreqFM;
          si4735.setFrequencyStep(ssIdxFM);
          band[bandIdx].currentStep = ssIdxFM;
        }
        setStep();
        DrawFila();
      }

      
      if (BroadBand == true) {
        if (CWShift == true)  {    // CW reset
            currentBFO = currentBFO - 700;
            band[bandIdx].lastBFO = currentBFO;
            CWShift = false;
          }
        #ifdef IhaveCrystal   
        band[bandIdx].lastmanuBFO = currentBFOmanu;
        #endif

        int n = selectedBroad;

        BroadBand = false;
        bandIdx = bb[n].BbandNum;
        if ((bandIdx == 0) and (currentAGCgain >= 28)) currentAGCgain = previousAGCgain = 26; // currentAGCgain in FM max. 26
        si4735.setAM();

        delay(50);
        currentBFO = band[bandIdx].lastBFO;
        freqDec = currentBFO; 
        currentMode = band[bandIdx].prefmod;
        #ifdef IhaveCrystal
        currentBFOmanu = band[bandIdx].lastmanuBFO;
        #endif
        ssbLoaded = false;
        //bwIdxAM =  3;
        BandSet();
        DrawFila(); //Draw first layer
      }

      if (HamBand == true) {
        if (CWShift == true)  {     // CW reset
          currentBFO = currentBFO - 700;
          band[bandIdx].lastBFO = currentBFO;
          CWShift = false;
        }
        #ifdef IhaveCrystal   
        band[bandIdx].lastmanuBFO = currentBFOmanu;
        #endif

        int n = selectedHam;

        HamBand = false;
        bandIdx = bn[n].BandNum;
        if (ssbLoaded == false) {
          si4735.setAM();
          delay(50);
        }
        #ifdef IhaveCrystal
        currentBFOmanu = band[bandIdx].lastmanuBFO;
        #endif
        currentBFO = band[bandIdx].lastBFO;
        freqDec = currentBFO; 
        currentMode = band[bandIdx].prefmod;
        BandSet();
        DrawFila();
      }


      if (PRESbut == true) {
        // delay(200);
        if (currentMode != 0) { // geen fm ?
          bandIdx = 0;
          currentMode = 0;
          bfoOn = false;
          previousPRES = -1;
        }
        FirstLayer  =  true;
        SecondLayer = false;
        previousPRES = -2;
      }

      if (VOLbut == true) {
        // delay(200);
        currentVOL = si4735.getVolume();
        previousVOL = currentVOL;
        FirstLayer  =  true;
        SecondLayer = false;
        FreqDispl();
      }

      if (FREQbut == true) {
        if (CWShift == true)  {    // CW reset
          currentBFO = currentBFO - 700;
          band[bandIdx].lastBFO = currentBFO;
          CWShift = false;
        }
        #ifdef IhaveCrystal   
        band[bandIdx].lastmanuBFO = currentBFOmanu;
        #endif
        
        FREQbut = false;
        BandSet();
        DrawFila();       
        // spr.createSprite(But_Key_Width-6, But_Key_Height-5);
        // spr.fillScreen(COLOR_BACKGROUND);
        // for (int n = 0 ; n < 12; n++) { // which keys are pressed?
        //   if ((x > ((kp[n].Xkeypos) + (kp[n].Xkeypnr))) and (x < ((kp[n].Xkeypos) + (kp[n].Xkeypsr) + (kp[n].Xkeypnr))) and (y > ((kp[n].Ykeypos) + (kp[n].Ykeypnr))) and (y < ((kp[n].Ykeypos) + (kp[n].Ykeypsr) + (kp[n].Ykeypnr)))) {
        //     if ( n == 11) { // Send button is red
        //         spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Green);
        //     } else {
        //         spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Red);
        //     }
        //     spr.setTextColor(COLOR_BUTTON_TEXT);
        //     spr.setTextSize(2);
        //     spr.setTextDatum(BC_DATUM);
        //     spr.setTextPadding(0);
        //     spr.drawString((Keypathtext[kp[n].KeypNum]), (But_Key_Width/2)-2, (But_Key_Height/2)+9);
        //     spr.pushSprite((kp[n].Xkeypos + kp[n].Xkeypnr + 3), (kp[n].Ykeypos + kp[n].Ykeypnr  + 3));

        //     Beep(1, 0);
        //     delay(100);

        //     if ( n == 11) { // Send button is red
        //         spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Red);
        //     } else {
        //         spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Green);
        //     }
        //     spr.setTextColor(COLOR_BUTTON_TEXT);
        //     spr.setTextSize(2);
        //     spr.setTextDatum(BC_DATUM);
        //     spr.setTextPadding(0);
        //     spr.drawString((Keypathtext[kp[n].KeypNum]), (But_Key_Width/2)-2, (But_Key_Height/2)+9);
        //     spr.pushSprite((kp[n].Xkeypos + kp[n].Xkeypnr + 3), (kp[n].Ykeypos + kp[n].Ykeypnr  + 3));
        //     spr.deleteSprite();
          
        //     if ((n >= 0) and (n <= 8)) Freqcalq(n + 1);
        //     if (n == 10) Freqcalq(0);
        //     if (n == 9) {
        //       Decipoint = true;
        //       fact = 10;
        //     }          
        //     if (n == 11) {// SET button
        //       FREQbut = false;
        //       Displayfreq = (Displayfreq / 10) + dpfrq;
        //       if(Displayfreq < 1) {
        //         tft.setCursor(7, 25);
        //         tft.print("Freqency < 1 has no function");
        //         ErrorBeep();
        //       }else{
        //         if ((Displayfreq > 30) and (Displayfreq < 87.5 )) {
        //           tft.setCursor(7, 25);
        //           tft.print("Freqency > 30Mhz and < 87.5 MHz");
        //           ErrorBeep();
        //         }else{
        //           if ((Displayfreq >= 108) and (Displayfreq < 153 )) {
        //             tft.setCursor(7, 25);
        //             tft.print("Freqency >= 108 and < 153 ");
        //             ErrorBeep();
        //           }else{
        //             if (Displayfreq > 30000) Displayfreq = Displayfreq / 1000000;
        //             if ((Displayfreq <= 30000) and (Displayfreq >= 153) and (Decipoint == false )) Displayfreq = Displayfreq / 1000;
        //             if ((Displayfreq >= 87.5) and (Displayfreq <= 108)) {
        //               currentFrequency = Displayfreq * 100;
        //               bandIdx = 0;
        //               band[bandIdx].currentFreq = currentFrequency;
        //             }else{
        //               currentFrequency = Displayfreq * 1000;
        //               for (int q = 1 ; q <= lastBand; q++) {
        //                 if (((currentFrequency) >= band[q].minimumFreq) and ((currentFrequency) <= band[q].maximumFreq)) {
        //                   bandIdx = q; 
        //                   currentMode = band[q].prefmod;
        //                   if (((band[bandIdx].bandType == MW_BAND_TYPE) or (band[bandIdx].bandType == LW_BAND_TYPE))and( currentMode == AM)) {           
        //                     ssIdxMW = band[bandIdx].currentStep;         
        //                   }
             
        //                   if ((band[bandIdx].bandType == SW_BAND_TYPE) and( currentMode == AM)) {             
        //                     ssIdxAM = band[bandIdx].currentStep;
        //                   }
      
        //                   if (currentMode == FM){
        //                     ssIdxFM = band[bandIdx].currentStep;
        //                   }
        //                   break;
        //                 }
        //               }
        //               delay(100);
        //               band[bandIdx].currentFreq = currentFrequency;
        //               //currentMode = band[bandIdx].prefmod;
        //               freqDec = currentBFO = band[bandIdx].lastBFO = 0;
        //             }
        //           }
        //         }
        //       } 
        //       #ifdef IhaveCrystal
        //       currentBFOmanu = band[bandIdx].lastmanuBFO;
        //       #endif
        //       BandSet();
        //       DrawFila();
        //     }//   End   n=11 Send button
        //   }
        // }
      }//end freq
    }// end second layer

    if (ThirdLayer) { //==================================================

    } // end ThirdLayer

    if (ForthLayer) { //===============================================================

    } // end ForthLayer
  }// end pressed
   
  if (currentMode == LSB || currentMode == USB || currentMode == CW) // set BFO value in si4735
  {
    if ((currentBFO != previousBFO) or (currentBFOmanu != previousBFOmanu))
    {
      previousBFO = currentBFO;
      previousBFOmanu = currentBFOmanu;
      si4735.setSSBBfo(currentBFO+currentBFOmanu);
      if (bfoOn) FreqDispl();
    }
  }
  
  if (currentPRES != previousPRES)
  {
    si4735.getCurrentReceivedSignalQuality();
    if (si4735.isCurrentTuneFM() == false) {
      bandIdx = 0;
      band[bandIdx].currentFreq = ((preset[currentPRES].presetIdx));
      BandSet();
    }
    
    tft.setCursor(0, 20);
    if (currentPRES > lastPreset) currentPRES = 0;
    if (currentPRES < 0) currentPRES = lastPreset;
    previousPRES = currentPRES;
    DrawDispl();
    // tft.fillRect(XFreqDispl + 6, YFreqDispl + 19 , 228, 48, TFT_BLACK);// Black freq. field
    // Quick and dirty solution for battery icon 15x29
    tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 , 228-9, 10, TFT_BLACK); // Black freq. field
    tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 +10, 228, 48-10, TFT_BLACK); // Black freq. field
    // End - Quick and dirty solution for battery icon 15x29
    AGCfreqdisp();
    tft.setTextColor(TFT_YELLOW, TFT_BLACK );
    tft.setTextSize(2);
    tft.setTextDatum(BC_DATUM);

    tft.drawString(String(currentPRES) + ") " + String(((preset[currentPRES].presetIdx) / 100), 1), 60, 51);
    tft.setTextColor(TFT_WHITE, TFT_BLACK );
    tft.drawString(String(preset[currentPRES].PresetName), 175, 51);
    bandIdx = 0;
    si4735.setFrequency((preset[currentPRES].presetIdx));
    band[bandIdx].currentFreq = si4735.getFrequency();
  }


  if (currentVOL != previousVOL)
  {
    currentVOL = currentVOL + (currentVOL - previousVOL);
    tft.setCursor(0, 20);
    if (currentVOL > MaxVOL) currentVOL = MaxVOL;
    if (currentVOL < MinVOL) currentVOL = MinVOL;
    previousVOL = currentVOL;
    si4735.setVolume(currentVOL);
    FreqDispl();
  }

  if (currentBrightness != previousBrightness)
  {
    if (currentBrightness < MaxBrightness) currentBrightness = MaxBrightness;
    if (currentBrightness > MinBrightness) currentBrightness = MinBrightness;
    previousBrightness = currentBrightness;
    ledcWrite(LedChannelforTFT, currentBrightness);
  }

  if (currentAGCgain != previousAGCgain)
  {
    AGCgain = 1;
    tft.setCursor(0, 20);
    if (si4735.isCurrentTuneFM())  MaxAGCgain = MaxAGCgainFM;
    else MaxAGCgain = MaxAGCgainAM;

    if (currentAGCgain > MaxAGCgain) currentAGCgain = MaxAGCgain;
    if (currentAGCgain < MinAGCgain) currentAGCgain = MinAGCgain;

    previousAGCgain = currentAGCgain;
    si4735.setAutomaticGainControl(1, currentAGCgain);
    DrawDispl();
  }


//=======================================================================================
}// end loop
//=======================================================================================

//=======================================================================================
//void Dispoff()  {
//=======================================================================================
//  if (((millis() - DisplayOnTime) > MIN_ELAPSED_DISPL_TIME * 300) and (DISplay == true)) {
//    DISplay = false;

//    ledcWrite(LedChannelforTFT, 0);
//    tft.writecommand(TFT_DISPOFF);
    
//    PRESbut = false;
//    DrawDispl();
//    DisplayOnTime = millis();
//  }
//}

//=======================================================================================
void VOLbutoff()  {
//=======================================================================================
  if (((millis() - VOLbutOnTime) > MIN_ELAPSED_VOLbut_TIME * 30) and (VOLbut == true)) {
    VOLbut = false;
    FreqDispl();
  }
  if (VOLbut == false) VOLbutOnTime = millis();
}



//=======================================================================================
void DisplayRDS()  {
//=======================================================================================
  if (( currentMode == FM) and ((FirstLayer) or (ThirdLayer))) {
    if ( currentFrequency != previousFrequency ) {
      previousFrequency = currentFrequency;
      tft.fillRect(XFreqDispl + 60, YFreqDispl + 70, 130, 20, TFT_BLACK);  // clear RDS text
    }
    if ((RDS) and  (NewSNR >= 9)) checkRDS();
    else  tft.fillRect(XFreqDispl + 60, YFreqDispl + 70, 130, 20, TFT_BLACK); // clear RDS text
  }
}

//=======================================================================================
void showtimeRSSI() {
//=======================================================================================
  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * RSSIfact) // 150 * 1  = 150 msec refresh time RSSI
  {
    si4735.getCurrentReceivedSignalQuality();
    NewRSSI = si4735.getCurrentRSSI();
    NewSNR = si4735.getCurrentSNR();
    if (OldRSSI != NewRSSI)
    {
      OldRSSI = NewRSSI;
      showRSSI();
    }
    elapsedRSSI = millis();
  }
}

//=======================================================================================
void showRSSI() {
//=======================================================================================
  if ((  currentMode == FM ) and ((FirstLayer) or (ThirdLayer))) {
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    tft.setTextColor(TFT_YELLOW, TFT_RED);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    tft.fillRect(XFreqDispl + 191, YFreqDispl + 71 , 38, 11, TFT_RED); // STEREO MONO
    tft.drawString(buffer, XFreqDispl + 210, YFreqDispl + 81); 
  }
  rssi = NewRSSI;
  if ((FirstLayer) or (ThirdLayer)) Smeter();
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  if ((FirstLayer) or (ThirdLayer)) {  // dBuV and dB at freq. display
    tft.fillRect(XFreqDispl + 7, YFreqDispl + 8 , 80, 10, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("RSSI = " , XFreqDispl + 7, YFreqDispl + 0);
    tft.drawString("SNR = ", XFreqDispl + 65, YFreqDispl + 0);
    tft.setTextDatum(TR_DATUM);
    
tft.setTextColor(TFT_YELLOW, TFT_BLACK);

tft.drawString(String(NewRSSI) + " dBuV" , XFreqDispl + 50, YFreqDispl + 10);

tft.drawString(String(NewSNR) + " dB", XFreqDispl + 93, YFreqDispl + 10);
  
  }
  VOLbutoff();
}

//=======================================================================================
void encoderCheck()  {
//=======================================================================================
  if (encoderCount != 0)
  {
    if (DISplay == false) { //  Wake-up  Display
      DisplayOnTime = millis();
//      tft.writecommand(TFT_DISPON);
      ledcWrite(LedChannelforTFT, currentBrightness);
      DISplay = true;
    }
    int mainpurp = 1;
     
    if (bfoOn)  {
      #ifdef IhaveCrystal
      currentBFOmanu = (encoderCount == 1) ? (currentBFOmanu + currentBFOStep) : (currentBFOmanu - currentBFOStep);
      mainpurp = 0;
      #endif
      }
    
    if (PRESbut) {     // FM preset
      currentPRES = (encoderCount == 1) ? (currentPRES + currentPRESStep) : (currentPRES - currentPRESStep);
      mainpurp = 0;
    }

    if (VOLbut) {     // Volume control
      currentVOL = (encoderCount == 1) ? (currentVOL + currentVOLStep) : (currentVOL - currentVOLStep);
      mainpurp = 0;
    }

    if (AGCgainbut) {     // AGC gain control
      currentAGCgain = (encoderCount == 1) ? (currentAGCgain + currentAGCgainStep) : (currentAGCgain - currentAGCgainStep);
      mainpurp = 0;
    }

    if (bright) {     // Brightness control
      currentBrightness = (encoderCount == 1) ? (currentBrightness - 20) : (currentBrightness + 20);
      mainpurp = 0;
    }

    if (mainpurp == 1)
    {
   
      if (currentMode == LSB || currentMode == USB || currentMode == CW) {
        if (encoderCount == 1) {              
          freqDec = freqDec - freqstep;
          int freqTot = (si4735.getFrequency() * 1000) + (freqDec * -1);
          if ( freqTot > (band[bandIdx].maximumFreq * 1000)) {
            si4735.setFrequency(band[bandIdx].maximumFreq);
            freqDec = 0;
          }  
          if (freqDec <= -16000)  {
            freqDec = freqDec + 16000;
            int freqPlus16 = currentFrequency + 16; //trunc(freqDec/1000);
            si4735.setHardwareAudioMute(1);
            AudioMut = true;
            elapsedAudMut = millis();
            si4735.setFrequency(freqPlus16);
          } 
          currentBFO = freqDec;
        } else {   
          freqDec = freqDec + freqstep;
          int freqTot = (si4735.getFrequency() * 1000) - freqDec;
          if ( freqTot < (band[bandIdx].minimumFreq * 1000)) {
            si4735.setFrequency(band[bandIdx].minimumFreq);
            freqDec = 0;
          }
          if (freqDec >= 16000)  {
            freqDec = freqDec - 16000;
            int freqMin16 = currentFrequency - 16;
            si4735.setHardwareAudioMute(1);
            AudioMut = true;
            elapsedAudMut = millis();
            si4735.setFrequency(freqMin16);
          }
          currentBFO = freqDec;
        }
      band[bandIdx].lastBFO = currentBFO; 
      } else  {
        if (encoderCount == 1) {
          si4735.frequencyUp();
        } else {
          si4735.frequencyDown();
        }
      }
      band[bandIdx].currentFreq = si4735.getFrequency();
      //band[bandIdx].lastBFO = currentBFO;
    }
    FreqDispl();
    encoderCount = 0;
  }
}

//=======================================================================================
void encoderButtonCheck()  {
//=======================================================================================
  //Encoder button

  int LONGPRESSTIMEOUT = 300;      // 500ms
  int LONGLONGPRESSTIMEOUT = 1500;   // 1500ms
  boolean longPress = false;
  boolean longLongPress = false;
  int pressTime = millis();

  int previousButton = (selectedMenu + lastButton - 1) % lastButton;
  int currentButton = (selectedMenu) % lastButton;
  int nextButton = (selectedMenu + 1) % lastButton;
  int selectedButton0;
  int selectedButton1;
  int selectedButton2;
  bool AGCstat;
          
  if (digitalRead(ENCODER_SWITCH) == LOW) {
    // Beep(1, 0);
    delay(300);
    int mainpurp = 1;
    if (DISplay == false) { //  Wake-up  Display
      DisplayOnTime = millis();
//      tft.writecommand(TFT_DISPON);
      ledcWrite(LedChannelforTFT, currentBrightness);
      DISplay = true;
      mainpurp = 0;
    }
    if (PRESbut == true) {// FM preset selection
      PRESbut = false;
      DrawDispl();
      mainpurp = 0;
    }

    if (bright) {// Brightness control exit
     bright = false;
     DrawDispl ();
     mainpurp = 0;
    }

    if (AGCgainbut == true) {// AGCgain Menu exit
      AGCgainbut = false;
      DrawDispl();
      mainpurp = 0;
    }

    if (mainpurp == 1) { 
      // Long press detection
      // If sort press, enable Volume or BFO
      // If long press, show first layer menu
      // If long long press, actually do a reset!
      while (digitalRead(ENCODER_SWITCH) == LOW) { 
        if ( millis() - pressTime > LONGPRESSTIMEOUT ) {
          if (FirstLayer and !longPress and !longLongPress) {

            selectedButton0 = selectedButton1 = selectedButton2 = 255;
          
            si4735.getAutomaticGainControl();
            AGCstat = si4735.isAgcEnabled();
          
            if (FirstLayer) {
              if (AGCstat) {
                if (previousButton == AGC) selectedButton0 = 0;
                if (currentButton  == AGC) selectedButton1 = 1;
                if (nextButton     == AGC) selectedButton2 = 2;
              }
              if (Mutestat) {
                if (previousButton == MUTE) selectedButton0 = 0;
                if (currentButton  == MUTE) selectedButton1 = 1;
                if (nextButton     == MUTE) selectedButton2 = 2;
              }
              if (RDS) {
                if (previousButton == RDSbut) selectedButton0 = 0;
                if (currentButton  == RDSbut) selectedButton1 = 1;
                if (nextButton     == RDSbut) selectedButton2 = 2;
              }
              if (bfoOn) {
                if (previousButton == BFO) selectedButton0 = 0;
                if (currentButton  == BFO) selectedButton1 = 1;
                if (nextButton     == BFO) selectedButton2 = 2;
              }
            }
            DrawMenu(bt[previousButton].ButtonNam, bt[currentButton].ButtonNam, bt[nextButton].ButtonNam, selectedButton0, selectedButton1, selectedButton2, 1);
          }
          longPress = true;
          if ( millis() - pressTime > LONGLONGPRESSTIMEOUT ) {
            if (!longLongPress) {
              showWarningMessage(" Reset in 500 ms");
            }
            longPress = false;
            longLongPress = true;
          }
        }
      }
      // End of long press detection

      if (FirstLayer) {    
        if ( longLongPress ){
          // Serial.println("[encoderButtonCheck] - Do long long press things");
          resetFunc();
        } else {
          if (longPress) {
            // Serial.println("[encoderButtonCheck] - Do long press things");
            DrawButFila();
            if (!pressed) {
              DrawSmeter();
              DrawVolumeIndicator();
              OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
            }

          } else {
            // Serial.println("[encoderButtonCheck] - Do regular press things");
            if (VOLbut)  {
              VOLbut = false;
              FreqDispl();
              while (digitalRead(ENCODER_SWITCH) == LOW) { 
              }
            } else {
              if (bfoOn == true) {
                bfoOn = false;
              } else {
                VOLbut = true;
              }
              FreqDispl();
            }  
          }
        }
      }
    }
    while (digitalRead(ENCODER_SWITCH) == LOW) { 
    } 
  }  
}

//=======================================================================================
void setStep()  {
//=======================================================================================
  // This command should work only for SSB mode
  if (bfoOn && (currentMode == LSB || currentMode == USB || currentMode == CW))
  {
    currentBFOStep = (currentBFOStep == 10) ? 10 : 25;
  }
  useBand();
  DrawDispl();
}

//=======================================================================================
void Beep(int cnt, int tlb) {
//=======================================================================================
  int tla = 100;
  for (int i = 0; i < cnt; i++) {
    digitalWrite(BEEPER, beepOn);
    delay(tla);
    digitalWrite(BEEPER, beepOff);
    delay(tlb);
  }
}

//=======================================================================================
void DrawFila()   {// Draw of first layer
//=======================================================================================
  FirstLayer = true;
  SecondLayer = false;
  tft.fillScreen(TFT_BLACK);
  DrawDispl();
  DrawBatteryLevel(currentBatteryLevel);
  batteryCharging = false;
  DrawSmeter();
  DrawVolumeIndicator();
  OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
}

/* //=======================================================================================
void DrawThla()  {  // Draw of Third layer
//=======================================================================================
  ThirdLayer = true;
  ForthLayer = false;
  tft.fillScreen(TFT_BLACK);
  DrawButThla();
  DrawDispl();
  DrawSmeter();
  DrawVolumeIndicator();
  OldRSSI = 2047;  // Force RSSI/SNR/Stereo-Mono/SMeter/Volume bar actualization
}
 */
//=======================================================================================
void DrawButFila() { // Buttons first layer
//=======================================================================================

  int previousButton = (selectedMenu + lastButton - 1) % lastButton;
  int currentButton = (selectedMenu) % lastButton;
  int nextButton = (selectedMenu + 1) % lastButton;
  int selectedButton0;
  int selectedButton1;
  int selectedButton2;
  bool AGCstat;

  selectedButton0 = selectedButton1 = selectedButton2 = 255;

  si4735.getAutomaticGainControl();
  AGCstat = si4735.isAgcEnabled();

  if (FirstLayer) {
    if (AGCstat) {
      if (previousButton == AGC) selectedButton0 = 0;
      if (currentButton  == AGC) selectedButton1 = 1;
      if (nextButton     == AGC) selectedButton2 = 2;
    }
    if (Mutestat) {
      if (previousButton == MUTE) selectedButton0 = 0;
      if (currentButton  == MUTE) selectedButton1 = 1;
      if (nextButton     == MUTE) selectedButton2 = 2;
    }
    if (RDS) {
      if (previousButton == RDSbut) selectedButton0 = 0;
      if (currentButton  == RDSbut) selectedButton1 = 1;
      if (nextButton     == RDSbut) selectedButton2 = 2;
    }
    if (bfoOn) {
      if (previousButton == BFO) selectedButton0 = 0;
      if (currentButton  == BFO) selectedButton1 = 1;
      if (nextButton     == BFO) selectedButton2 = 2;
    }
  }

  DrawMenu(bt[previousButton].ButtonNam, bt[currentButton].ButtonNam, bt[nextButton].ButtonNam, selectedButton0, selectedButton1, selectedButton2, 1);

  selectedButton0 = selectedButton1 = selectedButton2 = 255;

  // Wait for a selection or timeout
  // Read rotary encoder
  int selectMenuTimeout = 4000;
  int selectMenuTime = millis();
  while ( millis() - selectMenuTime < selectMenuTimeout ) {
    if ( encoderCount != 0 ) {
      selectedMenu = (encoderCount == 1) ? (selectedMenu + 1) : (selectedMenu - 1);
      if ( selectedMenu < 0 ) {
        selectedMenu = lastButton - 1;
      }

      previousButton = (selectedMenu + lastButton - 1) % lastButton;
      currentButton = (selectedMenu) % lastButton;
      nextButton = (selectedMenu + 1) % lastButton;

      selectedMenu = currentButton;

      if (FirstLayer) {
        if (AGCstat) {
          if (previousButton == AGC) selectedButton0 = 0;
          if (currentButton  == AGC) selectedButton1 = 1;
          if (nextButton     == AGC) selectedButton2 = 2;
        }
        if (Mutestat) {
          if (previousButton == MUTE) selectedButton0 = 0;
          if (currentButton  == MUTE) selectedButton1 = 1;
          if (nextButton     == MUTE) selectedButton2 = 2;
        }
        if (RDS) {
          if (previousButton == RDSbut) selectedButton0 = 0;
          if (currentButton  == RDSbut) selectedButton1 = 1;
          if (nextButton     == RDSbut) selectedButton2 = 2;
        }
        if (bfoOn) {
          if (previousButton == BFO) selectedButton0 = 0;
          if (currentButton  == BFO) selectedButton1 = 1;
          if (nextButton     == BFO) selectedButton2 = 2;
        }
      }

      DrawMenu(bt[previousButton].ButtonNam, bt[currentButton].ButtonNam, bt[nextButton].ButtonNam, selectedButton0, selectedButton1, selectedButton2, 1);

      selectedButton0 = selectedButton1 = selectedButton2 = 255;

      selectMenuTime = millis();
      encoderCount = 0;

    }
    if ( digitalRead(ENCODER_SWITCH) == LOW ) {
      pressed = true;
      delay(200);
      return;
    }

  }
  
}

//=======================================================================================
void DrawMenu(const char *buttonName0,const char *buttonName1,const char *buttonName2,int selectedButton0,int selectedButton1,int selectedButton2, int highlightButton) { // Draw a generic 3 Buttons Menu
//=======================================================================================

  spr.createSprite(Xbutsiz,Ybutsiz);
  spr.fillScreen(TFT_BLACK);
  spr.setTextColor(COLOR_BUTTON_TEXT);
  spr.setTextSize(2);
  spr.setTextDatum(BC_DATUM);
  spr.setTextPadding(0);

  if ( highlightButton == 0 || selectedButton0 == 0 ) {
    if ( highlightButton == 0 && selectedButton0 == 0 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red_Highlight);  
    }
    if ( highlightButton == 0 && selectedButton0 != 0 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue_Highlight);  
    }    
    if ( highlightButton != 0 && selectedButton0 == 0 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red);  
    }    
  } else {
    spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue);
  }
  spr.drawString(buttonName0,(Xbutsiz/2)+2, (Ybutsiz/2)+9);
  spr.pushSprite(Xbutsiz*0  + Xbutst, Ybutsiz*0 + Ybutst);

  if ( highlightButton == 1 || selectedButton1 == 1 ) {
    if ( highlightButton == 1 && selectedButton1 == 1 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red_Highlight);  
    }
    if ( highlightButton == 1 && selectedButton1 != 1 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue_Highlight);  
    }    
    if ( highlightButton != 1 && selectedButton1 == 1 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red);  
    }    
  } else {
    spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue);
  }
  spr.drawString(buttonName1,(Xbutsiz/2)+2, (Ybutsiz/2)+9);
  spr.pushSprite(Xbutsiz*1  + Xbutst, Ybutsiz*0 + Ybutst);

  if ( highlightButton == 2 || selectedButton2 == 2 ) {
    if ( highlightButton == 2 && selectedButton2 == 2 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red_Highlight);  
    }
    if ( highlightButton == 2 && selectedButton2 != 2 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue_Highlight);  
    }    
    if ( highlightButton != 2 && selectedButton2 == 2 ) {
      spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Red);  
    }    
  } else {
    spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue);
  }
  spr.drawString(buttonName2,(Xbutsiz/2)+2, (Ybutsiz/2)+9);
  spr.pushSprite(Xbutsiz*2  + Xbutst, Ybutsiz*0 + Ybutst);
  spr.deleteSprite();
  
}


/* //=======================================================================================
void DrawButThla() { // Buttons Third layer
//=======================================================================================
  spr.createSprite(Xbutsiz,Ybutsiz);
  for (int n = 0 ; n <= lastButton; n++) {
     spr.fillScreen(COLOR_BACKGROUND);
     spr.pushImage(0, 0, But_Width, But_Height, (uint16_t *)But_Blue);
     spr.setTextColor(COLOR_BUTTON_TEXT);
     spr.setTextSize(2);
     spr.setTextDatum(BC_DATUM);
     spr.setTextPadding(0);
     spr.drawString((bt[n].ButtonNam1),(Xbutsiz/2)+2, (Ybutsiz/2)+9);
     spr.pushSprite(bt[bt[n].ButtonNum1].XButos + Xbutst, bt[bt[n].ButtonNum1].YButos + Ybutst);
  }
     spr.deleteSprite();
}
 */

//=======================================================================================
void DrawVolumeIndicator()  {
//=======================================================================================
  tft.setTextSize(1);
  // tft.fillRect(XVolInd + borderThickness, YVolInd + borderThickness, 240 - (2 * borderThickness), 23 - (2 * borderThickness), TFT_BLACK);
  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.setCursor(XVolInd + borderThickness +  5, YVolInd + borderThickness + 55);
  // tft.print("Vol");
}

//=======================================================================================
void DrawSmeter()  {
//=======================================================================================
 String IStr;
//  tft.setFreeFont(&Serif_bold_10);
  tft.setTextSize(1);
  //tft.fillRect(Xsmtr, Ysmtr, 320, 53, TFT_VIOLET);
  tft.fillRect(Xsmtr + 3, Ysmtr + 40, 314, 16, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  for (int i = 0; i < 10; i++) {
    
    IStr = String(i);
    tft.setCursor((Xsmtr + 45 + (i * 10)), Ysmtr + 34);
    tft.print(i);
  }
  for (int i = 1; i < 7; i++) {
    
    IStr = String(i * 10);
    tft.setCursor((Xsmtr + 125 + (i * 14)), Ysmtr + 34);
    if ((i == 2) or (i == 4) or (i == 6))  {
      tft.print("+");
      tft.print(i * 10);

  tft.fillRect(Xsmtr + 41, Ysmtr + 43 , 186, 1, TFT_WHITE);
  tft.fillRect(Xsmtr + 41, Ysmtr + 56 , 186, 1, TFT_WHITE);   
   
    }
  }
  // tft.fillRect(Xsmtr + 50, Ysmtr + 32 , 112, 4, TFT_ORANGE);
  //tft.fillRect(Xsmtr + 162, Ysmtr + 32 , 100, 4, TFT_RED);
  tft.setFreeFont(NULL);
  // end Smeter
}


//=======================================================================================
void drawMUTE()  {
//=======================================================================================
   if (Mutestat) {
     si4735.setAudioMute(audioMuteOn);
   } else {
     si4735.setAudioMute(audioMuteOff);
   }
}

//=======================================================================================
void drawAGC()  {
//=======================================================================================
  si4735.getAutomaticGainControl();
}

//=======================================================================================
void drawKeyPath() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  spr.createSprite(But_Key_Width-6, But_Key_Height-5);
  spr.fillScreen(COLOR_BACKGROUND);

  for (int n = 0 ; n <= lastKPath; n++) {
    if ( n == 11) { // Send button is red
      spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Red);
    } else {
      spr.pushImage(0, 0, But_Key_Width, But_Key_Height, (uint16_t *)But_Key_Green);
    }

    spr.setTextColor(COLOR_BUTTON_TEXT);
    spr.setTextSize(2);
    spr.setTextDatum(BC_DATUM);
    spr.setTextPadding(0);
    spr.drawString((Keypathtext[kp[n].KeypNum]), (But_Key_Width/2)-2, (But_Key_Height/2)+9);
    spr.pushSprite((kp[n].Xkeypos + kp[n].Xkeypnr + 3), (kp[n].Ykeypos + kp[n].Ykeypnr  + 3));
  }
  spr.deleteSprite();
}

//=======================================================================================
void HamBandlist() {
//=======================================================================================

  int previousButton = (selectedHam + lastHam - 1) % lastHam;
  int currentButton = (selectedHam) % lastHam;
  int nextButton = (selectedHam + 1) % lastHam;

  DrawMenu(band[bn[previousButton].BandNum].bandName, band[bn[currentButton].BandNum].bandName, band[bn[nextButton].BandNum].bandName, 255, 255, 255, 1);

  // Wait for a selection or timeout
  // Read rotary encoder
  int selectHamTimeout = 4000;
  int selectHamTime = millis();
  while ( millis() - selectHamTime < selectHamTimeout ) {
    if ( encoderCount != 0 ) {
      selectedHam = (encoderCount == 1) ? (selectedHam + 1) : (selectedHam - 1);
      if ( selectedHam < 0 ) {
        selectedHam = lastHam - 1;
      }

      previousButton = (selectedHam + lastHam - 1) % lastHam;
      currentButton = (selectedHam) % lastHam;
      nextButton = (selectedHam + 1) % lastHam;

      selectedHam = currentButton;

      DrawMenu(band[bn[previousButton].BandNum].bandName, band[bn[currentButton].BandNum].bandName, band[bn[nextButton].BandNum].bandName, 255, 255, 255, 1);

      selectHamTime = millis();
      encoderCount = 0;

    }
    if ( digitalRead(ENCODER_SWITCH) == LOW ) {
      pressed = true;
      while (digitalRead(ENCODER_SWITCH) == LOW) {
        }
      delay(200);
      return;
    }
  }
}

//=======================================================================================
void BroadBandlist() {
//=======================================================================================
  int previousButton = (selectedBroad + lastBroad - 1) % lastBroad;
  int currentButton = (selectedBroad) % lastBroad;
  int nextButton = (selectedBroad + 1) % lastBroad;

  DrawMenu(band[bb[previousButton].BbandNum].bandName, band[bb[currentButton].BbandNum].bandName, band[bb[nextButton].BbandNum].bandName, 255, 255, 255, 1);

  // Wait for a selection or timeout
  // Read rotary encoder
  int selectBroadTimeout = 4000;
  int selectBroadTime = millis();
  while ( millis() - selectBroadTime < selectBroadTimeout ) {
    if ( encoderCount != 0 ) {
      selectedBroad = (encoderCount == 1) ? (selectedBroad + 1) : (selectedBroad - 1);
      if ( selectedBroad < 0 ) {
        selectedBroad = lastBroad - 1;
      }

      previousButton = (selectedBroad + lastBroad - 1) % lastBroad;
      currentButton = (selectedBroad) % lastBroad;
      nextButton = (selectedBroad + 1) % lastBroad;

      selectedBroad = currentButton;

      DrawMenu(band[bb[previousButton].BbandNum].bandName, band[bb[currentButton].BbandNum].bandName, band[bb[nextButton].BbandNum].bandName, 255, 255, 255, 1);

      selectBroadTime = millis();
      encoderCount = 0;

    }
    if ( digitalRead(ENCODER_SWITCH) == LOW ) {
      pressed = true;
      while (digitalRead(ENCODER_SWITCH) == LOW) {
        }
      delay(200);
      return;
    }

  }

}

//=======================================================================================
void Steplist() {
//=======================================================================================

  if ( currentMode != FM) {
    int previousButton = (selectedStep + lastStep - 1) % lastStep;
    int currentButton = (selectedStep) % lastStep;
    int nextButton = (selectedStep + 1) % lastStep;
  
    DrawMenu(stepsize[previousButton], stepsize[currentButton], stepsize[nextButton], 255, 255, 255, 1);
  
    // Wait for a selection or timeout
    // Read rotary encoder
    int selectStepTimeout = 4000;
    int selectStepTime = millis();
    while ( millis() - selectStepTime < selectStepTimeout ) {
      if ( encoderCount != 0 ) {
        selectedStep = (encoderCount == 1) ? (selectedStep + 1) : (selectedStep - 1);
        if ( selectedStep < 0 ) {
          selectedStep = lastStep - 1;
        }
  
        previousButton = (selectedStep + lastStep - 1) % lastStep;
        currentButton = (selectedStep) % lastStep;
        nextButton = (selectedStep + 1) % lastStep;
  
        selectedStep = currentButton;
  
        DrawMenu(stepsize[previousButton], stepsize[currentButton], stepsize[nextButton], 255, 255, 255, 1);
  
        selectStepTime = millis();
        encoderCount = 0;
  
      }
      if ( digitalRead(ENCODER_SWITCH) == LOW ) {
        pressed = true;
        while (digitalRead(ENCODER_SWITCH) == LOW) {
          }
        delay(200);
        return;
      }
  
     }
  } else {
      int previousButton = (selectedStepFM + lastStepFM - 1) % lastStepFM;
      int currentButton = (selectedStepFM) % lastStepFM;
      int nextButton = (selectedStepFM + 1) % lastStepFM;
    
      DrawMenu(stepsizeFM[previousButton], stepsizeFM[currentButton], stepsizeFM[nextButton], 255, 255, 255, 1);
    
      // Wait for a selection or timeout
      // Read rotary encoder
      int selectStepTimeout = 4000;
      int selectStepTime = millis();
      while ( millis() - selectStepTime < selectStepTimeout ) {
        if ( encoderCount != 0 ) {
          selectedStepFM = (encoderCount == 1) ? (selectedStepFM + 1) : (selectedStepFM - 1);
          if ( selectedStepFM < 0 ) {
            selectedStepFM = lastStepFM - 1;
          }
    
          previousButton = (selectedStepFM + lastStepFM - 1) % lastStepFM;
          currentButton = (selectedStepFM) % lastStepFM;
          nextButton = (selectedStepFM + 1) % lastStepFM;
    
          selectedStepFM = currentButton;
    
          DrawMenu(stepsizeFM[previousButton], stepsizeFM[currentButton], stepsizeFM[nextButton], 255, 255, 255, 1);
    
          selectStepTime = millis();
          encoderCount = 0;
    
        }
        if ( digitalRead(ENCODER_SWITCH) == LOW ) {
          pressed = true;
          while (digitalRead(ENCODER_SWITCH) == LOW) {
          }
          delay(200);
          return;
        }
    
      }
  }
}

//=======================================================================================
void Modelist() {
//=======================================================================================
  int previousButton = (selectedMode + lastMod - 1) % lastMod;
  int currentButton = (selectedMode) % lastMod;
  int nextButton = (selectedMode + 1) % lastMod;

  DrawMenu(bandModeDesc[md[previousButton].Modenum], bandModeDesc[md[currentButton].Modenum], bandModeDesc[md[nextButton].Modenum], 255, 255, 255, 1);

  // Wait for a selection or timeout
  // Read rotary encoder
  int selectModeTimeout = 4000;
  int selectModeTime = millis();
  while ( millis() - selectModeTime < selectModeTimeout ) {
    if ( encoderCount != 0 ) {
      selectedMode = (encoderCount == 1) ? (selectedMode + 1) : (selectedMode - 1);
      if ( selectedMode < 0 ) {
        selectedMode = lastMod - 1;
      }

      previousButton = (selectedMode + lastMod - 1) % lastMod;
      currentButton = (selectedMode) % lastMod;
      nextButton = (selectedMode + 1) % lastMod;

      selectedMode = currentButton;

      DrawMenu(bandModeDesc[md[previousButton].Modenum], bandModeDesc[md[currentButton].Modenum], bandModeDesc[md[nextButton].Modenum], 255, 255, 255, 1);

      selectModeTime = millis();
      encoderCount = 0;

    }
    if ( digitalRead(ENCODER_SWITCH) == LOW ) {
      pressed = true;
      while (digitalRead(ENCODER_SWITCH) == LOW) {
        }
      delay(200);
      return;
     }
 
  }

}

//=======================================================================================
void BWList()  {
//=======================================================================================
  if ( currentMode == AM) {

    int previousButton = (selectedBWAM + lastBWAM - 1) % lastBWAM;
    int currentButton = (selectedBWAM) % lastBWAM;
    int nextButton = (selectedBWAM + 1) % lastBWAM;
  
    DrawMenu(bandwidthAM[bw[previousButton].BandWidthAM], bandwidthAM[bw[currentButton].BandWidthAM], bandwidthAM[bw[nextButton].BandWidthAM], 255, 255, 255, 1);
  
    // Wait for a selection or timeout
    // Read rotary encoder
    int selectBWTimeout = 4000;
    int selectBWTime = millis();
    while ( millis() - selectBWTime < selectBWTimeout ) {
      if ( encoderCount != 0 ) {
        selectedBWAM = (encoderCount == 1) ? (selectedBWAM + 1) : (selectedBWAM - 1);
        if ( selectedBWAM < 0 ) {
          selectedBWAM = lastBWAM - 1;
        }
  
        previousButton = (selectedBWAM + lastBWAM - 1) % lastBWAM;
        currentButton = (selectedBWAM) % lastBWAM;
        nextButton = (selectedBWAM + 1) % lastBWAM;
  
        selectedBWAM = currentButton;
  
        DrawMenu(bandwidthAM[bw[previousButton].BandWidthAM], bandwidthAM[bw[currentButton].BandWidthAM], bandwidthAM[bw[nextButton].BandWidthAM], 255, 255, 255, 1);
  
        selectBWTime = millis();
        encoderCount = 0;
  
      }
      if ( digitalRead(ENCODER_SWITCH) == LOW ) {
        pressed = true;
        while (digitalRead(ENCODER_SWITCH) == LOW) {
          }
        delay(200);
        return;
      }
  
    }
  }

  if ( currentMode == FM) {

    int previousButton = (selectedBWFM + lastBWFM - 1) % lastBWFM;
    int currentButton = (selectedBWFM) % lastBWFM;
    int nextButton = (selectedBWFM + 1) % lastBWFM;
  
    DrawMenu(bandwidthFM[bw[previousButton].BandWidthFM], bandwidthFM[bw[currentButton].BandWidthFM], bandwidthFM[bw[nextButton].BandWidthFM], 255, 255, 255, 1);
  
    // Wait for a selection or timeout
    // Read rotary encoder
    int selectBWTimeout = 4000;
    int selectBWTime = millis();
    while ( millis() - selectBWTime < selectBWTimeout ) {
      if ( encoderCount != 0 ) {
        selectedBWFM = (encoderCount == 1) ? (selectedBWFM + 1) : (selectedBWFM - 1);
        if ( selectedBWFM < 0 ) {
          selectedBWFM = lastBWFM - 1;
        }
  
        previousButton = (selectedBWFM + lastBWFM - 1) % lastBWFM;
        currentButton = (selectedBWFM) % lastBWFM;
        nextButton = (selectedBWFM + 1) % lastBWFM;
  
        selectedBWFM = currentButton;
  
        DrawMenu(bandwidthFM[bw[previousButton].BandWidthFM], bandwidthFM[bw[currentButton].BandWidthFM], bandwidthFM[bw[nextButton].BandWidthFM], 255, 255, 255, 1);
  
        selectBWTime = millis();
        encoderCount = 0;
  
      }
      if ( digitalRead(ENCODER_SWITCH) == LOW ) {
        pressed = true;
        while (digitalRead(ENCODER_SWITCH) == LOW) {
          }
        delay(200);
        return;
      }
  
    }
  }

  if ((currentMode == LSB) or ( currentMode == USB) or ( CWShift == true)) {

    int previousButton = (selectedBWSSB + lastBWSSB - 1) % lastBWSSB;
    int currentButton = (selectedBWSSB) % lastBWSSB;
    int nextButton = (selectedBWSSB + 1) % lastBWSSB;
  
    DrawMenu(bandwidthSSB[bw[previousButton].BandWidthSSB], bandwidthSSB[bw[currentButton].BandWidthSSB], bandwidthSSB[bw[nextButton].BandWidthSSB], 255, 255, 255, 1);
  
    // Wait for a selection or timeout
    // Read rotary encoder
    int selectBWTimeout = 4000;
    int selectBWTime = millis();
    while ( millis() - selectBWTime < selectBWTimeout ) {
      if ( encoderCount != 0 ) {
        selectedBWSSB = (encoderCount == 1) ? (selectedBWSSB + 1) : (selectedBWSSB - 1);
        if ( selectedBWSSB < 0 ) {
          selectedBWSSB = lastBWSSB - 1;
        }
  
        previousButton = (selectedBWSSB + lastBWSSB - 1) % lastBWSSB;
        currentButton = (selectedBWSSB) % lastBWSSB;
        nextButton = (selectedBWSSB + 1) % lastBWSSB;
  
        selectedBWSSB = currentButton;
  
        DrawMenu(bandwidthSSB[bw[previousButton].BandWidthSSB], bandwidthSSB[bw[currentButton].BandWidthSSB], bandwidthSSB[bw[nextButton].BandWidthSSB], 255, 255, 255, 1);
  
        selectBWTime = millis();
        encoderCount = 0;
  
      }
      if ( digitalRead(ENCODER_SWITCH) == LOW ) {
        pressed = true;
        while (digitalRead(ENCODER_SWITCH) == LOW) {
          }
        delay(200);
        return;
      }
 
    }

  }

}


//=======================================================================================
void subrstatus() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("SW Ver: V4.1.1 Jan-15-2023 (p. 1 of 2)",5,10);
  tft.drawString("BY RICARDO - GERT - RXAVIER - IU4ALH",5, 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Mod.     : " + String(bandModeDesc[band[bandIdx].prefmod]), 5, 30);
  if ( currentMode != FM)  tft.drawString("Freq.    : " + String(currentFrequency, 0) + " KHz", 5, 40);
  else tft.drawString("Freq.    : " + String(currentFrequency / 100, 1) + " MHz", 5, 40);
  si4735.getCurrentReceivedSignalQuality();
  tft.drawString("RSSI/SNR : " + String(si4735.getCurrentRSSI()) + "dBuV", 5, 50); // si4735.getCurrentSNR()
  tft.drawString(" / " + String(si4735.getCurrentSNR()) + "uV", 105, 50);
  if (  currentMode == FM ) {
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    tft.drawString(" / " + String(buffer), 165, 50);
  }
  si4735.getAutomaticGainControl();
  si4735.getCurrentReceivedSignalQuality();
  tft.drawString("LNA GAIN index: " + String(si4735.getAgcGainIndex()) + "/" + String(currentAGCAtt), 5, 60);
  tft.drawString("Volume   : )" + String(si4735.getVolume()), 5, 70);
  sprintf(buffer, "%s", (si4735.isAgcEnabled()) ? "AGC ON " : "AGC OFF");
  tft.drawString(buffer, 5, 80);
  if (bfoOn) tft.drawString("BFO ON  ", 5,90);
  else tft.drawString("BFO OFF ", 5,90);
  tft.drawString("AVC max GAIN  : " + String(si4735.getCurrentAvcAmMaxGain()), 5, 100);
  tft.drawString("Ant. Cap = " + String(si4735.getAntennaTuningCapacitor()) , 5, 110);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Press Button for the next page", 5, 120);
  presStat = true;
  while (presStat) {
      if (digitalRead(ENCODER_SWITCH) == LOW) {
        presStat = false;
        while (digitalRead(ENCODER_SWITCH) == LOW) { 
        }
        delay(200);
      }
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("SW Ver: V4.1.1 Jan-15-2023 (p. 2 of 2)",5,10);
  tft.drawString("BY RICARDO - GERT - RXAVIER - IU4ALH",5, 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Band          :  " + String(bandIdx) + "  " + String(band[bandIdx].bandName) , 5, 30);
  tft.drawString("Bandwidth SSB : " + String(bandwidthSSB[bwIdxSSB]) + " KHz", 5, 40);
  tft.drawString("Bandwidth AM  : " + String(bandwidthAM[bwIdxAM]) + " KHz", 5, 50);
  tft.drawString("Bandwidth FM  : " + String(bandwidthFM[bwIdxFM]) + " KHz", 5, 60);
  tft.drawString("Stepsize  MW  :   " + String(ssIdxMW) + " KHz", 5, 70);
  tft.drawString("Stepsize  AM  :   " + String(ssIdxAM) + " KHz", 5, 80);
  tft.drawString("Stepsize SSB  :   " "1 KHz fixed", 5, 90);
  tft.drawString("Stepsize  FM  : " + String(ssIdxFM * 10) + " KHz", 5, 100);
  // int vsupply;
  // vsupply = analogRead(VBAT_MON);
  // tft.drawString("Power Supply  : " + String(((1.66 / 1850)*vsupply) * 2) + " V.", 5, 110);
  tft.drawString("Power Supply  : " + String(battery.getBatteryVolts()) + " V.", 5, 110);

  delay(400);
  presStat = true;
  while (presStat) {
      if (digitalRead(ENCODER_SWITCH) == LOW) {
        presStat = false;
        while (digitalRead(ENCODER_SWITCH) == LOW) { 
        }
        delay(200);
      }
  }
  x = y = 0;
  Beep(1, 0);
  // delay(400);
}

//=======================================================================================
void showRDSStation() {
//=======================================================================================
  if ((FirstLayer) or (ThirdLayer)) {
    tft.setCursor(XFreqDispl + 75, YFreqDispl + 70);
    tft.print(stationName);
  }
  delay(250);
}


//=======================================================================================
void showrdsMsg() {
//=======================================================================================
  if ((FirstLayer) or (ThirdLayer)) {
 if (PRESbut) tft.setTextColor(TFT_ORANGE, TFT_BLACK); else tft.setTextColor(TFT_ORANGE, TFT_BLACK);																								   
 //   tft.setCursor(0, YFreqDispl + 75); //sulle coord di DSP radio...
 //   tft.print(rdsMsg);
   showrdsmsg.pushSprite(0, YFreqDispl + 92);  // posizione riga scrollrds
  showrdsmsg.scroll(-14, 0);     // -8 scroll stext 1 pixel left, up/down default is 0

  tcount--;
  if (tcount <=0){ 
    tcount = 50; // 90  - 120  velocity repeat sprite rds
    showrdsmsg.setTextSize(2);
    showrdsmsg.setTextColor(TFT_WHITE);
    showrdsmsg.drawString(rdsMsg, XFreqDispl + 310, 0, 1); // draw at 240,0 in sprite, font 2
   }			   		  
  }
//  delay(100);
}

// =====================================
void showRDSTime() {
// =======================================
if ((FirstLayer) or (ThirdLayer)) {
    tft.setTextSize(2);  
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(BC_DATUM);
    tft.setCursor(253, YFreqDispl + 52); 
    calcRDSTime();
    tft.print(rdsTime);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(BC_DATUM);
    tft.setCursor(254, YFreqDispl + 32);
    tft.print("Time:");
       delay(100);
  }
 }

// =====================================
void calcRDSTime() {
  // =======================================
  if (strlen(rdsTime) > 10) {
    int gmtHour = 0;
    int gmtMinute = 0;
    int gmtHourOffset = 0;
    int gmtMinuteOffset = 0;
    gmtHour = gmtHour + ((rdsTime[0] - 48) * 10);
    gmtHour = gmtHour + (rdsTime[1] - 48);
    gmtMinute = gmtMinute + ((rdsTime[3] - 48) * 10);
    gmtMinute = gmtMinute + (rdsTime[4] - 48);
    gmtHourOffset = gmtHourOffset + ((rdsTime[7] - 48) * 10);
    gmtHourOffset = gmtHourOffset + (rdsTime[8] - 48);
    gmtMinuteOffset = gmtMinuteOffset + ((rdsTime[10] - 48) * 10);
    gmtMinuteOffset = gmtMinuteOffset + (rdsTime[11] - 48);
    if (rdsTime[6] == '-') {
      gmtHour = gmtHour + gmtHourOffset;
      if (gmtHour > 23) gmtHour = gmtHour - 24;
      gmtMinute = gmtMinute + gmtMinuteOffset;
    } else {
      gmtHour = gmtHour - gmtHourOffset;
      if (gmtHour < 0) gmtHour = gmtHour + 24;
      gmtMinute = gmtMinute - gmtMinuteOffset;
    }
    rdsTime[0] = (trunc(gmtHour / 10)) + 48;
    rdsTime[1] = gmtHour - (trunc(gmtHour / 10) * 10) + 48;
    rdsTime[3] = (trunc(gmtMinute / 10)) + 48;
    rdsTime[4] = gmtMinute - (trunc(gmtMinute / 10) * 10) + 48;
    rdsTime[5] = '\0';
  }
}






//=======================================================================================
void checkRDS() {
//=======================================================================================
  si4735.getRdsStatus();
  if (si4735.getRdsReceived()) {
    if (si4735.getRdsSync() && si4735.getRdsSyncFound() ) {
      stationName = si4735.getRdsText0A(); // nome stazione
      rdsMsg = si4735.getRdsText2A(); // testo rds news
      rdsTime = si4735.getRdsTime(); // orario
      tft.setTextSize(2);
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setTextDatum(BC_DATUM);
      if ( stationName != NULL )   showRDSStation();
	  
	    if ( rdsMsg != NULL )   showrdsMsg(); 

      tft.setTextSize(1);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setTextDatum(BC_DATUM);
      if ( rdsTime != NULL )   showRDSTime();

    }
  }
}

//=======================================================================================
void FreqDispl() {
//=======================================================================================
  if (!PRESbut) {
  if ((FirstLayer) or (ThirdLayer)) {
    currentFrequency = si4735.getFrequency();
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    // tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 , 228, 48, TFT_BLACK); // Black freq. field
    // Quick and dirty solution for battery icon 15x29
    tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 , 228-9, 10, TFT_BLACK); // Black freq. field
    tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 +10, 228, 48-10, TFT_BLACK); // Black freq. field
    // End - Quick and dirty solution for battery icon 15x29
    AGCfreqdisp();
    BFOStepdisp();
    tft.setTextSize(4);
    tft.setTextDatum(BC_DATUM);
    if ((VOLbut) or (AGCgainbut) or (bright)) {
      if (VOLbut) {
        tft.setTextSize(3);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.drawString(String(map(currentVOL, 20, 63, 0, 100)), XFreqDispl + 60, YFreqDispl + 53);
        tft.setTextSize(2);
        tft.drawString( "Volume", XFreqDispl + 160, YFreqDispl + 53);
      }
      if (AGCgainbut) {
        tft.setTextSize(2);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.drawString(String(currentAGCgain), XFreqDispl + 40, YFreqDispl + 53);
        tft.setTextSize(2);
        tft.drawString("RF Attenuation", XFreqDispl + 150, YFreqDispl + 53);
      }
      if (bright) {
        tft.setTextSize(3);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.drawString(String(map(currentBrightness, 0, 270, 0, 100)), XFreqDispl + 60, YFreqDispl + 53);
        tft.setTextSize(2);
        tft.drawString( "Brightness", XFreqDispl + 160, YFreqDispl + 53);
      }

    } else {
      if ((band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) and (bfoOn == false)) {
          Displayfreq =  currentFrequency;
          tft.setTextSize(1);
          tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
          tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
          tft.drawString(String(Displayfreq, 0), XFreqDispl + 120, YFreqDispl + 61);
          tft.setTextSize(2);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          tft.setFreeFont(NULL);      
          tft.drawString("KHz", XFreqDispl + 215, YFreqDispl + 61);
      /*  } */

      }
      if (band[bandIdx].bandType == FM_BAND_TYPE) {
        Displayfreq =  currentFrequency / 100;
        tft.setTextSize(1);
        tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
        tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
        tft.drawString(String(Displayfreq, 2), XFreqDispl + 120, YFreqDispl + 63);
        tft.setTextSize(2);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.setFreeFont(NULL); 
        tft.drawString("MHz", XFreqDispl + 225, YFreqDispl + 54);
      }
      if ((currentMode == AM) and (band[bandIdx].bandType != MW_BAND_TYPE) and (band[bandIdx].bandType != LW_BAND_TYPE)) {
        Displayfreq =  currentFrequency / 1000;
        tft.setTextSize(1);
        tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
        tft.setTextDatum(BC_DATUM);
        tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
        tft.drawString(String(Displayfreq, 3), XFreqDispl + 120, YFreqDispl + 61);
        tft.setTextSize(2);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.setFreeFont(NULL); 
        tft.drawString("MHz", XFreqDispl + 225, YFreqDispl + 61);
      }
      if (currentMode == LSB || currentMode == USB  || currentMode == CW ) {
        if (bfoOn) {
          tft.setTextSize(1);
          tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
          tft.setTextDatum(BR_DATUM);
          tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
          #ifdef IhaveCrystal
            tft.drawString(String(currentBFOmanu), XFreqDispl + 180, YFreqDispl + 61);
          #endif
          tft.setFreeFont(NULL);
          tft.setTextSize(2);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          tft.drawString("Hz", XFreqDispl + 229, YFreqDispl + 84);
          tft.setTextDatum(BC_DATUM);   
        }
        else {
          // tft.fillRect( XFreqDispl + 6, YFreqDispl + 26 , 228, 45, TFT_BLACK); // Black freq. field
          // Quick and dirty solution for battery icon 15x29
          tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 , 228-9, 10, TFT_BLACK); // Black freq. field
          tft.fillRect( XFreqDispl + 6, YFreqDispl + 19 +10, 228, 45-10, TFT_BLACK); // Black freq. field
          // End - Quick and dirty solution for battery icon 15x29
          Displayfreq = (currentFrequency*1000) - (band[bandIdx].lastBFO);
          if (CWShift) Displayfreq = Displayfreq + 700;
          int mhz = trunc(Displayfreq/1000000);
          int khz = Displayfreq-(mhz * 1000000);
          khz = trunc(khz/1000);
          int hz = Displayfreq-(mhz * 1000000)- (khz * 1000);
          char s[12] = {'\0'};
          sprintf(s,"%i.%03i.%02i",mhz,khz,hz/10);
          //tft.setTextSize(2);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          tft.setFreeFont(NULL); 
          tft.setTextSize(1);
          tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
          tft.setTextDatum(BR_DATUM);
          tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
          tft.drawString(String(s), XFreqDispl + 220, YFreqDispl + 61);
          tft.setTextSize(2);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          tft.setFreeFont(NULL); 
          tft.drawString("MHz", XFreqDispl + 229, YFreqDispl + 84);
          if (freqstepnr == 0)  tft.fillRect(XFreqDispl + 140, YFreqDispl + 62, 20, 5, TFT_ORANGE);
          if (freqstepnr == 1)  tft.fillRect(XFreqDispl + 170, YFreqDispl + 62, 20, 5, TFT_ORANGE);
          if (freqstepnr == 2)  tft.fillRect(XFreqDispl + 198, YFreqDispl + 62, 20, 5, TFT_ORANGE);
          tft.setTextDatum(BC_DATUM);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        }
      }
    }

    drawFreqScale();

  }
  }
}

/*
 * Draw a frequency scale.  
 */
void drawFreqScale() {
// #################### Frequency Scale #####################

  int XFreqScalesiz=320;  //
  int YFreqScalesiz=58;  //
  uint8_t xPosScale=0;   // Initial x position of frequency scale
  uint8_t yPosScale=0; // Initial y position of frequency scale

  freqScale.createSprite(XFreqScalesiz,YFreqScalesiz);
  freqScale.fillScreen(TFT_BLACK);
  freqScale.setTextColor(TFT_WHITE);
  freqScale.setTextSize(1);
  freqScale.setTextDatum(MC_DATUM);
  freqScale.setTextPadding(0);

  freqScale.fillTriangle(xPosScale+156,yPosScale,xPosScale+160,yPosScale+10,xPosScale+164,yPosScale,TFT_RED);
  freqScale.drawLine(xPosScale+160,yPosScale+2,xPosScale+160,yPosScale+58,TFT_RED);

  int temp=(currentFrequency/10.00)-20;
  for(int i=0;i<40;i++)
  {
    if (!(temp<band[bandIdx].minimumFreq/10.00 or temp>band[bandIdx].maximumFreq/10.00)) {
      if((temp%10)==0){
        freqScale.drawLine(xPosScale+(i*8),yPosScale+58,xPosScale+(i*8),yPosScale+28,0xC638);
        
        freqScale.drawLine(xPosScale+(i*8)+1,yPosScale+58,xPosScale+(i*8)+1,yPosScale+28,0xC638);
        if (currentMode == FM) freqScale.drawFloat(temp/10.0,1,xPosScale+(i*8),yPosScale+18,2);
        else if (temp >= 100) freqScale.drawFloat(temp/100.0,3,xPosScale+(i*8),yPosScale+18,2);
               else freqScale.drawNumber(temp*10,xPosScale+(i*8),yPosScale+18,2);
      } else if((temp%5)==0 && (temp%10)!=0) {
        freqScale.drawLine(xPosScale+(i*8),yPosScale+58,xPosScale+(i*8),yPosScale+38,0xC638);
        freqScale.drawLine(xPosScale+(i*8)+1,yPosScale+58,xPosScale+(i*8)+1,yPosScale+38,0xC638);
        // tft.drawFloat(temp/10.0,1,i*8,144);
      } else {
        freqScale.drawLine(xPosScale+(i*8),yPosScale+58,xPosScale+(i*8),yPosScale+48,0xC638);
      }
    }
  
   temp=temp+1;
  }
  freqScale.pushSprite(0,140);
  freqScale.deleteSprite();
// #################### Frequency Scale #####################
}


/*
 * Checks the stop seeking criterias.  
 * Returns true if the user press the touch or rotates the encoder. 
 */
bool checkStopSeeking() {
  // Checks the encoder
  return (bool) encoderCount || (digitalRead(ENCODER_SWITCH) == LOW);   // returns true if the user rotates the encoder
} 

//=======================================================================================
void SeekFreq (uint16_t freq)  {
//=======================================================================================
  if ((FirstLayer) or (ThirdLayer))  {
    currentFrequency = freq;
    tft.setTextSize(4);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    tft.fillRect( XFreqDispl + 6, YFreqDispl + 28 , 228, 32, TFT_BLACK); // Black freq. field
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
      Displayfreq =  currentFrequency;
      tft.setTextSize(1);
      tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
      tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
      tft.drawString(String(Displayfreq, 0), XFreqDispl + 120, YFreqDispl + 61);
      tft.setTextSize(2);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setFreeFont(NULL);      
      tft.drawString("KHz", XFreqDispl + 215, YFreqDispl + 61);
    }
    if (band[bandIdx].bandType == FM_BAND_TYPE) {
      Displayfreq =  currentFrequency / 100;
      tft.setTextSize(1);
      tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
      tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
      tft.drawString(String(Displayfreq, 2), XFreqDispl + 120, YFreqDispl + 62);
      tft.setTextSize(2);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setFreeFont(NULL); 
      tft.drawString("MHz", XFreqDispl + 225, YFreqDispl + 54);
    }
    if (band[bandIdx].bandType == SW_BAND_TYPE)  {   
      Displayfreq =  currentFrequency / 1000;
      tft.setTextSize(1);
      tft.setTextColor(COLOR_INDICATOR_FREQ, COLOR_BACKGROUND);
      tft.setTextDatum(BC_DATUM);
      tft.setFreeFont(&DSEG7_Classic_Mini_Regular_38);
      tft.drawString(String(Displayfreq, 3), XFreqDispl + 120, YFreqDispl + 61);
      tft.setTextSize(2);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setFreeFont(NULL); 
      tft.drawString("MHz", XFreqDispl + 225, YFreqDispl + 61);
    }
    showtimeRSSI();

  }
}

//=======================================================================================
void DrawDispl() {
//=======================================================================================
  if ( borderThickness ) {
    tft.fillRect(XFreqDispl, YFreqDispl, 240, 88, TFT_GREY);
    tft.fillRect(XFreqDispl + borderThickness, YFreqDispl + borderThickness, 240 - 2*borderThickness, 88 - 2*borderThickness, TFT_BLACK);
  }
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(tft.textWidth("BACON"));
  tft.drawString(band[bandIdx].bandName, XFreqDispl + 260, YFreqDispl + 18);
  tft.setTextPadding(0);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Band", XFreqDispl + 260, YFreqDispl + 8);

  FreqDispl();
  //if (band[bandIdx].bandType != FM_BAND_TYPE) {
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Mod.", XFreqDispl + 180, YFreqDispl + 8);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    Modtext = bandModeDesc[currentMode];
    if ((Modtext == "USB") and (CWShift == true)){
      Modtext = "CW";
    }else {
      Modtext = bandModeDesc[currentMode];
    }
    tft.drawString(Modtext, XFreqDispl + 180, YFreqDispl + 18);
    tft.setTextPadding(tft.textWidth("2.2kHz"));
    if (currentMode == AM) BWtext = bandwidthAM[bwIdxAM];
    if (currentMode == LSB || currentMode == USB || currentMode == CW) BWtext = bandwidthSSB[bwIdxSSB];
    if (currentMode == FM){
      //tft.setTextPadding(tft.textWidth("100 kHz")); 
      BWtext = (bandwidthFM[bwIdxFM]);
    }
    if (BWtext == "AUT") {
      tft.setTextPadding(tft.textWidth("8.8 KHz"));
      tft.drawString(BWtext, XFreqDispl + 220, YFreqDispl + 18);
      tft.setTextPadding(0);
    }else tft.drawString(BWtext + " KHz", XFreqDispl + 220, YFreqDispl + 18);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Width", XFreqDispl + 220, YFreqDispl + 8);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    if (currentMode == FM){
      tft.drawString(String((band[bandIdx].currentStep)*10) + " KHz", XFreqDispl + 300, YFreqDispl + 18);
    } else  tft.drawString(String(band[bandIdx].currentStep) + " KHz", XFreqDispl + 300, YFreqDispl + 18);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Step", XFreqDispl + 300, YFreqDispl + 8);
}

//=======================================================================================
void AGCfreqdisp() {
//=======================================================================================

  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  tft.drawString("AGC", XFreqDispl + 150, YFreqDispl + 8);//16
  tft.drawString("    ", XFreqDispl + 150, YFreqDispl + 18);
  si4735.getAutomaticGainControl();
  if (si4735.isAgcEnabled()) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("On", XFreqDispl + 150, YFreqDispl + 18);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else {
    if (AGCgain == 0)   {
      tft.drawString("Off", XFreqDispl + 150, YFreqDispl + 18);
    } else {
      tft.drawString(String(currentAGCgain), XFreqDispl + 150, YFreqDispl + 18);
    }
  }
}


//=======================================================================================
void BFOStepdisp() {
//=======================================================================================
  if (band[bandIdx].bandType != FM_BAND_TYPE) {
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(tft.textWidth("XXX"));
    tft.drawString("BFO", XFreqDispl + 120, YFreqDispl + 8);
    tft.setTextPadding(tft.textWidth("88"));

    if (bfoOn) {
      #ifdef IhaveCrystal
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawString(String(currentBFOStep), XFreqDispl + 120, YFreqDispl + 18);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      #endif
    } else {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.drawString("  ", XFreqDispl + 120, YFreqDispl + 26);
    }
  }
}

//=======================================================================================
void ErrorBeep()  {
//=======================================================================================
  Beep(4, 100);
  delay(2000);
}





//=======================================================================================
void MuteAud() {
//=======================================================================================
  // Stop muting only if this condition has changed
  if (((millis() - elapsedAudMut) > MIN_ELAPSED_AudMut_TIME ) and (AudioMut = true))// 150 msec
  {
    AudioMut = false;
    si4735.setHardwareAudioMute(0);
  }
}

//=======================================================================================
void showFirmwareInformation() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Firmware Information");
  tft.println("====================");
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Part Number (HEX)........: ");
  tft.println(si4735.getFirmwarePN(), HEX);
  tft.print("Firmware Major Revision..: ");
  tft.println(si4735.getFirmwareFWMAJOR());
  tft.print("Firmware Minor Revision..: ");
  tft.println(si4735.getFirmwareFWMINOR());
  tft.print("Patch ID ................: ");
  tft.print(si4735.getFirmwarePATCHH(), HEX);
  tft.println(si4735.getFirmwarePATCHL(), HEX);
  tft.print("Component Major Revision.: ");
  tft.println(si4735.getFirmwareCMPMAJOR());
  tft.print("Component Minor Revision.: ");
  tft.println(si4735.getFirmwareCMPMINOR());
  tft.print("Chip Revision............: ");
  tft.println(si4735.getFirmwareCHIPREV());
  
  presStat = true;
  while (presStat) {
      if (digitalRead(ENCODER_SWITCH) == LOW) {
        presStat = false;
        while (digitalRead(ENCODER_SWITCH) == LOW) {
        }
        delay(200);
      }
  }
  // x = y = 0;
  // Beep(1, 0);
  // delay(400);
}

//=======================================================================================
void showWarningMessage(const char *warningMessage) {
//=======================================================================================
  int warningTxtTimeout = 3500;
  int warningTxtTime = millis();
  bool warning = true;
  tft.setTextSize(2);
  // tft.fillRect(Xsmtr, Ysmtr, 240, 55, TFT_GREY);
  tft.fillRect(Xsmtr + borderThickness, Ysmtr + borderThickness, 240 - (2 * borderThickness), 45 - (2 * borderThickness), TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(Xsmtr + borderThickness +  5, Ysmtr + borderThickness + 20);
  tft.print(warningMessage);
  
  // Wait for rotary encoder, button ou timeout
  while ( warning ) { 
    if ( encoderCount != 0 ) warning = false;
    if ( digitalRead(ENCODER_SWITCH) == LOW ) {
      warning = false;
      delay(200);
    }
    if ( millis() - warningTxtTime > warningTxtTimeout ) warning = false;
  }
  encoderCount = 0;
}

//=======================================================================================
void DrawBatteryLevel(int batteryLevel) { // Draw battery level icon
//=======================================================================================

  spr.createSprite(Xbatsiz,Ybatsiz);
  // spr.fillScreen(TFT_BLACK);

  if ( batteryLevel == 1 ) {
    spr.pushImage(0, 0, BatteryLevel1_15x29Width, BatteryLevel1_15x29Height, (uint16_t *)BatteryLevel1_15x29);
  }
  if ( batteryLevel == 2 ) {
    spr.pushImage(0, 0, BatteryLevel2_15x29Width, BatteryLevel2_15x29Height, (uint16_t *)BatteryLevel2_15x29);
  }
  if ( batteryLevel == 3 ) {
    spr.pushImage(0, 0, BatteryLevel3_15x29Width, BatteryLevel3_15x29Height, (uint16_t *)BatteryLevel3_15x29);
  }
  if ( batteryLevel == 4 ) {
    spr.pushImage(0, 0, BatteryLevel4_15x29Width, BatteryLevel4_15x29Height, (uint16_t *)BatteryLevel4_15x29);
  }
  if ( batteryLevel == 5 ) {
    spr.pushImage(0, 0, BatteryCharging_15x29Width, BatteryCharging_15x29Height, (uint16_t *)BatteryCharging_15x29);
  }

  spr.pushSprite(315 - Xbatsiz, 110 - Ybatsiz*0);
  spr.deleteSprite();

}

void batteryMonitor() {
  if(battery.getBatteryVolts() >= MIN_USB_VOLTAGE){
    if (!batteryCharging) {
      batteryCharging = true;
      DrawBatteryLevel(5);
    }
  } else {
      batteryCharging = false;
      int batteryLevel = battery.getBatteryChargeLevel();
      if(batteryLevel >=80){
        currentBatteryLevel = 4;
      }else if(batteryLevel < 80 && batteryLevel >= 50 ){
        currentBatteryLevel = 3;
      }else if(batteryLevel < 50 && batteryLevel >= 20 ){
        currentBatteryLevel = 2;
      }else if(batteryLevel < 20 ){
        currentBatteryLevel = 1;
      }  
      if (currentBatteryLevel != previousBatteryLevel){
      DrawBatteryLevel(currentBatteryLevel);
      previousBatteryLevel = currentBatteryLevel;
      }
  }
}
