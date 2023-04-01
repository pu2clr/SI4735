/*
  This sketch shows how to setup the ESP32 as a 32.768 kHz clock source. 
  This way, you can not need a active crystal or another signal generator.
  Use the Serial Monitor to interact with the receiver.

  SI4735 and ESP32 I2C wireup

  | Si4735    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin 15    |   RESET   |   GPIO12            |
  | pin 18    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 17    |   SCLK    |   22 (SCL / GPIO22) |
  | pin 19    |   RCLK    |   26                |

  On SI4735, the active crystal or external clock must be connected to the pin 19

  SI4735 and ESP32 I2S wireup

  SI4732 and ESP32 I2C and RCLK wireup (not checked so far)

  | SI4732    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin  9    |   RESET   |   GPIO12            |
  | pin 12    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 11    |   SCLK    |   22 (SCL / GPIO22) |
  | pin 13    |   RCLK    |   26                |

  On SI4732, the active crystal or external clock must be connected to the pin 13

  Ricardo Lima Caratti - Apr, 2023

*/

#include <SI4735.h>
#include "driver/ledc.h"

#define RESET_PIN 12
#define RCLK_PIN 26

#define I2C_SDA 21
#define I2C_CLK 22


SI4735 rx;

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = {"6", "4", "3", "2", "1", "1.8", "2.5"};
uint8_t currentVolume = 40;

uint16_t amLastFrequency = 810;
uint16_t fmLastFrequency = 10390;

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_2_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 32768};

ledc_channel_config_t ledc_channel = {
    .gpio_num = RCLK_PIN,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 2};

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

void showStatus()
{
  currentFrequency = rx.getFrequency();
  rx.getStatus();
  rx.getCurrentReceivedSignalQuality();
  Serial.print("You are tuned on ");
  if (rx.isCurrentTuneFM())
  {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print((rx.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    Serial.print(currentFrequency);
    Serial.print("kHz");
  }
  Serial.print(" [SNR:");
  Serial.print(rx.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" Signal:");
  Serial.print(rx.getCurrentRSSI());
  Serial.println("dBuV");

  Serial.print(" Volume:");
  Serial.print(rx.getVolume());
  Serial.println("]");
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  digitalWrite(RESET_PIN, HIGH);

  Wire.begin(I2C_SDA, I2C_CLK);

  delay(500);
  Serial.println("\nrx.setup...");
  Serial.flush();

  // create 32768 clock with ESP32
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);

  // Sets active 32.768kHz crystal (32768Hz)
  rx.setRefClock(32768);      // Ref = 32768Hz
  rx.setRefClockPrescaler(1); // 32768 x 1 = 32768Hz

  rx.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_ANALOG_AUDIO, XOSCEN_RCLK); // Analog and external RCLK

  delay(1000);
  rx.setFM(8400, 10800, 10270, 10); // frequency station 10650 (106.50 MHz)

  delay(500);

  Serial.print("\nThe current frequency is: ");
  Serial.println(rx.getFrequency());
  Serial.flush();

  delay(2000);
  rx.setVolume(currentVolume);

  showHelp();
  showStatus();
}


void switchModeAmFm(uint16_t f) {

}


void loop()
{

  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
    case '+':
      rx.setVolume(++currentVolume);
      currentVolume = rx.getVolume();
      break;
    case '-':
      rx.setVolume(--currentVolume);
      currentVolume = rx.getVolume();
      break;
    case 'a':
    case 'A':
      switchModeAmFm(amLastFrequency);
      break;
    case 'f':
    case 'F':
      switchModeAmFm(fmLastFrequency);
      break;
    case 'U':
    case 'u':
      rx.frequencyUp();
      showStatus();
      delay(900);
      break;
    case 'D':
    case 'd':
      rx.frequencyDown();
      showStatus();
      delay(900);
      break;
    case 'b':
    case 'B':
      if (rx.isCurrentTuneFM())
      {
        Serial.println("Not valid for FM");
      }
      else
      {
        if (bandwidthIdx > 6)
          bandwidthIdx = 0;
        rx.setBandwidth(bandwidthIdx, 1);
        Serial.print("Filter - Bandwidth: ");
        Serial.print(String(bandwidth[bandwidthIdx]));
        Serial.println(" kHz");
        bandwidthIdx++;
      }
      break;
    case 'S':
      rx.seekStationUp();
      break;
    case 's':
      rx.seekStationDown();
      break;
    case '0':
      showStatus();
      delay(1200);
      break;
    case '?':
      showHelp();
      break;
    default:
      break;
    }
  }
}
