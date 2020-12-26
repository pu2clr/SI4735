# Examples with TFT

This folder has some examples of using the TFT. If you want to use the touch screen version, I recommend reading the touch calibration process carefully. See [SI47XX_02_TFT_TOUCH_SHIELD](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_04_TFT#si47xx_02_tft_touch_shield) below. 


__Folder [SI47XX_10_RDS](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_10_RDS) has almost the same examples found here. The main difference is the RDS support. Also, SI47XX_10_RDS is updated more frequently. I recomend you to use the [SI47XX_10_RDS folder](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_10_RDS) instead__.   


![Si4735 and TFT Projects](https://github.com/pu2clr/SI4735/blob/master/extras/images/SI4735_TFT_PROJECTS.png)

<BR> 

Videos:
* [Si4735 All in One Receiver running on Arduino Pro Mini 3.3V (8MHz) and TFT Display](https://youtu.be/oL8qyRglZ8I)
* [Si4735 All in One Receiver with TFT Touch Screen](https://youtu.be/Zs59ThVumuE)


## SI47XX_01_TFT_ILI9225 

This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a [SPI TFT from MICROYUM (2" - 176 x 220)](https://github.com/Nkawu/TFT_22_ILI9225/wiki) based on ILI9225 driver.  The Arduino library used to control that display device is MCUFRIEND_kbv. Please, install it before start working with this sketch.  

It is also a complete radio capable to tune LW, MW, SW on AM and SSB mode and also receive the regular commercial stations.  It is important to know the SSB support works on SI4735-D60 and SI4732-A10 devices. 

1. Encoder to tune stations;
2. Band selection via push buttons
3. AM, FM and SSB;
4. LW, MW and SW;
5. Bandwidth filter;
6. BFO Control; 
7. VFO/BFO switching via encoder push button;
8. 12 SW bands + one from (1.7MHz to 30MHz). 
9. Frequency step switch (1, 5, 10, 100 and 500kHz kHz);
  

If you are using the same circuit used on examples with OLED and LCD, you have to change some buttons wire up. This TFT device takes five pins from Arduino. For this reason, it is necessary change the pins of some buttons. Fortunately, you can use the ATmega328 analog pins as digital pins. The table below.   


### Wire up and functions

The table below show the pins wire up for this example on Arduino Pro Mini.

| Device name               | Device Pin / Description  |  Arduino Pin  |
| ----------------          | --------------------      | ------------  |
| __Display TFT__           |                           |               |                    
|                           | RST (RESET)               |      8        |  
|                           | RS  or DC  or A0          |      9        |
|                           | CS  or SS                 |     10        |
|                           | SDA orSDI or MOSI         |     11        | 
|                           | CLK                       |     13        | 
|                           | BL (LED)                  |    +VCC       |
| __Si4735__                |                           |               |
|                           | RESET (pin 15)            |     12        |
|                           | SDIO (pin 18)             |     A4        |
|                           | SCLK (pin 17)             |     A5        |
| __Buttons__               |                           |               | 
|                           | Switch MODE (AM/LSB/AM)   |      4        |
|                           | Banddwith                 |      5        | 
|                           | Next band                 |      6        |
|                           | Previous band             |      7        |
|                           | AGC ON/OF                 |     14 / A0   |
|                           | Frequency Step            |     15 / A1   | 
|                           | VFO/VFO Switch            |     16 / A2   |
| __Encoder__               |                           |               |
|                           | A                         |       2       |
|                           | B                         |       3       |



## SI47XX_02_TFT_TOUCH_SHIELD

This sketch uses the TFT Touch Shield (2.4") from mcufriend. You can use it on __Mega2560__ or __Arduino DUE__. See the wire up below. 


### Touch screen calibration process

The Arduino library used to control the TFT shield from mcufriend or equivalent, is the MCUFRIEND_kbv. Please, install this library before start working with SI47XX_02_TFT_TOUCH_SHIELD sketch.

All toutch screen needs to be calibrated to work properly. To do that, use the __TouchScreen_Calibr_native.ino__ that comes with MCUFRIEND_kbv library. Read the TouchScreen_Calibr_native.ino and check the XP, XM , YP and YM pins configuration. You might need to change the XP, XM , YP and YM values in the TouchScreen_Calibr_native.ino depending on the display you are using.  In the __TouchScreen_Calibr_native.ino__ sketch, check the corresponding code lines as shown below.


```cpp
// MCUFRIEND UNO shield shares pins with the TFT.
#if defined(ESP32)
int XP = 27, YP = 4, XM = 15, YM = 14;  //most common configuration
#else
// int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
int XP = 7, YP = A2, XM = A1, YM = 6;  //most common configuration
#endif
//#include <TouchScreen.h>         //Adafruit Library
//TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
//TSPoint tp;                            //global point
#include "TouchScreen_kbv.h"         //my hacked version
TouchScreen_kbv ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint_kbv tp;                            //global point
```

__In some TFT devices, it is necessary to change the XP, YP, XM and YM pins setup__.

Follow the instructions provided by the calibration sketch. During the calibration process, the __TouchScreen_Calibr_native.ino__ will give you, via Serial Monitor, the information like shown below. 

cx=184 cy=874 cz=291 LEFT, BOT, Pressure
cx=494 cy=187 cz=595 MIDW, TOP, Pressure
cx=497 cy=872 cz=367 MIDW, BOT, Pressure
cx=800 cy=180 cz=671 RT, TOP, Pressure
cx=802 cy=524 cz=589 RT, MIDH, Pressure
cx=809 cy=865 cz=515 RT, BOT, Pressure
MCUFRIEND_kbv ID=0x2053  240 x 320

```cpp
const int XP=7,XM=A1,YP=A2,YM=6; //240x320 ID=0x2053
const int TS_LEFT=155,TS_RT=831,TS_TOP=158,TS_BOT=892;
```

PORTRAIT CALIBRATION     240 x 320
x = map(p.x, LEFT=155, RT=831, 0, 240)
y = map(p.y, TOP=158, BOT=892, 0, 320)
Touch Pin Wiring XP=7 XM=A1 YP=A2 YM=6
LANDSCAPE CALIBRATION    320 x 240
x = map(p.y, LEFT=158, RT=892, 0, 320)
y = map(p.x, TOP=831, BOT=155, 0, 240)

Finally, you must copy and paste the two lines highlighted above to the SI47XX_02_TFT_TOUCH_SHIELD sketch. The following code illustrates this action.

```cpp
MCUFRIEND_kbv tft;
SI4735 si4735;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP=7,XM=A1,YP=A2,YM=6; //240x320 ID=0x2053
const int TS_LEFT=155,TS_RT=831,TS_TOP=158,TS_BOT=892;
```

See more on [MCUFRIEND_kbv documentation](https://github.com/prenticedavid/MCUFRIEND_kbv/blob/master/extras/mcufriend_how_to.txt)

The video below can help you to to setup your display. 

1. [Setting up 2.4 Inch TFT LCD Arduino Shield ILI9341 HX8347 from Banggood](https://youtu.be/mGevO10I-pU)


### Features:

1. Encoder to tune stations;
2. Band selection via touch screen;
3. Direct Mode selection via toucn screen - FM, AM (MW and SW) and SSB (LSB and USB);
4. Bandwidth filter;
5. BFO Control; 
6. VFO/BFO switching via encoder button
7. Frequency step switch (1, 5 and 10kHz);
8. Volume control 
9. Audio mute; 


### Wire up


  |Device name      | Function                |  MEGA/DUE Pin     |
  |-----------      | ----------------------  |  -------------    |
  | __Si4735__      |                         |                   |
  | pin 15          |   RESET                 |   22              |  
  | pin 18          |   SDIO                  |   20 (SDA)        |
  | pin 17          |   SCLK                  |   21 (SCL)        |
  | __Encoder__     |                         |                   |
  | A               |                         |   18              |
  | B               |                         |   19              |
  | BUTTON          |  ENCODER PUSH BUTTON    |   23              |  


It is important to know that any Arduino DUE digital pin can be configured as interrupt. 


## SI47XX_03_TFT_ATMEGA328_ST7735

This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a SPI TFT ST7735 1.8"
The  purpose  of  this  example  is  to  demonstrate a prototype  receiver based  on  the  SI4735  and  the  "PU2CLR SI4735 Arduino Library" working with the TFT ST7735 display. It is not the purpose of this prototype to provide you a beautiful interface. To be honest, I think you can do it better than me. 

It is  a  complete  radio  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the regular comercial  stations. If  you  are  using  the  same  circuit  used  on  examples with OLED and LCD,  you have to change some buttons wire up. This TFT device takes five pins from Arduino.  For this reason, it is necessary change the pins of some buttons. Fortunately, you can use the ATmega328 analog pins as digital pins.

The libraries Adafruit_GFX and Adafruit_ST7735 take a lot of memory space from Arduino. 
You have few space to improve your prototype with standard Arduino Pro Mini.
However, you can use some approaches:  
1. Shrink or remove the bootloader from Arduino Pro Mini;
2. The Arduino Nano e Uno has smaller bootloader than the Arduino Pro Mini
3. Port this sketch to a bigger board like Arduino Mega or DUE. 


Wire up on Arduino UNO, Pro mini

| Device name               | Device Pin / Description      |  Arduino Pin  |
| ----------------          | ----------------------------- | ------------  |
| Display TFT               |                               |               |
|                           | RST (RESET)                   |     (*3) 8    |
|                           | RS or DC or A0                |     (*3) 9    |
|                           | CS or SS                      |     10        |
|                           | SDI or SDA or MOSI            |     11        |
|                           | CLK                           |     13        |
|                           | BL (LED)                      |    +VCC       |  
|     Si4735                |                               |               |
|                           | (*4) RESET (pin 15)           |     12        |
|                           | (*4) SDIO (pin 18)            |     A4        |
|                           | (*4) SCLK (pin 17)            |     A5        |
|                           | (*5) SEN (pin 16)             |    GND        |    
|     Buttons               |                               |               |
|                           | (*1)Switch MODE (AM/LSB/AM)   |      4        |
|                           | (*1)Banddwith                 |      5        |
|                           | (*1)BAND                      |      6        |
|                           | (*2)SEEK                      |      7        |
|                           | (*1)AGC/Attenuation           |     14 / A0   |
|                           | (*1)STEP                      |     15 / A1   | 
|                           | VFO/VFO Switch (Encoder)      |     16 / A2   |
|    Encoder                |                               |               |
|                           | A                             |       2       |
|                           | B                             |       3       |

(*1) You have to press the push button and after, rotate the encoder to select the parameter. After you activate a command by pressing a push button, it will keep active for 2,5 seconds. 
(*2) The SEEK direction is based on the last movement of the encoder. If the last movement of the encoder was clockwise, the SEEK will be towards the upper limit. If the last movement of the encoder was ounterclockwise, the SEEK direction will be towards the lower limit.  
(*3) You might need to switch from 8 to 9  depending of your ST7735 device     
(*4) If you are using the SI4732-A10, check the corresponding pin numbers.  
(*5) If you are using the SI4735-D60, connect the SEN pin to the ground; 
     If you are using the SI4732-A10, connect the SEN pin to the +Vcc. 



