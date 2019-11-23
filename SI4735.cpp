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

SI4735::SI4735()
{
    for (int i = 0; i < 65; i++)
        rds_buffer[i] = ' ';
}

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
        delayMicroseconds(2500);
        Wire.requestFrom(SI473X_ADDR, 1);
    } while (!(Wire.read() & B10000000));
};

/*
 * Powerup in Analog Mode
 * You have to call setPowerUp method before. 
 */
void SI4735::analogPowerUp(void)
{
    reset();
    // Initiate the I2C bus
    // Wire.begin();
    delayMicroseconds(1000);
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(powerUp.raw[0]); // Content of ARG1
    Wire.write(powerUp.raw[1]); // COntent of ARG2
    Wire.endTransmission();
    // page 12 - Delay at least 500 ms between powerup command and first tune command.
    delayMicroseconds(2500);
}

/*
 * Gets firmware information 
 */
void SI4735::getFirmware(void)
{
    waitToSend();

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(GET_REV);
    Wire.endTransmission();

    // Request for 9 bytes response
    Wire.requestFrom(SI473X_ADDR, 9);

    for (int i = 0; i < 9; i++)
        firmwareInfo.raw[i] = Wire.read();

    delayMicroseconds(550);
}

/* 
 * Starts the Si473X device. 
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte interruptPin interrupt Arduino Pin (see your Arduino pinout). If less than 0, iterrupt disabled
 * @param byte defaultFunction
 */
void SI4735::setup(byte resetPin, int interruptPin, byte defaultFunction)
{
    byte interruptEnable = 0;
    Wire.begin();

    this->resetPin = resetPin;
    this->interruptPin = interruptPin;

    // Arduino interrupt setup (you have to know which Arduino Pins can deal with interrupt).
    if (interruptPin >= 0)
    {
        pinMode(interruptPin, INPUT);
        attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_hundler, RISING);
        interruptEnable = 1;
    }

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
    setPowerUp(interruptEnable, 1, 0, 1, defaultFunction, SI473X_ANALOG_AUDIO);

    analogPowerUp();
    setVolume(20); // Default volume level.
    getFirmware();
}

/* 
 * Starts the Si473X device.  
 * Use this setup if you are not using interrupt resource
 * 
 * @param byte resetPin Digital Arduino Pin used to RESET command 
 * @param byte defaultFunction
 */
void SI4735::setup(byte resetPin, byte defaultFunction)
{
    setup(resetPin, -1, defaultFunction);
}

/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * @param byte CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param byte GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param byte PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param byte XOSCEN byte XOSCEN set external Crystal enabled or disabled 
 * @param byte FUNC sets the receiver function have to be used [0 = FM Receive; 1 = AM (LW/MW/SW) and SSB (if SSB patch apllied)]
 * @param byte OPMODE set the kind of audio mode you want to use.
 */
void SI4735::setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE)
{
    powerUp.arg.CTSIEN = CTSIEN;   // 1 -> Interrupt anabled;
    powerUp.arg.GPO2OEN = GPO2OEN; // 1 -> GPO2 Output Enable;
    powerUp.arg.PATCH = PATCH;     // 0 -> Boot normally;
    powerUp.arg.XOSCEN = XOSCEN;   // 1 -> Use external crystal oscillator;
    powerUp.arg.FUNC = FUNC;       // 0 = FM Receive; 1 = AM/SSB (LW/MW/SW) Receiver.
    powerUp.arg.OPMODE = OPMODE;   // 0x5 = 00000101 = Analog audio outputs (LOUT/ROUT).

    // Set the current tuning frequancy mode 0X20 = FM and 0x40 = AM (LW/MW/SW)
    // See See Si47XX PROGRAMMING GUIDE; AN332; pages 55 and 124

    if (FUNC == 0)
    {
        currentTune = FM_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 1;
    }
    else
    {
        currentTune = AM_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 0;
    }
    currentFrequencyParams.arg.FAST = 0;
    currentFrequencyParams.arg.DUMMY1 = 0;
    currentFrequencyParams.arg.ANTCAPH = 0;
    currentFrequencyParams.arg.ANTCAPL = 0;
}

/*
 * Selects the tuning capacitor value.
 * 
 * For FM, Antenna Tuning Capacitor is valid only when using TXO/LPI pin as the antenna input.
 * 
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 71 and 136
 * 
 * 
 * @param capacitor If zero, the tuning capacitor value is selected automatically. 
 *                  If the value is set to anything other than 0:
 *                  AM - the tuning capacitance is manually set as 95 fF x ANTCAP + 7 pF. ANTCAP manual range is 1–6143;
 *                  FM - the valid range is 0 to 191.    
 *                  According to Silicon Labs, automatic capacitor tuning is recommended (value 0). 
 */
void SI4735::setTuneFrequencyAntennaCapacitor(unsigned capacitor)
{

    si47x_antenna_capacitor cap;

    cap.value = capacitor;

    currentFrequencyParams.arg.DUMMY1 = 0;

    if (currentTune == FM_TUNE_FREQ)
    {
        // For FM, the capacitor value has just one byte
        currentFrequencyParams.arg.ANTCAPH = (capacitor <= 191) ? cap.raw.ANTCAPL : 0;
    }
    else
    {
        if (capacitor <= 6143)
        {
            currentFrequencyParams.arg.FREEZE = 0; // This parameter is not used for AM
            currentFrequencyParams.arg.ANTCAPH = cap.raw.ANTCAPH;
            currentFrequencyParams.arg.ANTCAPL = cap.raw.ANTCAPL;
        }
    }
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
    currentFrequencyParams.arg.FREQH = currentFrequency.raw.FREQH;
    currentFrequencyParams.arg.FREQL = currentFrequency.raw.FREQL;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(currentTune);
    Wire.write(currentFrequencyParams.raw[0]); // Send byte with FAST and  FREEZE information; if not FM must be 0;
    Wire.write(currentFrequencyParams.arg.FREQH);
    Wire.write(currentFrequencyParams.arg.FREQL);
    Wire.write(currentFrequencyParams.arg.ANTCAPH);
    // If current tune is not FM sent one more byte
    if (currentTune != FM_TUNE_FREQ)
        Wire.write(currentFrequencyParams.arg.ANTCAPL);
    Wire.endTransmission();
    delayMicroseconds(550);
    
    currentWorkFrequency = freq;
}

/* 
 * Set the current step value. 
 * @param step if you are using FM, 10 means 100KHz. If you are using AM 10 means 10KHz
 *             For AM, 1 (1KHz) to 10 (10KHz) are valid values.
 *             For FM 5 (50KHz) and 10 (100KHz) are valid values.  
 */
void SI4735::setFrequencyStep(byte step)
{
    currentStep = step;
}

/*
 *  Increments the current frequency on current band/function by using the current step.
 *  See setFrequencyStep
 */
void SI4735::frequencyUp()
{
    if (currentWorkFrequency >= currentMaximumFrequency)
        currentWorkFrequency = currentMinimumFrequency;
    else
        currentWorkFrequency += currentStep;

    setFrequency(currentWorkFrequency);
}

/*
 *  Decrements the current frequency on current band/function by using the current step.
 *  See setFrequencyStep
 */
void SI4735::frequencyDown()
{

    if (currentWorkFrequency <= currentMinimumFrequency)
        currentWorkFrequency = currentMaximumFrequency;
    else
        currentWorkFrequency -= currentStep;

    setFrequency(currentWorkFrequency);
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
 * Set the radio to AM function. It means: LW MW and SW.
 */
void SI4735::setSSB()
{
    // It starts with the same AM parameters.
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
}

/*
 * Set the radio to AM (LW/MW/SW) function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency 
 * @param step step used to go to the next channel   
 */
void SI4735::setAM(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setAM();

    currentWorkFrequency = initialFreq;

    setFrequency(currentWorkFrequency);

    delayMicroseconds(1000);
}

/*
 * Set the radio to SSB (LW/MW/SW) function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency 
 * @param step step used to go to the next channel   
 */
void SI4735::setSSB(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setSSB();

    currentWorkFrequency = initialFreq;

    setFrequency(currentWorkFrequency);

    delayMicroseconds(1000);
}

/*
 * Set the radio to FM function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency (default frequency)
 * @param step step used to go to the next channel   
 */
void SI4735::setFM(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setFM();

    currentWorkFrequency = initialFreq;

    setFrequency(currentWorkFrequency);

    delayMicroseconds(1000);
}

/*
 * Selects the bandwidth of the channel filter for AM reception. The choices are 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). 
 * The default bandwidth is 2 kHz.
 * Works only in AM / SSB (LW/MW/SW) 
 * @param AMCHFLT the choices are:   0 = 6 kHz Bandwidth                    
 *                                   1 = 4 kHz Bandwidth
 *                                   2 = 3 kHz Bandwidth
 *                                   3 = 2 kHz Bandwidth
 *                                   4 = 1 kHz Bandwidth
 *                                   5 = 1.8 kHz Bandwidth
 *                                   6 = 2.5 kHz Bandwidth, gradual roll off
 *                                   7–15 = Reserved (Do not use).
 * @param AMPLFLT Enables the AM Power Line Noise Rejection Filter.
 */
void SI4735::setBandwidth(byte AMCHFLT, byte AMPLFLT)
{
    si47x_bandwidth_config filter;
    si47x_property property;

    if (currentTune == FM_TUNE_FREQ) // Only for AM/SSB mode
        return;

    if (AMCHFLT > 6)
        return;

    property.value = AM_CHANNEL_FILTER;

    filter.param.AMCHFLT = AMCHFLT;
    filter.param.AMPLFLT = AMPLFLT;

    waitToSend();
    this->volume = volume;
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00
    Wire.write(property.raw.byteHigh); // High byte first
    Wire.write(property.raw.byteLow);  // Low byte after
    Wire.write(filter.raw[1]);         // Raw data for AMCHFLT and
    Wire.write(filter.raw[0]);         // AMPLFLT
    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * Returns true (1) if the current function is FM (FM_TUNE_FREQ).
 */
bool SI4735::isCurrentTuneFM()
{
    return (currentTune == FM_TUNE_FREQ);
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

    currentWorkFrequency = freq.value;

    getCurrentReceivedSignalQuality(0);

    return freq.value;
}

/*
 * Gets the current status  of the Si4735 (AM or FM)
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 *
 */
void SI4735::getStatus()
{
    getStatus(0, 1);
}

/*
 * Gets the current status  of the Si4735 (AM or FM)
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 * 
 * @param byte INTACK Seek/Tune Interrupt Clear. If set, clears the seek/tune complete interrupt status indicator;
 * @param byte CANCEL Cancel seek. If set, aborts a seek currently in progress;
 * 
 */
void SI4735::getStatus(byte INTACK, byte CANCEL)
{
    si47x_tune_status status;
    byte cmd = (currentTune == FM_TUNE_FREQ) ? FM_TUNE_STATUS : AM_TUNE_STATUS;

    waitToSend();

    status.arg.INTACK = INTACK;
    status.arg.CANCEL = CANCEL;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(status.raw);
    Wire.endTransmission();

    waitToSend();

    Wire.requestFrom(SI473X_ADDR, 7);

    // Gets response information
    for (byte i = 0; i < 7; i++)
    {
        currentStatus.raw[i] = Wire.read();
    }

    delayMicroseconds(2500);
}

/*
 * Queries AGC STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 80; for AM page 142.
 * See AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18. 
 * After call this method, you can call isAgcEnabled to know the AGC status and getAgcGainIndex to know the gain index value.
 */
void SI4735::getAutomaticGainControl()
{
    byte cmd;

    if (currentTune == FM_TUNE_FREQ)
    { // FM TUNE
        cmd = FM_AGC_STATUS;
    }
    else
    { // AM TUNE - SAME COMMAND used on SSB mode
        cmd = AM_AGC_STATUS;
    }

    waitToSend();

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.endTransmission();

    waitToSend();

    Wire.requestFrom(SI473X_ADDR, 3);
    currentAgcStatus.raw[0] = Wire.read(); // STATUS response
    currentAgcStatus.raw[1] = Wire.read(); // RESP 1
    currentAgcStatus.raw[2] = Wire.read(); // RESP 2

    delayMicroseconds(2500);
}

/* 
 * If FM, overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 
 * (minimum attenuation) and 26 (maximum attenuation);
 * If AM/SSB, Overrides the AM AGC setting by disabling the AGC and forcing the gain index that ranges between 0 
 * (minimum attenuation) and 37+ATTN_BACKUP (maximum attenuation);
 * 
 * @param byte AGCDIS This param selects whether the AGC is enabled or disabled (0 = AGC enabled; 1 = AGC disabled);
 * @param byte AGCDX AGC Index (0 = Minimum attenuation (max gain); 1 – 36 = Intermediate attenuation); 
 *             > 37 - Maximum attenuation (min gain) ).
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 81; for AM page 143 
 */
void SI4735::setAutomaticGainControl(byte AGCDIS, byte AGCDX)
{
    si47x_agc_overrride agc;

    byte cmd;

    cmd = (currentTune == FM_TUNE_FREQ) ? FM_AGC_OVERRIDE : AM_AGC_OVERRIDE;

    agc.arg.AGCDIS = AGCDIS;
    agc.arg.AGCDX = AGCDX;

    waitToSend();

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(agc.raw[0]);
    Wire.write(agc.raw[1]);
    Wire.endTransmission();

    delayMicroseconds(2500);
}

/*
 * Queries the status of the Received Signal Quality (RSQ) of the current channel
 * Command FM_RSQ_STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 75 and 141
 * 
 * @param INTACK Interrupt Acknowledge. 
 *        0 = Interrupt status preserved; 
 *        1 = Clears RSQINT, BLENDINT, SNRHINT, SNRLINT, RSSIHINT, RSSILINT, MULTHINT, MULTLINT.
 */
void SI4735::getCurrentReceivedSignalQuality(byte INTACK)
{

    byte arg;
    byte cmd;
    int bytesResponse;

    if (currentTune == FM_TUNE_FREQ)
    { // FM TUNE
        cmd = FM_RSQ_STATUS;
        bytesResponse = 7;
    }
    else
    { // AM TUNE
        cmd = AM_RSQ_STATUS;
        bytesResponse = 5;
    }

    waitToSend();

    arg = INTACK;
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(arg); // send B00000001
    Wire.endTransmission();

    waitToSend();

    Wire.requestFrom(SI473X_ADDR, bytesResponse);
    // Gets response information
    for (byte i = 0; i < bytesResponse; i++)
    {
        currentRqsStatus.raw[i] = Wire.read();
    }
    delayMicroseconds(2500);
}

/*
 * Look for a station 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 55, 72, 125 and 137
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
 * Search for the next station 
 */
void SI4735::seekStationUp()
{
    seekStation(1, 1);
}

/*
 * Search the previous station
 */
void SI4735::seekStationDown()
{
    seekStation(0, 1);
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
 * RDS implementation 
 */

/* 
 * Configures interrupt related to RDS
 * Use this method if want to use interrupt
 * See Si47XX PROGRAMMING GUIDE; AN332; page 103
 * 
 * @param RDSRECV If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
 * @param RDSSYNCLOST If set, generate RDSINT when RDS loses synchronization.
 * @param RDSSYNCFOUND set, generate RDSINT when RDS gains synchronization.
 * @param RDSNEWBLOCKA If set, generate an interrupt when Block A data is found or subsequently changed
 * @param RDSNEWBLOCKB If set, generate an interrupt when Block B data is found or subsequently changed
 */
void SI4735::setRdsIntSource(byte RDSNEWBLOCKB, byte RDSNEWBLOCKA, byte RDSSYNCFOUND, byte RDSSYNCLOST, byte RDSRECV)
{
    si47x_property property;
    si47x_rds_int_source rds_int_source;

    if (currentTune != FM_TUNE_FREQ)
        return;

    rds_int_source.refined.RDSNEWBLOCKB = RDSNEWBLOCKB;
    rds_int_source.refined.RDSNEWBLOCKA = RDSNEWBLOCKA;
    rds_int_source.refined.RDSSYNCFOUND = RDSSYNCFOUND;
    rds_int_source.refined.RDSSYNCLOST = RDSSYNCLOST;
    rds_int_source.refined.RDSRECV = RDSRECV;
    rds_int_source.refined.DUMMY1 = 0;
    rds_int_source.refined.DUMMY2 = 0;

    property.value = FM_RDS_INT_SOURCE;

    waitToSend();

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00 (I need to check it)
    Wire.write(property.raw.byteHigh); // Send property - High Byte - most significant first
    Wire.write(property.raw.byteLow);  // Low Byte
    Wire.write(rds_int_source.raw[1]); // Send the argments. Most significant first
    Wire.write(rds_int_source.raw[0]);
    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * RDS COMMAND FM_RDS_STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78
 * @param INTACK Interrupt Acknowledge; 0 = RDSINT status preserved. 1 = Clears RDSINT.
 * @param MTFIFO 0 = If FIFO not empty, read and remove oldest FIFO entry; 1 = Clear RDS Receive FIFO.
 * @param STATUSONLY Determines if data should be removed from the RDS FIFO.
 */
void SI4735::getRdsStatus(byte INTACK, byte MTFIFO, byte STATUSONLY)
{
    si47x_rds_command rds_cmd;
    // checking current FUNC (Am or FM)
    if (currentTune != FM_TUNE_FREQ)
        return;

    waitToSend();

    rds_cmd.arg.INTACK = INTACK;
    rds_cmd.arg.MTFIFO = MTFIFO;
    rds_cmd.arg.STATUSONLY = STATUSONLY;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(FM_RDS_STATUS);
    Wire.write(rds_cmd.raw);
    Wire.endTransmission();

    waitToSend();

    // Get response
    Wire.requestFrom(SI473X_ADDR, 13);

    // Gets response information
    // currentRdsStatus will have the RDS information
    for (byte i = 0; i < 13; i++)
    {
        currentRdsStatus.raw[i] = Wire.read();
    }

    delayMicroseconds(550);
}

/*
 * Gets RDS Status.
 * Call getRdsStatus(byte INTACK, byte MTFIFO, byte STATUSONLY) if you want other behaviour
 */
void SI4735::getRdsStatus()
{
    getRdsStatus(0, 0, 0);
}

/*
 * Set RDS property  FM_RDS_CONFIG
 * 
 * @param byte RDSEN RDS Processing Enable; 1 = RDS processing enabled.
 * @param byte BLETHA Block Error Threshold BLOCKA.   
 * @param byte BLETHB Block Error Threshold BLOCKB.  
 * @param byte BLETHC Block Error Threshold BLOCKC.  
 * @param byte BLETHD Block Error Threshold BLOCKD. 
 *  
 * IMPORTANT: 
 * All block errors must be less than or equal the associated block error threshold 
 * for the group to be stored in the RDS FIFO. 
 * 0 = No errors.
 * 1 = 1–2 bit errors detected and corrected. 
 * 2 = 3–5 bit errors detected and corrected. 
 * 3 = Uncorrectable.
 * Recommended Block Error Threshold options:
 *  2,2,2,2 = No group stored if any errors are uncorrected.
 *  3,3,3,3 = Group stored regardless of errors.
 *  0,0,0,0 = No group stored containing corrected or uncorrected errors.
 *  3,2,3,3 = Group stored with corrected errors on B, regardless of errors on A, C, or D.
 */
void SI4735::setRdsConfig(byte RDSEN, byte BLETHA, byte BLETHB, byte BLETHC, byte BLETHD)
{
    si47x_property property;
    si47x_rds_config config;

    waitToSend();

    // Set property value
    property.value = FM_RDS_CONFIG;

    // Arguments
    config.arg.RDSEN = RDSEN;
    config.arg.BLETHA = BLETHA;
    config.arg.BLETHB = BLETHB;
    config.arg.BLETHC = BLETHC;
    config.arg.BLETHD = BLETHD;
    config.arg.DUMMY1 = 0;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00 (I need to check it)
    Wire.write(property.raw.byteHigh); // Send property - High Byte - most significant first
    Wire.write(property.raw.byteLow);  // Low Byte
    Wire.write(config.raw[1]);         // Send the argments. Most significant first
    Wire.write(config.raw[0]);
    Wire.endTransmission();
    delayMicroseconds(550);
}

// TO DO

/* 
 * Returns the programa type. 
 * Read the Block A content
 */
unsigned SI4735::getRdsPI(void)
{

    if (getRdsReceived() && getRdsNewBlockA())
    {
        return currentRdsStatus.resp.BLOCKAL;
    }
    return 0;
}

/*
 * Returns the Group Type (extracted from the Block B) 
 */
unsigned SI4735::getRdsGroupType(void)
{

    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.groupType;
}

/*
 * Gets the version code (extracted from the Block B)
 * Returns  0=A or 1=B
 */
unsigned SI4735::getRdsVersionCode(void)
{

    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.versionCode;
}

/* 
 * Returns the Program Type (extracted from the Block B)
 */
unsigned SI4735::getRdsProgramType(void)
{

    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.programType;
}

char *SI4735::getNext2Block(char *c)
{
    c[1] = (currentRdsStatus.resp.BLOCKDL < 32 || currentRdsStatus.resp.BLOCKDL > 127) ? '.' : currentRdsStatus.resp.BLOCKDL;
    c[0] = (currentRdsStatus.resp.BLOCKDH < 32 || currentRdsStatus.resp.BLOCKDH > 127) ? '.' : currentRdsStatus.resp.BLOCKDH;
}

char *SI4735::getNext4Block(char *c)
{

    c[1] = (currentRdsStatus.resp.BLOCKCL < 32 || currentRdsStatus.resp.BLOCKCL > 127) ? '.' : currentRdsStatus.resp.BLOCKCL;
    c[0] = (currentRdsStatus.resp.BLOCKCH < 32 || currentRdsStatus.resp.BLOCKCH > 127) ? '.' : currentRdsStatus.resp.BLOCKCH;
    c[3] = (currentRdsStatus.resp.BLOCKDL < 32 || currentRdsStatus.resp.BLOCKDL > 127) ? '.' : currentRdsStatus.resp.BLOCKDL;
    c[2] = (currentRdsStatus.resp.BLOCKDH < 32 || currentRdsStatus.resp.BLOCKDH > 127) ? '.' : currentRdsStatus.resp.BLOCKDH;
}

/*
 * Gets the RDS Text when the message is of the Group Type 2 version A
 */
String SI4735::getRdsText(void)
{
    // Under Test and construction...

    si47x_rds_blockb blkb;
    byte offset;
    byte newB;

    for (int i = 0; i < 64; i++)
        rds_buffer[i] = ' ';

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    offset = blkb.refined.content;
    if (offset < 16)
        getNext4Block(&rds_buffer[offset * 4]);
    do
    {
        getRdsStatus();
        blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
        blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    } while (offset == blkb.refined.content);
    offset = blkb.refined.content;
    if (offset < 16)
        getNext4Block(&rds_buffer[offset * 4]);

    do
    {
        getRdsStatus();
        blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
        blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    } while (offset == blkb.refined.content);
    offset = blkb.refined.content;
    if (offset < 16)
        getNext4Block(&rds_buffer[offset * 4]);

    do
    {
        getRdsStatus();
        blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
        blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    } while (offset == blkb.refined.content);
    offset = blkb.refined.content;
    if (offset < 16)
        getNext4Block(&rds_buffer[offset * 4]);

    do
    {
        getRdsStatus();
        blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
        blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;
    } while (offset == blkb.refined.content);
    offset = blkb.refined.content;
    if (offset < 16)
        getNext4Block(&rds_buffer[offset * 4]);

    rds_buffer[64] = 0;

    return String(rds_buffer);
}

/* 
 * Gets the RDS time and date when the Group type is 4 
 */
String SI4735::getRdsTime()
{

    // Under Test and construction

    si47x_rds_date_time dt;

    String s;
    unsigned y, m, d;

    dt.raw[2] = currentRdsStatus.resp.BLOCKCL;
    dt.raw[3] = currentRdsStatus.resp.BLOCKCH;

    dt.raw[0] = currentRdsStatus.resp.BLOCKDL;
    dt.raw[1] = currentRdsStatus.resp.BLOCKDH;

    y = (unsigned)(dt.refined.mjd - 15078.2) / 365.25;
    m = ((unsigned)(dt.refined.mjd - 14956.1) - (unsigned)(y * 365.25)) / 30.6001;
    d = (unsigned)(dt.refined.mjd - 14956) - (unsigned)(y * 365.25) - (m * 30.6001);

    if (m > 13)
        y++;

    y = y % 100;

    s = String(dt.refined.hour) + ":" + String(dt.refined.minute) + " - " + String(d) + "/" + String(m) +
        "/" + String(y) + "-" + String(dt.refined.offset);

    return s;
}

/*
 * SSB Comand and properties implementation 
 * 
 * First consideration: I will write here about the Patches released by SIlicon Labs for some customers....
 * 
 */

/* 
 * Sets the SSB Beat Frequency Offset (BFO). 
 * @param offset 16-bit signed value (unit in Hz). The valid range is -16383 to +16383 Hz. 
 */
void SI4735::setSsbBfo(int offset)
{

    si47x_property property;
    si47x_frequency bfo_offset;

    if (currentTune == FM_TUNE_FREQ) // Only for AM/SSB mode
        return;

    waitToSend();

    property.value = SSB_BFO;
    bfo_offset.value = offset;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00
    Wire.write(property.raw.byteHigh); // High byte first
    Wire.write(property.raw.byteLow);  // Low byte after
    Wire.write(bfo_offset.raw.FREQH);  // Offset freq. high byte first
    Wire.write(bfo_offset.raw.FREQL);  // Offset freq. low byte first

    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * Set the SSB receiver mode details:
 * 1) Enable or disable AFC track to carrier function for receiving normal AM signals;
 * 2) Set the audio bandwidth;
 * 3) Set the side band cutoff filter;
 * 4) Set soft-mute based on RSSI or SNR;
 * 5) Enable or disbable automatic volume control (AVC) function. 
 * 
 * See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 
 * 
 * @param AUDIOBW SSB Audio bandwidth; 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz.
 * @param SBCUTFLT SSB side band cutoff filter for band passand low pass filter
 *                 if 0, the band pass filter to cutoff both the unwanted side band and high frequency 
 *                  component > 2KHz of the wanted side band (default).
 * @param AVC_DIVIDER set 0 for SSB mode; set 3 for SYNC mode.
 * @param AVCEN SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default).
 * @param SMUTESEL SSB Soft-mute Based on RSSI or SNR.
 * @param DSP_AFCDIS DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable. 
 */
void SI4735::setSsbMode(byte AUDIOBW, byte SBCUTFLT, byte AVC_DIVIDER, byte AVCEN, byte SMUTESEL, byte DSP_AFCDIS)
{
    si47x_ssb_mode ssb;
    si47x_property property;

    if (currentTune == FM_TUNE_FREQ) // Only AM/SSB mode
        return;

    waitToSend();

    property.value = SSB_MODE;

    ssb.param.AUDIOBW = AUDIOBW;
    ssb.param.SBCUTFLT = SBCUTFLT;
    ssb.param.AVC_DIVIDER = AVC_DIVIDER;
    ssb.param.AVCEN = AVCEN;
    ssb.param.SMUTESEL = SMUTESEL;
    ssb.param.DSP_AFCDIS = DSP_AFCDIS;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00
    Wire.write(property.raw.byteHigh); // High byte first
    Wire.write(property.raw.byteLow);  // Low byte after
    Wire.write(ssb.raw[1]);            // SSB MODE params; freq. high byte first
    Wire.write(ssb.raw[0]);            // SSB MODE params; freq. low byte after

    Wire.endTransmission();
    delayMicroseconds(550);
}
