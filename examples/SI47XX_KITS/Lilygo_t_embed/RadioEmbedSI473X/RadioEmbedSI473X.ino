/**
 * UNDER CONSTRUCTION...
 * 
 * This example is based on sketch writen by Volos Projects.
 * 
 */

#include <Wire.h>
#include <SI4735.h>
#include <FastLED.h> // 
#include "TFT_eSPI.h"
#include <RotaryEncoder.h>


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3
#define RESET_PIN 16  // To be checked ... not sure


#define PIN_IN1 2
#define PIN_IN2 1

#define NUM_LEDS 7
#define DATA_PIN 42
#define CLOCK_PIN 45
CRGB leds[NUM_LEDS];

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);


#define color1 0xC638
#define color2  0xC638

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint16_t previousFrequency;
// uint8_t currentStep = 1;
uint8_t currentBFOStep = 25;

// Datatype to deal with bandwidth on AM, SSB and FM in numerical order.
// Ordering by bandwidth values.
typedef struct
{
  uint8_t idx;      // SI473X device bandwidth index value
  const char *desc; // bandwidth description
} Bandwidth;

int8_t bwIdxSSB = 4;
Bandwidth bandwidthSSB[] = {
  {4, "0.5"}, // 0
  {5, "1.0"}, // 1
  {0, "1.2"}, // 2
  {1, "2.2"}, // 3
  {2, "3.0"}, // 4  - default
  {3, "4.0"}  // 5
};              // 3 = 4kHz

int8_t bwIdxAM = 4;
const int maxFilterAM = 6;
Bandwidth bandwidthAM[] = {
  {4, "1.0"}, // 0
  {5, "1.8"}, // 1
  {3, "2.0"}, // 2
  {6, "2.5"}, // 3
  {2, "3.0"}, // 4 - default
  {1, "4.0"}, // 5
  {0, "6.0"}  // 6
};

int8_t bwIdxFM = 0;
Bandwidth bandwidthFM[] = {
  {0, "AUT"}, // Automatic - default
  {1, "110"}, // Force wide (110 kHz) channel filter.
  {2, " 84"},
  {3, " 60"},
  {4, " 40"}
};

// Atenuação and AGC
int8_t agcIdx = 0;
uint8_t disableAgc = 0;
uint8_t agcNdx = 0;
int8_t smIdx = 8;
int8_t avcIdx = 38;

int tabStep[] = {1,    // 0
                 5,    // 1
                 9,    // 2
                 10,   // 3
                 50,   // 4
                 100
                }; // 5

const int lastStep = (sizeof tabStep / sizeof(int)) - 1;
int idxStep = 3;

/*
   Band data structure
*/
typedef struct
{
  uint8_t bandType;        // Band type (FM, MW or SW)
  uint16_t minimumFreq;    // Minimum frequency of the band
  uint16_t maximumFreq;    // Maximum frequency of the band
  uint16_t currentFreq;    // Default frequency or current frequency
  uint16_t currentStepIdx; // Idex of tabStep:  Defeult frequency step (See tabStep)
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
  {LW_BAND_TYPE, 100, 510, 300, 0, 4},
  {MW_BAND_TYPE, 520, 1720, 810, 3, 4},       // AM/MW from 520 to 1720kHz; default 810kHz; default step frequency index is 3 (10kHz); default bandwidth index is 4 (3kHz)
  {MW_BAND_TYPE, 531, 1701, 783, 2, 4},       // MW for Europe, Africa and Asia
  {SW_BAND_TYPE, 1700, 3500, 1900, 0, 4},     // 160 meters
  {SW_BAND_TYPE, 3500, 4500, 3700, 0, 5},     // 80 meters
  {SW_BAND_TYPE, 4500, 5600, 4850, 1, 4},
  {SW_BAND_TYPE, 5600, 6800, 6000, 1, 4},
  {SW_BAND_TYPE, 6800, 7300, 7100, 0, 4},     // 40 meters
  {SW_BAND_TYPE, 7200, 8500, 7200, 1, 4},     // 41 meters
  {SW_BAND_TYPE, 8500, 10000, 9600, 1, 4},
  {SW_BAND_TYPE, 10000, 11200, 10100, 0, 4},  // 30 meters
  {SW_BAND_TYPE, 11200, 12500, 11940, 1, 4},
  {SW_BAND_TYPE, 13400, 13900, 13600, 1, 4},
  {SW_BAND_TYPE, 14000, 14500, 14200, 0, 4},  // 20 meters
  {SW_BAND_TYPE, 15000, 15900, 15300, 1, 4},
  {SW_BAND_TYPE, 17200, 17900, 17600, 1, 4},
  {SW_BAND_TYPE, 18000, 18300, 18100, 0, 4},  // 17 meters
  {SW_BAND_TYPE, 21000, 21400, 21200, 0, 4},  // 15 mters
  {SW_BAND_TYPE, 21400, 21900, 21500, 1, 4},  // 13 mters
  {SW_BAND_TYPE, 24890, 26200, 24940, 0, 4},  // 12 meters
  {SW_BAND_TYPE, 26200, 28000, 27500, 0, 4},  // CB band (11 meters)
  {SW_BAND_TYPE, 28000, 30000, 28400, 0, 4},  // 10 meters
  {FM_BAND_TYPE, 6400, 8400, 7000, 3, 0},     // FM from 64 to 84MHz; default 70MHz; default step frequency index is 3; default bandwidth index AUTO
  {FM_BAND_TYPE, 8400, 10800, 10570, 3, 0}
};


const int lastBand = (sizeof band / sizeof(Band)) - 1;
int bandIdx = 1;



int value=980;
int minimal=880;
int maximal=1080;
int strength=0;
String sta[6]={"96.6","101.0","89.4","106,5","98.2","92.4"};

float freq=0.00;

SI4735 radio;

bool muted=0;
int deb=0;

void setup() {
  
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);

  tft.begin();
  tft.writecommand(0x11);
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  pinMode(0, INPUT_PULLUP);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  Wire.begin(43,44);
    

  radio.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error

  radio.setup(RESET_PIN, MW_BAND_TYPE); //
  radio.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM/SSB mode (between 12 and 90dB)
  

  spr.createSprite(320,170);
  spr.setTextDatum(4);
  spr.setSwapBytes(true);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.setTextColor(color1,TFT_BLACK);

  leds[0] = CRGB::Red;
  leds[1] = CRGB::White;
  leds[2] = CRGB::Red;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Red;
  leds[5] = CRGB::Blue;
  leds[6] = CRGB::Red;
  FastLED.show();
  drawSprite();

}

void readEncoder() {

  static int pos = 0;
  encoder.tick();

  if(digitalRead(0)==0){
    if(deb==0){
      deb=1;
      muted=!muted;
      radio.setAudioMute(muted);
      drawSprite();
      delay(200);
    }
  }else deb=0;
  
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    
    if(newPos>pos)
    value=value-1;
      if(newPos<pos)
      value=value+1;
    
    pos = newPos;
    
    drawSprite();
  } 

}

void drawSprite()
{
freq=value/10.00;
if(muted==false)
radio.setFrequency(freq);

getCurrentReceivedSignalQuality();
radio.getCurrentRSSI();

spr.fillSprite(TFT_BLACK);
spr.setTextColor(TFT_WHITE,TFT_BLACK);

spr.drawFloat(freq,1,160,64,7);
spr.setFreeFont(&Orbitron_Light_24);
spr.drawString("FM Radio",160,12);
spr.drawString("STATIONS",38,14,2);
spr.drawRoundRect(1,1,76,110,4,0xAD55);
spr.drawRoundRect(240,20,76,22,4,TFT_WHITE);

spr.drawRect(290,6,20,9,TFT_WHITE);
spr.fillRect(291,7,12,7,0x34CD);
spr.fillRect(310,8,2,5,TFT_WHITE);

spr.setTextFont(0);
spr.setTextColor(0xBEDF,TFT_BLACK);
for(int i=0;i<6;i++){
spr.drawString(sta[i],38,32+(i*12));
spr.fillCircle(16,31+(i*12),2,0xFBAE);
}
spr.setTextColor(TFT_WHITE,TFT_BLACK);

spr.drawString("SIGNAL:",266,54);
spr.drawString("MUTED",260,102,2);
spr.fillRoundRect(288,96,20,20,3,0xCC40);

if(muted==1)
spr.fillCircle(297,105,6,TFT_WHITE);


for(int i=0;i<strength;i++)
spr.fillRect(244+(i*4),80-(i*1),2,4+(i*1),0x3526);



spr.fillTriangle(156,104,160,114,164,104,TFT_RED);

  
 int temp=value-20;
 for(int i=0;i<40;i++)
 {
 if((temp%10)==0){
  spr.drawLine(i*8,170,i*8,140,color1);
  
  spr.drawLine((i*8)+1,170,(i*8)+1,140,color1);
  spr.drawFloat(temp/10.0,1,i*8,130,2);
  }
 else if((temp%5)==0 && (temp%10)!=0)
 {spr.drawLine(i*8,170,i*8,150,color1);
 spr.drawLine((i*8)+1,170,(i*8)+1,150,color1);
 //spr.drawFloat(temp/10.0,1,i*8,144);
 }
 else
  {spr.drawLine(i*8,170,i*8,160,color1);}
 
  temp=temp+1;
 }
spr.drawString("Stereo: "+String(radio.getCurrentPilot()),275,31,2);


spr.drawLine(160,114,160,170,TFT_RED);
spr.pushSprite(0,0);

}

void loop() { 

  readEncoder();
}
