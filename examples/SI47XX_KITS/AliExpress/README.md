# KIT sold on Aliexpress and eBay

The kit below is sold on Aliexpress and eBay.  Although the seller does not explicitly mention that the Arduino sketch used by the KIT is based on this library (PU2CLR SI4735 Arduino Library), I’ve been testing successfully the sketch [SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V7](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_KITS/AliExpress/SI473X_ALL_IN_ONE_OLED_RDS_CHINESE_V7) on some KITs acquired by some Brazilians experimenters. __The author of this library does not know the seller of this kit and does not have a commercial relationship with any commercial product that uses the library. It is important you understand that there is no guarantee that this sketch will work correctly in your current product__.

__IT IS IMPORTANT TO SAY THAT THE SSB PATCH CONTENT IS NOT PART OF THIS LIBRARY. ALSO, IT IS IMPORTANT TO SAY THAT THE AUTHOR OF THIS LIBRARY DOES NOT ENCOURAGE ANYONE TO USE THE SSB PATCHES CONTENT FOR COMMERCIAL PURPOSES. IN OTHER WORDS, THIS LIBRARY ONLY SUPPORTS SSB PATCHES, THE PATCHES THEMSELVES ARE NOT PART OF THIS LIBRARY__.

 __ATTENTION: If you do not have experience with Arduino platform, please, do not try to upload the sketches available here to your receiver. It can make your device stop working.__ 

__IN NO EVENT SHALL THE AUTHOR OF THIS LIBRARY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE PRODUCT__.

__DO NOT BUY THIS RECEIVER BEFORE READING COMMENTS AND REPORTS ABOUT IT. FOLLOW THE REFERENCES BELOW:__

* [SWLing - Any thoughts on this inexpensive Si4732-based receiver?](https://swling.com/blog/2021/04/any-thoughts-on-this-inexpensive-si4732-based-receiver/)
* [Kevin O'Reilly: SI4732 The end of the line?](https://youtu.be/A3Mus-7lTrk)
* [Si47XX for Radio Experimenters](https://www.facebook.com/groups/532613604253401)
* [SI4735 SI4732 All Band Radio Receiver User Group](https://www.facebook.com/groups/340642344032449)
* [SI4732 shortwave receiver](https://swli-05940-mi.blogspot.com/2021/04/si4732-shortwave-receiver.html)


The photos below show the KIT sold on Aliexpress and eBay.

<BR> 

![PL102BA-S V:2.1 10628](./images/ali_000.png)


# Firmware Update via Desktop Arduino IDE

## New features

* The final code (HEX file) is about 2.5K smaller than the previous one (now  you have more memory to add new features)
* Band, Volume, Step, Bandwidth AGC/Attenuation are now controlled by encoder
* __"Band -:  and "Vol -"__ buttons are free to implement new features. They have no function for while.  
* All the previous status of the receiver can be rescued when you turn it on (including SSB mode, bandwidth, volume, frequency, BFO etc)
* The bandwidth now is a property of the band (you can use different bandwidth for different bands)
* Bandwidth control on FM mode (Auto, 110, 84, 60 and 40 kHz)
* FM/RDS presentation was  improved
* The seek function was improved (it is more precise on FM mode). The seek direction is controlled by the encoder (clockwise or counter-clockwise . Press encoder push button for seeking
* Steps: 1, 5, 9, 10 and 50 kHz;
* Now you can configure MW band space to 9 or 10 kHz
* New FM band from 64 to 84 MHz
* New MW band from 531 to 1701 kHz for Europe, Africa and Asia
* The frequency on Display is bigger than the previous version
* Now the bandwidth sequence is ordered by bandwidth values.


The steps below will guide you to update the firmware of the KIT

DO NOT TRY IT IF YOU DON'T KNOW WHAT ARE YOU DOING.

Be sure you are using the last version of the Arduino IDE
Read the comments of the sketch before uploading. 


## Step 1

Open the Arduino IDE and go to the __Tools__ menu and select  __Manage Libraries…__

![Firmware Update 01](./images/p01.png)

## Step 2

Look for SI4735 library and click on Update

![Firmware Update 02](./images/p02.png)


## Sept 3 

Check the version installed 

![Firmware Update 03](./images/p03.png)


## Step 4

Install the Libraries __Tiny4kOLED__ and __TinyOLED-Fonts__ Arduino libraries for OLED. Use the same approach shown before. 


![Firmware Update 04](./images/p04.png)



![Firmware Update 05](./images/p05.png)



## Step 5

Go to File Menu, Examples item  and look for the firmware for this receiver / KIT


![Firmware Update 06](./images/p06.png)



## Step 6

Check the sketch you have just loaded from PU2CLR_SI4735 Examples folder


![Firmware Update 07](./images/p07.png)


## Step 7

Select the right Arduino Board.  On Tools Menu, select Arduino AVR Boards and Arduino Nano. 
Read the comments of the sketche. 


![Firmware Update 08](./images/p08.png)



## Step 8 

Select the right Processor. On Tools menu, select “Processor:”. 
Depending on the version of the Arduino Nano used, it can be __ATmega328 (Old Bootloader)__ or __ATmega328__.
Try __ATmega328 (Old Bootloader)__ first. If you get error, try __ATmega328__.

![Firmware Update 09](./images/p09.png)


## Setup 9

Connect your Receiver (Arduino device) to your computer and select the right COM port. The COM number will depend on your computer setup.

![Firmware Update 10](./images/p10.png)


## Step 10

Finally, run the upload. 

![Firmware Update 11](./images/p11.png)



## Final result


![Firmware Update 12](./images/OLED01.png)



![Firmware Update 12](./images/OLED02.png)

