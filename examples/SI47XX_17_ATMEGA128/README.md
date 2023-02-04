# Atmega128 based board example     

Before compiling, please, install the [MegaCore]( https://github.com/MCUdude/MegaCore ) board in your Arduino IDE.
* [Click here to know how to Install MegaCore board manager]( https://github.com/MCUdude/MegaCore#boards-manager-installation )

## Compile using the options below: 

* Bootloader = No bootload
* Compile LTO = LTO Enabled
* Clock = Select the right crystal setup you are using


## Atmega128 and components wire up. 
  
| Device name   | Device Pin / Description | ATmega128 |
| --------------| -------------------------| ----------|
|    OLED       |                          |           |
|               | SDA/SDIO                 |  PD1      | 
|               | SCL/SCLK                 |  PD0      | 
|    Encoder    |                          |           |
|               | A                        |  PE4      |
|               | B                        |  PE5      |
|               | PUSH BUTTON (encoder)    |  PB0      |


## ATmega128 and SI4735-D60 or SI4732-A10 wire up

| Si4735  | SI4732   | DESC.  | Seeduino     | 
|---------| -------- |--------|------------- |
| pin 15  |  pin 9   | RESET  |   PB2/12 [1] |  
| pin 18  |  pin 12  | SDIO   |   PD1        |
| pin 17  |  pin 11  | SCLK   |   PD0        |


1. Use a 22k pull-up resistor 

