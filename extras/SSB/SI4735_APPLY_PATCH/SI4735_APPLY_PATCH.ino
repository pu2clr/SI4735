#include <SI4735.h>
#include <avr/pgmspace.h>
#include "patch_content.h"

#define RESET_PIN 12
#define AM_FUNCTION 1

// You have to set APPLY_PATCH to 1 if you reaaly want to start this process
bool APPLY_PATCH = true;
bool PATCH_FINISIHED = false;

SI4735 si4735_patch;

void setup()
{

  Serial.begin(9600);

  if (!APPLY_PATCH)
  {
    showWarning();
  }
  else
  {
    delay(1000);

    si4735_patch.setup(RESET_PIN, AM_FUNCTION);
    delay(1000);
    showFirmwareInformation();
  }
}

void showWarning()
{
  Serial.println("Read before the files attention.txt, reademe.txt or leiame.txt.");
  
}

void showFirmwareInformation()
{

  si4735_patch.getFirmware();

  Serial.println("Firmware Information.");
  Serial.print("Part Number (HEX)........: ");
  Serial.println(si4735_patch.getFirmwarePN(), HEX);
  Serial.print("Firmware Major Revision..: ");
  Serial.println(si4735_patch.getFirmwareFWMAJOR());
  Serial.print("Firmware Minor Revision..: ");
  Serial.println(si4735_patch.getFirmwareFWMINOR());
  Serial.print("Patch ID ................: ");
  Serial.print(si4735_patch.getFirmwarePATCHH(), HEX);
  Serial.println(si4735_patch.getFirmwarePATCHL(), HEX);
  Serial.print("Component Major Revision.: ");
  Serial.println(si4735_patch.getFirmwareCMPMAJOR());
  Serial.print("Component Minor Revision.: ");
  Serial.println(si4735_patch.getFirmwareCMPMINOR());
  Serial.print("Chip Revision............: ");
  Serial.println(si4735_patch.getFirmwareCHIPREV());
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
        Serial.println("Type Y if you agree with the terms or ? to see the terms!");
        delay(5000);
      }
    }
  }
  else
  {
    Serial.println("PATCH applyed. Upload a clean or new sketch to this Arduino and Shutdown it!");
    delay(5000);
  }
}

void prepereSi4735ToPatch()
{

  Wire.begin();

  si4735_patch.powerDown();

  delay(1000);


  si4735_patch.waitToSend();
        
  // Set the initial SI4735-D60 to patch mode
  
  // CTSIEN   1 -> Interrupt anabled; 
  // GPO2OEN  1 -> GPO2 Output Enable;
  // PATCH    1 -> Boot patch mode;
  // XOSCEN   1 -> Use external crystal oscillator;
  // FUNC     defaultFunction = 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
  // OPMODE   SI473X_ANALOG_AUDIO = 00000101 = Analog audio outputs (LOUT/ROUT).
  si4735_patch.setPowerUp(1, 1, 1, 1, 1, SI473X_ANALOG_AUDIO);
  // Powerup with the parameters above.
  si4735_patch.analogPowerUp();
  delay(1000);
}

void applyPatch()
{

  int offset = 0;
  int i = 0;
  byte content;

  Serial.println("Applying the patch in 5s...");
  delay(5000);
  prepereSi4735ToPatch();
  delay(1000);
  si4735_patch.waitToSend();
  Wire.beginTransmission(SI473X_ADDR);
  for (offset = 0; offset < size_content_initialization; offset += 8)
  {
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_initialization + (i + offset));
      Wire.write(content);
    }
    si4735_patch.waitToSend();
    delayMicroseconds(600);
  }
  
  Wire.endTransmission();
  si4735_patch.powerDown();
  delay(5000);

  prepereSi4735ToPatch();
  delay(1000);
  si4735_patch.waitToSend();
  Wire.beginTransmission(SI473X_ADDR);
  for (offset = 0; offset < size_content_full; offset += 8)
  {
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_full + (i + offset));
      Wire.write(content);
    }
    si4735_patch.waitToSend();
    delayMicroseconds(600);
  }
  Wire.endTransmission();
  si4735_patch.powerDown();
  delay(5000);

 
  Serial.println("Patch applyed!");

  si4735_patch.setup(RESET_PIN, AM_FUNCTION);
  delay(2000);
  si4735_patch.setSsbConfig(1, 0, 0, 1, 0, 1);
  si4735_patch.setSSB(7000, 7200,  7100, 1,2);

  si4735_patch.setSsbBfo(400);
  
  PATCH_FINISIHED = true;
}

void loop()
{
  if (APPLY_PATCH)
  {
    confirmationYouAreSureAndApply();
  }

}
