# ESP32 and Si4735 Arduino Library

This folder show some examples with Si4735 Arduino Library  and ESP32 platform.




## Example SI47XX_01_ESP32_AM_FM_TOUCH_SERIAL_MONITOR

This sketch uses just the Serial Monitor as the human interface to test and validation of the SI4735 Arduino Library on ESP platform. It can be very useful to test your circuit. 

### The main advantages of using this sketch are: 

* It is a easy way to check if your setup is working;
* You do not need to connect any display device to make your radio works;
* You do not need connect any push buttons or encoders to change volume and frequency;
* The Arduino IDE is all you need to control the radio.  


### Si4735 Wire up


| Si4735    | Function              |ESP LOLIN32 WEMOS (GPIO) |
|-----------| ----------------------|-------------------------|
| pin 15    |   RESET               |   25 (GPIO25)           |  
| pin 18    |   SDIO                |   21 (SDA / GPIO21)     |
| pin 17    |   SCLK                |   22 (SCL / GPIO22)     |



## Example SI47XX_02_ESP32_TOUCH_ALL_IN_ONE

This example is a complete FM/AM/LW/SW (with SSB support) receiver based on SI4735 running on ESP32 


### __Features:__
1) This sketch has been successfully tested on ESP LOLIN32 (WEMOS) and ESP32 DEVKIT;
2) It uses the capacitive touch ESP32 resource;
3) I2C LiquidCrystal/LCD 20x4;
4) Encoder;
5) FM, AM (MW and SW) and SSB (LSB and USB);
6) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
7) BFO Control; and
8) Frequency step switch (1, 5 and 10KHz). 

### About SSB support

The the __PU2CLR SI4735 Arduino Library__ implements a set of methods to apply patches and deal with SSB mode. All documentation about pathces can be seen [here](https://pu2clr.github.io/SI4735/extras/apidoc/html/group__group17.html).

__First of all, it is important to say that the SSB patch content is not part of this library__. The paches used here were made available by Mr. [Vadim Afonkin](https://youtu.be/fgjPGnTAVgM) on his [Dropbox repository](https://www.dropbox.com/sh/xzofrl8rfaaqh59/AAA5au2_CVdi50NBtt0IivyIa?dl=0). It is important to note that the author of this library does not encourage anyone to use the SSB patches content for commercial purposes. __In other words, this library only supports SSB patches, the patches themselves are not part of this library__.

[Read more about PU2CLR SI4735 Arduino Library SSB support](https://pu2clr.github.io/SI4735/#si4735-patch-support-for-single-side-band)


### ESP32 considerations

The ESP32 has a great resource to interact with system. Instead of buttons, you can use the capacitive sensors available on ESP32 board family. 

__The ESP32 is a 3.3V part. Pay attention on your I2C LCD device voltage. If it is not 3.3V, the system cannot work properly__.

The ESP32 board has enough RAM memory to support all SSB patch and a nice TFT Display library. Consider this feature to improve your sketch.


### Schematic 

You can use the same schematic used on Arduino family. 
However, you have pay attention to the new configuration pins on ESP32. See tables bellow. The circuit below shows a generic setup for ESP32. __It is very important to consider the pinout of your ESP32 board__.

![Schematic for I2C display device](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic_esp32_eagle.png)


### ESP-LOLIN32-WEMOS touch capacitive 

| GPIO   | Used as  | Constant number (#define)| Function Description | 
| ------ | -------- | ------------------ | ----------- | 
| GPIO32 | Capacitive | 32 (GPIO32) | Switch MODE (Am/LSB/USB) | 
| GPIO33 | Capacitive | 33 (GPIO33) | Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz |
| GPIO27 | Capacitive | 27 (GPIO27) | Volume Up |
| GPIO14 | Capacitive | 14 (GPIO14) | Volume Down |
| GPIO12 | Capacitive | 12 (GPIO12) | Switch to the next band |
| GPIO4  | Capacitive |  4 (GPIO4)  | Switch to the previous band | 
| GPIO2  | Capacitive |  2 (GPIO2)  | Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
| GPIO15 | Capacitive | 15 (GPUI15) | Used to select the enconder control (BFO or VFO) 


### Encoder wire up

| Encoder   | Function              |ESP LOLIN32 WEMOS (GPIO) |
|-----------| ----------------------|-------------------------|
| A         |  interrupt            |   16  (GPIO16)          |
| B         |  interrupt            |   17  (GPIO17)          |
| BUTTON    |  ENCODER PUSH BUTTON  |   23  (GPIO23)          |  



It seams that in some ESP32 board, the I2C bus is not configured prorpelly by default. However, you can set almost any pin on ESP32 to setup I2C capabilities. All you have to do is call __Wire.begin(SDA, SCL);__ where SDA and SCL are the ESP32 GPIO pins. The code below shows that.

```cpp
// I2C bus pin on ESP32
#define ESP32_I2C_SDA 21
#define ESP32_I2C_SCL 22

.
.
.
.

void setup() {


  // The line below may be necessary to setup I2C pins on ESP32 Dev Module
  Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);


}
```


### Main features:

* This sketch has been successfully tested on ESP LOLIN32 (WEMOS) and ESP32 Devkit;
* It uses the capacitive touch ESP32 resource;
* I2C LiquidCrystal/LCD 20x4;
* Encoder;
* FM, AM (MW and SW) and SSB (LSB and USB);
* Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
* BFO Control; and
* Frequency step switch (1, 5 and 10KHz).


## Example SI47XX_03_ESP32_ALL_IN_ONE_TFT

Still under construction this sketch will show how to use TFT with ESP32, Si4735 device and Si4735 Arduino Library.



### ESP32 and TFT  wire up 
  
|  LCD SCREEN           | NODEMCU ESP32 PIN                          |
| -------------------  | ------------------------------------------- |
|      VCC             |     3.3V                                    |
|      GND             |     GND                                     |
|      LED             |     3.3V                                    |
|   SCL (SPI Clock)    | GPIO18 / VSPI SCK (SPI Clock)               |
|   SDA (SPI Data)     | GPIO23 / VSPI MOSI (MOSI, Data to Screen)   |
|   RS or DC           | GPIO2 (In some boards it is labelled as DC) |
|   RST (Screen reset) | GPIO4                                       |
|   CS  or SS          | GPIO5 VSPI SS (Slave Select or Chip Select) |


