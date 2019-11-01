
#include "SI4735.h"

#define INTERRUPT_PIN 2
#define RESET_PIN 12
// Pages 17 and 18 from Si48XX ATDD PROGRAMMING GUIDE
#define DEFAULT_BAND 4 // FM => 0 to 19; AM => 20 to 24; SW => 25 to 40

SI4735 si4735;

void setup()
{
    Serial.begin(9600);
    Serial.println("Teste!!!!");
    delay(500);
    si4735.setup(RESET_PIN, INTERRUPT_PIN, DEFAULT_BAND);

}

void loop() {
  
}
