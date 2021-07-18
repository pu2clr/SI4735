/*
   Test and validation of the SI4735 Arduino Library.
   This example shows how can setup the radio on SW by using the Si4735 Arduino Library.
   This sketch divides the HF spectrum in many bands. See band table.
   
   ATTENTION:  Please, avoid using the computer connected to the mains during testing.

   The main advantages of using this sketch are:
    1) It is a easy way to check if your circuit is working;
    2) You do not need to connect any display device to make your radio works;
    3) You do not need connect any push buttons or encoders to change volume and frequency;
    4) The Arduino IDE is all you need to control the radio.

   This sketch has been successfully tested on:
    1) Pro Mini 3.3V; 
    2) UNO (by using a voltage converter); 
    3) Arduino Yún;
    4) Arduino Mega (by using a voltage converter); and 
    5) ESP32 (LOLIN32 WEMOS)

    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | SCLK (pin 17)   |     A5        |


  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0


typedef struct {
  const char *freqName;
  uint16_t   minimumFreq;
  uint16_t   maximumFreq;
  uint16_t   currentFreq;
  uint16_t   currentStep;
} Band;


Band band[] = {{"60m",4700, 5200, 4850, 5},
  {"49m",5700, 6200, 6000, 5},
  {"41m",7100, 7600, 7300, 5},
  {"31m",9300, 10000, 9600, 5},
  {"25m",11400, 12200, 11940, 5},
  {"22m",13500, 13900, 13600, 5},
  {"19m",15000, 15800, 15200, 5},
  {"16m",17400, 17900, 17600, 5},
  {"13m",21400, 21800, 21500, 5},
  {"11m",25600, 27500, 27220, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 3; // Default SW band is 31M

uint16_t currentFrequency;

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Test and validation of the SI4735 Arduino Library.");
  Serial.println("AM and FM station tuning test.");


  // gets and sets the Si47XX I2C bus address.
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }

  showHelp();

  delay(500);

  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800,  10390, 10);

  delay(500);

  currentFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}

// Instructions
void showHelp() {
  Serial.println("Type F to FM; A to MW; and 1 or 2 to SW");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type X to show current status");
  Serial.println("Type W to switch to SW");
  Serial.println("Type 1 to go to the next SW band");
  Serial.println("Type 2 to go to the previous SW band");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency and status
void showStatus()
{

  delay(250);
  band[currentFreqIdx].currentFreq = currentFrequency = si4735.getFrequency();

  Serial.print("You are tuned on ");
  if (si4735.isCurrentTuneFM() ) {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
  } else {
    Serial.print(currentFrequency);
    Serial.print("kHz");
  }

  si4735.getCurrentReceivedSignalQuality();
  Serial.print(" [SNR:" );
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" RSSI:" );
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");

}


void showBandName() {
  Serial.println("Band: ");
  Serial.println(band[currentFreqIdx].freqName);
  Serial.println("*******");  
}

// Main
void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case '+':
        si4735.volumeUp();
        break;
      case '-':
        si4735.volumeDown();
        break;
      case 'a':
      case 'A':
        si4735.setAM(570, 1710,  810, 10);
        si4735.setAvcAmMaxGain(32); // Sets the maximum gain for automatic volume control on AM mode
        showStatus();
        break;
      case 'f':
      case 'F':
        si4735.setFM(8600, 10800,  10390, 10);
        showStatus();
        break;
      case '2':
        if ( currentFreqIdx < lastBand ) {
          currentFreqIdx++;
        } else {
          currentFreqIdx = 0;
        }
        si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
        si4735.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM mode

        delay(100);
        currentFrequency = band[currentFreqIdx].currentFreq;
        showBandName();
        showStatus();
        break;
      case '1':
        if ( currentFreqIdx > 0 ) {
          currentFreqIdx--;
        } else {
          currentFreqIdx = lastBand;
        }
        si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
        delay(100);
        currentFrequency = band[currentFreqIdx].currentFreq;
        showBandName();
        showStatus();
        break;
      case 'W':
      case 'w':
        si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
        delay(100);
        currentFrequency = band[currentFreqIdx].currentFreq;
        showBandName();
        showStatus();         
        break;  
      case 'U':
      case 'u':
        si4735.frequencyUp();
        showStatus();
        break;
      case 'D':
      case 'd':
        si4735.frequencyDown();
        showStatus();
        break;
      case 'S':
        si4735.seekStationUp();
        showStatus();
        break;
      case 's':
        si4735.seekStationDown();
        showStatus();
        break;
      case 'X':
        showStatus();
        break;
      case '?':
        showHelp();
        break;
      default:
        break;
    }
  }

  delay(200);
}
