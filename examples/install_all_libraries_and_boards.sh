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
# copy/overwrite the file arduino-cli.yaml from the current folder to you .arduinoIDE folder 
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





