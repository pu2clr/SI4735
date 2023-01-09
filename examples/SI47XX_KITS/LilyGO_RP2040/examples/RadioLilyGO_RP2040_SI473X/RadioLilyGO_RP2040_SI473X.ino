/*

  UNDER CONSTRUCTION...

  LilyGO T-Display and SI4735 wire up 
  
  | Si4735 pin     |  PICO Pin  |
  | ---------------| ---------- |
  | RESET (pin 15) |     GP16   |
  | SDIO (pin 18)  |     GP0    |
  | CLK (pin 17)   |     GP1    |


  | Rotary Encoder  |  PICO Pin  |
  | ----------------| ---------- |
  | RESET (pin 15)  |     GP16   |
  | SDIO (pin 18)   |     GP0    |
  | CLK (pin 17)    |     GP1    |


 
  Copy the lib/TFT_eSPI folder from the  https://github.com/Xinyuan-LilyGO/LILYGO-T-display-RP2040 to you Arduino IDE, libraries folder

  
*/

#include <Wire.h>
#include <SI4735.h>

#include "TFT_eSPI.h"
#include <RotaryEncoder.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

#define ENCODER_PIN_A   9       // to be checked
#define ENCODER_PIN_B  10       // to be checked
#define ENCODER_PUSH_BUTTON 21

#define RESET_PIN 16
#define I2C_SDA 8               
#define I2C_SCL 9               

// LilyGO T-Display reserved pins
#define TFT_BL 4
#define PIN_PWR_ON 22
#define PIN_BOTTON1 6
#define PIN_BOTTON2 7
#define PIN_RED_LED 25
#define PIN_BAT_VOL 26

#define FM_BAND_TYPE 0
#define MW_BAND_TYPE 1
#define SW_BAND_TYPE 2
#define LW_BAND_TYPE 3


RotaryEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, RotaryEncoder::LatchMode::TWO03);

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

  // Turn the 
  pinMode(PIN_PWR_ON, OUTPUT);
  digitalWrite(PIN_PWR_ON, HIGH);

  tft.begin();
  tft.writecommand(0x11);
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // pinMode(0, INPUT_PULLUP);

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();
  
  delay(500);
  // radio.setI2CFastModeCustom(100000);
  // radio.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error
  // radio.setup(RESET_PIN, MW_BAND_TYPE);
  // delay(200);
  // radio.setTuneFrequencyAntennaCapacitor(0);
  // radio.setFM(6400, 10800, 10390, 10);
  // delay(200);
  // freq = radio.getFrequency() / 100.0;

  // radio.setVolume(58);

  spr.createSprite(320, 170);
  spr.setTextDatum(4);
  spr.setSwapBytes(true);
  spr.setFreeFont(&Orbitron_Light_24);
  spr.setTextColor(color1, TFT_BLACK);

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
      // radio.setAudioMute(muted);
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

  // if (encoderCount == 1)
    // radio.frequencyUp();
  // else if (encoderCount == -1)
  //  radio.frequencyDown();

  freq = 10390 / 100.0;
  value = freq * 10;

  // strength=getStrength();

  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);

  spr.drawFloat(freq, 1, 100, 40, 7);
  spr.setTextColor(TFT_WHITE, TFT_BLACK);

  /*
  for (int i = 0; i < strength; i++)
  {
    if (i < 9)
      spr.fillRect(100 + (i * 4), 50 - (i * 1), 2, 4 + (i * 1), TFT_GREEN);
    else
      spr.fillRect(100 + (i * 4), 50 - (i * 1), 2, 4 + (i * 1), TFT_RED);
  }
  */
  // if (radio.getCurrentPilot())
  //   spr.drawString("Stereo", 275, 31, 2);
  // else
  //   spr.drawString("Mono", 275, 31, 2);

  // spr.drawLine(160, 114, 160, 170, TFT_RED);
  spr.pushSprite(0, 0);
}

int getStrength()
{

  uint8_t rssi;

  // rssi = radio.getCurrentRSSI();

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

  return 17;

}

void loop()
{
  readEncoder();
  // radio.getCurrentReceivedSignalQuality();
  strength = getStrength();

  delay(5);
}
