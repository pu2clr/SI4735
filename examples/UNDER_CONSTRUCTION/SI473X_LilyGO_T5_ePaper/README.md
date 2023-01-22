# LilyGO T5 ePaper

[Click here know how to setup the LilyGO ePaper](https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series)

1. Download the repository above to your computer
2. Unzip the downloaded file
3. Go to the lib folder and copy all the content to your Arduino folder library (in general Documents/Arduino/libraries)
4. Install / configure the ESP32 on your Arduino IDE
    * On Arduino IDE, go to Preferences, edit the field "Additional Boards Manager URLs:" and add the URL 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    * Go to Tools menu, select Boards, Board Manager and add the ESP32 board.
5. On Arduino IDE, select __ESP32 Dev Module__
6. Connect the LilyGO T5 ePaper board
6. select the correspondent port (COM<x> or /dev/xxx.yyy )
7. compile and upload the sketch

