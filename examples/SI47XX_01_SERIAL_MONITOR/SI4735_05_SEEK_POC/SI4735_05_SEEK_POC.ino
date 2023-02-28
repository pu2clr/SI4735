/*
   Test and validation of the seek function
    This sketch has been successfully tested on Pro Mini 3.3V; 
   The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | CLK (pin 17)    |     A5        |

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 12
#define AM_FUNCTION 1
#define FM_FUNCTION 0

uint16_t currentFrequency;
uint16_t previousFrequency;

SI4735 si47xx;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  digitalWrite(RESET_PIN, HIGH);
  Serial.println("AM and FM station tuning test.");
  showHelp();
  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = si47xx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }
  delay(500);
  si47xx.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si47xx.setFM(8400, 10800, 10390, 10);
  previousFrequency = currentFrequency = si47xx.getFrequency();
  delay(500);
  showFrequency(currentFrequency);
  si47xx.setVolume(45);
}

void showHelp()
{
  Serial.println("Type F to FM; A to MW; 1 to All Band (100kHz to 30MHz)");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down with progress");
  Serial.println("Type > or < to seek down or up ");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
}

// Show current frequency. This function also will be called by si47xx.seekStationProgress to show the seek progress.
void showFrequency( uint16_t freq ) {
  if (si47xx.isCurrentTuneFM())
  {
    Serial.print(String(freq / 100.0, 2));
    Serial.println("MHz ");
  }
  else
  {
    Serial.print(freq);
    Serial.println("kHz");
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
    case 'a':
    case 'A':
      si47xx.setAM(520, 1750, 810, 10);
      si47xx.setSeekAmLimits(520, 1750);
      si47xx.setSeekAmSpacing(10); // spacing 10kHz
      break;
    case 'f':
    case 'F':
      si47xx.setFM(8600, 10800, 10390, 50);
      si47xx.setSeekAmRssiThreshold(0);
      si47xx.setSeekAmSNRThreshold(10);
      break;
    case '1':
      si47xx.setAM(100, 30000, 7200, 5);
      si47xx.setSeekAmLimits(7100, 7500);   // Range for seeking.
      si47xx.setSeekAmSpacing(1);           // spacing 1kHz
      break;
    case 'U':
    case 'u':
      si47xx.frequencyUp();
      break;
    case 'D':
    case 'd':
      si47xx.frequencyDown();
      break;
    case 'S':
      si47xx.seekStationProgress(showFrequency,SEEK_UP); // Seek up and call showFrequency to show progress 
      break;
    case 's':
      si47xx.seekStationProgress(showFrequency,SEEK_DOWN); // // Seek down and call showFrequency to show progress 
      break;
    case '>':
      si47xx.seekStationUp();
      break;
    case '<':
      si47xx.seekStationDown();
      break;      
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  }

  currentFrequency = si47xx.getFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showFrequency(currentFrequency);
    delay(300);
  }  
}
