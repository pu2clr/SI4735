# STM32 Si4735 Arduino Library Support 

Test and validation of the SI4735 Arduino Library.
The examples of this folder are using the STM32F103 Series. See references below to know how to setup STM32 on Arduino IDE. 


The table below shows the Si4735 and STM32F103C8 pin connections 
    
| Si4735 pin      |  Arduino Pin  |
| ----------------| ------------  |
| RESET (pin 15)  |     PA12      |
| SDIO (pin 18)   |     PB7 (B7)  |
| SCLK (pin 17)   |     PB6 (B6)  |



## STM32 setup on Arduino IDE


[Arduino core support for STM32 based boards](https://github.com/stm32duino/Arduino_Core_STM32)
[STM32CubeProgrammer software description](https://www.st.com/resource/en/user_manual/dm00403500-stm32cubeprogrammer-software-description-stmicroelectronics.pdf)
[Getting Started With Stm32 Using Arduino IDE](https://www.instructables.com/id/Getting-Started-With-Stm32-Using-Arduino-IDE/)
[Getting Started with STM32 using Arduino IDE: Blinking LED](https://circuitdigest.com/microcontroller-projects/getting-started-with-stm32-development-board-stm32f103c8-using-arduino-ide)



### Board Manager tested

* Option 1: https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
* Option 2: http://dan.drown.org/stm32duino/package_STM32duino_index.json



### Arduino IDE setup 

On __File__ menu, go to __Arduino Preferences__ and add the URL __https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json__ in __Aditional Boards Manager URLs__ text box. 

![Arduino Preferences](https://github.com/pu2clr/SI4735/blob/master/extras/images/STM32_00.png)


After, go to __Tools__, select Board and click on __Boards Manager...__ Then, install the board __STM32 Cores__. 

![Arduino Boards Manager](https://github.com/pu2clr/SI4735/blob/master/extras/images/STM32_02.png)


Finally, select the right STM32 board you are using. 

![Arduino Boards Manager](https://github.com/pu2clr/SI4735/blob/master/extras/images/STM32_03.png)


## Video

![Si4735 Arduino Library and Bluepill STM32F103 ](https://youtu.be/v4o5_lKKATc)