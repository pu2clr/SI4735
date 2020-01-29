/*
    This sketch saves the SI4735 SSB patch content on an EEPROM.
    It migh useful for Board or MCU with few memory.

    Ricardo Lima Caratti Dec, 2020    
*/

// under construction ...

#include <SI4735.h>


#define EEPROM_I2C_ADDR   0


void setup() {


}

void eepromWrite(int offset, byte val, int i2c_offset)
{
  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_offset);
 
  // Send memory offset as two 8-bit bytes
  Wire.write((int)(offset >> 8));   // Most significant Byte 
  Wire.write((int)(offset & 0xFF)); // Less significant Byte
 
  // Send data to be stored
  Wire.write(val);
 
  // End the transmission
  Wire.endTransmission();
 
  // Add 5ms delay for EEPROM
  delay(5);
}

// Erase all eeprom content
void eepromClear() {
}



void loop() {


}
