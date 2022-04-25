
/**
   
   Bandpass filter circuit tester.

   This receiver / sketch use only AM mode MW and SW bands. 

   This sketch uses the Arduino IDE serial monitor to check the switches.
   You can select the bandpass filter by using the keyboard and typing 0, 1, 2 or 3.

   By Ricardo Lima Caratti, Jul 2020.
*/

#include <SI4735.h>
#include "AutoBPF.h"


#define RESET_PIN 12

#define AM_FUNCTION 1
#define AUDIO_MUTE 1   



#define FILTER_PIN1   14
#define FILTER_PIN2   15

typedef struct
{
  const char *freqName;
  uint16_t minimumFreq;
  uint16_t maximumFreq;
  uint16_t currentFreq;
  uint16_t currentStep;
} Band;

Band band[] = {{" MW", 520,  1700, 810, 10},
               {"60m", 4700, 5200, 4850, 5},
               {"49m", 5700, 6200, 6000, 5},
               {"41m", 7100, 7600, 7300, 5},
               {"31m", 9300, 10000, 9600, 5},
               {"25m", 11400, 12200, 11940, 5},
               {"22m", 13500, 13900, 13740, 5},
               {"19m", 15000, 15800, 15300, 5},
               {"16m", 17400, 17900, 17850, 5},
               {"13m", 21400, 21800, 21525, 5},
               {"11m", 25600, 27500, 27220, 1}};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int currentFreqIdx = 3; // Default SW band is 31M

uint16_t currentFrequency;
uint8_t bpfValue;

AutoBPF bpf;            // Declare the Auto bandpass filter class.

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {};

  Serial.println("\nTest and validation of the Band Pass Filter working with the SI473X based receiver.\n");

  si4735.setAudioMuteMcuPin(AUDIO_MUTE);    // avoiding pop in the speaker

  bpf.setup(FILTER_PIN1, FILTER_PIN2);
  bpf.setFilter(0);



  // gets and sets the Si47XX I2C bus address.
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if (si4735Addr == 0)
  {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1)
      ;
  }
  else
  {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }


  delay(500);
  si4735.setup(RESET_PIN, AM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);

  delay(500);

  currentFrequency = si4735.getFrequency();
  si4735.setVolume(50);
  showHelp();
  showStatus();
}

void showHelp() {
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type X to show current status");
  Serial.println("Type W go direct to MW/AM");
  Serial.println("Type > to go to the next SW band");
  Serial.println("Type < to go to the previous SW band");
  Serial.println("\nType 0, 1, 2 or 3 to select the bandpass filter");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
}

// Show current frequency and status
void showStatus()
{

  delay(250);
  band[currentFreqIdx].currentFreq = currentFrequency = si4735.getFrequency();

  Serial.print("You are tuned on ");

  Serial.println("Band: ");
  Serial.println(band[currentFreqIdx].freqName);
  
  
  Serial.print(currentFrequency);
  Serial.print("kHz");
  si4735.getCurrentReceivedSignalQuality();
  Serial.print(" [SNR:");
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" RSSI:");
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");
   
  Serial.print("Current BPF: ");
  Serial.println(bpfValue);
}


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
      break;
    case '>':
      if (currentFreqIdx < lastBand)
      {
        currentFreqIdx++;
      }
      else
      {
        currentFreqIdx = 0;
      }
      si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
      si4735.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM mode

      delay(100);
      currentFrequency = band[currentFreqIdx].currentFreq;
      showStatus();
      break;
    case '<':
      if (currentFreqIdx > 0)
      {
        currentFreqIdx--;
      }
      else
      {
        currentFreqIdx = lastBand;
      }
      si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
      delay(100);
      currentFrequency = band[currentFreqIdx].currentFreq;
      showStatus();
      break;
    case 'W':
    case 'w':
      currentFreqIdx = 0;
      si4735.setAM(band[currentFreqIdx].minimumFreq, band[currentFreqIdx].maximumFreq, band[currentFreqIdx].currentFreq, band[currentFreqIdx].currentStep);
      delay(100);
      currentFrequency = band[currentFreqIdx].currentFreq;
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
    case '0':
    case '1':
    case '2':
    case '3':
      bpfValue = key - '0'; // Converts char digit number to integer value.
      Serial.print("\nYou selected the BPF ");
      Serial.print(bpfValue);  
      bpf.setFilter(bpfValue);
      Serial.print("\n\nCheck the receiver...\n\n");
      showStatus();
      break;
    default:
      break;
    }
  }
  delay(5);
}
