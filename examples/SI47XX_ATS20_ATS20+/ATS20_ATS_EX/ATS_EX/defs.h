#pragma once

//If you set this def to 0 project will be compiled without RDS 
//and everything related to RDS will be excluded from build
#define USE_RDS 1

#define EEPROM_APP_ID				235
#define EEPROM_DATA_START_ADDRESS	1
#define EEPROM_VERSION_ADDRESS      1000
#define EEPROM_APP_ID_ADDRESS       0

//EEPROM Settings
#define STORE_TIME 10000 // Inactive time to save our settings

// OLED Const values
#define DEFAULT_FONT FONT8X16POB
#define RST_PIN -1
#define RESET_PIN 12

//Battery charge monitoring analog pin (Voltage divider 10-10 KOhm directly from battery)
#define BATTERY_VOLTAGE_PIN A2

// Encoder
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Buttons
#define MODE_SWITCH       4 
#define BANDWIDTH_BUTTON  5
#define VOLUME_BUTTON     6
#define AVC_BUTTON        7
#define BAND_BUTTON       8 
#define SOFTMUTE_BUTTON   9
#define AGC_BUTTON       11
#define STEP_BUTTON      10

#define ENCODER_BUTTON   14

// Default values
#define MIN_ELAPSED_TIME 100
#define MIN_ELAPSED_RSSI_TIME 150
#define DEFAULT_VOLUME 25
#define ADJUSTMENT_ACTIVE_TIMEOUT 3000

// Band settings
#define SW_LIMIT_LOW		1710
#define SW_LIMIT_HIGH		30000
#define LW_LIMIT_LOW		153
#define CB_LIMIT_LOW		26200
#define CB_LIMIT_HIGH		28000

#define BAND_DELAY                 2
#define VOLUME_DELAY               1 

#define buttonEvent                NULL