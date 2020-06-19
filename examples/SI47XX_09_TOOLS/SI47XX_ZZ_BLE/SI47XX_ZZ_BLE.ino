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


Band band[] = {{"60m",4700, 5200, 4850, 5},
  {"49m",5700, 6200, 6000, 5},
  {"41m",7100, 7600, 7300, 5},
  {"31m",9300, 10000, 9600, 5},
  {"25m",11400, 12200, 11940, 5},
  {"22m",13500, 13900, 13600, 5},
  {"19m",15000, 15800, 15200, 5},
  {"16m",17400, 17900, 17600, 5},
  {"13m",21400, 21800, 21500, 5},
  {"11m",25600, 27500, 27220, 1}
};

const int lastBand = (sizeof band / sizeof(Band)) - 1;
int  currentFreqIdx = 3; // Default SW band is 31M

uint16_t currentFrequency;


// Initiate BLE (HM10) Instance
SoftwareSerial ble(BLUETOOTH_TX, BLUETOOTH_RX);

SI4735 si4735;

void setup()
{
  // Serial.begin(9600);
  // while(!Serial);

  showHelp();
  
  // ble.begin(57600);
  ble.begin(9600);
  
  // Serial.println("TESTE");
 
  // gets and sets the Si47XX I2C bus address.
  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if ( si4735Addr == 0 ) {
    // Serial.println("Si473X not found!");
    // Serial.flush();
    while (1);
  } else {
    // Serial.print("The Si473X I2C address is 0x");
    // Serial.println(si4735Addr, HEX);
  }


  delay(500);

  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800,  10390, 10);

  delay(500);

  currentFrequency = si4735.getFrequency();
  si4735.setVolume(55);
  showStatus();
}

// Instructions
void showHelp() {
  // Serial.println("Type F to FM; A to MW; and 1 or 2 to SW");
  // Serial.println("Type U to increase and D to decrease the frequency");
  // Serial.println("==================================================");
  delay(1000);
}

// Show current frequency and status
void showStatus()
{
  String sFreq;  
  band[currentFreqIdx].currentFreq = currentFrequency = si4735.getFrequency();
  // Serial.print("You are tuned on ");
  if (si4735.isCurrentTuneFM() ) {
    sFreq = String(currentFrequency / 100.0, 2);
    // Serial.print(sFreq);
    // Serial.print("MHz ");
    // Serial.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
  } else {
    // Serial.print(currentFrequency);
    // Serial.print("KHz");
    sFreq = String(currentFrequency); 
  }

  // si4735.getCurrentReceivedSignalQuality();
  // Serial.print(" [SNR:" );
  // Serial.print(si4735.getCurrentSNR());
  // Serial.print("dB");

  // Serial.print(" RSSI:" );
  // Serial.print(si4735.getCurrentRSSI());
  // Serial.println("dBuV]");

  ble.print(sFreq + " MHz\n");

}



void showBandName() {
  // Serial.println("Band: ");
  // Serial.println(band[currentFreqIdx].freqName);
  // Serial.println("*******");  
}



// Send VFO/BFO database to mobile device
void sendDatabase()
{
  // Building  JSON string  =>  {"band":["MW  ", "SW2  "...]}
  String jsonStr = "#B{\"bands\":[";  // #J Means that a Json information will be processed by the SmartPhone
  short i;

  // Building Json string to send to the mobile device

  // Bands table
  for (i = 0; i < lastBand; i++)
  {
    jsonStr.concat("{\"name\":\"");
    jsonStr.concat("TESTE");
    jsonStr.concat("\", \"unt\":\"");
    jsonStr.concat("TESTE");
    jsonStr.concat("\"},");
  }
  jsonStr.concat("{\"name\":\"");
  jsonStr.concat("BAND");
  jsonStr.concat("\", \"unt\":\"");
  jsonStr.concat("MHZ");
  jsonStr.concat("\"}]}\n"); // '\n' means the and of the message
  // Send json Band table to the mobile device
  ble.print(jsonStr);
  // Serial.println(jsonStr);

  /**
  // Steps table
  jsonStr = "#S{\"steps\":[";;
  for (i = 0; i < lastStepVFO; i++) {
    jsonStr.concat("{\"name\":\"");
    jsonStr.concat(step[i].name);
    jsonStr.concat("\",\"value\":");
    jsonStr.concat(step[i].value);
    jsonStr.concat("},");
  } */
  jsonStr.concat("{\"name\":\"");
  jsonStr.concat("TESTE");
  jsonStr.concat("\",\"value\":");
  jsonStr.concat("TESTE");
  jsonStr.concat("}]}\n");
  // Send json Step table to the mobile device 
  // Serial.println(jsonStr);
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
        si4735.setAM(520, 1710, 810, 10);
        showStatus();    
      break;
    case 's':
      // changeBand(findByBandName("SW")); // find the next SW band in the table and return the array intex position
        si4735.setAM(7100, 7500, 7205, 5);
        showStatus();
      break;
    case 'f':
      si4735.setFM(8400, 10800,  10650, 10);
      showStatus();
      break;
    case 'v':
      // changeBand(findByBandName("VHF")); // find the next VHF band in the table and return the array intex position
      break;
    case 'a':
      // changeBand(findByBandName("AIR")); // find the next AIR band in the table and return the array intex position
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
