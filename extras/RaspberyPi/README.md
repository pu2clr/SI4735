# PU2CLR Arduino Library on Raspberry PI

__UNDER CONSTRUCTION....__

Here you will have some tips about "PU2CLR SI4735 Arduino Library" and Raspberry Pi.  


## Using Arduino IDE to generate Raspbery Pi code

I would like to thank Mr. Ryk DeYoe for testing and share this setup.

Mr. Ryk wrote on [SI47XX for hobbyists si47xx@groups.io](https://groups.io/g/si47xx):

I'm using an Rpi for a carputer project.  Didn't really occur to me that FM radio would be an option.  I'm using Openauto Pro as the OS/media server.  Looking in their forums, FM radio isn't supported and they have no interest is building it.  SDR usb dongles aren't an option to them as they use too many resources on the Rpi.  So I went out on my own.  Keep in mind this is alpha / hacky stuff, but it may help someone out there. 

I ran into Mr. Caratti's arduino library while I was doing research on building hardware for FM reception.  The si473x chip looked like a great option and had all the features I wanted.  Ricardo's library was easy to read and very complete (i'm not a programmer, i was a sysadmin for many years).  Only one problem; it was for arduino.  I thought maybe i could port it to  Rpi functions but after looking at 8000+ lines of code (i'm not a programmer), i realized that i don't have the talents for that.

I ordered an si4732-A10 from Digi-Key to mess around with since it looks like the 4735 is hard to get hold of.  I looked at some other projects that controlled the 4703 chip. Promising, but missing some features.  I found a C program from someone in germany for the 4731 and compiled but it didn't operate correctly.  I downloaded some python scripts for the 4703 (wouldn't operate my chip) and 4735.  The python script for the 4735 almost works, but uses an older wire library.  I tried modifying it and got some chip control but I was losing something in the I2C protocol, I couldn't get the chip to tune (no STC interrupt completion). 

Ugh.  I went back around full circle to Ricardo's library.  I started searching in earnest and found an article on running arduino code on Rpi.  Following that i installed the Arduino IDE on my Rpi (linux 32 bit version for arm processor).  I installed the Rpi board in Arduino IDE using the [RaspiArduino framework](https://github.com/me-no-dev/RasPiArduino).  I also installed [Ricardo's library](https://github.com/pu2clr/SI4735) in Arduino IDE (Tools > Manage Libraries).  I'm using VNC to remote into the Rpi from my laptop to run the IDE.  Kind of slow, but works.

I compiled the simple sample program in the IDE, but added the   "#include SI4735.h" at the top.  In the IDE, i set the board to "Raspberry Pi B+/2" (Tools > Board > Raspberry Pi).  In the IDE, instead of a "sketch", it's possible to make a binary file (Sketch > Export Compiled Binary).  I exported the code and (with a few trivial warnings) it created a binary file.  In the pi, you have to set the executable permission on the file (chmod +x <filename>).  The simple file executed just fine.  

Since the simple (mostly blank) file was ok and didn't complain about the SI4735 library, I downloaded an example file from the si473x github.  I had to modify some of the code (set the bus address, 0x11 or decimal 17 on mine, use Console.print instead of Serial.print functions for stdout display).  I exported and ran the binary. I could control the chip!  Tuning, seeking, changing bands all worked! Finally getting somewhere.  I've installed all the necessary software on my laptop for developing as the Rpi is strained with editing / compiling / VNC remote connections.  I can work on the laptop and just copy the binary to the Rpi to run.  

Next was to get sound.  Because i'm using this as a media player/carputer/android auto headunit, I didn't want to use the analog output jack from the Rpi (and Openauto requires an external usb sound card).  The Rpi has I2S capabilities, both input and output, so that's where i wanted to go.

The process on the Rpi uses a device tree overlay to create a virtual sound card.  Using a definition file from this github, a device is compiled and set to load at boot. I'm using a GPIO pin from the Rpi to set an external clock for the si4732.  I modified the example file some more to add the external clock properties (XOSCEN_RCLK, SI473X_DIGITAL_AUDIO1, RefClock (32768) and RefClockPrescaler(1)).  I setup arecord to record a 60 second .wav file while i ran the si473x program in another window seeking stations. 

Since i only have a piece of wire as an antenna, it didn't tune well, but to my  surprise i could hear static hissing in the file!  I2S sound was working from the chip!  After reading endlessly on reference clocks, I2S/MSB/LSB formats, audio sampling rates and on and on ad nauseam, i didn't really have a lot of hope but here was nice clear static!  I found out I could play my .wav file with the media player without having to set a recording duration so I set the record to infinity while I played the file and messed around with seeking the FM band.  The chip mutes while it's seeking, but sometimes it would lock on a frequency and play the static....until it reached a rather strong station in my area aaaaand....I heard MUSIC!....then a station identification!....then today's lottery numbers!....then music again!!  I was able to tune it stonger with smaller tune steps and a lot of the static faded, it's still there (no antenna and mono only) but the sound quality was very very good.

I've since piped my arecord output to the aplay program which defaults to the external usb sound card and cranked it up on external speakers without a media player.  Fantastic sound quality (even in mono) and for some reason no skipping or buffer under/over runs on the Rpi.  The resource usage is low too, i can use the Rpi while the chip is operating.

I know this post is TLDR; and I haven't put all the details in here but I just wanted <whoever> to know that this amazing library will run on the Rpi.  I'll make another post with more hardware / connection / pitfall details.  So much thanks to Ricardo Caratti for his work on this amazing library.  I don't have Facebook, so if someone would please, link this to the si473x Facebook page so other Rpi experimenters can find it.  

### More detais about Ryk's Raspberry Pi setup

* [Group IO](https://mail.google.com/mail/u/0/#inbox/WhctKKWxVFQKTQcvzjzXzDJWgLbPprlcMFNvJlfDrwvlgdJZlLghBcSfSqhsLmCNVmDSbgQ)
* [Install raspiArduino connector on github](https://github.com/me-no-dev/RasPiArduino)
* [Install the PU2CLR SI4735 Arduino Library](https://github.com/pu2clr/SI4735#library-installation)
* [How to Run Arduino Sketches on Raspberry Pi](https://www.deviceplus.com/raspberry-pi/how-to-run-arduino-sketches-on-raspberry-pi/)
* ['virtual' sound card that can record the I2S pin](https://github.com/AkiyukiOkayasu/RaspberryPi_I2S_Master)


## Porting PU2CLR SI4735 Arduino Library to Raspberry Pi

__UNDER CONSTRUCTION....__

* [Wiring Pi Site](http://wiringpi.com/)
* [WiringPi (Unofficial Mirror/Fork)](https://github.com/WiringPi/WiringPi)
* [Reading I2C Inputs in Raspberry Pi Using C](https://www.instructables.com/Reading-I2C-Inputs-in-Raspberry-Pi-using-C/)
* [i2c.c File Reference](https://alanbarr.github.io/RaspberryPi-GPIO/i2c_8c.html)
* [Extending Python with C or C++](https://docs.python.org/3/extending/extending.html)
* [Cross Compiling wiringPi on OSX using Docker](https://gist.github.com/jonlidgard/e581a31c9885bee34a18b60efa3774d8)




#si4735 #si4732 #raspberry #arduino #si4730 