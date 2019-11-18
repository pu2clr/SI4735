# Si4735 Library for Arduino

This is an Arduino library for the SI4735, BROADCAST AM/FM/SW RADIO RECEIVER IC from Silicon Labs.  This library is intended to provide an easier interface for controlling the SI4435 by using Arduino platform. __The communication used by this library is I2C__. 

This library can be freely distributed using the MIT Free Software model. [Copyright (c) 2019 Ricardo Lima Caratti](https://github.com/pu2clr/SI4735#mit-licence)  

Contact: pu2clr@gmail.com


__Attention__: 
* __Documentation  under construction__. 
* __The SI4835 is a 3.3V part. If you are not using a 3.3V version of Arduino, you have to use a kind of 5V to 3.3V converter__.


## Summary

1. [License Copyright](https://github.com/pu2clr/SI4735#mit-licence)
2. [Thanks](https://github.com/pu2clr/SI4735#thanks)
3. [Your support is important](https://github.com/pu2clr/SI4735#your-support-is-important)
4. [About the SI4735](https://github.com/pu2clr/SI4735#about-the-si4735)
5. [Terminology](https://github.com/pu2clr/SI4735#si4735-terminology)
6. [Labrary Features](https://github.com/pu2clr/SI4735#si4735-arduino-library-features)
7. [Library Installation](https://github.com/pu2clr/SI4735#library-installation)
   * [Installing via Arduino IDE](https://github.com/pu2clr/SI4735#installing-via-arduino-ide)
   * [Installing via this repository](https://github.com/pu2clr/SI4735#installing-via-this-repository) 
8. [Hardware Requirements and Setup](https://github.com/pu2clr/SI4735#hardware-requirements-and-setup)
   * [Schematic](https://github.com/pu2clr/SI4735#schematic)
   * [Component Parts](https://github.com/pu2clr/SI4735#parts)
   * [Photos](https://github.com/pu2clr/SI4735#photos)
9.  [API Documentation](https://github.com/pu2clr/SI4735#api-documentation)
   * [Defined Data Types and Structures](https://github.com/pu2clr/SI4735#defined-data-types-and-structures)
   * [__Public Methods__](https://github.com/pu2clr/SI4735#public-methods)
     * [__Usual methods__](https://github.com/pu2clr/SI4735#public-methods)
       * [setup](https://github.com/pu2clr/SI4735#setup)
       * [setPowerUp](https://github.com/pu2clr/SI4735#setpowerup)
       * [analogPowerUp](https://github.com/pu2clr/SI4735#analogpowerup)
       * [setFrequency](https://github.com/pu2clr/SI4735#setfrequency)
       * [frequencyUp](https://github.com/pu2clr/SI4735#frequencyup)
       * [frequencyDown](https://github.com/pu2clr/SI4735#frequencydown)
       * [isCurrentTuneFM](https://github.com/pu2clr/SI4735#iscurrenttunefm)
       * [seekStation](https://github.com/pu2clr/SI4735#seekstation)
       * [setAM](https://github.com/pu2clr/SI4735#setam)
       * [setFM](https://github.com/pu2clr/SI4735#setfm)
       * [setVolume](https://github.com/pu2clr/SI4735#setvolume)
       * [volumeUp](https://github.com/pu2clr/SI4735#volumeup)
       * [volumeDown](https://github.com/pu2clr/SI4735#volumedown)
       * [getCurrentVolume](https://github.com/pu2clr/SI4735#getcurrentvolume)
     * [__Si4735 current status__](https://github.com/pu2clr/SI4735#si4735-current-status)
       * [getStatus](https://github.com/pu2clr/SI4735#getstatus)
       * [getTuneCompleteTriggered](https://github.com/pu2clr/SI4735#gettunecompletetriggered)
       * [getSignalQualityInterrupt](https://github.com/pu2clr/SI4735#getsignalqualityinterrupt)
       * [getRadioDataSystemInterrupt](getRadioDataSystemInterrupt)
       * [getStatusError](https://github.com/pu2clr/SI4735#getstatuserror)
       * [getStatusCTS](https://github.com/pu2clr/SI4735#getstatuscts)
       * [getACFIndicator](https://github.com/pu2clr/SI4735#getacfindicator)
       * [getBandLimit](https://github.com/pu2clr/SI4735#getbandlimit)
       * [getReceivedSignalStrengthIndicator](https://github.com/pu2clr/SI4735#getreceivedsignalstrengthindicator)
       * [getStatusSNR](https://github.com/pu2clr/SI4735#getstatussnr)
       * [getStatusMULT](https://github.com/pu2clr/SI4735#getstatusmult)
       * [getAntennaTuningCapacitor](https://github.com/pu2clr/SI4735#getantennatuningcapacitor)
       * [getStatusValid](https://github.com/pu2clr/SI4735#getstatusvalid)
     * [__SI4735 Received Signal Quality__](https://github.com/pu2clr/SI4735#si4735-received-signal-quality)
       * [getCurrentReceivedSignalQuality](https://github.com/pu2clr/SI4735#getcurrentreceivedsignalquality)
       * [getCurrentRSSI](https://github.com/pu2clr/SI4735#getcurrentrssi)
       * [getCurrentSNR](https://github.com/pu2clr/SI4735#getcurrentsnr)
       * [getCurrentRssiDetectLow](https://github.com/pu2clr/SI4735#getcurrentrssidetectlow)
       * [getCurrentRssiDetectHigh](https://github.com/pu2clr/SI4735#getcurrentrssidetecthigh)
       * [getCurrentSnrDetectLow](https://github.com/pu2clr/SI4735#getcurrentsnrdetectlow)
       * [getCurrentSnrDetectHigh](https://github.com/pu2clr/SI4735#getcurrentsnrdetecthigh)
       * [getCurrentValidChannel](https://github.com/pu2clr/SI4735#getcurrentvalidchannel)
       * [getCurrentAfcRailIndicator](https://github.com/pu2clr/SI4735#getcurrentafcrailindicator)
       * [getCurrentSoftMuteIndicator](https://github.com/pu2clr/SI4735#getcurrentsoftmuteindicator)
       * [getCurrentStereoBlend](https://github.com/pu2clr/SI4735#getcurrentstereoblend)
       * [getCurrentPilot](https://github.com/pu2clr/SI4735#getcurrentpilot)
       * [getCurrentMultipath](https://github.com/pu2clr/SI4735#getcurrentmultipath)
       * [getCurrentSignedFrequencyOffset](https://github.com/pu2clr/SI4735#getcurrentsignedfrequencyoffset)
       * [getCurrentMultipathDetectLow](https://github.com/pu2clr/SI4735#getcurrentmultipathdetectlow)
       * [getCurrentMultipathDetectHigh](https://github.com/pu2clr/SI4735#getcurrentmultipathdetecthigh)
       * [getCurrentBlendDetectInterrupt](https://github.com/pu2clr/SI4735#getcurrentblenddetectinterrupt)
     * [__Current AGC Status__](https://github.com/pu2clr/SI4735#current-agc-status)
       * [getAutomaticGainControl](https://github.com/pu2clr/SI4735#getautomaticgaincontrol)
       * [isAgcEnabled](https://github.com/pu2clr/SI4735#isagcenabled)
       * [getAgcGainIndex](https://github.com/pu2clr/SI4735#getagcgainindex)
     * [__SI4735 Firmware Information__](https://github.com/pu2clr/SI4735#si4735-firmware-information)
       * [getFirmwarePN](https://github.com/pu2clr/SI4735#getfirmwarepn)
       * [getFirmwareFWMAJOR](https://github.com/pu2clr/SI4735#getfirmwarefwmajor)
       * [getFirmwareFWMINOR](https://github.com/pu2clr/SI4735#getfirmwarefwminor)
       * [getFirmwarePATCHH](https://github.com/pu2clr/SI4735#getfirmwarepatchh)
       * [getFirmwarePATCHL](https://github.com/pu2clr/SI4735#getfirmwarepatchl)
       * [getFirmwareCMPMAJOR](https://github.com/pu2clr/SI4735#getfirmwarecmpmajor)
       * [getFirmwareCMPMINOR](https://github.com/pu2clr/SI4735#getfirmwarecmpminor)
       * [getFirmwareCHIPREV](https://github.com/pu2clr/SI4735#getfirmwarechiprev)
     * [__RDS__](https://github.com/pu2clr/SI4735#rds)
       * [setRdsIntSource](https://github.com/pu2clr/SI4735#setrdsintsource)
       * [setRdsConfig](https://github.com/pu2clr/SI4735#setrdsconfig)
       * [getRdsStatus](https://github.com/pu2clr/SI4735#getrdsstatus)
       * [getRdsReceived](https://github.com/pu2clr/SI4735#getrdsreceived)
       * [getRdsSyncLost](https://github.com/pu2clr/SI4735#getrdssynclost)
       * [getRdsSyncFound](https://github.com/pu2clr/SI4735#getrdssyncfound)
       * [getRdsNewBlockA](https://github.com/pu2clr/SI4735#getrdsnewblocka)
       * [getRdsNewBlockB](https://github.com/pu2clr/SI4735#getrdsnewblockb)
       * [getRdsSync](https://github.com/pu2clr/SI4735#getrdssync)
       * [getGroupLost](https://github.com/pu2clr/SI4735#getgrouplost)
       * [getNumRdsFifoUsed](https://github.com/pu2clr/SI4735#getnumrdsfifoused)
       * [getRdsPI](https://github.com/pu2clr/SI4735#getrdspi)
       * [getRdsGroupType](https://github.com/pu2clr/SI4735#getrdsgrouptype)
       * [getRdsVersionCode](https://github.com/pu2clr/SI4735#getrdsversioncode)
       * [getRdsProgramType](https://github.com/pu2clr/SI4735#getrdsprogramtype)
       * [getRdsText](https://github.com/pu2clr/SI4735#getrdstext)
       * [getRdsTime](https://github.com/pu2clr/SI4735#getrdstime)
     * [__Single Side Band (SSB) Support__](https://github.com/pu2clr/SI4735#single-side-band-ssb-support)
10. [References](https://github.com/pu2clr/SI4735#references)
11. [Examples]()
12. [Videos]() 



## MIT License 

Copyright (c) 2019 Ricardo Lima Caratti

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE ARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



## Thanks

I would like to thank Mr. Francisco Scaramella  for the suggestions and contributions provided in the electronics field as well as for the testing of the functions implemented in this library.



## Your support is important.

If you would like to support this library development, consider joining this project via Github. Alternatively, make suggestions on features you would like available in this library. Thank you!




## About the SI4735  

The Si4735 is DSP radio receiver IC from Silcon Labs. It has great performance on AM (LW/MW/SW) and local FM station. The SI4735 can be programmed by using commands and responses. You can contron it via a microcontroller like Arduino. To make the SI4735 perform an action, the microccontroller have to send a set of bytes (command and arguments) tha the device interpret it and executes the given command.




### The main Si4735 features

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
* Great Programming Guide and additional documentation to deal with the device




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

This library uses the I2C communication protocol and implements most of the functions offered by Si4735 (BROADCAST AM / FM / SW / LW RADIO RECEIVER). 

1. Open Source 
2. Built Based on [Si47XX PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN332.pdf)
3. C++ Lenguage and Object-oriented programming
4. Available on Arduino IDE (Manage Libraries)
5. Simplifies projects based on SI4735
6. I2C communication



## Library Installation

You can install this library on your Arduino environment using different methods. The best ways to do that are described below.  

### Installing via Arduino IDE

This is the easiest method to install this library.

![Installing from Arduino IDE 01](extras/images/lib_install_01.png)


![Installing from Arduino IDE 02](extras/images/lib_install_02.png)



### Installing via this repository 

![Installing from this repository](extras/images/install_lib_from_git_01.png)


![Installing from this repository](extras/images/install_lib_from_git_02.png)


First, you have to download this library in zip format. [Click here](https://github.com/pu2clr/SI4735/archive/master.zip) to download.

Unzip the SI4735.zip file in your Arduino Library folder. 

* On __Windows__: "My Documents\Arduino\libraries"
* On __MAC OS__: ˜/Documents/Arduino/libraries
* On __Linux__: ˜/Documents/Arduino/libraries


<BR>
<BR>
<BR>
<BR>


## Hardware Requirements and Setup

This library has been written for the Arduino platform and has been successfully tested on Pro Mini. I beleave it will work on any other Arduino with I2C support.



### Arduino 5V and Si4735

The SI4735 device  works with  3.3V only. If you are not using a 3.3V version of Arduino, you have to use a kind of converter. 



### Schematic

The main porpuse of this prototype is to test de Si4735 Arduino Library. It does not intend to be a real radio for exigent listener. However, it is possible to start with it and after include some devices to improve, for example,  its sensibility,  beyond other desired features.

The image bellow shows a version of Slicon Labs SSOP Typical Application Schematic. The basic difference are the pull-up resitors on I2C bus. 

![Basic Schematic](./extras/images/basic_schematic.png)


<BR>
<BR>

#### The image bellow shows the Slicon Labs SSOP Typical Application Schematic.


![Silicon Labs Schematic](./extras/images/silicon_labs_schematic_pag_19.png)


<BR>
<BR>

### Parts

The table below shows the component parts used to build the radio prototype based on Si4735 and used the Slicon Labs SSOP Typical Application Schematic as main source. However, some parts were included by the author of this project. 


|Part	| Description |
|-------| ------------ |
| C1    | 22nF Monolithic Multilayer Chip Ceramic non polarized capacitor (Place it close to VA pin)|
| C2    | 100pF Monolithic Multilayer Chip Ceramic non polarized capacitor |
| C3    | 470nF Monolithic Multilayer Chip Ceramic non polarized capacitor| 
| C4    | 100nF Monolithic Multilayer Chip Ceramic non polarized capacitor (Place it close to VD pin)|
| C5 and C6 | 22pF (Crystal load capacitors) | 
| C7 and C8[ˆ1] | 4.7uF Monolithic Multilayer Chip Ceramic non polarized capacitor | 
| R3    | 2.2K |
| (R4 and R5)[ˆ2] | 10K (pull-up resistors) |
| L1 | Ferrite loop stick (about 500 μH) |
| X1    | 32.768 kHz crystal |

  * [ˆ1]: C7 and C8 are ceramic capacitors included by the author of this project. They are not present on original Silicon Labs schematic. 
  * [ˆ2]: R4 and R5 are pull-up resistor included by the author of this project. They are not present on original Silicon Labs schematic. 

__Notes from Silicon Labs Broadcast AM/FM/SW/LW Radio Receiver documentation (page 12)__:
* Place C1 close to VA and C4 close to VD pin.
* All grounds connect directly to GND plane on PCB.
* Pins 6 and 7 are no connects, leave floating.
* Pins 10 and 11 are unused. Tie these pins to GND.
* To ensure proper operation and receiver performance, follow the guidelines in “AN383: Si47xx Antenna, Schematic,
* Layout, and Design Guidelines.” Silicon Laboratories will evaluate schematics and layouts for qualified customers.
* Pin 8 connects to the FM antenna interface, and pin 12 connects to the AM antenna interface.
* Place Si473x-D60 as close as possible to antenna and keep the FMI and AMI traces as short as possible.



## Photos 

It was a bit hard to solder the kind of CI on adapter. However, by using a electronic magnifier it was possible.



### SI4735 soldered on adapter

|Si4735 on Adapter|Si4735 on Adapter| 
|---------------|---------------|  
|![SI4735 on adapter 01](./extras/images/si4735_on_adapter_01.png)|![SI4735 on adapter 03](./extras/images/si4735_on_adapter_03.png)|

|Si4735 on Adapter|Si4735 on Adapter| 
|---------------|---------------|   
|![SI4735 on adapter 04](./extras/images/adapter_01.png)|![SI4735 on adapter 05](./extras/images/adapter_02.png)|


#### Protoboard

The basic circuit built on protoboard is based on the “SSOP Typical Application Schematic”, suggested by the Silicon Labs Documentation (Si4730/31/34/35-D60-BROADCAST AM/FM/SW/LW RADIO RECEIVER; page 19). Two pull-up 10K resistors were added on I2C bus. 

![Protoboard 00](./extras/images/protoboard_00.png)



![Protoboard 01](./extras/images/protoboard_01.png)




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

<BR>
<BR>
<BR>


## Public Methods

To use the methods below you have to declare the class SI4735 in your sketch. The folder __examples__ has some examples that show how to use these methods. For example: see [Proof of Concept for SI4735 Arduino Library](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino) 

<BR>


### setup

```cpp
/* 
 * Starts the Si473X device. 
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte interruptPin interrupt Arduino Pin (see your Arduino pinout). If less than 0, iterrupt disabled
 * @param byte defaultFunction
 */
void SI4735::setup(byte resetPin, byte interruptPin, byte defaultFunction)
```

#### If you are not using interrupt use the __setup__ method as shown below.

```cpp
/* 
 * Starts the Si473X device.  
 * Use this setup if you are not using interrupt resource
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte defaultFunction
 */
void SI4735::setup(byte resetPin, byte defaultFunction)
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

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)



### setPowerUp

```cpp
/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * @param byte CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param byte GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param byte PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param byte XOSCEN byte XOSCEN set external Crystal enabled or disabled 
 * @param byte FUNC sets the receiver function have to be used [0 = FM Receive; 1 = AM (LW/MW/SW) and SSB (if SSB patch apllied)]
 * @param byte OPMODE set the kind of audio mode you want to use.
 */
void SI4735::setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE)
```

### analogPowerUp


```cpp
/*
 * Powerup in Analog Mode
 * You have to call setPowerUp before call analogPowerUp. 
 * Use setPowerUp to select FM, AM or SSB (if SSB patch apllied) mode.
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

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)


### frequencyUp
```cpp
/*
 *  Increments the current frequency on current band/function by using the current step.
 *  See setFrequencyStep
 */ 
void SI4735::frequencyUp()
```


### frequencyDown
```cpp
/*
 *  Decrements the current frequency on current band/function by using the current step.
 *  See setFrequencyStep
 */ 
void SI4735::frequencyDown()
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

#### seekStationUp and seekStationDown

```cpp
/*
 * Search for the next station 
 */
void SI4735::seekStationUp() 

/*
 * Search the previous station
 */
void SI4735::seekStationDown()
```

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)



### setAM

```cpp
/*
 * Set the radio to AM function. It means: LW, MW and SW.
 */ 
void SI4735::setAM()
```

##### You can also use __setAM__ with parameters as shown below

```cpp
/*
 * Set the radio to AM (LW/MW/SW) function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency 
 * @param step step used to go to the next channel   
 */
void SI4735::setAM(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step)
```


### setFM

```cpp
/*
 * Set the radio to FM function
 */
void SI4735::setFM()
```

##### You can also use __setFM__ with parameters as shown below

```cpp
/*
 * Set the radio to FM function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency (default frequency)
 * @param step step used to go to the next channel   
 */
void SI4735::setFM(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step)
```


#### Example of using setAM() and setFM()

```cpp
    switch (key)
    {
    case 'A':
      si4735.setAM();
      si4735.setFrequency(am_freq);
      break;
    case 'F':
      si4735.setFM();
      si4735.setFrequency(fm_freq);
      break;
      .
      .
      .
```

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)


#### isCurrentTuneFM

```cpp 
/*
 * Returns true if the radio is running FM (FM_TUNE_FREQ).
 */ 
bool SI4735::isCurrentTuneFM()
```



### setVolume

```cpp
/* 
 * Set the volume level
 * @param byte volume (domain: 0 - 63) 
 */
void SI4735::setVolume(byte volume)
```

#### Example of using setVolume()

```cpp
  si4735.setVolume(45);
```

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)



### volumeUp

```cpp
/*
 *  Set sound volume level Up   
 */
void SI4735::volumeUp()
```


### volumeDown
```cpp
/*
 *  Set sound volume level Down   
 */
void SI4735::volumeDown()
```

#### Example of using volumeUp() and volumeDown()

```cpp
    switch (key)
    {
    case '+':
        si4735.volumeUp();
        break;
    case '-': 
        si4735.volumeDown();
        break;
    .
    .
    .    
```

[See full example](https://github.com/pu2clr/SI4735/blob/master/examples/SI4735_POC/SI4735_POC.ino)


### getCurrentVolume

```cpp 
/*
 * Returns the current volume level
 */ 
inline byte SI4735::getCurrentVolume() 
```


<BR>
<BR>
<BR>

## SI4735 Current Status

Allows to query the current frequency, RSSI, SNR, multipath, and the antenna tuning capacitance value (0-191). 
You have to call getStatus before any method to get the information about SI4735 status

<BR>


### getStatus

```cpp
/*
 * Gets the current status  of the Si4735 (AM or FM)
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 *
 */
void SI4735::getStatus()


/*
 * Gets the current status  of the Si4735 (AM or FM)
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 * 
 * @param byte INTACK Seek/Tune Interrupt Clear. If set, clears the seek/tune complete interrupt status indicator;
 * @param byte CANCEL Cancel seek. If set, aborts a seek currently in progress;
 * 
 */
void SI4735::getStatus(byte INTACK, byte CANCEL) {
```


### getTuneCompleteTriggered

```cpp
/*
 * Tune complete has been triggered (STCINT)
 */
inline bool SI4735::getTuneCompleteTriggered()
```


### getSignalQualityInterrup

```cpp
/* 
 * Gets Received Signal Quality Interrupt(RSQINT)
 * 
 */
inline bool SI4735::getSignalQualityInterrup()
```


### getRadioDataSystemInterrupt

```cpp
/* 
 * Gets Radio Data System (RDS) Interrupt
 */
inline bool SI4735::getRadioDataSystemInterrupt()
```


### getStatusError

```cpp
/*
 * Return the Error flag (true or false) of status of the least Tune or Seek 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 * @return true or false
 */
inline bool SI4735::getStatusError()
```



### getStatusCTS

```cpp
/*
 * Gets the Error flag of status response 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 */
inline bool SI4735::getStatusCTS()
```


### getACFIndicator

```cpp
/* 
 * Returns true if the AFC rails (AFC Rail Indicator). 
 */
inline bool SI4735::getACFIndicator()
```



### getBandLimit


```cpp
/* 
 * Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or 
 * wrapped to the original frequency (WRAP = 1).
 */
inline bool SI4735::getBandLimit()
```



### getReceivedSignalStrengthIndicator
```cpp
/*
 * Received Signal Strength Indicator.
 * This byte contains the receive signal strength when tune is complete (dBμV).
 */
inline byte SI4735::getReceivedSignalStrengthIndicator()
```

### getStatusSNR

```cpp
/*
 * SNR.
 * This byte contains the SNR metric when tune is complete (dB).
 */
inline byte SI4735::getStatusSNR()
```

### getStatusMULT

```cpp
/* 
 * Multipath.
 * This byte contains the multipath metric when tune is complete. 
 * Multipath indi- cator is available only for Si474x, Si4706-C30 and later and 
 * Si4704/05/30/31/34/35/84/85-D50 and later.
 */
inline byte SI4735::getStatusMULT()
```


### getAntennaTuningCapacitor

```cpp
/* 
 * Read Antenna Tuning Capacitor (Si4704/05/06/2x only). 
 * Returns a byte that contains the current antenna tuning capacitor value.
 */
inline byte SI4735::getAntennaTuningCapacitor()
```


### getStatusValid

```cpp
/*
 * Returns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108) 
 * and would have been found during a Seek.
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 */
inline bool SI4735::getStatusValid()
```

<BR>
<BR>
<BR>

## SI4735 Received Signal Quality

You have a set o methods that allowto get some information about Received Signal Quality.
See  Si47XX PROGRAMMING GUIDE; AN332; pages 75 and 141


### getCurrentReceivedSignalQuality

```cpp
/*  
 * Queries the status of the Received Signal Quality (RSQ) of the current channel. The methods getCurrentRSSI(), getCurrentSNR() etc,
 * depend on this method. So, it have to be called first. However, this method is called internally by getFrequency(). In this case,
 * you do not need to use getCurrentReceivedSignalQuality if you are using getFrequency.
 * In other words, you can call getCurrentRSSI(), getCurrentSNR() etc, after call getFrequency().
 * 
 * @param INTACK Interrupt Acknowledge; 0 = Interrupt status preserved; 1 = Clears RSQINT, SNRHINT, SNRLINT, RSSIHINT, RSSILINT
 */
void getCurrentReceivedSignalQuality(byte INTACK)
```

### getCurrentRSSI

```cpp
/* 
 * Gets current receive signal strength (0–127 dBμV).
 * AM and FM
 */
inline byte SI4735::getCurrentRSSI() 
```

### getCurrentSNR

```cpp
/*
 * Gets current SNR metric (0–127 dB).
 * AM and FM
 */
inline byte SI4735::getCurrentSNR() 
```

### getCurrentRssiDetectLow

```cpp
/* 
 * Returns true if RSSI Detect Low.
 * AM and FM
 */ 
inline bool SI4735::getCurrentRssiDetectLow() 
```  


### getCurrentRssiDetectHigh

```cpp
/* 
 * Return true if RSSI Detect High
 * AM and FM
 */ 
inline bool SI4735::getCurrentRssiDetectHigh() 
```

### getCurrentSnrDetectLow

```cpp
/* 
 * Returns true if SNR Detect Low.
 */ 
inline bool SI4735::getCurrentSnrDetectLow() 
```  

### getCurrentSnrDetectHigh

```cpp
/* 
 * Returns true if SNR Detect High
 */ 
inline bool SI4735::getCurrentSnrDetectHigh() 
```    

### getCurrentValidChannel

```cpp
/*
 *  Returns true if Valid Channel.
 */ 
inline bool SI4735::getCurrentValidChannel() 
```  

### getCurrentAfcRailIndicator

```cpp
/*
 *  Returns true AFC Rail Indicator.
 */ 
inline bool SI4735::getCurrentAfcRailIndicator() 
```    

### getCurrentSoftMuteIndicator

```cpp
/*
 * Returns true if soft mute is engaged.
 */
inline bool SI4735::getCurrentSoftMuteIndicator() 
```    

### getCurrentStereoBlend

```cpp
/* 
 *  Returns the amount of stereo blend in% (100 = full stereo, 0 = full mono).
 */ 
inline byte SI4735::getCurrentStereoBlend()
```    

### getCurrentPilot

```cpp
/*
 * Returns true if stereo pilot presence.
 */ 
inline bool SI4735::getCurrentPilot() 
```    

### getCurrentMultipath

```cpp
/* 
 * Returns the current multipath metric. (0 = no multipath; 100 = full multipath)
 */ 
inline byte SI4735::getCurrentMultipath() 
``` 

### getCurrentSignedFrequencyOffset

```cpp
/* 
 * Returns Signed frequency offset (kHz).
 */ 
inline byte SI4735::getCurrentSignedFrequencyOffset() 
```  

### getCurrentMultipathDetectLow

```cpp
/* 
 * Returns true if Multipath Detect Low.
 */ 
inline bool SI4735::getCurrentMultipathDetectLow()  
```   

### getCurrentMultipathDetectHigh

```cpp
/*
 * Returns true if Multipath Detect High
 */ 
inline bool SI4735::getCurrentMultipathDetectHigh()   
```

### getCurrentBlendDetectInterrupt

```cpp
/*
 * Returns true if Blend Detect Interrupt
 */ 
inline bool SI4735::getCurrentBlendDetectInterrupt()  
```

<BR>
<BR>
<BR>

## __Current AGC Status__

 Methods to query AGC status.  Returns whether the AGC is enabled or disabled and it returns the gain index. You have to call __getAutomaticGainControl__ before isAgcEnabled and getAgcGainIndex.


### getAutomaticGainControl

```cpp
/*
 * Queries AGC STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 80; for AM page 142.
 * See AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18. 
 * After call this method, you can call isAgcEnabled to know the AGC status and getAgcGainIndex to know the gain index value.
 */
void SI4735::getAutomaticGainControl()
```

### isAgcEnabled

```cpp
/* 
 *  Returns true if the AGC is enabled
 */ 
inline bool isAgcEnabled()
```


### getAgcGainIndex

```cpp
/* 
 *  Returns the current AGC gain index.
 */ 
inline byte getAgcGainIndex() 
```


<BR>
<BR>
<BR>

## SI4735 Firmware Information

Allows to query the part number, chip revision, firmware revision, patch revision and component revision numbers.

<BR>


### getFirmwarePN

```cpp
/*
 * Returns the final 2 digits of Part Number (HEX)
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwarePN()
```


### getFirmwareFWMAJOR

```cpp
/*
 * Returns the Firmware Major Revision (ASCII).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwareFWMAJOR()
```


### getFirmwareFWMINOR

```cpp
/*
 * Returns the Firmware Minor Revision (ASCII).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwareFWMINOR()
```


### getFirmwarePATCHH

```cpp
/*
 * Returns the Patch ID High Byte (HEX).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwarePATCHH()
```

### getFirmwarePATCHL

```cpp
/*
 * Returns the Patch ID Low Byte (HEX).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwarePATCHL()
```

### getFirmwareCMPMAJOR

```cpp
/*
 * Returns the Component Major Revision (ASCII).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwareCMPMAJOR()
```

### getFirmwareCMPMINOR


```cpp
/*
 * Returns the Component Minor Revision (ASCII).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66.
 */
inline byte SI4735::getFirmwareCMPMINOR()
```


### getFirmwareCHIPREV

```cpp
/*
 * Returns the Chip Revision (ASCII).
 * See Si47XX PROGRAMMING GUIDE; AN332; page 66
 */
inline byte SI4735::getFirmwareCHIPREV()
```


<BR>
<BR>
<BR>


## RDS

    This library implements some RDS features of the SI4735.


### setRdsIntSource

```cpp
/* 
 * Configures interrupt related to RDS
 * Use this method if want to use interrupt
 * See Si47XX PROGRAMMING GUIDE; AN332; page 103
 * 
 * @param RDSRECV If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
 * @param RDSSYNCLOST If set, generate RDSINT when RDS loses synchronization.
 * @param RDSSYNCFOUND set, generate RDSINT when RDS gains synchronization.
 * @param RDSNEWBLOCKA If set, generate an interrupt when Block A data is found or subsequently changed
 * @param RDSNEWBLOCKB If set, generate an interrupt when Block B data is found or subsequently changed
 */
void SI4735::setRdsIntSource(byte RDSNEWBLOCKB, byte RDSNEWBLOCKA, byte RDSSYNCFOUND, byte RDSSYNCLOST, byte RDSRECV)
```

### setRdsConfig

```cpp
/*
 * Set RDS property 
 * 
 * @param byte RDSEN RDS Processing Enable; 1 = RDS processing enabled.
 * @param byte BLETHA Block Error Threshold BLOCKA.   
 * @param byte BLETHB Block Error Threshold BLOCKB.  
 * @param byte BLETHC Block Error Threshold BLOCKC.  
 * @param byte BLETHD Block Error Threshold BLOCKD. 
 *  
 * IMPORTANT: 
 * All block errors must be less than or equal the associated block error threshold 
 * for the group to be stored in the RDS FIFO. 
 * 0 = No errors.
 * 1 = 1–2 bit errors detected and corrected. 
 * 2 = 3–5 bit errors detected and corrected. 
 * 3 = Uncorrectable.
 * Recommended Block Error Threshold options:
 *  2,2,2,2 = No group stored if any errors are uncorrected.
 *  3,3,3,3 = Group stored regardless of errors.
 *  0,0,0,0 = No group stored containing corrected or uncorrected errors.
 *  3,2,3,3 = Group stored with corrected errors on B, regardless of errors on A, C, or D.
 */
void SI4735::setRdsConfig(byte RDSEN, byte BLETHA, byte BLETHB, byte BLETHC, byte BLETHD)
```


### getRdsStatus

```cpp
/*
 * RDS COMMAND FM_RDS_STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78
 * @param INTACK Interrupt Acknowledge; 0 = RDSINT status preserved. 1 = Clears RDSINT.
 * @param MTFIFO 0 = If FIFO not empty, read and remove oldest FIFO entry; 1 = Clear RDS Receive FIFO.
 * @param STATUSONLY Determines if data should be removed from the RDS FIFO.
 */
void SI4735::getRdsStatus(byte INTACK, byte MTFIFO, byte STATUSONLY)
```

##### You can also use __getRdsStatus__ with no parameters as shown below

```cpp
/*
 * Gets RDS Status.
 * Call getRdsStatus(byte INTACK, byte MTFIFO, byte STATUSONLY) if you want other behaviour
 * same getRdsStatus(0,0,0)
 */
void SI4735::getRdsStatus() 
```


### getRdsReceived

```cpp
/*
 * Returns true if the number of the groups is filled.
 * (1 = FIFO filled to minimum number of groups)
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsReceived()       
```


### getRdsSyncLost

```cpp
/* 
 * Returns true if when the RDS synchronization status is lost (1 = Lost RDS synchronization)
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsSyncLost()  
```

### getRdsSyncFound

```cpp
/* 
 * Returns true when RDS synchronization status is found (Found RDS synchronization)
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsSyncFound() 
```

### getRdsNewBlockA

```cpp
/* 
 * Returns true when a valid Block A data has been received.
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsNewBlockA() 
```

### getRdsNewBlockB

```cpp
/* 
 * Returns true when a valid Block B data has been received.
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsNewBlockB() 
```

### getRdsSync

```cpp
/* 
 * Returns true when RDS currently synchronized.
 * You have to call getRdsStatus before. 
 */ 
inline bool getRdsSync() 
```


### getGroupLost

```cpp
/*
 * Returns true when One or more RDS groups discarded due to FIFO overrun.
 * You have to call getRdsStatus before. 
 */ 
 inline bool getGroupLost()          
 ```


### getNumRdsFifoUsed

```cpp
/*
 * Returns the number of groups remaining in the RDS FIFO (0 if empty).
 * You have to call getRdsStatus before.   
 */
inline byte getNumRdsFifoUsed() 
```

### getRdsPI

```cpp
/* 
 * Returns the programa type. 
 * Read the Block A content
 */  
unsigned SI4735::getRdsPI(void) 
```

### getRdsGroupType

```cpp
/*
 * Returns the Group Type (extracted from the Block B) 
 */ 
unsigned SI4735::getRdsGroupType(void)
```

### getRdsVersionCode

```cpp
/*
 * Gets the version code (extracted from the Block B)
 * Returns  0=A or 1=B
 */
unsigned SI4735::getRdsVersionCode(void)
```

### getRdsProgramType

```cpp
/* 
 * Returns the Program Type (extracted from the Block B)
 */ 
unsigned SI4735::getRdsProgramType(void)
```


### getRdsText

```cpp
/*
 * Gets the RDS Text when the message is of the Group Type 2 version A
 */  
String SI4735::getRdsText(void)
```

### getRdsTime

```cpp
/* 
 * Gets the RDS time and date when the Group type is 4 
 */  
String SI4735::getRdsTime()
```

<BR>
<BR>
<BR>

## Single Side Band (SSB) Support

This feature will work only on SI4735-D60. 
To use this feature, you have to aplya a specific AM SSB patch.
 
 Feature and documentation under construction....


<BR>
<BR>
<BR>

## References

1. [Silicon Labs Si4737 WB/AM/FM Stereo/RDS single-chip receiver HAL library for Arduino ](https://github.com/rickeywang/Si4737_i2c)
2. [BROADCAST AM/FM/SW/LW RADIO RECEIVER](https://www.silabs.com/documents/public/data-sheets/Si4730-31-34-35-D60.pdf)
3. [SI47XX PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN332.pdf)
4. [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries#toc3)
5. [Specification of the radio data system (RDS) for VHF/FM sound broadcasting in the frequency range from 87,5 to 108,0 MHz](http://www.interactive-radio-system.com/docs/EN50067_RDS_Standard.pdf)
6. [Radio Data System](https://en.wikipedia.org/wiki/Radio_Data_System)
7. [RDS Encoder](https://haddadi.github.io/papers/RDSencoderHaddadi.pdf)
8. [RDS in Europe, RBDS in the USA –What are the differences and how canreceivers cope with both systems?](https://sigidwiki.com/images/a/ad/RDS_in_Europe,_RBDS_in_the_USA.pdf)
9. [RBDS & RDS PTY Codes and Program Types](https://www.fmsystems-inc.com/rbds-rds-pty-codes-program-types/)
10. [Using RDS/RBDS with the Si4701/03](https://www.silabs.com/documents/public/application-notes/AN243.pdf)



## Videos

![SI4735 and Arduino - First Steps](https://i9.ytimg.com/vi/i77U8WHXc18/mq3.jpg?sqp=CI3Yiu4F&rs=AOn4CLAjP37P9lB0h9XXbmP_8T-pGBvWVw)
[CLick here](https://youtu.be/i77U8WHXc18)
