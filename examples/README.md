# Examples

This folder has some examples that might help you to use the Si4735 Arduino Library in your project. If you find some error or problem during your implementation, please let me know. 

## Summary

1. [SS4735 Arduino Library example with LCD 20x4 I2C](examples#SI4735_LCD_20x4_I2C)
2. [SS4735 Arduino Library example with I2C OLED](examples#SI4735_OLED_I2C)
3. [Schematic for I2C display device, buttons and encoder](examples#schematic-for-i2c-display-device-buttons-and-encoder)
4. [Prof of Concept](examples#proof-of-concept)
5. [Proof of Concept with SW](examples#proof-of-concept-with-sw)


## SS4735 Arduino Library example with LCD 20x4 I2C

This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the LiquidCrystal I2C Library by Frank de Brabander. You can install LiquidCrystal I2C via Manage Libraries on your Arduino IDE. 

## SS4735 Arduino Library example with I2C OLED

This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the SSD1306Ascii Arduino Library by Bill Greiman. You can install SSD1306Ascii via Manage Libraries on your Arduino IDE. 


## Schematic for I2C display device, buttons and encoder

The schematic below is a sugestion to connect a I2C Display (OLED or LCD 20x4 or another device), buntons and encoder as well. 

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_i2c.png)

__Pay attention to the your I2C device voltage. The system can become unstable if you are using a wrong voltage__.

## Proof of Concept

The main advantage of using this Proof of Concept is: you do not need to connect any display device to make your radio works. The Arduino IDE is all you need to control the radio.  So, I strongly recommend starting with this sketch. This example is a proof of concept of SI4735 Arduino Library. The [SI4735_POC.ino](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_POC) sketch uses just the __Serial Monitor__ and works only on your __Arduino IDE__. However, you can easily replace the Serial Monitor functions to the device functions that you want to use. The [SS4735 Arduino Library example with LCD 20x4 I2C](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_LCD_20x4_I2C) and [SS4735 Arduino Library example with I2C OLED](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_OLED_I2C) are good examples that show this approach.



## Proof of Concept with SW

This example shows how can you use SW features of the Si4735 Arduino Library. Like the previous exemple (Proof of Concept), the [SI4735_POC_SW.ino](examples/SI4735_POC_SW/SI4735_POC_SW.ino) sketch __works only on Arduino IDE__. It uses the Serial Monitor to get the commands and show the information. 



## Schematic for Proof of Concept

The schematic below is the same shown at the main page of this project and shows how you can setup you SI4735 device.

![Silicon Labs Schematic](../extras/images/basic_schematic.png)





