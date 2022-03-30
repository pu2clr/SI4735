# This script uses the arduino-cli to compile the arduino sketches using command line (without Arduino IDE).
# It is very useful to check the library after bug fixes and improvments. 

# compiles POC
echo "POC"
arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_01_SERIAL_MONITOR/SI4735_01_POC

arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_01_SERIAL_MONITOR/SI4735_02_POC_SW

arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_01_SERIAL_MONITOR/SI4735_03_POC_SSB

arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_01_SERIAL_MONITOR/Si4735_04_HARDWARE_MUTE_CIRCUIT

# compiles Mirko V2
echo "Mirko V2"
arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_02_for_Mirko_Pavleski_radio/MIRKO_V2

# compiles KIT ATS-20
echo "ATS-20 KIT"
arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_KITS/AliExpress/SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V7

# compiles ESP32 LCD16x2_ALL_IN_ONE
echo "ESP32 LCD16x2_ALL_IN_ONE"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_06_ESP32/LCD16x2_ALL_IN_ONE 

# compiles Gert's KIT
echo "Gert's KIT (ESP32)"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_KITS/THIAGO_LIMA/GERT_BAAK/SI4735_2.8_TFT_SI5351_V4.2

# compiles 
echo "FELIX ANGGA"
arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_KITS/THIAGO_LIMA/FELIX_ANGGA/SLAMETRADIO

# compiles SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2 on Arduino DUE and Mega 
echo "Arduino RDS DUE and MEGA"
echo "*** DUE"
arduino-cli compile --fqbn arduino:sam:arduino_due_x /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
echo "*** Mega"
arduino-cli compile --fqbn arduino:avr:mega /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2

# compiles SI47XX_03_RDS_TFT_ILI9225_NEW on Arduino Nano
echo "Arduino Nano - RDS TFT"
arduino-cli compile -b arduino:avr:nano /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_10_RDS/SI47XX_03_RDS_TFT_ILI9225_NEW

# compiles STM32 sketch
echo "STM32"
arduino-cli compile --fqbn STM32:stm32:Nucleo_64 /Users/rcaratti/Desenvolvimento/eu/Arduino/DSP/si4735/SI4735/examples/SI47XX_07_STM32/STM32_04_OLED_ALL_IN_ONE_V2


