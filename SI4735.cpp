/*
 * This is a library for the SI4735, BROADCAST AM/FM/SW RADIO RECEIVER, IC from Silicon Labs for the 
 * Arduino development environment.  
 * This library is intended to provide an easier interface for controlling the SI4435.
 * 
 * See documentation on https://github.com/pu2clr/SI4735
 * 
 * By Ricardo Lima Caratti, Nov 2019.
 */

#include <SI4735.h>

/*
* 
* This function is called whenever the Si4735 changes. 
*/
void SI4735::waitInterrupr(void)
{
    while (!data_from_si4735) ;
}


/*
 * Reset the SI473X   
 * See Si47XX PROGRAMMING GUIDE; AN332;
 */
void SI4735::reset()
{
    pinMode(resetPin, OUTPUT);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(250);
    digitalWrite(resetPin, HIGH);
    delay(500);
}

/*
 * Wait for the si473x is ready (Clear to Send status bit have to be 1).  
 */
void SI4735::waitToSend() {
    do{
        delayMicroseconds(2000);
        Wire.requestFrom(SI473X_ADDR, 0x01);
    } while (!(Wire.read() & B10000000));
};

/*
 * Powerup in Analog Mode
 */
void SI4735::analogPowerUp(void) {

    reset();

    // Initiate the I2C bus
    // Wire.begin();
    delayMicroseconds(1000);

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(powerUp.raw[0]);     // Content of ARG1
    Wire.write(powerUp.raw[1]);     // COntent of ARG2
    Wire.endTransmission();
    // page 12 - Delay at least 500 ms between powerup command and first tune command.
    delayMicroseconds(550);

}


/*
 * Set the radio to a new band. 
 */
void SI4735::setBand(byte new_band)
{
  // TO DO
}


/*
 * Gets firmware information 
 */
void SI4735::getFirmware( void) {
    //TO DO
}


/* 
 * Starts the Si473X device. 
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte interruptPin interrupt Arduino Pin (see your Arduino pinout)
 * @param byte defaultFunction
 */ 
void SI4735::setup(byte resetPin, byte interruptPin, byte defaultFunction)
{
    Wire.begin();

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    // Arduino interrupt setup (you have to know which Arduino Pins can deal with interrupt).
    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);

    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);

    data_from_si4735 = false;

    // Set the initial SI473X behavior
    // CTSIEN   1 -> Interrupt anabled;
    // GPO2OEN  1 -> GPO2 Output Enable;
    // PATCH    0 -> Boot normally;
    // XOSCEN   1 -> Use external crystal oscillator;
    // FUNC     defaultFunction = 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
    // OPMODE   SI473X_ANALOG_AUDIO = 00000101 = Analog audio outputs (LOUT/ROUT).

    // Serial.print("defaultFunction: ");
    // Serial.print(defaultFunction);
        
    setPowerUp(1, 1, 0, 1, defaultFunction, SI473X_ANALOG_AUDIO);
    // Do Power Up
    analogPowerUp();
}

/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * 
 * @param byte CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param byte GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param byte PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param byte XOSCEN byte XOSCEN set external Crystal enabled or disabled 
 * @param byte FUNC sets the receiver function have to be used (0 = FM Receive; 1 = AM (LW/MW/SW) Receiver)
 * @param byte OPMODE set the kind of audio mode you want to use.
 */
void SI4735::setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE)
{
    powerUp.arg.CTSIEN = CTSIEN;              // 1 -> Interrupt anabled;
    powerUp.arg.GPO2OEN = GPO2OEN;            // 1 -> GPO2 Output Enable;
    powerUp.arg.PATCH = PATCH;                // 0 -> Boot normally;
    powerUp.arg.XOSCEN = XOSCEN;              // 1 -> Use external crystal oscillator;
    powerUp.arg.FUNC = FUNC;                  // 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
    powerUp.arg.OPMODE = OPMODE;              // 0x5 = 00000101 = Analog audio outputs (LOUT/ROUT).

    // Set the current tuning frequancy mode 0X20 = FM and 0x40 = AM (LW/MW/SW)
    // See See Si47XX PROGRAMMING GUIDE; AN332; pages 55 and 124

 
    currentTune = (FUNC == 0)? FM_TUNE_FREQ : AM_TUNE_FREQ;
}

void SI4735::setFrequency(unsigned freq) {

    waitToSend(); // Wait for the si473x is ready. 

    currentFrequency.value = freq;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(currentTune);
    Wire.write(0x00);
    Wire.write(currentFrequency.raw.FREQH);
    Wire.write(currentFrequency.raw.FREQL);
    Wire.write(0x00);

    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * Look for a station 
 * See Si47XX PROGRAMMING GUIDE; AN332; page 72
 * 
 * @param SEEKUP Seek Up/Down. Determines the direction of the search, either UP = 1, or DOWN = 0. 
 * @param Wrap/Halt. Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
 */
void SI4735::seekStation(byte SEEKUP, byte WRAP)
{
    si47x_seek seek;

    byte seek_start = (currentTune == FM_TUNE_FREQ)? FM_SEEK_START : AM_SEEK_START;

    seek.arg.SEEKUP = SEEKUP;
    seek.arg.WRAP = WRAP;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(seek_start); 
    Wire.write(seek.raw);
    Wire.endTransmission();

    delayMicroseconds(550);
}

void SI4735::setAM() {
    setPowerUp(1, 1, 0, 1, 1, SI473X_ANALOG_AUDIO);
    analogPowerUp();
}

void SI4735::setFM() {
    setPowerUp(1, 1, 0, 1, 0, SI473X_ANALOG_AUDIO);
    analogPowerUp();
}
