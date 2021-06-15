/*
   Test and validation of the SI4735 Arduino Library and ESP8266.

   ATTENTION:  Please, avoid using the computer connected to the mains during testing.

   The main advantages of using this sketch are: 
    1) It is a easy way to check if your circuit is working;
    2) You do not need to connect any display device to make your radio works;
    3) You do not need connect any push buttons or encoders to change volume and frequency;
    4) The Arduino IDE is all you need to control the radio.  
   
   This sketch has been successfully tested on ESP8266 on SI4735-G60 or SI4732-A10

  | Si4735-G60 | Function              |   ESP8266             |
  |------------| ----------------------|-----------------------|
  | pin 15     |   RESET               |   2 (GPIO2)           |  
  | pin 18     |   SDIO                |   4 (SDA / GPIO4)     |
  | pin 17     |   SCLK                |   5 (SCL / GPIO5)     |


  | Si4732-A10 | Function              |   ESP8266             |
  |------------| ----------------------|-----------------------|
  | pin 9      |   RESET               |   2 (GPIO2)           |  
  | pin 12     |   SDIO                |   4 (SDA / GPIO4)     |
  | pin 11     |   SCLK                |   5 (SCL / GPIO5)     |



  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, May 2021.
*/

#include <SI4735.h>

#define RESET_PIN 2           // (GPIO02)

// I2C bus pin on ESP32
#define ESP32_I2C_SDA 4       // (GPIO04)
#define ESP32_I2C_SCL 5       // (GPIO05)

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
  Serial.flush();
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  delay(1000);

  digitalWrite(RESET_PIN, HIGH);
  Serial.println("AM and FM station tuning test.");

  showHelp();
  

  // The line below may be necessary to setup I2C pins on ESP32
  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);

  // Gets the current SI473X I2C address 
  int si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  si4735.setup(RESET_PIN, FM_FUNCTION);
  delay(300);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10390, 10);
  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
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
  }
  currentFrequency = si4735.getCurrentFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }

  delay(5);
}
