# Examples

This folder contains several other examples that use encoders, buttons, OLED and LCD. This folder also has some sketches used to make tests on the IC Si4735.

## Summary for other examples

1. [Si47XX 01 - Arduino Library example with I2C OLED](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-01---arduino-library-example-with-i2c-oled)
2. [Si47XX 02 - Arduino Library example with LCD 20x4 I2C](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-02---arduino-library-example-with-lcd-20x4-i2c)
3. [Si47XX 03 - Bandwidth filter test (11 band SW receiver with OLED)](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-03---bandwidth-filter-test-11-band-sw-receiver-with-oled)
4. [Si47XX 04 - SSB firmware update support with OLED/I2C)](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-04---ssb-firmware-update-support-with-oledi2c)
5. [Si47XX 05 - SSB firmware update support with LCD/I2C](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-05---ssb-firmware-update-support-with-lcdi2c)
6. [Si47XX 06 - AM/FM radio with OLCD and OLED as a S-Meter](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-06---amfm-radio-with-olcd-and-oled-as-a-s-meter)
7. [Si47XX 07 - SW Receiver with LCD and OLED as a S-Meter](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-07---sw-receiver-with-lcd-and-oled-as-a-s-meter)
8. [SI47XX 08 - AM/FM receiver with ATtiny85](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES#si47xx-08---amfm-receiver-with-attiny85) 


<BR>

## Si47XX 01 - Arduino Library example with I2C OLED

This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the SSD1306Ascii Arduino Library by Bill Greiman. You can install SSD1306Ascii via Manage Libraries on your Arduino IDE. The photo below show this example on a protoboard. 

__This sketch has been successfully tested on Pro Mini 3.3V.__ 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_04_OLED_I2C/SI4735_04_OLED_I2C.ino) to see this example.

![Silicon Labs Schematic](../extras/images/prot_oled_01.png)


__Click [here](https://youtu.be/7Sg4z8tDSA8) to see a video about this example__.


<BR>

## Schematic for I2C display device, buttons and encoder (used on examples "Si47XX 01" and "Si47XX 02")

The schematic below is a sugestion to connect a I2C Display (OLED or LCD 20x4 or another device), buntons and encoder as well. 

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_i2c.png)

__Pay attention to the your I2C device voltage. The system can become unstable if you are using a wrong voltage__.

<BR>


## Si47XX 02 - Arduino Library example with LCD 20x4 I2C

It is a example that shows how to set up a LCD and SI4735 on the same I2C bus. 
This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the LiquidCrystal I2C Library by Frank de Brabander. You can install LiquidCrystal I2C via Manage Libraries on your Arduino IDE. 


__Click [here](https://youtu.be/vzunuxam_Lg) to see a video about this example__.

<BR>


## Si47XX 03 - Bandwidth filter test (11 band SW Receiver with OLED)

This example is a 11 band SW receiver based on SI4735.  It shows the selection of the bandwidth of the channel filter for AM reception (in this case SW). The choices are: 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). With this resource, you can separate  two stations in adjacent channels. __You can use the same schematic shown for the examples 03 and 04__. 

__This sketch has been successfully tested on Pro Mini 3.3V.__ 


__Click [here](https://youtu.be/dN1s3RoXGos) to see a video about this example__.

<BR>


## Si47XX 04 - SSB firmware update support with OLED/I2C

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. 

  __This sketch has been successfully tested on Pro Mini 3.3V.__ 

  This sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 9KB from Arduino Flash Memory.   
    
  In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
  There is little information available about patching the SI4735. The following information is the understanding of the author of 
  this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device. 
  Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device. 
  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by 
  the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
  Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

  ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment. 
  Given this, it is at your own risk to continue with the procedures suggested here. 
  This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60. 
  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.  

  Features of this sketch: 

  1) Internal Arduino pull-up (no external resistors on schematic for buttons and encoder);
  2) Only SSB (LSB and USB);
  3) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
  4) Eight ham radio bands pre configured;
  5) BFO Control; and
  6) Frequency step switch (1, 5 and 10KHz);

  Main Parts: 
  Encoder with push button; 
  Seven bush buttons;
  OLED Display with I2C protocol;
  Arduino Pro mini 3.3V;  


Pay attention on the push buttons added on this example. The table below shows the commands used by this sketch.

| Defined constant | Arduino Pin | Description |
| ---------------- | ----------- | ----------- | 
| BANDWIDTH_BUTTON | 5           | Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz |
| VOL_UP           | 6           | Volume Up |
| VOL_DOWN 7       | 7           | Volume Down |
| BAND_BUTTON_UP   | 8           | Next band |
| BAND_BUTTON_DOWN | 9           | Previous band | 
| AGC_SWITCH       | 11          | Switch AGC ON/OF | 
| STEP_SWITCH      | 10          | Used to select the increment or decrement frequency step (1, 5 or 10 KHz) |
| BFO_SWITCH       | 13          | Used to select the enconder control (BFO or VFO) |


__Click [here](https://youtu.be/W2Ssjb9P_f4) to see a video about this example__.

<BR>

## Si47XX 05 - SSB firmware update support with LCD/I2C

Is sketch is similar to the Si47XX 04 - SSB firmware update support with OLED/I2C. 

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
3) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
4) Volume control;
5) Eight ham radio bands pre configured;
6) BFO Control; 
7) Frequency step switch (1, 5 and 10KHz);  and
8) BFO step (10 and 50 Hz)
   

Follow the schematic below to use this sketch. 

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_internal_pullup_i2c.png)

__See some shortwave antenna configuration on__  [Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383](https://www.silabs.com/documents/public/application-notes/AN383.pdf)

<BR>

Pay attention on the push buttons added on this example. The table below shows the commands used by this sketch.

| Defined constant | Arduino Pin | Description |
| ---------------- | ----------- | ----------- | 
| BANDWIDTH_BUTTON | 5           | Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz |
| VOL_UP           | 6           | Volume Up |
| VOL_DOWN 7       | 7           | Volume Down |
| BAND_BUTTON_UP   | 8           | Next band |
| BAND_BUTTON_DOWN | 9           | Previous band | 
| AGC_SWITCH       | 11          | Switch AGC ON/OF | 
| STEP_SWITCH      | 10          | Used to select the increment or decrement frequency step (1, 5 or 10 KHz) |
| BFO_SWITCH       | 13          | Used to select the enconder control (BFO or VFO) |


## Si47XX 06 - AM/FM radio with OLCD and OLED as a S-Meter

This example uses two display I2C devices and the Si4735 connected on the same I2C bus. 
It is an simple AM and FM radio. The schematic is the same used on Example 10. See below.

__This sketch has been successfully tested on Pro Mini 3.3V.__ 

<BR>

![Protoboard with OLED as a S Meter](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_AM_FM_SMETER.png)

<BR>

## Si47XX 07 - SW Receiver with LCD and OLED as a S-Meter 

Like the previous example , this example uses two display and the SI4735 connect on the same I2C bus.
This radio has 11 bands.

__This sketch has been successfully tested on Pro Mini 3.3V.__ 

<BR>

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_internal_pullup_i2c.png)

__See some shortwave antenna configuration on__  [Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383](https://www.silabs.com/documents/public/application-notes/AN383.pdf)

<BR>

![Protoboard with OLED as a S Meter](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_SW_SMETER.png)

<BR>

## SI47XX 08 - AM/FM receiver with ATtiny85  


This example implements an AM and FM receiver using the ATtiny85 as the master MCU.

The Attiny core board/plataform can be installed using the Arduino IDE boards manager. Insert the URL http://drazzy.com/package_drazzy.com_index.json on board manager.

On menu Preferences, enter the above URL in "Additional Boards Manager URLs;
To setup ATtiny85 on Arduino IDE, go to Tools Menu, Board, Board Manager and install "ATTinyCore by Spence Konde". See also the video [How to "SI4735 Arduino Library and ATtiny85"](https://youtu.be/zb9TZtYVu-s).

See [ATTiny Core - 1634, x313, x4, x41, x5, x61, x7, x8 and 828 for Arduino](https://github.com/SpenceKonde/ATTinyCore).

See also [ATtiny85 pinout](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf).


### ATtiny85 Schematic


The pull-up resistor values may vary depending on the devices connected to the I2C bus. Select the lowest possible resistors. The schematic below show 4.7K for pull-up resistors. However, it can be lower than it.


![Schematic ATtiny85 and Si4735](https://github.com/pu2clr/SI4735/blob/master/extras/images/attiny85_schematic.png)


### Photo

The photo below show the Si4730 on adapter board with the basic components suggested by Silicon Labs.

![Photo ATtiny85 and Si4730 on Adapter](https://github.com/pu2clr/SI4735/blob/master/extras/images/SI4730_on_adapterA.png)

<BR> 

The photo below shows the Si4730 on adapter board connect attached on a mini breadboard.

![Photo ATtiny85 and Si4730 on mini breadboard](https://github.com/pu2clr/SI4735/blob/master/extras/images/SI4730_on_BreadboardA.png)

<BR>

The Si4730 and ATtiny85 FM radio prototype. 

![Photo ATtiny85 and Si4730](https://github.com/pu2clr/SI4735/blob/master/extras/images/SI4730_attiny85A.jpg)
