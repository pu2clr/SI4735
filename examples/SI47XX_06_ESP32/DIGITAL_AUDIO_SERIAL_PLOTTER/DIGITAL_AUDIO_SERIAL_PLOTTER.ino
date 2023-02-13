/*
  This sketch was a  Jarno's contribution. 
  I added the SI4732 wireup dicumentation and changed the RESET pin setup. 
  Also I have refenrenced the documentations where this sketch was based on.
  I would like to thank Mr. Jarno for his contribution. 

  SI4735 and ESP32 I2C wireup

  | Si4735    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin 15    |   RESET   |   12 (GPIO17)       |  
  | pin 18    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 17    |   SCLK    |   22 (SCL / GPIO22) |

  SI4735 and ESP32 I2S wireup 

  | Si4735    | Function  |  DAC      | ESP LOLIN32 WEMOS (GPIO)              |
  |-----------| ----------|-----------|---------------------------------------|
  | pin 1     |   DIO     |  DIN      |  33 (SerialData / GPIO23)             |
  | pin 2     |   DFS     |  LRCK     |  25 (WordSelect / GPIO25)             |
  | pin 3     |   DCLK    |  BCK      |  32 (ContinuousSerialClock) / GPIO26) |


  SI4732 and ESP32 I2C wireup

  | SI4732    | Function  | ESP32               |
  |-----------| ----------|---------------------|
  | pin  9    |   RESET   |   12 (GPIO17)       |  
  | pin 12    |   SDIO    |   21 (SDA / GPIO21) |
  | pin 11    |   SCLK    |   22 (SCL / GPIO22) |

  SI4732 and ESP32 I2S wireup

  | SI4732   | Function   | DAC      | ESP LOLIN32 WEMOS (GPIO)              |
  |-----------| ----------|----------|---------------------------------------|  
  | pin  1    |  DFS      | LRCK     |  25 (WordSelect / GPIO25)             |
  | pin 16    |  DIO      | DIN      |  33 (SerialData / GPIO23)             |
  | pin  2    |  DCLK     | BSK      |  32 (ContinuousSerialClock) / GPIO26) |     

  NOTE: if you use PCM5102-module, connect SCK to GND

  IMPORTANT: This setup does not work with regular crystal setup. 
             You need a external active crystal or signal generator setup. 
             See Digital Audio support: https://pu2clr.github.io/SI4735/#digital-audio-support
             See SI473X and external active crystal oscillator or signal generator: https://github.com/pu2clr/SI4735/tree/master/extras/schematic#si473x-and-external-active-crystal-oscillator-or-signal-generator

  Reference
  I2S setup and Serial plotter code from https://dronebotworkshop.com/esp32-i2s/
  ESP32-audioI2S library: https://github.com/schreibfaul1/ESP32-audioI2S
  I2S from espressif ESP32:  https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/i2s.html

  Other references: 
  I2S Communication on ESP32 to Transmit and Receive Audio Data Using MAX98357A: https://circuitdigest.com/microcontroller-projects/i2s-communication-on-esp32-to-transmit-and-receive-audio-data-using-max98357a
  I2S Sound Tutorial for ESP32: https://diyi0t.com/i2s-sound-tutorial-for-esp32/
  Bluetooth A2DP – Streaming from an Digital I2S Microphone: https://www.pschatzmann.ch/home/2021/04/29/bluetooth-a2dp-streaming-from-an-digital-i2s-microphone/
  A Simple Arduino Bluetooth Music Receiver and Sender for the ESP32: https://github.com/pschatzmann/ESP32-A2DP

  This sketch was written By Jarno Lehtinen, Fev, 2023 (https://github.com/mcgurk?tab=repositories). 

*/

#include <SI4735.h>
#include <driver/i2s.h>

#define RESET_PIN 12

SI4735 si4735;

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

// Define input buffer length
#define bufferLen 64
int16_t sBuffer[bufferLen];

const i2s_config_t i2s_config = {
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = 48000,
  .bits_per_sample = i2s_bits_per_sample_t(16),
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
  .intr_alloc_flags = 0,
  .dma_buf_count = 8,
  .dma_buf_len = bufferLen,
  .use_apll = false
};

const i2s_pin_config_t pin_config = {
  .bck_io_num = I2S_SCK,
  .ws_io_num = I2S_WS,
  .data_out_num = -1,
  .data_in_num = I2S_SD
};

void setup() {
  Serial.begin(115200);
  while(!Serial);

  digitalWrite(RESET_PIN, HIGH);
  
  Wire.begin();

  delay(500);
  Serial.println("\nsi4735.setup..."); 
  Serial.flush();

  // Use SI473X_DIGITAL_AUDIO1       - Digital audio output (SI4735 device pins: 3/DCLK, 24/LOUT/DFS, 23/ROUT/DIO )
  // Use SI473X_DIGITAL_AUDIO2       - Digital audio output (SI4735 device pins: 3/DCLK, 2/DFS, 1/DIO)
  // Use SI473X_ANALOG_DIGITAL_AUDIO - Analog and digital audio outputs (24/LOUT/ 23/ROUT and 3/DCLK, 2/DFS, 1/DIO)
  // XOSCEN_RCLK                     - Use external source clock (active crystal or signal generator)

  // si4735.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_ANALOG_DIGITAL_AUDIO, XOSCEN_RCLK); // Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS, DIO), external RCLK
  si4735.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_DIGITAL_AUDIO2, XOSCEN_RCLK); 
  Serial.println(" Done!");
  delay(500);
  si4735.setFM(8400, 10800, 10390, 10); // frequency station 10650 (106.50 MHz)
  delay(500);
  Serial.print("\nsi4735.getFrequency: "); 
  Serial.println(si4735.getFrequency());
  Serial.flush();
  delay(1000);
  si4735.setVolume(50);


  si4735.digitalOutputSampleRate(48000); // 48 or 48000? To be checked
  // si4735.digitalOutputSampleRate(48); // 48 or 48000? To be checked
  

  // OSIZE Dgital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).
  // OMONO Digital Output Mono Mode (0=Use mono/stereo blend ).
  // OMODE Digital Output Mode (0=I2S, 6 = Left-justified, 8 = MSB at second DCLK after DFS pulse, 12 = MSB at first DCLK after DFS pulse).
  // OFALL Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge) 
  si4735.digitalOutputFormat(0 /* OSIZE */, 0 /* OMONO */, 0 /* OMODE */, 0/* OFALL*/);

  delay(500);

  // Set up I2S
  
  // i2s_install();
  // i2s_setpin();
  // i2s_start(I2S_PORT);

  
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_start(I2S_NUM_0);
  

 
}

void loop() {

  // False print statements to "lock range" on serial plotter display
  // Change rangelimit value to adjust "sensitivity"
  int rangelimit = 20000;
  Serial.print(rangelimit * -1);
  Serial.print(" ");
  Serial.print(rangelimit);
  Serial.print(" ");


  // Get I2S data and place in data buffer
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_NUM_0, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);

  if (result == ESP_OK)
  {
    // Read I2S data buffer
    int16_t samples_read = bytesIn / 8;
    if (samples_read > 0) {
      float mean = 0;
      for (int16_t i = 0; i < samples_read; ++i) {
        mean += (sBuffer[i]);
      }

      // Average the data reading
      mean /= samples_read;

      // Print to serial plotter
      Serial.println(mean);
    }
  }
}
