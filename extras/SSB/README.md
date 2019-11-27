# SI4735 and SSB support 

This sketch is a proof of concept of applying patches on Si4735 devices. In this case SSBRX patche.
In this context, a patch is a piece of software used to change the behavior of the SI4735 device.

There is little information available about patching the SI4735. The following information is the understanding
of the author of this sketch and is not necessarily correct.

A patch is executed internally (run by internal MCU) of the device. Usually, patches are  used to fixes bugs
or add improvements and  new features of the firmware installed in the internal ROM of the device.
Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device
by the host MCU (in this case Arduino).
Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
Consequently, the content of the patch has to be transferred again to the device each time after turn on the
system or reset the device.

ATTENTION:
The author of this sketch does not guarantee that this procedure will work in your development environment.
Given this, it is at your own risk to continue with the procedures suggested here.
This sketch works with the I2C communication protocol and is designed to apply a SSB extension PATCH to CI
SI4735-D60. Once again, the author disclaims any liability for any damage this procedure may cause to your
SI4735 or other devices that you are using. If you are sure about it and still want to apply the patch,
set the variable "APPLY_PATCH to true".


## Found at Internet

Thanks to Mr Vadim Afonkin for making available the SSBRX patches for SI4735-D60 on his Dropbox repository. 
[SI4735 Patch](https://www.dropbox.com/sh/xzofrl8rfaaqh59/AAA5au2_CVdi50NBtt0IivyIa?dl=0)

## References

1. [SI47XX PROGRAMMING GUIDE](https://www.silabs.com/documents/public/application-notes/AN332.pdf)
2. AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES
