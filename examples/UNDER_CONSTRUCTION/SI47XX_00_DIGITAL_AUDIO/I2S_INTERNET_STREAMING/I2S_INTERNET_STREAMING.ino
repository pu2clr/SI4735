/*

  Under construction... 

  The station received by the SI473X device will be streaming via Internet.


  SI4735 and ESP32 I2C wireup

  | Si4735    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin 15    |   RESET   |   GPIO12            |
  | pin 18    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 17    |   SCLK    |   22 (SCL / GPIO22) |

  On SI4735, the active crystal or external clock must be connected to the pin 19

  SI4735 and ESP32 I2S wireup

  The table below shows the SI4735,  DAC MAX98357A and ESP32 wireup

  | Si4735    | Function  |  DAC MAX98357A  | ESP32                                 |
  |-----------| ----------|-----------------|---------------------------------------|
  | pin 1     | DOUT      |  DIN            |  SerialData / GPIO32                  |
  | pin 2     | DFS       |  RC             |  WordSelect / GPIO25                  |
  | pin 3     | DCLK      |  BCLK           |  ContinuousSerialClock) / GPIO33)     |

  The table below shows the SI4735,  DAC CJMCU and ESP32 wireup

  | Si4735    | Function  |  DAC MAX98357A  | ESP32                                 |
  |-----------| ----------|-----------------|---------------------------------------|
  | pin 1     | DOUT      |  DIN            |  SerialData / GPIO32                  |
  | pin 2     | DFS       |  WSEL           |  WordSelect / GPIO25                  |
  | pin 3     | DCLK      |  BCLK           |  ContinuousSerialClock) / GPIO33)     |


  SI4732 and ESP32 I2C wireup

  | SI4732    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin  9    |   RESET   |   GPIO12            |
  | pin 12    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 11    |   SCLK    |   22 (SCL / GPIO22) |

  SI4732 and ESP32 I2S wireup

  | SI4732   | Function   | DAC      | ESP LOLIN32 WEMOS (GPIO)              |
  |-----------| ----------|----------|---------------------------------------|
  | pin  1    |  DFS/WS   | LRCK     |  WordSelect / GPIO25                  |
  | pin 16    |  DIO/SD   | DIN      |  SerialData / GPIO32                  |
  | pin  2    |  DCLK/SCK | BSK      |  ContinuousSerialClock)  / GPIO33     |


  On SI4732, the active crystal or external clock must be connected to the pin 13


  IMPORTANT: This setup does not work with regular crystal setup.
             You need a external active crystal or signal generator setup.
             See Digital Audio support: https://pu2clr.github.io/SI4735/#digital-audio-support
             See SI473X and external active crystal oscillator or signal generator: https://github.com/pu2clr/SI4735/tree/master/extras/schematic#si473x-and-external-active-crystal-oscillator-or-signal-generator

  Reference
  I2S setup and Serial plotter code from https://dronebotworkshop.com/esp32-i2s/
  ESP32-audioI2S library: https://github.com/schreibfaul1/ESP32-audioI2S
  I2S from espressif ESP32:  https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/i2s.html


  Streaming: 

  https://www.hackster.io/julianfschroeter/stream-your-audio-on-the-esp32-2e4661
  * Como usar microfone no ESP32 (ESP32 voice streamer): https://www.dobitaobyte.com.br/como-usar-microfone-no-esp32-esp32-voice-streamer/
  https://gist.github.com/GrahamM/1d5ded26b23f808a80520e8c1510713a
  



  Other references:
  I2S Communication on ESP32 to Transmit and Receive Audio Data Using MAX98357A: https://circuitdigest.com/microcontroller-projects/i2s-communication-on-esp32-to-transmit-and-receive-audio-data-using-max98357a
  I2S Sound Tutorial for ESP32: https://diyi0t.com/i2s-sound-tutorial-for-esp32/
  Bluetooth A2DP – Streaming from an Digital I2S Microphone: https://www.pschatzmann.ch/home/2021/04/29/bluetooth-a2dp-streaming-from-an-digital-i2s-microphone/
  A Simple Arduino Bluetooth Music Receiver and Sender for the ESP32: https://github.com/pschatzmann/ESP32-A2DP
  Si4735 I2S module - https://gitlab.com/retrojdm/si4735-i2s-module

  This sketch was written By Jarno Lehtinen, Fev, 2023 (https://github.com/mcgurk?tab=repositories).

*/

#include <SI4735.h>
#include <driver/i2s.h>
#include <soc/i2s_reg.h>
#include "WiFi.h"
#include "AsyncUDP.h"

#define RESET_PIN 12

SI4735 rx;

#define I2S_WS 25
#define I2S_SD 32
#define I2S_SCK 33

#define I2C_SDA 21
#define I2C_CLK 22

// Define input buffer length
#define BUFFER_SIZE 1024

// WiFi connection - Network name and password
const char *ssidName = "NETVIRTUA201";
const char *ssidPswd = "15830970";

// UDP Destination

const int udpPort = 1234;


// UDP
AsyncUDP udp;

uint8_t stream_buffer[BUFFER_SIZE];

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t bandwidthIdx = 0;
const char *bandwidth[] = { "6", "4", "3", "2", "1", "1.8", "2.5" };
uint8_t currentVolume = 40;

uint16_t amLastFrequency = 810;
uint16_t fmLastFrequency = 10390;

const i2s_config_t i2s_config = {
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = 48000,
  .bits_per_sample = i2s_bits_per_sample_t(16),  // Resolution: More bits better quality
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
  .intr_alloc_flags = 0,
  .dma_buf_count = 8,
  .dma_buf_len = BUFFER_SIZE,
  .use_apll = false
};

const i2s_pin_config_t pin_config = {
  .bck_io_num = I2S_SCK,
  .ws_io_num = I2S_WS,
  .data_out_num = -1,
  .data_in_num = I2S_SD
};

void doHalt() { 
  while(1) delay(1);
}

void showHelp() {
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

// WiFi setup
void setupWiFi() {

  Serial.println("\nWifi - Network setup!");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidName, ssidPswd);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi connection Failed");
    doHalt();
  }

  Serial.println("Connected!");
  Serial.print("UDP Listening on IP: ");
  Serial.println(WiFi.localIP());

  // UDP connection 
  if (udp.connect(WiFi.localIP(), udpPort)) {
      Serial.println("Connected to host via UDP");
      delay(500);
  } else {
    Serial.println("UDP setup Failed");
    doHalt();
  }

  if (udp.listen(udpPort)) {
    Serial.print("UDP Listening to IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("UDP Port: ");
    Serial.println(udpPort);
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.print("UDP Packet Type: ");
      Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
      Serial.print(", From: ");
      Serial.print(packet.remoteIP());
      Serial.print(":");
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length());
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();
      //reply to the client
      packet.printf("Got %u bytes of data", packet.length());
    });
  }
}

void inline si473x_streaming() {
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_NUM_0, stream_buffer, BUFFER_SIZE, &bytesIn, portMAX_DELAY);
  if (result == ESP_OK) {
      udp.write((uint8_t *)stream_buffer, bytesIn);
  }
}

void showStatus() {
  currentFrequency = rx.getFrequency();
  rx.getStatus();
  rx.getCurrentReceivedSignalQuality();
  Serial.print("You are tuned on ");
  if (rx.isCurrentTuneFM()) {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print((rx.getCurrentPilot()) ? "STEREO" : "MONO");
  } else {
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

/**
 * Switch from FM to AM or AM to FM mode using Digital Audio Setup
 */
void switchModeAmFm(uint16_t f) {

  if (rx.isCurrentTuneFM()) {
    fmLastFrequency = currentFrequency;
    rx.setup(RESET_PIN, -1, AM_CURRENT_MODE, SI473X_DIGITAL_AUDIO2, XOSCEN_RCLK);
    rx.setAM(570, 1710, f, 10);
    rx.digitalOutputSampleRate(48000);
    rx.digitalOutputFormat(0, 0, 0, 0);
    rx.setVolume(currentVolume);
  } else {
    amLastFrequency = currentFrequency;
    rx.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_DIGITAL_AUDIO2, XOSCEN_RCLK);
    rx.setFM(8400, 10800, f, 10);
    rx.digitalOutputSampleRate(48000);
    rx.digitalOutputFormat(0, 0, 0, 0);
    rx.setVolume(currentVolume);
  }
  showStatus();
  delay(1000);
}


void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  digitalWrite(RESET_PIN, HIGH);

  Wire.begin(I2C_SDA, I2C_CLK);

  delay(500);
  Serial.println("\nrx.setup...");
  Serial.flush();

  // Sets active 32.768kHz crystal (32768Hz)
  rx.setRefClock(32768);       // Ref = 32768Hz
  rx.setRefClockPrescaler(1);  // 32768 x 1 = 32768Hz

  // Use SI473X_DIGITAL_AUDIO1       - Digital audio output (SI4735 device pins: 3/DCLK, 24/LOUT/DFS, 23/ROUT/DIO )
  // Use SI473X_DIGITAL_AUDIO2       - Digital audio output (SI4735 device pins: 3/DCLK, 2/DFS, 1/DIO)
  // Use SI473X_ANALOG_DIGITAL_AUDIO - Analog and digital audio outputs (24/LOUT/ 23/ROUT and 3/DCLK, 2/DFS, 1/DIO)
  // XOSCEN_RCLK                     - Use external source clock (active crystal or signal generator)
  rx.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_ANALOG_DIGITAL_AUDIO, XOSCEN_RCLK);  // Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS, DIO), external RCLK
  // rx.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_DIGITAL_AUDIO2, XOSCEN_RCLK);
  Serial.println("SI473X device started with Digital Audio setup!");
  delay(1000);
  rx.setFM(8400, 10800, 10650, 10);  // frequency station 10650 (106.50 MHz)
  // rx.setAM(570, 1710, 810, 10);
  delay(500);
  Serial.print("\nrx.getFrequency: ");
  Serial.println(rx.getFrequency());
  Serial.flush();
  delay(2000);
  Serial.print("\nThe current frequency is: ");
  Serial.println(rx.getFrequency());
  Serial.flush();
  delay(2000);
  rx.setVolume(currentVolume);

  Serial.print("\nSetting SI473X Sample rate to 48K.");
  rx.digitalOutputSampleRate(48000);

  delay(2000);

  // OSIZE Dgital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).
  // OMONO Digital Output Mono Mode (0=Use mono/stereo blend ).
  // OMODE Digital Output Mode (0=I2S, 6 = Left-justified, 8 = MSB at second DCLK after DFS pulse, 12 = MSB at first DCLK after DFS pulse).
  // OFALL Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge)
  rx.digitalOutputFormat(0 /* OSIZE */, 0 /* OMONO */, 0 /* OMODE */, 0 /* OFALL*/);

  Serial.print("\nSI473X device is setted to digital Audio.");

  delay(2000);

  Serial.print("\nSetting ESP32 I2S.");

  if ( i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK ) {
    Serial.println("I2S driver error during installing!");
    doHalt();
  }
  delay(500);


   // Alterations for SPH0645 to ensure we receive MSB correctly.
  REG_SET_BIT(I2S_TIMING_REG(I2S_NUM_0), BIT(9));   // I2S_RX_SD_IN_DELAY
  REG_SET_BIT(I2S_CONF_REG(I2S_NUM_0), I2S_RX_MSB_SHIFT);  // Phillips I2S - WS changes a cycle earlier

  if ( i2s_set_pin(I2S_NUM_0, &pin_config) !=  ESP_OK) { 
    Serial.println("I2S set pin error during!");
    doHalt();   
  } 
  delay(500);

  i2s_start(I2S_NUM_0);
  delay(500);
  
  Serial.print("\nI2S setup is done!");

  setupWiFi();

  delay(2000);

  showHelp();
  showStatus();
}

void loop() {
  si473x_streaming();

  if (Serial.available() > 0) {
    char key = Serial.read();
    switch (key) {
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
        if (rx.isCurrentTuneFM()) {
          Serial.println("Not valid for FM");
        } else {
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
