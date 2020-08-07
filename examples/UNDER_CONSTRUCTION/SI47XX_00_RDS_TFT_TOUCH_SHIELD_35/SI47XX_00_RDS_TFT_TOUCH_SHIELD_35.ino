/*
  
  Under construction...

  This sketch uses the mcufriend TFT touch Display Shield 3.5".
  You can use it on Mega2560 or DUE.

  REMEMBER:
  ALL Touch panels and wiring is DIFFERENT
  copy-paste results from TouchScreen_Calibr_native.ino that comes with MCUFRIEND_kbv library.
  ATTENTION: if you do not get success at first, check and change the XP, XM , YP and YM defined pins.


  Features:
  1) This sketch has been successfully tested on Arduino Mega2560 and Arduino DUE;
  2) It uses the touch screen interface provided by mcufriend TFT;
  3) Encoder;
  4) FM, AM (MW and SW) and SSB (LSB and USB);
  5) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
  6) BFO Control;
  7) RDS;
  8) Frequency step switch (1, 5 and 10KHz).

  Wire up

  Function                MEGA2560 or DUE  Pin
  ----------------------  --------------------
  SDA                     20
  SCL                     21
  ENCODER_A               18        - On Arduino DUE all Digital pin can be used with Interrupt
  ENCODER_B               19        - On Arduino DUE all Digital pin can be used with Interrupt
  RESET                   22
  ENCODER PUSH BUTTON     23
  AUDIO MUTE CIRCUIT      24        - Optional external audio mute circuit (avoid click/pop in the speaker)

  ATTENTION: Your toutch screen needs to be calibrated to work properly. 
             To do that, use the TouchScreen_Calibr_native.ino that comes with MCUFRIEND_kbv library.
             Read the TouchScreen_Calibr_native.ino and check the XP, XM , YP and YM pins configuration.
             You might need to change the XP, XM , YP and YM values in the TouchScreen_Calibr_native.ino 
             depending on the display you are using.

  This sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 8KB of memory.
  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
  There is little information available about patching the SI4735. The following information is the understanding of the author of
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device.
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by
  the host MCU (in this case the Arduino Mega2560). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
  Given this, it is at your own risk to continue with the procedures suggested here.
  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.

  Libraries used: SI4735; Adafruit_GFX; MCUFRIEND_kbv; FreeDefaultFonts; TouchScreen;

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Feb 2020
*/

#include <SI4735.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "DSEG7_Classic_Mini_Regular_30.h"

#include "Rotary.h"

#include "patch_init.h" // SSB patch for whole SSBRX initialization string

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define RESET_PIN 22           // Mega2560 digital Pin used to RESET
#define ENCODER_PUSH_BUTTON 23 // Used to switch BFO and VFO or other function
#define AUDIO_MUTE_CIRCUIT 24  // If you have an external mute circuit, use this pin to connect it.

// Enconder PINs (interrupt pins used on DUE. All Digital DUE Pins can be used as interrupt)
#define ENCODER_PIN_A 18
#define ENCODER_PIN_B 19

#define AM_FUNCTION 1
#define FM_FUNCTION 0

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150
#define ELAPSED_COMMAND 2000  // time to turn off the last command 
#define DEFAULT_VOLUME 45 // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4
#define SSB 1

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

bool cmdBFO = false;
bool cmdAudioMute = false;
bool cmdAntenna = false;
bool cmdVolume = false;  // if true, the encoder will control the volume.
bool cmdAgcAtt = false;
bool cmdFilter = false;
bool cmdStep = false;
bool cmdBand = false;
bool cmdSmute = false;


bool ssbLoaded = false;
bool fmStereo = true;
bool touch = false;

// AGC and attenuation control
uint8_t agcIdx = 0;
uint8_t disableAgc = 0;
uint8_t agcNdx = 0;

uint16_t antennaIdx = 0;

int8_t softMuteIdx = 0;

int currentBFO = 0;
int previousBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

long elapsedCommand = millis();

uint8_t rssi = 0;

// Encoder control variables
volatile int encoderCount = 0;

typedef struct
{
  const char *bandName;
  uint8_t bandType;     // Band type (FM, MW or SW)
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint16_t currentStep; // Defeult step (increment and decrement)
} Band;

/*
   Band table
*/
Band band[] = {
    {"FM  ", FM_BAND_TYPE, 8400, 10800, 10390, 10},
    {"LW  ", LW_BAND_TYPE, 100, 510, 300, 1},
    {"AM  ", MW_BAND_TYPE, 520, 1720, 810, 10},
    {"160m", SW_BAND_TYPE, 1800, 3500, 1900, 1}, // 160 meters
    {"80m ", SW_BAND_TYPE, 3500, 4500, 3700, 1}, // 80 meters
    {"60m ", SW_BAND_TYPE, 4500, 5500, 4850, 5},
    {"49m ", SW_BAND_TYPE, 5600, 6300, 6000, 5},
    {"40m ", SW_BAND_TYPE, 6800, 7200, 7100, 1}, // 40 meters
    {"41m ", SW_BAND_TYPE, 7200, 7900, 7205, 5}, // 41 meters
    {"31m ", SW_BAND_TYPE, 9200, 10000, 9600, 5},
    {"30m ", SW_BAND_TYPE, 10000, 11000, 10100, 1}, // 30 meters
    {"25m ", SW_BAND_TYPE, 11200, 12500, 11940, 5},
    {"22m ", SW_BAND_TYPE, 13400, 13900, 13600, 5},
    {"20m ", SW_BAND_TYPE, 14000, 14500, 14200, 1}, // 20 meters
    {"19m ", SW_BAND_TYPE, 15000, 15900, 15300, 5},
    {"18m ", SW_BAND_TYPE, 17200, 17900, 17600, 5},
    {"17m ", SW_BAND_TYPE, 18000, 18300, 18100, 1}, // 17 meters
    {"15m ", SW_BAND_TYPE, 21000, 21499, 21200, 1}, // 15 mters
    {"13m ", SW_BAND_TYPE, 21500, 21900, 21525, 5}, // 15 mters
    {"12m ", SW_BAND_TYPE, 24890, 26200, 24940, 1}, // 12 meters
    {"CB  ", SW_BAND_TYPE, 26200, 27900, 27500, 1}, // CB band (11 meters)
    {"10m ", SW_BAND_TYPE, 28000, 30000, 28400, 1},
    {"All ", SW_BAND_TYPE, 100, 30000, 15000, 1} // All HF in one band
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int lastSwBand = 7; // Saves the last SW band used


int tabStep[] = {1,5,10,50,100,500,1000};
const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
int idxStep = 0;

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
uint16_t currentStep = 1;
uint8_t currentBFOStep = 25;

int8_t bwIdxSSB = 2;
const char *bandwitdthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

int8_t bwIdxAM = 1;
const char *bandwitdthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

char buffer[255];
char bufferFreq[10];
char bufferStereo[10];
char bufferCapacitor[10];
char bufferCapacitorRead[10];
char bufferBFO[15];
char bufferStep[15];
char bufferUnit[10];
char bufferVolume[10];

char bufferAGC[15];
char bufferBW[20];
char bufferMode[15];
char bufferBandName[15];
char bufferSoftMute[15];


Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
MCUFRIEND_kbv tft;
SI4735 si4735;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
// ATTENTION: if you do not get success at first, check and change the XP, XM , YP and YM pins.
// Read TouchScreen_Calibr_native.ino  (MCUFRIEND shield shares pins with the TFT). 

// TFT Touch shield  (my old and original MFUFIEND toutch screen)
// const int XP = 6, XM = A2, YP = A1, YM = 7; //240x320 ID=0x9328
// const int TS_LEFT = 170, TS_RT = 827, TS_TOP = 130, TS_BOT = 868;

// TFT Touch shield 2 (my new kind of mcufriend toutch screen)
//const int XP=7,XM=A1,YP=A2,YM=6; //240x320 ID=0x2053
// const int TS_LEFT=155,TS_RT=831,TS_TOP=158,TS_BOT=892;

// TFT 3.5" MCUFRIEND
const int XP=7,XM=A1,YP=A2,YM=6; //320x480 ID=0x6814
const int TS_LEFT=149,TS_RT=846,TS_TOP=120,TS_BOT=918;


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 480);
Adafruit_GFX_Button bNextBand, bPreviousBand, bVolumeLevel, bSeekUp, bSeekDown, bStep, bAudioMute, bAM, bLSB, bUSB, bFM, bMW, bSW, bFilter, bAGC, bSoftMute, bCapAmi, bCapAuto, bBFO;

int pixel_x, pixel_y; //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT); //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH); //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed)
  {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void setup(void)
{

  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  pinMode(ENCODER_PUSH_BUTTON, INPUT_PULLUP);

  si4735.setAudioMuteMcuPin(AUDIO_MUTE_CIRCUIT);

  uint16_t ID = tft.readID();

  if (ID == 0xD3D3)
    ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0); //PORTRAIT
  
  tft.fillScreen(BLACK);

  // tft.setFont(&FreeSans12pt7b);
  showText(75, 30, 3, NULL, GREEN, "SI4735");
  showText(75, 90, 3, NULL, YELLOW, "Arduino");
  showText(75, 160, 3, NULL, YELLOW, "Library");
  showText(70, 240, 3, NULL, WHITE, "By PU2CLR");
  showText(50, 340, 1, NULL, WHITE, "https://pu2clr.github.io/SI4735/");
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
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
    showText(55, 440, 1, NULL, RED, buffer);
  }
  delay(5000);

  tft.fillScreen(BLACK);

  showTemplate();

  // Atach Encoder pins interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, 1);
  // si4735.setup(RESET_PIN, -1, POWER_UP_FM, SI473X_ANALOG_AUDIO, XOSCEN_CRYSTAL);

  // Set up the radio for the current band (see index table variable bandIdx )
  delay(100);
  useBand();
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(DEFAULT_VOLUME);
  tft.setFont(NULL); // default font
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

/**
 * Disable all commands
 * 
 */
void disableCommands() {
    cmdBFO = false;
    cmdAudioMute = false;
    cmdAntenna = false;
    cmdVolume = false;  // if true, the encoder will control the volume.
    cmdAgcAtt = false;
    cmdFilter = false;
    cmdStep = false;
    cmdBand = false;
    cmdSmute = false;
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
  tft.setFont(f);
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
  pNew = (char *) newValue;

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

void showTemplate()
{

  int w = tft.width();

  // Area used to show the frequency
  tft.drawRect(0, 0, w, 50, WHITE);

  tft.drawRect(0, 100, w, 270, CYAN);
  tft.setFont(NULL);
  
  bPreviousBand.initButton(&tft, 45, 130, 60, 40, WHITE, CYAN, BLACK, (char *)"Band-", 1);
  bNextBand.initButton(&tft, 120, 130, 60, 40, WHITE, CYAN, BLACK, (char *)"Band+", 1);
  bVolumeLevel.initButton(&tft, 195, 130, 60, 40, WHITE, CYAN, BLACK, (char *)"Vol", 1);
  bAudioMute.initButton(&tft, 270, 130, 60, 40, WHITE, CYAN, BLACK, (char *)"Mute", 1);

  bSeekDown.initButton(&tft, 45, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"Seek-", 1);
  bSeekUp.initButton(&tft, 120, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"Seek+", 1);
  bBFO.initButton(&tft, 195, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"BFO", 1);
  bStep.initButton(&tft, 270, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"Step", 1);

  bFM.initButton(&tft, 45, 235, 60, 40, WHITE, CYAN, BLACK, (char *)"FM", 1);
  bMW.initButton(&tft, 120, 235, 60, 40, WHITE, CYAN, BLACK, (char *)"MW", 1);
  bSW.initButton(&tft, 195, 235, 60, 40, WHITE, CYAN, BLACK, (char *)"SW", 1);
  bAGC.initButton(&tft, 270, 235, 60, 40, WHITE, CYAN, BLACK, (char *)"ATT", 1);

  bAM.initButton(&tft, 45, 285, 60, 40, WHITE, CYAN, BLACK, (char *)"AM", 1);
  bLSB.initButton(&tft, 120, 285, 60, 40, WHITE, CYAN, BLACK, (char *)"LSB", 1);
  bUSB.initButton(&tft, 195, 285, 60, 40, WHITE, CYAN, BLACK, (char *)"USB", 1);
  bFilter.initButton(&tft, 270, 285, 60, 40, WHITE, CYAN, BLACK, (char *)"|Y|", 1);

  bSoftMute.initButton(&tft, 45, 335, 60, 40, WHITE, CYAN, BLACK, (char *)"SMute", 1);
  bCapAmi.initButton(&tft, 120, 335, 60, 40, WHITE, CYAN, BLACK, (char *)"AntM", 1);
  bCapAuto.initButton(&tft, 195, 335, 60, 40, WHITE, CYAN, BLACK, (char *)"AntA", 1);

  // Exibe os botões (teclado touch)
  bNextBand.drawButton(true);
  bPreviousBand.drawButton(true);
  bVolumeLevel.drawButton(true);
  bAudioMute.drawButton(true);
  bSeekUp.drawButton(true);
  bSeekDown.drawButton(true);
  bStep.drawButton(true);
  bBFO.drawButton(true);
  bFM.drawButton(true);
  bMW.drawButton(true);
  bSW.drawButton(true);
  bAM.drawButton(true);
  bLSB.drawButton(true);
  bUSB.drawButton(true);
  bFilter.drawButton(true);
  bAGC.drawButton(true);
  bSoftMute.drawButton(true);
  bCapAmi.drawButton(true);
  bCapAuto.drawButton(true);

  showText(0, 392, 1, NULL, GREEN, "RSSI");
  tft.drawRect(30, 388, (w - 32), 12, CYAN);

  showText(0, 450, 1, NULL, YELLOW, "PU2CLR-Si4535 Arduino Library-Example");
  showText(0, 465, 1, NULL, YELLOW, "DIY - You can do it better.");

  tft.setFont(NULL);
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

  char aux[15];
  char sFreq[15];

  tft.setFont(&DSEG7_Classic_Mini_Regular_30);
  tft.setTextSize(1);
  if (si4735.isCurrentTuneFM())
  {
    sprintf(aux, "%5.5d", currentFrequency);
    sFreq[0] = (aux[0] == '0') ? ' ' : aux[0];
    sFreq[1] = aux[1];
    sFreq[2] = aux[2];
    sFreq[3] = aux[3];
    sFreq[4] = '\0';

    tft.drawChar(139, 37, '.', YELLOW, BLACK, 1);
  }
  else
  {
    sprintf(sFreq, "%5d", currentFrequency);
    tft.drawChar(139, 37, '.', BLACK, BLACK, 1);
  }

  color = (cmdBFO) ? CYAN : YELLOW;
    
  showFrequencyValue(50, 37, bufferFreq, sFreq, color, 30, 1);

  if (currentMode == LSB || currentMode == USB)
  {
    showBFO();
  }

  tft.setFont(NULL); // default font
  showCapacitor();
}

/**
 * Shows the frequency during seek process
 * 
 */
void showFrequencySeek(uint16_t freq)
{
  previousFrequency = currentFrequency = freq;
  showFrequency();
}



/**
 * Clears status area
 * 
 */
void clearStatusArea() {
  tft.fillRect(0, 60, tft.width(), 36, BLACK); // Clear all status area
}


/**
 * Clears frequency area
 * 
 */
void clearFrequencyArea() {
  tft.fillRect(2, 2, tft.width() - 4, 46, BLACK);
}


int getStepIndex(int st) {
  for (int i = 0; i < lastStep; i++) {
    if ( st == tabStep[i] ) return i;
  }
  return 0;
}


/**
 * Clears buffer control variable
 * 
 */
void clearBuffer()
{
  bufferAGC[0] = '\0';
  bufferBW[0] = '\0';
  bufferMode[0] = '\0';
  bufferBandName[0] = '\0';
  bufferMode[0] = '\0';
  bufferBFO[0] = '\0';
  bufferFreq[0] = '\0';
  bufferUnit[0] = '\0';
  bufferCapacitorRead[0] = '\0';
  bufferVolume[0] = '\0';
  bufferStep[0] = '\0';
  bufferSoftMute[0] = '\0';
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

  si4735.getStatus();
  si4735.getCurrentReceivedSignalQuality();

  si4735.getFrequency();
  showFrequency();

  showVolume();

  tft.setFont(NULL); // default font
  printText(5, 5, 2, bufferBandName, band[bandIdx].bandName, CYAN, 11);

  if (band[bandIdx].bandType == SW_BAND_TYPE)
  {
    printText(5, 30, 2, bufferMode, bandModeDesc[currentMode], CYAN, 11);
  }
  else
  {
    printText(5, 30, 2, bufferMode, "    ", BLACK, 11);
  }

  if (si4735.isCurrentTuneFM())
  {
    printText(250, 30, 2, bufferUnit, "MHz", WHITE, 12);
    return;
  }

  printText(250, 30, 2, bufferUnit, "KHz", WHITE, 12);

  showBandwitdth();
  showAgcAtt();
  showStep();
  showSoftMute();

}


/**
 * SHow bandwitdth on AM or SSB mode
 * 
 */
void showBandwitdth()
{
  char bw[20];

  tft.setFont(NULL);
  if (currentMode == LSB || currentMode == USB)
  {
    sprintf(bw, "BW:%s KHz", bandwitdthSSB[bwIdxSSB]);
    printText(5, 85, 1, bufferBW, bw, GREEN, 7);
    showBFO();
  }
  else if (currentMode == AM)
  {
    sprintf(bw, "BW:%s KHz", bandwitdthAM[bwIdxAM]);
    printText(5, 85, 1, bufferBW, bw, GREEN, 7);
  }
}

/**
 * Shows AGC and Attenuation status
 * 
 */
void showAgcAtt() {
  char sAgc[15];
  
  si4735.getAutomaticGainControl();
  if (agcNdx == 0 && agcIdx == 0)
    strcpy(sAgc, "AGC ON");
  else
  {
    sprintf(sAgc, "ATT: %2d", agcNdx);
  }
  tft.setFont(NULL);
  printText(90, 85, 1, bufferAGC, sAgc, GREEN, 7);
}

/**
 * Shows RSSI level
 * 
 */
void showRSSI()
{

  int w = tft.width();
  int signalLevel;

  if (currentMode == FM)
  {
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    printText(240, 10, 1, bufferStereo, buffer, GREEN, 11);
  } else {
    sprintf(buffer, "%s", "      ");
    printText(240, 10, 1, bufferStereo, buffer, BLACK, 11);
  }


  signalLevel = map(rssi, 0, 63, 0, w - 34);
  tft.fillRect(30, 390, w-34, 8, BLACK);
  tft.fillRect(30, 390, signalLevel, 8, (signalLevel > 25) ? YELLOW : RED);
}

void showStep() {

  char sStep[15];
  tft.setFont(NULL); // default font

  sprintf(sStep, "Step: %4d",currentStep);
  printText(240, 60, 1, bufferStep, sStep, GREEN, 7);

}


void showSoftMute() {
  
 char sMute[15];
 tft.setFont(NULL); // default font

 sprintf(sMute, "SMute: %2d", softMuteIdx);
 printText(160, 85, 1, bufferSoftMute, sMute, GREEN, 7);

}

void showBFO()
{
  tft.setFont(NULL); // default font
  sprintf(buffer, "%c%d", (currentBFO>=0)?'+':'-',abs(currentBFO));
  printText(200, 4, 2, bufferBFO, buffer, YELLOW, 11);
}

/**
 * Shows the current volume level
 * 
 */
void showVolume()
{
  char sVolume[15];

  sprintf(sVolume, "Vol: %2.2d",si4735.getVolume());
  printText(260, 85, 1, bufferVolume, sVolume, GREEN, 7);
}

// Internal tuning capacitor test
void showCapacitor() {
  /*
  uint16_t capValue = si4735.getAntennaTuningCapacitor();
  sprintf(buffer, "%d", antennaIdx);
  printText(10, 430, 2, bufferCapacitor, buffer, YELLOW, 11);
  sprintf(buffer, "%d", capValue);
  printText(100, 430, 2, bufferCapacitorRead, buffer, YELLOW, 11);
  */
}

char *rdsMsg;
char *stationName;
char *rdsTime;
char bufferStatioName[255];
char bufferRdsMsg[255];
char bufferRdsTime[32];

void showRDSMsg()
{
  if (strcmp(bufferRdsMsg, rdsMsg) == 0)
    return;
  printText(5, 85, 1, bufferRdsMsg, rdsMsg, GREEN, 6);
  delay(250);
}

void showRDSStation()
{
  if (strcmp(bufferStatioName, stationName) == 0)
    return;
  printText(5, 60, 1, bufferStatioName, stationName, GREEN, 6);
  delay(250);
}

void showRDSTime()
{

  if (strcmp(bufferRdsTime, rdsTime) == 0)
    return;
  printText(150, 60, 1, bufferRdsTime, rdsTime, GREEN, 6);
  delay(250);
}

void checkRDS()
{

  si4735.getRdsStatus();
  if (si4735.getRdsReceived())
  {
    if (si4735.getRdsSync() && si4735.getRdsSyncFound())
    {
      rdsMsg = si4735.getRdsText2A();
      stationName = si4735.getRdsText0A();
      rdsTime = si4735.getRdsTime();
      if (rdsMsg != NULL)
        showRDSMsg();
      if (stationName != NULL)
        showRDSStation();
      if (rdsTime != NULL)
        showRDSTime();
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
  band[bandIdx].currentStep = currentStep;

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
  band[bandIdx].currentStep = currentStep;
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
  si4735.reset();
  si4735.queryLibraryId(); // Is it really necessary here? I will check it.
  si4735.patchPowerUp();
  delay(50);
  si4735.setI2CFastMode(); // Recommended
  // si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
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
  si4735.setSSBConfig(bwIdxSSB, 1, 0, 0, 0, 1);
  delay(25);
  ssbLoaded = true;
}

/*
   Switch the radio to current band
*/
void useBand()
{
  cmdAntenna = false;

  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    // si4735.setFMDeEmphasis(1); // 1 = 50 μs. Used in Europe, Australia, Japan;
    si4735.setSeekFmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);
    // Define here the best criteria to find a FM station during the seeking process 
    // si4735.setSeekFmSpacing(10); // frequency spacing for FM seek (5, 10 or 20. They mean 50, 100 or 200 KHz)
    // si4735.setSeekAmRssiThreshold(0);
    // si4735.setSeekFmSrnThreshold(3);
    
    cmdBFO = ssbLoaded = false;
    si4735.setRdsConfig(1, 2, 2, 2, 2);
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
    {
      antennaIdx = 0;
      si4735.setTuneFrequencyAntennaCapacitor(antennaIdx);
    }
    else
    {
      antennaIdx = 1;
      lastSwBand = bandIdx;
      si4735.setTuneFrequencyAntennaCapacitor(antennaIdx);
    }

    if (ssbLoaded)
    {
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      si4735.setSSBAutomaticVolumeControl(1);
      si4735.setSsbSoftMuteMaxAttenuation(softMuteIdx); // Disable Soft Mute for SSB
      showBFO();
    }
    else
    {
      currentMode = AM;
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      si4735.setAmSoftMuteMaxAttenuation(softMuteIdx); // // Disable Soft Mute for AM
      cmdBFO = false;
    }

    // Sets the seeking limits and space.
    si4735.setSeekAmLimits(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq);               // Consider the range all defined current band
    si4735.setSeekAmSpacing((band[bandIdx].currentStep > 10) ? 10 : band[bandIdx].currentStep); // Max 10KHz for spacing
  }
  delay(100);

  // Sets AGC or attenuation control
  si4735.setAutomaticGainControl(disableAgc, agcNdx);

  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  idxStep = getStepIndex(currentStep);

  showStatus();
}

void switchAgc(int8_t v) {

  agcIdx = (v == 1) ? agcIdx + 1 : agcIdx - 1;

  if (agcIdx == 0)
  {
    disableAgc = 0; // Turns AGC ON
    agcNdx = 0;
  }
  else if (agcIdx == 1)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 0;     // Sets minimum attenuation
  }
  else if (agcIdx == 2)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 5;     // Increases the attenuation AM/SSB AGC Index  = 10
  }
  else if (agcIdx == 3)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 10;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx == 4)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 15;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx == 5)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 20;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx == 6)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 25;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx == 7)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 30;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx == 8)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 35;    // Increases the attenuation AM/SSB AGC Index  = 30
  }
  else if (agcIdx >= 9)
  {
    disableAgc = 1; // Turns AGC OFF
    agcNdx = 0;     // Increases the attenuation AM/SSB AGC Index  = 30
    agcIdx = 0;
  }
  // Sets AGC on/off and gain
  si4735.setAutomaticGainControl(disableAgc, agcNdx);
  showAgcAtt();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  elapsedCommand = millis();
}

void switchFilter(uint8_t v) {
  if (currentMode == LSB || currentMode == USB)
  {
    bwIdxSSB = ( v == 1)? bwIdxSSB+1:bwIdxSSB-1;

    if (bwIdxSSB > 5)
       bwIdxSSB = 0;
    else if ( bwIdxSSB < 0 ) 
      bwIdxSSB = 5;

    si4735.setSSBAudioBandwidth(bwIdxSSB);
    // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
    if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
      si4735.setSBBSidebandCutoffFilter(0);
    else
      si4735.setSBBSidebandCutoffFilter(1);
  }
  else if (currentMode == AM)
  {
    bwIdxAM = ( v == 1)? bwIdxAM+1:bwIdxAM-1;

    if (bwIdxAM > 6)
      bwIdxAM = 0;
    else if ( bwIdxAM < 0) 
      bwIdxAM = 6;  
      
    si4735.setBandwidth(bwIdxAM, 1);
  }
  showBandwitdth();
  elapsedCommand = millis();
  delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
}


void switchStep(int8_t v) {

   // This command should work only for SSB mode
    if (cmdBFO && (currentMode == LSB || currentMode == USB))
    {
      currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
      showBFO();
    }
    else
    {
      idxStep = ( v == 1 )? idxStep + 1 : idxStep - 1;
      if ( idxStep > lastStep) 
         idxStep = 0;
      else if ( idxStep < 0 )
         idxStep = lastStep;   

      currentStep = tabStep[idxStep];

      si4735.setFrequencyStep(currentStep);
      band[bandIdx].currentStep = currentStep;
      si4735.setSeekAmSpacing((currentStep > 10) ? 10 : currentStep); // Max 10KHz for spacing
      showStep();
    }
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    elapsedCommand = millis();
}

void switchSoftMute( int8_t v) {
  softMuteIdx = (v == 1)? softMuteIdx + 1 : softMuteIdx - 1;
  if (softMuteIdx > 16) 
     softMuteIdx = 0; 
  else if (softMuteIdx < 0) 
     softMuteIdx = 16;

   si4735.setAmSoftMuteMaxAttenuation(softMuteIdx);
   showSoftMute();
   elapsedCommand = millis();
}

/**
 * @brief Proccess the volume level
 * 
 * @param v 1 = Up; !1 = down
 */
void doVolume(int8_t v) {

  if ( v == 1) 
    si4735.volumeUp();
   else
    si4735.volumeDown();

  showVolume();
  elapsedCommand = millis();
}


void doBFO() {
  bufferFreq[0] = '\0';
  cmdBFO = !cmdBFO;
  if (cmdBFO)
  {
    bBFO.initButton(&tft, 195, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"VFO", 1);
    showBFO();
  } else {
    bBFO.initButton(&tft, 195, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"BFO", 1);
  }
  bBFO.drawButton(true);
  showStatus();
  elapsedCommand = millis();
}

/* two buttons are quite simple
*/
void loop(void)
{
  bool down = Touch_getXY();
  bNextBand.press(down && bNextBand.contains(pixel_x, pixel_y));
  bPreviousBand.press(down && bPreviousBand.contains(pixel_x, pixel_y));
  bVolumeLevel.press(down && bVolumeLevel.contains(pixel_x, pixel_y));
  bBFO.press(down && bBFO.contains(pixel_x, pixel_y));
  bSeekUp.press(down && bSeekUp.contains(pixel_x, pixel_y));
  bSeekDown.press(down && bSeekDown.contains(pixel_x, pixel_y));
  bStep.press(down && bStep.contains(pixel_x, pixel_y));
  bAudioMute.press(down && bAudioMute.contains(pixel_x, pixel_y));
  bFM.press(down && bFM.contains(pixel_x, pixel_y));
  bMW.press(down && bMW.contains(pixel_x, pixel_y));
  bSW.press(down && bSW.contains(pixel_x, pixel_y));
  bAM.press(down && bAM.contains(pixel_x, pixel_y));
  bLSB.press(down && bLSB.contains(pixel_x, pixel_y));
  bUSB.press(down && bUSB.contains(pixel_x, pixel_y));
  bFilter.press(down && bFilter.contains(pixel_x, pixel_y));
  bAGC.press(down && bAGC.contains(pixel_x, pixel_y));
  bSoftMute.press(down && bSoftMute.contains(pixel_x, pixel_y));
  bCapAmi.press(down && bCapAmi.contains(pixel_x, pixel_y));
  bCapAuto.press(down && bCapAuto.contains(pixel_x, pixel_y));

  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (cmdBFO)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      si4735.setSSBBfo(currentBFO);
      showBFO();
      elapsedCommand = millis();
    }
    else if (cmdVolume)
    {
      doVolume(encoderCount);
    }
    else if (cmdAgcAtt)
    {
      switchAgc(encoderCount);
    }
    else if (cmdFilter) {
      switchFilter(encoderCount);
    } 
    else if (cmdStep) {
      switchStep(encoderCount);
    }
    else if (cmdSmute) {
      switchSoftMute(encoderCount);
    } else if ( cmdBand ) {
      if (encoderCount == 1) 
         bandUp();
      else
      {
        bandDown();
      }
      elapsedCommand = millis();   
    }
    else if ( cmdAntenna ) {
      antennaIdx = (encoderCount == 1) ? (antennaIdx + currentStep) : (antennaIdx - currentStep);
      si4735.setTuneFrequencyAntennaCapacitor(antennaIdx);
      showCapacitor();
    } 
    else
    {
      if (encoderCount == 1)
        si4735.frequencyUp();
      else
        si4735.frequencyDown();

      // Show the current frequency only if it has changed
      currentFrequency = si4735.getFrequency();
      showFrequency();
    }
    encoderCount = 0;
  }

  if (bNextBand.justPressed())
    bandUp();

  if (bPreviousBand.justPressed())
    bandDown();

  // Volume
  if (bVolumeLevel.justPressed())
  {
    disableCommands();
    si4735.setAudioMute(cmdAudioMute);
    cmdVolume = true;
    delay(MIN_ELAPSED_TIME);
  }

  // Mute
  if (bAudioMute.justPressed())
  {
    cmdAudioMute = !cmdAudioMute;
    si4735.setAudioMute(cmdAudioMute);
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  if (bBFO.justPressed())
  {
    if (currentMode == LSB || currentMode == USB) {
      doBFO();
    }
    delay(MIN_ELAPSED_TIME);
  }

  // SEEK Test
  if (bSeekUp.justPressed())
  {
      si4735.seekStationProgress(showFrequencySeek, SEEK_UP);
      // si4735.seekNextStation(); // This method does not show the progress
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
      currentFrequency = si4735.getFrequency();
      showStatus();
  }

  if (bSeekDown.justPressed())
  {
      si4735.seekStationProgress(showFrequencySeek, SEEK_DOWN);
      // si4735.seekPreviousStation(); // This method does not show the progress
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
      currentFrequency = si4735.getFrequency();
      showStatus();
  }


  // SMute
  if (bSoftMute.justPressed())
  {
    disableCommands();
    cmdSmute = true;
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  if (bCapAmi.justPressed())
  {
    cmdAntenna = !cmdAntenna;  
    showCapacitor();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  if (bCapAuto.justPressed())
  {
    disableCommands();
    antennaIdx = (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)? 0:1;
    si4735.setTuneFrequencyAntennaCapacitor(antennaIdx);
    showCapacitor();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  if (bAM.justPressed())
  {
    if (currentMode != FM)
    {
      currentMode = AM;
      ssbLoaded = false;
      cmdBFO = false;
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
      showFrequency();
    }
  }

  if (bFM.justPressed())
  {
    if (currentMode != FM)
    {
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      ssbLoaded = false;
      cmdBFO = false;
      currentMode = FM;
      bandIdx = 0;
      useBand();
      showFrequency();
    }
  }

  if (bMW.justPressed())
  {
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    ssbLoaded = false;
    cmdBFO = false;
    currentMode = AM;
    bandIdx = 2; // See Band table
    useBand();
  }

  if (bSW.justPressed())
  {
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    ssbLoaded = false;
    cmdBFO = false;
    currentMode = AM;
    bandIdx = lastSwBand; // See Band table
    useBand();
  }

  if (bLSB.justPressed())
  {
    if (currentMode != FM)
    {
      if (!ssbLoaded)
      {
        loadSSB();
      }
      currentMode = LSB;
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
    }
  }

  if (bUSB.justPressed())
  {
    if (currentMode != FM)
    {
      if (!ssbLoaded)
      {
        loadSSB();
      }
      currentMode = USB;
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
    }
  }

  // AGC and attenuation control
  if (bAGC.justPressed())
  {
    disableCommands();
    cmdAgcAtt = true;
    delay(MIN_ELAPSED_TIME);
  }

  if (bFilter.justPressed())
  {
    disableCommands();
    cmdFilter =  true;
    delay(MIN_ELAPSED_TIME);
  }

  if (bStep.justPressed())
  {
    // switchStep();
    disableCommands();
    cmdStep = true;
    delay(MIN_ELAPSED_TIME);
  }

  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)
  {
    if (currentMode == LSB || currentMode == USB) {
        doBFO();
    } else {
      cmdBand = !cmdBand;
    }
    delay(250);
  }

  // Show RSSI status only if this condition has changed
  if ((millis() - elapsedRSSI) > MIN_ELAPSED_RSSI_TIME * 12)
  {
    si4735.getCurrentReceivedSignalQuality();
    int aux = si4735.getCurrentRSSI();
    if (rssi != aux)
    {
      rssi = aux;
      showRSSI();
    }
    elapsedRSSI = millis();
  }

  if (currentMode == FM)
  {
    if (currentFrequency != previousFrequency)
    {
      clearStatusArea();
      bufferVolume[0] = '\0';
      showVolume();
      bufferStatioName[0] = bufferRdsMsg[0] = rdsTime[0] = bufferRdsTime[0] = rdsMsg[0] = stationName[0] = '\0';
      showRDSMsg();
      showRDSStation();
      previousFrequency = currentFrequency;
    }
    checkRDS();
  }

  // Disable commands control
  if ( (millis() - elapsedCommand) > ELAPSED_COMMAND ) {
    if ( cmdBFO ) {
      bufferFreq[0] = '\0';
      bBFO.initButton(&tft, 195, 185, 60, 40, WHITE, CYAN, BLACK, (char *)"BFO", 1);
      bBFO.drawButton(true);
      cmdBFO = false;
      showFrequency();
    }
    disableCommands();
    elapsedCommand = millis();
  }

  delay(20);
}
