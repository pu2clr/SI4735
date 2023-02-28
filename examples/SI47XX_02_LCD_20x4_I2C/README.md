# Receivers based on Si4735 working with LCD20x4 / I2C 

This folder has examples of radios based on Si4735 working with LCD20x4 and I2C adapter device. 


## Si47XX 01 - SI47XX_01_LCD_20x4_I2C 

This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. Also, will download a SSB patch to your SI4735 device. It will take about 9KB from Arduino Flash Memory.  

<BR>

__This sketch has been successfully tested on:__

* Pro Mini 3.3V; 
* UNO (by using a voltage converter); 
* Arduino Yún (by using a voltage converter); 
* Arduino Micro (see the operating voltage of your Micro); 
* Arduino Mega (by using a voltage converter); and 
* Arduino DUE;

Features of this sketch: 

1) __Internal Arduino pull-up (no external resistors on schematic for buttons and encoder)__;
2) Only SSB (LSB and USB);
3) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4kHz;
4) Volume control;
5) Eight ham radio bands pre configured;
6) BFO Control; 
7) Frequency step switch (1, 5 and 10kHz);  and
8) BFO step (10 and 50 Hz)
   

Follow the schematic below to use this sketch. 

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/schematic_basic_oled_SI47XX_02_ALL_IN_ONE_OLED.png)

#### __ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:__
This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit. If this occurs in your project, change the circuit to use external pull-up on pin 13. 


__See some shortwave antenna configuration on__  [Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383](https://www.silabs.com/documents/public/application-notes/AN383.pdf)

<BR>

Pay attention on the push buttons added on this example. The table below shows the commands used by this sketch.

| Defined constant | Arduino Pin | Description |
| ---------------- | ----------- | ----------- | 
| BANDWIDTH_BUTTON | 5           | Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 kHz |
| VOL_UP           | 6           | Volume Up |
| VOL_DOWN 7       | 7           | Volume Down |
| BAND_BUTTON_UP   | 8           | Next band |
| BAND_BUTTON_DOWN | 9           | Previous band | 
| AGC_SWITCH       | 11          | Switch AGC ON/OF | 
| STEP_SWITCH      | 10          | Used to select the increment or decrement frequency step (1, 5 or 10 kHz) |
| BFO_SWITCH       | 13          | Used to select the enconder control (BFO or VFO) |

<BR>

## Si47XX 02 - AM/FM radio with OLCD and OLED as a S-Meter

This example uses two display I2C devices and the Si4735 connected on the same I2C bus. 
It is an simple AM and FM radio. 

__This sketch has been successfully tested on Pro Mini 3.3V.__ 

<BR>

![Protoboard with OLED as a S Meter](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_AM_FM_SMETER.png)

<BR>

## SI47XX_03_SW_LCD_SMETER_I2C - SW Receiver with LCD and OLED as a S-Meter 

Like the previous example , this example uses two display and the SI4735 connect on the same I2C bus.
This radio has 11 bands.

__This sketch has been successfully tested on Pro Mini 3.3V.__ 

<BR>

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_internal_pullup_i2c.png)


<BR>
__The picture below shows the SI4735-D60 pinout__

![Si4735-D60 pinout](https://github.com/pu2clr/SI4735/blob/master/extras/images/Si4735-D60-pinout.png)


#### __ABOUT DIGITAL pin 13 and INPUT PULL-UP on Arduino Pro Mini, UNO or similar:__
This pin has a LED and a resistor connected on the board. When this pin is set to HIGH the LED comes on. If you use the internal pull-up resistor of the pin 13, you might experiment problem due to the drop voltage caused by the LED circuit. If this occurs in your project, change the circuit to use external pull-up on pin 13. 


__See some shortwave antenna configuration on__  [Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383](https://www.silabs.com/documents/public/application-notes/AN383.pdf)

<BR>

![Protoboard with OLED as a S Meter](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_SW_SMETER.png)

<BR>

## SI473X_04_ALL_IN_ONE - All in One radio 

This sketch implements an FM/LW/MW/SW - SSB radio.

This sketch uses I2C LiquidCrystal/LCD, buttons and  Encoder.
  
This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C). Look for LiquidCrystal I2C on Manager Libraries.

This sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 8KB of the Arduino memory.

In this context, a patch is a piece of software used to change the behavior of the SI4735 device. There is little information available about patching the SI4735. The following information is the understanding of the author of  this project and it is not necessarily correct. A patch is executed internally (run by nternal MCU) of the device.  Usually, patches are used to fixes bugs or add improvements and new features of the firmware  installed in the internal ROM of the device.  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system. Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment. Given this, it is at your own risk to continue with the procedures suggested here.
This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.


### Features of this sketch:

* FM, AM (MW and SW) and SSB (LSB and USB);
* Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4kHz;
* 22 commercial and ham radio bands pre configured;
* BFO Control; and
* Frequency step switch (1, 5 and 10kHz);

<BR>

This sketch has been successfully tested on:

* Pro Mini 3.3V; 
* UNO (by using a voltage converter); 
* Arduino Yún (by using a voltage converter); 
* Arduino Micro (see the operating voltage of your Micro); 
* Arduino Mega (by using a voltage converter); 
* Arduino DUE;
* ESP32 Dev Kit; and
* ESP LOLIN32 WEMOS


### Schematic

![Schematic All In One](https://github.com/pu2clr/SI4735/blob/master/extras/images/schematic_basic_with_buttons_i2c.png)




