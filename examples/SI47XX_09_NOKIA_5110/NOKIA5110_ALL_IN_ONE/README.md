# MANUAL FOR DIY Si4730 All Band Radio (LW, MW, SW, FM)" receiver by Mirko Pavleski

This sketch was built to work with the project [DIY Si4730 All Band Radio (LW, MW, SW, FM)" receiver by Mirko Pavleski](Thttps://create.arduino.cc/projecthub/mircemk/diy-si4730-all-band-radio-lw-mw-sw-fm-1894d9). 

Sketch developed by Ricardo Lima Caratti - Jun, 2021

## SSB MODE

If you are using a __SI4735-D60__ or __SI4732-A10__ based circuit, you can also use this sketch to add the SSB functionality  to the original Pavleski's project. If you are using the original SI4730-D60 based circuit, the SSB will not work. However, the __STEP, FM/RDS, MODE, AGC, Attenuation, bandwidth, Soft Mute, Audio Volume and Shortwave__ will work fine. 

## PL102BA-S V:2.1 vs NE928-10A V:01

There are two boards very common on eBay and AlliExpress based on SI4730.

The Mirko Pavleski project can with  NE928 or PL102 boards based on SI4730.  The __NE928-A10 does not__ support SW and FM RDS. The __PL102BA supports__ SW and FM RDS. __Both does not support SSB patches__. See more information about these boards: 

1. “PL102BA-S V:2.1 10628” based on SI4730-D60 with shortwave support and FM / RDS. 
   See https://pu2clr.github.io/SI4735/extras/BOARD_PL102BA/ for more details.
2. “NE928-10A V:01” based on SI4730-B20 WITHOUT shortwave and FM / RDS support. 
   See https://pu2clr.github.io/SI4735/extras/BOARD_NE928_10A_V_01/ for more details. 


## New features

This sketch is very similar to the previous sketch (MIRKO_V1.ino). The new features of the MIRKO_V2.ino is shown below.


1. New menu setup;
2. The receiver current status is stored into Arduino EEPROM;
3. FM RDS;
4. FM frequency step;
5. FM Bandwidth control.

## All features 

1. Modes: FM and AM; 
2. [*1] SSB; 
3. FM bands covering from 64 to 108 MHz;
4. [*1] LW band; 
5. Two MW bands (one for Europe, Africa and Asia);
6. Twenty bands. One VHF(FM); one LW; two MW; and Sixteen SW bands covering from 3000 to 30000 kHz;
7. [*1] FM/RDS; 
8. Bandwidth control on FM mode (Auto, 110, 84, 60 and 40 kHz);
9. Bandwidth control on AM mode (1, 2, 2.5, 3, 4 and 6 kHz);
10. [*1] Bandwidth control on SSB mode (0.5, 1, 1.2, 2.2, 3 and 4 kHz);
11. Receiver status stored into the EEPROM
12. All the previous status of the receiver can be rescued when you turn it on ([*1] including SSB mode, bandwidth, volume, frequency, BFO etc)
13. The bandwidth is a property of the band (you can use different bandwidth for different bands)
14. The seek function was improved (it is more precise on FM mode). 
15. Steps: 1, 5, 9, 10, 50 and 100 kHz
16. Now you can configure MW band space to 9 or 10 kHz
17. Now the bandwidth sequence is ordered by bandwidth values

[*1] - If you are using PL102BA-S V:2.1, SI4730-D60, SI4732-A10,  SI4734-D60; SI4735-D60


## User Manual - Commands

1. BAND SELECTION

   * Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counterclockwise.
   * When the desired band is shown on display, you  can press the button once again or wait for about 2 seconds. 
   * You will notice the control will go back to the VFO.

2. STEP, MODE, SEEK UP, SEEK DOWN, AGC/Attenuation, bandwidth, FM/RDS, Soft Mute and VOLUME

     * Press the encoder push button twice (within 1/2 second).
     * After that, the display will show you the Menu text. Rotate the encoder clockwise or counterclockwise to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc). 
     * After that, select the option you want to setup by pressing the encoder push button once again. 
     * After that, rotate the encoder clockwise or counterclockwise to select the parameter.
     * Finally, you can press the button once again or wait for about 2 seconds. 
     * The control will go back to the VFO.  

3. VFO/BFO Switch 

    * Press the encoder push button twice (within 1/2 second).
    * Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode. 
    * Press the encoder push button once again. 
    * Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset).
    * If you press the button again or stop rotating the ancoder for about 2 seconds, the control will go back to the VFO. 

__ATTENTION__: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time. If you do that, you might alternate the command status (enable and disable) randomly. 

4. EEPROM RESET

   The main information of the receiver is stored into the Arduino EEPROM. This way, when you turn the receiver on, the last receiver status is rescued. To RESET the receiver to DEFAULT status, turn it on with the encoder pust button pressed. Check the message "EEPROM RESETED". 


