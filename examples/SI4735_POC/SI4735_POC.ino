/*
   Test and validation of the SI4735 Arduino Library.

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0



unsigned currentFrequency;
unsigned previousFrequency;

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  Serial.println("Test and validation of the SI4735 Arduino Library.");
  Serial.println("AM and FM station tuning test.");

  showHelp();

  delay(500);

  si4735.setup(RESET_PIN, INTERRUPT_PIN, FM_FUNCTION);
  
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800,  10390, 10);
  delay(500);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}


void showHelp() {
  Serial.println("Type F to FM; A to MW; L to LW; and 1 to SW");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  Serial.print("You are tuned on ");
  if (si4735.isCurrentTuneFM() ) {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.println(" MHz");
  } else {
    Serial.print(currentFrequency);
    Serial.println(" KHz");
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
        si4735.setAM(570, 1710,  810, 10);
        break;
      case 'f':
      case 'F':
        si4735.setFM(8600, 10800,  10390, 10);
        break;
      case 'U':
      case 'u':
        si4735.frequencyUp();
        break;
      case 'D':
      case 'd':
        si4735.frequencyDown();
        break;
      case 'S':
        si4735.seekStationUp();
        break;
      case 's':
        si4735.seekStationDown();
        break;
      case '?':
        showHelp();
        break;
      default:
        break;
    }
  }

  currentFrequency = si4735.getFrequency();
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }

  delay(10);
}
