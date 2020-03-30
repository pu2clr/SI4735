# Examples with TFT

This folder has some examples of using the TFT. 


![Si4735 and TFT Projects](https://github.com/pu2clr/SI4735/blob/master/extras/images/SI4735_TFT_PROJECTS.png)

<BR> 

Videos:
* [Si4735 All in One Receiver running on Arduino Pro Mini 3.3V (8MHz) and TFT Display](https://youtu.be/oL8qyRglZ8I)
* [Si4735 All in One Receiver with TFT Touch Screen](https://youtu.be/Zs59ThVumuE)


## SI47XX_01_TFT_ILI9225 

This sketch uses an Arduino Pro Mini, 3.3V (8MZ) with a [SPI TFT from MICROYUM (2" - 176 x 220)](https://github.com/Nkawu/TFT_22_ILI9225/wiki) based on ILI9225 driver. It is also a complete radio capable to tune LW, MW, SW on AM and SSB mode and also receive the regular commercial stations. 

1. Encoder to tune stations;
2. Band selection via push buttons
3. AM, FM and SSB;
4. LW, MW and SW;
5. Bandwidth filter;
6. BFO Control; 
7. VFO/BFO switching via encoder push button;
8. 12 SW bands + one from (1.7MHz to 30MHz). 
9. Frequency step switch (1, 5, 10, 100 and 500KHz KHz);


If you are using the same circuit used on examples with OLED and LCD, you have to change some buttons wire up. This TFT device takes five pins from Arduino. For this reason, it is necessary change the pins of some buttons. Fortunately, you can use the ATmega328 analog pins as digital pins. The table below.   


### Wire up and functions

The table below show the pins wire up for this example on Arduino Pro Mini.

| Device name               | Device Pin / Description  |  Arduino Pin  |
| ----------------          | --------------------      | ------------  |
| __Display TFT__           |                           |               |                    
|                           | RST (RESET)               |      8        |  
|                           | RS                        |      9        |
|                           | CS                        |     10        |
|                           | SDI                       |     11        | 
|                           | CLK                       |     13        | 
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
|                           | VFO/VFO Switch            |     16 / A3   |
| __Encoder__               |                           |               |
|                           | A                         |       2       |
|                           | B                         |       3       |



## SI47XX_02_TFT_TOUCH_SHIELD

This sketch uses the TFT Touch Shield (2.4") from mcufriend. You can use it on Mega2560 and Arduino DUE. See the table bellow to right wire up the Mega/DUE to the devices (Si4735 and encoder). 

It is important to know that any digital pin on Arduino DUE can be configured as interrupt. 

### Features:

1. Encoder to tune stations;
2. Band selection via touch screen;
3. Direct Mode selection via toucn screen - FM, AM (MW and SW) and SSB (LSB and USB);
4. Bandwidth filter;
5. BFO Control; 
6. VFO/BFO switching via encoder button
7. Frequency step switch (1, 5 and 10KHz);
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



