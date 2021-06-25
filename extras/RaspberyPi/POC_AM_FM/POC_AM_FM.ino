/*
   Raspberry PI test. 
   To Raspberry PI on Arduino IDE setup check: https://github.com/me-no-dev/RasPiArduino

   

    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Raspberry Pi  |
    | ----------------| ------------   |
    | RESET (pin 15)  |                |
    | SDIO (pin 18)   |                |
    | CLK (pin 17)    |                |

  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

   By Ricardo Lima Caratti, Nov 2021.
*/

#include <SI4735.h>
#include <stdio.h>

#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


SI4735 rx;

void setup()
{
  digitalWrite(RESET_PIN, HIGH);
  
  printf("AM and FM station tuning test.");

  showHelp();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    printf("Si473X not found!");
    // Serial.flush();
    while (1);
  } else {
    printf("The Si473X I2C address is %x", si4735Addr);
  }


  delay(500);
  rx.setup(RESET_PIN, FM_FUNCTION);
  // rx.setup(RESET_PIN, -1, 1, SI473X_ANALOG_AUDIO);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  rx.setFM(8400, 10800, 10650, 10);
  delay(500);
  currentFrequency = previousFrequency = rx.getFrequency();
  rx.setVolume(45);
  showStatus();
}

void showHelp()
{

  printf("\nType F to FM; A to MW; 1 to All Band (100kHz to 30MHz)");
  printf("\nType U to increase and D to decrease the frequency");
  printf("\nType S or s to seek station Up or Down");
  printf("\nType + or - to volume Up or Down");
  printf("\nType 0 to show current status");
  printf("\nType B to change Bandwidth filter");
  printf("\nType 4 to 8 (4 to step 1; 5 to step 5kHz; 6 to 10kHz; 7 to 100kHz; 8 to 1000kHz)");
  printf("\nType ? to this help.");
  printf("\n==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  // rx.getStatus();
  previousFrequency = currentFrequency = rx.getFrequency();
  rx.getCurrentReceivedSignalQuality();
  printf("\nYou are tuned on ");
  if (rx.isCurrentTuneFM())
  {
    printf("%6.2f", (currentFrequency / 100.0));
    printf("MHz ");
    printf((rx.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    printf("%5.0f", currentFrequency);
    printf("kHz ");
  }
  printf(" [SNR:");
  printf("%d",rx.getCurrentSNR());
  printf("dB");

  printf(" Signal:");
  printf("%d", rx.getCurrentRSSI());
  printf("dBuV]");
}

void showFrequency( uint16_t freq ) {

  if (rx.isCurrentTuneFM())
  {
    printf("\n%6.2f", freq / 100.0);
    printf("MHz ");
  }
  else
  {
    printf("\n%5.0f", freq);
    printf("kHz");
  }
  
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
      rx.volumeUp();
      break;
    case '-':
      rx.volumeDown();
      break;
    case 'a':
    case 'A':
      rx.setAM(520, 1750, 810, 10);
      rx.setSeekAmLimits(520, 1750);
      rx.setSeekAmSpacing(10); // spacing 10kHz
      break;
    case 'f':
    case 'F':
      rx.setFM(8600, 10800, 10390, 50);
      rx.setSeekAmRssiThreshold(0);
      rx.setSeekAmSrnThreshold(10);
      break;
    case '1':
      rx.setAM(100, 30000, 7200, 5);
      rx.setSeekAmLimits(100, 30000);   // Range for seeking.
      rx.setSeekAmSpacing(1); // spacing 1kHz
      printf("\nALL - LW/MW/SW");
      break;
    case 'U':
    case 'u':
      rx.frequencyUp();
      break;
    case 'D':
    case 'd':
      rx.frequencyDown();
      break;
    case 'b':
    case 'B':
      if (rx.isCurrentTuneFM())
      {
        printf("\nNot valid for FM");
      }
      else
      {
        if (bandwidthIdx > 6)
          bandwidthIdx = 0;
        rx.setBandwidth(bandwidthIdx, 1);
        printf("Filter - Bandwidth: ");
        printf("%s", bandwidth[bandwidthIdx]);
        printf(" kHz");
        bandwidthIdx++;
      }
      break;
    case 'S':
      rx.seekStationProgress(showFrequency,1);
      // rx.seekStationUp();
      break;
    case 's':
      rx.seekStationProgress(showFrequency,0);
      // rx.seekStationDown();
      break;
    case '0':
      showStatus();
      break;
    case '4':
      rx.setFrequencyStep(1);
      printf("\nStep 1");
      break;  
    case '5':
      rx.setFrequencyStep(5);
      printf("\nStep 5");
      break;    
    case '6':
      rx.setFrequencyStep(10);
      printf("\nStep 10");
      break;
    case '7':
      rx.setFrequencyStep(100);
      printf("\nStep 100");      
      break;
    case '8':
      rx.setFrequencyStep(1000);
      printf("\nStep 1000");    
      break;
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  }
  delay(100);
  currentFrequency = rx.getCurrentFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }
}
