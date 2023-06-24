/*
   RDS test and validation u thesing the SI4735 Arduino Library.
   It is an FM, MW and SW (1700kHz to 30000kHz)
   
   ATTENTION:  Please, avoid using a computer connected to the mains during testing. Doing so may result
                in interference which may interefere with the radio tuner chip.

   With this sketch, you only need the Arduino IDE and its Serial monitor to test and
   control the circuit.
   This means:
    1) You don't need buttons, rotary encoders, potentiometers, etc. to control the circuit
    2) You don't need a screen to see output from the Arduino board
    3) All functions supported by this sketch can be accessed over the serial monitor
   
   This sketch has been successfully tested on:
    1) Pro Mini 3.3V
    2) UNO (by using a voltage converter)
    3) Arduino Yún
    4) Arduino Mega (by using a voltage converter)
    5) ESP32 (LOLIN32 WEMOS)


    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     15/A1     |
    | SDIO (pin 18)   |     A4        |
    | CLK (pin 17)    |     A5        |


  I strongly recommend starting with this sketch.

  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#define RESET_PIN 14

#define FM_FUNCTION 0

#define PULLING_RDS 40

long pulling_rds = millis();


// uint16_t currentFrequency = 9470; // Your local FM station with RDS/RBDS service
uint16_t currentFrequency = 8990; // Your local FM station with RDS/RBDS service

char buffer[120];

SI4735 rx;

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  digitalWrite(RESET_PIN, HIGH);
  
  Serial.println(" FM station tuning and RDS test.");

  showHelp();

  // Look for the Si47XX I2C bus address
  int16_t si4735Addr = rx.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1);
  } else {
    Serial.print("The SI473X / SI474X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }

  delay(500);
  rx.setup(RESET_PIN, FM_FUNCTION);

  rx.setFM(8400, 10800, currentFrequency, 10);
  delay(500);
  rx.setRdsConfig(3, 3, 3, 3, 3);
  rx.setFifoCount(1);
  rx.setVolume(40);
  showStatus();
}

void showHelp()
{
  Serial.println("FM RDS test");
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  rx.getStatus();
  rx.getCurrentReceivedSignalQuality();

  currentFrequency = rx.getFrequency();

  rx.getCurrentReceivedSignalQuality();
  sprintf(buffer,"\n %u | SNR: %d db | RSSI: %d dBuV", currentFrequency, rx.getCurrentSNR(), rx.getCurrentRSSI() );
  Serial.print(buffer);
}

// Begin RDS 
char *utcTime;
char *stationName;
char *programInfo;

void checkRds() {
  rx.rdsBeginQuery(); // gets RDS information
  utcTime = rx.getRdsDateTime();
  stationName = rx.getRdsStationName();
  programInfo = rx.getRdsProgramInformation();
  buffer[0] = '\n'; // New line control
  buffer[1] = '\0'; // end of string char array
  
  if ( stationName != NULL ) {
    strcat(buffer,"| Station Name: ");
    strcat(buffer, stationName);
  }
  if ( programInfo != NULL ) {
    strcat(buffer," | Program information: ");
    strcat(buffer, programInfo);
  }

  if ( utcTime != NULL ) {
    strcat(buffer," | Time (UTC): ");
    strcat(buffer, utcTime);
  }

  Serial.print("\ngetNumRdsFifoUsed: ");
  Serial.print(rx.getNumRdsFifoUsed() );
  Serial.print("\nSync: ");
  if ( rx.getNumRdsFifoUsed() > 0 ) {
  Serial.print(rx.getRdsSync() );
  if (strlen(buffer) > 4)  Serial.print(buffer);
  }

}

// End RDS

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
    case '+':
      rx.volumeUp();
      break;
    case '-':
      rx.volumeDown();
      break;
    case 'U':
    case 'u':
      rx.frequencyUp();
      break;
    case 'D':
    case 'd':
      rx.frequencyDown();
      break;
    case 'S':
      rx.seekNextStation();
      break;
    case 's':
      rx.seekPreviousStation();
      break;
    case '?':
      showHelp();
      break;
    default:
      Serial.println("Command error.");
      showHelp();
      break;
    }
    showStatus();
  }

  if ( (millis() - pulling_rds) > PULLING_RDS ) {
    checkRds();
    pulling_rds = millis();
  }  

  delay(5);
}
