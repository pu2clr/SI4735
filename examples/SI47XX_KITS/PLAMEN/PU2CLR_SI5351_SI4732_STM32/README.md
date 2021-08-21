# Manual for Dual Converter All Band Receiver (AM, SSB and FM modes based on SI5351 and SI473X) - Plamen's design

## This receiver is aimed to work from 150kHz to 170MHz on AM, SSB and FM modes

## This firmware is UNDER CONSTRUCTION....

* Sketch developed by Ricardo Lima Caratti - Jul, 2021
* Hardware developed by Plamen Panteleev, LZ1PPL. All you need to know about the hardware can be found [here.](https://www.lz1ppl.com/en/2021/04/22/si4735-all-mode-reciver/?fbclid=IwAR10n3x89ayj332m2X3x65AAR6bMVARHZ5VdDtkEzusT_qmUE-F6u9_QIaI)


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

4. EEPROM RESET

   The main information of the receiver is stored into the Arduino EEPROM. This way, when you turn the receiver on, the last receiver status is rescued. To RESET the receiver to DEFAULT status, turn it on with the encoder pust button pressed. Check the message "EEPROM RESETED". 


__ATTENTION__: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time. If you do that, you might alternate the command status (enable and disable) randomly. 


