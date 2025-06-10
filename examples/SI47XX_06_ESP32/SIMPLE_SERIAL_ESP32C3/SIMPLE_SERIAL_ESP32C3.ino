/*
  Simple example of using the PU2CLR SI4735 Arduino Library on ESP32C3.
  This sketch turns the ESP32C3 and the SI4735 into a small FM/AM receiver
  controlled via the Serial Monitor.

  Connections for a typical ESP32C3 (LUATOS):
    * RESET    -> GPIO8
    * SDA      -> GPIO4
    * SCL      -> GPIO5
  Connect the audio output pins of the SI4735 to an amplifier or headphone.

  Based on the SI4735_01_POC example of this library.
  By PU2CLR, adapted for ESP32C3, 2025.
*/

#include <SI4735.h>
#include <Wire.h>

// Pin definitions for ESP32C3
#define RESET_PIN    8  // GPIO8 connected to RST pin of SI4735
#define ESP32_I2C_SDA 4 // GPIO4 connected to SDIO (pin 18)
#define ESP32_I2C_SCL 5 // GPIO5 connected to SCLK (pin 17)

#define AM_FUNCTION 1
#define FM_FUNCTION 0

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};

SI4735 rx;

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  // setup I2C pins explicitly for ESP32C3
  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);

  Serial.println("SI4735 ESP32C3 Serial Monitor Demo");
  showHelp();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The SI473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }

  delay(500);
  rx.setup(RESET_PIN, FM_FUNCTION);      // Start in FM mode
  rx.setFM(8400, 10800, 10650, 10);      // 84-108 MHz, start at 106.5 MHz
  delay(500);
  currentFrequency = previousFrequency = rx.getFrequency();
  rx.setVolume(45);
  showStatus();
}

void showHelp()
{
  Serial.println("Commands: F=FM, A=AM, U/D=Freq +/-, S/s=Seek, +=Vol+, -=Vol-, B=BW, 0=Status, ?=Help");
  Serial.println("==================================================");
}

void showStatus()
{
  previousFrequency = currentFrequency = rx.getFrequency();
  rx.getCurrentReceivedSignalQuality();
  Serial.print("Tuned to ");
  if (rx.isCurrentTuneFM())
  {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print(" MHz ");
    Serial.print((rx.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    Serial.print(currentFrequency);
    Serial.print(" kHz");
  }
  Serial.print(" [SNR:");
  Serial.print(rx.getCurrentSNR());
  Serial.print("dB Signal:");
  Serial.print(rx.getCurrentRSSI());
  Serial.println("dBuV]");
}

void showFrequency( uint16_t freq )
{
  if (rx.isCurrentTuneFM())
  {
    Serial.print(String(freq / 100.0, 2));
    Serial.println(" MHz");
  }
  else
  {
    Serial.print(freq);
    Serial.println(" kHz");
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case '+':
        rx.volumeUp();
        break;
      case '-':
        rx.volumeDown();
        break;
      case 'a':
      case 'A':
        rx.setAM(520, 1710, 1000, 10);     // MW band example
        break;
      case 'f':
      case 'F':
        rx.setFM(8400, 10800, 10650, 10);  // FM band
        break;
      case 'U':
      case 'u':
        rx.frequencyUp();
        break;
      case 'D':
      case 'd':
        rx.frequencyDown();
        break;
      case 'B':
      case 'b':
        if (!rx.isCurrentTuneFM())
        {
          if (bandwidthIdx > 6) bandwidthIdx = 0;
          rx.setBandwidth(bandwidthIdx, 1);
          Serial.print("AM Filter: ");
          Serial.print(bandwidth[bandwidthIdx]);
          Serial.println(" kHz");
          bandwidthIdx++;
        }
        break;
      case 'S':
        rx.seekStationProgress(showFrequency, 1);
        break;
      case 's':
        rx.seekStationProgress(showFrequency, 0);
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
  delay(100);
  currentFrequency = rx.getCurrentFrequency();
  if (currentFrequency != previousFrequency)
  {
    previousFrequency = currentFrequency;
    showStatus();
    delay(300);
  }
}

