/*

   This sketch tests the method/function getProperty of the Si4735 Arduino Library

   The table below shows the Si4735 and Arduino Pro Mini pin connections

    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | CLK (pin 17)   |     A5        |

  I strongly recommend starting with this sketch.

  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

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
  while (!Serial);

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
  si4735.setup(RESET_PIN, AM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setAM(520, 1750, 810, 10);
  si4735.setSeekAmLimits(520, 1750);
  si4735.setSeekAmSpacing(10); // spacing 50kHz
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
  Serial.println("Type P to show some properties.");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}


/**
    setProperty and getProperty public methods should be used with caution. You have to know what are you doing.
    The masc parameter is used to extract the information from the returned value of getProperty.
*/
void showProperty(uint16_t property, uint16_t masc ) {

  Serial.print("\n**************************************\n");
  Serial.print("Property.:" );
  Serial.print(property, HEX);
  Serial.print(" -> ");
  Serial.print(si4735.getProperty(property) & masc , BIN);
  Serial.print(" -> ");
  Serial.print(si4735.getProperty(property) & masc , HEX);
  Serial.print(" -> ");
  Serial.println(si4735.getProperty(property) & masc);
}

void showProperties() {
  showProperty(RX_VOLUME, 0b1111111111111111); // Get volume value from Si47XX device.

  showProperty(AM_CHANNEL_FILTER, 0b0000000000001111); //the bandwidth of the channel filter for AM reception. The choices are 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz).

  showProperty(AM_NB_RATE, 0b1111111111111111); // Noise blanking rate in 100 Hz units. Default value is 64.

  showProperty(REFCLK_FREQ, 0b1111111111111111); // Crystal reference clock in Hz

  showProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0b1111111111111111); // maximum gain for automatic volume control.

  showProperty(AM_SEEK_RSSI_THRESHOLD, 0b1111111111111111); // RSSI threshold for a valid AM Seek/Tune.


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
        si4735.setFM(8600, 10800, 10390, 10);
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
      case '?':
        showHelp();
        break;
      case 'P':
      case 'p':
        showProperties();
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
