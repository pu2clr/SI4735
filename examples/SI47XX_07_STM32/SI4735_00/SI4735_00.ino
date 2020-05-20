/*
   Test and validation of the SI4735 Arduino Library and your circuit.
   This sketch just check if your STM32 Board and Si47XX Device is working 

   No interface is available here. 
   If your prototype is working, you will hear the receiver in the speaker. 

   The table below shows the Si4735 and STM32F103C8 pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     PA12      |
    | SDIO (pin 18)   |     PB7 (B7)  |
    | SCLK (pin 17)   |     PB6 (B6)  |

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, 2020.
*/

#include <SI4735.h>

#define RESET_PIN PA12

#define AM_FUNCTION 1
#define FM_FUNCTION 0


SI4735 si4735;

void setup()
{
  delay(500);
  si4735.setup(RESET_PIN, FM_FUNCTION);
  delay(500);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800, 10570, 10);
  // Switching to AM mode
  delay(5000);
  si4735.setAM(520, 1700, 810, 10);
}

// Main
void loop()
{

}
