/*
   Test and validation of the SI4735 Arduino Library on ATtiny85.
   It is a simple AM/FM radio implementation. 

  Prototype documentation : https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti, Jan 2020.
*/

#include <SI4735.h>
#include <LiquidCrystal_I2C.h>


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

LiquidCrystal_I2C lcd(0x27, 16, 2); 


void setup()
{

  pinMode(SEEK_BUTTON_UP, INPUT_PULLUP);
  pinMode(AM_FM_BUTTON, INPUT_PULLUP);


  lcd.init();

  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Si4735-Attiny85");
  lcd.setCursor(0, 1);
  lcd.print("   By PU2CLR   ");
 
  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10390, 10);
  delay(500);
  currentFrequency = si4735.getFrequency();
  si4735.setVolume(55);
  delay(100);
  lcd.clear();
  showStatus();
}

/* 
 *  Shows the currend frequency
 */
void showStatus() {
  // Clear just the frequency information space.
  lcd.setCursor(5,0); 
  lcd.print("        ");
  
  lcd.setCursor(0, 0);
  if (si4735.isCurrentTuneFM() ) {
    lcd.print("FM ");
    lcd.setCursor(5, 0);
    lcd.print(currentFrequency / 100.0);
    lcd.setCursor(13, 0);
    lcd.print("MHz");
  } else {
    lcd.print("AM ");
    lcd.setCursor(5, 0);
    lcd.print(currentFrequency);
    lcd.setCursor(13, 0);
    lcd.print("kHz");
  }
}

/*
 * Show the current Signal Quality
 */
void showSignalQuality() {
  si4735.getCurrentReceivedSignalQuality();
  lcd.setCursor(0,1);
  
  if ( si4735.isCurrentTuneFM() ) 
      lcd.print((si4735.getCurrentPilot()) ? "ST" : "MO");
  else 
      lcd.println("MW");
      
  lcd.setCursor(5,1);
  lcd.print("S:");
  lcd.setCursor(7,1);
  lcd.print("         ");
  lcd.setCursor(7,1);
  lcd.print(si4735.getCurrentRSSI());
  lcd.print("dBuV");
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
