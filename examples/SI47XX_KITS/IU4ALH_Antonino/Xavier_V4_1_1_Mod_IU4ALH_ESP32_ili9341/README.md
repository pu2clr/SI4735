
**v4.1 Mod.Jan 14.01.2023  By IU4ALH - This mod, is for ESP32 and ili9341 driver SPI
			Dispalys ( 2.2 - 2.4 - 2.8 - 3.2 inch ), with Full RDS Services - name station, News ( in scroll mode), Time - 
            New layout for S-Meter , new layout for screen. **
**V4.1    Jan-22-2022 Battery Indicator.**
 
**V4.0    Jan-16-2022 New version based on Gert Baak's V3.4 sketch.**

* New user interface
* "Press and hold" performing different actions
* Battery icon
* Bug fixes

This sketch was made by [Ralph Xavier] (https://github.com/ralphxavier/SI4735)

The sketch and documentation available here are based on Gert's sketch.
I would like to thank to [Gert Baak](https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino) 
for his excelent job. You are great!

This sketch also uses the [PU2CLR SI4735 Library for Arduino](https://github.com/pu2clr/SI4735). Thanks for the very nice work.

This sketch uses a Rotary Encoder and [TTGO T-Display](http://www.lilygo.cn/prod_view.aspx?TypeId=50044&Id=1126).
The radio is fully controlled by the Rotary Encoder.

IMPORTANT:

    This release implements different behaviors for different "press and hold" durations on its single button.

    There are 3 different button "press and hold" durations: Short Press Duration (SP), Long Press Duration (LP) 
    and Long Long Press Duration (LLP)

        - SP:  Short Press - less than 500ms.
        - LP:  Long Press - between 500ms and 1500ms.
        - LLP: Long Long Press - 1500ms or more.
  
    At the main screen (Frequency Display), the duration of the button press and hold performs 3 different actions:
        - SP:  Volume controle
        - LP:  Opens First Layer Menu
        - LLP: Do a reset.

    At the others screen/Menu, the duration of the button press performs only one action:
        - SP:  Select
        - LP:  Do Nothing
        - LLP: Do Nothing.


**V3      Jun-06-2021 Initial Release.**

First Release. Alpha version.

**DISCLAMER**

ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment.
Given this, it is at your own risk to continue with the procedures suggested here.
This library works with the I2C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60.
Once again, the author disclaims any liability for any damage this procedure may cause to your SI4735 or other devices, like an ATS-100 that you are using.
There is NO GUARANTEE that this sketch will work with the Chinese ATS-100 KIT sold on AliExpress, eBay etc.
The author of this sketch and Arduino Library does not know the seller of this kit and does not have a commercial relationship with any commercial product that uses the Arduino Library.
It is important you understand that there is no guarantee that this sketch will work correctly in your current product.
SO, DO NOT TRY IT IF YOU DON'T KNOW WHAT ARE YOU DOING. YOU MUST BE ABLE TO GO BACK TO THE PREVIOUS VERSION IF THIS SKETCH DOES NOT WORK FOR YOU.
