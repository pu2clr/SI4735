
# THIAGO'S KIT FIRMWARES / SKETCHES

The KIT made by Thiago Lima is based on __ESP32 ( ESP32 WROOM-32)__ and __SI4732-A10__ devices. It is based on the Gert's design. You can find the original project made by __Gert Baak__ [here](https://github.com/pe0mgb/SI4735-Radio-ESP32-Touchscreen-Arduino?fbclid=IwAR3TQd2j4HxAFvpcGkbXiPuDly8m2OnGclTDiqthnkbqqe2fN1McP2m3WSI).


All sketches in this folder use the [PU2CLR SI4735 Arduino Library](https://pu2clr.github.io/SI4735/) developed by Ricardo.
All sketches were tested on __ESP32 WROOM-32__, TFT 2.8" "240 x 320" Touchscreen with an __ILI9341 controller__ and Rotary Encoder with push button. For the TFT display the __TFT_eSPI__ Arduino Library is used. The ILI9341 config file is available here.

__After installing the TFT_eSPI Arduino Library, copy the files Setup1_ILI9341.h and User_Setup_Select.h to the /Documents/Arduino/libraries/TFT_eSPI folder. If you are using Windows, check where the Arduino stores the installed libraries.__ 

Read the documentation available in each ino file. The control of the radio is 100% done by the touchscreen and Rotary Encoder.


### Sketches features:

* FM (VHF) support (64–108 MHz)
* AM (MW) band support (520–1710 kHz)
* SW band support (2.3–30.0 MHz)
* LW band support (153–279 kHz)
* S-meter
* SNR and RSSI indication
* Volume indicator
* FM Stereo and RDS. RDS has a SNR theshold and on / off button
* AM and SSB with 1,5,9 & 10 KHz tuningstep
* FM, AM and SSB (USB & LSB) modulation. SSB controlled by BFO
* Preset for FM controlled by rotary encoder 
* May be finished by rotary encoder switch or automatically by waking up the screen 
* Band selection for Broadcast (17) and Ham bands (11)
* Prefered modulation for all Ham and Broadcast bands
* Bandwidth control for AM and SSB
* On screen Keyboard for frequency control
* Tuningstep control for all broadcast and ham bands (except FM) 
* Mute
* Volume control by rotary encoder. Function is switching off after 30 sec automatically
* AM/FM seek tuning
* Automatic frequency control (AFC)
* Automatic gain control (AGC)
* Manual gain control by the rotary encoder
 
To save power from the battery, display is switching off after 5 minutes. 
Display is waking up by the use of one of the controllers e.g. screen, rotary controller or rotary controller switch.  Power consumption 120 mA and 80 mA when display is switched off.


## Schematic 

### SI4732-A10 version 

![SI4732-A10 version](./SI4732_Radio_schematics.png)


### SI4735-G60 version


![SI4732-A10 version](./SI4735_Radio_schematics.png)


## Firmware Update Tutorial


### Install the last version of Arduino IDE

Go to [Arduino website](https://www.arduino.cc/)

### Configure the ESP32 on Arduino IDE 

Open the Arduino IDE and select the __Preferences...__ menu item. 
Go to __Additional Boards Manager URLs:__ and add the URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

After adding the ESP32 URL, go to __Tools__ menu, select the __Boards__ item and then select __Boards Manager...__ option. Finally install ESP32 board.


### Install the PU2CLR SI4735 Arduino Library

Go to __Tools__ menu and select __Manage libraries...__. Finally install the PU2CLR SI4735 Arduino Library.


### Install the TFT_eSPI  Arduino Library

Go to __Tools__ menu and select __Manage libraries...__. Finally install the TFT_eSPI Arduino Library.


### Copy two configurarion files to TFT_eSPI library folder

__After installing the TFT_eSPI Arduino Library, copy the files Setup1_ILI9341.h and User_Setup_Select.h available here to the /Documents/Arduino/libraries/TFT_eSPI folder. If you are using Windows, check where the Arduino stores the installed libraries.__ 

### Compile and upload the firmware

* Open the desired firmware with the Arduino IDE;
* Connect the ESP32 device to your computer;
* Select the board __ESP32 Dev Module__;
* Select the right Port (it will depend on your SO);
* Run the __Upload__ command.


[Check this video on Youtube](https://youtu.be/Gv8bHQbzdO4)







