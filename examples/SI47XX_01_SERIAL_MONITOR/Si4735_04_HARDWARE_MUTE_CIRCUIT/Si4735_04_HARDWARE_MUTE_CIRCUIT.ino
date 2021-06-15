/*

  This sketch shows how to control the external audio mute circuit.
  Some users may be uncomfortable with the loud popping of the speaker during some transitions caused by some SI47XX commands.  
  This problem occurs during the transition from the power down to power up. 
  Every time the user changes the mode (FM to AM or AM to FM) the power down and power up commands are required by the Si47XX devices.
  If you have a extra circuit in your receiver to mute the audio on amplifier input, you can configure an Arduino pin to control it by this library.

  All you have to do is calling the setAudioMuteMcuPin( <digital_arduino_pin> ) before call the receiver setup.
   
  Test and validation of the SI4735 Arduino Library.
  It is a FM, MW and SW (1700kHz to 30000kHz)
   

    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | AUDIO_MUTE      |     A0/14     |
    | SDIO (pin 18)   |     A4        |
    | CLK (pin 17)   |      A5        |


  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0

#define AUDIO_MUTE  14 // defines de Digital Pin 14 (A0 configured as digital) to enable and disable external mute circuit.

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};


SI4735 radio;

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  digitalWrite(RESET_PIN, HIGH);
  
  Serial.println("AM and FM station tuning test.");

  showHelp();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = radio.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }


  delay(500);

  // Comment and uncomment the line below to compare.
  radio.setAudioMuteMcuPin(AUDIO_MUTE); // Tells the system to control an external audio mute circuit. And it is enough. Finish.
  
  radio.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  radio.setFM(8400, 10800, 10390, 10);
  delay(500);
  currentFrequency = previousFrequency = radio.getFrequency();
  radio.setVolume(45);
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
  radio.getStatus();
  radio.getCurrentReceivedSignalQuality();
  Serial.print("You are tuned on ");
  if (radio.isCurrentTuneFM())
  {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print((radio.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    Serial.print(currentFrequency);
    Serial.print("kHz");
  }
  Serial.print(" [SNR:");
  Serial.print(radio.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" Signal:");
  Serial.print(radio.getCurrentRSSI());
  Serial.println("dBuV]");
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
      radio.volumeUp();
      break;
    case '-':
      radio.volumeDown();
      break;
    case 'a':
    case 'A':
      radio.setAM(520, 1750, 810, 10);
      radio.setSeekAmLimits(520, 1750);
      radio.setSeekAmSpacing(10); // spacing 50kHz
      break;
    case 'f':
    case 'F':
      radio.setFM(8600, 10800, 10390, 10);
      break;
    case '1':
      radio.setAM(100, 30000, 9600, 5);
      radio.setSeekAmLimits(100, 30000);
      radio.setSeekAmSpacing(5); // spacing 50kHz
      break;
    case 'U':
    case 'u':
      radio.frequencyUp();
      break;
    case 'D':
    case 'd':
      radio.frequencyDown();
      break;
    case 'b':
    case 'B':
      if (radio.isCurrentTuneFM())
      {
        Serial.println("Not valid for FM");
      }
      else
      {
        if (bandwidthIdx > 6)
          bandwidthIdx = 0;
        radio.setBandwidth(bandwidthIdx, 1);
        Serial.print("Filter - Bandwidth: ");
        Serial.print(String(bandwidth[bandwidthIdx]));
        Serial.println(" kHz");
        bandwidthIdx++;
      }
      break;
    case 'S':
      radio.seekStationUp();
      break;
    case 's':
      radio.seekStationDown();
      break;
    case '0':
      showStatus();
      break;
    case '4':
      radio.setFrequencyStep(1);
      break;  
    case '5':
      radio.setFrequencyStep(5);
      break;    
    case '6':
      radio.setFrequencyStep(10);
      break;
    case '7':
      radio.setFrequencyStep(100);
      break;
    case '8':
      radio.setFrequencyStep(1000);
      break;
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  }
  delay(100);
  currentFrequency = radio.getCurrentFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }
}
