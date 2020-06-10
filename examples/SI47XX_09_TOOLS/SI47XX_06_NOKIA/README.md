# Nokia 5110 LCD

This project is under construction.
Waiting for the Nokia 5110 from eBay. 
The sketch is compiling. However, the system was not tested.  

## This project uses: 

* Arduino Nano 5V
* I2C bus shift voltage converter to connect the Si4735 to the Arduino
* Nokia 5110 LCD
* Home brew Si4735 board  

## Receiver features 

* FM, AM and SSB
* LW, MW and SW (from 3 to 30 MHz)
* Volume, Band select and filters controlled via buttons

## Arduino Wire up

| NOKIA 5110 pin | Arduino Pin | Description |
| --------------- | ----------- | ----------- |
| NOKIA_SCK       | 13          | SCK is LCD serial clock (SCLK) |
| NOKIA_DIN       | 11          | MOSI is LCD DIN |
| NOKIA_DC        |  5          | LCD Data/Command select (D/C) (DC) |
| NOKIA_CE        |  4          | LCD chip select (CE or CS) |
| NOKIA_RST       |  6          | LCD reset (RST) | 


## Si4735, buttons and encoder wire up 

The table below show the pins wire up for this example on Arduino Pro Mini.

| Device name               | Device Pin / Description  |  Arduino Pin  |
| ----------------          | --------------------      | ------------  |
| __Si4735__                |                           |               |
|                           | RESET (pin 15)            |     12 (D12)  |
|                           | SDIO (pin 18)             |     A4 (SDA)  |
|                           | SCLK (pin 17)             |     A5 (SCL)  |
| __Buttons__               |                           |               | 
|                           | Switch MODE (AM/LSB/AM)   |      14 (A0)  |
|                           | Banddwith                 |      15 (A1)  | 
|                           | Volume Up                 |      16 (A2)  |
|                           | Volume Down               |      17 (3)   |
|                           | Band Up                   |      20 (A6)  |
|                           | Band Down                 |      21 (A7)  | 
|                           | AGC Switch                |       8 (D8)  |
|                           | STEP Switch               |       9 (D9)  |
|                           | BFO/VFO Switch            |      10 (D10) |
| __Encoder__               |                           |               |
|                           | A                         |       2       |
|                           | B                         |       3       |

