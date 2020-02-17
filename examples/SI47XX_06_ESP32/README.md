# ESP32 and Si4735 Arduino Library

This folder show some examples with Si4735 Arduino Library  and ESP32 platform.



## Example SI47XX_01_ESP32_AM_FM_TOUCH_SERIAL_MONITOR

This sketch uses just the Serial Monitor as the human interface to test and validation of the SI4735 Arduino Library on ESP platform. It is very useful to test your circuit and setup. 

### The main advantages of using this sketch are: 

* It is a easy way to check if your circuit is working;
* You do not need to connect any display device to make your radio works;
* You do not need connect any push buttons or encoders to change volume and frequency;
* The Arduino IDE is all you need to control the radio.  


## Example SI47XX_02_ESP32_TOUCH_ALL_IN_ONE

This sketch uses the LCD20x4 display with a I2C adapter. 

### Main features:

* This sketch has been successfully tested on ESP LOLIN32 (WEMOS) and ESP32 Devkit;
* It uses the capacitive touch ESP32 resource;
* I2C LiquidCrystal/LCD 20x4;
* Encoder;
* FM, AM (MW and SW) and SSB (LSB and USB);
* Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
* BFO Control; and
* Frequency step switch (1, 5 and 10KHz).


### 