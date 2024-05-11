# User Manual

This skech demonstrates a way to report the current status of the receiver via Morse Code.
However, the prototype used by this sketch is not intended for blind people.
It is just an idea to inspire people to build receivers with accessibility features.

### Commands

#### BAND SELECTION
   
    1. Select the band by pressing the encoder push button once and then rotate the encoder clockwise or counter clockwise
    2. When the desired band is shown on display, you can press the button once again or wait for about 2 seconds. The control will then revert back to VFO

#### STEP, MODE, AGC/Attenuation, bandwidth, Soft Mute and VOLUME
   
    1. Double press the encoder push button (within 1/2 second)
    2. The display will now show you the Menu text. Rotate the encoder clockwise or counterclockwise to select the option (STEP, MODE, AGC/Attenuation, bandwidth, VOLUME, etc)
    3. Select the option you want to setup by pressing the encoder push button again
    4. Rotate the encoder clockwise or counterclockwise to select the desired parameter
    5. Finally, you can press the button once again or wait for about 2 seconds. The control will then revert back to VFO

#### VFO/BFO Switch
   
    1. Double press the encoder push button twice (within 1/2 second)
    2. Rotate the encoder clockwise or counterclockwise and go to the BFO option. This option is shown only on SSB mode
    3. Press the encoder push button again
    4. Rotate the encoder clockwise or counterclockwise to increment or decrement the BFO (select the offset)
    5. If you press the button again or stop rotating the encoder for about 2 seconds, the control will revert back to VFO

#### SEEK
   
    1. Select the menu by pressing twice the encoder push button. The seek direction is based on the last encoder movement. Rotate clockwise to seek up, or counterclockwise to seek down.
   
#### ATTENTION:

   Try to quickly press and release the push button. If you hold the button too long, the board may randomly alternate the command status (enable and disable).
    After about 2 seconds, the current command or action is disabled automatically and the interface reverts back to VFO control.

#### TIPS:

You can adjust the time to disable a current command or action by changing the constant ELAPSED_COMMAND
(#define ELAPSED_COMMAND 2000). The value 2000 means 2 seconds. Adjust this value according to your preference.
Try to refine the constant ELAPSED_CLICK (#define ELAPSED_CLICK 1500). This constant controls the time
for double click on encoder push button. Smaller values demand more agility in the double click.
