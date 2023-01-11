/*
  | Si4735    | Function              | ESP LOLIN32 WEMOS (GPIO) |
  |-----------| ----------------------|--------------------------|
  | pin 15    |   RESET               |   17 (GPIO17)            |  
  | pin 18    |   SDIO                |   21 (SDA / GPIO21)      |
  | pin 17    |   SCLK                |   22 (SCL / GPIO22)      |

  | Si4735    | Function              | DAC       | ESP LOLIN32 WEMOS (GPIO)              |
  |-----------| ----------------------|-----------|---------------------------------------|
  | pin 1     |   DIO                 |  DIN      |  23 (SerialData / GPIO23)             |
  | pin 2     |   DFS                 |  LRCK     |  25 (WordSelect / GPIO25)             |
  | pin 3     |   DCLK                |  BCK      |  26 (ContinuousSerialClock) / GPIO26) |

  (analog audio out: ROUT 23, LOUT 24)
  note!: if you use PCM5102-module, connect SCK to GND

  I2S setup and Serial plotter code from https://dronebotworkshop.com/esp32-i2s/
*/

#include <SI4735.h>
#include <driver/i2s.h>

#define RESET_PIN 17

SI4735 si4735;

#define I2S_WS 25
#define I2S_SD 23
#define I2S_SCK 26

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
  Serial.print("si4735.setup...");
  //si4735.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_ANALOG_DIGITAL_AUDIO, XOSCEN_RCLK); // Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS, DIO), external RCLK
  si4735.setup(RESET_PIN, -1, FM_CURRENT_MODE, SI473X_ANALOG_DIGITAL_AUDIO, XOSCEN_CRYSTAL); // Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS, DIO), crystal
  Serial.println(" Done!");
  si4735.setFM(8400, 10800, 9430, 10); // frequency/station 94.30MHz
  delay(500);
  Serial.print("si4735.getFrequency: "); Serial.println(si4735.getFrequency());
  si4735.setVolume(63);

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_start(I2S_NUM_0);

  si4735.digitalOutputSampleRate(48000);
  si4735.digitalOutputFormat(0, 0, 0, 0);
  /* uint8_t	OSIZE Dgital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).
     uint8_t	OMONO Digital Output Mono Mode (0=Use mono/stereo blend ).
     uint8_t	OMODE Digital Output Mode (0=I2S, 6 = Left-justified, 8 = MSB at second DCLK after DFS pulse, 12 = MSB at first DCLK after DFS pulse).
     uint8_t	OFALL Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge) */
}

void loop() {

  // False print statements to "lock range" on serial plotter display
  // Change rangelimit value to adjust "sensitivity"
  int rangelimit = 3000;
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
