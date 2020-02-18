/*

  Under construction......

  It is a sketch for a radio All band and SSB based on Si4735.

  Under construction.....


  This sketch uses the TFT from MICROYUM 2.0"

  wire up on Arduino UNO, Pro mini

  TFT               Pin
  SCK/SCL           13
  SDA/SDI/MOSI      11
  CS/SS             10
  DC/A0/RS          9
  RET/RESET/RTS     8


  Last update: Jan 2020.
*/

#include <SI4735.h>

// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#include <SPI.h>
#include "TFT_22_ILI9225.h"


#include "Rotary.h"

// Test it with patch_init.h or patch_full.h. Do not try load both.
#include "patch_init.h" // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h


// TFT MICROYUM or ILI9225 based device pin setup
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 0   // 0 if wired to +3.3V directly
#define TFT_BRIGHTNESS 200



#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3


#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons controllers
#define MODE_BUTTON 4       // Switch MODE (Am/LSB/USB)
#define BAND_BUTTON 5  // Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz
#define VOL_BUTTON  6       // Volume Up
#define MENU_BUTTON 7       // Volume Down

/*
#define BAND_BUTTON_UP 0   // Next band
#define BAND_BUTTON_DOWN 0 // Previous band
#define AGC_SWITCH 0      // Switch AGC ON/OF
#define STEP_SWITCH 0     // Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
#define MENU_BUTTON 0      // Used to select the enconder control (BFO or VFO)
*/


#define CAPACITANCE 30  // You might need to adjust this value.
#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150

#define DEFAULT_VOLUME 50 // change it for your favorite sound volume

#define FM 0
#define LSB 1
#define USB 2
#define AM 3
#define LW 4

#define SSB 1


bool bfoOn = false;
bool disableAgc = true;
bool ssbLoaded = false;
bool fmStereo = true;
bool touch = false;

int currentBFO = 0;
int previousBFO = 0;

long elapsedRSSI = millis();
long elapsedButton = millis();
long elapsedFrequency = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentStep = 1;
uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char * const bandwitdthSSB[] PROGMEM = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char * const bandwitdthAM[] PROGMEM = {"6", "4", "3", "2", "1", "1.8", "2.5"};

const char * const bandModeDesc[] PROGMEM = {"FM ", "LSB", "USB", "AM "};
uint8_t currentMode = FM;

char buffer[64]; // Useful to handle string


/*
   Band data structure
*/
typedef struct
{
  const char *bandName; // Band description
  uint8_t bandType;     // Band type (FM, MW or SW)
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint16_t currentStep; // Defeult step (increment and decrement)
} Band;

/*
   Band table
*/
Band band[]  = {
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
  {"10m ", SW_BAND_TYPE, 28000, 30000, 28400, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

uint8_t rssi = 0;
uint8_t stereo = 1;
uint8_t volume = DEFAULT_VOLUME;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);


// For 1.44" and 1.8" TFT with ST7735 use:
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);


SI4735 si4735;

void setup()
{
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);


    pinMode(MODE_BUTTON, INPUT_PULLUP);
    pinMode(BAND_BUTTON, INPUT_PULLUP);
    pinMode(VOL_BUTTON, INPUT_PULLUP);
    pinMode(MENU_BUTTON, INPUT_PULLUP);


  // Use this initializer if using a 1.8" TFT screen:
  tft.begin();
  tft.setOrientation(1);
  tft.clear();
  tft.setFont(Terminal6x8);
  tft.drawText(36, 20, "SI4735 Arduino Library", COLOR_RED); // Print string
  tft.drawText(80, 60, "By PU2CLR", COLOR_YELLOW);
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    tft.drawText(20, 120, "Si47XX was not detected!", COLOR_YELLOW);
    // while (1);
  } else {
    sprintf(buffer, "The Si473X I2C address is 0x%x ", si4735Addr);
    tft.drawText(30, 120, buffer, COLOR_RED);
  }


  delay(5000);
  tft.clear();
  tft.setFont(Trebuchet_MS16x21);

  showTemplate();
  // while (1);
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);


  // The line below may be necessary to setup I2C pins on ESP32
  // Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);

  si4735.setup(RESET_PIN, 1);

  // Set up the radio for the current band (see index table variable bandIdx )
  useBand();
  currentFrequency = previousFrequency = si4735.getFrequency();

  si4735.setVolume(volume);
  showStatus();
}


/*
   Shows the static content on  display
*/
void showTemplate() {


  // See https://github.com/Nkawu/TFT_22_ILI9225/wiki

  tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
  tft.drawRectangle(2, 2, tft.maxX() - 3, 40, COLOR_YELLOW);
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
  /*
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(msg);
  */
}


/*
    Reads encoder via interrupt
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
   Shows frequency information on Display
*/

char bufferFreq[10];

void showFrequency()
{
  float freq;
  int iFreq, dFreq;
  uint16_t color;

  // Clear the frequency content field
  tft.drawText(5, 10, bufferFreq,  COLOR_BLACK);

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
  color = (bfoOn) ? COLOR_CYAN : COLOR_YELLOW;

  tft.drawText(5, 10, buffer, color);
  strcpy(bufferFreq, buffer); 
}

/*
    Show some basic information on display
*/
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

    /*
    // tft.fillRect(150, 2, 85, 36, ST77XX_BLACK);
    if (si4735.isCurrentTuneFM()) {
    showText(170, 30, 2, NULL, ST77XX_WHITE, "MHz");
    } else {
    sprintf(buffer, "Step:%2d", currentStep);
    showText(170, 10, 1, NULL, ST77XX_WHITE, buffer);
    showText(170, 30, 2, NULL, ST77XX_WHITE, "KHz");
    }

    // tft.fillRect(0, 60, 250, 36, ST77XX_BLACK);

    if ( band[bandIdx].bandType == SW_BAND_TYPE) {
    sprintf(buffer, "%s %s", band[bandIdx].bandName, bandModeDesc[currentMode]);
    showText(5, 60, 2, NULL, ST77XX_RED, buffer );
    }
    else {
    sprintf(buffer, "%s", band[bandIdx].bandName);
    showText(5, 60, 2, NULL, ST77XX_RED, buffer );
    }

    showText(70, 85, 1, NULL, ST77XX_BLACK, bufferAGC);
    si4735.getAutomaticGainControl();
    sprintf(buffer, "AGC %s", (si4735.isAgcEnabled()) ? "ON" : "OFF");
    strcpy(bufferAGC, buffer);

    if (currentMode == LSB || currentMode == USB)
    {
    showText(5, 85, 1, NULL, ST77XX_BLACK, bufferBW );
    sprintf(buffer, "BW:%s KHz", bandwitdthSSB[bwIdxSSB]);
    showText(5, 85, 1, NULL, ST77XX_GREEN, buffer );
    strcpy( bufferBW, buffer);
    showBFO();
    showText(70, 85, 1, NULL, ST77XX_GREEN, bufferAGC);
    }
    else if (currentMode == AM)
    {
    showText(5, 85, 1, NULL, ST77XX_BLACK, bufferBW );
    sprintf(buffer, "BW:%s KHz", bandwitdthAM[bwIdxAM]);
    showText(5, 85, 1, NULL, ST77XX_GREEN, buffer );
    strcpy( bufferBW, buffer);
    showText(70, 85, 1, NULL, ST77XX_GREEN, bufferAGC);
    }
    tft.setFont(NULL); */
}

/* *******************************
   Shows RSSI status
*/
char bufferStereo[10];

void showRSSI() {
  /*
    if (  currentMode == FM ) {
    showText(5, 85, 1, NULL, ST77XX_BLACK, bufferStereo );
    sprintf(buffer, "%s", (si4735.getCurrentPilot()) ? "STEREO" : "MONO");
    showText(5, 85, 1, NULL, ST77XX_GREEN, buffer );
    strcpy(bufferStereo, buffer);
    }
  */
}


/*
   Shows the volume level on LCD
*/
void showVolume()
{

}


char bufferBFO[15];
char bufferStep[15];

void showBFO()
{
  /*
    showText(150, 60, 1, NULL, ST77XX_BLACK, bufferBFO );
    showText(150, 77, 1, NULL, ST77XX_BLACK, bufferStep);
    sprintf(buffer, "BFO.:%+d", currentBFO);
    showText(150, 60, 1, NULL, ST77XX_GREEN, buffer );
    strcpy(bufferBFO, buffer);
    // tft.fillRect(128, 78, 110, 18, ST77XX_BLACK);
    sprintf(buffer, "Step:%2d", currentBFOStep);
    showText(150, 77, 1, NULL, ST77XX_GREEN, buffer);
    strcpy(bufferStep, buffer);
  */
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
  }
  else
  {
    if (band[bandIdx].bandType == MW_BAND_TYPE || band[bandIdx].bandType == LW_BAND_TYPE)
      si4735.setTuneFrequencyAntennaCapacitor(0);
    else
      si4735.setTuneFrequencyAntennaCapacitor(1);

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





void loop()
{
  // Check if the encoder has moved.
  if (encoderCount != 0)
  {
    if (bfoOn)
    {
      currentBFO = (encoderCount == 1) ? (currentBFO + currentBFOStep) : (currentBFO - currentBFOStep);
    }
    else
    {
      if (encoderCount == 1)
        si4735.frequencyUp();
      else
        si4735.frequencyDown();

      // Show the current frequency only if it has changed
      currentFrequency = si4735.getFrequency();
    }
    encoderCount = 0;
  }

  // Check button commands
  if ((millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    // check if some button is pressed
    if (digitalRead(BAND_BUTTON) == LOW)
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
    else if ( false /*digitalRead(BAND_BUTTON_UP) == LOW */)
      bandUp();
    else if (false /*digitalRead(BAND_BUTTON_DOWN) == LOW */)
      bandDown();
    else if (digitalRead(VOL_BUTTON) == LOW)
    {
      si4735.volumeUp();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(VOL_DOWN) == LOW)
    {
      si4735.volumeDown();
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (digitalRead(MENU_BUTTON) == LOW)
    {
      if (currentMode == LSB || currentMode == USB) {
        bfoOn = !bfoOn;
        if (bfoOn)
          showBFO();
        showStatus();
      } else if (currentMode == FM) {
        si4735.seekStationUp();
        delay(30);
        currentFrequency = si4735.getFrequency();
      }
      delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
    }
    else if (false /*digitalRead(AGC_SWITCH) == LOW */)
    {
      disableAgc = !disableAgc;
      // siwtch on/off ACG; AGC Index = 0. It means Minimum attenuation (max gain)
      si4735.setAutomaticGainControl(disableAgc, 1);
      showStatus();
    }
    else if (false /*digitalRead(STEP_SWITCH) == LOW */)
    { /*
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
          else
            currentStep = 1;
          si4735.setFrequencyStep(currentStep);
          band[bandIdx].currentStep = currentStep;
          showStatus();
        }
        delay(MIN_ELAPSED_TIME); // waits a little more for releasing the button.
        } */
    }
    else if (digitalRead(MODE_BUTTON) == LOW)
    { /*
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
        } */
    }
    elapsedButton = millis();
  }

  // Show the current frequency only if it has changed
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showFrequency();
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

  // Show volume level only if this condition has changed
  if (si4735.getCurrentVolume() != volume)
  {
    volume = si4735.getCurrentVolume();
    showVolume();
  }

  if (currentMode == LSB || currentMode == USB)
  {
    if (currentBFO != previousBFO)
    {
      previousBFO = currentBFO;
      si4735.setSSBBfo(currentBFO);
      showBFO();
    }
  }

  delay(10);
}
