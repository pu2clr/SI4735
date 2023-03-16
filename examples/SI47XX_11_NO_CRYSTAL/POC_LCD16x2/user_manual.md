#User Manual

This sketch was built to check the external active crystal oscillator instead of a passive crystal.

[Link to the external oscillator schematic](https://github.com/pu2clr/SI4735/tree/master/extras/schematic#si473x-and-external-active-crystal-oscillator-or-signal-generator)

[Link to encoder and LCD16x2 schematic](https://github.com/pu2clr/SI4735/tree/master/extras/schematic#standalone-atmega328-with-or-without-external-crystal-si4735-d60-and-lcd-16x2)

This sketch uses the ATmega328 internal EEPROM to store the current status of the receiver.
This way, the receiver can start working from the last status (before turning it off).
If you need to reset the status stored into the EEPROM, keep the encoder push button pressed during system startup.

If you are using a SI4735-D60 or SI4732-A10 based circuit, you can also use this sketch to add the SSB functionality to the
original Pavleski's project. If you are using the original SI4730-D60 based circuit, the SSB will not work. However, the STEP,
MODE, AGC, Attenuation, bandwidth, Soft Mute, Audio Volume and Shortwave will still work.

###Commands

1. BAND SELECTION

    1.1 Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counterclockwise.  
    1.2 When the desired band is shown on display, you  can press the button once again or wait for about 2 seconds.  
    1.3 The control will then go back to the VFO.

2. STEP, MODE, AGC/Attenuation, bandwidth, Soft Mute and VOLUME

     2.1. Press the encoder push button twice (within 1/2 second).  
     2.2. After that, the display will show you the Menu text. Rotate the encoder clockwise or counterclockwise
          to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc).  
     2.3. After that, select the option you want to setup by pressing the encoder push button once again.  
     2.4. After that, rotate the encoder clockwise or counterclockwise to select the parameter.  
     2.5. Finally, you can press the button once again or wait for about 2 seconds.
          The control will then go back to the VFO.

3. VFO/BFO Switch

    3.1. Press the encoder push button twice (within 1/2 second).  
    3.2. Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode.  
    3.3. Press the encoder push button once again.  
    3.4. Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset).  
    3.5. If you press the button again or stop rotating the encoder for about 2 seconds, the control will go back to the VFO.


4. SEEK

    4.1. Select the menu by pressing twice the encoder push button. The seek direction is based on the last encoder
        movement. If clockwise, the seek will go up. If counterclockwise, the seek will go down.

    __ATTENTION:__ Try press and release the push button quickly. If you hold the button too long, the board may
    randomly alternate the command status (enable and disable).

    After about 2 seconds, the current command or action is disabled automatically and the interface goes back
    to VFO control.


###TIPS:

You can adjust the time to disable a current command or action by changing the constant ELAPSED_COMMAND
(#define ELAPSED_COMMAND 2000). The value 2000 means 2 seconds. Increase or decrease the value to find the speed you prefer.
Try to refine the constant ELAPSED_CLICK (#define ELAPSED_CLICK 1500). This constant controls the time
for double click on encoder push button. Smaller values demand more agility in the double click.
