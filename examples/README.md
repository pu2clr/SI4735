# Examples

This folder has a set of examples that might help you to use the Si4735 Arduino Library in your project. If you find some error or problem during your implementation, please let me know. 

It is important to know tha the author of this library  just try to show how you can use this library and the Si4735 device in your appication. Other devices used in some examples like encoders, buttons and display (OLED, LCD or TFT) are not the focus of this subject. Saing  that, it is possible some implementation using theses devices might need different approaches than used here. 

The most of the example sketches has been developed to run on cross-plataform (ATmega328, Mega2560, DUE, ATmega-32u4, ESP32, ATtiny85 etc). However, in some cases, it was not possible due to hardware limitations in some platform. Each sketch has the set of boards you can run it.  

## About the examples folders

* The folder [SI47XX_01_SERIAL_MONITOR](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR) has basic examples using the Arduino Serial Monitor as the human interface. 
By using the examples of this folder, you will do not need any external device to control the Si4735. All you have to do is turning on the Serial Monitor. These examples are usefull to test your circuit board and help you understand the use of the Si4735 Arduino Lirary. 
* The folther [SI47XX_02_LCD_20x4_I2C]() has some examples that show how you can build a radio based on Si473X using the regular LCD20x4 with a I2C adapter device. 
* The folder [SI47XX_03_OLED_I2C]() has some examples that show how you can build a radio based on Si473X using the regular I2C OLED. They are very similar to the LCD20x4 sketches. The main difference is the Arduino Library controller of these devices.  
* The folder [SI47XX_04_TFT]() has some example with TFT and touch screen as an human interface to control the radio based on Si473X.
* The folder [SI47XX_05_ATTINY85]() has examples of using the Si4735 Arduino Library running on ATtiny85. 
* The folder [SI47XX_06_ESP32]() has examples that use some great resources of ESP32 boards. It is worth mentioning that the most of the other examples can also run on ESP32. 













