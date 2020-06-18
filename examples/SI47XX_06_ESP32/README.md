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
1) This sketch has been successfully tested on ESP LOLIN32 (WEMOS);
2) It uses the capacitive touch ESP32 resource;
3) I2C LiquidCrystal/LCD 20x4;
4) Encoder;
5) FM, AM (MW and SW) and SSB (LSB and USB);
6) Audio bandwidth filter 0.5, 1, 1.2, 2.2, 3 and 4Khz;
7) BFO Control; and
8) Frequency step switch (1, 5 and 10KHz). 
  
To support the SSB mode, This sketch downloads a SSB patch to your SI4735 device (patch_init.h). 
In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
There is little information available about patching the SI4735. The following information is the understanding of the author of this project and it is not necessarily correct. A patch is executed internally (run by internal MCU) of the device. Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device.
Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case __ESP32__. Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.
<BR>
__ATTENTION:__ The author of this project does not guarantee that procedures will work in your development environment. Given this, it is at your own risk to continue with the procedures suggested here. This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.  Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices that you are using.

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
| GPIO32 | Capacitive | 32 | Switch MODE (Am/LSB/USB) | 
| GPIO33 | Capacitive | 33 | Used to select the banddwith. Values: 1.2, 2.2, 3.0, 4.0, 0.5, 1.0 KHz |
| GPIO27 | Capacitive | 27 | Volume Up |
| GPIO14 | Capacitive | 14 | Volume Down |
| GPIO12 | Capacitive | 12 | Switch to the next band |
| GPIO4  | Capacitive |  4 | Switch to the previous band | 
| GPIO2  | Capacitive |  2 | Used to select the increment or decrement frequency step (1, 5 or 10 KHz)
| GPIO15 | Capacitive | 15 | Used to select the enconder control (BFO or VFO) 


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


