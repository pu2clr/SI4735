/*

  Under construction
  
 Test and validation of the SI4735 Arduino Library with NBFM support.

 ATTENTION:  Please, avoid using the computer connected to the mains during testing. 



 The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735-D60 pin  |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | SCLK (pin 17)   |     A5        |



Prototype documentation : https://pu2clr.github.io/SI4735/
PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

By Ricardo Lima Caratti, Nov 2019. 
*/

#include <SI4735.h>
#include "Rotary.h"
// Test it with patch_init.h or patch_full.h. Do not try load both.
// #include <patch_init.h> // SSB patch for whole SSBRX initialization string
#include <patch_full.h>    // SSB patch for whole SSBRX full download

const uint16_t size_content = sizeof patch_content; // see patch_content in patch_full.h or patch_init.h

#define NBFM_FUNCTION 0 // same FM
#define RESET_PIN 9 // 12

// Some variables to check the SI4735 status
uint16_t currentFrequency;
uint8_t currentStep = 5;

uint8_t bandwidthIdx = 2;
const char *bandwidth[] = {"1.2", "2.2", "3.0", "4.0", "0.5", "1.0"};


long et1 = 0, et2 = 0;


int currentFreqIdx = 0;

uint8_t rssi = 0;


SI4735 si4735;

void setup()
{

  Serial.begin(9600);
  while(!Serial);

  
  Serial.println("Si4735 Arduino Library");
  Serial.println("NBFM TEST");
  Serial.println("By PU2CLR");


  // Gets and sets the Si47XX I2C bus address
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }


  si4735.setup(RESET_PIN, NBFM_FUNCTION);

  si4735.setI2CFastMode();         // 400000 (400kHz)
  delay(10);
  Serial.println("NBFM patch is loading...");
  et1 = millis();
  loadNBFM();
  et2 = millis();
  Serial.print("NBFM patch was loaded in: ");
  Serial.print( (et2 - et1) );
  Serial.println("ms");
  delay(100);
  currentFrequency = 30000;
  si4735.setNBFM(25600, 43200, currentFrequency, currentStep);
  // currentFrequency = 10650;
  // si4735.setNBFM(6400, 10800, currentFrequency, currentStep);
  
  Serial.print("Aqui 1!");
  delay(100);
  currentFrequency = si4735.getFrequency();
  si4735.setVolume(60);
  Serial.print("Aqui 2!");
  showHelp();
  showStatus();
}

void showSeparator()
{
  Serial.println("\n**************************");
}

void showHelp()
{
  showSeparator();
  Serial.println("Type: ");
  Serial.println("U to frequency up or D to frequency down");
  Serial.println("> to go to the next band or < to go to the previous band");
  Serial.println("W to sitch the filter bandwidth");
  Serial.println("B to go to increment the BFO or b decrement the BFO");
  Serial.println("G to switch on/off the Automatic Gain Control");
  Serial.println("A to switch the LNA Gain Index (0, 1, 5, 15 e 26");
  Serial.println("S to switch the frequency increment and decrement step");
  Serial.println("s to switch the BFO increment and decrement step");
  Serial.println("X Shows the current status");
  Serial.println("H to show this help");
}

// Show current frequency
void showFrequency()
{
  String freqDisplay;
  freqDisplay = String((float)currentFrequency);
  showSeparator();
  Serial.print("Current Frequency: ");
  Serial.print(freqDisplay);
  Serial.print("MHz");
  Serial.print(" | Step: ");
  Serial.println(currentStep);
}

void showStatus()
{
  showSeparator();
  Serial.print("NBFM | ");

  // si4735.getAutomaticGainControl();
  // si4735.getCurrentReceivedSignalQuality();
    

  showFrequency();
}



/*
 * This function loads the contents of the patch_content array into the CI (Si4735) and starts the radio on
 * SSB mode.
 */
void loadNBFM()
{
  si4735.loadPatchNBFM(patch_content, size_content);
}
/*
   Main
*/
void loop()
{
  // Check if exist some command to execute
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    if (key == 'U' || key == 'u')
    { // frequency up
      si4735.frequencyUp();
      delay(250);
      currentFrequency = si4735.getCurrentFrequency();
      showFrequency();
    }
    else if (key == 'D' || key == 'd')
    { // frequency down
      si4735.frequencyDown();
      delay(250);
      currentFrequency = si4735.getCurrentFrequency();
      showFrequency();
    }
    else if (key == 'V')
    { // volume down
      si4735.volumeUp();
      showStatus();
    }
    else if (key == 'v')
    { // volume down
      si4735.volumeDown();
      showStatus();
    }
    else if (key == 'S') // switches the frequency increment and decrement step
    {
      if (currentStep == 1)
        currentStep = 5;
      else if (currentStep == 5)
        currentStep = 10;
      else
        currentStep = 1;
      si4735.setFrequencyStep(currentStep);
      showFrequency();
    }
    else if (key == 'X' || key == 'x') 
      showStatus();
    else if (key == 'H' || key == 'h')
      showHelp();
  }
  delay(200);
}
