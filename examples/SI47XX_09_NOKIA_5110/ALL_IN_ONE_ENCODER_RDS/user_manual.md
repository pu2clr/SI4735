# User Manual

You can control and select any function of this receiver using the encoder and push button under the encoder
This receiver saves its status in the internal EEPROM of the ATmega328. This way, when you turn
the receiver on again, the last status will be restored.

## Commands

### BAND SELECTION

1. Select the band by pressing the encoder push button once, and then rotate the encoder clockwise or counterclockwise to cycle through bands.
2. When the desired band is shown on display, you can press the button once again or wait for about 2 seconds. The control will then revert back to the VFO.

### STEP, MODE, AGC/Attenuation, bandwidth, Soft Mute, Automatic Volume Control (AVC) and VOLUME

1. Double press the encoder push button (Press twice within 1/2 second).  
2. The display will show you the Menu text. Rotate the encoder clockwise or counterclockwise to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc).  
3. Select the option you want to setup by pressing the encoder push button once again.  
4. Rotate the encoder clockwise or counterclockwise to select the parameter.  
5. Finally, you can press the button once again or wait for about 2 seconds. The control will go back to the VFO.  

### VFO/BFO Switch

1. Double press the encoder push button (Press twice within 1/2 second).  
2. Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode.  
3. Press the encoder push button again.  
4. Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset).  
5. If you press the button again or stop rotating the encoder for about 2 seconds, the control will go back to the VFO.

### SEEK

1. Select the menu by pressing twice the encoder push button.
2. Rotate the encoder clockwise or counterclockwise to select "Seek Up" or "Seek Down"
3. Press the encoder push button once on "Seek Up" or "Seek Down" to seek up or down.

### RESET

 This sketch saves some current receiver and band parameters into the internal Arduino EEPROM (frequency, band, mode, volume and band parameters).  
 For example:  if you select an attenuation parameter or filter parameter or something else on a given band, those information will be saved into the internal Arduino EEPROM.
 This way, when you turn the receiver on again, that setup will be restored to that given band.
 To reset the EEPROM content, follow the steps below:

1. Turn the receiver off
2. Press and keep pressing the encoder push button  
3. Turn the receiver on and wait for the "EEPROM RESET" message

### ATTENTION:

To save EEPROM write cycles:

- Any receiver parameter change will only be saved after 10 seconds of inactivity, allow the system 10 seconds after changing a parameter and powering down in order to save you settings.
- Only modified parameters will be saved to EEPROM.

### ATTENTION:

Press and release the push button quickly. If you hold the button down, the microcontroller will think the button is being rapidly pressed, this will make it rapidly alternate whichever setting the button was operating. Just a quick press and release is the best way to operate the button.
