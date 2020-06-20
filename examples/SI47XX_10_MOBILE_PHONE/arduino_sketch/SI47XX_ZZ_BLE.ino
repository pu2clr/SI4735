/*
   Test and validation of the SI4735 Arduino Library.
   This example shows how can setup the radio on SW by using the Si4735 Arduino Library.
   This sketch divides the HF spectrum in many bands. See band table.
   
   ATTENTION:  Please, avoid using the computer connected to the mains during testing.

   The main advantages of using this sketch are:
    1) It is a easy way to check if your circuit is working;
    2) You do not need to connect any display device to make your radio works;
    3) You do not need connect any push buttons or encoders to change volume and frequency;
    4) The Arduino IDE is all you need to control the radio.

   This sketch has been successfully tested on:
    1) Pro Mini 3.3V; 
    2) UNO (by using a voltage converter); 
    3) Arduino Yún;
    4) Arduino Mega (by using a voltage converter); and 
    5) ESP32 (LOLIN32 WEMOS)

    The table below shows the Si4735 and Arduino Pro Mini pin connections 
    
    | Si4735 pin      |  Arduino Pin  |
    | ----------------| ------------  |
    | RESET (pin 15)  |     12        |
    | SDIO (pin 18)   |     A4        |
    | SCLK (pin 17)   |     A5        |


  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/
  Schematic: https://github.com/pu2clr/SI4735/blob/master/extras/images/basic_schematic.png

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>
#include <SoftwareSerial.h>

#define BLUETOOTH_TX 10
#define BLUETOOTH_RX 11


#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0


typedef struct {
  const char *freqName;
  uint16_t   minimumFreq;
  uint16_t   maximumFreq;
  uint16_t   currentFreq;
  uint16_t   currentStep;
} Band;

Band band[] = {
    {"FM", 8400, 10800, 10650, 10},
    {"AM", 570, 1710, 810, 10},
    {"60m", 4700, 5200, 4850, 5},
    {"49m", 5700, 6200, 6000, 5},
    {"41m", 7100, 7600, 7300, 5},
    {"31m", 9300, 10000, 9600, 5},
    {"25m", 11400, 12200, 11940, 5},
    {"22m", 13500, 13900, 13600, 5},
    {"19m", 15000, 15800, 15200, 5},
    {"16m", 17400, 17900, 17600, 5},
    {"13m", 21400, 21800, 21500, 5},
    {"11m", 25600, 27500, 27220, 1}};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  idxFreq = 0; // Default SW band is 31M
int lastSw = 2;
uint16_t currentFrequency;


// Initiate BLE (HM10) Instance
SoftwareSerial ble(BLUETOOTH_TX, BLUETOOTH_RX);

SI4735 si4735;

void setup()
{
  ble.begin(9600);
 
  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(band[idxFreq].minimumFreq, band[idxFreq].maximumFreq,  band[idxFreq].currentFreq, band[idxFreq].currentStep);

  delay(500);

  currentFrequency = si4735.getFrequency();
  si4735.setVolume(55);
  showStatus();
}


// Show current frequency and status
void showStatus()
{
  String sFreq;  
  String sUnit;

  band[idxFreq].currentFreq = currentFrequency = si4735.getFrequency();

  if (si4735.isCurrentTuneFM() ) {
    sFreq = String(currentFrequency / 100.0, 2);
    sUnit = "MHz";
  } else {
    sFreq = String(currentFrequency); 
    sUnit = "KHz";
  }

  ble.print(sFreq + " " + sUnit + "\n");

}


// Send VFO/BFO database to mobile device
void sendDatabase()
{
  // Building  JSON string  =>  {"band":["MW  ", "SW2  "...]}
  String jsonStr = "#B{\"bands\":[";  // #J Means that a Json information will be processed by the SmartPhone
  short i;

  // Building Json string to send to the mobile device

  // Bands table
  for (i = 0; i < lastBand -1 ; i++)
  {
    jsonStr.concat("{\"name\":\"");
    jsonStr.concat(band[i].freqName);
    jsonStr.concat("\", \"unt\":\"");
    jsonStr.concat("KHz");
    jsonStr.concat("\"},");
  }
  jsonStr.concat("{\"name\":\"");
  jsonStr.concat(band[i].freqName);
  jsonStr.concat("\", \"unt\":\"");
  jsonStr.concat("KHz");
  jsonStr.concat("\"}]}\n"); // '\n' means the and of the message
  // Send json Band table to the mobile device
  ble.print(jsonStr);
}

// Process a long message sent by the Smartphone (message started with '#')
void processMessage()
{
  String buffer = ble.readString();
  // Serial.println(buffer);
  // TO DO
}


// Main
void loop()
{

  if (ble.available())
  {
    // Just testing. Will be improved
    char c = ble.read(); // Get message from mobile device (Smartphone)
    switch (c)
    {
    case '+':
        si4735.frequencyUp();
        showStatus();     
      break;
    case '-':
        si4735.frequencyDown();
        showStatus();
      break;
     case 'd':
      // sendDatabase(); // Send VFO/BFO information (Bands, Steps and current status) to mobile device
      break;
    case 'm':
      idxFreq = 1;
      si4735.setAM(band[idxFreq].minimumFreq, band[idxFreq].maximumFreq, band[idxFreq].currentFreq, band[idxFreq].currentStep);
      showStatus();    
      break;
    case 's':
      if (idxFreq > lastBand) 
         idxFreq = 2; 
      else if ( idxFreq < 2) 
         idxFreq = lastSw;
      else 
         idxFreq++;

      lastSw = idxFreq;      
      si4735.setAM(band[idxFreq].minimumFreq, band[idxFreq].maximumFreq, band[idxFreq].currentFreq, band[idxFreq].currentStep);
      showStatus();
      break;
    case 'f':
      idxFreq = 0;
      si4735.setFM(band[idxFreq].minimumFreq, band[idxFreq].maximumFreq,  band[idxFreq].currentFreq, band[idxFreq].currentStep);
      showStatus();
      break;
    case '>':
      si4735.volumeUp();
      break;
    case '<':
      si4735.volumeDown();
      break; 
    case '#':
      // Follow the protocol
      processMessage();
      break;
    default:
      break;
    }
  } // end bluetooth control
   delay(2);
}
