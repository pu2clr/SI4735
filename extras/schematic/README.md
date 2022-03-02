# Schematics and Sketches

Please, also check the comments at the beginning of each sketch example available in this library. You will find the SI473X, button, encoder, display and Arduino settings.  

__IT IS IMPORTANT TO SAY THAT THE CIRCUITS DESIGN BUILT BY THE AUTHOR ARE NOT INTEND TO BE A FINAL PRODUCT. SOME IMPORTANT ASPECTS AND DETAILS ABOUT A GOOD RECEIVER WERE OMITTED HERE. THE IDEA OF THE CIRCUITS MADE BY THE AUTHOR WERE ORIENTED TO BE A PROVE OF CONCEPT OF THE FUNCTIONS IMPLEMENTED IN THE ARDUINO LIBRARY. SO, COMPONENTS LIKE RF FRONT-END, BAND PASS FILTER,  ESD, ANTENNA DESIGN ARE NOT THE MAIN PART OF THIS PROJECT__.

__This project is about a library to control the SI47XX devices and the focus of this project is the library and its functionalities. Please, don't ask the author to assist you with displays, encoders, buttons or something else out of the "PU2CLR SI4735 Arduino Library" scope. Thanks.__

### Use the following groups to guide you in your projects

* __Facebook__ group called [__Si47XX for Radio Experimenters__](https://www.facebook.com/groups/532613604253401/). The purpose is exchanging experiences with projects based on Silicon Labs  SI47XX IC family. 
* __group.io__ [SI47XX for hobbyists](https://groups.io/g/si47xx). This group is formed by people with experience in electronics and firmware development. 
  

## SI4735 and SI4732 pinout

If the schematic uses the SI4735 but you have the SI4732, all you have to do is following the lables of those devices to replace them. See pictures below. See the table __SI4735-D60 and SI4732-A10 replacement__  below. 

### The picture below shows the SI4735-D60/SI4730-D60 pinout (SSOP)

![Si4735-D60 pinout](../images/Si4735-D60-pinout.png)

### The picture below shows the SI4732-A10 pinout (16L SOIC Package)

![Si4732-A10 pinout](../images/SI4732_A10_pinout.png)

 

### SI4735-D60 and SI4732-A10 replacement 

| SI4735-D60 PIN      | SI4732-A10 PIN    | Note | 
| ------------------- | ----------------- | -----| 
| #3 (GP03/[DCLK])    | #2 (GP03/[DCLK])  |      |
| #8 (FMI)            | #6 (FMI)          |      | 
| #9 (RFGND)          | #7 (RFGND)        | Depending on your design, you can use this pin connected to GND |
| #10 and #11 (NC)    | Not applicable    | Connected to GND if you have the SI4735 | 
| #12 (AMI)           | #8 (AMI)          |      |
| #13 and #14 (GND)   | #15 (GND)         |      | 
| #15 (RST)           | #9 (RST)          |      | 
| #16 (SEN)           | #10 (SENB)        | __(*1 ) See the text ATTENTION below__ |  
| #17 (SCLK)          | #11 (SCLK)        |      |
| #18 (SDIO)          | #12 (SDIO)        |      | 
| #19 (RCLK)          | #13 (RCLK)        |      | 
| #20 and #21 (VD VA) | #14 (VDD)         | Connected to +VCC (between 2.8V and 3.7V) | 
| #22 (DBYP)          | Not applicable    |      |
| #23 (ROUT/DOUT)     | #16 (ROUT/DOUT)   |      | 
| #24 (LOUT/DFS)      | #1 (LOUT/DFS)     |      |             

(*1) __ATTENTION__: 

While the Si4735 device provides the 0x11 I²C bus address when the SEN pin is connected to the ground, the SI4732-A10 provides the same address when the SEN pin is connected to the +VCC.  The SI4735 Arduino Library provides the function __getDeviceI2CAddress__ to detect the I²C bus address automatically. This way, you don't need to worry about this setup if you use this function. See [getDeviceI2CAddress()](https://pu2clr.github.io/SI4735/extras/apidoc/html/group__group05.html#ga668008457910e916ebadf15bda2e5b29). By default, connect SEN/SENB pin to the GND. See schematics below.

<BR>

### Basic Schematic (Atmega328 based board) 

The two schematics below show the basic setup. One using the SI4735-D60 and other using the SI4732-A10. These schematics will help you to replace the SI4735-D60 with the SI4732-A10 or vice versa. 


#### SI4735-D60 Version

The schematic below shows how to connect the SI473X (SSOP24 package) circuit with Arduino Pro Mini 3.3V/8MHz.


![Basic Schematic](../images/schematic_basic_eagle.png)

[All Sketches on SI47XX_01_SERIAL_MONITOR folder](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR)


|Part	| Description |
|-------| ------------ |
| C1    | 22nF Monolithic Multilayer Chip Ceramic non polarized capacitor (Place it close to VA pin)|
| C2    | 1nF Monolithic Multilayer Chip Ceramic non polarized capacitor |
| C3    | 470nF Monolithic Multilayer Chip Ceramic non polarized capacitor| 
| C4    | 100nF Monolithic Multilayer Chip Ceramic non polarized capacitor (Place it close to VD pin)|
| C5 and C6 | 22pF (Crystal load capacitors) | 
| C7 and C8 *1 | 4.7uF Monolithic Multilayer Chip Ceramic non polarized capacitor | 
| R3    | 2.2K |
| (R4 and R5) *2 | 2.2K to 10K (pull-up resistors) |
| L1 | Ferrite loop stick (about 500 μH) |
| X1    | 32.768 kHz crystal |
| SI4735 | digital CMOS AM(LW, MW and SW)/FM radio receiver IC |

#### SI4732-A10 Version

The schematic below shows how to connect the SI4732-A10  circuit with Arduino Pro Mini 3.3V/8MHz.


![Basic Schematic](../images/schematic_basic_SI4732.png)

[All Sketches on SI47XX_01_SERIAL_MONITOR folder](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR)



<BR>
<BR>

## Atmega328 based board and OLED


![Atmega328 based board and OLED](../images/schematic_basic_oled_SI47XX_02_ALL_IN_ONE_OLED.png)

Sketche [SI47XX_02_ALL_IN_ONE_OLED](https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_03_OLED_I2C/SI47XX_02_ALL_IN_ONE_OLED/)

<BR>

## ESP32 based board 


The schematic below guides you to build a SI473X based receiver using the ESP32 device. 


#### ESP32 basic setup

![ESP32 Basic schematic](../images/schematic_esp32_basic.png)

#### ESP32 DEVKIT with OLED and Encoder setup

![Esp32 based board schematic](../images/schematic_esp32_oled_all_in_one.png)


[All Sketches on SI47XX_06_ESP32 folder](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR)


* [Si4735 Arduino Library - Arduino Mega 2560 and ESP32 LOLIN32 WEMOS](https://youtu.be/HrxR-bUqbjw)

{% include esp32_mega2560.html %}



## ESP8266 


![ESP8266_12_F Basic schematic](../images/schematic_esp8266_12_f.png)



![ESP8266 Basic schematic](../images/schematic_esp8266_basic.png)


## ESP8266 OLED


![ESP32 Basic schematic](../images/schematic_esp8266_oled_SI47XX_02_ALL_IN_ONE.png)



<BR>

## Standalone ATmega328 with or without external Crystal (SI4735-D60 and LCD 16x2). 

The schematic below can be also used with a regular Arduino Board based on ATmega328. It is almost the same circuit posted by Mirko Pavleski on his [Arduino Project repository](https://create.arduino.cc/projecthub/mircemk/diy-si4730-all-band-radio-lw-mw-sw-fm-1894d9). If you use an SI4735-D60 or SI4732-A10, you can have a All band receiver with FM, AM and SSB modes. 

![Standalone ATmega328 with or without external Crystal (LCD 16x2 version)](../images/schematic_basic_atmega328_standalone_12MHz_LCD16x2.png)



Sketch [SI47XX_02_for_Mirko_Pavleski_radio](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_02_for_Mirko_Pavleski_radio). 


Please, use the [MiniCore](https://github.com/MCUdude/MiniCore) setup on your Arduino IDE to deal with standalone Atmega328.

See  video:

* [All band receiver based on SI473X and standalone ATmega328 Modes: FM (VHF) and AM and SSB (HF)](https://youtu.be/H-uIJclH9l8)

{% include atmega328_standalone.html %}


<BR>

