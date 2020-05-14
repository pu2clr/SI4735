/*
  This sketch stores the SI4735 SSB patch content on an EEPROM.
  It might useful for Board or MCU with few memory.
  The Idea is saving memory of your MCU by storing a SI47XX patche in an external memory (EEPROM)

  Under construction....

  Ricardo Lima Caratti 2020
*/

// under construction ...
#include <Wire.h>

#include "patch_init.h" // SSB patch for whole SSBRX initialization string

const uint16_t size_content = sizeof ssb_patch_content; // see ssb_patch_content in patch_full.h or patch_init.h

//defines the EEPROM I2C addresss.
#define EEPROM_I2C_ADDR 0x50 // You might need to change this value

#define EEPROM_OFFSET 10     // START WRITING in eeprom 

char buffer(80);

typedef union {
  struct
  {
    uint8_t lowByte;
    uint8_t highByte;
  } raw;
  uint8_t  byteArray[2];
  uint16_t value;
} bytes_to_word16;


// Interger to bytes representation
bytes_to_word16 size_value;
const uint8_t content_id[] =    "SI4735-D60-init-"; // 16 bytes
// const uint8_t content_id[] = "SI4735-D60-full-"; // 16 bytes
const uint16_t size_id = sizeof content_id -1 ;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  showMsg("Storing the patch file..");

  showMsgValue("Size of patch ID.....: %u bytes.", size_id);
  showMsgValue("Size of patch content: %u bytes.", size_content);
  uint32_t t1 =  millis();
  eepromWritePatch();
  // clearEeprom();
  uint32_t t2 = millis();
  showMsgValue("Finish! Elapsed time: %ul milliseconds.", t2 - t1);

  showMsg("Checking the values stored");
  checkPatch();


}


void showMsg(const char *msg) {
  Serial.println(msg);
}

void showMsgValue(const char *msg, uint16_t value) {
  char buffer[80];
  sprintf(buffer, msg, value);
  Serial.println(buffer);
}



void eepromWritePatch() {

  bytes_to_word16 size_patch;
  int nBlk =  size_content / 8;
  int nOffset;
  uint8_t content[8];

  // Stores the identification of the patch
  eepromWriteBlock(EEPROM_I2C_ADDR, EEPROM_OFFSET + 0, &content_id[0], 8); // Writes the first 8 bytes of the identification
  eepromWriteBlock(EEPROM_I2C_ADDR, EEPROM_OFFSET + 8, &content_id[8], 8); // Writes the last  8 bytes of the identification

  // Stores the patch size (unsigned integer - 16 bits)
  size_patch.value = size_content;
  
  eepromWriteBlock(EEPROM_I2C_ADDR, EEPROM_OFFSET + 16, size_patch.byteArray, 2); // Writes the size of the patch (integer value)
  nOffset = EEPROM_OFFSET + 16 + 2;

  // Stores the patch content
  for (int i = 0; i < nBlk; i++ ) {
    for (int k = 0; k < 8; k++ )  {
      content[k] = pgm_read_byte_near(ssb_patch_content + (i * 8) + k);
    }

    /*
      if ( nOffset < 64 ) {
      for (int k = 0; k < 8; k++ ) Serial.println(content[k],HEX);
      } */

    eepromWriteBlock(EEPROM_I2C_ADDR, nOffset, content, 8);
    nOffset += 8;
  }

}

void  eepromWriteBlock(uint8_t i2c_address, uint16_t offset, uint8_t const * pData, uint8_t blockSize)
{
  bytes_to_word16 eeprom;
  eeprom.value = offset;

  Wire.beginTransmission(i2c_address);
  Wire.write(eeprom.raw.highByte); // Most significant Byte
  Wire.write(eeprom.raw.lowByte);  // Less significant Byte

  for (int i = 0; i < blockSize; i++)
    Wire.write(*pData++); //dispatch the data bytes

  Wire.endTransmission();
  delay(5);

}


void checkPatch() {

  bytes_to_word16 eeprom;
  bytes_to_word16 patch_size;

  delay(500);


  // Shows the patch identification stored in the eeprom
  Serial.print("\nStored Patch ID.........: ");
  Wire.beginTransmission(EEPROM_I2C_ADDR);
  Wire.write(0x00);
  Wire.write(EEPROM_OFFSET);
  Wire.endTransmission();
  delay(5);
  // Shows the patch identification.
  Wire.requestFrom(EEPROM_I2C_ADDR, 16);
  for ( int k = 0; k < 16; k++)  {
    uint8_t c = Wire.read();
    Serial.write(c);
  }
  delay(5);

  // Shows the size of the patch stored in the eeprom.
  Serial.print("\nStored patch size.......: ");
  eeprom.value = EEPROM_OFFSET + 16;
  Wire.beginTransmission(EEPROM_I2C_ADDR);
  Wire.write(eeprom.raw.highByte); // Most significant Byte
  Wire.write(eeprom.raw.lowByte);  // Less significant Byte
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(EEPROM_I2C_ADDR, 2);
  patch_size.byteArray[0] = Wire.read();
  patch_size.byteArray[1] = Wire.read();
  Serial.print(patch_size.value);

  Serial.println("\nReading Patch Content sample......");
  Serial.println("\nFirst 8 patch commands sample.");

  dumpEeprom(EEPROM_OFFSET + 16 + 2, 8);

  Serial.println("\nLast 8 patch commands sample.");
  dumpEeprom(EEPROM_OFFSET + 16 + 2 + size_content - 64, 8);

}


void dumpEeprom(uint16_t offset, int sample) {

  bytes_to_word16 eeprom;

  delay(500);

  eeprom.value = offset;

  for ( int i = 0; i < sample; i++) {

    Wire.beginTransmission(EEPROM_I2C_ADDR);
    Wire.write(eeprom.raw.highByte); // offset Most significant Byte
    Wire.write(eeprom.raw.lowByte);  // offset Less significant Byte
    Wire.endTransmission();
    delay(5);

    Wire.requestFrom(EEPROM_I2C_ADDR, 8);
    for ( int k = 0; k < sample; k++) {
      uint8_t c = Wire.read();
      Serial.print(c, HEX); // SHows patch id
      Serial.print(" ");
    }
    Serial.print("\n");
    eeprom.value += 8;
    delay(5);
  }
}



// Erases all eeprom content
void clearEeprom()
{
  uint8_t content[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  for ( int i = 0; i < 16384 ; i += 8 ) {
    Wire.beginTransmission(EEPROM_I2C_ADDR);
    eepromWriteBlock(EEPROM_I2C_ADDR, i, content, 8);
  }
}

void loop()
{
}
