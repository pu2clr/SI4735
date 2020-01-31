# Examples

This folder has some examples that might help you to use the Si4735 Arduino Library in your project. If you find some error or problem during your implementation, please let me know. 

## Summary

1. [Example 01 - Proof of Concept](https://github.com/pu2clr/SI4735/tree/master/examples#example-01---proof-of-concept)
2. [Example 02 - Proof of Concept with SW](https://github.com/pu2clr/SI4735/tree/master/examples#example-02---proof-of-concept-with-sw)
3. [Example 03 - Proof of Concept with SSB](https://github.com/pu2clr/SI4735/tree/master/examples#example-03---proof-of-concept-with-ssb)
4. [Example 04 - SI4735 all in one with SSB Support](https://github.com/pu2clr/SI4735/tree/master/examples#example-04---si4735-all-in-one-with-ssb-support)
5. [Example 05 - SI4730/SI4735 and the ATtiny85](https://github.com/pu2clr/SI4735/tree/master/examples#example-05---si4730si4735-and-the-attiny85)
6. [Examples with LCD, OLED, Buttons and Encoder](https://github.com/pu2clr/SI4735/tree/master/examples#examples-with-lcd-oled-buttons-and-encoder)
7. [Related videos](https://github.com/pu2clr/SI4735/tree/master/examples#related-videos)


<BR>

## Example 01 - Proof of Concept

Test and validation of the SI4735 Arduino Library. The main advantages of using this sketch are: 
* It is a easy way to check if your circuit is working;
* You do not need to connect any display device to make your radio works;
* You do not need connect any push buttons or encoders to change volume and frequency;
* The Arduino IDE is all you need to control the radio.  

__This sketch has been successfully tested on Pro Mini 3.3V; UNO (by using a voltage converter); Arduino Yún (by using a voltage converter); Arduino Micro (see the operating voltage of your Micro); Arduino Mega (by using a voltage converter); Arduino DUE; and ESP32 (LOLIN32 WEMOS)__

I strongly recommend starting with this sketch. This example is a proof of concept of SI4735 Arduino Library. The sketch of this example uses just the __Serial Monitor__ and works only on your __Arduino IDE__. However, you can easily replace the Serial Monitor functions to the device functions that you want to use. 


### Controls used by example 01

| Keyboard command | Description |           
| ---------------- | ----------- |
| A or a | Select MW(AM) |
| F  or f| Select FN |
| U or u | Frequency Up | 
| D or d | Frequency Down |
| S | Seek - Goes to next station |
| s | Seek - Goes to previous station |
| + | Volume Up |
| - | Volume Down |
| 0 | Shows current status | 
| 1 | Goes to previous SW band | 
| 2 | Goes to next SW band |

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_01_POC/SI4735_01_POC.ino) to see this example.


<BR>

## Example 02 - Proof of Concept with SW

This example shows how can you use SW features of the Si4735 Arduino Library. Like the previous exemple (Proof of Concept), the sketch of this example __works only on Arduino IDE__. It uses the Serial Monitor to get the commands and show the information. 

__This sketch has been successfully tested on Pro Mini 3.3V; UNO (by using a voltage converter); Arduino Yún (by using a voltage converter); Arduino Micro (see the operating voltage of your Micro); Arduino Mega (by using a voltage converter); Arduino DUE; and ESP32 (LOLIN32 WEMOS)__

### SW Bands used by the example 02
| Band | From (KHz) | To (KHz) | Default (KHz)|
| ---- | ---------- | -------- | ------------ | 
| 60m | 4600| 5200| 4700|
| 49m | 5700| 6200| 6000|
| 40-41m | 7000 | 7500 | 7200|
| 31m | 9300 | 10000 | 9600 |
| 25m | 11400 | 12200 | 1800|
| 22m | 13500 | 13900 | 13600|
| 19m | 15000 | 15800 | 15200|
| 16m | 17400 | 17900 | 17600|
| 21m | 21400 | 21800 | 21500|
| 11m | 27000 | 27500 | 27220|


### Controls used by the example 02

| Keyboard command | Description |
| ---------------- | ----------- | 
| A or a | Select MW(AM) |
| F or f | Select FN |
| U or u | Frequency Up | 
| D or d | Frequency Down |
| S | Seek - Goes to next station |
| s | Seek - Goes to previous station |
| + | Volume Up |
| - | Volume Down |
| 1 | Previous SW band | 
| 2 | Next SW band | 
| 0 | Show current status | 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_02_POC_SW/SI4735_02_POC_SW.ino) to see this example.

<BR>

## Schematic for Proof of Concept (Examples 01 and 02)

The schematic below is the same shown at the main page of this project and shows how you can setup you SI4735 device.

![Silicon Labs Schematic](../extras/images/basic_schematic.png)

__See some shortwave antenna configuration on__  [Si47XX ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES; AN383](https://www.silabs.com/documents/public/application-notes/AN383.pdf)


<BR>

## Example 03 - Proof of Concept with SSB

This example shows how can you use SSB features of the Si4735 Arduino Library. It uses the Serial Monitor to get the commands and show the information. 

__This sketch has been successfully tested on Pro Mini 3.3V; UNO (by using a voltage converter); Arduino Yún (by using a voltage converter); Arduino Micro (see the operating voltage of your Micro); Arduino Mega (by using a voltage converter); Arduino DUE; and ESP32 (LOLIN32 WEMOS)__

This sketch will download a SSB patch to your SI4735 device (patch_full.h or patch_init.h). It can take up to 15KB of the Arduino memory.
    
In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
There is little information available about patching the SI4735. The following information is the understanding of the author of 
this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device. 
Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device. 

Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system. Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment. 
Given this, it is at your own risk to continue with the procedures suggested here. 
This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60. 
Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.  

Features of this sketch: 

1) Internal Arduino pull-up (no external resistors on schematic for buttons and encoder);
2) Only SSB (LSB and USB);
3) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
4) Ten ham radio bands pre configured;
5) BFO Control; and
6) Frequency step switch (1, 5 and 10KHz);

  Main Parts: 
  Encoder with push button; 
  Seven bush buttons;
  OLED Display with I2C protocol;
  Arduino Pro mini 3.3V;  

### Commands tha you can use via Arduino Serial Monitor
| key | Description  |
| --- | -----------  |
| U   | frequency up | 
| D   | frequency down |
| >   | next band | 
| <   | previous band |
| W   | sitches the filter bandwidth |
| B   | increments the BFO |
| b   | decrement the BFO |
| G   | switches on/off the Automatic Gain Control |
| S   | switches the frequency increment and decrement step |
| s   | switches the BFO increment and decrement step |
| X   | shows the current status | 
| H   | shows this help |


For this example you can use the sketch below.

![Silicon Labs Schematic](../extras/images/basic_schematic.png)

<BR>

## Example 04 - SI4735 all in one with SSB Support

This sketch implements an FM/LW/MW/SW - SSB radio.

This sketch uses I2C LiquidCrystal/LCD, buttons and  Encoder.
  
This sketch uses the Rotary Encoder Class implementation from Ben Buxton (the source code is included
together with this sketch) and LiquidCrystal I2C Library by Frank de Brabander (https://github.com/johnrickman/LiquidCrystal_I2C). Look for LiquidCrystal I2C on Manager Libraries.

This sketch will download a SSB patch to your SI4735 device (patch_content.h). It will take about 8KB of the Arduino memory.

In this context, a patch is a piece of software used to change the behavior of the SI4735 device. There is little information available about patching the SI4735. The following information is the understanding of the author of  this project and it is not necessarily correct. A patch is executed internally (run by nternal MCU) of the device.  Usually, patches are used to fixes bugs or add improvements and new features of the firmware  installed in the internal ROM of the device.  Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system. Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment. Given this, it is at your own risk to continue with the procedures suggested here.
This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.

Features of this sketch:

* FM, AM (MW and SW) and SSB (LSB and USB);
* Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
* 22 commercial and ham radio bands pre configured;
* BFO Control; and
* Frequency step switch (1, 5 and 10KHz);


__This sketch has been successfully tested on Pro Mini 3.3V; UNO (by using a voltage converter); Arduino Yún (by using a voltage converter); Arduino Micro (see the operating voltage of your Micro); Arduino Mega (by using a voltage converter); and Arduino DUE;__


### Schematic

![Schematic All In One](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_i2c.png)


### Protoboard

![Protoboard All In One](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_00.png)


See the video [Evaluating SSB audio from the SI4735-based radio prototype to see this example working](https://youtu.be/Xj7zuiyvysI)


### Features
* This example implements AM (MW and SW), FM and SSB. 
* 22 commercial and ham radio bands pre configured;
* FM;
* AM;
* HF bands;
* SSB; 
* Audio bandwidth filter: 
  * SSB: 0.5, 1, 1.2, 2.2, 3 and 4Khz;  
  * AM:  1, 1.8, 2, 2.5, 3, 4 and 6 KHz;
* BFO Control via encoder;
* Frequency step options: 1, 5 and 10KHz;


<BR>

## Example 05 - SI4730/SI4735 and the ATtiny85 

This example implements a FM radio using the ATtiny85 as the master MCU.

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


## Examples with LCD, OLED, Buttons and Encoder

The folder [SI4735_99_OTHER_EXAMPLES](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_99_OTHER_EXAMPLES) contains several other examples that use encoders, buttons, OLED and LCD. This folder also has some sketches used to make tests on the IC Si4735.


## Related videos

* [SI4735 Arduino Library and a simple FM and AM radio implementation](https://youtu.be/vzunuxam_Lg) 
* [Si4735 Arduino Library example. OLED Application.](https://youtu.be/7Sg4z8tDSA8)
* [Si4735 Arduino Library and SSB Support (Listening to a QSO on 20 meters)](https://youtu.be/W2Ssjb9P_f4)
* [SI4735 ARDUINO LIBRARY BANDWIDTH TEST](https://youtu.be/dN1s3RoXGos) 
* [Evaluating SSB audio from the SI4735-based radio prototype](https://youtu.be/Xj7zuiyvysI)
* [Si4735 Arduino Library - Arduino Mega 2560 and “ESP32 LOLIN32 WEMOS](https://youtu.be/ArFav_SFEXI)
* [How to "SI4735 Arduino Library and ATtiny85"](https://youtu.be/zb9TZtYVu-s)
* [SI4735 Arduino Library working with ATtiny85](https://youtu.be/U9Xpqh3K4e0)
* [Install the ESP32 Board in Arduino IDE in less than 1 minute (Windows, Mac OS X, and Linux)](https://youtu.be/mBaS3YnqDaU)
* [Install Arduino Due on Arduino IDE](https://youtu.be/ogXQIYnI8qE)
  