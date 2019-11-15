/*
   Test and validation of the SI4735 Arduino Library.

   By Ricardo Lima Caratti, Nov 2019.
*/

#include <SI4735.h>

#include <LiquidCrystal_I2C.h>

#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0



unsigned currentFrequency;
unsigned previousFrequency;
byte rssi = 0;

SI4735 si4735;
LiquidCrystal_I2C lcd(0x27, 20, 4);  // please check the address of you I2C device

void setup()
{

  Serial.begin(9600);

  
  lcd.init();

  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("SI4735");
  lcd.setCursor(2, 1);
  lcd.print("Arduino Library");
  lcd.setCursor(0, 3);
  lcd.print("By PU2CLR - Ricardo");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FM and AM (MW & SW)");
  lcd.setCursor(4, 2);
  lcd.print("Tuning Test");

  showHelp();

  delay(500);

  si4735.setup(RESET_PIN, FM_FUNCTION);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.9 MHz; step 100KHz)
  si4735.setFM(8400, 10800,  10390, 10);

  delay(500);

  currentFrequency = previousFrequency = si4735.getFrequency();
  si4735.setVolume(45);
  showStatus();
}


void showHelp() {

  delay(1000);
}

// Show current frequency
void showStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (si4735.isCurrentTuneFM() ) {
    lcd.print("FM ");
    lcd.setCursor(7, 0);
    lcd.print(String(currentFrequency / 100.0, 2));
    lcd.setCursor(17, 0);
    lcd.print("MHz ");
    lcd.setCursor(0,3);
    lcd.print((si4735.getCurrentPilot())?"STEREO":"MONO");
  } else {
    lcd.print("AM ");
    lcd.setCursor(8, 0);    
    lcd.print(currentFrequency);
    lcd.setCursor(17, 0);
    lcd.print("KHz");
  }
  // lcd.setCursor(10,3);
  // lcd.print("S:");
  // lcd.setCursor(13,3);
  // lcd.print(si4735.getCurrentRSSI());
  lcd.setCursor(16,3);
  lcd.print("dBuV");
  delay(300);

}


void showRSSI() {
  lcd.setCursor(10,3);
  lcd.print("S:");
  lcd.setCursor(13,3);
  lcd.print(si4735.getCurrentRSSI());
  delay(300);
}

// Main
void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case '+':
        si4735.volumeUp();
        break;
      case '-':
        si4735.volumeDown();
        break;
      case 'a':
      case 'A':
        si4735.setAM(570, 1710,  810, 10);
        break;
      case 'f':
      case 'F':
        si4735.setFM(8600, 10800,  10390, 10);
        break;
      case '1':
        si4735.setAM(9400, 9990,  9600, 5);
        break;
      case 'U':
      case 'u':
        si4735.frequencyUp();
        break;
      case 'D':
      case 'd':
        si4735.frequencyDown();
        break;
      case 'S':
        si4735.seekStationUp();
        break;
      case 's':
        si4735.seekStationDown();
        break;
      case '0':
        showStatus();
        break;
      case '?':
        showHelp();
        break;
      default:
        break;
    }
  }
  delay(50);
  currentFrequency = si4735.getFrequency();
  if ( currentFrequency != previousFrequency ) {
    previousFrequency = currentFrequency;
    showStatus();
  }
  
  if ( rssi != si4735.getCurrentRSSI() ) {
    rssi = si4735.getCurrentRSSI();
    showRSSI();   
  }
  
}
