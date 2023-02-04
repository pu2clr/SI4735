# Atmega32 based board example   

Before compiling, please, install the MightyCore board in your Arduino IDE. 

  * [Click here to know more](https://github.com/MCUdude/MightyCore)
  * [How to install MightyCore board manager](https://github.com/MCUdude/MightyCore#how-to-install)

## Compile using the options below: 

* Bootloader = No bootload
* Compile LTO = LTO Enabled
* Pinout = Standard pinout
* Clock = Select the right crystal setup you are using


# Atmega32 and components wire up. 
  
| Device name   | Device Pin / Description | ATmega32  |
| --------------| -------------------------| ----------|
|    OLED       |                          |           |
|               | SDA/SDIO                 |  17 (PC1) | 
|               | SCL/SCLK                 |  16 (PC0) | 
|    Encoder    |                          |           |
|               | A                        |  10 (PD2) |
|               | B                        |  11 (PD3) |
|               | PUSH BUTTON (encoder)    |  0 (PB0)  |

## ATmega32 and SI4735-D60 or SI4732-A10 wire up

| Si4735  | SI4732   | DESC.  | ATmega32 | 
|---------| -------- |--------|----------|
| pin 15  |  pin 9   | RESET  |   2 (PB2) [1] |  
| pin 18  |  pin 12  | SDIO   |  17 (PC1)|
| pin 17  |  pin 11  | SCLK   |  16 (PC0)|

1. You may need a 22k pull-up resistor connected to the reset pin
