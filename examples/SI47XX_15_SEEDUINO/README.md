# Seeeduino Xiao Board


The Seeeduino XIAO  is very small board that can be programmed on Arduino IDE platform. This board uses the powerful SAMD21 microchip, ARM® Cortex®-M0+ 32bit 48MHz, with 256KB Flash and 32KB SRAM.

See [Seeeduino XIAO](https://wiki.seeedstudio.com/Seeeduino-XIAO/) for more details.


## Seeeduino XIAO and Si4732 / Si4735 Wire up

| Si4735         | Si4732        | Seeeduino |
| -------------- | ------------  | ----------|
| RESET (pin 15) | RESET (Pin 9) |    3 (D3) |
| SDIO (pin 18)  | SDIO (pin 12) |    4 (A4) |
| CLK (pin 17)   | CLK (pin 11)  |    5 (A5) |



## Seeduino XIAO and OLED and Encoder

| Device  | Seeeduino |
| --------| ----------|
| OLED    |           |
|   SDA   |    4 (A4) |
|   SCLK  |    5 (A5) |
| Encoder |           |
|   Pin A |    1 (D1) |
|   Pin B |    2 (D2) |
|  Button |    0 (D0) |


## Seeduino XIAO and TFT ST7735

| ST7735  | Seeeduino |
| --------| ----------|
| DC      |   4 (D4)  | 
| RES     |   5 (D5)  |
| SDA     |  10 (Mosi)| 
| SCL     |   8 (SCK) |
| VCC     |  3V3      | 
| GND     |  GND      |
| BLK     |  3V3      |  

 



