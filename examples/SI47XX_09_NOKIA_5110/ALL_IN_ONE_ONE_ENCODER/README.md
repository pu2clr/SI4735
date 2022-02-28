# MANUAL FOR DIY Si4730 All Band Radio (LW, MW, SW, FM)" receiver by Mirko Pavleski

Sketch developed by Ricardo Lima Caratti - Jun, 2021

## SSB MODE

If you are using a __SI4735-D60__ or __SI4732-A10__ based circuit, you can also use this sketch to add the SSB functionality  to the original Pavleski's project. If you are using the original SI4730-D60 based circuit, the SSB will not work. However, the __STEP,  MODE, AGC, Attenuation, bandwidth, Soft Mute, Audio Volume and Shortwave__ will work fine.

## PL102BA-S V:2.1 vs NE928-10A V:01

There are two boards very common on eBay and AlliExpress based on SI4730.

1. “PL102BA-S V:2.1 10628” based on SI4730-D60 with shortwave support and FM / RDS.
   See https://pu2clr.github.io/SI4735/extras/BOARD_PL102BA/ for more details.
2. “NE928-10A V:01” based on SI4730-B20 WITHOUT shortwave and FM / RDS support.
   See https://pu2clr.github.io/SI4735/extras/BOARD_NE928_10A_V_01/ for more details.



## User Manual - Commands

1. BAND SELECTION

   * Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counterclockwise.
   * When the desired band is shown on display, you  can press the button once again or wait for about 2 seconds.
   * You will notice the control will go back to the VFO.

2. STEP, MODE, SEEK UP, SEEK DOWN, AGC/Attenuation, bandwidth, Soft Mute and VOLUME

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
