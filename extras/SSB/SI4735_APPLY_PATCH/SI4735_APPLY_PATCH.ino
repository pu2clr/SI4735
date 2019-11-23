#include <SI4735.h>
#include <avr/pgmspace.h>
#include "patch_content.h"


#define RESET_PIN 12
#define AM_FUNCTION 1


// You have to set APPLY_PATCH to 1 if you reaaly want to start this process
bool APPLY_PATCH = true;
bool PATCH_FINISIHED = false;



SI4735 si4735_patch;


void setup() {

  Serial.begin(9600);

  if ( !APPLY_PATCH ) {
    showWarning();
  } else {
    delay(1000);

    si4735_patch.setup(RESET_PIN, AM_FUNCTION);
    delay(1000);      
    showFirmwareInformation();  
  }


}



void showWarning() {

  Serial.println("The author of this program does not guarantee that this procedure will work in your development environment.");
  Serial.println("Given this, it is at your own risk to continue with the procedures suggested here.");
  Serial.println("This sketch works with the I2C communication protocol and is designed to apply a SSB extension PATCH to CI SI4735-D60.");
  Serial.println("Once again the author disclaims any liability for any damage this procedure may cause to your CI SI4735 or other devices that you are using.");
  Serial.println("If you are sure about it, set the variable APPLY_PATCH to true and upload this sketch again!");
  delay(20000);

}


void showFirmwareInformation() {

    si4735_patch.getFirmware();
    
    Serial.println("Firmware Information.");
    Serial.print("Part Number (HEX)........: ");
    Serial.println(si4735_patch.getFirmwarePN(), HEX);
    Serial.print("Firmware Major Revision..: ");
    Serial.println(si4735_patch.getFirmwareFWMAJOR());
    Serial.print("Firmware Minor Revision..: ");
    Serial.println(si4735_patch.getFirmwareFWMINOR());
    Serial.print("Patch ID ................: ");
    Serial.print(si4735_patch.getFirmwarePATCHH(),HEX);
    Serial.println(si4735_patch.getFirmwarePATCHL(),HEX);
    Serial.print("Component Major Revision.: ");
    Serial.println(si4735_patch.getFirmwareCMPMAJOR());
    Serial.print("Component Minor Revision.: ");
    Serial.println(si4735_patch.getFirmwareCMPMINOR());
    Serial.print("Chip Revision............: ");
    Serial.println(si4735_patch.getFirmwareCHIPREV());
}


void confirmationYouAreSureAndApply() {

  if ( !PATCH_FINISIHED ) {

    if (Serial.available() > 0)
    {
      char key = Serial.read();
      if ( key == 'Y') {
        applyPatch();
      } else if ( key == '?') {
        showWarning();
      } else {
        Serial.println("Type Y if you agree with the terms or ? to see the terms!");
        delay(5000);
      }

    }
  } else {
    Serial.println("PATCH applyed. Upload a clean or new sketch to this Arduino and Shutdown it!");
    delay(5000);
  }
}


void prepereSi4735ToPatch() {

    Wire.begin();
    // Set the initial SI473X behavior
    // CTSIEN   0 -> Interrupt anabled;
    // GPO2OEN  1 -> GPO2 Output Enable;
    // PATCH    1 -> Boot patch mode;
    // XOSCEN   1 -> Use external crystal oscillator;
    // FUNC     defaultFunction = 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
    // OPMODE   SI473X_ANALOG_AUDIO = 00000101 = Analog audio outputs (LOUT/ROUT).
    si4735_patch.setPowerUp(0, 1, 1, 1, 1, SI473X_ANALOG_AUDIO);
    // Powerup with the parameters above.
    si4735_patch.analogPowerUp();
    delay(1000);

}


void applyPatch() {

  int offset = 0;
  int i = 0;
  byte content;

  Serial.println("Applying the patch...");

  Serial.print("Sending ");
  Serial.print(size_content_initialization);
  Serial.println(" bytes to the CI...");

  delay(5000);
  for (offset; offset < size_content_initialization; offset += 8 ) {

    Serial.println("Sending..");
    for (i = 0; i< 8; i++) {
      content = pgm_read_byte_near(ssb_patch_content_initialization + (i + offset));
        Serial.print(content, HEX);
        Serial.print(" ");
    }
    Serial.println("Waiting...");
    delay(50);    
  }
  delay(5000);

  Serial.println("Patch applyed!");
  PATCH_FINISIHED = true;

}




void loop() {
  if (APPLY_PATCH) {
    confirmationYouAreSureAndApply();
  } else {

  }
}
