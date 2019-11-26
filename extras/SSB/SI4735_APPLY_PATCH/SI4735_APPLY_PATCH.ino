#include <SI4735.h>
#include <avr/pgmspace.h>
#include "patch_content2.h"

#define RESET_PIN 12
#define AM_FUNCTION 1

// You have to set APPLY_PATCH to 1 if you reaaly want to start this process
bool APPLY_PATCH = true;
bool PATCH_FINISIHED = false;

SI4735 si4735;

void setup()
{

  Serial.begin(9600);

  if (!APPLY_PATCH)
  {
    showWarning();
  }
  else
  {
    si4735.setup(RESET_PIN, 15);
    Serial.println("Type Y");
  }
}

void showWarning()
{
  // Serial.println("Read before the files attention.txt, reademe.txt or leiame.txt.");
}

void confirmationYouAreSureAndApply()
{

  if (!PATCH_FINISIHED)
  {

    if (Serial.available() > 0)
    {
      char key = Serial.read();
      if (key == 'Y')
      {
        applyPatch();
      }
      else if (key == '?')
      {
        showWarning();
      }
      else
      {
        // Serial.println("Type Y if you agree with the terms or ? to see the terms!");
        delay(5000);
      }
    }
  }
  else
  {
    // Serial.println("PATCH applyed. Upload a clean or new sketch to this Arduino and Shutdown it!");
    delay(5000);
  }
}

/*
   Power Up with patch configuration
   See Si47XX PROGRAMMING GUIDE; page 219 and 220
*/
void prepereSi4735ToPatch()
{

  /*  
  si4735.powerDown();

  // reset
  pinMode(RESET_PIN, OUTPUT);
  delay(250);
  digitalWrite(RESET_PIN, LOW);
  delay(250);
  digitalWrite(RESET_PIN, HIGH);

  delay(500);

  // POWER_UP to get Firmware Information
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0xCF); // Set to Read Library ID, Enable Interrupts.
  Wire.write(0x50); // Set to Analog Line Input.
  Wire.endTransmission();
  delayMicroseconds(2500);
  */
  Wire.requestFrom(SI473X_ADDR, 8);

  Serial.println("Firmware Inf.:");
  for (int i = 0; i < 8; i++)
  {
    Serial.println(Wire.read(), HEX);
  }

  si4735.waitToSend();

  // POWER_UP to start patch mode
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0xE2); // Set to FM Transmit, set patch enable, enable interrupts.
  Wire.write(0x50); // Set to Analog Line Input.
  Wire.endTransmission();
  delayMicroseconds(2500);

  si4735.waitToSend();
}

void applyPatch()
{

  int offset = 0;
  int i = 0;
  byte content;
  byte cmd_status;

  Serial.println("Applying.");
  delay(500);

  prepereSi4735ToPatch();

  si4735.waitToSend();
  /*
  // Send patch for whole SSBRX initialization string
  for (offset = 0; offset < size_content_initialization; offset += 8)
  {
    si4735.waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_initialization + (i + offset));
      Wire.write(content);
    }
    Wire.endTransmission();
    delayMicroseconds(600);
    // if ( offset > 80 and (offset % 80) == 0 )  Serial.println(offset);
    Wire.requestFrom(SI473X_ADDR, 1);
    cmd_status = Wire.read();
    if (cmd_status != 0x80) Serial.println(cmd_status, BIN);
  } 
  */
  delay(500);
  
  // Send patch for whole SSBRX full download
  for (offset = 0; offset < size_content_full; offset += 8)
  {
    Wire.beginTransmission(SI473X_ADDR);
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_full + (i + offset));
      Wire.write(content);
    }
    Wire.endTransmission();
    // if ( offset > 80 and (offset % 80) == 0 ) Serial.println(offset);
    delayMicroseconds(600);
    Wire.requestFrom(SI473X_ADDR, 1);
    cmd_status = Wire.read();
    if (cmd_status != 0x80) Serial.println(cmd_status, BIN);
    si4735.waitToSend();
  } 
  
  delay(500);
  Serial.println("Applied!");

  delay(500);
  si4735.setPowerUp(0, 0, 0, 1, 1, SI473X_ANALOG_AUDIO);
  delay(500);
  si4735.analogPowerUp();
  delay(500);
  si4735.setSsbConfig(1, 1, 0, 0, 0, 1);
  delay(500);
  si4735.setSSB(28350, 28550, 28400, 1, 2);
  si4735.setVolume(62);
  si4735.frequencyUp();
  delay(500);
  si4735.frequencyDown();

  si4735.setSsbBfo(-900);
  delay(10000);
  si4735.setSsbBfo(0);
  delay(10000);
  si4735.setSsbBfo(300);
  delay(10000);
  si4735.setSsbBfo(600);

  PATCH_FINISIHED = true;
}

void loop()
{
  if (APPLY_PATCH)
  {
    confirmationYouAreSureAndApply();
  }
}
