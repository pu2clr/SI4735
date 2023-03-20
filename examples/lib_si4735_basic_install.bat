:: This script runs on Linux and MacOS and it is useful to install the basic enviroment to develop
:: applications (receivers) using the PU2CLR Arduino Library.
::
:: Please, check the comments above
::
echo off
:: go to homefolder
cd %homepath%
:: Download and install the arduino command line - arduino-cli
echo on
echo "Installing arduino-cli.exe to your home folder (arduino.cli.exe) 
echo off  
curl -fsSL https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip --output arduinocli.zip
tar -xf arduinocli.zip 
:: It should be installed in the ~/bin folder
:: Start the arduino-cli configuration
echo on 
echo "Configuring Arduino boards and libraries. It can take a long time...." 
arduino-cli config init 
:: Add all boards used by the examples (ATmega, Attiny, ESP32, STM32 etc) to the board list
arduino-cli config set board_manager.additional_urls http://arduino.esp8266.com/stable/package_esp8266com_index.json http://dan.drown.org/stm32duino/package_STM32duino_index.json  http://drazzy.com/package_drazzy.com_index.json https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json https://mcudude.github.io/MegaCore/package_MCUdude_MegaCore_index.json https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json https://raw.githubusercontent.com/DavidGuo-CS/OSOYOO_Arduino/main/package_osoyoo_boards_index.json https://raw.githubusercontent.com/VSChina/azureiotdevkit_tools/master/package_azureboard_index.json  https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json https://raw.githubusercontent.com/dbuezas/lgt8fx/master/package_lgt8fx_index.json  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json https://raw.githubusercontent.com/nulllaborg/arduino_nulllab/master/package_nulllab_boards_index.json https://www.pjrc.com/teensy/package_teensy_index.json

:: Update the index of boards that can be installed
arduino-cli core update-index

echo "This may take several minutes or hours. Please wait!"
echo "Installing the libraries"
:: uncomment the lines if you and to include more libraries 
arduino-cli lib install "PU2CLR SI4735"
arduino-cli lib install "Adafruit BusIO"
arduino-cli lib install "Adafruit SH110X"
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Adafruit ST7735 and ST7789 Library"
arduino-cli lib install "Adafruit PCD8544 Nokia 5110 LCD library"
:: arduino-cli lib install "Adafruit TouchScreen"
:: arduino-cli lib install "ES32Lab"
:: arduino-cli lib install "Etherkit Si5351"
:: arduino-cli lib install "FlashStorage_SAMD"
arduino-cli lib install "LiquidCrystal"
arduino-cli lib install "LiquidCrystal I2C"
arduino-cli lib install "MCUFRIEND_kbv"
arduino-cli lib install "TFT_22_ILI9225"
::arduino-cli lib install "TFT_eSPI_ES32Lab"
arduino-cli lib install "Tiny4kOLED"
arduino-cli lib install "TinyOLED-Fonts"
echo "----------------------------------------------------"
echo "Installing the library Adafruit_SH1106 via zip file"
SET ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
curl -fsSL https://github.com/wonho-maker/Adafruit_SH1106/archive/refs/heads/master.zip --output SU1106.zip
arduino-cli lib install --zip-path ./SU1106.zip
echo "----------------------------------------------------"
echo "The site of library LCD5110_Graph does not allow deep linking."
echo "So, if you need to run the example with Nokia 5110 display, please, install library LCD5110_Graph library manually." 
echo "See: http://www.rinkydinkelectronics.com/library.php?id=47"
echo "Download the file and run arduino-cli lib install --zip-path ~Downloads/LCD5110_Graph.zip"
echo "----------------------------------------------------"

:: uncomment the lines below if you want some additional boards 
echo "Installing the boards"
arduino-cli core install arduino:avr
arduino-cli core install lgt8fx:avr
arduino-cli core install MiniCore:avr
arduino-cli core install arduino:sam
:: arduino-cli core install esp32:esp32
:: arduino-cli core install esp8266:esp8266
:: arduino-cli core install stm32duino:STM32F1
:: arduino-cli core install stm32duino:STM32F4
:: arduino-cli core install STMicroelectronics:stm32
:: arduino-cli core install STM32:stm32
:: arduino-cli core install rp2040:rp2040
:: arduino-cli core install Seeeduino:samd
:: arduino-cli core install ATTinyCore:avr
:: arduino-cli core install MegaCore:avr
:: arduino-cli core install MightyCore:avr
:: arduino-cli core install teensy:avr

echo "Finish"
