# User Manual

It is  a  complete  receiver  capable  to  tune  LW,  MW,  SW  on  AM  and  SSB  mode  and  also  receive  the
regular  comercial  stations.

This sketch runs on ESP32 device LilyGO T-Embed panel and it was based on [Volos'](https://youtu.be/bg2Ysrh85Ek) project and adapted to SI473X platform. 

## Receiver Commands

After compiling and installing the ALL_IN_ONE_T_Embed.ino sketch on the ESP32 or LilyGO T-Embed, follow the guidelines below to operate the receiver.

### 1. BAND SELECTION

   Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counterclockwise.
   When the desired band is shown on display, you  can press the button once again to select the band. 


### 2. STEP, MODE, AGC/Attenuation, Automatic Volume Control (AVC), bandwidth, Soft Mute and VOLUME

     2.1. Press the encoder push button twice (within 1/2 second).
     2.2. After that, the display will show you the Menu text. Rotate the encoder clockwise or counterclockwise 
          to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc). 
     2.3. After that, select the option you want to setup by pressing the encoder push button once again. 
     2.4. After that, rotate the encoder clockwise or counterclockwise to select the parameter.
     2.5. Finally, you can press the button once again to go back to the main screen.

### 3. VFO/BFO Switch 

    3.1. Press the encoder push button twice (within 1/2 second).
    3.2. Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode. 
    3.3. Press the encoder push button once again. 
    3.4. Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset).
    3.5. If you press the button again to go back to main screen. 

### 4. SEEK 

   It is important to know that the direction of the last movement of the encoder will define the direction of the station search (from the current position to lower or higher frequencies).

   4.1. Select the menu by pressing twice the encoder push button. 
   4.2. Rotate the encoder clockwise or counterclockwise to select the "Seek" option.
   4.3. Press the encoder push button once.


ATTENTION: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time. 
           If you do that, you might alternate the command status (enable and disable) randomly. 

