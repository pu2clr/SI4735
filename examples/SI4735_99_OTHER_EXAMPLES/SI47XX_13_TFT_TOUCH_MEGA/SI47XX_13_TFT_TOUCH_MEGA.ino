/* 
 * This sketch uses the mcufriend TFT touct Display on Mega 
 * 
 * By Ricardo Lima Caratti, Feb 2020
 */

#include <SI4735.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>
#include <TouchScreen.h>
#include "Rotary.h"


#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define RESET_PIN 22 // Mega2560 digital Pin used to RESET
// Enconder PINs (interrupt pins used on Mega2560)
#define ENCODER_PIN_A 18
#define ENCODER_PIN_B 19

#define AM_FUNCTION 1
#define FM_FUNCTION 0

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3


// Encoder control variables
volatile int encoderCount = 0;


typedef struct
{
  uint8_t bandType;     // Band type (FM, MW or SW)
  uint16_t minimumFreq; // Minimum frequency of the band
  uint16_t maximumFreq; // maximum frequency of the band
  uint16_t currentFreq; // Default frequency or current frequency
  uint16_t currentStep; // Defeult step (increment and decrement)
} Band;

/*
   Band tables
*/
Band band[] = {
    {FM_BAND_TYPE, 8400, 10800, 10390, 10},
    {LW_BAND_TYPE, 100, 510, 300, 1},
    {MW_BAND_TYPE, 520, 1720, 810, 10},
    {SW_BAND_TYPE, 1800, 3500, 1900, 1}, // 160 meters
    {SW_BAND_TYPE, 3500, 4500, 3700, 1}, // 80 meters
    {SW_BAND_TYPE, 4500, 5500, 4850, 5},
    {SW_BAND_TYPE, 5600, 6300, 6000, 5},
    {SW_BAND_TYPE, 6800, 7800, 7200, 5}, // 40 meters
    {SW_BAND_TYPE, 9200, 10000, 9600, 5},
    {SW_BAND_TYPE, 10000, 11000, 10100, 1}, // 30 meters
    {SW_BAND_TYPE, 11200, 12500, 11940, 5},
    {SW_BAND_TYPE, 13400, 13900, 13600, 5},
    {SW_BAND_TYPE, 14000, 14500, 14200, 1}, // 20 meters
    {SW_BAND_TYPE, 15000, 15900, 15300, 5},
    {SW_BAND_TYPE, 17200, 17900, 17600, 5},
    {SW_BAND_TYPE, 18000, 18300, 18100, 1},  // 17 meters
    {SW_BAND_TYPE, 21000, 21900, 21200, 1},  // 15 mters
    {SW_BAND_TYPE, 24890, 26200, 24940, 1},  // 12 meters
    {SW_BAND_TYPE, 26200, 27900, 27500, 1},  // CB band (11 meters)
    {SW_BAND_TYPE, 28000, 30000, 28400, 1}}; // 10 meters


const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 0;

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
uint8_t currentStep = 1;
uint8_t currentBFOStep = 25;

uint8_t bwIdxSSB = 2;
const char *bandwitdthSSB[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};

uint8_t bwIdxAM = 1;
const char *bandwitdthAM[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


char buffer[64];

Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
MCUFRIEND_kbv tft;
SI4735 si4735;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 6, XM = A2, YP = A1, YM = 7; //240x320 ID=0x9328
const int TS_LEFT = 294, TS_RT = 795, TS_TOP = 189, TS_BOT = 778;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button bSeekUp, bSeekDown, bSwitchBand;

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

  uint16_t ID = tft.readID();

  if (ID == 0xD3D3)
    ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0); //PORTRAIT
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSans9pt7b);
  bSeekUp.initButton(&tft, 60, 200, 115, 40, WHITE, CYAN, BLACK, (char *)"Seek Up", 1);
  bSeekDown.initButton(&tft, 180, 200, 115, 40, WHITE, CYAN, BLACK, (char *)"Seek Down", 1);
  bSwitchBand.initButton(&tft, 120, 250, 200, 40, WHITE, CYAN, BLACK, (char *)"AM/FM", 1);
  bSeekUp.drawButton(false);
  bSeekDown.drawButton(false);
  bSwitchBand.drawButton(false);
  tft.fillRect(40, 80, 160, 80, RED);

  showText(35, 15, 1, &FreeSans9pt7b, GREEN, "SI4735 Arduino Library");
  showText(35, 35, 1, &FreeSans9pt7b, YELLOW, "       By PU2CLR      ");

  // Atach Encoder pins interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, 1);

  // Set up the radio for the current band (see index table variable bandIdx )
  // useBand();
  currentFrequency = previousFrequency = si4735.getFrequency();

  tft.setFont(&FreeSans9pt7b); // default font
}


/* 
 * Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt 
 * 
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
 * Shows a text on a given position; with a given size and font, and with a given color
 * 
 * @param int x column
 * @param int y line
 * @param int sz font size
 * @param const GFXfont *f font type
 * @param uint16_t color
 * @param char * msg message 
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
 * 
 */
void showFrequency()
{
  float freq;
  int iFreq, dFreq;
  if (si4735.isCurrentTuneFM())
  {
    freq = currentFrequency / 100.0;
    iFreq = (int)freq;
    dFreq = (int)(freq - iFreq);
    sprintf(buffer, "%3d.%2d MHz", iFreq, dFreq);
  }
  else
  {
    freq = currentFrequency / 1000.0;
    iFreq = (int)freq;
    dFreq = (int)(freq - iFreq);
    sprintf(buffer, "%2d.%3d KHz", iFreq, dFreq);
  }
  showText(10, 40, 2, &FreeSans12pt7b, buffer, BLUE);

  tft.setFont(&FreeSans9pt7b); // default font
}

void showStatus()
{
  si4735.getStatus();
  si4735.getCurrentReceivedSignalQuality();
  // SRN
  tft.fillRect(20, 280, 160, 80, BLUE);
}

void showBFO()
{
}

void showVolume()
{
}

/* two buttons are quite simple
*/
void loop(void)
{
  bool down = Touch_getXY();
  bSeekUp.press(down && bSeekUp.contains(pixel_x, pixel_y));
  bSeekDown.press(down && bSeekDown.contains(pixel_x, pixel_y));
  bSwitchBand.press(down && bSwitchBand.contains(pixel_x, pixel_y));

  if (bSeekUp.justReleased())
    bSeekUp.drawButton(false);

  if (bSeekDown.justReleased())
    bSeekDown.drawButton(false);

  if (bSeekUp.justPressed())
  {
    bSeekUp.drawButton(false);
    // tft.fillRect(40, 80, 160, 80, GREEN);
  }

  if (bSeekDown.justPressed())
  {
    bSeekDown.drawButton(false);
    // tft.fillRect(40, 80, 160, 80, RED);
  }

  if (bSwitchBand.justReleased())
    bSwitchBand.drawButton(false);

  if (bSwitchBand.justPressed())
  {
    bSwitchBand.drawButton(false);
    // tft.fillRect(40, 80, 160, 80, YELLOW);
  }

}
