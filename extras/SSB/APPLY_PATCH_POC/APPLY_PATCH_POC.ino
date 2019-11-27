
/*
   This sketch tries to apply patches on Si4735 devices.
   In this context, a patch is a piece of software used to change the behavior of the SI4735 device.

   There is little information available about patching the SI4735. The following information is the understanding
   of the author of this sketch and is not necessarily correct.

   A patch is executed internally (run by internal MCU) of the device. Usually, patches are  used to fixes bugs
   or add improvements and  new features of the firmware installed in the internal ROM of the device.
   Patches to the SI4735 are distributed in binary form and have to be transferred to the internal RAM of the device
   by the host MCU (in this case Arduino).
   Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system.
   Consequently, the content of the patch has to be transferred again to the device each time after turn on the
   system or reset the device.

   ATTENTION:
   The author of this sketch does not guarantee that this procedure will work in your development environment.
   Given this, it is at your own risk to continue with the procedures suggested here.
   This sketch works with the I2C communication protocol and is designed to apply a SSB extension PATCH to CI
   SI4735-D60. Once again, the author disclaims any liability for any damage this procedure may cause to your
   SI4735 or other devices that you are using. If you are sure about it and still want to apply the patch,
   set the variable "APPLY_PATCH to true".

   By Ricardo Lima Caratti, Nov 2019

*/

#include <Wire.h>
#include "patch_content.h"

#define SI473X_ADDR 0x11    // SI473X I2C buss address
#define POWER_UP 0x01       // Power up device and mode selection.

#define RESET_PIN 12


// Set the variable below to true if you want to apply the patch.
bool APPLY_PATCH = false;
bool FIRMWARE_OK = false;

const int size_content_initialization = sizeof ssb_patch_content_initialization;
const int size_content_full = sizeof ssb_patch_content_full;


void setup() {

  Serial.begin(9600);
  while (!Serial);

  if ( !APPLY_PATCH ) {
    Serial.println("Set the APPLY_PATCH variable to true if you really want to run this sketch and upload it again!");
    while (1);
  } else {
    firmwarePowerUp();
    if ( !FIRMWARE_OK ) {
      Serial.println("Check if your firmware is compatible! If so, set FIRMWARE_OK to true and uplaod this sketch again.");
      while (1);
    }

    // Aplay the patch
    patchPowerUp();
    downloadPatch();
  

  }

}

/*
   Reset the device
*/
void reset() {
  pinMode(RESET_PIN, OUTPUT);
  delayMicroseconds(500);
  digitalWrite(RESET_PIN, LOW);
  delay(500);
  digitalWrite(RESET_PIN, HIGH);
  delay(500);
}


/*
   Wait for the si473x gets ready (Clear to Send status bit have to be 1).
*/
inline void waitCTS() {
  do
  {
    delayMicroseconds(600);
    Wire.requestFrom(SI473X_ADDR, 1);
  } while (!(Wire.read() & B10000000));
}

/*
 * Normal Start Up the SI4735 (AM FUNCTION)
 * See Si47XX PROGRAMMING GUIDE; AN332; page 129
*/
void ssbPowerUp() {
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0b00010001); // Set to AM/SSB, disable interrupt; disable GPO2OEN; boot normaly; enable External Crystal Oscillator  .
  Wire.write(0b00000101); // Set to Analog Line Input.
  Wire.endTransmission();
  waitCTS();
}

/*
 * The first command that is sent to the device is the POWER_UP command to confirm that
 * the patch is compatible with the internal device library revision.
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.
*/
void firmwarePowerUp() {
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0b00011111); // Set to Read Library ID, disable interrupt; disable GPO2OEN; boot normaly; enable External Crystal Oscillator  .
  Wire.write(0b00000101); // Set to Analog Line Input.
  Wire.endTransmission();
  waitCTS();

  Wire.requestFrom(SI473X_ADDR, 8);
  Serial.println("Firmware Information.:");
  for (int i = 0; i < 8; i++)
  {
    Serial.println(Wire.read(), HEX);
  }
}

/*
 * Start Up with patch enabled
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.
 */
void patchPowerUp() {
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP);
  Wire.write(0b00110001); // Set to AM, Enable External Crystal Oscillator; Set patch enable; GPO2 output disabled; CTS interrupt disabled.
  Wire.write(0b00000101); // Set to Analog Output
  Wire.endTransmission();
  waitCTS();
}

/*
 * Transfer the content of the patch (ssb_patch_content_full) to the SI4735
 */
void downloadPatch() {
  byte content, cmd_status;
  int i, line, offset;
  Serial.println("Applying patch...");
  // Send patch for whole SSBRX full download
  for (offset = 0; offset < size_content_full; offset += 8)
  {
    line++;
    Wire.beginTransmission(SI473X_ADDR);
    for (i = 0; i < 8; i++)
    {
      content = pgm_read_byte_near(ssb_patch_content_full + (i + offset));
      Wire.write(content);
    }
    Wire.endTransmission();
    waitCTS();
    Wire.requestFrom(SI473X_ADDR, 1);
    cmd_status = Wire.read();
    if (cmd_status != 0x80) {
      Serial.print("Status/Error: ");
      Serial.print(cmd_status, BIN);
      Serial.print("; linha: ");
      Serial.print(line);
      Serial.print("; offset: ");
      Serial.println(offset + i);
    }
    waitCTS();
  }
  Serial.println("Patch applied!");
}

/* 
 *  Sets the SI4735 to work on SSB
 */
void setSSB() {
  // TO DO
}

/*
 * Sets the frequency of the receiver
 */
void setFrequency(unsigned frequency) {
  // TO DO 
}

/*
 * Sets the BFO offset
 */
void setBFO() {
  // TO DO
}


void loop() {


}
