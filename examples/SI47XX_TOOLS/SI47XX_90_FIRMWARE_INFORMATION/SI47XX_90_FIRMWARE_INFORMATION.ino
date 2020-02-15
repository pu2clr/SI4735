#include <SI4735.h>

SI4735 si4735;

void setup() {

  Serial.begin(9600);
  Serial.println("Type S to seek up or s to seek down");

  // 12 -> RESET PIN
  //  0 -  FM FUNCTION
  si4735.setup(12, 0);

  // Start working on FM at 103,9MHz
  si4735.setFM(8400, 10800,  10390, 10);

  delay(500);
  si4735.setVolume(50);
  delay(500);
  showFirmwareInformation();

}


void showFirmwareInformation() {

  // si4735.getFirmware();

  Serial.println("Firmware Information.");
  Serial.print("Part Number (HEX)........: ");
  Serial.println(si4735.getFirmwarePN(), HEX);
  Serial.print("Firmware Major Revision..: ");
  Serial.println(si4735.getFirmwareFWMAJOR());
  Serial.print("Firmware Minor Revision..: ");
  Serial.println(si4735.getFirmwareFWMINOR());
  Serial.print("Patch ID ................: ");
  Serial.print(si4735.getFirmwarePATCHH(), HEX);
  Serial.println(si4735.getFirmwarePATCHL(), HEX);
  Serial.print("Component Major Revision.: ");
  Serial.println(si4735.getFirmwareCMPMAJOR());
  Serial.print("Component Minor Revision.: ");
  Serial.println(si4735.getFirmwareCMPMINOR());
  Serial.print("Chip Revision............: ");
  Serial.println(si4735.getFirmwareCHIPREV());
}

void loop() {
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case 'S':
        si4735.seekStationUp();
        break;
      case 's':
        si4735.seekStationDown();
        break;
      default:
        break;
    }
  }
  delay(300);
}
