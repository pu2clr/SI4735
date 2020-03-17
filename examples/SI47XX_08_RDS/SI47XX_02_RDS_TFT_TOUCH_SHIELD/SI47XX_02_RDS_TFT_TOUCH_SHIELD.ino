/*

  Under construction......

  This sketch uses the MICROYUM 3.5" TFT touct Display Shield (www.microyum.cc).
  It works only on Arduino Mega 2560.

  Features:
  1) This sketch has been successfully tested on Arduino Mega2560 and DUE;
  2) It uses the touch screen interface provided by mcufriend TFT;
  3) Encoder;
  4) FM, AM (MW and SW) and SSB (LSB and USB);
  5) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
  6) BFO Control; and
  7) Frequency step switch (1, 5 and 10KHz).


  Wire up

  Function                MEGA Pin
  ----------------------  -------------
  SDA                     20
  SCL                     21
  ENCODER_A               18        - On DUE all Digital pin can be used with Interrupt
  ENCODER_B               19        - On DUE all Digital pin can be used with Interrupt
  RESET                   22
  ENCODER PUSH BUTTON     23

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

  By Ricardo Lima Caratti, Feb 2020
*/

#include <SI4735.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#include "Rotary.h"

#include "patch_init.h" // SSB patch for whole SSBRX initialization string

#define MINPRESSURE 200
#define MAXPRESSURE 1000


#define RESET_PIN 22            // Mega2560 digital Pin used to RESET
#define ENCODER_PUSH_BUTTON 23  // Used to switch BFO and VFO or other function  

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
#define DEFAULT_VOLUME 45 // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4
#define SSB 1

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

bool bfoOn = false;
bool audioMute = false;
bool disableAgc = true;
bool ssbLoaded = false;
bool fmStereo = true;
bool touch = false;

int currentBFO = 0;
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
  {"41m ", SW_BAND_TYPE, 6800, 7800, 7100, 5}, // 40 meters
  {"31m ", SW_BAND_TYPE, 9200, 10000, 9600, 5},
  {"30m ", SW_BAND_TYPE, 10000, 11000, 10100, 1}, // 30 meters
  {"25m ", SW_BAND_TYPE, 11200, 12500, 11940, 5},
  {"22m ", SW_BAND_TYPE, 13400, 13900, 13600, 5},
  {"20m ", SW_BAND_TYPE, 14000, 14500, 14200, 1}, // 20 meters
  {"19m ", SW_BAND_TYPE, 15000, 15900, 15300, 5},
  {"18m ", SW_BAND_TYPE, 17200, 17900, 17600, 5},
  {"17m ", SW_BAND_TYPE, 18000, 18300, 18100, 1},  // 17 meters
  {"15m ", SW_BAND_TYPE, 21000, 21900, 21200, 1},  // 15 mters
  {"12m ", SW_BAND_TYPE, 24890, 26200, 24940, 1},  // 12 meters
  {"CB  ", SW_BAND_TYPE, 26200, 27900, 27500, 1},  // CB band (11 meters)
  {"10m ", SW_BAND_TYPE, 28000, 30000, 28400, 1},
  {"All HF", SW_BAND_TYPE, 100, 30000, 15000, 1} // All HF in one band
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;
int lastSwBand = 7;  // Saves the last SW band used

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
uint16_t currentStep = 1;
uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char *bandwitdthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char *bandwitdthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char *bandModeDesc[] = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

char buffer[255];
char bufferFreq[10];
char bufferStereo[10];

Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
MCUFRIEND_kbv tft;
SI4735 si4735;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 6, XM = A2, YP = A1, YM = 7; //240x320 ID=0x9328
const int TS_LEFT = 170, TS_RT = 827, TS_TOP = 130, TS_BOT = 868;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 320);
Adafruit_GFX_Button bNextBand, bPreviousBand, bVolumeUp, bVolumeDown, bSeekUp, bSeekDown, bStep, bAudioMute, bAM, bLSB, bUSB, bFM, bMW, bSW, bFilter, bAGC;

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

  uint16_t ID = tft.readID();

  if (ID == 0xD3D3)
    ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0); //PORTRAIT
  tft.fillScreen(BLACK);


  // tft.setFont(&FreeSans12pt7b);
  showText(55, 30, 2, NULL, GREEN, "SI4735");
  showText(55, 90, 2, NULL, YELLOW, "Arduino");
  showText(55, 160, 2, NULL, YELLOW, "Library");
  showText(55, 240, 2, NULL, WHITE, "By PU2CLR");
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    tft.fillScreen(BLACK);
    showText(0, 160, 2, NULL, RED, "Si473X not");
    showText(0, 240, 2, NULL, RED, "detected!!");
    while (1);
  } else {
    sprintf(buffer, "The Si473X I2C address is 0x%x ", si4735Addr);
    showText(25, 290, 1, NULL, RED, buffer);
  }
  delay(3000);

  tft.fillScreen(BLACK);

  showTemplate();

  // Atach Encoder pins interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, 1);
  
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



void showTemplate() {

  // Área reservada à frequência
  tft.drawRect(0, 0, tft.width(), 50, WHITE);


  tft.drawRect(0, 100, tft.width(), 160, CYAN);
  tft.setFont(NULL);
  bPreviousBand.initButton(&tft, 30, 120, 40, 30, WHITE, CYAN, BLACK, (char *)"Band-", 1);
  bNextBand.initButton(&tft, 90, 120, 40, 30, WHITE, CYAN, BLACK, (char *)"Band+", 1);
  bVolumeDown.initButton(&tft, 150, 120, 40, 30, WHITE, CYAN, BLACK, (char *)"Vol-", 1);
  bVolumeUp.initButton(&tft, 210, 120, 40, 30, WHITE, CYAN, BLACK, (char *)"Vol+", 1);
  bSeekDown.initButton(&tft, 30, 160, 40, 30, WHITE, CYAN, BLACK, (char *)"Seek-", 1);
  bSeekUp.initButton(&tft, 90, 160, 40, 30, WHITE, CYAN, BLACK, (char *)"Seek+", 1);
  bAudioMute.initButton(&tft, 150, 160, 40, 30, WHITE, CYAN, BLACK, (char *)"Mute", 1);
  bStep.initButton(&tft, 210, 160, 40, 30, WHITE, CYAN, BLACK, (char *)"Step", 1);
  bFM.initButton(&tft, 30, 200, 40, 30, WHITE, CYAN, BLACK, (char *)"FM", 1);
  bMW.initButton(&tft, 90, 200, 40, 30, WHITE, CYAN, BLACK, (char *)"MW", 1);
  bSW.initButton(&tft, 150, 200, 40, 30, WHITE, CYAN, BLACK, (char *)"SW", 1);
  bAGC.initButton(&tft, 210, 200, 40, 30, WHITE, CYAN, BLACK, (char *)"AGC", 1);
  bAM.initButton(&tft, 30, 240, 40, 30, WHITE, CYAN, BLACK, (char *)"AM", 1);
  bLSB.initButton(&tft, 90, 240, 40, 30, WHITE, CYAN, BLACK, (char *)"LSB", 1);
  bUSB.initButton(&tft, 150, 240, 40, 30, WHITE, CYAN, BLACK, (char *)"USB", 1);
  bFilter.initButton(&tft, 210, 240, 40, 30, WHITE, CYAN, BLACK, (char *)"|Y|", 1);

  // Exibe os botões (teclado touch)
  bNextBand.drawButton(true);
  bPreviousBand.drawButton(true);
  bVolumeUp.drawButton(true);
  bVolumeDown.drawButton(true);
  bSeekUp.drawButton(true);
  bSeekDown.drawButton(true);
  bStep.drawButton(true);
  bAudioMute.drawButton(true);
  bFM.drawButton(true);
  bMW.drawButton(true);
  bSW.drawButton(true);
  bAM.drawButton(true);
  bLSB.drawButton(true);
  bUSB.drawButton(true);
  bFilter.drawButton(true);
  bAGC.drawButton(true);

  showText(0, 270, 1, NULL, YELLOW, "PU2CLR-Si4535 Arduino Library-Example");
  showText(0, 285, 1, NULL, YELLOW, "DIY - You can make it better.");

  showText(0, 302, 1, NULL, GREEN, "RSSI" );
  tft.drawRect(30, 298, 210, 12, CYAN);

  tft.setFont(NULL);

}


/*
    Prevents blinking during the frequency display.
    Erases the old digits if it has changed and print the new digit values.
*/
void showFrequencyValue(int col, int line, char *oldValue, char *newValue, uint16_t color) {

  int c = col;

  // prints just changed digits
  while (*oldValue && *newValue) {
    if ( *oldValue != *newValue ) {
      tft.drawChar(c, line, *oldValue, BLACK, BLACK, 4);
      tft.drawChar(c, line, *newValue, color, BLACK, 4);
    }
    oldValue++;
    newValue++;
    c += 25;
  }

  // Is there anything else to erase?
  while (*oldValue) {
    tft.drawChar(c, line, *oldValue, BLACK, BLACK, 4);
    oldValue++;
    c += 25;
  }

  // Is there anything else to print?
  while (*newValue) {
    tft.drawChar(c, line, *newValue, color, BLACK, 4);
    newValue++;
    c += 25;
  }

}


void showFrequency()
{
  float freq;
  int iFreq, dFreq;
  uint16_t color;

  if (si4735.isCurrentTuneFM())
  {
    freq = currentFrequency / 100.0;
    dtostrf(freq, 3, 1, buffer);
  }
  else
  {
    freq = currentFrequency / 1000.0;
    if ( currentFrequency < 1000 )
      sprintf(buffer, "%5d", currentFrequency);
    else
      dtostrf(freq, 2, 3, buffer);
  }
  color = (bfoOn) ? CYAN : YELLOW;
  // showText(10, 10, 4, NULL, color, buffer);
  showFrequencyValue(10, 10, bufferFreq, buffer, color);
  tft.setFont(NULL); // default font
  strcpy(bufferFreq, buffer);
}


char bufferBW[15];
char bufferAGC[10];

void showStatus()
{
  char unit[5];
  si4735.getStatus();
  si4735.getCurrentReceivedSignalQuality();
  // SRN
  si4735.getFrequency();
  showFrequency();

  tft.fillRect(155, 2, 83, 36, BLACK);
  if (si4735.isCurrentTuneFM()) {
    showText(170, 30, 2, NULL, WHITE, "MHz");
  } else {
    sprintf(buffer, "Step:%2d", currentStep);
    showText(170, 10, 1, NULL, WHITE, buffer);
    showText(170, 30, 2, NULL, WHITE, "KHz");
  }

  tft.fillRect(0, 60, 250, 36, BLACK);

  if ( band[bandIdx].bandType == SW_BAND_TYPE) {
    sprintf(buffer, "%s %s", band[bandIdx].bandName, bandModeDesc[currentMode]);
    showText(5, 60, 2, NULL, RED, buffer );
  }
  else {
    sprintf(buffer, "%s", band[bandIdx].bandName);
    showText(5, 60, 2, NULL, RED, buffer );
  }

  showText(70, 85, 1, NULL, BLACK, bufferAGC);
  si4735.getAutomaticGainControl();
  sprintf(buffer, "AGC %s", (si4735.isAgcEnabled()) ? "ON" : "OFF");
  strcpy(bufferAGC, buffer);

  if (currentMode == LSB || currentMode == USB)
  {
    showText(5, 85, 1, NULL, BLACK, bufferBW );
    sprintf(buffer, "BW:%s KHz", bandwitdthSSB[bwIdxSSB]);
    showText(5, 85, 1, NULL, GREEN, buffer );
    strcpy( bufferBW, buffer);
    showBFO();
    showText(70, 85, 1, NULL, GREEN, bufferAGC);
  }
  else if (currentMode == AM)
  {
    showText(5, 85, 1, NULL, BLACK, bufferBW );
    sprintf(buffer, "BW:%s KHz", bandwitdthAM[bwIdxAM]);
    showText(5, 85, 1, NULL, GREEN, buffer );
    strcpy( bufferBW, buffer);
    showText(70, 85, 1, NULL, GREEN, bufferAGC);
  }
  tft.setFont(NULL);
}

void showRSSI() {

  int signalLevel;

  if (  currentMode == FM ) {
    showText(5, 85, 1, NULL, BLACK, bufferStereo );
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    showText(5, 85, 1, NULL, GREEN, buffer );
    strcpy(bufferStereo, buffer);
  }

  signalLevel = map(rssi, 0, 63, 0, 208);
  tft.fillRect(30, 300, 209, 8, BLACK);
  tft.fillRect(30, 300, signalLevel, 8, (signalLevel > 25) ? CYAN : RED);


}


char bufferBFO[15];
char bufferStep[15];

void showBFO()
{

  showText(150, 60, 1, NULL, BLACK, bufferBFO );
  showText(150, 77, 1, NULL, BLACK, bufferStep);

  sprintf(buffer, "BFO.:%+d", currentBFO);
  showText(150, 60, 1, NULL, GREEN, buffer );
  strcpy(bufferBFO, buffer);
  tft.fillRect(128, 78, 110, 18, BLACK);
  sprintf(buffer, "Step:%2d", currentBFOStep);
  showText(150, 77, 1, NULL, GREEN, buffer);
  strcpy(bufferStep, buffer);
}



char *rdsMsg;
char *stationName;
char bufferStatioName[255];
char bufferRdsMsg[255];


void showRDSMsg() {
  

  showText(60, 85, 1, NULL, BLACK, bufferRdsMsg );
  sprintf(buffer, "%s", rdsMsg);
  showText(60, 85, 1, NULL, GREEN, buffer );
  strcpy(bufferRdsMsg, buffer);
  
  delay(50); 
}


void showRDSStation() {
  
  showText(60, 60, 1, NULL, BLACK, bufferStatioName );
  sprintf(buffer, "%s", stationName);
  showText(60, 60, 1, NULL, GREEN, buffer );
  strcpy(bufferStatioName, buffer);

  delay(50); 
}

void checkRDS() {

  si4735.getRdsStatus();
  if (si4735.getRdsReceived()) {
    if (si4735.getRdsSync() && si4735.getRdsSyncFound() ) {
      rdsMsg = si4735.getRdsText2A();
      stationName = si4735.getRdsText0A();
      if ( rdsMsg != NULL )   showRDSMsg();
      if ( stationName != NULL )   showRDSStation();
    }
  }
}

void showVolume()
{
}


/*
   Goes to the next band (see Band table)
*/
void bandUp()
{
  uint8_t v;
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
  // si4735.setI2CFastMode(); // Recommended
  si4735.setI2CFastModeCustom(500000); // It is a test and may crash.
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
  if (band[bandIdx].bandType == FM_BAND_TYPE)
  {
    currentMode = FM;
    si4735.setTuneFrequencyAntennaCapacitor(0);
    si4735.setFM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
    bfoOn = ssbLoaded = false;
    si4735.RdsInit();
    si4735.setRdsConfig(1, 2, 2, 2, 2);    
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      si4735.setTuneFrequencyAntennaCapacitor(0);
    else {
      lastSwBand =  bandIdx;
      si4735.setTuneFrequencyAntennaCapacitor(1);
    }

    if (ssbLoaded)
    {
      si4735.setSSB(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep, currentMode);
      si4735.setSSBAutomaticVolumeControl(1);
      si4735.setSsbSoftMuteMaxAttenuation(0); // Disable Soft Mute for SSB
    }
    else
    {
      currentMode = AM;
      si4735.setAM(band[bandIdx].minimumFreq, band[bandIdx].maximumFreq, band[bandIdx].currentFreq, band[bandIdx].currentStep);
      si4735.setAutomaticGainControl(1, 0);
      si4735.setAmSoftMuteMaxAttenuation(0); // // Disable Soft Mute for AM
      bfoOn = false;
    }
  }
  delay(100);
  currentFrequency = band[bandIdx].currentFreq;
  currentStep = band[bandIdx].currentStep;
  showStatus();
}



/* two buttons are quite simple
*/
void loop(void)
{
  bool down = Touch_getXY();
  bNextBand.press(down && bNextBand.contains(pixel_x, pixel_y));
  bPreviousBand.press(down && bPreviousBand.contains(pixel_x, pixel_y));
  bVolumeUp.press(down && bVolumeUp.contains(pixel_x, pixel_y));
  bVolumeDown.press(down && bVolumeDown.contains(pixel_x, pixel_y));
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


  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
      si4735.setSSBBfo(currentBFO);
      showBFO();
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

  // if (bNextBand.justReleased())
  //   bNextBand.drawButton(true);

  // if (bPreviousBand.justReleased())
  //   bPreviousBand.drawButton(true);

  if (bNextBand.justPressed())
  {
    // bNextBand.drawButton(true);
    bandUp();
  }

  if (bPreviousBand.justPressed())
  {
    // bPreviousBand.drawButton(true);
    bandDown();
  }

  // if (bVolumeUp.justReleased())
  //   bVolumeUp.drawButton(true);

  if (bVolumeUp.justPressed())
  {
    // bVolumeUp.drawButton(true);
    si4735.volumeUp();
    delay(MIN_ELAPSED_TIME);
  }

  // if (bVolumeDown.justReleased())
  //   bVolumeDown.drawButton(true);

  if (bVolumeDown.justPressed())
  {
    // bVolumeDown.drawButton(true);
    si4735.volumeDown();
    delay(MIN_ELAPSED_TIME);
  }


  // if (bSeekUp.justReleased())
  //   bSeekUp.drawButton(true);

  if (bSeekUp.justPressed())
  {
    // bSeekUp.drawButton(true);
    if (currentMode == FM) {
      si4735.seekStationUp();
      delay(15);
      currentFrequency = si4735.getFrequency();
    }
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    showStatus();
  }


  if (bSeekDown.justPressed())
  {
    // bSeekUp.drawButton(true);
    if (currentMode == FM) {
      si4735.seekStationDown();
      delay(15);
      currentFrequency = si4735.getFrequency();
    }
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    showStatus();
  }


  if (bAudioMute.justPressed())
  {
    audioMute = !audioMute;
    si4735.setAudioMute(audioMute);
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }

  // if (bMode.justReleased())
  //   bMode.drawButton(true);
  /*
    if (bMode.justPressed())
    {
    if (currentMode != FM ) {
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
        ssbLoaded = false;
        bfoOn = false;
      }
      // Nothing to do if you are in FM mode
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
    }
    } */

  if (bAM.justPressed())
  {
    if (currentMode != FM ) {
      currentMode = AM;
      ssbLoaded = false;
      bfoOn = false;
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
    }
  }



  if (bFM.justPressed())
  {
    if (currentMode != FM ) {
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      ssbLoaded = false;
      bfoOn = false;
      currentMode = FM;
      bandIdx = 0;
      useBand();
    }
  }


  if (bMW.justPressed())
  {
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    ssbLoaded = false;
    bfoOn = false;
    currentMode = AM;
    bandIdx = 2;   // See Band table
    useBand();
  }

  if (bSW.justPressed())
  {
    band[bandIdx].currentFreq = currentFrequency;
    band[bandIdx].currentStep = currentStep;
    ssbLoaded = false;
    bfoOn = false;
    currentMode = AM;
    bandIdx = lastSwBand;   // See Band table
    useBand();
  }


  if (bLSB.justPressed())
  {
    if (currentMode != FM ) {
      if (!ssbLoaded) {
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
    if (currentMode != FM ) {
      if (!ssbLoaded) {
        loadSSB();
      }
      currentMode = USB;
      band[bandIdx].currentFreq = currentFrequency;
      band[bandIdx].currentStep = currentStep;
      useBand();
    }
  }


  if (bAGC.justPressed())
  {
    disableAgc = !disableAgc;
    // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
    si4735.setAutomaticGainControl(disableAgc, 1);
    showStatus();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }


  if (bFilter.justPressed())
  {
    if (currentMode == LSB || currentMode == USB)
    {
      bwIdxSSB++;
      if (bwIdxSSB > 5)
        bwIdxSSB = 0;
      si4735.setSSBAudioBandwidth(bwIdxSSB);
      // If audio bandwidth selected is about 2 kHz or below, it is recommended to set Sideband Cutoff Filter to 0.
      if (bwIdxSSB == 0 || bwIdxSSB == 4 || bwIdxSSB == 5)
        si4735.setSBBSidebandCutoffFilter(0);
      else
        si4735.setSBBSidebandCutoffFilter(1);
    }
    else if (currentMode == AM)
    {
      bwIdxAM++;
      if (bwIdxAM > 6)
        bwIdxAM = 0;
      si4735.setBandwidth(bwIdxAM, 0);
    }
    showStatus();
    delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
  }


  if (bStep.justPressed())
  {
    if ( currentMode == FM) {
      fmStereo = !fmStereo;
      if ( fmStereo )
        si4735.setFmStereoOn();
      else
        si4735.setFmStereoOff(); // It is not working so far.
    } else {

      // This command should work only for SSB mode
      if (bfoOn && (currentMode == LSB || currentMode == USB))
      {
        currentBFOStep = (currentBFOStep == 25) ? 10 : 25;
        showBFO();
      }
      else
      {
        if (currentStep == 1)
          currentStep = 5;
        else if (currentStep == 5)
          currentStep = 10;
        else if (currentStep == 10)
          currentStep = 100;
        else if (currentStep == 100  && bandIdx == lastBand )
          currentStep = 500;
        else
          currentStep = 1;
        si4735.setFrequencyStep(currentStep);
        band[bandIdx].currentStep = currentStep;
        showStatus();
      }
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
  }

  if (digitalRead(ENCODER_PUSH_BUTTON) == LOW)  {
    bufferFreq[0] = '\0';
    if (currentMode == LSB || currentMode == USB) {
      bfoOn = !bfoOn;
      if (bfoOn) {
        showBFO();
      }
      showStatus();
    }
    delay(100);
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

  if ( currentMode == FM) {
    if ( currentFrequency != previousFrequency ) {
      rdsMsg = stationName = "\0";
      showRDSMsg();
      showRDSStation();
      previousFrequency = currentFrequency;
    }
    checkRDS();
  }

  delay(20);
}
