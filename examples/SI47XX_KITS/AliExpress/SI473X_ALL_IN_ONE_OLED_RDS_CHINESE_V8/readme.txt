  TThis sketch will download a SSB patch to your SI4735 device (patch_init.h). It will take about 8KB to 15KB of the Arduino memory.

Firstly, the SSB patch content is not part of this library.
These patches were published by Mr. [Vadim Afonkin](https://youtu.be/fgjPGnTAVgM) on his Dropbox repository.
The author of this Si4735 Arduino Library does not encourage anyone to use the SSB patches content for 
commercial purposes. In other words, while this library supports SSB patches, the patches themselves 
should not be considered a part of this library.

    In this context, a patch is a piece of software used to change the behavior of the SI4735 device.
    There is little information available about patching the SI4735. The following information is the understanding of the author of
    this project and is not necessarily correct. 

    A patch is executed internally (run by internal MCU) of the device. Usually, patches
    are used to fix bugs or add improvements and new features over what the firmware installed
    in the internal ROM of the device offers. Patches for the SI4735 are distributed in binary
    form and are transferred to the internal RAM of the device by the host MCU (in this case, Arduino boards).

    Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off
    the system. Consequently, the content of the patch has to be transferred to the device every
    time the device is powered up.

    ATTENTION: The author of this project cannot guarantee that procedures shown
    here will work in your development environment. Proceed at your own risk.
    This library works with the I²C communication protocol to send an SSB extension
    PATCH to SI4735-D60 and SI4732-A10 devices. Once again, the author disclaims any
    and all liability for any damage or effects this procedure may have on your devices.
    Procced at your own risk.
