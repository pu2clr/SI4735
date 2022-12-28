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

#define PIN_IN1 2
#define PIN_IN2 1

#define NUM_LEDS 7
#define DATA_PIN 42
#define CLOCK_PIN 45
CRGB leds[NUM_LEDS];

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);


#define color1 0xC638
#define color2  0xC638

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
