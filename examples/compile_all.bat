
Rem Teste

ECHO "ATmega and LGTF8X based boards"
arduino-cli compile -b arduino:avr:nano .\SI47XX_01_SERIAL_MONITOR\SI4735_01_POC --output-dir %homepath%/Downloads/hex/atmega/SI4735_01_POC
arduino-cli compile -b lgt8fx:avr:328 .\SI47XX_01_SERIAL_MONITOR\SI4735_01_POC --output-dir %homepath%/Downloads/hex/lgt8fx/SI4735_01_POC
arduino-cli compile -b arduino:avr:nano .\SI47XX_02_for_Mirko_Pavleski_radio\MIRKO_V2 --output-dir %homepath%/Downloads/hex/atmega/MIRKO_V2
arduino-cli compile -b lgt8fx:avr:328   .\SI47XX_02_for_Mirko_Pavleski_radio\MIRKO_V2 --output-dir %homepath%/Downloads/hex/lgt8fx/MIRKO_V2
arduino-cli compile -b arduino:avr:nano .\SI47XX_03_OLED_I2C\SI47XX_02_ALL_IN_ONE_OLED --output-dir %homepath%/Downloads/hex/atmega/SI47XX_02_ALL_IN_ONE_OLED
arduino-cli compile -b arduino:avr:pro .\SI47XX_09_NOKIA_5110\ALL_IN_ONE_ONE_ENCODER --output-dir %homepath%/Downloads/hex/atmega/SI47XX_09_NOKIA_5110
arduino-cli compile -b lgt8fx:avr:328  .\SI47XX_09_NOKIA_5110\ALL_IN_ONE_ONE_ENCODER --output-dir %homepath%/Downloads/hex/lgt8fx/SI47XX_09_NOKIA_5110
arduino-cli compile -b arduino:avr:nano .\SI47XX_04_TFT\SI47XX_01_TFT_ILI9225 --output-dir %homepath%/Downloads/hex/atmega/SI47XX_01_TFT_ILI9225


ECHO "ATmega 32 and 128 based boards"
arduino-cli compile -b MightyCore:avr:32:bootloader=no_bootloader,LTO=Os_flto,clock=16MHz_external  .\SI47XX_17_ATMEGA32\OLED_ALL_IN_ONE --output-dir %homepath%/Downloads/hex/atmega32/OLED_ALL_IN_ONE
arduino-cli compile -b MegaCore:avr:128:bootloader=no_bootloader,LTO=Os_flto .\SI47XX_17_ATMEGA128\OLED_ALL_IN_ONE --output-dir %homepath%/Downloads/hex/atmega128/OLED_ALL_IN_ONE 

ECHO "ATtiny Core"
arduino-cli compile --fqbn ATTinyCore:avr:attinyx5  .\SI47XX_05_ATTINY85\SI47XX_02_ATTINY85_MINI_OLED_I2C  %homepath%/Downloads/hex/attiny/SI47XX_02_ATTINY85_MINI_OLED_I2C 



arduino-cli compile --fqbn esp8266:esp8266:generic  .\SI47XX_06_ESP8266\OLED_ALL_IN_ONE 
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso .\SI47XX_06_ESP32\LCD16x2_ALL_IN_ONE

arduino-cli compile --fqbn Seeeduino:samd:seeed_XIAO_m0  .\SI47XX_15_SEEEDUINO\OLED_ALL_IN_ONE

arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C .\SI47XX_07_STM32\STM32_04_OLED_ALL_IN_ONE_V2

arduino-cli compile --fqbn arduino:sam:arduino_due_x .\SI47XX_16_ARDUINO_DUE_MEGA\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
arduino-cli compile --fqbn arduino:avr:mega .\SI47XX_16_ARDUINO_DUE_MEGA\SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 