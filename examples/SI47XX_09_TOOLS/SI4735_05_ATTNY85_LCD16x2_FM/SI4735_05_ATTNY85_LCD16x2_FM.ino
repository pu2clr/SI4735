/*
   Test and validation of the SI4735 Arduino Library on ATtiny85.
   It is a simple FM radio implementation.
   
   By Ricardo Lima Caratti, Jan 2020.
*/

#include <SI4735.h>
#include <LiquidCrystal_I2C.h>

#define RESET_PIN 3
#define SEEK_BUTTON_UP 4    // Seek Up
#define SEEK_BUTTON_DOWN 1  // Seek Down
#define FM_FUNCTION 0

uint16_t currentFrequency;

SI4735 si4735;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  pinMode(SEEK_BUTTON_UP, INPUT_PULLUP);
  pinMode(SEEK_BUTTON_DOWN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Si4735-Attiny85");
  lcd.setCursor(0, 1);
  lcd.print("   By PU2CLR   ");

  si4735.setup(RESET_PIN, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800, 10390, 10);
  delay(100);
  currentFrequency = si4735.getFrequency();
  si4735.setVolume(55);
  delay(100);
  lcd.clear();
  showStatus();
}

void showStatus() {
  // Clear just the frequency information space.
  lcd.setCursor(5, 0);
  lcd.print("        ");
  lcd.setCursor(0, 0);
  lcd.print("FM ");
  lcd.setCursor(5, 0);
  lcd.print(currentFrequency / 100.0);
  lcd.setCursor(13, 0);
  lcd.print("MHz");
}

void loop()
{
  if (digitalRead(SEEK_BUTTON_UP) == LOW ) {
    si4735.seekStationUp();
    currentFrequency = si4735.getFrequency();
    showStatus();
  }
  else if (digitalRead(SEEK_BUTTON_DOWN) == LOW ) {
    si4735.seekStationDown();
    currentFrequency = si4735.getFrequency();
    showStatus();
  }
  delay(50);
}
