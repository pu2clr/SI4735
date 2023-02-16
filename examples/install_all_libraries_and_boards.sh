<<TodoBefore

PLEASE, read each comment before runing this script.
This script runs on Mac OS and Linux and installs all libraries and boards available on Arduino platform that is used by the examples.
This may be an option to configure your programming environment for arduino and the PU2CLR SI473X library.

First, run the script ./lib_si4735_basic_install.sh 
It will install the arduino-cli and configure your Arduino enviroment (basic libraries and boards) for SI4735 Arduino Library

ATTENTION... Before running the command below you need to know: 
             You can remove libraries and boards you do not intend to use from the script;
             All board installed will consume about 6GB of storage space;
             It may take several minutes or hours if you want to run the command below withou adapting it for your needs.
                  
run the scripts below

./lib_si4735_basic_install.sh
./install_all_libraries_and_boards.sh

TodoBefore

export PATH=~/bin:$PATH

## Please.. run the script lib_si4735_basic_install.sh before running it.

arduino-cli core update-index
# Install the PU2CLR SI4735 Arduino Library and all other libraries used by the examples
echo "Installing all libraries needed by the PU2CLR Arduino Library examples"
echo "It can take a long time and 6GB of your storage space"
arduino-cli lib install "PU2CLR SI4735"
arduino-cli lib install "Adafruit BusIO"
arduino-cli lib install "Adafruit SH110X"
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Adafruit ST7735 and ST7789 Library"
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
echo "The LCD5110_Graph library needs to be installed manually. See: http://www.rinkydinkelectronics.com/library.php?id=47"
echo "The Adafruit_SH1106 library needs to be installed manually. See: https://github.com/wonho-maker/Adafruit_SH1106"

# Install all boards used by the examples
echo "Installing all boards needed by the PU2CLR SI4735 Arduino examples"
arduino-cli core install arduino:avr
arduino-cli core install lgt8fx:avr
arduino-cli core install MiniCore:avr
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

echo "Finish"
