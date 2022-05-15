// The sketches and documentation available here are based on Gert's sketch that made his work available for avery one.
// I would like to thank to [Gert Baak](https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino).
//
//  Thi sketch was made  by
//  V3.0 27-07-2020 Bug-support and little improvements.
//  This sketch is based on the SI4735 Library of Ricardo PU2CLR. Thanks for the very nice work.
//  This sketch uses  a 2.8 inch 240*320 touch-screen with ILI9341, ESP32 WROOM-32 and Rotary Encoder.
//  The radio is fully controlled by the (Touch)Screen and Rotary Encoder
//  This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
//  together with this sketch).
//  For the touch-screen the library TFT_eSPI is used. The configuration setup-file: setup1_ILI9341 is also
//  included.
//  Also a schematic drawing is available.
//  ABOUT SSB PATCH:
//  This sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 8KB of the Arduino memory.
//  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
//  There is little information available about patching the SI4735. The following information is the understanding of the author of
//  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
//  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
//  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by
//  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
//  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.
//
//  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
//  Given this, it is at your own risk to continue with the procedures suggested here.
//  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
//  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.
//
//  Library TFT_eSPI you may download from here : https://github.com/Bodmer/TFT_eSPI
//  Library Rotary is provided with the program
//  Library SI4735 you may download from here   : https://github.com/pu2clr/SI4735
//
//  *********************************
//  **   Display connections etc.  **
//  *********************************
//  |------------|------------------|------------|------------|------------|
//  |Display 2.8 |      ESP32       |   Si4735   |  Encoder   |  Beeper    |
//  |  ILI9341   |                  |            |            |            |        Encoder        1,2,3
//  |------------|------------------|------------|------------|------------|        Encoder switch 4,5
//  |   Vcc      |     3V3     | 01 |    Vcc     |            |            |        pin 33 with 18K to 3.3 volt and 18K to ground.
//  |   GND      |     GND     | 02 |    GND     |     2,4    |            |        pin 32 (Beeper) via 2K to base V1  BC547
//  |   CS       |     15      | 03 |            |            |            |        Collector via beeper to 5v
//  |   Reset    |      4      | 04 |            |            |            |        Emmitor to ground
//  |   D/C      |      2      | 05 |            |            |            |
//  |   SDI      |     23      | 06 |            |            |            |        Encoder        1,2,3
//  |   SCK      |     18      | 07 |            |            |            |        Encoder switch 4,5
//  |   LED Coll.|     14 2K   | 08 |            |            |            |        Display LED
//  |   SDO      |             | 09 |            |            |            |        Emmitor  V2 BC557 to 3.3 V
//  |   T_CLK    |     18      | 10 |            |            |            |        Base with 2K to pin 14 (Display_Led)
//  |   T_CS     |      5      | 11 |            |            |            |        Collector to led pin display
//  |   T_DIN    |     23      | 12 |            |            |            |
//  |   T_DO     |     19      | 13 |            |            |            |
//  |   T_IRQ    |     34      | 14 |            |            |            |
//  |            |     12      |    |   Reset    |            |            |
//  |            |     21      |    |    SDA     |            |            |
//  |            |     22      |    |    SCL     |            |            |
//  |            |     16      |    |            |      1     |            |
//  |            |     17      |    |            |      3     |            |
//  |            |     33      |    |            |      5     |            |
//  |            |     32 2K   |    |            |            |     In     |
//  |            |     27 Mute |    |see schematics           |            |
//  |------------|-------------|----|------------|------------|------------|

// #include "./TFT_eSPI/TFT_eSPI.h"
#include <SI4735.h>
#include <SPI.h>
// #include "./TFT_eSPI/TFT_eSPI.h"
#include <TFT_eSPI.h>
#include "EEPROM.h"
#include "Rotary.h"
#include "DSEG7_Classic_Mini_Bold_20.h"
#include "DSEG7_Classic_Mini_Bold_30.h"
#include "Serif_bold_10.h"
#include "Serif_bold_15.h"
#include "Serif_bold_20.h"
#include "Serif_plain_10.h"
#include "Serif_plain_15.h"

// =================================================
//#define IhaveVertTFT
#define IhaveHoriTFT
// =================================================

#include <patch_init.h> // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

#define ESP32_I2C_SDA    21  // I2C bus pin on ESP32
#define ESP32_I2C_SCL    22  // I2C bus pin on ESP32
#define RESET_PIN        12
#define ENCODER_PIN_A    17  // http://www.buxtronix.net/2011/10/rotary-encoders-done-properly.html
#define ENCODER_PIN_B    16
#define ENCODER_SWITCH   33
#define BEEPER           32
#define Display_Led      14
#define displayon         0
#define displayoff        1
#define beepOn            1
#define beepOff           0
#define AUDIO_MUTE       27

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define MIN_ELAPSED_TIME             100
#define MIN_ELAPSED_RSSI_TIME        150
#define MIN_ELAPSED_DISPL_TIME      1000
//#define MIN_ELAPSED_RDS_TIME         5
#define DEFAULT_VOLUME                45  // change it for your favorite start sound volume
#define MIN_ELAPSED_VOLbut_TIME     1000

#define FM          0
#define LSB         1
#define USB         2
#define AM          3
#define SYNC        4

bool bfoOn          = false;
bool ssbLoaded      = false;

bool FirstLayer = true;
bool SecondLayer = false;
bool ThirdLayer = false;
bool ForthLayer = false;
bool HamBand = false;
bool Modebut = false;
bool FREQbut = false;
bool Decipoint = false;
bool STEPbut = false;
bool BroadBand;
bool BandWidth;
bool MISCbut = false;
bool PRESbut = false;
bool VOLbut = false;
bool DISplay = false;
bool Mutestat = false;
bool AGCgainbut = false;
bool writingEeprom = false;

bool pressed;
bool press1;
bool audioMuteOn = true;
bool audioMuteOff = false;
bool RDS = true; // RDS on  or  off
bool SEEK =  false;

int currentBFO;
int previousBFO = 0;
int nrbox       = 0;
int OldRSSI;
int NewRSSI;
int NewSNR;
int encBut;
int AGCgain     = 0;

long elapsedRSSI        = millis();
long elapsedRDS         = millis();
long stationNameElapsed = millis();
long DisplayOnTime      = millis();
long VOLbutOnTime       = millis();

volatile int encoderCount  = 0;

uint16_t previousFrequency;
uint8_t currentStep        =  1;

uint8_t currentBFOStep     = 25;

uint8_t currentPRES        =  0;
uint8_t previousPRES       =  0;
uint8_t currentPRESStep    =  1;

int currentAGCgain     =  1;
int previousAGCgain    =  1;
uint8_t currentAGCgainStep =  1;
uint8_t MaxAGCgain;
uint8_t MaxAGCgainFM       = 26;
uint8_t MaxAGCgainAM       = 37;
uint8_t MinAGCgain         =  1;



int currentVOL         =  0;
int previousVOL        =  0;
uint8_t currentVOLStep     =  1;
uint8_t MaxVOL             = 63;
uint8_t MinVOL             =  0;

uint8_t currentAGCAtt      =  0;
uint8_t bwIdxSSB;
uint8_t bwIdxAM;
uint8_t bandIdx;
uint8_t currentMode = FM;
uint8_t previousMode;
uint16_t x = 0, y = 0; // To store the touch coordinates
uint8_t encoderStatus;
uint16_t freqstep;
uint8_t freqstepnr = 0; //1kHz
int   freqDec = 0;
float Displayfreq      = 0;
float currentFrequency = 0;
float dpfrq            = 0;
float fact             = 1;

String BWtext;
String RDSbuttext;
String AGCgainbuttext;

const char *bandwidthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};
const char *bandwidthAM[]  = {"6.0", "4.0", "3.0", "2.0", "1.0", "1.8", "2.5"};
const char *Keypathtext[]  = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "ENTER", "Clear"};
const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM ", "SYN"};

char buffer[64]; // Useful to handle string
char buffer1[64];

const char *stationName;
char bufferStatioName[40];

char bufferFrequency[15];
char bufferVFO[15];
char bufferUnit[5];
char bufferBandName[10];
char bufferVolume[10];
char bufferAgcGain[10];
char bufferRDS[65];
char bufferAux[15];

const int ledChannel = 0;
const int resolution = 1;

//=======================================================   Buttons First and Third Layer   ==========================
typedef struct // Buttons first layer
{
  const char *ButtonNam;
  
  int    ButtonNum;       // Button location at display from 0 to 11. To move around buttons freely at first layer.
  const char *ButtonNam1;
  int    ButtonNum1;      // Button location at display from 0 to 11. To move around buttons freely at third layer.
  int    XButos;          // Xoffset
  long   YButos;          // Yoffset
} Button;

int ytotoffset = 0;

//  Button table
int Xbutst  =   0;               // X Start location Buttons
int Ybutst  = 160 + ytotoffset;  // Y Start location Buttons

int Xsmtr   =   0;
int Ysmtr   =  85 + ytotoffset;  // S meter

int XVolInd =   0;
int YVolInd = 135 + ytotoffset;  // Volume indicator

int XFreqDispl  =   0;
int YFreqDispl  =   0 + ytotoffset;  // display

int Xbutsiz =  80;  //size of buttons first & third layer
int Ybutsiz =  40;

#ifdef IhaveVertTFT
int Xbut0  = 0 * Xbutsiz ; int Ybut0  = 0 * Ybutsiz; // location calqualation for 12 first layer buttons
int Xbut1  = 1 * Xbutsiz ; int Ybut1  = 0 * Ybutsiz;
int Xbut2  = 2 * Xbutsiz ; int Ybut2  = 0 * Ybutsiz;
int Xbut3  = 0 * Xbutsiz ; int Ybut3  = 1 * Ybutsiz;
int Xbut4  = 1 * Xbutsiz ; int Ybut4  = 1 * Ybutsiz;
int Xbut5  = 2 * Xbutsiz ; int Ybut5  = 1 * Ybutsiz;
int Xbut6  = 0 * Xbutsiz ; int Ybut6  = 2 * Ybutsiz;
int Xbut7  = 1 * Xbutsiz ; int Ybut7  = 2 * Ybutsiz;
int Xbut8  = 2 * Xbutsiz ; int Ybut8  = 2 * Ybutsiz;
int Xbut9  = 0 * Xbutsiz ; int Ybut9  = 3 * Ybutsiz;
int Xbut10 = 1 * Xbutsiz ; int Ybut10 = 3 * Ybutsiz;
int Xbut11 = 2 * Xbutsiz ; int Ybut11 = 3 * Ybutsiz;
#endif

#ifdef IhaveHoriTFT
int Xbut0  = 0 * Xbutsiz ; int Ybut0   = 0 * Ybutsiz; // location calqualation for 12 first layer buttons
int Xbut1  = 1 * Xbutsiz ; int Ybut1   = 0 * Ybutsiz;
int Xbut2  = 2 * Xbutsiz ; int Ybut2   = 0 * Ybutsiz;
int Xbut3  = 3 * Xbutsiz ; int Ybut3   = 0 * Ybutsiz;
int Xbut4  = 0 * Xbutsiz ; int Ybut4   = 1 * Ybutsiz;
int Xbut5  = 1 * Xbutsiz ; int Ybut5   = 1 * Ybutsiz;
int Xbut6  = 2 * Xbutsiz ; int Ybut6   = 1 * Ybutsiz;
int Xbut7  = 3 * Xbutsiz ; int Ybut7   = 1 * Ybutsiz;
int Xbut8  = 3 * Xbutsiz ; long Ybut8  =-4 * Ybutsiz;
int Xbut9  = 3 * Xbutsiz ; long Ybut9  =-3 * Ybutsiz;
int Xbut10 = 3 * Xbutsiz ; long Ybut10 =-2 * Ybutsiz;
int Xbut11 = 3 * Xbutsiz ; long Ybut11 =-1 * Ybutsiz;
#endif

#define HAM       0
#define BFO       1
#define FREQ      2
#define AGC       3
#define MUTE      4
#define VOL       5
#define MODE      6
#define BANDW     7
#define STEP      8
#define BROAD     9
#define PRESET   10
#define NEXT     11

#define SEEKUP    0
#define SEEKDN    1
#define STATUS    2
#define RDSbut    3
#define AGCset    4
#define NR4       5
#define NR5       6
#define NR6       7
#define NR7       8   //                                                 |----|
#define NR8       9   //                                                 |  8 |
#define NR9      10   //                                                 |----|
#define PREV     11   //                                                 |  9 |  
                      //                                                 |----|
                      //                                                 | 10 |
#ifdef IhaveHoriTFT   //                                                 |----|
Button bt[] = {       //                                                 | 11 |
  { "HAM"   ,  0 , "SEEK>" , 0 , Xbut0 , Ybut0  }, //     |----|----|----|----|
  { "BFO"   ,  4 , "SEEK<" , 4 , Xbut1 , Ybut1  }, //     |  0 |  1 |  2 |  3 |
  { "TECLA" ,  2 , "INFO"  , 6 , Xbut2 , Ybut2  }, //     |----|----|----|----|
  { "AGC"   ,  5 , "RDS"   , 8 , Xbut3 , Ybut3  }, //     |  4 |  5 |  6 |  7 |
  { "MUTE"  ,  9 , "AGC"   , 1 , Xbut4 , Ybut4  }, //     |----|----|----|----|
  { "VOL"   ,  8 , ""      , 5 , Xbut5 , Ybut5  },    
  { "MODO"  ,  3 , ""      ,10 , Xbut6 , Ybut6  },    
  { "FILTRO",  6 , ""      ,11 , Xbut7 , Ybut7  },     
  { "PASSO" , 11 , ""      , 2 , Xbut8 , Ybut8  },    
  { "BANDA" ,  1 , ""      , 3 , Xbut9 , Ybut9  },
  { "PSET"  , 10 , ""      , 9 , Xbut10, Ybut10 },
  { "MENU"  ,  7 , "VOLTA" , 7 , Xbut11, Ybut11 }
};
#endif

#ifdef IhaveVertTFT                                                 
Button bt[] = {                                                 
  { "HAM"   ,  0 , "SEEKUP", 0 , Xbut0 , Ybut0  }, //     |----|----|----|
  { "BFO"   ,  3 , "SEEKDN", 3 , Xbut1 , Ybut1  }, //     |  0 |  1 |  2 |
  { "FREQ"  ,  2 , "STATUS",10 , Xbut2 , Ybut2  }, //     |----|----|----|
  { "AGC"   ,  4 , "RDS"   , 9 , Xbut3 , Ybut3  }, //     |  3 |  4 |  5 |
  { "MUTE"  ,  8 , "AGCset", 2 , Xbut4 , Ybut4  }, //     |----|----|----|
  { "VOL"   ,  7 , ""      , 5 , Xbut5 , Ybut5  }, //     |  6 |  7 |  8 |     
  { "MODE"  ,  9 , ""      , 6 , Xbut6 , Ybut6  }, //     |----|----|----|     
  { "BANDW" ,  5 , ""      , 7 , Xbut7 , Ybut7  }, //     |  9 | 10 | 11 |     
  { "STEP"  ,  6 , ""      , 8 , Xbut8 , Ybut8  }, //     |----|----|----|     
  { "BROAD" ,  1 , ""      , 1 , Xbut9 , Ybut9  },
  { "PRESET", 10 , ""      , 4 , Xbut10, Ybut10 },
  { "NEXT"  , 11 , "PREV"  ,11 , Xbut11, Ybut11 }
};
#endif


// You may freely move around the button (blue) position on the display to your flavour by changing the position in ButtonNum and ButtonNum1
// You have to stay in the First or Third Layer
//======================================================= End  Buttons First  and Third Layer   ======================

//======================================================= Tunings Steps     ===============================
typedef struct // Tuning steps
{
  uint16_t stepFreq;
  uint16_t Xstepos;          //Xoffset
  uint16_t Xstepsr;          //X size rectang
  uint16_t Ystepos;          //Yoffset
  uint16_t Ystepsr;          //Y size rectang
  uint16_t Ystepnr;          //Y next rectang
} Step;

//  Tuning steps table

#ifdef IhaveHoriTFT
  uint16_t Xfstep = 60;
  uint16_t Yfstep = 20;
#endif

#ifdef IhaveVertTFT
  uint16_t Xfstep =  70;
  uint16_t Yfstep = 100;
#endif

Step sp[] = {
  { 1 , Xfstep, 200, Yfstep, 50,  0},
  { 5 , Xfstep, 200, Yfstep, 50, 50},
  { 9 , Xfstep, 200, Yfstep, 50,100},
  { 10, Xfstep, 200, Yfstep, 50,150}
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

#ifdef IhaveHoriTFT
  uint16_t Xfmod = 60;
  uint16_t Yfmod = 0;
#endif

#ifdef IhaveVertTFT
  uint16_t Xfmod = 70;
  uint16_t Yfmod = 90;
#endif

Mode md[] = {
  { 0  , Xfmod, 200, Yfmod, 50,  0},
  { 1  , Xfmod, 200, Yfmod, 50, 50},
  { 2  , Xfmod, 200, Yfmod, 50,100},
  { 3  , Xfmod, 200, Yfmod, 50,150},
  { 4  , Xfmod, 200, Yfmod, 50,200}
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

#ifdef IhaveHoriTFT
uint16_t Xpath = 10;
uint16_t Ypath = 30;

Keypath kp[] = {
  {  0 , Xpath,  100 ,   0 , Ypath , 50 ,   0},
  {  1 , Xpath,  100 , 100 , Ypath , 50 ,   0},
  {  2 , Xpath,  100 , 200 , Ypath , 50 ,   0},
  {  3 , Xpath,  100 ,   0 , Ypath , 50 ,  50},
  {  4 , Xpath,  100 , 100 , Ypath , 50 ,  50},
  {  5 , Xpath,  100 , 200 , Ypath , 50 ,  50},
  {  6 , Xpath,  100 ,   0 , Ypath , 50 , 100},
  {  7 , Xpath,  100 , 100 , Ypath , 50 , 100},
  {  8 , Xpath,  100 , 200 , Ypath , 50 , 100},
  {  9 , Xpath,  100 ,   0 , Ypath , 50 , 150},
  { 10 , Xpath,  100 , 100 , Ypath , 50 , 150},
  { 11 , Xpath,  100 , 200 , Ypath , 50 , 150},
};
#endif

#ifdef IhaveVertTFT
uint16_t Xpath = 30;
uint16_t Ypath = 30;

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

//======================================================= Bandwidth AM & FM     ===============================
typedef struct // Bandwidth AM & SSB
{
  uint16_t BandWidthAM;
  uint16_t BandWidthSSB;
  uint16_t Xos;          //Xoffset
  uint16_t Xsr;          //X size rectang
  uint16_t Yos;          //Yoffset
  uint16_t Ysr;          //X size rectang
  uint16_t Ynr;          //Y next rectang
} Bandwidth;

//  Bandwidth table
#ifdef IhaveHoriTFT
uint16_t XfBW = 60;
uint16_t YfBW = 15;
#endif

#ifdef IhaveVertTFT
uint16_t XfBW = 10;
uint16_t YfBW = 5;
#endif

Bandwidth bw[] = {
  { 4 , 4 , XfBW, 200, YfBW, 30,   0},
  { 5 , 5 , XfBW, 200, YfBW, 30,  30},
  { 3 , 0 , XfBW, 200, YfBW, 30,  60},
  { 6 , 1 , XfBW, 200, YfBW, 30,  90},
  { 2 , 2 , XfBW, 200, YfBW, 30, 120},
  { 1 , 3 , XfBW, 200, YfBW, 30, 150},
  { 0 , 0 , XfBW, 200, YfBW, 30, 180},
};
//======================================================= End Bandwidth AM & FM     ===========================

//======================================================= Broad Band Definitions     ==========================
typedef struct // Broad-Band switch
{
  uint16_t BbandNum; // bandIdx
  uint16_t Xbbandos;          //Xoffset
  uint16_t Xbbandsr;          //X size rectang
  uint16_t Xbbandnr;          //X next rectang
  uint16_t Ybbandos;          //Yoffset
  uint16_t Ybbandsr;          //X size rectang
  uint16_t Ybbandnr;          //Y next rectang
} BBandnumber;

//  Bandnumber table for the broad-bands

#ifdef IhaveVertTFT
uint16_t Xfbband = 0;
uint16_t Yfbband = 45;
#endif

#ifdef IhaveHoriTFT
uint16_t Xfbband = 10;
uint16_t Yfbband = 15;
#endif


BBandnumber bb[] = {
  {  0 , Xfbband, 100 ,  0 ,  Yfbband , 35 ,   0}, // 0
  {  1 , Xfbband, 100 ,  0 ,  Yfbband , 35 ,  35}, // 1
  {  2 , Xfbband, 100 ,  0 ,  Yfbband , 35 ,  70}, // 2
  {  6 , Xfbband, 100 ,  0 ,  Yfbband , 35 , 105}, // 3
  {  7 , Xfbband, 100 ,  0 ,  Yfbband , 35 , 140}, // 4
  {  9 , Xfbband, 100 ,  0 ,  Yfbband , 35 , 175}, // 5
  { 11 , Xfbband, 100 ,100 ,  Yfbband , 35 ,   0}, // 6
  { 13 , Xfbband, 100 ,100 ,  Yfbband , 35 ,  35}, // 7
  { 14 , Xfbband, 100 ,100 ,  Yfbband , 35 ,  70}, // 8
  { 16 , Xfbband, 100 ,100 ,  Yfbband , 35 , 105}, // 9
  { 17 , Xfbband, 100 ,100 ,  Yfbband , 35 , 140}, //10
  { 19 , Xfbband, 100 ,100 ,  Yfbband , 35 , 175}, //11
  { 21 , Xfbband, 100 ,200 ,  Yfbband , 35 ,   0}, //12
  { 22 , Xfbband, 100 ,200 ,  Yfbband , 35 ,  35}, //13
  { 24 , Xfbband, 100 ,200 ,  Yfbband , 35 ,  70}, //14
  { 26 , Xfbband, 100 ,200 ,  Yfbband , 35 , 105}, //15
  { 27 , Xfbband, 100 ,200 ,  Yfbband , 35 , 140}, //16
  { 29 , Xfbband, 100 ,200 ,  Yfbband , 35 , 175}, //17

};
//======================================================= End Broad Band Definitions     ======================

//======================================================= Ham Band Definitions     ============================
typedef struct // Ham Band switch
{
  uint16_t BandNum; // bandIdx
  uint16_t HamBandTxt;
  uint16_t Xbandos;          //Xoffset
  uint16_t Xbandsr;          //X size rectang
  uint16_t Xbandnr;          //X next rectang
  uint16_t Ybandos;          //Yoffset
  uint16_t Ybandsr;          //Y size rectang
  uint16_t Ybandnr;          //Y next rectang
} Bandnumber;

//  Bandnumber table for the hambands

#ifdef IhaveVertTFT
  uint16_t Xfband = 10;
  uint16_t Yfband = 30;
#endif

#ifdef IhaveHoriTFT
  uint16_t Xfband = 10;
  uint16_t Yfband = 15;
#endif

Bandnumber bn[] = {
  {  3 , 0 , Xfband, 150 ,   0 , Yfband , 35 ,   0},
  {  4 , 1 , Xfband, 150 ,   0 , Yfband , 35 ,  35},
  {  5 , 2 , Xfband, 150 ,   0 , Yfband , 35 ,  70},
  {  8 , 3 , Xfband, 150 ,   0 , Yfband , 35 , 105},
  { 10 , 4 , Xfband, 150 ,   0 , Yfband , 35 , 140},
  { 12 , 5 , Xfband, 150 ,   0 , Yfband , 35 , 175},
  { 15 , 6 , Xfband, 150 , 150 , Yfband , 35 ,   0},
  { 18 , 7 , Xfband, 150 , 150 , Yfband , 35 ,  35},
  { 20 , 8 , Xfband, 150 , 150 , Yfband , 35 ,  70},
  { 23 , 9 , Xfband, 150 , 150 , Yfband , 35 , 105},
  { 25 , 10 ,Xfband, 150 , 150 , Yfband , 35 , 140},
  { 28 , 11 ,Xfband, 150 , 150 , Yfband , 35 , 175}
};
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
  uint16_t currentStep; // Default step (increment and decrement)
  //float BFOf1;            // BFO set for f1
  //float F1b;              // Freq1 in kHz
  //float BFOf2;            // BFO set for f2
  //float F2b;              // Freq2 in kHz
} Band;

//   Band table

Band band[] = {
  {   "FM", FM_BAND_TYPE,  FM,  8750, 10800,  9890,10}, //  FM          0
  {   "OL", LW_BAND_TYPE,  AM,   153,   279,   198, 1}, //  LW          1
  {   "OM", MW_BAND_TYPE,  AM,   520,  1710,  1000,10}, //  MW          2
  { "TUDO", LW_BAND_TYPE,  AM,   150, 30000,   284, 1}, // All/LW/MW/SW 3
  {  "630", SW_BAND_TYPE, LSB,   472,   479,   475, 1}, // Ham  630M    4
  { "160M", SW_BAND_TYPE, LSB,  1800,  1910,  1899, 1}, // Ham  160M    5
  {   "OT", SW_BAND_TYPE,  AM,  1920,  3200,  2400, 5}, //      120M    6
  {  "90M", SW_BAND_TYPE,  AM,  3200,  3400,  3300, 5}, //       90M    7
  {  "80M", SW_BAND_TYPE, LSB,  3500,  3800,  3700, 1}, // Ham   80M    8
  {  "75M", SW_BAND_TYPE,  AM,  3900,  4000,  3950, 5}, //       75M    9
  {  "60M", SW_BAND_TYPE, USB,  5330,  5410,  5375, 1}, // Ham   60M   10
  {  "49M", SW_BAND_TYPE,  AM,  5900,  6200,  6000, 5}, //       49M   11
  {  "40M", SW_BAND_TYPE, LSB,  7000,  7200,  7132, 1}, // Ham   40M   12
  {  "41M", SW_BAND_TYPE,  AM,  7200,  7450,  7210, 5}, //       41M   13
  {  "31M", SW_BAND_TYPE,  AM,  9400,  9900,  9600, 5}, //       31M   14
  {  "30M", SW_BAND_TYPE, USB, 10100, 10150, 10125, 1}, // Ham   30M   15
  {  "25M", SW_BAND_TYPE,  AM, 11600, 12100, 11700, 5}, //       25M   16
  {  "22M", SW_BAND_TYPE,  AM, 13570, 13870, 13700, 5}, //       22M   17
  {  "20M", SW_BAND_TYPE, USB, 14000, 14350, 14200, 1}, // Ham   20M   18
  {  "19M", SW_BAND_TYPE,  AM, 15100, 15830, 15700, 5}, //       19M   19
  {  "17M", SW_BAND_TYPE, USB, 18068, 18168, 18100, 1}, // Ham   17M   20
  {  "16M", SW_BAND_TYPE,  AM, 17480, 17900, 17600, 5}, //       16M   21
  {  "15M", SW_BAND_TYPE,  AM, 18900, 19020, 18950, 5}, //       15M   22
  {  "15M", SW_BAND_TYPE, USB, 21000, 21450, 21350, 1}, // Ham   15M   23
  {  "13M", SW_BAND_TYPE,  AM, 21450, 21850, 21500, 5}, //       13M   24
  {  "12M", SW_BAND_TYPE, USB, 24890, 24990, 24940, 1}, // Ham   12M   25
  {  "11M", SW_BAND_TYPE,  AM, 25670, 26100, 25800, 5}, //       11M   26
  {   "PX", SW_BAND_TYPE,  AM, 26200, 27990, 27200, 1}, // CB band     27
  {  "10M", SW_BAND_TYPE, USB, 28000, 30000, 28500, 1}, // Ham   10M   28
  {   "OC", SW_BAND_TYPE,  AM,  1730, 30000, 15500, 5}  // Whole SW    29
};
//======================================================= End THE Band Definitions     ========================

//======================================================= FM Presets     ======================================
typedef struct // Preset data
{
  float      presetIdx;
  const char *PresetName;
} FM_Preset ;

FM_Preset preset[] = {

  8800  , "",      // 00 
  
};


//======================================================= END FM Presets     ======================================

const int lastButton = (sizeof bt / sizeof(Button)) - 1;
const int lastBand   = (sizeof band / sizeof(Band)) - 1;
const int lastHam    = (sizeof bn / sizeof(Bandnumber)) - 1;
const int lastBroad  = (sizeof bb / sizeof(BBandnumber)) - 1;
const int lastMod    = (sizeof md / sizeof(Mode)) - 1;
const int lastBW     = (sizeof bw / sizeof(Bandwidth)) - 1;
const int lastStep   = (sizeof sp / sizeof(Step)) - 1;
const int lastKPath  = (sizeof kp / sizeof(Keypath)) - 1;
const int lastPreset = (sizeof preset / sizeof (FM_Preset)) - 1;

#define offsetEEPROM       0x30
#define EEPROM_SIZE        150

struct StoreStruct {
  byte     chkDigit;
  byte     bandIdx; 
  uint16_t Freq; 
  uint8_t  currentMode;
  uint8_t  bwIdxSSB;
  uint8_t  bwIdxAM;
  uint8_t  currentStep;
  int      currentBFO;
  uint8_t  currentAGCAtt;
  uint8_t  currentVOL;
  uint8_t  currentBFOStep;
  uint8_t  RDS;
};

StoreStruct storage = {
  '#',  //First time check
    0,  //bandIdx 
 8930,  //Freq
    0,  //mode
    1,  //bwIdxSSB
    3,  //bwIdxAM
    9,  //currentStep
 -125,  //currentBFO  
    2,  //currentAGCAtt
   45,  //currentVOL 
   25,  //currentBFOStep
    1   //RDS
};

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

TFT_eSPI tft = TFT_eSPI();

SI4735 si4735;

//=======================================================================================
void IRAM_ATTR RotaryEncFreq() {
//=======================================================================================
  // rotary encoder events
  if (!writingEeprom){
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

//=======================================================================================
void setup() {
//=======================================================================================
  // Serial.begin(115200);
  pinMode(Display_Led, OUTPUT);
  pinMode(BEEPER, OUTPUT);
  digitalWrite(Display_Led, displayon);
  DISplay = true;

  Beep(1, 200);

  tft.init();
  #ifdef IhaveVertTFT
    tft.setRotation(2);
  #endif

  #ifdef IhaveHoriTFT
    tft.setRotation(1);
  #endif
  
  //tft.setRotation(0); // Rotate 0
  //tft.setRotation(1); // Rotate 90
  //tft.setRotation(2); // Rotate 180
  //tft.setRotation(3); // Rotate 270

  // Cleans the EEPROM content. 
  // if the encoder push button is pressed during the system initialization, the EEPROM will be clened.
  for (int i = 0; i < 10; i++) {
      encBut = analogRead(ENCODER_SWITCH);
      if (encBut < 500) {
        resetEEPROM(); // Cleans the EEPROM.
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.println(F("A EEPROM FOI LIMPA!")); 
        while(1);  // Stops the System. It is needed to turn it off. 
      }
      delay(100);
  } 

  #ifdef IhaveVertTFT
    // Calibration code for touchscreen : for 2.8 inch & Rotation = 2
    uint16_t calData[5] = { 258, 3566, 413, 3512, 2 };
    tft.setTouch(calData);
  #endif
  
  #ifdef IhaveHoriTFT
    // Calibration code for touchscreen : Rotation = 1
    uint16_t calData[5] = { 387, 3530, 246, 3555, 7 };
    tft.setTouch(calData);
  #endif

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println(F("failed to initialise EEPROM"));
    // Serial.println(F("failed to initialise EEPROM"));
    while(1); 
  }

  // RESET the EEPROM
  if (EEPROM.read(offsetEEPROM) != storage.chkDigit){
    // Serial.println(F("Writing defaults...."));
    saveConfig();
  }
  loadConfig();
  printConfig();
  

  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL); //I2C for SI4735

  // Serial.println(F("I2C - Inicio"));

  // Encoder pins
  pinMode(ENCODER_PIN_A , INPUT_PULLUP); //Rotary encoder Freqency/bfo/preset
  pinMode(ENCODER_PIN_B , INPUT_PULLUP);
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), RotaryEncFreq, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), RotaryEncFreq, CHANGE);

  si4735.setAudioMuteMcuPin(AUDIO_MUTE);
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);

  // Serial.println(F("I2C - ADDR"));
  // Serial.println(si4735Addr);

  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.setCursor(7, 25);
  tft.setFreeFont(&Serif_bold_20);
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  // Serial.println("    SI4732 RADIO");
  // Serial.println("Version 1");
  tft.println(" RADIO DSP");
  tft.setCursor(7, 75);
  tft.println(" SOFT PE0MGB-PU2CLR");
  tft.setCursor(7, 125);
  tft.println(" PCB BY THIAGO");
  tft.setCursor(7, 200);
  delay(1000);
  tft.setTextColor(TFT_RED, TFT_BLUE);
  if ( si4735Addr == 0 ) {
    tft.print("Si4735 not detected");
    tft.setFreeFont(NULL);
    // Serial.println("Si4735 not detected");
    while (1);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_20);
    tft.setTextSize(1);
    tft.print(" SI4732 :  ");
    tft.println(si4735Addr, HEX);
    tft.setFreeFont(NULL);
    // Serial.print("Si473X addr :  ");
    // Serial.println(si4735Addr, HEX);
  }
  
  delay(1500);


  // Setup the radio from last setup in EEPROM

  // Serial.println(F("Iniciando VARIAVEIS"));

  bandIdx                   = storage.bandIdx;
  band[bandIdx].currentFreq = storage.Freq;
  currentMode               = storage.currentMode;
  bwIdxSSB                  = storage.bwIdxSSB;
  bwIdxAM                   = storage.bwIdxAM;
  currentStep               = storage.currentStep;
  currentBFO                = storage.currentBFO;
  currentAGCAtt             = storage.currentAGCAtt;
  currentVOL                = storage.currentVOL;
  currentBFOStep            = storage.currentBFOStep;
  RDS                       = storage.RDS;

  // Serial.println(F("Iniciando O SI4732"));

  if (bandIdx == 0)  si4735.setup(RESET_PIN, 0); // Start in FM
  else si4735.setup(RESET_PIN, 1); // Start in AM
  if (bandIdx != 0) si4735.setAM();

  // Serial.println(F("SI4732 Iniciado"));

  freqstep = 1000;//hz
  previousBFO = -1;
  si4735.setVolume(currentVOL);
  previousVOL = currentVOL;
 
  BandSet();
  if (currentStep != band[bandIdx].currentStep ) band[bandIdx].currentStep = currentStep;
  currentFrequency = previousFrequency = si4735.getFrequency();
  Beep(2, 200);
  encBut = 600;
  x = y = 0;
  DrawFila();
  si4735.setSeekFmSpacing(1);        
  si4735.setSeekFmLimits(8750,10790);
  si4735.setSeekAmRssiThreshold(20);
  si4735.setSeekAmSNRThreshold(10);
  si4735.setSeekFmRssiThreshold(5);
  si4735.setSeekFmSNRThreshold(5);

  xTaskCreate(SaveInEeprom, "SaveInEeprom", 2048, NULL, 1, NULL);

  // Serial.println(F("Fim do Setup"));

}// end setup


/**
 * Cleans the buffer contents
 * 
 */
void cleanBuffer() {
  bufferVolume[0] = bufferAgcGain[0] = bufferFrequency[0] = bufferUnit[0] = bufferBandName[0] = bufferVFO[0] = '\0';
}


//=======================================================================================
void SaveInEeprom (void* arg)  {
//=======================================================================================  
  while(1) {     
    storage.bandIdx = bandIdx;
    storage.Freq =  band[bandIdx].currentFreq;
    storage.currentMode = currentMode;
    storage.bwIdxSSB = bwIdxSSB;
    storage.bwIdxAM = bwIdxAM;
    storage.currentStep = currentStep;
    storage.currentBFO = currentBFO;
    storage.currentAGCAtt = currentAGCAtt;
    storage.currentVOL = currentVOL;
    storage.currentBFOStep = currentBFOStep;
    storage.RDS = RDS;
    for (unsigned int t = 0; t < sizeof(storage); t++) {
      delay(1);
      if (EEPROM.read(offsetEEPROM + t) != *((char*)&storage + t)){
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
    if (EEPROM.read(offsetEEPROM + t) != *((char*)&storage + t)){
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
    // Serial.println("Load config done");  
  }    
}

//=======================================================================================
void printConfig() {
//=======================================================================================
  // Serial.println(sizeof(storage));
  // if (EEPROM.read(offsetEEPROM) == storage.chkDigit){
  //  for (unsigned int t = 0; t < sizeof(storage); t++)
  //    Serial.write(EEPROM.read(offsetEEPROM + t)); 
  //  Serial.println();
  //setSettings(0);
  //}
}

//=======================================================================================
void BandSet()  {
//=======================================================================================
  if (bandIdx == 0) currentMode = 0;// only mod FM in FM band
  if ((currentMode == AM) || (currentMode == FM)) ssbLoaded = false;

  if ((currentMode == LSB) || (currentMode == USB) || (currentMode == SYNC))
  {
    if (ssbLoaded == false) {
      loadSSB();
    }
  }
  useBand();
  setBandWidth();
}

//=======================================================================================
void useBand()  {
//=======================================================================================
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  cleanBuffer();
  
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    bfoOn = false;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    delay(100);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    si4735.setFMDeEmphasis(1);
    ssbLoaded = false;
    si4735.RdsInit();
    si4735.setRdsConfig(1, 2, 2, 2, 2);
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
      si4735.setTuneFrequencyAntennaCapacitor(0);
    } else { //SW_BAND_TYPE
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
      //si4735.setSSBSidebandCutoffFilter(0);
     
      
      si4735.setSSBBfo(currentBFO);     
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
      si4735.setSSBSidebandCutoffFilter(0);
    else
      si4735.setSSBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    si4735.setBandwidth(bwIdxAM, 0);
  }
}

//=======================================================================================
void loadSSB()  {
//=======================================================================================
  // si4735.reset();
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  si4735.patchPowerUp();
  delay(50);
  si4735.setI2CFastMode(); // Recommended
  //si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
  si4735.downloadPatch(ssb_patch_content, size_content);
  si4735.setI2CStandardMode(); // goes back to default (100kHz)

  // delay(50);
  // Parameters
  // AUDIOBW - SSB Audio bandwidth; 0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz;
  // SBCUTFLT SSB - side band cutoff filter for band passand low pass filter ( 0 or 1)
  // AVC_DIVIDER  - set 0 for SSB mode; set 3 for SYNC mode.
  // AVCEN - SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
  // SMUTESEL - SSB Soft-mute Based on RSSI or SNR (0 or 1).
  // DSP_AFCDIS - DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
  if (currentMode == SYNC) {
    si4735.setSSBConfig(bwIdxSSB, 1, 3, 0, 0, 0); // SYNC MODE
    currentBFO = 0;
  }
  else  
    si4735.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);

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
  tft.setFreeFont(&Serif_bold_20);
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(30, 20);
  if (Decipoint) {
    tft.print((Displayfreq / 10) + dpfrq, 4);
  }
  else {
    tft.print((Displayfreq / 10) + dpfrq, 0);
    if (((Displayfreq / 10) + dpfrq) <= 30000); 
    tft.setFreeFont(NULL);
  }
}


//=======================================================================================
void Smeter() {
//=======================================================================================
  int spoint;
  if (currentMode != FM) {
    //dBuV to S point conversion HF
    if ((rssi >= 0) and (rssi <=  1)) spoint =  12;                    // S0
    if ((rssi >  1) and (rssi <=  1)) spoint =  24;                    // S1
    if ((rssi >  2) and (rssi <=  3)) spoint =  36;                    // S2
    if ((rssi >  3) and (rssi <=  4)) spoint =  48;                    // S3
    if ((rssi >  4) and (rssi <= 10)) spoint =  48+(rssi- 4)*2;        // S4
    if ((rssi > 10) and (rssi <= 16)) spoint =  60+(rssi-10)*2;        // S5
    if ((rssi > 16) and (rssi <= 22)) spoint =  72+(rssi-16)*2;        // S6
    if ((rssi > 22) and (rssi <= 28)) spoint =  84+(rssi-22)*2;        // S7
    if ((rssi > 28) and (rssi <= 34)) spoint =  96+(rssi-28)*2;        // S8
    if ((rssi > 34) and (rssi <= 44)) spoint = 108+(rssi-34)*2;        // S9
    if ((rssi > 44) and (rssi <= 54)) spoint = 124+(rssi-44)*2;        // S9 +10
    if ((rssi > 54) and (rssi <= 64)) spoint = 140+(rssi-54)*2;        // S9 +20
    if ((rssi > 64) and (rssi <= 74)) spoint = 156+(rssi-64)*2;        // S9 +30
    if ((rssi > 74) and (rssi <= 84)) spoint = 172+(rssi-74)*2;        // S9 +40
    if ((rssi > 84) and (rssi <= 94)) spoint = 188+(rssi-84)*2;        // S9 +50
    if  (rssi > 94)                   spoint = 204;                    // S9 +60
    if  (rssi > 95)                   spoint = 208;                    //>S9 +60
  }
  else
  {
    //dBuV to S point conversion FM
    if  (rssi <  1) spoint = 36;
    if ((rssi >  1) and (rssi <=  2)) spoint =  60;                    // S6
    if ((rssi >  2) and (rssi <=  8)) spoint =  84+(rssi- 2)*2;        // S7 
    if ((rssi >  8) and (rssi <= 14)) spoint =  96+(rssi- 8)*2;        // S8
    if ((rssi > 14) and (rssi <= 24)) spoint = 108+(rssi-14)*2;        // S9
    if ((rssi > 24) and (rssi <= 34)) spoint = 124+(rssi-24)*2;        // S9 +10
    if ((rssi > 34) and (rssi <= 44)) spoint = 140+(rssi-34)*2;        // S9 +20
    if ((rssi > 44) and (rssi <= 54)) spoint = 156+(rssi-44)*2;        // S9 +30
    if ((rssi > 54) and (rssi <= 64)) spoint = 172+(rssi-54)*2;        // S9 +40
    if ((rssi > 64) and (rssi <= 74)) spoint = 188+(rssi-64)*2;        // S9 +50
    if  (rssi > 74)                   spoint = 204;                    // S9 +60
    if  (rssi > 76)                   spoint = 208;                    //>S9 +60
  }
  
  tft.fillRect(Xsmtr + 15, Ysmtr + 38 , (2 + spoint), 6, TFT_GREEN);
  tft.fillRect(Xsmtr + 17 + spoint, Ysmtr + 38 , 212 - (2 + spoint), 6, TFT_DARKGREEN);
}

//=======================================================================================
void VolumeIndicator(int vol) {
//=======================================================================================
  vol = map(vol, 20, 63, 0, 212);
  tft.fillRect(XVolInd + 15, YVolInd + 16 , (2 + vol), 6, TFT_ORANGE);
  tft.fillRect(XVolInd + 17 + vol, YVolInd + 16 , 212 - (2 + vol), 6, TFT_RED);
}


//=======================================================================================
void loop() {
//=======================================================================================
 
  if ((FirstLayer == true) or (ThirdLayer == true)) VolumeIndicator(si4735.getVolume());

  // Pressed will be set true is there is a valid touch on the screen
  while (((pressed == false) and (encoderCount == 0) and (encBut > 500)) or (writingEeprom)) {  // wait loop  
    pressed = tft.getTouch(&x, &y);
    encBut = analogRead(ENCODER_SWITCH);
    showtimeRSSI();
    DisplayRDS();
    Dispoff();
  }

  encoderCheck();        // Check if the encoder has moved.
  encoderButtonCheck();  // Check if encoderbutton is pressed

  if (pressed) {
    pressed = false;
    PRESbut = false; // Preset stopped after other button is pressed

    DisplayOnTime = millis();
    if (DISplay == false) {
      Beep(1, 0);
      delay(400);
      digitalWrite(Display_Led, displayon);
      DISplay = true;
      x = y = 0; // no valid touch only for switch on display led
    }
    if (FirstLayer) { //==================================================
      //Check which button is pressed in First Layer.
      for (int n = 0 ; n <= lastButton; n++) {
        if ((x > (Xbutst + (bt[bt[n].ButtonNum].XButos))) and (x < Xbutst + (bt[bt[n].ButtonNum].XButos) + Xbutsiz) and (y > Ybutst + (bt[bt[n].ButtonNum].YButos)) and (y < Ybutst + (bt[bt[n].ButtonNum].YButos) + Ybutsiz)) {
          Beep(1, 0);
          delay(400);
          x = 0;
          y = 0;

          if ((VOLbut == true) and (n != VOL)) {
            VOLbut = false;
            drawVOL();
            DrawDispl ();
          }

          if ((Mutestat == true) and (n != MUTE)) {
            Mutestat = false;
            drawMUTE();
          }

           if ((bfoOn == true) and (n == VOL)) {
            bfoOn = false;
            drawBFO();
          } 


          if (n == HAM) {
            delay(400);  //HamBand button
            HamBand = true;
            HamBandlist();
            FirstLayer = false;
            SecondLayer = true;
          }

          if (n == BFO) {           //==============================  // BFO button
            delay(400);

            if (currentMode == LSB || currentMode == USB)  {
              if (bfoOn == false) {
                bfoOn = true;
              } else {
                bfoOn = false;
              }
              cleanDispl();
              drawBFO();
              DrawDispl ();
            } else Beep(4, 100);
          }

          if (n == FREQ) {          //============================  // Frequency input
            delay(400);
            FREQbut = true;
            Decipoint = false;
            Displayfreq = 0;
            dpfrq = 0;
            drawKeyPath();
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
            cleanDispl();
            drawAGC();
            DrawDispl ();
          }

          if (n == MODE) {    //============================= MODE
            if (currentMode != FM)  {
              delay(400);// Mode
              Modebut = true;
              Modelist();
              FirstLayer = false;
              SecondLayer = true;
            } else Beep(4, 100);
          }


          if (n == BANDW) {        //=========================BANDWIDTH
            delay(400);
            if (currentMode != FM)  {
              BandWidth = true;
              BWList();
              FirstLayer = false;
              SecondLayer = true;
            } else Beep(4, 100);
          }


          if (n == STEP) {            //========================= STEPS for tune and bfo
            delay(400);
            if (currentMode != FM)  {
              if (bfoOn) setStep();
              else {
                cleanDispl();
                STEPbut = true;
                Steplist();
                FirstLayer = false;
                SecondLayer = true;
              }
            } else Beep(4, 100);
          }

          if (n == BROAD)  {
            delay(400);
            BroadBand = true;
            BroadBandlist();
            FirstLayer = false;
            SecondLayer = true;
          }
          if (n == PRESET) {
            delay(400);
            x = 0;
            y = 0;
            PRESbut = true;
            //tft.fillScreen(TFT_BLACK);
            FirstLayer = false;
            SecondLayer = true;
          }

          if (n == VOL) {
            delay(400);
            x = 0;
            y = 0;
            if (VOLbut == false) {
              VOLbut = true;
              currentVOL = si4735.getVolume();
              previousVOL = currentVOL;
              FirstLayer = false;
              SecondLayer = true;
            }
            else {
              VOLbut = false;
              FirstLayer = true;
              SecondLayer = false;
            }
            cleanDispl();
            FreqDispl();
            drawVOL();
          }

          if (n == MUTE) {
            delay(200);
            x = 0;
            y = 0;
            if (Mutestat == false)  {
              Mutestat = true;
            }
            else  {
              Mutestat = false;
            }
            drawMUTE();
          }

          if (n == NEXT) {
            delay(200);
            x = 0;
            y = 0;
            FirstLayer  = false;
            SecondLayer = false;
            ThirdLayer  = true;
            ForthLayer  = false;
            DrawThla();
            cleanDispl();
            showFrequency();
          }
        }
      }
    } // end FirstLayer

    if (SecondLayer) {  //===============================================================
      if (Modebut == true) {
        for (int n = 1 ; n <= lastMod; n++) {
          if ((x > md[n].Xmodos) and (x < ((md[n].Xmodos) + (md[n].Xmodsr))) and (y > ((md[n].Ymodos) + (md[n].Ymodnr))) and (y < ((md[n].Ymodos) + (md[n].Ymodsr) + (md[n].Ymodnr)))) {
            Beep(1, 0);
            delay(400);
            Modebut = false;
            currentMode = n;
            BandSet();
            DrawFila();
          }
        }
      }

      if (BandWidth == true) {
        if ( currentMode == AM) {
          for (int n = 0 ; n < 7; n++) {
            if ((x > bw[n].Xos) and (x < ((bw[n].Xos) + (bw[n].Xsr))) and (y > ((bw[n].Yos) + (bw[n].Ynr))) and (y < ((bw[n].Yos) + (bw[n].Ysr) + (bw[n].Ynr)))) {
              Beep(1, 0);
              delay(400);
              bwIdxAM = bw[n].BandWidthAM;
              BandWidth = false;
              BandSet();
              //setBandWidth();
              DrawFila();

            }
          }
        }
        else {
          for (int n = 0 ; n < 6; n++) {
            if ((x > bw[n].Xos) and (x < ((bw[n].Xos) + (bw[n].Xsr))) and (y > ((bw[n].Yos) + (bw[n].Ynr))) and (y < ((bw[n].Yos) + (bw[n].Ysr) + (bw[n].Ynr)))) {
              Beep(1, 0);
              delay(400);
              bwIdxSSB = bw[n].BandWidthSSB;
              BandWidth = false;
              BandSet();
              DrawFila();
            }
          }
        }
      }

      if (STEPbut == true) {
        for (int n = 0 ; n < 4; n++) {
          if ((x > sp[n].Xstepos) and (x < ((sp[n].Xstepos) + (sp[n].Xstepsr))) and (y > ((sp[n].Ystepos) + (sp[n].Ystepnr))) and (y < ((sp[n].Ystepos) + (sp[n].Ystepsr) + (sp[n].Ystepnr)))) {
            Beep(1, 0);
            delay(400);
            STEPbut = false;
            currentStep = sp[n].stepFreq;
            setStep();
            DrawFila();
          }
        }
      }

      if (BroadBand == true) {
        for (int n = 0 ; n <= lastBroad; n++) {
          if ((x > ((bb[n].Xbbandos) + (bb[n].Xbbandnr))) and (x < ((bb[n].Xbbandos) + (bb[n].Xbbandsr) + (bb[n].Xbbandnr))) and (y > ((bb[n].Ybbandos) + (bb[n].Ybbandnr))) and (y < ((bb[n].Ybbandos) + (bb[n].Ybbandsr) + (bb[n].Ybbandnr)))) {
            Beep(1, 0);
            delay(400);
            BroadBand = false;
            bandIdx = bb[n].BbandNum;
            if ((bandIdx == 0) and (currentAGCgain >=28)) currentAGCgain = previousAGCgain = 26; // currentAGCgain in FM max. 26
            si4735.setAM();
            delay(50);
            currentMode = band[bandIdx].prefmod;
            bwIdxAM =  3;
            BandSet();
            DrawFila(); //Draw first layer
          }
        }
      }

      if (HamBand == true) {
        for (int n = 0 ; n <= lastHam; n++) {
          if ((x > ((bn[n].Xbandos) + (bn[n].Xbandnr))) and (x < ((bn[n].Xbandos) + (bn[n].Xbandsr) + (bn[n].Xbandnr))) and (y > ((bn[n].Ybandos) + (bn[n].Ybandnr))) and (y < ((bn[n].Ybandos) + (bn[n].Ybandsr) + (bn[n].Ybandnr)))) {
            Beep(1, 0);
            delay(400);
            HamBand = false;
            bandIdx = bn[n].BandNum;
            if (ssbLoaded == false) {
              si4735.setAM();
              delay(50);
            }
            currentMode = band[bandIdx].prefmod;
            bwIdxSSB = 1;
            BandSet();
            DrawFila();
          }
        }
      }


      if (PRESbut == true) {
        delay(200);
        if (currentMode != 0) { // geen fm ?
          bandIdx = 0;
          currentMode = 0;
          bfoOn = false;
          drawBFO();
          previousPRES = -1;
        }
        FirstLayer  =  true;
        SecondLayer = false;
        previousPRES = -2;
      }

      if (VOLbut == true) {
        delay(200);
        currentVOL = si4735.getVolume();
        previousVOL = currentVOL;
        FirstLayer  =  true;
        SecondLayer = false;
        cleanDispl();
        FreqDispl();
      }

      if (FREQbut == true) {
        for (int n = 0 ; n < 12; n++) { // which keys are pressed?
          if ((x > ((kp[n].Xkeypos) + (kp[n].Xkeypnr))) and (x < ((kp[n].Xkeypos) + (kp[n].Xkeypsr) + (kp[n].Xkeypnr))) and (y > ((kp[n].Ykeypos) + (kp[n].Ykeypnr))) and (y < ((kp[n].Ykeypos) + (kp[n].Ykeypsr) + (kp[n].Ykeypnr)))) {
            tft.fillRect((kp[n].Xkeypos + kp[n].Xkeypnr + 3) , (kp[n].Ykeypos + kp[n].Ykeypnr + 3), (kp[n].Xkeypsr - 6) , (kp[n].Ykeypsr - 6), TFT_WHITE);
            tft.setTextColor(TFT_RED, TFT_WHITE ); // Temporary white button when touched
            tft.setFreeFont(&Serif_bold_20);
            tft.setTextSize(1);
            tft.setTextDatum(BC_DATUM);
            tft.setTextPadding(0);
            tft.drawString((Keypathtext[kp[n].KeypNum]), ( kp[n].Xkeypos + kp[n].Xkeypnr + 50), (kp[n].Ykeypos + kp[n].Ykeypnr  + 37));
            Beep(1, 0);
            delay(100);
            tft.fillRect((kp[n].Xkeypos + kp[n].Xkeypnr + 3) , (kp[n].Ykeypos + kp[n].Ykeypnr + 3), (kp[n].Xkeypsr - 6) , (kp[n].Ykeypsr - 6), TFT_BLUE);
            tft.setTextColor(TFT_YELLOW, TFT_BLUE );
            tft.setTextSize(1);
            tft.setTextDatum(BC_DATUM);
            //tft.setTextPadding(0);
            tft.drawString((Keypathtext[kp[n].KeypNum]), ( kp[n].Xkeypos + kp[n].Xkeypnr + 50), (kp[n].Ykeypos + kp[n].Ykeypnr  + 37));
            if ((n >= 0) and (n <= 8)) Freqcalq(n + 1);
            if (n == 10) Freqcalq(0);
            if (n == 9) {
              Decipoint = true;
              fact = 10;
            }
            if (n == 11) {// Send button
              FREQbut = false;
              Displayfreq = (Displayfreq / 10) + dpfrq;
              if(Displayfreq < 1) {
                tft.setCursor(150, 20);
                tft.print("ERRO");
                ErrorBeep();
              }else{
                if ((Displayfreq > 30) and (Displayfreq < 87.5 )) {
                  tft.setCursor(150, 20);
                  tft.print("ERRO");
                  ErrorBeep();
                }else{
                  if ((Displayfreq >= 108) and (Displayfreq < 153 )) {
                    tft.setCursor(150, 20);
                    tft.print("ERRO");
                    ErrorBeep();
                  }else{
                    if (Displayfreq > 30000) Displayfreq = Displayfreq / 1000000;
                    if ((Displayfreq <= 30000) and (Displayfreq >= 153) and (Decipoint == false )) Displayfreq = Displayfreq / 1000;
                    if ((Displayfreq >= 87.5) and (Displayfreq <= 108)) {
                      currentFrequency = Displayfreq * 100;
                      bandIdx = 0;
                      band[bandIdx].currentFreq = currentFrequency;
                    }else{
                      currentFrequency = Displayfreq * 1000;
                      for (int q = 1 ; q <= lastBand; q++) {
                        if (((currentFrequency) >= band[q].minimumFreq) and ((currentFrequency) <= band[q].maximumFreq)) {
                          bandIdx = q;
                          currentMode = band[q].prefmod;
                          currentStep = band[bandIdx].currentStep;
                          break;
                        }
                      }
                      delay(100);
                      band[bandIdx].currentFreq = currentFrequency;
                      tft.setFreeFont(NULL);
                    }
                  }
                }
              }  
              BandSet();
              DrawFila();
            }//   End   n=11 Send button
          }
        }
      }//end freq
    }// end second layer

    if (ThirdLayer) { //==================================================
      //Check which button is pressed in Third Layer.
      for (int n = 0 ; n <= lastButton; n++) {
        if ((x > (Xbutst + (bt[bt[n].ButtonNum1].XButos))) and (x < Xbutst + (bt[bt[n].ButtonNum1].XButos) + Xbutsiz) and (y > Ybutst + (bt[bt[n].ButtonNum1].YButos)) and (y < Ybutst + (bt[bt[n].ButtonNum1].YButos) + Ybutsiz)) {
          Beep(1, 0);
          delay(400);
          x = 0;
          y = 0;

          if (n == SEEKUP) {
            delay(200);
            x = 0;
            y = 0;
            SEEK = true;
            if ((currentMode != LSB) and (currentMode != USB))   {
              if (currentMode != FM) {     // No FM
                if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     //9 kHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
                } 
                else {
                  bandIdx = 29;// all sw
                  si4735.setSeekAmSpacing(band[bandIdx].currentStep);     // 5 kHz
                  si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);  
                }
              }
              si4735.frequencyUp();
              si4735.seekStationProgress(SeekFreq, checkStopSeeking,  SEEK_UP);
              // si4735.seekStationProgress(SeekFreq,1);// 1 is up
              delay(300);
              currentFrequency = si4735.getFrequency();
              band[bandIdx].currentFreq = currentFrequency ;
              if (currentFrequency != previousFrequency)
              {
                previousFrequency = currentFrequency;
                DrawDispl();
                delay(300); 
              }
            }  
            cleanDispl();
            showFrequency();
            SEEK = false;
          }

         if (n == SEEKDN) {
           delay(200);
           x = 0;
           y = 0;
           SEEK = true;
           if ((currentMode != LSB) and (currentMode != USB))   {
             if (currentMode != FM) {     // No FM
               if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE) {
                 si4735.setSeekAmSpacing(band[bandIdx].currentStep);     //9 kHz
                 si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
               } else {
                 bandIdx = 29;// all sw
                 si4735.setSeekAmSpacing(band[bandIdx].currentStep);     // 5 kHz
                 si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);  
               }
           }
          si4735.frequencyDown();
          // si4735.seekStationProgress(SeekFreq,0);
          si4735.seekStationProgress(SeekFreq, checkStopSeeking,  SEEK_DOWN);
          delay(300);
          currentFrequency = si4735.getFrequency();
          band[bandIdx].currentFreq = currentFrequency ;
          if (currentFrequency != previousFrequency)
          {
            previousFrequency = currentFrequency;
            DrawDispl();
            delay(300); 
          }
          }
          cleanDispl();
          showFrequency();
          SEEK = false;
        }

          if (n == STATUS) {
            delay(200);
            x = 0;
            y = 0;
            subrstatus();
            DrawThla();
          }

          if (n == PREV) {
            delay(200);
            x = 0;
            y = 0;
            AGCgainbut = false;
            FirstLayer  = true;
            SecondLayer = false;
            ThirdLayer  = false;
            ForthLayer  = false;
            cleanDispl();
            DrawFila();
          }

          if (n == RDSbut) {
            delay(200);
            x = 0;
            y = 0;
            if (RDS) RDS = false;
            else RDS = true;
            //DrawRDSbut();
            //DrawThla();
            DrawButThla(); 
          }

          if (n == AGCset) {
            delay(200);
            x = 0;
            y = 0;
             if (AGCgainbut) AGCgainbut = false;
             else {
               bfoOn = false; // only AGC function at the rotory encoder
               AGCgainbut = true;
               si4735.getAutomaticGainControl();
               previousAGCgain = 37; // force to setup AGC gain
             }  
             FreqDispl();
             DrawThla();  
          }  
        }
      }
    } // end ThirdLayer

    if (ForthLayer) { //===============================================================

    }
  }// end pressed

  if (currentMode == LSB || currentMode == USB) // set BFO value in si4735
  {
    if (currentBFO != previousBFO)
    {
      previousBFO = currentBFO;
      si4735.setSSBBfo(currentBFO);
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
    tft.fillRect(XFreqDispl +6, YFreqDispl + 22 , 228, 32, TFT_BLACK);
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

  if (currentAGCgain != previousAGCgain)
  {
    AGCgain = 1;
    //currentAGCgain = currentAGCgain + (currentAGCgain - previousAGCgain);
    tft.setCursor(0, 20);
    if (si4735.isCurrentTuneFM())  MaxAGCgain = MaxAGCgainFM;
    else MaxAGCgain = MaxAGCgainAM;

    if (currentAGCgain > MaxAGCgain) currentAGCgain = MaxAGCgain;
    if (currentAGCgain < MinAGCgain) currentAGCgain = MinAGCgain;

    previousAGCgain = currentAGCgain;
    si4735.setAutomaticGainControl(1,currentAGCgain);
    DrawDispl();
    DrawAGCgainbut();
  }

  
  
//=======================================================================================
}// end loop
//=======================================================================================




//=======================================================================================
void Dispoff()  {
//=======================================================================================
  if (((millis() - DisplayOnTime) > MIN_ELAPSED_DISPL_TIME * 300) and (DISplay == true)) {
    DISplay = false;
    digitalWrite(Display_Led, displayoff);
    // Serial.println("Display off");
    PRESbut = false;
    cleanDispl();
    DrawDispl();
    DisplayOnTime = millis();
  }
}

//=======================================================================================
void VOLbutoff()  {
//=======================================================================================
  if (((millis() - VOLbutOnTime) > MIN_ELAPSED_VOLbut_TIME * 10) and (VOLbut == true)) {
    VOLbut = false;
    drawVOL();
    cleanDispl();
    FreqDispl();
  }
  if (VOLbut == false) VOLbutOnTime = millis();
}



//=======================================================================================
void DisplayRDS()  {
//=======================================================================================
  if (( currentMode == FM) and((FirstLayer) or (ThirdLayer))){
      if ( currentFrequency != previousFrequency ) {
        previousFrequency = currentFrequency;
        //bufferStatioName[0] = '\0';
        //stationName = '\0';
        tft.fillRect(XFreqDispl + 60, YFreqDispl + 54, 140, 20, TFT_BLACK);  // clear RDS text
      }
      if ((RDS) and  (NewSNR >= 12)) checkRDS(); 
      else  tft.fillRect(XFreqDispl + 60, YFreqDispl + 54, 140, 20, TFT_BLACK); // clear RDS text
  }
}

//=======================================================================================
void showtimeRSSI() {
//=======================================================================================
  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 3) // 150 * 10  = 1.5 sec refresh time RSSI
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
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "  STEREO" : "MONO");
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(&Serif_bold_10);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    tft.fillRect(XFreqDispl + 180, YFreqDispl + 10 , 50, 12, TFT_BLACK); // STEREO MONO
    //tft.drawString(buffer, XFreqDispl + 50, YFreqDispl + 20);
    tft.drawString(buffer, XFreqDispl + 200, YFreqDispl + 20);
    tft.setFreeFont(NULL);
  }
  rssi = NewRSSI;
  if ((FirstLayer) or (ThirdLayer)) Smeter();
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(&Serif_bold_10);
  tft.setTextSize(1);
  if ((FirstLayer) or (ThirdLayer)) {  // dBuV and volume at freq. display
    tft.fillRect(XFreqDispl + 7, YFreqDispl + 75 , 173, 10, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("RSSI = "+ String(NewRSSI) + " dBuV" , XFreqDispl + 10, YFreqDispl + 75);
    tft.setFreeFont(NULL);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setFreeFont(&Serif_bold_10);
    tft.setTextSize(1);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("SNR = " + String(NewSNR) + " dB", XFreqDispl + 225, YFreqDispl + 75);
    tft.setFreeFont(NULL);
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
      digitalWrite(Display_Led, displayon);
      DISplay = true;
    }
    int mainpurp = 1;
    
    if (bfoOn)  {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      mainpurp = 0;
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
    
    
    if (mainpurp == 1)
    {
     
      if (encoderCount == 1) {
        si4735.frequencyUp();
      } else {
        si4735.frequencyDown();
      }
      FreqDispl();
      band[bandIdx].currentFreq = si4735.getFrequency();
    }

    // Beta test
    if ( !FirstLayer && !AGCgainbut)  // If you move the encoder when you are on second leyer, so you will abort the current action. 
    {  
      cleanDispl();
      DrawFila(); 
      ThirdLayer = false;
      BroadBand = false;
      STEPbut = false;
      BandWidth = false;
      Modebut = false;
      HamBand = false;
      FREQbut = false;     
    }
    
    encoderCount = 0;
  }
}

//=======================================================================================
void encoderButtonCheck()  {
//=======================================================================================
  //Encoder button
  encBut = analogRead(ENCODER_SWITCH);
  if (encBut < 500) {
    Beep(1, 0);
    delay(400);
    if (DISplay == false) { //  Wake-up  Display
      DisplayOnTime = millis();
      digitalWrite(Display_Led, displayon);
      DISplay = true;
      return;
    }
    if (PRESbut == true) {// FM preset selection
      PRESbut = false;
      DrawDispl();
      return;
    }
    else {
      if (ssbLoaded) {  // SSB is on
        if (bfoOn) {
          bfoOn = false;
        }
        else {
          bfoOn = true;
        }
        //if (currentMode == FM) bfoOn = false;
        cleanDispl();  // TIDO --> Is that necessary?       
        drawBFO();
        DrawDispl();
      }
    }
  }
}

//=======================================================================================
void setStep()  {
//=======================================================================================
  // This command should work only for SSB mode
  if (bfoOn && (currentMode == LSB || currentMode == USB))
  {
    currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
  }
  else
  {
    si4735.setFrequencyStep(currentStep);
    band[bandIdx].currentStep = currentStep;
  }
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
  DrawButFila();
  DrawDispl();
  DrawSmeter();
  DrawVolumeIndicator();
}

//=======================================================================================
void DrawThla()  {  // Draw of Third layer
//=======================================================================================
  ThirdLayer = true;
  ForthLayer = false;
  tft.fillScreen(TFT_BLACK);
  DrawButThla();
  DrawDispl();
  DrawSmeter();
  DrawVolumeIndicator();
  DrawRDSbut();
  DrawAGCgainbut();
}

//=======================================================================================
void DrawButFila() { // Buttons first layer
//=======================================================================================
  //tft.fillScreen(TFT_BLACK);
  for (int n = 0 ; n <= lastButton; n++) {
    tft.fillRect(bt[bt[n].ButtonNum].XButos + Xbutst, bt[bt[n].ButtonNum].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
    tft.fillRect((bt[bt[n].ButtonNum].XButos + Xbutst + 3) , (bt[bt[n].ButtonNum].YButos + Ybutst + 3), (Xbutsiz - 6) , (Ybutsiz - 6), TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_15);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    tft.drawString((bt[n].ButtonNam), ( bt[bt[n].ButtonNum].XButos + Xbutst + (Xbutsiz / 2) ), (bt[bt[n].ButtonNum].YButos + Ybutst  + ((Ybutsiz) / 2 + 9)  ));
    tft.setFreeFont(NULL);
  }
  drawBFO();
  drawAGC();
}

//=======================================================================================
void DrawButThla() { // Buttons Third layer
//=======================================================================================
  //tft.fillScreen(TFT_BLACK);
  for (int n = 0 ; n <= lastButton; n++) {
    tft.fillRect(bt[bt[n].ButtonNum1].XButos + Xbutst, bt[bt[n].ButtonNum1].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
    tft.fillRect((bt[bt[n].ButtonNum1].XButos + Xbutst + 3) , (bt[bt[n].ButtonNum1].YButos + Ybutst + 3), (Xbutsiz - 6) , (Ybutsiz - 6), TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_15);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    tft.drawString((bt[n].ButtonNam1), ( bt[bt[n].ButtonNum1].XButos + Xbutst + (Xbutsiz / 2) ), (bt[bt[n].ButtonNum1].YButos + Ybutst  + ((Ybutsiz) / 2 + 9)  ));
    tft.setFreeFont(NULL);
    //tft.drawCentreString((bt[n].ButtonNam),( bt[bt[n].ButtonNum].XButos + Xbutst +(Xbutsiz/2) ),(bt[bt[n].ButtonNum].YButos+Ybutst  + ((Ybutsiz)/2+9)  ),1);
  }
  DrawRDSbut();
}


//=======================================================================================
void DrawVolumeIndicator()  {
//=======================================================================================
  tft.setFreeFont(&Serif_bold_10);
  tft.setTextSize(1);
  tft.fillRect(XVolInd, YVolInd, 240, 30, TFT_DARKGREY);
  tft.fillRect(XVolInd + 5, YVolInd + 5, 230, 20, TFT_BLACK);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setCursor(XVolInd +  15, YVolInd + 14);
  tft.print("0");
  tft.setCursor(XVolInd +  60, YVolInd + 14);
  tft.print("25");
  tft.setCursor(XVolInd + 110, YVolInd + 14);
  tft.print("50");
  tft.setCursor(XVolInd + 155, YVolInd + 14);
  tft.print("75");
  tft.setCursor(XVolInd + 200, YVolInd + 14);
  tft.print("100");
  tft.setFreeFont(NULL);
}

//=======================================================================================
void DrawSmeter()  {
//=======================================================================================
  String IStr;
  tft.setFreeFont(&Serif_bold_10);
  tft.setTextSize(1);
  tft.fillRect(Xsmtr, Ysmtr, 240, 55, TFT_DARKGREY);
  tft.fillRect(Xsmtr + 5, Ysmtr + 5, 230, 45, TFT_BLACK);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  for (int i = 0; i < 10; i++) {
    tft.fillRect(Xsmtr + 15 + (i * 12), Ysmtr + 24, 4, 8, TFT_ORANGE);
    IStr = String(i);
    tft.setCursor((Xsmtr + 14 + (i * 12)), Ysmtr + 17);
    tft.print(i);
  }
  for (int i = 1; i < 7; i++) {
    tft.fillRect((Xsmtr + 123 + (i * 16)), Ysmtr + 24, 4, 8, TFT_RED);
    IStr = String(i * 10);
    tft.setCursor((Xsmtr + 110 + (i * 16)), Ysmtr + 17);
    if ((i == 2) or (i == 4) or (i == 6))  {
      tft.print("+");
      tft.print(i * 10);
    }  
  }
  tft.fillRect(Xsmtr + 15, Ysmtr + 32 , 112, 4, TFT_ORANGE);
  tft.fillRect(Xsmtr + 127, Ysmtr + 32 , 100, 4, TFT_RED);
  tft.setFreeFont(NULL);
  // end Smeter
}

//=======================================================================================
void drawVOL()   {
//=======================================================================================
  int VOLbutcol;
  if (VOLbut) {
    VOLbutcol = TFT_RED;
  } else {
    VOLbutcol = TFT_BLUE;
  }
  tft.fillRect(bt[bt[5].ButtonNum].XButos + Xbutst, bt[bt[5].ButtonNum].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[5].ButtonNum].XButos + Xbutst + 3) , (bt[bt[5].ButtonNum].YButos + Ybutst + 3), (Xbutsiz - 6) , Ybutsiz - 6, VOLbutcol);
  tft.setTextColor(TFT_CYAN, VOLbutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString((bt[5].ButtonNam), ( bt[bt[5].ButtonNum].XButos + Xbutst + (Xbutsiz / 2)), (bt[bt[5].ButtonNum].YButos + Ybutst  + (Ybutsiz / 2 + 9)));
  tft.setFreeFont(NULL);
}

//=======================================================================================
void DrawAGCgainbut()  {
//=======================================================================================
if (ThirdLayer)  {  
  int AGCgainbutcol;
  if (AGCgainbut) {
    AGCgainbutcol = TFT_RED;
    tft.setTextColor(TFT_CYAN,TFT_RED );
    AGCgainbuttext = currentAGCgain;
  }else { 
    AGCgainbutcol = TFT_BLUE;
    tft.setTextColor(TFT_CYAN,TFT_BLUE );
    AGCgainbuttext = currentAGCgain;
  }
  tft.fillRect(bt[bt[4].ButtonNum1].XButos + Xbutst, bt[bt[4].ButtonNum1].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[4].ButtonNum1].XButos + Xbutst + 3) , (bt[bt[4].ButtonNum1].YButos + Ybutst + 3), (Xbutsiz - 6) , Ybutsiz - 6, AGCgainbutcol);
  tft.setTextColor(TFT_CYAN, AGCgainbutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString("AGC", ( bt[bt[4].ButtonNum1].XButos + Xbutst + (Xbutsiz/2)), (bt[bt[4].ButtonNum1].YButos + Ybutst  + (Ybutsiz/2+2)));
  tft.drawString(AGCgainbuttext,( bt[bt[4].ButtonNum1].XButos + Xbutst + (Xbutsiz/2)),(bt[bt[4].ButtonNum1].YButos+Ybutst  + (Ybutsiz/2+17)));
  tft.setFreeFont(NULL);
 }
}


//=======================================================================================
void DrawRDSbut()  {
//=======================================================================================
  int RDSbutcol;
  if (RDS) {
    RDSbutcol = TFT_RED;
    tft.setTextColor(TFT_CYAN,TFT_RED);
    RDSbuttext = "ON";
  }else{ 
    RDSbutcol = TFT_BLUE;
    tft.setTextColor(TFT_CYAN,TFT_BLUE );
    RDSbuttext = "OFF";
  }
  tft.fillRect(bt[bt[3].ButtonNum1].XButos + Xbutst, bt[bt[3].ButtonNum1].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[3].ButtonNum1].XButos + Xbutst + 3) , (bt[bt[3].ButtonNum1].YButos + Ybutst + 3), (Xbutsiz - 6) , Ybutsiz - 6, RDSbutcol);
  tft.setTextColor(TFT_CYAN, RDSbutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString("RDS", ( bt[bt[3].ButtonNum1].XButos + Xbutst + (Xbutsiz / 2)), (bt[bt[3].ButtonNum1].YButos + Ybutst  + (Ybutsiz / 2+2)));
  tft.drawString(RDSbuttext,( bt[bt[3].ButtonNum1].XButos + Xbutst + (Xbutsiz/2)),(bt[bt[3].ButtonNum1].YButos+Ybutst  + (Ybutsiz/2+17)));
  tft.setFreeFont(NULL);
}


//=======================================================================================
void drawMUTE()  {
//=======================================================================================
  int MUTEbutcol;
  if (Mutestat) {
    MUTEbutcol = TFT_RED;
    si4735.setAudioMute(audioMuteOn);
  } else {
    MUTEbutcol = TFT_BLUE;
    si4735.setAudioMute(audioMuteOff);
  }
  tft.fillRect(bt[bt[4].ButtonNum].XButos + Xbutst, bt[bt[4].ButtonNum].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[4].ButtonNum].XButos + Xbutst + 3) , (bt[bt[4].ButtonNum].YButos + Ybutst + 3), (Xbutsiz - 6) , Ybutsiz - 6, MUTEbutcol);
  tft.setTextColor(TFT_CYAN, MUTEbutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString((bt[4].ButtonNam), ( bt[bt[4].ButtonNum].XButos + Xbutst + (Xbutsiz / 2)), (bt[bt[4].ButtonNum].YButos + Ybutst  + (Ybutsiz / 2 + 9)));
  tft.setFreeFont(NULL);
}

//=======================================================================================
void drawAGC()  {
//=======================================================================================
  int AGCbutcol;
  si4735.getAutomaticGainControl();
  if (si4735.isAgcEnabled()) {
    AGCbutcol = TFT_RED;
  } else {
    AGCbutcol = TFT_BLUE;
  }
  tft.fillRect(bt[bt[3].ButtonNum].XButos + Xbutst, bt[bt[3].ButtonNum].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[3].ButtonNum].XButos + Xbutst + 3) , (bt[bt[3].ButtonNum].YButos + Ybutst + 3), (Xbutsiz - 6) , Ybutsiz - 6, AGCbutcol);
  tft.setTextColor(TFT_CYAN, AGCbutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString((bt[3].ButtonNam), ( bt[bt[3].ButtonNum].XButos + Xbutst + (Xbutsiz / 2)), (bt[bt[3].ButtonNum].YButos + Ybutst  + (Ybutsiz / 2 + 9)));
  tft.setFreeFont(NULL);
}

//=======================================================================================
void drawBFO ()  {
//=======================================================================================
  int BFObutcol;
  if (bfoOn) BFObutcol = TFT_RED;
  else BFObutcol = TFT_BLUE;
  tft.fillRect(bt[bt[1].ButtonNum].XButos + Xbutst, bt[bt[1].ButtonNum].YButos + Ybutst , Xbutsiz , Ybutsiz, TFT_DARKGREY);
  tft.fillRect((bt[bt[1].ButtonNum].XButos + Xbutst + 3) , (bt[bt[1].ButtonNum].YButos + Ybutst + 3), Xbutsiz - 6 , (Ybutsiz - 6), BFObutcol);
  tft.setTextColor(TFT_CYAN, BFObutcol);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.drawString((bt[1].ButtonNam), ( bt[bt[1].ButtonNum].XButos + Xbutst + (Xbutsiz / 2)), (bt[bt[1].ButtonNum].YButos + Ybutst  + (Ybutsiz / 2 + 9)));
  tft.setFreeFont(NULL);
}

//=======================================================================================
void drawKeyPath() {
//=======================================================================================
  int Sbutcol;
  tft.fillScreen(TFT_BLACK);
  for (int n = 0 ; n <= lastKPath; n++) {
    tft.fillRect(kp[n].Xkeypos + kp[n].Xkeypnr , kp[n].Ykeypos + kp[n].Ykeypnr , kp[n].Xkeypsr , kp[n].Ykeypsr, TFT_GREEN);
    if ( n == 11) { // S button is red
      Sbutcol = TFT_RED;
    } else {
      Sbutcol = TFT_BLUE;
    }
    tft.fillRect((kp[n].Xkeypos + kp[n].Xkeypnr + 3) , (kp[n].Ykeypos + kp[n].Ykeypnr + 3), (kp[n].Xkeypsr - 5) , (kp[n].Ykeypsr - 5), Sbutcol);
    tft.setTextColor(TFT_CYAN, Sbutcol );
    tft.setFreeFont(&Serif_bold_20);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(0);
    #ifdef IhaveHoriTFT
      tft.drawString((Keypathtext[kp[n].KeypNum]), ( kp[n].Xkeypos + kp[n].Xkeypnr + 50), (kp[n].Ykeypos + kp[n].Ykeypnr  + 37));
    #endif
    #ifdef IhaveVertTFT
      tft.drawString((Keypathtext[kp[n].KeypNum]), ( kp[n].Xkeypos + kp[n].Xkeypnr + 50), (kp[n].Ykeypos + kp[n].Ykeypnr  + 37));
    #endif
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void HamBandlist() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  for (int n = 0 ; n <= lastHam; n++) {
    tft.fillRect((bn[n].Xbandos + bn[n].Xbandnr) , (bn[n].Ybandos) + (bn[n].Ybandnr), (bn[n].Xbandsr) , (bn[n].Ybandsr), TFT_GREEN);
    tft.fillRect((bn[n].Xbandos + bn[n].Xbandnr + 3) , (bn[n].Ybandos) + (bn[n].Ybandnr + 3), (bn[n].Xbandsr - 6) , (bn[n].Ybandsr - 6), TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_20);
    tft.setTextSize(1);
    tft.drawString(band[bn[n].BandNum].bandName, (bn[n].Xbandos + bn[n].Xbandnr) + 70, (bn[n].Ybandos) + (bn[n].Ybandnr) + 30);
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void BroadBandlist() {
//=======================================================================================  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  for (int n = 0 ; n <= lastBroad; n++) {
    tft.fillRect((bb[n].Xbbandos + bb[n].Xbbandnr) , (bb[n].Ybbandos) + (bb[n].Ybbandnr), (bb[n].Xbbandsr) , (bb[n].Ybbandsr), TFT_GREEN);
    tft.fillRect((bb[n].Xbbandos + bb[n].Xbbandnr + 3) , (bb[n].Ybbandos) + (bb[n].Ybbandnr + 3), (bb[n].Xbbandsr - 6) , (bb[n].Ybbandsr - 6), TFT_BLUE);
    tft.setFreeFont(&Serif_bold_20);
    tft.setTextSize(1);
    tft.drawString(band[bb[n].BbandNum].bandName, (bb[n].Xbbandos + bb[n].Xbbandnr) + 50, (bb[n].Ybbandos) + (bb[n].Ybbandnr) + 30);
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void Steplist() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  for (int n = 0 ; n <= lastStep; n++) {
    tft.fillRect( (sp[n].Xstepos), (sp[n].Ystepos) + (sp[n].Ystepnr), (sp[n].Xstepsr), (sp[n].Ystepsr), TFT_GREEN);
    tft.fillRect( (sp[n].Xstepos + 3), sp[n].Ystepos + 3 + (sp[n].Ystepnr) , sp[n].Xstepsr - 6 , sp[n].Ystepsr - 6, TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_20);
    tft.setTextSize(1);
    tft.drawString(String(sp[n].stepFreq) + " kHz", (sp[n].Xstepos) + 100, (sp[n].Ystepos) + (sp[n].Ystepnr) + 37);
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void Modelist() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);  
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);  
  for (int n = 1 ; n <= lastMod; n++) {
    tft.fillRect( (md[n].Xmodos), (md[n].Ymodos) + (md[n].Ymodnr), (md[n].Xmodsr), (md[n].Ymodsr), TFT_GREEN);
    tft.fillRect( (md[n].Xmodos + 3), md[n].Ymodos + 3 + (md[n].Ymodnr) , md[n].Xmodsr - 6 , md[n].Ymodsr - 6, TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_20);  
    tft.setTextSize(1);
    tft.drawString(bandModeDesc[md[n].Modenum], (md[n].Xmodos) + 100, (md[n].Ymodos) + (md[n].Ymodnr) + 38);
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void BWList()  {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.setFreeFont(&Serif_bold_15);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.setTextPadding(0);
  tft.setFreeFont(NULL);
  if ( currentMode == AM) nrbox = 7;
  else nrbox = 6;
  for (int n = 0 ; n < nrbox; n++) {
    tft.fillRect( (bw[n].Xos), (bw[n].Yos) + (bw[n].Ynr), (bw[n].Xsr), (bw[n].Ysr), TFT_GREEN);
    tft.fillRect( (bw[n].Xos + 3), bw[n].Yos + 3 + (bw[n].Ynr) , bw[n].Xsr - 6 , bw[n].Ysr - 6, TFT_BLUE);
    tft.setTextColor(TFT_CYAN, TFT_BLUE);
    tft.setFreeFont(&Serif_bold_15);  
    tft.setTextSize(1);
    if ( currentMode == AM) tft.drawString(bandwidthAM[bw[n].BandWidthAM], (bw[n].Xos) + 100, (bw[n].Yos) + (bw[n].Ynr) + 25);
    else tft.drawString(bandwidthSSB[bw[n].BandWidthSSB], (bw[n].Xos) + 100, (bw[n].Yos) + (bw[n].Ynr) + 25);
    tft.setFreeFont(NULL);
  }
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.setFreeFont(&Serif_bold_15);  
  tft.setTextSize(1);
  if ( currentMode == AM)  tft.drawString("AM Bandw. in kHz"  , XfBW + 50, YfBW - 30);
  if ( currentMode == USB) tft.drawString("USB Bandw. in kHz" , XfBW + 50, YfBW - 30);
  if ( currentMode == LSB) tft.drawString("LSB Bandw. in kHz" , XfBW + 50, YfBW - 30);
  if ( currentMode == SYNC)  tft.drawString("SYNC Bandw. in kHz", XfBW + 50, YfBW - 30);
  tft.setFreeFont(NULL);
}

//=======================================================================================
void subrstatus() {
//=======================================================================================
  tft.fillScreen(TFT_BLACK);
  while (x == 0) {
    tft.setFreeFont(&Serif_bold_10);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Mod. : " + String(bandModeDesc[band[bandIdx].prefmod]), 150, 10);
    if ( currentMode != FM)  tft.drawString("Freq.    : " + String(currentFrequency, 0) + " kHz", 150, 20);
    else tft.drawString("Freq. : " + String(currentFrequency / 100, 1) + " MHz", 150, 20);
    si4735.getCurrentReceivedSignalQuality();
    tft.drawString("RSSI : " + String(si4735.getCurrentRSSI()) + "dBuV", 150, 30); // si4735.getCurrentSNR()
    tft.drawString("SNR : " + String(si4735.getCurrentSNR()) + "uV", 150, 40);
    if (  currentMode == FM ) {
      sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
      tft.drawString("         : " + String(buffer), 150, 50);
    }
    si4735.getAutomaticGainControl();
    si4735.getCurrentReceivedSignalQuality();
    tft.drawString("LNA GAIN : " + String(si4735.getAgcGainIndex()) + "/" + String(currentAGCAtt), 150, 60);
    tft.drawString("Volume : " + String(si4735.getVolume()), 150, 70);
    sprintf(buffer, "%s", (si4735.isAgcEnabled()) ? "AGC ON " : "AGC OFF");
    tft.drawString(buffer, 150, 80);
    if (bfoOn) tft.drawString("BFO ON  ", 150, 90);
    else tft.drawString("BFO OFF ", 150, 90);
    tft.drawString("AVC max GAIN : " + String(si4735.getCurrentAvcAmMaxGain()), 150, 100);
    tft.drawString("Ant. Cap = " + String(si4735.getAntennaTuningCapacitor()) , 150, 110);
    tft.setFreeFont(NULL);
    tft.setFreeFont(&Serif_bold_10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("BANDA : " + String(bandIdx) + "  " + String(band[bandIdx].bandName) , 150, 140);
    tft.drawString("FILTRO SSB : " + String(bandwidthSSB[bwIdxSSB]) + " kHz", 150, 150);
    tft.drawString("FILTRO AM : " + String(bandwidthAM[bwIdxAM]) + " kHz", 150, 160);
    tft.drawString("PASSO : " + String(currentStep), 150, 170);
    int vsupply = analogRead(ENCODER_SWITCH);
    tft.drawString("Power Supply : " + String(((1.66 / 1850)*vsupply) * 2) + " V.", 150, 180);
    tft.setFreeFont(NULL);
    tft.setFreeFont(&Serif_bold_15);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("SOFTWARE V2 BETA", 160, 205);
    tft.drawString("BY RICARDO - GERT - THIAGO", 160, 225);
    tft.setFreeFont(NULL);
    
    press1 = tft.getTouch(&x, &y);  
  }
  x=y=0;
  Beep(1, 0);
  delay(400);
}

//=======================================================================================
void showRDSStation() {
//=======================================================================================
  //if (strncmp(bufferStatioName, stationName,3) == 0 ) return;
  if ((FirstLayer) or (ThirdLayer)) {
     //tft.drawString(stationName, XFreqDispl + 120, YFreqDispl + 70);
     tft.setCursor(XFreqDispl + 130, YFreqDispl + 62);
     tft.print(stationName);  
     // TODO --> showContentWithoutBlink    
  }
  delay(250);
}

//=======================================================================================
void checkRDS() {
//=======================================================================================
  si4735.getRdsStatus();
  if (si4735.getRdsReceived()) {
    if (si4735.getRdsSync() && si4735.getRdsSyncFound() ) {
      stationName = si4735.getRdsText0A();
      tft.setTextSize(1);
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextDatum(BC_DATUM);
      if ( stationName != NULL)   showRDSStation();
    }
  }
}

/*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
void showContentWithoutBlink(int col, int line, char *oldValue, const char *newValue, uint32_t color, uint8_t space, uint8_t textSize)
{

  int c = col;

  char *pOld;
  char *pNew;

  pOld = oldValue;
  pNew = (char *) newValue;
  
  // prints just changed digits
  while (*pOld && *pNew)
  {
    if (*pOld != *pNew)
    {
      tft.drawChar(c, line, *pOld, TFT_BLACK, TFT_BLACK, textSize);
      tft.drawChar(c, line, *pNew, color, color, textSize);
    }

    pOld++;
    pNew++;
    c += space;
  }

  // Is there anything else to erase?
  while (*pOld)
  {
    tft.drawChar(c, line, *pOld, TFT_BLACK, TFT_BLACK, textSize);
    pOld++;
    c += space;
  }

  // Is there anything else to print?
  while (*pNew)
  {
    tft.drawChar(c, line, *pNew, color, color, textSize);
    pNew++;
    c += space;
  }
  strcpy(oldValue, newValue);
}


char *kHz =  "kHz";
char *mhz =  "MHz";

char * formatFrequency(char *strFreq) {

  char tmp[15];
  char *unt;

  sprintf(tmp, "%5.3u", si4735.getFrequency());  // TODO sprintf(tmp, "%5.3u", si4735.getFrequency());

  strFreq[0] = tmp[0];  
  strFreq[1] = tmp[1];
  if (si4735.isCurrentTuneFM())
  {
    strFreq[2] = tmp[2];
    strFreq[3] = tmp[3];
    strFreq[4] = '\0';
    unt = mhz;
  } else {
      strFreq[2] = tmp[2];
      strFreq[3] = tmp[3];
      strFreq[4] = tmp[4];
      strFreq[5] = '\0';
      unt = kHz;
  }

  return unt;;
}

inline void showContent(uint16_t col, uint16_t lin, char *oldContent, char *newContent, const GFXfont *font, uint16_t color, uint8_t space) {
  tft.setFreeFont(font);
  showContentWithoutBlink(col, lin, oldContent, newContent, color, space, 1);
  tft.setFreeFont(NULL);
}


void cleanDispl() {
tft.fillRect( XFreqDispl + 6, YFreqDispl + 22 , 228, 45, TFT_BLACK); // Black freq. field
 cleanBuffer();
} 



void showFrequency() {
  char tmpFrequency[10];
  char tmpVFO[10];
  char *untFreq; 
  if (bfoOn) {  
     sprintf(tmpVFO,"%5d",currentBFO);
     showContent(XFreqDispl + 95, YFreqDispl + 55, bufferVFO, tmpVFO, &DSEG7_Classic_Mini_Bold_20, TFT_ORANGE, 18);
     showContent(XFreqDispl + 192, YFreqDispl + 40, bufferAux, "Hz", &Serif_bold_10, TFT_GREEN, 10);
     showContent(XFreqDispl + 192, YFreqDispl + 55, bufferAux, "BFO", &Serif_bold_15, TFT_RED, 12);
     untFreq = formatFrequency(tmpFrequency);
     showContent(XFreqDispl + 5, YFreqDispl + 55, bufferFrequency, tmpFrequency, &DSEG7_Classic_Mini_Bold_20, TFT_CYAN, 18);
  } else {
     untFreq = formatFrequency(tmpFrequency);
     showContent(XFreqDispl + 60, YFreqDispl + 55, bufferFrequency, tmpFrequency, &DSEG7_Classic_Mini_Bold_30, TFT_CYAN, 26);
     if (band[bandIdx].bandType == FM_BAND_TYPE)
     tft.setFreeFont(&DSEG7_Classic_Mini_Bold_30);
     tft.drawChar(XFreqDispl + 60 + 78, YFreqDispl + 55, (si4735.isCurrentTuneFM())? '.':' ', TFT_CYAN, TFT_BLACK, 1);
     tft.setFreeFont(NULL);
     showContent(XFreqDispl + 192, YFreqDispl + 50, bufferUnit, untFreq, &Serif_bold_15, TFT_GREEN, 14);
     showContent(XFreqDispl + 10, YFreqDispl + 50, bufferBandName, (char *)band[bandIdx].bandName, &Serif_bold_15, TFT_RED, 12);
  }

}

void FreqDispl()
{

  char tmpAux[10];

  if (!FirstLayer && !ThirdLayer) // Nothing to do if you are on FirstLayer or ThirdLayer
    return;
  
  AGCfreqdisp();
  BFOfreqdisp();
  bufferAux[0] = '\0';
  // bufferAgcGain[0] = '\0';
  if (VOLbut) {
      sprintf(tmpAux,"%3d", (int) map(currentVOL,MinVOL,MaxVOL,0,100));
      showContent(XFreqDispl + 20, YFreqDispl + 50, bufferVolume, tmpAux, &DSEG7_Classic_Mini_Bold_20, TFT_CYAN, 20);
      showContent(XFreqDispl + 80, YFreqDispl + 50, bufferAux, " VOLUME", &Serif_bold_20, TFT_GREEN, 20);
  } else if (AGCgainbut) {
      sprintf(tmpAux,"%2.2d",currentAGCgain);
      showContent(XFreqDispl + 20, YFreqDispl + 50, bufferAux, tmpAux, &DSEG7_Classic_Mini_Bold_20, TFT_CYAN, 20);
      showContent(XFreqDispl + 80, YFreqDispl + 50, bufferAux, "ATT SET", &Serif_bold_20, TFT_GREEN, 20);
  } else {
    showFrequency();
  }

}


/**
 * Checks the stop seeking criterias.  
 * Returns true if the user press the touch or rotates the encoder. 
 */
bool checkStopSeeking() {
  // Checks the touch and encoder
  return (bool) encoderCount || tft.getTouch(&x, &y);   // returns true if the user rotates the encoder or touches on screen
} 


void SeekFreq (uint16_t freq) {
    if ((FirstLayer) || (ThirdLayer))  {
        currentFrequency = freq;
        showFrequency();
    }
}
   
//=======================================================================================
void DrawDispl() {
//=======================================================================================

  tft.fillRect(XFreqDispl, YFreqDispl, 240, 90, TFT_DARKGREY);
  tft.fillRect(XFreqDispl + 5, YFreqDispl + 5, 230, 80, TFT_BLACK);  
  FreqDispl();
 
  if (band[bandIdx].bandType != FM_BAND_TYPE) {
    tft.setFreeFont(&Serif_plain_10);
    tft.setTextSize(1);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString(bandModeDesc[currentMode], XFreqDispl + 81, YFreqDispl + 16);
    tft.setTextPadding(tft.textWidth("2.2kHz"));
    if (currentMode == AM) BWtext = bandwidthAM[bwIdxAM];
    else BWtext = bandwidthSSB[bwIdxSSB];
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("FT", XFreqDispl + 112, YFreqDispl + 17);
    tft.drawString(BWtext + "kHz", XFreqDispl + 144, YFreqDispl + 17);
    tft.setTextSize(1);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("ST", XFreqDispl + 183, YFreqDispl + 16);    
    tft.drawString(String(band[bandIdx].currentStep) + "kHz", XFreqDispl + 214, YFreqDispl + 16);
    tft.setFreeFont(NULL);
  }
}

//=======================================================================================
void AGCfreqdisp() {
//=======================================================================================
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(&Serif_plain_10);
  tft.setTextSize(1);
  tft.setTextDatum(BC_DATUM);
  tft.drawString("AGC", XFreqDispl + 50, YFreqDispl + 16);
  si4735.getAutomaticGainControl();
  if (si4735.isAgcEnabled()) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString("ON", XFreqDispl + 50, YFreqDispl + 26);    
  } else {
    if (AGCgain == 0)   {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString("OFF", XFreqDispl + 50, YFreqDispl + 26);
    } else {
      tft.drawString(String(currentAGCgain), XFreqDispl + 50, YFreqDispl + 26); 
      tft.setFreeFont(NULL); 
    }
  }
} 


//=======================================================================================
void BFOfreqdisp() {
//=======================================================================================
if (band[bandIdx].bandType != FM_BAND_TYPE) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setFreeFont(&Serif_plain_10);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.setTextPadding(tft.textWidth("XXX"));
    tft.drawString("BFO", XFreqDispl + 20, YFreqDispl + 16);
    tft.setTextPadding(tft.textWidth("88"));  
    if (bfoOn) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString(String(currentBFOStep), XFreqDispl + 20, YFreqDispl + 26);
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
      tft.drawString("  ", XFreqDispl + 20, YFreqDispl + 26);
      tft.setFreeFont(NULL);
    }
  }
}   

//=======================================================================================
void ErrorBeep()  {
//=======================================================================================
 Beep(4, 100);
 delay(2000);
}
