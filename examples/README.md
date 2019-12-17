# Examples

This folder has some examples that might help you to use the Si4735 Arduino Library in your project. If you find some error or problem during your implementation, please let me know. 

## Summary

1. [Example 01 - Proof of Concept](https://github.com/pu2clr/SI4735/tree/master/examples#example-01---proof-of-concept)
2. [Example 02 - Proof of Concept with SW](https://github.com/pu2clr/SI4735/tree/master/examples#example-02---proof-of-concept-with-sw)
   * [Schematic for Proof of Concept (Examples 01 and 02)](https://github.com/pu2clr/SI4735/tree/master/examples#schematic-for-proof-of-concept-examples-01-and-02)
3. [Example 03 - SS4735 Arduino Library example with LCD 20x4 I2C](https://github.com/pu2clr/SI4735/tree/master/examples#example-03---ss4735-arduino-library-example-with-lcd-20x4-i2c)
4. [Example 04 - SS4735 Arduino Library example with I2C OLED](https://github.com/pu2clr/SI4735/tree/master/examples#example-04---ss4735-arduino-library-example-with-i2c-oled)
   * [Schematic for I2C display device, buttons and encoder (Examples 03 and 04)](https://github.com/pu2clr/SI4735/tree/master/examples#schematic-for-i2c-display-device-buttons-and-encoder-examples-03-and-04)
5. [Example 05 - Bandwidth filter on MW (AM)](https://github.com/pu2clr/SI4735/tree/master/examples#example-05---bandwidth-filter-on-mw-am)
6. [Example 06 - Bandwidth filter test (9 band SW Receiver with OLED)](https://github.com/pu2clr/SI4735/tree/master/examples#example-06---bandwidth-filter-test-9-band-sw-receiver-with-oled)
7. [Example 07 - Si4735 SSB firmware update support - OLED/I2C Suport)](https://github.com/pu2clr/SI4735/tree/master/examples#example-07---si4735-ssb-firmware-update-support)
8. [Example 08 - Si4735 SSB firmware update support - LCD/I2C Support]()

<BR>
<BR>

## Example 01 - Proof of Concept

Test and validation of the SI4735 Arduino Library. The main advantages of using this sketch are: 
* It is a easy way to check if your circuit is working;
* You do not need to connect any display device to make your radio works;
* You do not need connect any push buttons or encoders to change volume and frequency;
* The Arduino IDE is all you need to control the radio.  

So, I strongly recommend starting with this sketch. This example is a proof of concept of SI4735 Arduino Library. The [SI4735_POC.ino](https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_POC) sketch uses just the __Serial Monitor__ and works only on your __Arduino IDE__. However, you can easily replace the Serial Monitor functions to the device functions that you want to use. 

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
| 0 | Show current status | 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_01_POC/SI4735_01_POC.ino) to see this example.


<BR>
<BR>

## Example 02 - Proof of Concept with SW

This example shows how can you use SW features of the Si4735 Arduino Library. Like the previous exemple (Proof of Concept), the [SI4735_POC_SW.ino](examples/SI4735_POC_SW/SI4735_POC_SW.ino) sketch __works only on Arduino IDE__. It uses the Serial Monitor to get the commands and show the information. 


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
<BR>

## Schematic for Proof of Concept (Examples 01 and 02)

The schematic below is the same shown at the main page of this project and shows how you can setup you SI4735 device.

![Silicon Labs Schematic](../extras/images/basic_schematic.png)

<BR>
<BR>

## Example 03 - SS4735 Arduino Library example with LCD 20x4 I2C

It is a example that shows how to set up a LCD and SI4735 on the same I2C bus. 
This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the LiquidCrystal I2C Library by Frank de Brabander. You can install LiquidCrystal I2C via Manage Libraries on your Arduino IDE. 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_03_LCD_20x4_I2C/SI4735_03_LCD_20x4_I2C.ino) to see this example.


__Click [here](https://youtu.be/vzunuxam_Lg) to see a video about this example__.

<BR>
<BR>

## Example 04 - SS4735 Arduino Library example with I2C OLED

This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch. This sketch also uses the SSD1306Ascii Arduino Library by Bill Greiman. You can install SSD1306Ascii via Manage Libraries on your Arduino IDE. The photo below show this example on a protoboard. 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_04_OLED_I2C/SI4735_04_OLED_I2C.ino) to see this example.

![Silicon Labs Schematic](../extras/images/prot_oled_01.png)


__Click [here](https://youtu.be/7Sg4z8tDSA8) to see a video about this example__.


<BR>
<BR>

## Schematic for I2C display device, buttons and encoder (Examples 03 and 04)

The schematic below is a sugestion to connect a I2C Display (OLED or LCD 20x4 or another device), buntons and encoder as well. 

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_with_buttons_i2c.png)

__Pay attention to the your I2C device voltage. The system can become unstable if you are using a wrong voltage__.


<BR>
<BR>


## Example 05 - Bandwidth filter on MW (AM)

Bandwidth of the channel filter for AM reception.  This sketch tests the bandwidth configuration on Si4735 device. It uses the Serial Monitor (Arduino IDE). The choices are: 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz).
 
Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_05_AM_FILTERS/SI4735_05_AM_FILTERS.ino) to see this example.


<BR>
<BR>


## Example 06 - Bandwidth filter test (9 band SW Receiver with OLED)

This example is a 9 band SW receiver based on SI4735.  It shows the selection of the bandwidth of the channel filter for AM reception (in this case SW). The choices are: 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). With this resource, you can separate  two stations in adjacent channels. __You can use the same schematic shown for the examples 03 and 04__. 

Click [here](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_06_SW_OLED_FILTER_TEST/SI4735_06_SW_OLED_FILTER_TEST.ino) to see this example.

__Click [here](https://youtu.be/dN1s3RoXGos) to see a video about this example__.

<BR>
<BR>

## Example 07 - Si4735 SSB firmware update support

  This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch.

  This sketch will download a SSB patch to your SI4735 device (patch_content.h). It will take about 15KB of the Arduino memory.
    
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
<BR>

### Example 08 - Si4735 SSB firmware update support - LCD/I2C Support


This sketch uses the Rotary Encoder Class implementation from Ben Buxton. The source code is included together with this sketch.
This sketch will download a SSB patch to your SI4735 device (patch_content.h). It will take about 15KB of the Arduino memory.

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


![Protoboard with OLED as a S Meter](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_SW_SMETER.png)


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