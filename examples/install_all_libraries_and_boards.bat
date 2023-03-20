ECHO OFF
:: PLEASE, read each comment before runing this script.
:: This script runs on Windows and installs all libraries and boards available on Arduino platform that is used by the examples.
:: This may be an option to configure your programming environment for arduino and the PU2CLR SI473X library.
::
:: First, install the arduino-cli before running this script.
::
:: Download the install file: https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.msi
:: Download the exe (binary)file: https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip
::
:: After install run:
::
:: arduino-cli config init
:: 
:: Check if the folder <User>\AppData\Local\Arduino15\  was created in your user account.
:: Check if the file <User>\AppData\Local\Arduino15\arduino-cli.yaml was created
:: Edit  the section below of your <User>\AppData\Local\Arduino15\arduino-cli.yaml and replace the section additional_urls as shown below
::
::
:: Finally
::
:: ATTENTION... Before running the command below you need to know: 
::              You can remove libraries and boards you do not intend to use from the script (install_all_libraries_and_boards.bat);
::              All board installed will consume about 6GB of storage space;
::              It may take several minutes or hours if you want to run the command below withou adapting it for your needs.
::
:: run the scripts below
:: .\lib_si4735_basic_install.bat
:: .\install_all_libraries_and_boards.bat

ECHO ON

echo "This may take several minutes or hours. Please wait!"
echo "All board installed will consume about 6GB of storage space."

echo "Installing the libraries"
arduino-cli core update-index
arduino-cli lib install "PU2CLR SI4735"
arduino-cli lib install "Adafruit BusIO"
arduino-cli lib install "Adafruit SH110X"
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Adafruit ST7735 and ST7789 Library"
arduino-cli lib install "Adafruit PCD8544 Nokia 5110 LCD library"
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
arduino-cli lib install "Adafruit PCD8544 Nokia 5110 LCD library"

echo "Installing the boards"
arduino-cli core install arduino:avr
arduino-cli core install lgt8fx:avr
arduino-cli core install MiniCore:avr
arduino-cli core install arduino:sam
arduino-cli core install esp32:esp32
arduino-cli core install esp8266:esp8266
arduino-cli core install stm32duino:STM32F1
arduino-cli core install stm32duino:STM32F4
arduino-cli core install STM32:stm32
arduino-cli core install STMicroelectronics:stm32
arduino-cli core install rp2040:rp2040
arduino-cli core install Seeeduino:samd
arduino-cli core install ATTinyCore:avr
arduino-cli core install MegaCore:avr
arduino-cli core install MightyCore:avr
arduino-cli core install teensy:avr

echo "Finish"
