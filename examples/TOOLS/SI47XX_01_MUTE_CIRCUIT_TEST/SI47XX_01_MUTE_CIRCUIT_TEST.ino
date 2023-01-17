/*
   Test and validation of the SI4735 Arduino Library and EXTRA/EXTERNAL mute circuit.
   Some users may be uncomfortable with the loud popping of the speaker during some transitions caused by some Si47XX device commands. 
   This problem occurs during the receiver transition from the power down to power up internal commands. 
   Also, every time the user changes the mode (FM to AM or AM to FM) the sequence power down and power up internal commands is required by the Si47XX devices.
   The SI47XX devices have about 0,7 V DC  (DC bias) component in the analog audio output pins (SI4735-D60 pins 23 and 24). 
   When the device goes to power down mode, the voltage on the audio pins drops to 0V. 
   The device do it internally and there is not a way to avoid that. 
   When the device goes to power up, that audio pins suddenly goes to the  0,7V DC again. 
   This transition causes the loud pop in the speaker. 
   So far, the author of this library have not found an internal solution to solve the loud popping of the speaker. 
   It is important to say that internal SI47XX mute or volume commands will not work for this purpose. 
   However, it is possible to solve this problem by adding an extra mute circuit and control it by the MCU (Atmega, ESP32, STM32, ATtiny85 etc).
   See mute circuit schematic on: https://github.com/pu2clr/SI4735#when-the-receiver-starts-or-when-i-switches-it-from-fm-to-am-and-vice-versa-i-have-loud-click-in-the-speaker
   
   ATTENTION:  Please, avoid using the computer connected to the mains during testing.

   The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
   | Si4735 pin      |  Arduino Pin  |
   | ----------------| ------------  |
   | RESET (pin 15)  |     12        |
   | SDIO (pin 18)   |     A4        |
   | CLK (pin 17)    |     A5        |

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

   By Ricardo Lima Caratti, Nov 2020.
*/

#include <SI4735.h>

#define RESET_PIN 12      // Digital arduino pin used to reset the SI473X device. 
#define AUDIO_MUTE_PIN 5  // Digital arduino pin used to external mute circuit.


#define AM_FUNCTION 1
#define FM_FUNCTION 0

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  digitalWrite(RESET_PIN, HIGH);


  // MUTE CIRCUIT CONTROL
  // Be sure you connected the AUDIO_MUTE_PIN to the external mute circuit.  
  // All you have to do to mute the audio during device transition is: 
  si4735.setAudioMuteMcuPin(AUDIO_MUTE_PIN);
  
  Serial.println("AM and FM station tuning test.");

  showHelp();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }


  delay(500);
  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10390, 10);
  delay(500);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}

void showHelp()
{

  Serial.println("Type F to FM; A to MW; 1 to All Band (100kHz to 30MHz)");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type 0 to show current status");
  Serial.println("Type B to change Bandwidth filter");
  Serial.println("Type 4 to 8 (4 to step 1; 5 to step 5kHz; 6 to 10kHz; 7 to 100kHz; 8 to 1000kHz)");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  // si4735.getStatus();
  previousFrequency = currentFrequency = si4735.getFrequency();
  si4735.getCurrentReceivedSignalQuality();
  Serial.print("You are tuned on ");
  if (si4735.isCurrentTuneFM())
  {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    Serial.print(currentFrequency);
    Serial.print("kHz");
  }
  Serial.print(" [SNR:");
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" Signal:");
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");
}

void showFrequency( uint16_t freq ) {

  if (si4735.isCurrentTuneFM())
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
      si4735.setAM(520, 1750, 810, 10);
      si4735.setSeekAmLimits(520, 1750);
      si4735.setSeekAmSpacing(10); // spacing 10kHz
      break;
    case 'f':
    case 'F':
      si4735.setFM(8600, 10800, 10390, 50);
      si4735.setSeekAmRssiThreshold(0);
      si4735.setSeekAmSNRThreshold(10);
      break;
    case '1':
      si4735.setAM(100, 30000, 7200, 5);
      si4735.setSeekAmLimits(7100, 7500);   // Range for seeking.
      si4735.setSeekAmSpacing(1); // spacing 1kHz
      break;
    case 'U':
    case 'u':
      si4735.frequencyUp();
      break;
    case 'D':
    case 'd':
      si4735.frequencyDown();
      break;
    case 'b':
    case 'B':
      if (si4735.isCurrentTuneFM())
      {
        Serial.println("Not valid for FM");
      }
      else
      {
        if (bandwidthIdx > 6)
          bandwidthIdx = 0;
        si4735.setBandwidth(bandwidthIdx, 1);
        Serial.print("Filter - Bandwidth: ");
        Serial.print(String(bandwidth[bandwidthIdx]));
        Serial.println(" kHz");
        bandwidthIdx++;
      }
      break;
    case 'S':
      si4735.seekStationProgress(showFrequency,1);
      // si4735.seekStationUp();
      break;
    case 's':
      si4735.seekStationProgress(showFrequency,0);
      // si4735.seekStationDown();
      break;
    case '0':
      showStatus();
      break;
    case '4':
      si4735.setFrequencyStep(1);
      break;  
    case '5':
      si4735.setFrequencyStep(5);
      break;    
    case '6':
      si4735.setFrequencyStep(10);
      break;
    case '7':
      si4735.setFrequencyStep(100);
      break;
    case '8':
      si4735.setFrequencyStep(1000);
      break;
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  }
  delay(100);
  currentFrequency = si4735.getCurrentFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }
}
