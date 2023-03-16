# Teensy and PU2CLR SI4735 Arduino Library

This folder show some examples with PU2CLR SI4735 Arduino Library and Teensy platform.


## Example AM_FM_SERIAL_MONITOR

This sketch uses just the Serial Monitor as the human interface to test and validation of the PU2CLR SI4735 Arduino Library on Teensy platform. It can be very useful to test your circuit. 

### With this sketch, you only need the Arduino IDE to control the circuit

This is great for testing your circuit because:

1) You don't need buttons, rotary encoders, potentiometers, etc. to control the circuit
2) You don't need a screen to see output from the Arduino board
3) All functions supported by this sketch can be accessed over the serial monitor


### SI4732 / SI4735 Wire up


#### Teensy 3.1 and components wire up. 
  
| Device name     | Device Pin / Description |  Teensy |
| ----------------| -------------------------| --------|
|    OLED         |                          |         |
|                 | SDA/SDIO                 |  A4     | 
|                 | SCL/SCLK                 |  A5     | 
|    Encoder      |                          |         |
|                 | A                        |   9     |
|                 | B                        |  10     |
|                 | PUSH BUTTON (encoder)    |  11     |


#### Teensy and SI4735-D60 or SI4732-A10 wire up

| Si4735  | SI4732   | DESC.  | Teensy 3.1 |
|---------| -------- |--------|------------|
| pin 15  |  pin 9   | RESET  |   12       |  
| pin 18  |  pin 12  | SDIO   |   A4 (SDA) |
| pin 17  |  pin 11  | SCLK   |   A5 (SCL) |



## Example OLED_ALL_IN_ONE

This example is a complete FM/AM/LW/SW (with SSB support) receiver based on SI4735 running on Teensy 



### Schematic 



![Teensy 3.1 Schematic](../../extras/images/schematic_teensy31.png)

