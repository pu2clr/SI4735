/*
  This sketch was an adaptation of the Volos's sketch.
  The original VolosR's sketch works with TEA5767 device and Arduino Library.
  This sketch works with SI473X CI family and PU2CLR SI4735 Arduino Library

  LilyGo T-Embed device wire up

  | Device name      | Device Pin / Description  |  ESP32        |
  | ---------------- | --------------------------| ------------  |
  |    LilyGo        | GPIO16 used for resetting |               |
  |                  | SDA/SDIO                  |  GPI18        |
  |                  | SCL/SCLK                  |  GPI8         |


  ESP32 and SI4735-D60 or SI4732-A10 wire up


  | Si4735  | SI4732   | DESC.  | ESP32    (GPIO)    |
  |---------| -------- |--------|--------------------|
  | pin 15  |  pin 9   | RESET  | 16 (GPIO16)        |
  | pin 18  |  pin 12  | SDIO   | 18 (SDA / GPIO18)  |
  | pin 17  |  pin 11  | SCLK   |  8 (SCL / GPIO8)   |


  See Volos's original project here: https://github.com/VolosR/TEmbedFMRadio

*/

#include <Wire.h>
#include <SI4735.h>
#include <FastLED.h>
#include "TFT_eSPI.h"
#include <RotaryEncoder.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

#define PIN_IN1 2
#define PIN_IN2 1

#define NUM_LEDS 7
#define DATA_PIN 42
#define CLOCK_PIN 45

#define RESET_PIN 16
#define ESP32_SDA 18
#define ESP32_CLK 8

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3

CRGB leds[NUM_LEDS];

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

#define color1 0xC638
#define color2 0xC638

volatile int encoderCount = 0;

int value = 980;
int minimal = 880;
int maximal = 1080;
int strength = 0;
String sta[6] = {"96.6", "101.0", "89.4", "106,5", "98.2", "92.4"};

float freq = 0.00;
SI4735 radio;

bool muted = 0;
int deb = 0;

void setup()
{

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

  Wire.begin(ESP32_SDA, ESP32_CLK);

  radio.setI2CFastModeCustom(100000);
  radio.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error
  radio.setup(RESET_PIN, MW_BAND_TYPE);
  delay(200);
  radio.setTuneFrequencyAntennaCapacitor(0);
  radio.setFM(6400, 10800, 10390, 10);
  delay(200);
  freq = radio.getFrequency() / 100.0;

  radio.setVolume(58);

  spr.createSprite(320, 170);
  spr.setTextDatum(4);
  spr.setSwapBytes(true);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.setTextColor(color1, TFT_BLACK);

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

void readEncoder()
{

  static int pos = 0;

  encoderCount = 0;

  encoder.tick();

  if (digitalRead(0) == 0)
  {
    if (deb == 0)
    {
      deb = 1;
      muted = !muted;
      radio.setAudioMute(muted);
      drawSprite();
      delay(200);
    }
  }
  else
    deb = 0;

  int newPos = encoder.getPosition();
  if (pos != newPos)
  {

    if (newPos > pos)
    {
      value = value - 1;
      encoderCount = -1;
    }
    if (newPos < pos)
    {
      value = value + 1;
      encoderCount = 1;
    }

    pos = newPos;

    drawSprite();
  }
}

void drawSprite()
{

  // if(muted==false)

  if (encoderCount == 1)
    radio.frequencyUp();
  else if (encoderCount == -1)
    radio.frequencyDown();

  freq = radio.getFrequency() / 100.0;
  value = freq * 10;

  // strength=getStrength();

  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);

  spr.drawFloat(freq, 1, 160, 64, 7);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.drawString("SI473X", 160, 12);
  spr.drawString("STATIONS", 38, 14, 2);
  spr.drawRoundRect(1, 1, 76, 110, 4, 0xAD55);
  spr.drawRoundRect(240, 20, 76, 22, 4, TFT_WHITE);

  spr.drawRect(290, 6, 20, 9, TFT_WHITE);
  spr.fillRect(291, 7, 12, 7, 0x34CD);
  spr.fillRect(310, 8, 2, 5, TFT_WHITE);

  spr.setTextFont(0);
  spr.setTextColor(0xBEDF, TFT_BLACK);
  for (int i = 0; i < 6; i++)
  {
    spr.drawString(sta[i], 38, 32 + (i * 12));
    spr.fillCircle(16, 31 + (i * 12), 2, 0xFBAE);
  }
  spr.setTextColor(TFT_WHITE, TFT_BLACK);

  spr.drawString("SIGNAL", 266, 54);
  spr.drawString("MUTED", 260, 102, 2);
  spr.fillRoundRect(288, 96, 20, 20, 3, 0xCC40);

  if (muted == 1)
    spr.fillCircle(297, 105, 6, TFT_WHITE);

  for (int i = 0; i < strength; i++)
  {
    if (i < 9)
      spr.fillRect(244 + (i * 4), 80 - (i * 1), 2, 4 + (i * 1), TFT_GREEN);
    else
      spr.fillRect(244 + (i * 4), 80 - (i * 1), 2, 4 + (i * 1), TFT_RED);
  }

  spr.fillTriangle(156, 104, 160, 114, 164, 104, TFT_RED);

  int temp = value - 20;
  for (int i = 0; i < 40; i++)
  {
    if ((temp % 10) == 0)
    {
      spr.drawLine(i * 8, 170, i * 8, 140, color1);

      spr.drawLine((i * 8) + 1, 170, (i * 8) + 1, 140, color1);
      spr.drawFloat(temp / 10.0, 1, i * 8, 130, 2);
    }
    else if ((temp % 5) == 0 && (temp % 10) != 0)
    {
      spr.drawLine(i * 8, 170, i * 8, 150, color1);
      spr.drawLine((i * 8) + 1, 170, (i * 8) + 1, 150, color1);
      // spr.drawFloat(temp/10.0,1,i*8,144);
    }
    else
    {
      spr.drawLine(i * 8, 170, i * 8, 160, color1);
    }

    temp = temp + 1;
  }

  if (radio.getCurrentPilot())
    spr.drawString("Stereo", 275, 31, 2);
  else
    spr.drawString("Mono", 275, 31, 2);

  spr.drawLine(160, 114, 160, 170, TFT_RED);
  spr.pushSprite(0, 0);
}

int getStrength()
{

  uint8_t rssi;

  rssi = radio.getCurrentRSSI();

  if ((rssi >= 0) and (rssi <= 1))
    return 1; // S0
  if ((rssi > 1) and (rssi <= 1))
    return 2; // S1
  if ((rssi > 2) and (rssi <= 3))
    return  3; // S2
  if ((rssi > 3) and (rssi <= 4))
    return  4; // S3
  if ((rssi > 4) and (rssi <= 10))
    return  5; // S4
  if ((rssi > 10) and (rssi <= 16))
    return 6; // S5
  if ((rssi > 16) and (rssi <= 22))
    return 7; // S6
  if ((rssi > 22) and (rssi <= 28))
    return  8; // S7
  if ((rssi > 28) and (rssi <= 34))
    return 9; // S8
  if ((rssi > 34) and (rssi <= 44))
    return 10; // S9
  if ((rssi > 44) and (rssi <= 54))
    return 11; // S9 +10
  if ((rssi > 54) and (rssi <= 64))
    return 12; // S9 +20
  if ((rssi > 64) and (rssi <= 74))
    return 13; // S9 +30
  if ((rssi > 74) and (rssi <= 84))
    return 14; // S9 +40
  if ((rssi > 84) and (rssi <= 94))
    return 15; // S9 +50
  if (rssi > 94)
    return 16; // S9 +60
  if (rssi > 95)
    return 17; //>S9 +60

  return 0;

}

void loop()
{
  readEncoder();
  radio.getCurrentReceivedSignalQuality();
  strength = getStrength();

  delay(5);
}
