/*
   Bandwidth of the channel filter for AM reception.
   This sketch tests the bandwidth configuration on Si4735 device

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN   12
#define AM_FUNCTION  1


unsigned currentFrequency;
unsigned previousFrequency;
byte bandwidthIdx = 0;
char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  Serial.println("Test and validation of the SI4735 Arduino Library.");
  Serial.println("AM bandwidth of the channel filter for SW reception");

  showHelp();

  delay(500);

  si4735.setup(RESET_PIN, AM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setAM(570, 1700,  810, 10);

  delay(500);

  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(50);

  // Starts bandwidth with 6kHz
  si4735.setBandwidth(bandwidthIdx, 1);
  showStatus();  
  
}

void showHelp() {
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type S to show current status");
  Serial.println("Type B to change Bandwidth filter");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  Serial.print("[You are tuned on ");
  Serial.print(currentFrequency);
  Serial.print(" kHz][");
  Serial.print(" Signal:" );
  Serial.print(si4735.getCurrentRSSI());
  Serial.print("dBuV]");
  Serial.print("[Bandwidth: ");
  Serial.print(String(bandwidth[bandwidthIdx]));
  Serial.println(" kHz]");
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
         bandwidthIdx++;
        if ( bandwidthIdx > 6 ) bandwidthIdx = 0;
         si4735.setBandwidth(bandwidthIdx, 1);
        showStatus();
        break;
      case 'S':
        showStatus();
        break;
      case '?':
        showHelp();
        break;
      default:
        break;
    }
  }
  delay(100);
  currentFrequency = si4735.getFrequency();
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }
}
