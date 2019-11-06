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
    while (!data_from_si4735)
        ;
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
void SI4735::waitToSend()
{
    do
    {
        delayMicroseconds(2000);
        Wire.requestFrom(SI473X_ADDR, 0x01);
    } while (!(Wire.read() & B10000000));
};

/*
 * Powerup in Analog Mode
 */
void SI4735::analogPowerUp(void)
{
    reset();
    // Initiate the I2C bus
    // Wire.begin();
    delayMicroseconds(1000);
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(powerUp.raw[0]); // Content of ARG1
    Wire.write(powerUp.raw[1]); // COntent of ARG2
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
void SI4735::getFirmware(void)
{
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

    setVolume(20); // Default volume level.
}

/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
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
    powerUp.arg.CTSIEN = CTSIEN;   // 1 -> Interrupt anabled;
    powerUp.arg.GPO2OEN = GPO2OEN; // 1 -> GPO2 Output Enable;
    powerUp.arg.PATCH = PATCH;     // 0 -> Boot normally;
    powerUp.arg.XOSCEN = XOSCEN;   // 1 -> Use external crystal oscillator;
    powerUp.arg.FUNC = FUNC;       // 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver.
    powerUp.arg.OPMODE = OPMODE;   // 0x5 = 00000101 = Analog audio outputs (LOUT/ROUT).

    // Set the current tuning frequancy mode 0X20 = FM and 0x40 = AM (LW/MW/SW)
    // See See Si47XX PROGRAMMING GUIDE; AN332; pages 55 and 124

    currentTune = (FUNC == 0) ? FM_TUNE_FREQ : AM_TUNE_FREQ;
}

/*
 * Set the frequency to the corrent function of the Si4735 (AM or FM)
 * You have to call setup or setPowerUp before call setFrequency.
 * 
 * @param unsigned freq Is the frequency to change. For example, FM => 10390 = 103.9 MHz; AM => 810 = 810 KHz. 
 */
void SI4735::setFrequency(unsigned freq)
{
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
 * Gets the current frequency of the Si4735 (AM or FM)
 * The method status do it an more. See getStatus below. 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 */
unsigned SI4735::getFrequency()
{
    si47x_frequency freq;
    getStatus(0, 1);
    freq.raw.FREQL = currentStatus.resp.READFREQL;
    freq.raw.FREQH = currentStatus.resp.READFREQH;
    return freq.value; 
}

/*
 * Tune complete has been triggered (STCINT)
 */
inline bool SI4735::getTuneCompleteTriggered()
{
    return currentStatus.resp.STCINT; // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
}

/* 
 * Gets Received Signal Quality Interrupt(RSQINT)
 * 
 */
inline bool SI4735::getSignalQualityInterrup()
{
    return currentStatus.resp.RSQINT; 
}

/* 
 * Gets Radio Data System (RDS) Interrupt
 */
inline bool SI4735::getRadioDataSystemInterrupt()
{
    return currentStatus.resp.RDSINT;
}

/*
 * Return the Error flag (true or false) of status of the least Tune or Seek 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 * @return true or false
 */
inline bool SI4735::getStatusError() {
    return currentStatus.resp.ERR;
}

/*
 * Gets the Error flag of status response 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 */
inline bool SI4735::getStatusCTS()
{
    return currentStatus.resp.CTS;
}

/* 
 * Returns true if the AFC rails (AFC Rail Indicator). 
 */
inline bool SI4735::getACFIndicator()
{
    return currentStatus.resp.AFCRL;
}

/* 
 * Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or 
 * wrapped to the original frequency (WRAP = 1).
 */
inline bool SI4735::getBandLimit()
{
    return currentStatus.resp.BLTF;
}

/*
 * Received Signal Strength Indicator.
 * This byte contains the receive signal strength when tune is complete (dBμV).
 */
inline byte SI4735::getReceivedSignalStrengthIndicator()
{
    return currentStatus.resp.RSSI;
}

/*
 * SNR.
 * This byte contains the SNR metric when tune is complete (dB).
 */
inline byte SI4735::getStatusSNR()
{
    return currentStatus.resp.SNR;
}

/* 
 * Multipath.
 * This byte contains the multipath metric when tune is complete. 
 * Multipath indi- cator is available only for Si474x, Si4706-C30 and later and 
 * Si4704/05/30/31/34/35/84/85-D50 and later.
 */
inline byte SI4735::getStatusMULT()
{
    return currentStatus.resp.MULT;
}

/* 
 * Read Antenna Tuning Capacitor (Si4704/05/06/2x only). 
 * Returns a byte that contains the current antenna tuning capacitor value.
 */
inline byte SI4735::getAntennaTuningCapacitor()
{
    return currentStatus.resp.READANTCAP;
}



/*
 * Returns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108) 
 * and would have been found during a Seek.
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
 */
inline bool SI4735::getStatusValid()
{
    return currentStatus.resp.VALID;
}

/*
 * Gets the current status  of the Si4735 (AM or FM)
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 *
 */
    void SI4735::getStatus()
{
    getStatus(0,1);
}

/*
 * Gets the current status  of the Si4735 (AM or FM)
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 * 
 * @param byte INTACK Seek/Tune Interrupt Clear. If set, clears the seek/tune complete interrupt status indicator;
 * @param byte CANCEL Cancel seek. If set, aborts a seek currently in progress;
 * 
 */
void SI4735::getStatus(byte INTACK, byte CANCEL) {
    si47x_tune_status status;
    si47x_frequency freq;
    byte cmd = (currentTune == FM_TUNE_FREQ) ? FM_TUNE_STATUS : AM_TUNE_STATUS;

    waitToSend();

    status.arg.INTACK = INTACK;
    status.arg.CANCEL = CANCEL;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(status.raw);
    Wire.endTransmission();

    waitToSend();

    Wire.requestFrom(SI473X_ADDR, 0x7);

    // Gets response information
    for (byte i = 0; i < 0x7; i++)
    {
        currentStatus.raw[i] = Wire.read();
    }

    #if defined(DEBUG)
    debugStatus();
    #endif
    
    delayMicroseconds(550);
}

    /*
 * Look for a station 
 * See Si47XX PROGRAMMING GUIDE; AN332; page 55, 72, 125 and 137
 * 
 * @param SEEKUP Seek Up/Down. Determines the direction of the search, either UP = 1, or DOWN = 0. 
 * @param Wrap/Halt. Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
 */
    void SI4735::seekStation(byte SEEKUP, byte WRAP)
{
    si47x_seek seek;

    // Check which FUNCTION (AM or FM) is working now
    byte seek_start = (currentTune == FM_TUNE_FREQ) ? FM_SEEK_START : AM_SEEK_START;

    waitToSend();

    seek.arg.SEEKUP = SEEKUP;
    seek.arg.WRAP = WRAP;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(seek_start);
    Wire.write(seek.raw);

    if (seek_start == AM_SEEK_START)
    {
        Wire.write(0x00); // Always 0
        Wire.write(0x00); // Always 0
        Wire.write(0x00); // Tuning Capacitor: The tuning capacitor value
        Wire.write(0x00); //                   will be selected automatically.
    }

    Wire.endTransmission();
    // delayMicroseconds(1000000);
    delay(100);
}

/* 
 * Set volume level
 * @param byte volume (domain: 0 - 63) 
 */
void SI4735::setVolume(byte volume)
{
    waitToSend();
    this->volume = volume;
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);   // Always 0x00
    Wire.write(0x40);   // RX_VOLUME 0x4000 -> 0x40 
    Wire.write(0x00);   // RX_VOLUME 0x4000 -> 0x00
    Wire.write(0x00);   // ARG1
    Wire.write(volume); // ARG2 (level: 0 to 63)
    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 *  Set sound volume level Up   
 *  
 */
void SI4735::volumeUp()
{
    if (volume < 63)
        volume++;
    setVolume(volume);
}

/*
 *  Set sound volume level Down   
 *  
 */
void SI4735::volumeDown()
{
    if (volume > 5)
        volume--;
    setVolume(volume);
}

/*
 * Set the radio to AM function. It means: LW MW and SW.
 */
void SI4735::setAM()
{
    setPowerUp(1, 1, 0, 1, 1, SI473X_ANALOG_AUDIO);
    analogPowerUp();
    setVolume(volume); // Set to previus configured volume
}

/*
 * Set the radio to FM function
 */
void SI4735::setFM()
{
    setPowerUp(1, 1, 0, 1, 0, SI473X_ANALOG_AUDIO);
    analogPowerUp();
    setVolume(volume); // Set to previus configured volume
    // teste
}

#if defined(DEBUG)
void SI4735::debugStatus()
{
    Serial.print("Tune complete (STCINT).: ");
    Serial.println(currentStatus.resp.STCINT); // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
    Serial.print("(RDS) Interrupt(RDSINT): ");
    Serial.println(currentStatus.resp.RDSINT); // Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
    Serial.print("Receiv.Sig.Quality INT: ");
    Serial.println(currentStatus.resp.RSQINT); // Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
    Serial.print("ERR...................: ");
    Serial.println(currentStatus.resp.ERR); // Error. 0 = No error 1 = Error
    Serial.print("CTS...................: ");
    Serial.println(currentStatus.resp.CTS); // Clear to Send.
    // RESP1
    Serial.print("Valid Channel..........: ");
    Serial.println(currentStatus.resp.VALID); // Valid Channel
    Serial.print("AFC Rail Indicator.....: ");
    Serial.println(currentStatus.resp.AFCRL); // AFC Rail Indicator
    Serial.print("Reports if a seek limit: ");
    Serial.println(currentStatus.resp.BLTF); // Reports if a seek hit the band limit
    // RESP4
    Serial.print("Receiv.Signal Ind.dBμV.: ");
    Serial.println(currentStatus.resp.RSSI); // Received Signal Strength Indicator (dBμV)
    // RESP5
    Serial.print("SNR (dB)...............: ");
    Serial.println(currentStatus.resp.SNR); // This Serial.println(currentStatus.resp.contains the SNR metric when tune is complete (dB).
    // RESP6
    Serial.print("multipath metric.......: ");
    Serial.println(currentStatus.resp.MULT); // Contains the multipath metric when tune is complete
    // RESP7
    Serial.print("Antenna Tuning cap......: ");
    Serial.println(currentStatus.resp.READANTCAP); // Contains the current antenna tuning capacitor value

    si47x_frequency freq;
    freq.raw.FREQL = currentStatus.resp.READFREQL;
    freq.raw.FREQH = currentStatus.resp.READFREQH;

    Serial.print("Palatable frequency.....: ");
    Serial.println(freq.value);
}
#endif

