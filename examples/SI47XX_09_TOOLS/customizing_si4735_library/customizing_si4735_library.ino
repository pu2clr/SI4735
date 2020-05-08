/**
 * Extending PU2CLR SI4735 Arduino Library
 * 
 * Maybe you need some Si47XX device functions that the PU2CLR SI4735 Arduino Library has not implemented so far. 
 * Also, you may want to change some existent function behaviors. 
 * The best way to customize the PU2CLR SI4735 Arduino Library for your needs is extending the current version of my library by using C++ OOP approaching.  
 * 
 * This sketch is an example that shows you how customize the PU2CLR SI4735 Arduino Library.
 * If you use this approach, all you have to do is downloading the current version of PU2CLR SI4735 Arduino Library. 
 * So, instead of using Si4735 class directly, you can use your own class based on SI4735.  
 * This way, you always have the current version of the original library customized for your needs. 
 * 
 * Copyright (c) 2019 Ricardo Lima Caratti - MIT License
 */

#include <SI4735.h>

#define RESET_PIN 12  // SELECT A PIN to control the SI47XX device RESET

class MyCustomSI4735 : public SI4735 {
  public:

    // New function /methods
    int methodA() {
      return 0;
    }

    int methodB() {
      return 1;
    }

    // Overwriting existent methods
    void setTuneFrequencyAntennaCapacitor(uint16_t capacitor) {
      // Your code here..
      // For example:

      currentFrequencyParams.arg.ANTCAPH = 0;           // Just an example
      currentFrequencyParams.arg.ANTCAPL = capacitor;   // Just an example
      return;
    }

    void reset() {
      // your reset code here
      pinMode(resetPin, OUTPUT);
      delay(1);
      digitalWrite(resetPin, LOW);
      delay(50);
      digitalWrite(resetPin, HIGH);
    }

};

MyCustomSI4735 radio;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  digitalWrite(RESET_PIN, HIGH);

  Serial.println("AM and FM station tuning test.");

  radio.setup(RESET_PIN, 0);
  radio.setFM(8400, 10800, 10390, 10);
  delay(500);
  radio.setVolume(45);

  // Setting the tune capacitor with your code instead Si4735 library code.
  radio.setTuneFrequencyAntennaCapacitor(100);

  // Calling new functions implemented by you.
  Serial.println(radio.methodA());
  Serial.println(radio.methodB());

}

void loop() {
  // Your code with your custom SI4734 library.
}
