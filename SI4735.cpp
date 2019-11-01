/*
 *
 *  Under construction.... Do not use it for while
 */ 

#include "SI4735.h"


/*
* 
* This function is called whenever the Si4735 changes. 
*/
void SI4735::waitInterrupr(void)
{
    while (!data_from_si4735) ;
}

/*
 * reset
 */
void SI4735::reset()
{
    waitToSend();
    data_from_si4735 = false;
    digitalWrite(resetPin, LOW);
    delayMicroseconds(200);
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(200);
    waitInterrupr();
    delayMicroseconds(2500);
}

void SI4735::waitToSend() {

    // TO DO
    while(false) {};
   
};

/*
 * Powerup in Analog Mode
 */
void SI4735::analogPowerUp() {
    waitToSend();
    data_from_si4735 = false;

}


/*
 * Set the radio to a new band. 
 */
void SI4735::setBand(byte new_band)
{
  // TO DO
}



void SI4735::getFirmware( void) {
    //TO DO
}


void SI4735::setup(unsigned int resetPin, unsigned int interruptPin, byte defaultBand)
{
    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    // Arduino interrupt setup.
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);

    data_from_si4735 = false;

    reset();

    // FM is the default BAND
    // See pages 17 and 18 (Table 8. Pre-defined Band Table) for more details
    setBand(defaultBand);

    // You need call it just once.
    getFirmware();

}
