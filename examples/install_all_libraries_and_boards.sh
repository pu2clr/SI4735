# This script install all libraries available on Arduino platform that is used by the examples.
# This may be an option to configure your programming environment for arduino.
# Install the arduino-cli before running this script. 
# You do not need to install all libraries below. Consider to install only the libraries used by 
# the example you intend to run. 

arduino-cli lib install "PU2CLR SI4735"
arduino-cli lib install "Adafruit BusIO"
arduino-cli lib install "Adafruit SH110X"
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Adafruit TouchScreen"
arduino-cli lib install "ES32Lab"
arduino-cli lib install "Etherkit Si5351"
arduino-cli lib install "FlashStorage_SAMD"
arduino-cli lib install "LiquidCrystal"
arduino-cli lib install "LiquidCrystal I2C"
arduino-cli lib install "MCUFRIEND_kbv"
arduino-cli lib install "TFT_22_ILI9225"
arduino-cli lib install "TFT_eSPI_ES32Lab"
arduino-cli lib install "Tiny4kOLED"
arduino-cli lib install "TinyOLED-Fonts"
# Nokia 5110 - The library LCD5110_Graph has to be installed manually. See: http://www.RinkyDinkElectronics.com/
# The "Adafruit_SH1106" has to be installed manually. See: https://github.com/wonho-maker/Adafruit_SH1106

# Istalling all boards used by the examnples
# Replace the section below of your arduino-cli.yaml
<< 'Todo'
board_manager:
  additional_urls:
  - http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - http://bigbits.oss-cn-qingdao.aliyuncs.com/Arduino_for_GD32V/package_longduino_index.json
  - http://dan.drown.org/stm32duino/package_STM32duino_index.json
  - http://drazzy.com/package_drazzy.com_index.json
  - https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
  - https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
  - https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
  - https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
  - https://mcudude.github.io/MegaCore/package_MCUdude_MegaCore_index.json
  - https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json
  - https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
  - https://raw.githubusercontent.com/DavidGuo-CS/OSOYOO_Arduino/main/package_osoyoo_boards_index.json
  - https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/master/package_azureboard_index.json
  - https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
  - https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json
  - https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  - https://raw.githubusercontent.com/nulllaborg/arduino_nulllab/master/package_nulllab_boards_index.json
Todo

arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli core install lgt8fx:avr
arduino-cli core install MiniCore:avr:328
arduino-cli core install arduino:sam
arduino-cli core install esp32:esp32
arduino-cli core install esp8266:esp8266
arduino-cli core install stm32duino:STM32F1
arduino-cli core install stm32duino:STM32F4
arduino-cli core install STM32:stm32
arduino-cli core install rp2040:rp2040
arduino-cli core install Seeeduino:samd
arduino-cli core install ATTinyCore:avr
arduino-cli core install MegaCore:avr
arduino-cli core install MightyCore:avr





