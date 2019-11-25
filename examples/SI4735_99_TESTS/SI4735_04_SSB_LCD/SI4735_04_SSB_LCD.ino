/*
  SS4735 SSB Test. Under constuction...
  Arduino Library example with LCD 20x4 I2C.
  Rotary Encoder: This sketch uses the Rotary Encoder Class implementation from Ben Buxton.
  The source code is included together with this sketch.

  By Ricardo Lima Caratti, Nov 2019.
*/


#include <LiquidCrystal_I2C.h>
#include <SI4735.h>
#include "patch_content2.h"
#include "Rotary.h"

#define AM_FUNCTION 1
// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

#define RESET_PIN 12

// Enconder PINs
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

// Buttons controllers
#define BANDWIDTH_BUTTON 5 // Next Band
#define BAND_BUTTON_UP 8   // Band Up
#define BAND_BUTTON_DOWN 9 // Band Down
#define BFO_UP 6           // BFO Up
#define BFO_DOWN 7         // BFO Down
// Seek Function

#define MIN_ELAPSED_TIME 100

long elapsedButton = millis();

// Encoder control variables
volatile int encoderCount = 0;

// Some variables to check the SI4735 status
unsigned currentFrequency;
unsigned previousFrequency;

byte bandwidthIdx = 0;
char *bandwitdth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

typedef struct {
  unsigned   minimumFreq;
  unsigned   maximumFreq;
  unsigned   currentFreq;
  unsigned   currentStep;
  byte       currentSsbMode;
} Band;

Band band[] = {
  {3500, 4000, 3750, 1, LSB_MODE},
  {7000, 7300, 7100, 1, LSB_MODE},
  {14000, 14400, 14200, 1, USB_MODE},
  {18000, 19000, 18100, 1, USB_MODE},
  {2100, 21400, 21200, 1, USB_MODE},
  {27000, 27500, 27220, 1, USB_MODE},
  {28000, 28500, 28400, 1, USB_MODE}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 1; // 40M

byte rssi = 0;
byte stereo = 1;
byte volume = 0;

int currentBFO = 0;
int previousBFO = 0;

// Devices class declarations
Rotary encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);

LiquidCrystal_I2C display(0x27, 20, 4); // please check the address of your I2C device

SI4735 si4735;

void setup()
{

  Serial.begin(9600);
  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  pinMode(BANDWIDTH_BUTTON, INPUT);
  pinMode(BAND_BUTTON_UP, INPUT);
  pinMode(BAND_BUTTON_DOWN, INPUT);
  pinMode(BFO_UP, INPUT);
  pinMode(BFO_DOWN, INPUT);

  display.init();


  delay(500);

  // Splash - Change it for your introduction text.
  display.setCursor(6, 0);
  display.print("Si4735");
  display.setCursor(2, 1);
  display.print("Arduino Library");
  delay(500);
  display.setCursor(4, 3);
  display.print("SSB Test");
  display.setCursor(4, 4);
  display.print("By PU2CLR");
  delay(3000);
  display.clear();
  // end Splash

  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

  si4735.setup(RESET_PIN, 15);

  applyPatch();

  delay(500);
  si4735.setup(RESET_PIN, 0);

  delay(500);
  display.clear();

  si4735.setup(RESET_PIN, 1);
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setSsbConfig(1, 0, 0, 1, 0, 1);
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep, band[currentFreqIdx].currentSsbMode);

  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(60);

  showStatus();
}

// Use Rotary.h and  Rotary.cpp implementation to process encoder via interrupt
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

// Show current frequency
void showStatus()
{
  String unit, freqDisplay;
  String bandMode;

  bandMode = String("SSB");
  unit = "KHz";
  freqDisplay = String(currentFrequency);


  display.setCursor(0, 0);
  display.print(String(bandMode));

  display.setCursor(16, 0);
  display.print(unit);

  display.setCursor(5, 0);
  display.print("        ");
  display.setCursor(5, 0);
  display.print(freqDisplay);

  display.setCursor(0, 2);
  display.print("            ");
  display.setCursor(0, 2);
  display.print("BW: ");
  display.print(String(bandwitdth[bandwidthIdx]));
  display.print(" KHz");
}

/* *******************************
   Shows RSSI status
*/
void showRSSI()
{
  int blk;

  display.setCursor(12, 4);
  display.print("S:");
  display.print(rssi);
  display.print(" dBuV");
}

void showBFO()
{
  display.setCursor(0, 4);
  display.print("          ");
  display.setCursor(0, 4);
  display.print("BFO: ");
  display.print(currentBFO);
  display.print("Hz");

}

void bandUp() {

  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if ( currentFreqIdx < lastBand ) {
    currentFreqIdx++;
  } else {
    currentFreqIdx = 0;
  }

  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep, band[currentFreqIdx].currentSsbMode);

  currentBFO = 0;

}

void bandDown() {
  // save the current frequency for the band
  band[currentFreqIdx].currentFreq = currentFrequency;
  if ( currentFreqIdx > 0 ) {
    currentFreqIdx--;
  } else {
    currentFreqIdx = lastBand;
  }
  si4735.setTuneFrequencyAntennaCapacitor(1); // Set antenna tuning capacitor for SW.
  si4735.setSSB(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep, band[currentFreqIdx].currentSsbMode);
  currentBFO = 0;
}

/*
   Power Up with patch configuration
   See Si47XX PROGRAMMING GUIDE; page 219 and 220
*/
void prepereSi4735ToPatch()
{
  si4735.waitToSend();
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0xE2); // Set to FM Transmit, set patch enable, enable interrupts.
  Wire.write(0x50); // Set to Analog Line Input.
  Wire.endTransmission();
  delayMicroseconds(2500);
  si4735.waitToSend();
}

void applyPatch()
{

  int offset = 0;
  int i = 0;
  byte content;

  display.setCursor(1, 2);
  display.print("Applying Patch..");

  delay(500);
  prepereSi4735ToPatch();

  // Send patch for whole SSBRX initialization string
  for (offset = 0; offset < size_content_initialization; offset += 8)
  {
    Wire.beginTransmission(SI473X_ADDR);
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_initialization + (i + offset));
      Wire.write(content);
    }
    Wire.endTransmission();
    si4735.waitToSend();
    delayMicroseconds(600);
  }

  // Send patch for whole SSBRX full download
  for (offset = 0; offset < size_content_full; offset += 8)
  {
    Wire.beginTransmission(SI473X_ADDR);
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_full + (i + offset));
      Wire.write(content);
    }
    Wire.endTransmission();
    si4735.waitToSend();
    delayMicroseconds(600);
  }
  delay(250);
  display.setCursor(2, 2);
  display.print("Patch Applayed.");

}

/*
   Main
*/
void loop()
{

  // Check if the encoder has moved.
  if (encoderCount != 0)
  {

    if (encoderCount == 1)
      si4735.frequencyUp();
    else
      si4735.frequencyDown();

    encoderCount = 0;
  }

  // Check button commands
  if (digitalRead(BANDWIDTH_BUTTON) | digitalRead(BAND_BUTTON_UP) | digitalRead(BAND_BUTTON_DOWN) | digitalRead(BFO_UP) | digitalRead(BFO_DOWN))
  {

    // check if some button is pressed
    if (digitalRead(BANDWIDTH_BUTTON) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
    {
      bandwidthIdx++;
      if (bandwidthIdx > 6)  bandwidthIdx = 0;

      si4735.setBandwidth(bandwidthIdx, 0);
      showStatus();
    }
    else if (digitalRead(BAND_BUTTON_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      bandUp();
    else if (digitalRead(BAND_BUTTON_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
      bandDown();
    else if (digitalRead(BFO_UP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME) {
      currentBFO += 100;
      si4735.setSsbBfo(currentBFO);
    }
    else if (digitalRead(BFO_DOWN) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME) {
      currentBFO -= 100;
      si4735.setSsbBfo(currentBFO);
    }
    elapsedButton = millis();
  }


  // Show the current frequency only if it has changed
  currentFrequency = si4735.getFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
  }

  // Show RSSI status only if this condition has changed
  if (rssi != si4735.getCurrentRSSI())
  {
    rssi = si4735.getCurrentRSSI();
    showRSSI();
  }

  if ( currentBFO != previousBFO ) {
    previousBFO = currentBFO;
    showBFO();
  }
  delay(5);
}
