# Examples

[This folder](https://github.com/pu2clr/SI4735/tree/master/examples) and subfolders have more than 30 examples that might help you to use the Si4735 Arduino Library in your project. Please, be guided by the comments inserted in each sketch.

If you find some error or problem during your implementation, please let me know. 

### IMPORTANT

__This project is about a library to control the SI47XX devices and the focus of this project is the "PU2CLR Si4735 Arduino Library" and its functionalities. Please, don't ask the author of this project to assist you with displays, encoders, buttons or something else out of the "PU2CLR SI4735 Arduino Library" scope. Thanks.__

#### Use the following groups to guide you in your projects: 

* __Facebook__ group called [__Si47XX for Radio Experimenters__](https://www.facebook.com/groups/532613604253401/). The purpose is exchanging experiences with projects based on Silicon Labs  SI47XX IC family. 
* __group.io__ [SI47XX for hobbyists](https://groups.io/g/si47xx). This group is formed by people with experience in electronics and firmware development. 
  

__IT IS IMPORTANT TO SAY THAT THE CIRCUITS DESIGN BUILT BY THE AUTHOR ARE NOT INTEND TO BE A FINAL PRODUCT. SOME IMPORTANT ASPECTS AND DETAILS ABOUT A GOOD RECEIVER WERE OMITTED HERE. THE IDEA OF THE CIRCUITS MADE BY THE AUTHOR WERE ORIENTED TO BE A PROOF OF CONCEPT OF THE FUNCTIONS IMPLEMENTED IN THE ARDUINO LIBRARY. SO, COMPONENTS LIKE FRONT-ENDS, BAND PASS FILTER,  ESD, ANTENNA DESIGN ARE NOT THE MAIN PART OF THIS PROJECT__.


Before using this examples, you most install the ["PU2CLR SI3735 Arduino Library"](https://github.com/pu2clr/SI4735#library-installation) on your Arduino IDE. Also be aware about other libraries that are eventually used to control LCD, OLED, TFT display, encoder etc. These libraries are referenced in the sample sketches themselves.

It is important to know tha the author of this library  just try to show how you can use this library and the Si4735 device in your appication. Other devices used in some examples like encoders, buttons and display (OLED, LCD or TFT) are not the focus of this subject. Said that, it is possible some implementation using theses devices might need different approaches used here.

The most of the sketches has been developed to run on cross-plataform (ATmega328, Mega2560, DUE, ATmega-32u4, ESP32, ATtiny85 etc). However, in some cases, it was not possible due to hardware limitations in some board. Each sketch has the set of boards you can run it.  

### It is strongly recommended running these examples with the latest version of the Si4735 Arduino Library. If you are using earlier versions of this library, some examples may not work correctly. Please, check if you are using the last release of this library.

{% include serialmonitor.html %}


#### If you have saved the examples in your personal folder, it is important to know the last versions of these examples come with the each new release of the library. Probably, some examples was also fixed or optimized. So, check if you are using the last version of the example too.


The folder [extra/schematic](https://github.com/pu2clr/SI4735/tree/master/extras/schematic) can guide you on circuits and components settings.  


## About the examples folders

* The folder [SI47XX_01_SERIAL_MONITOR](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR) has basic examples using the Arduino Serial Monitor as the human interface.
By using the examples of this folder, you will do not need any external device to control the Si4735. All you have to do is turning on the Serial Monitor. These examples are usefull to test your circuit board and help you understand the use of the Si4735 Arduino Lirary.
* The folther [SI47XX_02_LCD_20x4_I2C](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_02_LCD_20x4_I2C) has some examples that show how you can build a radio based on Si473X using the regular LCD20x4 with a I2C adapter device.
* The folder [SI47XX_03_OLED_I2C](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_03_OLED_I2C) has some examples that show how you can build a radio based on Si473X using the regular I2C OLED. They are very similar to the LCD20x4 sketches. The main difference is the Arduino Library controller of these devices.  
* The folder [SI47XX_04_TFT](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_04_TFT) has some example with TFT and touch screen as an human interface to control the radio based on Si473X.
* The folder [SI47XX_05_ATTINY85](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_05_ATTINY85) has examples of using the Si4735 Arduino Library running on ATtiny85.
* The folder [SI47XX_06_ESP32](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_06_ESP32) has examples that use some great resources of the ESP32 boards. It is worth mentioning that the most of the other examples can also run on ESP32.
* The folder [SI47XX_07_STM32](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_07_STM32) has examples of using the STM32F103 Series.
* The folder [SI47XX_08_TM1638](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_08_TM1638) has examples of using the TM1638 device (7 segments display, push buttons and LEDs).
* The folder [SI47XX_09_NOKIA_5110](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_09_NOKIA_5110) has examples of using the NOKIA display.
* The folder [SI47XX_10_RDS](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_10_RDS) has examples of using FM RDS/RBDS features of the Si4735 Arduino Library.


<BR>

## Main sketch examples

| sketch name | description |
| ----------- | ----------- |
| [SI4735_01_POC.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_01_SERIAL_MONITOR/SI4735_01_POC/SI4735_01_POC.ino) | Circuit test. Receiver AM (MW and SW) and FM. This sketch uses the Arduino IDE Serial Monitor  |
| [SI4735_03_POC_SSB.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_01_SERIAL_MONITOR/SI4735_03_POC_SSB/SI4735_03_POC_SSB.ino) | This receiver implements shows how to use patch and SSB via Arduino IDE Serial Monitor  |
| [SI473X_04_ALL_IN_ONE.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_02_LCD_20x4_I2C/SI473X_04_ALL_IN_ONE/SI473X_04_ALL_IN_ONE.ino) | This sketch uses I2C LiquidCrystal/LCD, buttons and  Encoder. It is a receiver FM, AM (MW and SW) and SSB (LSB and USB) |
| [SI4735_02_ALL_IN_ONE_OLED.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_03_OLED_I2C/SI4735_02_ALL_IN_ONE_OLED/SI4735_02_ALL_IN_ONE_OLED.ino)|  This sketch uses I2C OLED/I2C, buttons and  Encoder. It is a FM, AM (MW and SW) and SSB (LSB and USB) |
| [SI47XX_01_TFT_ILI9225/SI47XX_01_TFT_ILI9225.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_04_TFT/SI47XX_01_TFT_ILI9225/SI47XX_01_TFT_ILI9225.ino) | This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT from MICROYUM (2" - 176 x 220). It is also a complete radio capable to tune LW, MW, SW on AM and SSB mode and also receive the regular comercial stations.|
| [SI47XX_02_TFT_TOUCH_SHIELD/SI47XX_02_TFT_TOUCH_SHIELD.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_04_TFT/SI47XX_02_TFT_TOUCH_SHIELD/SI47XX_02_TFT_TOUCH_SHIELD.ino)  | This sketch uses the mcufriend TFT touch Display Shield.   You can use it on Mega2560 and Arduino DUE. It is a receiver FM, AM (MW and SW) and SSB (LSB and USB) | [SI47XX_01_ATTINY85_LCD_16x2_I2C.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_05_ATTINY85/SI47XX_01_ATTINY85_LCD_16x2_I2C/SI47XX_01_ATTINY85_LCD_16x2_I2C.ino) | Test and validation of the SI4735 Arduino Library on ATtiny85. This sketch uses a LCD 16x2 and buttons |
| [SI47XX_02_RDS_TFT_TOUCH_SHIELD/SI47XX_02_RDS_TFT_TOUCH_SHIELD.ino](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD/SI47XX_02_RDS_TFT_TOUCH_SHIELD.ino) | This sketch uses the mcufriend TFT touch Display Shield.  You can use it on Mega2560 or DUE. It is a FM/RDS, AM (LW,MW and SW) and SSB example |
|[SI47XX_11_NO_CRYSTAL](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_11_NO_CRYSTAL) | The examples found on this folder will show you how to configure the SI473X device with external oscillators (TCXO, Active Crystal, configurable clock generators like SI5351 etc). It can be useful to improve the stability of the system by using more precise oscillators instead of regular passive crystals. |
| [SI47XX_10_RDS/SI47XX_03_RDS_TFT_ILI9225](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_10_RDS/SI47XX_03_RDS_TFT_ILI9225) | FM/RDS AM and SSB receiver. This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT from MICROYUM (2" - 176 x 220). It is also a complete radio capable to tune LW, MW, SW on AM and SSB mode and also receive the  regular comercial stations |
| [SI47XX_KITS](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_KITS)| This folder has implementations based on this library that are running in some famous KIT based on SI4735-D60 and SI4732-A10 |
| [SI473X_12_MORSE_CODE_READOUT](https://github.com/pu2clr/SI4735/tree/master/examples/SI473X_12_MORSE_CODE_READOUT) | This example demonstrates a way to report the current status of the receiver via Morse Code. |
| [iOS and Android Remote Control](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_10_MOBILE_PHONE) | This example shows a way to use your smartphone as a remote control. |


[Back to the main page](https://pu2clr.github.io/SI4735/)
