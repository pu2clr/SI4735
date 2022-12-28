# LilyGO/T-Embed setup

![LilyGo T-Embed](./images/t_embed.jpg)


As you can see in the picture above, the LilyGO/T-Embed has some extra pins that you can use in your application. See Grove, GPIO PINOUT, and TFT Card Slot.  Preferably, use the pins SDA, SCL, IO16, IO17, IO40, IO38 and IO41.  The picture below shows the ESP32 pinout.    


![ESP32 S](./images/ESP32_S.jpg)


[See this link for more details about LilyGo T-Embed](https://github.com/Xinyuan-LilyGO/T-Embed)


## Arduino setup

On Arduino IDE, go to Preferences, edit the field "Additional Boards Manager URLs:" and add the URL 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json . 

After, on Tools menu, select the option "Board:", "Boards Manager...". 
Look for esp32 and install it.


[Go to LilyGo T-Embed github repository](https://github.com/Xinyuan-LilyGO/T-Embed) and download it. 
Unzip the downloaded file, go to lib folder, copy and paste all files to ypu Arduino installation, libraries folder. 







