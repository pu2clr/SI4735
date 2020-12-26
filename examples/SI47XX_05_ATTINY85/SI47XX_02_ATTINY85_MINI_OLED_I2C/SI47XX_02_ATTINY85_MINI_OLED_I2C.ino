/*
  Test and validation of the SI4735 Arduino Library on ATtiny85.
  It is a simple AM/FM radio implementation.

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Jan 2020.
*/

#include <SI4735.h>
#include <Tiny4kOLED.h>


#define RESET_PIN 3

#define AM_FM_BUTTON 1      // AM/FM SWITCH
#define SEEK_BUTTON_UP 4    // Seek Up
#define FM_FUNCTION 0
#define MAX_TIME 200

uint16_t currentFrequency;
uint16_t lastAmFrequency = 810;     // Starts AM on 810kHz;
uint16_t lastFmFrequency = 10390;   // Starts FM on 103,9MHz

long lastQuery = millis();

SI4735 si4735;



void setup()
{

  pinMode(SEEK_BUTTON_UP, INPUT_PULLUP);
  pinMode(AM_FM_BUTTON, INPUT_PULLUP);

  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);
  oled.setCursor(0, 0);
  oled.print("Si4735-Attiny85");
  oled.setCursor(0, 2);
  oled.print("   By PU2CLR   ");
  delay(2000);
  oled.clear();

  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10570, 10);
  delay(500);
  currentFrequency = si4735.getFrequency();
  si4735.setVolume(55);
  delay(100);
  showStatus();
}

/*
    Shows the currend frequency
*/
void showStatus() {
  // Clear just the frequency information space.
  oled.setCursor(0, 0);
  if (si4735.isCurrentTuneFM() ) {
    oled.print("FM ");
    oled.setCursor(38, 0);
    oled.print("      ");
    oled.setCursor(38, 0);
    oled.print(currentFrequency / 100.0);
    oled.setCursor(95, 0);
    oled.print("MHz");
  } else {
    oled.print("AM ");
    oled.setCursor(38, 0);
    oled.setCursor(38, 0);
    oled.print("      ");  
    oled.setCursor(38, 0);     
    oled.print(currentFrequency);
    oled.setCursor(95, 0);
    oled.print("kHz");
  }
}

/*
   Show the current Signal Quality
*/
void showSignalQuality() {
  si4735.getCurrentReceivedSignalQuality();
  oled.setCursor(0, 1);

  if ( si4735.isCurrentTuneFM() )
    oled.print((si4735.getCurrentPilot()) ? "ST" : "MO");
  else
    oled.println("MW");

  oled.setCursor(5, 1);
  oled.print("S:");
  oled.setCursor(7, 1);
  oled.print("         ");
  oled.setCursor(7, 1);
  oled.print(si4735.getCurrentRSSI());
  oled.print("dBuV");
}


void loop()
{
  if (digitalRead(AM_FM_BUTTON) == LOW ) {
    if  (si4735.isCurrentTuneFM() ) {
      lastFmFrequency = currentFrequency;
      si4735.setAM(520, 1710,  lastAmFrequency, 10);
    }
    else {
      lastAmFrequency = currentFrequency;
      si4735.setFM(8600, 10800,  lastFmFrequency, 10);
    }
    delay(250);
    currentFrequency = si4735.getFrequency();
    showStatus();
  } else if (digitalRead(SEEK_BUTTON_UP) == LOW ) {
    si4735.seekStationUp();
    currentFrequency = si4735.getFrequency();
    showStatus();
  }

  if ((millis() - lastQuery) > MAX_TIME) {
    lastQuery = millis();
  }
  delay(50);
}
