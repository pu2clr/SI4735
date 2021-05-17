# SlametRadio 

This firmware is my simple project for #stayathome movement during Covid-19 pandemic. This radio using SI4735 with pu2clr library that you can get [here](https://github.com/pu2clr/SI4735). 


## Installation

You must first install Si4735 library first from [PU2CLR](https://github.com/pu2clr/SI4735) after that you must install the LCD driver. I'm using 2.8 inch LCD with ILI9341 driver. So you must install TFT_eSPI from Bodmer [here](https://github.com/Bodmer/TFT_eSPI). If you done all that, you must having no problem running my code. 

## Wiring

Wiring for the SI4735 module is pretty the same from you can find on the pu2clr library. SDA and SCK to the SI4735 modul with pull-up resistor. SDA/SCK pin depend on your microcontroller.
For the LCD pin, you must first open the User_Setup.h located inside TFT_eSPI library folder. 
If you use ILI9341, you can uncomment line 33 to activate the module. 

If you use ESP32 you can uncomment line 178 to activate pin module. 
Make sure you connect to the right pin on the microcontroller. 
```// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP   ######

// For ESP32 Dev board (only tested with ILI9341 display)
// The hardware SPI can be mapped to any pins

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    5  // Data Command control pin
#define TFT_RST   17  // Reset pin (could connect to RST pin)

#define TOUCH_CS 32     // Chip select pin (T_CS) of touch screen
```
## DEMO
You  can watch the demo here from my [youtube channel](https://www.youtube.com/watch?v=NK19b_4QpZY)

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## Thanks
Big Thanks to Ricardo Lima Caratti for the great library PU2CLR 
