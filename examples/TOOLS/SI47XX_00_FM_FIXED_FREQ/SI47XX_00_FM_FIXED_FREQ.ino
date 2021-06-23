/*
  This sketch ca be used to check your circuit when you do not have any visual output.
  For example: standalone ATmega328. 
     
  ESP32 and SI4735-D60 or SI4732-A10 wire up

  | Si4735  | SI4732   | DESC.  | ATmega328 Pin   |
  |---------| -------- |--------|-----------------|
  | pin 15  |  pin 9   | RESET  |   12            |  
  | pin 18  |  pin 12  | SDIO   |   A4  (SDA)     |
  | pin 17  |  pin 11  | SCLK   |   A5  (SCL)     |

  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, May 2021.
*/

#include <SI4735.h>

#define RESET_PIN 12
#define FM_STATION_FREQ 10650  // 106.5 MHz - Select a better FM station frequency for you

SI4735 rx;

void setup()
{
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    while (1);
  }

  delay(500);
  rx.setup(RESET_PIN, 0);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 106.5 MHz; step 100kHz)
  rx.setFM(8400, 10800, FM_STATION_FREQ, 10);
  delay(300);
  rx.setVolume(35);
}

void loop () {

}
