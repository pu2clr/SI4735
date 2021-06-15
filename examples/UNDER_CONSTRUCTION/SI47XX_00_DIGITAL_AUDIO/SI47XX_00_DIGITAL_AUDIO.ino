/*
   
   Under construction...
    
   Digital Audio output test. 

   First of all, it is important to say that Crystal and digital audio mode cannot be used at the same time on SI47XX devices. 
   The document Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383; rev 0.8; page 6; there is the following note: 
   "Crystal and digital audio mode cannot be used at the same time".  
   So, for Digital Audio, you have to remove the crystal, and capacitors connected to the crystal from the circuit.

   
   It is a FM, MW and SW (1700kHz to 30000kHz)
   
   The main advantages of using this sketch are: 
    1) It is a easy way to check if your circuit is working;
    2) You do not need to connect any display device to make your radio works;
    3) You do not need connect any push buttons or encoders to change volume and frequency;
    4) The Arduino IDE is all you need to control the radio.  
   
   This sketch has been successfully tested on:
    1) Pro Mini 3.3V; 



    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | CLK (pin 17)    |     A5        |


   I strongly recommend starting with this sketch.

   Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 12

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
  // Starts the system using DIGITAL AUDIO setup.

  // Use SI473X_DIGITAL_AUDIO1       - Digital audio output (SI47XX pins: 3/DCLK, 24/LOUT/DFS, 23/ROUT/DIO)
  // Use SI473X_DIGITAL_AUDIO2       - Digital audio output (SI47XX pins: 3/DCLK, 2/DFS, 1/DIO)
  // Use SI473X_ANALOG_DIGITAL_AUDIO - Analog and digital audio outputs (24/LOUT/ 23/ROUT and 3/DCLK, 2/DFS, 1/DIO)
  
  si4735.setup(RESET_PIN, -1, FM_FUNCTION, SI473X_DIGITAL_AUDIO2);

  // 2 -> Digital Output Audio Sample Precision is 24 bits
  // 0 -> Output Mono Mode 
  // 0 -> Output Mode is I2S
  // 0 -> Output DCLK Edge (0 = use DCLK rising edge)
  si4735.digitalOutputFormat(2, 0, 0, 0);
  // Digital Output Sample Rate(32–48 ksps)
  si4735.digitalOutputSampleRate(48); 

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10570, 10);
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
  si4735.getStatus();
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
      si4735.setSeekAmSpacing(10); // spacing 50kHz
      break;
    case 'f':
    case 'F':
      si4735.setFM(8600, 10800, 10570, 10);
      break;
    case '1':
      si4735.setAM(100, 30000, 9600, 5);
      si4735.setSeekAmLimits(100, 30000);
      si4735.setSeekAmSpacing(5); // spacing 50kHz
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
      si4735.seekStationUp();
      break;
    case 's':
      si4735.seekStationDown();
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
    case 'X':
      si4735.setAudioMode(SI473X_ANALOG_AUDIO);
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
