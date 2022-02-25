# Arduino / ATmega328 with Nokia 5110

The schematic below shows the Arduino board based on ATmega 328 and the Nokia 5110 display

![Nokia 5110 schematic](../images/schematic_basic_Nokia5110.png)

Sketches on [SI47XX_08_TM1638](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_09_NOKIA_5110)


## Wire up on Arduino UNO, Pro mini and SI4735-D60


| Device name               | Device Pin / Description      |  Arduino Pin  |
| ----------------          | ----------------------------- | ------------  |
| Display NOKIA             |                               |               |
|                           | (1) RST (RESET)               |     8         |
|                           | (2) CE or CS                  |     9         |
|                           | (3) DC or DO                  |    10         |
|                           | (4) DIN or DI or MOSI         |    11         |
|                           | (5) CLK                       |    13         |
|                           | (6) VCC  (3V-5V)              |    +VCC       |
|                           | (7) BL/DL/LIGHT               |    +VCC       |
|                           | (8) GND                       |    GND        |
|     Si4735                |                               |               |
|                           | (*3) RESET (pin 15)           |     12        |
|                           | (*3) SDIO (pin 18)            |     A4        |
|                           | (*3) SCLK (pin 17)            |     A5        |
|                           | (*4) SEN (pin 16)             |    GND        |
|     Buttons               |                               |               |
|                           | (*1)Switch MODE (AM/LSB/AM)   |      4        |
|                           | (*1)Banddwith                 |      5        |
|                           | (*1)BAND                      |      6        |
|                           | (*2)SEEK                      |      7        |
|                           | (*1)AGC/Attenuation           |     14 / A0   |
|                           | (*1)STEP                      |     15 / A1   |
|                           | VFO/BFO Switch (Encoder)      |     16 / A2   |
|    Encoder                |                               |               |
|                           | A                             |       2       |
|                           | B                             |       3       |

  (*1) You have to press the push button and after, rotate the encoder to select the parameter.
       After you activate a command by pressing a push button, it will keep active for 2,5 seconds.
  (*2) The SEEK direction is based on the last movement of the encoder. If the last movement of
       the encoder was clockwise, the SEEK will be towards the upper limit. If the last movement of
       the encoder was counterclockwise, the SEEK direction will be towards the lower limit.
  (*3) - If you are using the SI4732-A10, check the corresponding pin numbers.
  (*4) - If you are using the SI4735-D60, connect the SEN pin to the ground;
         If you are using the SI4732-A10, connect the SEN pin to the +Vcc.



## User instructions 


1. BAND, MODE, AGC/Attenuation, banddwith and STEP

Press the the correspondent push button and after, rotate the encoder to select the option. For example: 
To switch the band, press the band button and then rotate the encoder clockwise or counterclockwise. 
The display will show you the current band. 

To switch the mode (AM, LSB or USB), press mode button and then rotate the encoder.

The same idea you can use to AGC/Attenuation, STEP and Banddwith.

Tip: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time. 
     If you do that, you might alternate the command status (enable and disable) randomly. 


2. SEEK COMMAND

The seek button should be used to find a station. The seek direction is based on the last encoder movement.
If clockwise, the seek will go up; if counterclockwise, the seek will go down.


3. VFO/VFO Switch 

To control the VFO and BFO, used the encoder push button. The display will show if you are using VFO or BFO.

