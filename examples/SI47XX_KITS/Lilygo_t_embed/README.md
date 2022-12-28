# LilyGO/T-Embed setup

LilyGO/T-Embed  is a panel that uses the ESP32S, an rotary encoder with push button, and a TFT ST7789 display, all together,  for application development. The picture below shows LilyGO/T-Embed.

<BR> 

![LilyGo T-Embed](./images/t_embed.jpg)


As you can see in the picture above, the LilyGO/T-Embed has some extra pins that you can use in your application. See Grove, GPIO PINOUT, and TFT Card Slot.  Preferably, use the pins SDA, SCL, IO16, IO17, IO40, IO38 and IO41.  The picture below shows the ESP32 pinout.    


![ESP32 S](./images/ESP32_S.jpg)

<BR>


[See this link for more details about LilyGo T-Embed](https://github.com/Xinyuan-LilyGO/T-Embed)


## Arduino setup

On Arduino IDE, go to Preferences, edit the field "Additional Boards Manager URLs:" and add the URL 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json . 

After, on Tools menu, select the option "Board:", "Boards Manager...". 
Look for esp32 and install it.


[Go to LilyGo T-Embed github repository](https://github.com/Xinyuan-LilyGO/T-Embed) and download it. 
Unzip the downloaded file, go to lib folder, copy and paste all files and folders to the your Arduino installation, libraries folder. 


## References 

* [Go to LilyGo T-Embed github repository](https://github.com/Xinyuan-LilyGO/T-Embed)
* [LILYGO Unveils the T-Embed ESP32-S3-Powered Universal Remote for Smart Home Projects and More](https://www.hackster.io/news/lilygo-unveils-the-t-embed-esp32-s3-powered-universal-remote-for-smart-home-projects-and-more-cad2e701d640)





