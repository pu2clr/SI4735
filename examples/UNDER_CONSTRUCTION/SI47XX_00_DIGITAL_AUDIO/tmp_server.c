/**
 * ESP32 I2S UDP Streamer
 *
 * This is influenced by maspetsberger's NoiseLevel at 
 * https://github.com/maspetsberger/esp32-i2s-mems/blob/master/examples/NoiseLevel/NoiseLevel.ino
 *
 * @author GrahamM
 */

#include <driver/i2s.h>
#include <soc/i2s_reg.h>
#include "WiFi.h"
#include "AsyncUDP.h"


// WiFi network name and password:
const char* ssidName = "<WiFiName>";
const char* ssidPswd = "<WiFiPassword>";

// UDP Destination
IPAddress udpAddress(192, 168, 0, 46);
const int udpPort = 4735;

// Connection state
boolean connected = false;

// ok

//The udp library class
AsyncUDP udp;

const i2s_port_t I2S_PORT = I2S_NUM_0;
const int BLOCK_SIZE = 128;

void setup() {
    Serial.begin(115200);
    Serial.println(" * Configuring WiFi");
    setupWiFi();
    Serial.println(" * Configuring I2S");
    I2SSetup();
    
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidName, ssidPswd);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while (1) {
            delay(1000);
        }
    }
}

void I2SSetup() {
    esp_err_t err;

    // The I2S config as per the example
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // We drive clk but only receive
                               // 112000 and below seem to be cleaner
        .sample_rate = 112000, //16000, 32000, 44100, 48000, 96000, 112000, 128000, 144000, 160000, 176000, 192000
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // WS signal must be BCLK/64 - this is how we manage it
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // Left by default
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt level 1
        .dma_buf_count = 4,                           // number of buffers
        .dma_buf_len = BLOCK_SIZE,                    // samples per buffer
    };

    // The pin config as per the setup
    const i2s_pin_config_t pin_config = {
        .bck_io_num = 14,   // Bit Clk
        .ws_io_num = 12,    // LR Clk
        .data_out_num = -1, // Data out
        .data_in_num = 32   // Data in
    };

    // Configuring the I2S driver and pins.
    err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.printf(" ! Failed installing driver: %d\n", err);
        while (true);
    }

    // Alterations for SPH0645 to ensure we receive MSB correctly.
    REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));   // I2S_RX_SD_IN_DELAY
    REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);  // Phillips I2S - WS changes a cycle earlier

    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        Serial.printf(" ! Failed setting pin: %d\n", err);
        while (true);
    }
    Serial.println(" + I2S driver installed.");
}

int32_t buffer[512];    // Effectively two 1024 byte buffers
volatile uint16_t rPtr = 0; // Yes, I should be using a pointer.

void mic_record()
{
    int num_bytes_read = i2s_read_bytes(I2S_PORT,
        (char*)buffer + rPtr,
        BLOCK_SIZE,     // Number of bytes
        portMAX_DELAY); // No timeout

    rPtr = rPtr + num_bytes_read;
    // Wrap this when we get to the end of the buffer
    if (rPtr > 2043) rPtr = 0;
}


void loop() {
    static uint8_t state = 0;
    mic_record();

    if (!connected) {
        if (udp.connect(udpAddress, udpPort)) {
            connected = true;
            Serial.println(" * Connected to host");
        }
    }
    else {
        switch (state) {
            case 0: // wait for index to pass halfway
                if (rPtr > 1023) {
                    state = 1;
                }
                break;
            case 1: // send the first half of the buffer
                state = 2;
                udp.write( (uint8_t *)buffer, 1024);
                break;
            case 2: // wait for index to wrap
                if (rPtr < 1023) {
                    state = 3;
                }
                break;
            case 3: // send second half of the buffer
                state = 0;
                udp.write((uint8_t*)buffer+1024, 1024);
                break;
        }
    }   
}