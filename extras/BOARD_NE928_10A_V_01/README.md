# About the board based on Si4730 labeled "NE928-10A V:01" 

This board is an AM (MW) and FM receiver. __It is based on SI4730-B20__. The PU2CLR SI4735 Arduino Library is compatible with it.  

Unlike the [PL102B board based on SI4730-D60](https://pu2clr.github.io/SI4735/extras/BOARD_PL102BA/), a very similar board and also supported by the Si4735 Arduino Library, __the NE928-10A DOES NOT SUPPORT LW, SW and FM/RDS__. Actually, judging by the Silicon Labs, the NE928-10A, PL102B or any board based on SI4730 device should not support LW, SW and FM/RDS. However, some tests with the __PL102B breakout and IC SI4730-D60__ have been received SW stations with success. It is iportante to say the author of this document does not guarantee that his experiments with boards based on the SI4730-D60 will work for you. See more [about the breakout board based on SI4730-D60 labeled “PL102BA-S V:2.1 10628”](https://pu2clr.github.io/SI4735/extras/BOARD_PL102BA/)


## The photos bellow show the "NE928-10A V:01" based on SI4730-B20


### NE928-10A V:01 pinout

![NE928-10A V:01 pinout](./NE928_Si4730_00.png)

### NE928-10A V:01 label

![NE928-10A V:01 label](./NE928_Si4730_01.png)


### IC label "3020 BHFI 913" (Si4730)

![Si4730 label ](./NE928_Si4730_04.jpg)


## Arduino Pro Mini 3.3V (8MHz) connection


|  board NE928-10A V:01 |  Arduino Pro Mini |
| --------------------- | ----------------- |
| GND  | GND | 
| VA   | VCC |
| SDIO | A4  |
| SCLK | A5  |
| RST  | D12 |

### Recommended sketches

* [SI4735_01_POC.ino](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_01_SERIAL_MONITOR/SI4735_01_POC)
* [SI47XX_01_OLED_I2C.ino](https://github.com/pu2clr/SI4735/tree/master/examples/SI47XX_03_OLED_I2C/SI47XX_01_OLED_I2C)


## The Si473X I2C address 

This board comes with the SEN pin connected to +V. So the I2C bus address is 0x63


## Firmware information 

The table below shows the query result of Firmware Information command.

| Firmware Information | Value |
| ------------------- | ----- |
| Part Number (HEX) | 1E |
| Firmware Major Revision | 50 |
| Firmware Minor Revision | 48 |
| Patch ID | 00 |
| Component Major Revision | 50 |
| Component Minor Revision | 48 |
| Chip Revision | 66 |


## Video

{% include NE928.html %}

[NE928-10A V:01 board working with "Si4735 Arduino Library](https://youtu.be/An7Iq_BLxJY)

