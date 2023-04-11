ECHO OFF
Rem This script uses the arduino-cli to compile the arduino sketches using command line (without Arduino IDE).
Rem It is very useful to check the library on multiple board types  after bug fixes and improvements. 
Rem It runs on a Windows CMD/Shell.
Rem Some compilation results (bin and hex files) will be stores in your Download directory (\Users\<username>\Downloads\hex)
Rem ATTENTION: be careful with --output-dir arduino-cli option. Before compiling, this option removes all the contents of the last level folder. 
Rem            For example: if you use "--output-dir %homepath%\Downloads", all the current content of the Downloads folder will be lost. 
Rem                         if you use "--output-dir Downloads\hex", all current content of the hex folder will be lost and the Downloads 
Rem                         content will be preserved. 
Rem  Ricardo Lima Caratti Mar 2022

ECHO "ATmega and LGTF8X based boards"
ECHO ON 
arduino-cli compile -b arduino:avr:nano .\SI47XX_01_SERIAL_MONITOR\SI4735_01_POC --output-dir %homepath%\Downloads\hex\atmega\SI4735_01_POC
arduino-cli compile -b lgt8fx:avr:328 .\SI47XX_01_SERIAL_MONITOR\SI4735_01_POC --output-dir %homepath%\Downloads\hex\lgt8fx\SI4735_01_POC
arduino-cli compile -b arduino:avr:nano .\SI47XX_02_for_Mirko_Pavleski_radio\MIRKO_V2 --output-dir %homepath%\Downloads\hex\atmega\MIRKO_V2
arduino-cli compile -b lgt8fx:avr:328   .\SI47XX_02_for_Mirko_Pavleski_radio\MIRKO_V2 --output-dir %homepath%\Downloads\hex\lgt8fx\MIRKO_V2
arduino-cli compile -b arduino:avr:nano .\SI47XX_03_OLED_I2C\SI47XX_02_ALL_IN_ONE_OLED --output-dir %homepath%\Downloads\hex\atmega\SI47XX_02_ALL_IN_ONE_OLED
arduino-cli compile -b arduino:avr:pro .\SI47XX_09_NOKIA_5110\ALL_IN_ONE_ONE_ENCODER --output-dir %homepath%\Downloads\hex\atmega\SI47XX_09_NOKIA_5110
arduino-cli compile -b lgt8fx:avr:328  .\SI47XX_09_NOKIA_5110\ALL_IN_ONE_ONE_ENCODER --output-dir %homepath%\Downloads\hex\lgt8fx\SI47XX_09_NOKIA_5110
arduino-cli compile -b arduino:avr:nano .\SI47XX_04_TFT\SI47XX_01_TFT_ILI9225 --output-dir %homepath%\Downloads\hex\atmega\SI47XX_01_TFT_ILI9225
arduino-cli compile -b arduino:avr:nano .\TOOLS\APPLYING_SSB_PATCH_WITHOUT_LIBRARIES --output-dir %homepath%\Downloads\hex\atmega\APPLYING_SSB_PATCH_WITHOUT_LIBRARIES
arduino-cli compile -b lgt8fx:avr:328 .\TOOLS\APPLYING_SSB_PATCH_WITHOUT_LIBRARIES --output-dir %homepath%\Downloads\hex\atmega\APPLYING_SSB_PATCH_WITHOUT_LIBRARIES
arduino-cli compile -b arduino:avr:nano .\TOOLS\SI47XX_02_STORE_EEPROM_BEFORE_SHUTDOWN --output-dir %homepath%\Downloads\hex\atmega\SI47XX_02_STORE_EEPROM_BEFORE_SHUTDOWN
arduino-cli compile -b lgt8fx:avr:328 .\TOOLS\SI47XX_02_STORE_EEPROM_BEFORE_SHUTDOWN --output-dir %homepath%\Downloads\hex\atmega\SI47XX_02_STORE_EEPROM_BEFORE_SHUTDOWN
arduino-cli compile -b arduino:avr:nano .\TOOLS\SI47XX_01_MUTE_CIRCUIT_TEST --output-dir %homepath%\Downloads\hex\atmega\SI47XX_01_MUTE_CIRCUIT_TEST
arduino-cli compile -b lgt8fx:avr:328 .\TOOLS\SI47XX_01_MUTE_CIRCUIT_TEST --output-dir %homepath%\Downloads\hex\atmega\SI47XX_01_MUTE_CIRCUIT_TEST
arduino-cli compile -b arduino:avr:nano .\TOOLS\SI47XX_90_FIRMWARE_INFORMATION --output-dir %homepath%\Downloads\hex\atmega\SI47XX_90_FIRMWARE_INFORMATION
arduino-cli compile -b lgt8fx:avr:328 .\TOOLS\SI47XX_90_FIRMWARE_INFORMATION --output-dir %homepath%\Downloads\hex\atmega\SI47XX_90_FIRMWARE_INFORMATION
arduino-cli compile -b arduino:avr:nano .\SI47XX_12_MORSE_CODE_READOUT --output-dir %homepath%\Downloads\hex\atmega\SI47XX_12_MORSE_CODE_READOUT
arduino-cli compile -b lgt8fx:avr:328 .\SI47XX_12_MORSE_CODE_READOUT --output-dir %homepath%\Downloads\hex\atmega\SI47XX_12_MORSE_CODE_READOUT


ECHO OFF
ECHO "ATmega 32 and 128 based boards"

ECHO ON 
arduino-cli compile -b MightyCore:avr:32:bootloader=no_bootloader,LTO=Os_flto,clock=16MHz_external  .\SI47XX_17_ATMEGA32\OLED_ALL_IN_ONE --output-dir %homepath%\Downloads\hex\atmega32\OLED_ALL_IN_ONE
arduino-cli compile -b MegaCore:avr:128:bootloader=no_bootloader,LTO=Os_flto .\SI47XX_17_ATMEGA128\OLED_ALL_IN_ONE --output-dir %homepath%\Downloads\hex\atmega128\OLED_ALL_IN_ONE 
ECHO OFF

ECHO "ATtiny Core"
ECHO ON 
arduino-cli compile --fqbn ATTinyCore:avr:attinyx5  .\SI47XX_05_ATTINY85\SI47XX_02_ATTINY85_MINI_OLED_I2C --output-dir %homepath%\Downloads\hex\ATTIMY85\SI47XX_02_ATTINY85_MINI_OLED_I2C 
echo "*** SSB"
arduino-cli compile --fqbn ATTinyCore:avr:attinyx5:clock=1internal .\SI47XX_05_ATTINY85\SI47XX_03_SSB_Tiny4kOLED --output-dir  %homepath%\Downloads\hex\ATTIMY85\SI47XX_03_SSB_Tiny4kOLED


ECHO OFF

ECHO "ESP8266 based board"
ECHO ON 
arduino-cli compile --fqbn esp8266:esp8266:generic  .\SI47XX_06_ESP8266\OLED_ALL_IN_ONE --output-dir %homepath%\Downloads\hex\esp8266\OLED_ALL_IN_ONE 
ECHO OFF

ECHO "ESP32 based board"
ECHO ON 
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso .\SI47XX_06_ESP32\LCD16x2_ALL_IN_ONE --output-dir %homepath%\Downloads\hex\esp32\OLED_ALL_IN_ONE 
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso .\SI47XX_KITS\GERT_BAAK\SI4735_2.8_TFT_SI5351_V4.2  --output-dir %homepath%\Downloads\hex\esp32\GERT_BAAK
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso .\SI47XX_06_ESP32\DIGITAL_AUDIO_SERIAL_PLOTTER  --output-dir %homepath%\Downloads\hex\esp32\DIGITAL_AUDIO_SERIAL_PLOTTER
ECHO OFF

ECHO "Raspbery Pico / RP2040 based board"
ECHO ON 
arduino-cli compile --fqbn rp2040:rp2040:rpipico .\SI47XX_13_RASPBERRY_PICO\POC_01  --output-dir %homepath%\Downloads\hex\RP2040\POC_01
arduino-cli compile --fqbn rp2040:rp2040:rpipico .\SI47XX_13_RASPBERRY_PICO\OLED_I2C  --output-dir %homepath%\Downloads\hex\RP2040\OLED_I2C
ECHO OFF


ECHO "Seeeduino:samd based board"
ECHO ON 
arduino-cli compile --fqbn Seeeduino:samd:seeed_XIAO_m0  .\SI47XX_15_SEEEDUINO\OLED_ALL_IN_ONE --output-dir %homepath%\Downloads\hex\seeeduino\OLED_ALL_IN_ONE 
ECHO OFF

ECHO "STM32 based board"
ECHO ON 
arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C ./SI47XX_07_STM32/STM32_04_OLED_ALL_IN_ONE_V2 --output-dir ~/Downloads/hex/STM32/F1/STM32_04_OLED_ALL_IN_ONE_V2
arduino-cli compile --fqbn STMicroelectronics:stm32:Disco    ./SI47XX_07_STM32/STM32_04_OLED_ALL_IN_ONE_V2 --output-dir ~/Downloads/hex/STM32/Disco/STM32_04_OLED_ALL_IN_ONE_V2 
arduino-cli compile --fqbn STMicroelectronics:stm32:GenF4 ./SI47XX_07_STM32/STM32_04_OLED_ALL_IN_ONE_V2 --output-dir ~/Downloads/hex/STM32/F4/STM32_04_OLED_ALL_IN_ONE_V2 
echo "STM32 - PLAMEN"
arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C ./SI47XX_KITS/PLAMEN/PU2CLR_SI5351_SI4732_STM32 --output-dir ~/Downloads/hex/STM32/PU2CLR_SI5351_SI4732_STM32
ECHO OFF

echo "Teensy"
ECHO ON
arduino-cli compile --fqbn teensy:avr:teensy41   ./SI47XX_14_TEENSY/OLED_ALL_IN_ONE --output-dir ~/Downloads/hex/TEENSY/OLED_ALL_IN_ONE
ECHO OFF

ECHO "Arduino DUE and MEGA"
ECHO ON 
arduino-cli compile --fqbn arduino:sam:arduino_due_x .\SI47XX_16_ARDUINO_DUE_MEGA\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 --output-dir %homepath%\Downloads\hex\due\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 
arduino-cli compile --fqbn arduino:avr:mega .\SI47XX_16_ARDUINO_DUE_MEGA\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 --output-dir %homepath%\Downloads\hex\mega\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 
ECHO OFF

ECHO "MiniCore - You can save memory with this approach on ATmega328" 
ECHO ON 
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  .\SI47XX_04_TFT/SI47XX_03_TFT_ATMEGA328_ST7735 --output-dir %homepath%\Downloads\hex\MiniCore\SI47XX_03_TFT_ATMEGA328_ST7735
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  .\SI47XX_04_TFT\SI47XX_01_TFT_ILI9225 --output-dir %homepath%\Downloads\hex\MiniCore\SI47XX_01_TFT_ILI9225
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  .\SI47XX_02_for_Mirko_Pavleski_radio\MIRKO_V2 --output-dir %homepath%\Downloads\hex\MiniCore\MIRKO_V2
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  .\SI47XX_03_OLED_I2C\SI47XX_02_ALL_IN_ONE_OLED --output-dir %homepath%\Downloads\hex\MiniCore\SI47XX_02_ALL_IN_ONE_OLED
arduino-cli compile -b MiniCore:avr:328:bootloader=no_bootloader,LTO=Os_flto  .\SI47XX_09_NOKIA_5110\ALL_IN_ONE_ONE_ENCODER --output-dir %homepath%\Downloads\hex\MiniCore\SI47XX_09_NOKIA_5110
