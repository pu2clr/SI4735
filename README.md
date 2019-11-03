# Si4735 Library for Arduino

This is a library for the SI4735, BROADCAST AM/FM/SW RADIO RECEIVER,  IC from Silicon Labs for the Arduino development environment.  This library is intended to provide an easier interface for controlling the SI4435.

__The SI4835 is a 3.3V part. If you are not using a +3.3V version of Arduino, you have to use a kind of 5V to 3.3V converter.__

By Ricardo Lima Caratti, Oct, 2019. 

__Attention: Code and Documentation  Under construction.__ 


## Summary

1. [Your support is important](https://github.com/pu2clr/SI4735#your-support-is-important)
2. [About the SI4735](https://github.com/pu2clr/SI4735#about-the-si4735)
3. [Terminology](https://github.com/pu2clr/SI4735#si4735-terminology)
4. [Labrary Features](https://github.com/pu2clr/SI4735#si4735-arduino-library-features)
5. [Library Installation](https://github.com/pu2clr/SI4735#library-installation)
6. [Hardware Requirements and Setup]()
   1. [Schematic]()
   2. [Component Parts]()
   3. [Playing with Arduino UNO or Pro Mini 5v and SI4844]()
   4. [Photos]()
7. [API Documentation](h)
   1. [Defined Data Types and Structures]()
   2. [Public Methods]()
8. [References]()
9.  [Examples]()
10. [Videos]() 


## Your support is important.

If you would like to support this library development, consider joining this project via Github. Alternatively, make suggestions on features you would like available in this library. Thank you!


## About the SI4735  

The Si4735-D60 is DSP radio receiver IC from Silcon Labs. It has great performance on AM (LW/MW/SW) and local FM station. The SI4735  is programmed using commands and responses. You can contron it via a microcontroller like Arduino. To make the SI4735 perform an action, the microccontroller have to send a set of bytes (command and arguments) tha the device interpret it and executes the given command.


### The main Si4735-D60 features

* Great Programming Guide and additional documentation to deal with the device
* FM band support (64–108 MHz)
* AM (MW) band support (520–1710 kHz)
* SW band support (2.3–26.1 MHz)
* LW band support (153–279 kHz)
* Allows firmware upgrade. Including the possibility of adjustments to demodulate SSB.
* Advanced AM/FM seek tuning
* Automatic frequency control (AFC)
* Automatic gain control (AGC)
* Digital FM stereo decoder
* AM/FM/SW/LW digital tuning
* RDS/RBDS processor
* Digital audio out
* I2C and SPI interface 


## SI4735 Terminology


| Term | Description |
| ---- | ----- |
|Arduino Libraries|Libraries are files written in C or C++ (.c, .cpp) which provide your sketches with extra functionality. The SI4735 Library provides extra functionalities to make easier the Arduino deal with Si4735 device| 
|IDE   |Integrated Development Environment|      
|Sketch|Name that Arduino environment uses for a program|
|interrupt|In this context, it is a Arduino Resource. Allows important tasks to be performed regardless of the flow of your program|
|C++| A object-oriented programming (OOP) language. It is a superset of the C language with an additional concept of "classes." |
|programming guide| In this context it refers to [Si48XX ATDD PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN610.pdf)|
|POC| Proof of Concept|
| SEN | Serial enable pin, active low; used as device select in 3-wire and SPI operation and address selection in 2-wire operation| 
| SDIO | Serial data in/data out pin|
| SCLK | Serial clock pin|
| RST  | Also RSTb—Reset pin, active low |
| RCLK | External reference clock |
| GPO | General purpose output |
| CTS | Clear to send |
| STC | Seek/Tune Complete |
| NVM | Non-volatile internal device memory |
| CMD | Command byte |
| COMMANDn | Command register (16-bit) in 3-Wire mode (n = 1 to 4) |
| ARGn | Argument byte (n = 1 to 7) | 
| STATUS | Status byte |
| RESP | Response byte (n = 1 to 15) |
| RESPONSEn | Response register (16-bit) in 3-Wire mode (n = 1 to 8)| 


## SI4735 Arduino Library Features

1. Open Source 
2. Built Based on [Si47XX PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN332.pdf)
3. C++ Lenguage and Object-oriented programming
4. Available on Arduino IDE (Manage Libraries)
5. Simplifies projects based on SI4735


## Library Installation



## Arduino 5V and Si4844


## Hardware Requirements and Setup

This library has been written for the Arduino platform and has been successfully tested on Pro Mini. I beleave it will work on any other Arduino with I2C support.


### Arduino 5V and Si4844

The SI4735 device  works with  3.3V only. If you are not using a 3.3V version of Arduino, you have to use a kind of converter. 

### Schematic

The image bellow shows a version of Slicon Labs SSOP Typical Application Schematic. The basic difference are the pull-up resitors on I2C bus. 

![Basic Schematic](https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png)


The image bellow shows the Slicon Labs SSOP Typical Application Schematic.


![Silicon Labs Schematic](https://github.com/pu2clr/SI4735/blob/master/extras/images/silicon_labs_schematic_pag_19.png)



### Parts



## Photos 

### SI4735 soldered on adapter

![SI4735 on adapter 01](https://github.com/pu2clr/SI4735/blob/master/extras/images/si4735_on_adapter_00.png)


![SI4735 on adapter 01](https://github.com/pu2clr/SI4735/blob/master/extras/images/si4735_on_adapter_01.png)


![SI4735 on adapter 03](https://github.com/pu2clr/SI4735/blob/master/extras/images/si4735_on_adapter_03.png)

#### Protoboard



## API Documentation

API documentation under construction... 


## References

1. [Silicon Labs Si4737 WB/AM/FM Stereo/RDS single-chip receiver HAL library for Arduino ](https://github.com/rickeywang/Si4737_i2c)
2. 


## Videos