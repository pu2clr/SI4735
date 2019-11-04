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
7. [API Documentation](https://github.com/pu2clr/SI4735#api-documentation)
   1. [Defined Data Types and Structures](https://github.com/pu2clr/SI4735#defined-data-types-and-structures)
   2. [Public Methods]()
   3. [setup](https://github.com/pu2clr/SI4735#setup)
   4. [setPowerUp](https://github.com/pu2clr/SI4735#setpowerup)
   5. [analogPowerUp](https://github.com/pu2clr/SI4735#analogpowerup)
   6. [setFrequency](https://github.com/pu2clr/SI4735#setfrequency)
   7. [seekStation](https://github.com/pu2clr/SI4735#seekstation)
   8. [setAM](https://github.com/pu2clr/SI4735#setam)
   9. [setFM](https://github.com/pu2clr/SI4735#setfm)
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
|programming guide| In this context it refers to [Si47XX PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN332.pdf)|
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


![SI4735 on adapter 04](https://github.com/pu2clr/SI4735/blob/master/extras/images/adapter_01.png)


![SI4735 on adapter 05](https://github.com/pu2clr/SI4735/blob/master/extras/images/adapter_02.png)



#### Protoboard

![Protoboard 00](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_00.png)



![Protoboard 01](https://github.com/pu2clr/SI4735/blob/master/extras/images/protoboard_01.png)




## API Documentation



### Defined Data Types and Structures


To make the SI4735 device easier to deal, some defined data types were built to handle byte and bits responses.




```cpp
/*
 * Power Up arguments data type 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 65
 */ 
typedef union {
    struct
    {
        // ARG1
        byte FUNC       : 4;  // Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
        byte XOSCEN     : 1;  // Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
        byte PATCH      : 1;  // Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
        byte GPO2OEN    : 1;  // GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
        byte CTSIEN     : 1;  // CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).
        // ARG2
        byte OPMODE; // Application Setting. See page 65
    } arg;
    byte raw[2]; // same arg memory position, so same content.
} si473x_powerup;

/*
 * Represents how the frequency is stored in the si4735.
 * It helps to convert frequency in unsigned int to two bytes (FREQL and FREQH)  
 */
typedef union {
    struct
    {
        byte FREQL; // Tune Frequency High Byte.
        byte FREQH; // Tune Frequency Low Byte.
    } raw;
    unsigned value;
} si47x_frequency;

/* 
 *  Represents searching for a valid frequency data type.
 */
typedef union {
    struct
    {
        byte RESERVED1 : 2;
        byte WRAP : 1;   // Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
        byte SEEKUP : 1; // Determines the direction of the search, either UP = 1, or DOWN = 0.
        byte RESERVED2 : 4;
    } arg;
    byte raw;
} si47x_seek;

```


## Public Methods



### setup

```cpp
/* 
 * Starts the Si473X device. 
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte interruptPin interrupt Arduino Pin (see your Arduino pinout). 
 * @param byte defaultFunction
 */ 
void SI4735::setup(byte resetPin, byte interruptPin, byte defaultFunction)
```

#### Example of using setup

```cpp

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

SI4735 si4735;

void setup()
{
    si4735.setup(RESET_PIN, INTERRUPT_PIN, FM_FUNCTION);
}
```


### setPowerUp

```cpp
/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * 
 * @param byte CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param byte GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param byte PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param byte XOSCEN byte XOSCEN set external Crystal enabled or disabled 
 * @param byte FUNC sets the receiver function have to be used (0 = FM Receive; 1 = AM (LW/MW/SW) Receiver)
 * @param byte OPMODE set the kind of audio mode you want to use.
 */
void SI4735::setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE)
```

### analogPowerUp


```cpp
/*
 * Powerup in Analog Mode
 * You have to call setPowerUp before call analogPowerUp 
 */
void SI4735::analogPowerUp(void) 
```

#### Example of using analogPowerUp

```cpp 
    // Set the initial SI473X behavior
    // CTSIEN   1 -> Interrupt anabled;
    // GPO2OEN  1 -> GPO2 Output Enable;
    // PATCH    0 -> Boot normally;
    // XOSCEN   1 -> Use external crystal oscillator;
    // FUNC     defaultFunction = 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
    // OPMODE   SI473X_ANALOG_AUDIO = 00000101 = Analog audio outputs (LOUT/ROUT).

    setPowerUp(1, 1, 0, 1, defaultFunction, SI473X_ANALOG_AUDIO);
    analogPowerUp();
```    

### setFrequency

```cpp
/*
 * Set the frequency to the corrent function of the Si4735 (AM or FM)
 * You have to call setup or setPowerUp before call setFrequency.
 * 
 * @param unsigned freq Is the frequency to change. For example, FM => 10390 = 103.9 MHz; AM => 810 = 810 KHz. 
 */
void SI4735::setFrequency(unsigned freq) 
```

#### Example of using setFrequency

```cpp
    si4735.setFM(); 
    si4735.setFrequency(fm_freq); 
    showStatus(fm_freq,"MHz");
```    

### seekStation
```cpp
/*
 * Look for a station 
 * See Si47XX PROGRAMMING GUIDE; AN332; page 72
 * 
 * @param SEEKUP Seek Up/Down. Determines the direction of the search, either UP = 1, or DOWN = 0. 
 * @param Wrap/Halt. Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
 */
void SI4735::seekStation(byte SEEKUP, byte WRAP)
```

#### Example of using seekStation

```cpp
    si4735.seekStation(1,1);
```

### setAM

```cpp
/*
 * Set the radio to AM function
 */ 
void SI4735::setAM()
```

### setFM

```cpp
/*
 * Set the radio to FM function
 */
void SI4735::setFM()
```



## References

1. [Silicon Labs Si4737 WB/AM/FM Stereo/RDS single-chip receiver HAL library for Arduino ](https://github.com/rickeywang/Si4737_i2c)
2. 


## Videos