User Manual

Commands

1. BAND SELECTION
   
   Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counterclockwise.
   When the desired band is shown on display, you  can press the button once again or wait for about 2 seconds. 
   The control will revert back to the VFO.

2. STEP, MODE, AGC (Automatic Gain Control)/Attenuation, bandwidth, Soft Mute, AVC (Automatic Volume Control) and VOLUME
   
     2.1. Press the encoder push button twice (within 1/2 second).
     2.2. After that, the display will show you the Menu text. Rotate the encoder clockwise or counterclockwise 
   
          to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc). 
   
     2.3. After that, select the option you want to setup by pressing the encoder push button once again. 
     2.4. After that, rotate the encoder clockwise or counterclockwise to select the parameter.
     2.5. Finally, you can press the button once again or wait for about 2 seconds. 
   
          The control will go back to the VFO.  

3. VFO/BFO Switch (IT DOES NOT WORK ON DIGITAL MODE)
   
    3.1. Press the encoder push button twice (within 1/2 second).
    3.2. Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode. 
    3.3. Press the encoder push button once again. 
    3.4. Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset).
    3.5. If you press the button again or stop rotating the ancoder for about 2 seconds, the control will go back to the VFO. 

4. SEEK 
   
   4.1. Select the menu by pressing twice the encoder push button. 
   4.2. Rotate the encoder clockwise or counterclockwise to select "Seek Up"  or "Seek Down".
   4.3. Press the encoder push button once on "Seek Up"  or "Seek Down".

5. RESET 
   
   This sketch saves some current receiver and band parameters into the internal Arduino EEPROM (frequency, band, mode, volume and band parameters). 
   For example:  if you select an attenuation parameter or filter parameter or something else on a given band, those information will be saved into the internal Arduino EEPROM. 
   This way, when you turn the receiver on again, that setup will be restored to that given band. 
   To reset the EEPROM content, folow the steps below: 
   
   5.1. Turn the receiver off;
   5.2. Press and keep pressing the encoder push button;
   5.3. Turn the receiver on and wait for the message "EEPROM RESETED".
   
   ATTENTION: To save EEPROM write cycles, any receiver parameter change will only be saved after 10 seconds of inactivity; 
   
              Only modified parameters will be saved; 
              If no parameter is modified, no writing will be made to the EEPROM; 
              It you turn the receiver off before 10 seconds after any modification, the current data will not be saved;
              You need to wait for 10 seconds after any modification to save the current receiver setup.

ATTENTION: Try press and release the push button fastly. I mean, do not keep the button pressed for a long time. 
           If you do that, you might alternate the command status (enable and disable) randomly. 
