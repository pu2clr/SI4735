# Si4735 Library for Arduino

This is a library for the SI4844, BROADCAST ANALOG TUNING DIGITAL DISPLAY AM/FM/SW RADIO RECEIVER,  IC from Silicon Labs for the Arduino development environment.  This library is intended to provide an easier interface for controlling the SI4844.

By Ricardo Lima Caratti, Oct, 2019. 

__Attention: Code and Documentation  Under construction.__
__Do not try use it for while.__

__To be continue....__


## SI4735 Terminology

| Term | Description |
| ---- | ----- |
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




## References

1. [Silicon Labs Si4737 WB/AM/FM Stereo/RDS single-chip receiver HAL library for Arduino ](https://github.com/rickeywang/Si4737_i2c)
2. [Silicon Labs Si4735 LW/SW/AM/FM Stereo/RDS single-chip receiver HAL library for Arduino](https://github.com/csdexter/Si4735/wiki)
3. 
