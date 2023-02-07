# This script uses the arduino-cli to compile the arduino sketches using command line (without Arduino IDE).
# It is very useful to check the library on multiple boards types  after bug fixes and improvments. 
# It runs on a MacOS but you can port it to Linux o Windows easily.
# Some compilation results (bin and hex files) will be stores in your Download directory (/Users/<username>/Downloads/hex)
# ATTENTION: be careful with --output-dir arduino-cli option. Before compiling, this option removes all the contents of the last level folder. 
#            For example: if you use "--output-dir ~/Downloads", all the current content of the Downloads folder will be lost. 
#                         if you use "--output-dir ~/Downloads/hex", all current content of the hex folder will be lost and the Downloads 
#                         content will be preserved. 
# Ricardo Lima Caratti Mar 2022

# compiles POC
echo "POC"
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_01_POC --output-dir ~/Downloads/hex/atmega/SI4735_01_POC
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_02_POC_SW --output-dir ~/Downloads/hex/atmega/SI4735_02_POC_SW
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_03_POC_SSB --output-dir ~/Downloads/hex/atmega/SI4735_03_POC_SSB
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_03_POC_SSB_COMPRESSED --output-dir ~/Downloads/hex/atmega/SI4735_03_POC_SSB_COMPRESSED
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/Si4735_04_HARDWARE_MUTE_CIRCUIT --output-dir ~/Downloads/hex/atmega/Si4735_04_HARDWARE_MUTE_CIRCUIT
arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_06_SSB_EEPROM --output-dir ~/Downloads/hex/atmega/SI4735_06_SSB_EEPROM
arduino-cli compile -b arduino:avr:nano ./SI47XX_99_AUTO_BANDPASS_FILTER/SI473X_01_POC_SERIAL_MONITOR --output-dir ~/Downloads/hex/atmega/SI473X_01_POC_SERIAL_MONITOR

echo "LCD 20x4 All In One"
arduino-cli compile -b arduino:avr:nano ./SI47XX_02_LCD_20x4_I2C/SI473X_04_ALL_IN_ONE

echo "LCD_20x4 with SMETER I2C"
echo "**** Atmega328 - Nano"
arduino-cli compile -b arduino:avr:nano ./SI47XX_02_LCD_20x4_I2C/SI47XX_02_LCD_20x4_SMETER_I2C --output-dir ~/Downloads/hex/atmega/SI47XX_02_LCD_20x4_SMETER_I2C
echo "**** LGT8FX - Nano"
arduino-cli compile -b lgt8fx:avr:328 ./SI47XX_02_LCD_20x4_I2C/SI47XX_02_LCD_20x4_SMETER_I2C  --output-dir ~/Downloads/hex/lgt8fx/SI47XX_02_LCD_20x4_SMETER_I2C


echo "OLED"
arduino-cli compile -b arduino:avr:nano ./SI47XX_03_OLED_I2C/SI47XX_02_ALL_IN_ONE_OLED

echo "Nokia 5110 with Pro mini / Atmega328"
# arduino-cli compile -b arduino:avr:pro -u -p /dev/cu.usbserial-1420 ./SI47XX_09_NOKIA_5110/ALL_IN_ONE_ONE_ENCODER
arduino-cli compile -b arduino:avr:pro ./SI47XX_09_NOKIA_5110/ALL_IN_ONE_ONE_ENCODER  --output-dir ~/Downloads/hex/atmega/SI47XX_09_NOKIA_5110

echo "Nokia 5110 with LGT8FX"
arduino-cli compile  -b lgt8fx:avr:328  ./SI47XX_09_NOKIA_5110/ALL_IN_ONE_ONE_ENCODER --output-dir ~/Downloads/hex/lgt8fx/SI47XX_09_NOKIA_5110

echo "TFT"
echo "*** Atmega328 and ILI9225"
arduino-cli compile -b arduino:avr:nano ./SI47XX_04_TFT/SI47XX_01_TFT_ILI9225

## echo "*** LGT8FX and ILI9225"
## arduino-cli compile -b lgt8fx:avr:328 --build-property "build.extra_flags=-Os -w -Wextra -fno-exceptions  -fno-threadsafe-statics -flto" ./SI47XX_04_TFT/SI47XX_01_TFT_ILI9225

## echo "*** ST7735"
## 
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  ./SI47XX_04_TFT/SI47XX_03_TFT_ATMEGA328_ST7735 --output-dir ~/Downloads/hex/atmega/SI47XX_03_TFT_ATMEGA328_ST7735


echo "POC Active Crystal"
arduino-cli compile -b arduino:avr:nano ./SI47XX_11_NO_CRYSTAL/POC_ACTIVE_CRYSTAL
arduino-cli compile -b arduino:avr:nano ./SI47XX_11_NO_CRYSTAL/POC_LCD16x2
arduino-cli compile --fqbn arduino:sam:arduino_due_x ./SI47XX_11_NO_CRYSTAL/SI47XX_RDS_TOUCH_SHIELD_REF_CLOCK


# compiles Mirko V2
echo "Mirko V2 with Arduino Nano / Atmega328"
arduino-cli compile -b arduino:avr:nano ./SI47XX_02_for_Mirko_Pavleski_radio/MIRKO_V2 --output-dir ~/Downloads/hex/atmega/MIRKO_V2

echo "Mirko V2 with LGT8FX"
arduino-cli compile  -b lgt8fx:avr:328  ./SI47XX_02_for_Mirko_Pavleski_radio/MIRKO_V2 --output-dir ~/Downloads/hex/lgt8fx/MIRKO_V2


# compiles KIT ATS-20
echo "ATS-20 KIT"
echo "*** V7"
arduino-cli compile -b arduino:avr:nano ./SI47XX_KITS/AliExpress/SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V7
echo "*** V8"
arduino-cli compile -b arduino:avr:nano ./SI47XX_KITS/AliExpress/SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V8


# The command below will work if you fix the Tiny4kOLED_common.h error: macro "nop" passed 1 arguments, but takes just 0  void nop(void);
# arduino-cli compile -b lgt8fx:avr:328 ./SI47XX_KITS/AliExpress/SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V8


# compiles ESP32 LCD16x2_ALL_IN_ONE
echo "ESP32 LCD16x2_ALL_IN_ONE"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso ./SI47XX_06_ESP32/LCD16x2_ALL_IN_ONE --output-dir ~/Downloads/hex/ESP32/LCD16x2_ALL_IN_ONE

echo "ESP32 I2S Audio Example"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso ./SI47XX_06_ESP32/DIGITAL_AUDIO_SERIAL_PLOTTER --output-dir ~/Downloads/hex/ESP32/DIGITAL_AUDIO_SERIAL_PLOTTER


# compiles Gert's KIT
echo "Gert's KIT (ESP32)"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso ./SI47XX_KITS/GERT_BAAK/SI4735_2.8_TFT_SI5351_V4.2  --output-dir ~/Downloads/hex/ESP32/GERT_BAAK


# compiles 
echo "FELIX ANGGA"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso ./SI47XX_KITS/FELIX_ANGGA/SLAMETRADIO


# echo "LilyGO T-Embed - Needs the right TFT_eSPI library setup"
# arduino-cli compile --fqbn esp32:esp32:esp32s3 ./SI47XX_KITS/Lilygo_t_embed/examples/ALL_IN_ONE_T_Embed

# compile ESP8266
echo "ESP8266 All In One"
arduino-cli compile --fqbn esp8266:esp8266:generic  ./SI47XX_06_ESP8266/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/ESP8266/OLED_ALL_IN_ONE

# compiles SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 on Arduino DUE and Mega 
echo "Arduino RDS DUE and MEGA"
echo "*** DUE"
arduino-cli compile --fqbn arduino:sam:arduino_due_x ./SI47XX_16_ARDUINO_DUE_MEGA/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 --output-dir ~/Downloads/hex/DUE/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
echo "*** Mega"
arduino-cli compile --fqbn arduino:avr:mega ./SI47XX_16_ARDUINO_DUE_MEGA/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 --output-dir ~/Downloads/hex/MEGA/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2


# compiles SI47XX_03_RDS_TFT_ILI9225_NEW on Arduino Nano
echo "Arduino Nano - RDS TFT"
arduino-cli compile -b arduino:avr:nano ./SI47XX_10_RDS/SI47XX_03_RDS_TFT_ILI9225_NEW

# compiles STM32 sketch
echo "STM32"
# arduino-cli board -b stm32duino:STM32F1:genericSTM32F103C  details
arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C ./SI47XX_07_STM32/STM32_04_OLED_ALL_IN_ONE_V2 --output-dir ~/Downloads/hex/STM32/STM32_04_OLED_ALL_IN_ONE_V2

echo "STM32 - PLAMEN"
arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C ./SI47XX_KITS/PLAMEN/PU2CLR_SI5351_SI4732_STM32 --output-dir ~/Downloads/hex/STM32/PU2CLR_SI5351_SI4732_STM32

# compiles SEEEDUINO XIAO
echo "SEEEDUINO" 

# arduino-cli compile --fqbn Seeeduino:samd:seeed_XIAO_m0 -u -p /dev/cu.usbmodem14201   ./SI47XX_15_SEEEDUINO/OLED_ALL_IN_ONE
arduino-cli compile --fqbn Seeeduino:samd:seeed_XIAO_m0  ./SI47XX_15_SEEEDUINO/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/SEEEDUINO/OLED_ALL_IN_ONE

# compiles ATTiny85
echo "ATTINY85"
echo "*** SSB"
# arduino-cli board -b ATTinyCore:avr:attinyx5  details   
arduino-cli compile --fqbn ATTinyCore:avr:attinyx5:clock=1internal ./SI47XX_05_ATTINY85/SI47XX_03_SSB_Tiny4kOLED --output-dir ~/Downloads/hex/ATTIMY85/SI47XX_03_SSB_Tiny4kOLED
echo "*** OLED"
arduino-cli compile --fqbn ATTinyCore:avr:attinyx5  ./SI47XX_05_ATTINY85/SI47XX_02_ATTINY85_MINI_OLED_I2C --output-dir ~/Downloads/hex/ATTIMY85/SI47XX_02_ATTINY85_MINI_OLED_I2C

# compile Atmega128 e Atmega32
echo "ATMega128 MegaCore"
arduino-cli compile -b MegaCore:avr:128:bootloader=no_bootloader,LTO=Os_flto ./SI47XX_17_ATMEGA128/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/ATMEGA128/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/atmega128/OLED_ALL_IN_ONE

# echo "ATMega32  MightyCore"
# To know more option  run: arduino-cli board -b MightyCore:avr:32 details
arduino-cli compile -b MightyCore:avr:32:bootloader=no_bootloader,LTO=Os_flto,clock=16MHz_external  ./SI47XX_17_ATMEGA32/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/ATMEGA32/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/atmega32/OLED_ALL_IN_ONE
