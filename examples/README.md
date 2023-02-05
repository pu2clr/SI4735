# [Examples](https://pu2clr.github.io/SI4735/examples)

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


<BR>

# Arduino Tips


## How to compile and upload sketches

This library was developed to work with Arduino environment. There are many enviroment that you can use to compile and upload Arduino sketches to your board.  The list below shows some of them: 

* [Arduino IDE](https://www.arduino.cc/en/software)
* [Visual Studio with Arduino extensions](https://marketplace.visualstudio.com/items?itemName=VisualMicro.ArduinoIDEforVisualStudio)
* [PlatformIO](https://dronebotworkshop.com/platformio/)
* [Arduino Command Line - arduino-cli](https://www.arduino.cc/pro/cli) 
  

The most popular environment to write sketch, compile and upload is the [Arduino IDE](https://www.arduino.cc/en/software).
After installing the [PU2CLR Si4735 Library on Arduino IDE](https://youtu.be/M9h-tlV_l-k) you can compile and upload any sketch available here by using the Menu examples of the Arduino IDE. Whatch the video [Installing PU2CLR Si4735 Library on Arduino IDE](https://youtu.be/M9h-tlV_l-k) to know how to compile, upload and run Arduino Application.

Another way to compile and upload sketches to Arduino board is using the [arduino-cli](https://www.arduino.cc/pro/cli). 
See the section below for details.  

<BR>

## Arduino CLI - A faster alternative to the Arduino IDE

Arduino CLI (arduino-cli) is a solution that allows you to compile, build, upload, manage boards and libraries via shell command lines. This way, you do not need to use the traditional Arduino IDE. Depending on the development environment you use, arduino-cli may be more appropriate, given it consumes less computer resources. You will notice that the arduino-cli allows compiles and uploades process faster. There is, however, the drawback of being a less intuitive environment than the Arduino IDE, offering a longer learning time for most users. The links below can help you to know more about arduino-cli. 

* [Click here for more detail about arduino-cli](https://arduino.github.io/arduino-cli/0.21/).
* [Getting started](https://arduino.github.io/arduino-cli/0.21/getting-started/)
* [Click here to watch a video about the arduino-cli](https://youtu.be/J-qGn1eEidA)


### [Installing via Homebrew (macOS/Linux)](https://arduino.github.io/arduino-cli/0.21/installation/)

The easier way to install arduino-cli on your Linux or MACOS system is shown below.

```bash
$ brew update
$ brew install arduino-cli
```

More about installing arduino-cli on Linux, MACOS and Windows, go to [https://arduino.github.io/arduino-cli/0.21/installation/](https://arduino.github.io/arduino-cli/0.21/installation/).


### Main commands examples

It is important to say that all boards and libraries installed via your Arduino IDE is used by arduino-cli. I mean, all Arduino environment setup is shared by Arduino IDE and arduino-cli.
This way, if you install a new board via arduino-cli, it will be seen by Arduino IDE and vice-versa. 


### The command below identifies the boards connected to your computer (COM3, COM4, /dev/xxx).

It is very useful to show the boards connected to your computer

```bash
$ arduino-cli board list

Port                            Protocol Type              Board Name                     FQBN                          Core       
/dev/cu.usbmodem14201           serial   Serial Port (USB) Arduino Due (Programming Port) arduino:sam:arduino_due_x_dbg arduino:sam
/dev/cu.usbserial-1410          serial   Serial Port (USB) Unknown     

```

### The command below shows all boards available 

It is very useful to give you the FQBN information to compile and upload the right board. 

```bash
$ arduino-cli board listall
```

### The command below shows the board installed in your environment.

```bash
$ arduino-cli core list

Board Name                                       FQBN                                            
3D printer boards                                STM32:stm32:3dprinter                           
4D Systems gen4 IoD Range                        esp8266:esp8266:gen4iod                         
AI Thinker ESP32-CAM                             esp32:esp32:esp32cam
.
.
.
Arduino Due (Native USB Port)                    arduino:sam:arduino_due_x                       
Arduino Due (Programming Port)                   arduino:sam:arduino_due_x_dbg   
.
.
.
Arduino Nano                                     arduino:avr:nano                                
Arduino Nano 33 BLE                              arduino:mbed_nano:nano33ble      
.
.
.   
Arduino Uno                                      arduino:avr:uno       
```

#### Example: 

```bash
$ arduino-cli core list
  

ID                   Installed Latest Name                                                    
arduino:avr          1.8.5     1.8.5  Arduino AVR Boards                                      
arduino:mbed_nano    3.0.0     3.0.0  Arduino Mbed OS Nano Boards                             
arduino:mbed_rp2040  3.0.0     3.0.0  Arduino Mbed OS RP2040 Boards                           
arduino:sam          1.6.12    1.6.12 Arduino SAM Boards (32-bits ARM Cortex-M3)              
arduino:samd         1.8.13    1.8.13 Arduino SAMD Boards (32-bits ARM Cortex-M0+)            
atmel-avr-xminis:avr 0.6.0     0.6.0  Atmel AVR Xplained-minis                                
attiny:avr           1.0.2     1.0.2  ATtiny Microcontrollers                                 
ATTinyCore:avr       1.5.2     1.5.2  ATTinyCore                                              
esp32:esp32          2.0.2     2.0.2  ESP32 Arduino                                           
esp8266:esp8266      3.0.2     3.0.2  ESP8266 Boards (3.0.2)                                  
MegaCore:avr         2.1.3     2.1.3  MegaCore                                                
MightyCore:avr       2.1.3     2.1.3  MightyCore                                              
MiniCore:avr         2.1.3     2.1.3  MiniCore                                                
rp2040:rp2040        1.13.0    1.13.0 Raspberry Pi RP2040 Boards(1.13.0)                      
Seeeduino:samd       1.8.2     1.8.2  Seeed SAMD (32-bits ARM Cortex-M0+ and Cortex-M4) Boards
STM32:stm32          1.9.0     1.9.0  STM32 Boards (selected from submenu)  
teensy:avr           1.56.1    1.56.1 Teensyduino                    
```


### The command below just compiles the sketch SI47XX_01_SERIAL_MONITOR/SI4735_01_POC for an Arduino Nano

```bash
$ arduino-cli compile -b arduino:avr:nano ./SI47XX_01_SERIAL_MONITOR/SI4735_01_POC
```

### The command below compiles and uploads the sketch into the Arduino Nano board 

```bash
$ arduino-cli compile  -b arduino:avr:nano -u -p yourPort ./SI47XX_01_SERIAL_MONITOR/SI4735_01_POC. 
```
Where __yourPort__ can be COM3, COM4, COM(N) if you are using Microsoft Windows; or /dev/XXX if you are using unix like SO (Linux or  MacOS).


### Example using Arduino DUE connected to a MACOS

```bash
$ arduino-cli compile --fqbn arduino:sam:arduino_due_x -u -p /dev/cu.usbmodem14201  ./SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
```


__See command arduino-cli board list below to know how to get the port.__


### Example using Arduino Mega

```bash
$ arduino-cli compile --fqbn arduino:avr:mega -u -p /dev/cu.usbserial-1410  ./SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
```

If you need the .hex, .elf or  .bin files you can use 

```bash
$ arduino-cli compile --fqbn arduino:avr:mega --build-path /Users/UserName/Downloads ./SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
```

### Example using ESP32

#### Example using ESP32 DEVMODE

```bash
$ arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso -u -dev -u -p /dev/cu.usbserial-0001  ./SI47XX_KITS/THIAGO_LIMA/GERT_BAAK/SI4735_2.8_TFT_SI5351_V4.2
```
where /dev/cu.usbserial-0001 is the Port device. It can be different in your environment. Check it via __arduino-cli board list__ command. 

#### Example using ESP32 LOLIN32 

```bash
arduino-cli compile --fqbn esp32:esp32:lolin32 -u -p /dev/cu.usbserial-00874A3C ./SI47XX_KITS/THIAGO_LIMA/GERT_BAAK/SI4735_2.8_TFT_SI5351_V4.2 -v
```

### Example using STM32 (compiling only)

arduino-cli compile --fqbn STM32:stm32:Nucleo_64 ./SI47XX_KITS/PLAMEN/PU2CLR_SI5351_SI4732_STM32


#### To know the right port you have to use, try the command below: 

```bash
$ arduino-cli board list
```


#### arduino-cli board list command example 

```bash
arduino-cli board list
Port                            Protocol Type              Board Name                     FQBN                          Core       
/dev/cu.usbmodem14201           serial   Serial Port (USB) Arduino Due (Programming Port) arduino:sam:arduino_due_x_dbg arduino:sam
```

#### arduino-cli compile and upload on Arduino DUE

```bash
$ arduino-cli compile --fqbn arduino:sam:arduino_due_x -u -p /dev/cu.usbmodem14201  ./SI47XX_10_RDS/SI47XX_02_RDS_TFT_TOUCH_SHIELD_35_V2
  
Sketch uses 90896 bytes (17%) of program storage space. Maximum is 524288 bytes.
Atmel SMART device 0x285e0a60 found
Erase flash
done in 0.033 seconds

Write 93876 bytes to flash (367 pages)
[==============================] 100% (367/367 pages)
done in 18.038 seconds
Set boot flash true
CPU reset.
```

##### arduino-cli board list command with two devices connected to the computer at the same time (Arduino DUE and Arduino Mega)

The command below show two Arduino boards connected to the computer at the same time. The first is an Arduino DUE as deteiled below and the second is an Arduino Mega with no details.
Unfortunatly, in some cases, the Arduino IDE and arduino-cli can not show details about the divice connected to the computer. In this case, you have to conclude this by yourself. 

```bash
$ arduino-cli board list

Port                            Protocol Type              Board Name                     FQBN                          Core       
/dev/cu.usbmodem14201           serial   Serial Port (USB) Arduino Due (Programming Port) arduino:sam:arduino_due_x_dbg arduino:sam
/dev/cu.usbserial-1410          serial   Serial Port (USB) Unknown     

```

##### arduino-cli board list command with two devices connected at the same time (Arduino Yún and Arduino Micro)

```bash

$ arduino-cli board list

Port                            Protocol Type              Board Name    FQBN              Core       
/dev/cu.usbmodem14101           serial   Serial Port (USB) Arduino Yún   arduino:avr:yun   arduino:avr
/dev/cu.usbmodem14201           serial   Serial Port (USB) Arduino Micro arduino:avr:micro arduino:avr
```


The table below show some Arduino board FQBN

| Board Name | FQBN | 
| ---------- | ---- |                                      
| Arduino DUE                         | arduino:sam:arduino_due_x |                        
| Arduino Duemilanove or Diecimila    | arduino:avr:diecimila  |                                    
| Arduino Ethernet                    | arduino:avr:ethernet   |                                                                
| Arduino Industrial 101              | arduino:avr:chiwawa    |                   
| Arduino Leonardo                    | arduino:avr:leonardo   |                                   
| Arduino M0                          | arduino:samd:mzero_bl  |                   
| Arduino M0 Pro (Native USB Port)    | arduino:samd:mzero_pro_bl  |                
| Arduino M0 Pro (Programming Port)   | arduino:samd:mzero_pro_bl_dbg |                            
| Arduino MKR WiFi 1010               | arduino:samd:mkrwifi1010 |                                                   
| Arduino Mega 2560                   | arduino:avr:mega         |                 
| Arduino Micro                       | arduino:avr:micro        |                 
| Arduino Mini                        | arduino:avr:mini         |                 
| Arduino NANO 33 IoT                 | arduino:samd:nano_33_iot |                               
| Arduino Nano                        | arduino:avr:nano         |                              
| Arduino Pro or Pro Mini             | arduino:avr:pro          |                                                    
| Arduino Uno                         | arduino:avr:uno          |                 
| Arduino Uno WiFi                    | arduino:avr:unowifi      |                                
| Arduino Yún                         | arduino:avr:yun          |                 
| Arduino Yún Mini                    | arduino:avr:yunmini      |                 
| Arduino Zero (Native USB Port)      | arduino:samd:arduino_zero_native |          
| Arduino Zero (Programming Port)     | arduino:samd:arduino_zero_edbg  |                        
| ESP32 Dev Mode                      | esp32:esp32:esp32-poe-is |
| ESP32 LOLIN                         | esp32:esp32:lolin32 |
| ESP8266                             | esp8266:esp8266:generic |  
| Raspberry Pi Pico                   | rp2040:rp2040:rpipico | 
| Seeeduino XIAO                      | Seeeduino:samd:seeed_XIAO_m0 |          
| STM32                               | STM32:stm32:Nucleo_64 | 

type __arduino-cli board listall__ to see all boards available.


### arcuino-cli references

* [Getting Started with arduino-cli](https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5)
* [Arduino CLI and the art of command line](https://youtu.be/cVod8k713_8)
* [Getting started](https://arduino.github.io/arduino-cli/0.21/getting-started/)
* [Arduino CLI: Getting Started](https://youtu.be/J-qGn1eEidA)
* [Arduino-cli - github.com](https://github.com/arduino/arduino-cli)
* [Arduino-cli: compile, upload and manage libraries, cores, and boards](https://www.pcbway.com/blog/Activities/Arduino_cli__compile__upload_and_manage_libraries__cores__and_boards.html)




## Dealing images with TFT display 

It can be very useful to improve the visual interface to your project.

* First, convert your image to C/C++ code. To do that, you can use this tools:[https://www.mischianti.org/images-to-byte-array-online-converter-cpp-arduino/](https://www.mischianti.org/images-to-byte-array-online-converter-cpp-arduino/).
  * Use the following parameters to create the C/C++ based on image you want to convert;
    * scretch to fill canvas;
    * Arduino code, single bitmaps;
    * Horizontal - 2 bytes per pixel (565);
    * User Identifier: yourImage (an C/C++ valiable name);
    * Click on Generate button
* Check the firt line genereted by the app: __const uint16_t YourImage [] PROGMEM = {__ ....
* Copy and paste of the code generetaed by the app to a .h file (create a empity .h file and then paste the C/C++ code. example: yourImage.h);
* Include the created .h file (yourImage.h) in your sketch main code (example: #include "./images/yourImage.h");
* Finally, call the Adafruit_GFX function: tft.drawRGBBitmap(0, 0, yourImage, 168, 120); 

