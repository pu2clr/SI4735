/*
   Test and validation of the SI4735 Arduino Library and ESP32.

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

  | Si4735    | Function              |ESP LOLIN32 WEMOS (GPIO) |
  |-----------| ----------------------|-------------------------|
  | pin 15    |   RESET               |   12 (GPIO12)           |  
  | pin 18    |   SDIO                |   21 (SDA / GPIO21)     |
  | pin 17    |   SCLK                |   22 (SCL / GPIO22)     |


   I strongly recommend starting with this sketch.

   Prototype documentation : https://pu2clr.github.io/SI4735/
   PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 12              // GPIO12

#define TOUCH_BAND_BUTTON_UP 13   // Next band (GPIO13)
#define TOUCH_BAND_BUTTON_DOWN 14 // Previous band (GPIO14)

// I2C bus pin on ESP32
#define ESP32_I2C_SDA 21     // GPIO21
#define ESP32_I2C_SCL 22     // GPIO22 

#define CAPACITANCE 30


#define AM_FUNCTION 1
#define FM_FUNCTION 0

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

SI4735 si4735;

void showHelp()
{

  Serial.println("Type F to FM; A to MW; L to LW; and 1 to SW");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type 0 to show current status");
  Serial.println("Type B to change Bandwidth filter");
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



int touchUp, touchDown;


int readX(int pin) {
  int val;
  val = 0;
  for (int i = 0; i < 50; i++ )
    val += touchRead(pin);
  return (val / 50);  
}



void setup()
{
  Serial.begin(9600);
  while(!Serial);

  digitalWrite(RESET_PIN, HIGH);
  
  Serial.println("AM and FM station tuning test.");

  showHelp();

  // The line below may be necessary to setup I2C pins on ESP32
  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);

  delay(500);
  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10390, 10);
  delay(500);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}


// Main
void loop()
{

  touchUp = readX(TOUCH_BAND_BUTTON_UP);
  touchDown = readX(TOUCH_BAND_BUTTON_DOWN);
  
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
      si4735.setAM(570, 1710, 810, 10);
      break;
    case 'f':
    case 'F':
      si4735.setFM(8600, 10800, 10390, 10);
      break;
    case '1':
      si4735.setAM(9400, 9990, 9600, 5);
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
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  } else if ( (touchUp < CAPACITANCE) ) // goes to the next band
  {
    si4735.seekStationUp();;
    delay(100);
  }
  else if ( (touchDown < CAPACITANCE) ) // goes to the previous band
  {
    si4735.seekStationDown();
    delay(100);
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
