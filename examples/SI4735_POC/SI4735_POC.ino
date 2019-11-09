/*
 * Test and validation of the SI4735 Arduino Library.
 * 
 * By Ricardo Lima Caratti, Nov 2019.
 */

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define AM_FUNCTION 1
#define FM_FUNCTION 0

byte band = FM_FUNCTION;

const unsigned min_am = 570;
const unsigned max_am = 1710;
unsigned am_freq = min_am; // Change it to your local AM station 

const unsigned min_fm = 8400;
const unsigned max_fm = 10900;
unsigned fm_freq = 10390; // Change it to your local FM station 

unsigned currentFrequency;

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  Serial.println("Test and validation of the SI4735 Arduino Library.");
  Serial.println("AM and FM station tuning test.");

  delay(500);
  si4735.setup(RESET_PIN, INTERRUPT_PIN, FM_FUNCTION);
  delay(500);

  Serial.println("==================================================");
  Serial.println("Type F to FM; A to AM");
  Serial.println("Type I to increase and D to decrease the frequency");
  Serial.println("Type S to seek station");
  Serial.println("Type + or - to volume Up or Down");  
  Serial.println("Type ? to debug. See SI4735.h to anable or disable it");  
  Serial.println("==================================================");

 
  band = FM_FUNCTION;
  si4735.setFrequency(fm_freq);
  showStatus(fm_freq, "MHz");
  si4735.setVolume(45);
}

void showInfo() {
  Serial.print("(RDS): ");
  Serial.println((int) si4735.getRadioDataSystemInterrupt());
} 


// Show current frequency
void showStatus(unsigned freq, String unit)
{
  byte d; 
  float f; 
  if (band == FM_FUNCTION) {
    f = freq/100.0;
    d = 1;
  } else {
    f = freq/1.0;
    d = 0;    
  }
   
  Serial.print("Current Frequency: ");
  Serial.print(String(f,d));
  Serial.println(unit);
}

// Increase the frequency
void nextFreq()
{
  if (band == FM_FUNCTION)
  {
    if (fm_freq < max_fm)
    {
      fm_freq += 10;
      si4735.setFrequency(fm_freq);
      showStatus(fm_freq, "MHz");
    }
    else
    {
      Serial.print("You have reached the maximum FM frequency for this band.");
    }
  }
  else
  {
    if (am_freq < max_am)
    {
      am_freq += 10;
      si4735.setFrequency(am_freq);
      showStatus(am_freq, "KHz");
    }
    else
    {
      Serial.print("You have reached the maximum AM frequency for this band.");
    }
  }
}

// Decrease the frequency
void prevFreq()
{
  if (band == FM_FUNCTION)
  {
    if (fm_freq > min_fm)
    {
      fm_freq -= 10;
      si4735.setFrequency(fm_freq);
      showStatus(fm_freq, "MHz");
      currentFrequency = fm_freq;
    }
    else
    {
      Serial.print("You have reached the minimum FM frequency for this band.");
    }
  }
  else
  {
    if (am_freq > min_am)
    {
      am_freq -= 10;
      si4735.setFrequency(am_freq);
      showStatus(am_freq, "kHz");
      currentFrequency = am_freq;
    }
    else
    {
      Serial.print("You have reached the maximum AM frequency for this band.");
    }
  }
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
      band = AM_FUNCTION;
      si4735.setAM();
      si4735.setFrequency(am_freq);
      showStatus(am_freq, "KHz");
      break;
    case 'f':
    case 'F':
      band = FM_FUNCTION;
      si4735.setFM();
      si4735.setFrequency(fm_freq);
      showStatus(fm_freq, "MHz");
      break;
    case 'I':
    case 'i':
      nextFreq();
      break;
    case 'D':
    case 'd':
      prevFreq();
      break;
    case 'S':
    case 's':
      // Look for the next station (AM or FM, depending on current function)
      si4735.seekStation(1, 1);
      Serial.println("Station found");
      currentFrequency = si4735.getFrequency();
      if (band == FM_FUNCTION) {
          fm_freq = currentFrequency;
          showStatus(fm_freq, "MHz");
      } else {
         am_freq = currentFrequency;
         showStatus(am_freq, "KHz");
      }
      break;
    case '?': 
        // Show Status information
        si4735.getStatus(); 
        showInfo();
        break;  
    default:
      break;
    }
  }
}
