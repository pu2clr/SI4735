/*
 * This is a library for the SI4735, BROADCAST AM/FM/SW RADIO RECEIVER, IC from Silicon Labs for the 
 * Arduino development environment.  It works with I2C protocol. 
 * This library is intended to provide an easier interface for controlling the SI4435.
 * See documentation on https://github.com/pu2clr/SI4735.
 * 
 * See also: 
 *  Si47XX PROGRAMMING GUIDE; AN332
 *  AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES
 * 
 * Pay attention: 
 * According to Si47XX PROGRAMMING GUIDE; AN332; page 207, "For write operations, the system controller next 
 * sends a data byte on SDIO, which is captured by the device on rising edges of SCLK. The device acknowledges 
 * each data byte by driving SDIO low for one cycle on the next falling edge of SCLK. 
 * The system controller may write up to 8 data bytes in a single 2-wire transaction. 
 * The first byte is a command, and the next seven bytes are arguments. Writing more than 8 bytes results 
 * in unpredictable device behavior". So, If you are extending this library, consider that restriction presented earlier.
 * 
 * ATTENTION: inline methods are implemented in SI4735.h
 * 
 * By Ricardo Lima Caratti, Nov 2019.
 */

#include <SI4735.h>

SI4735::SI4735()
{
    clearRdsBuffer2A();
    clearRdsBuffer2B();
    clearRdsBuffer0A();

    rdsTextAdress2A = rdsTextAdress2B = lastTextFlagAB = rdsTextAdress0A = 0;

    // 1 = LSB and 2 = USB; 0 = AM, FM or WB
    currentSsbStatus = 0;
}

void SI4735::clearRdsBuffer2A()
{
    for (int i = 0; i < 65; i++)
        rds_buffer2A[i] = ' '; // Radio Text buffer - Program Information
}

void SI4735::clearRdsBuffer2B()
{
    for (int i = 0; i < 33; i++)
        rds_buffer2B[i] = ' '; // Radio Text buffer - Station Informaation
}

void SI4735::clearRdsBuffer0A()
{
    for (int i = 0; i < 9; i++)
        rds_buffer0A[i] = ' '; // Station Name buffer
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
    delay(100);
    digitalWrite(resetPin, LOW);
    delay(100);
    digitalWrite(resetPin, HIGH);
    delay(100);
}

/*
 * Wait for the si473x is ready (Clear to Send status bit have to be 1).  
 */
void SI4735::waitToSend()
{
    do
    {
        delayMicroseconds(500);
        Wire.requestFrom(SI473X_ADDR, 1);
    } while (!(Wire.read() & B10000000));
}

/*
 * Powerup in Analog Mode
 * You have to call setPowerUp method before. 
 */
void SI4735::analogPowerUp(void)
{
    // delayMicroseconds(1000);
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(powerUp.raw[0]); // Content of ARG1
    Wire.write(powerUp.raw[1]); // COntent of ARG2
    Wire.endTransmission();
    // Delay at least 500 ms between powerup command and first tune command to wait for
    // the oscillator to stabilize if XOSCEN is set and crystal is used as the RCLK.
    waitToSend();
    delay(550);
}

/* 
 * Moves the device from powerup to powerdown mode.
 * After Power Down command, only the Power Up command is accepted.
 */
void SI4735::powerDown(void)
{
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_DOWN);
    Wire.endTransmission();
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

    do
    {
        waitToSend();
        // Request for 9 bytes response
        Wire.requestFrom(SI473X_ADDR, 9);
        for (int i = 0; i < 9; i++)
            firmwareInfo.raw[i] = Wire.read();
    } while (firmwareInfo.resp.ERR);
}

/* 
 * Starts the Si473X device. 
 * 
 * @param uint8_t resetPin Digital Arduino Pin used to RESET command 
 * @param uint8_t interruptPin interrupt Arduino Pin (see your Arduino pinout). If less than 0, iterrupt disabled
 * @param uint8_t defaultFunction
 */
void SI4735::setup(uint8_t resetPin, int interruptPin, uint8_t defaultFunction)
{
    uint8_t interruptEnable = 0;
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
    setPowerUp(interruptEnable, 0, 0, 1, defaultFunction, SI473X_ANALOG_AUDIO);

    reset();
    analogPowerUp();
    setVolume(30); // Default volume level.
    getFirmware();
}

/* 
 * Starts the Si473X device.  
 * Use this setup if you are not using interrupt resource
 * 
 * @param uint8_t resetPin Digital Arduino Pin used to RESET command 
 * @param uint8_t defaultFunction
 */
void SI4735::setup(uint8_t resetPin, uint8_t defaultFunction)
{
    setup(resetPin, -1, defaultFunction);
}

/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * @param uint8_t CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param uint8_t GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param uint8_t PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param uint8_t XOSCEN sets external Crystal enabled or disabled 
 * @param uint8_t FUNC sets the receiver function have to be used [0 = FM Receive; 1 = AM (LW/MW/SW) and SSB (if SSB patch apllied)]
 * @param uint8_t OPMODE set the kind of audio mode you want to use.
 */
void SI4735::setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE)
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
    currentFrequencyParams.arg.ANTCAPL = 1;
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
void SI4735::setTuneFrequencyAntennaCapacitor(uint16_t capacitor)
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
 * @param uint16_t  freq Is the frequency to change. For example, FM => 10390 = 103.9 MHz; AM => 810 = 810 KHz. 
 */
void SI4735::setFrequency(uint16_t freq)
{
    waitToSend(); // Wait for the si473x is ready.
    currentFrequency.value = freq;
    currentFrequencyParams.arg.FREQH = currentFrequency.raw.FREQH;
    currentFrequencyParams.arg.FREQL = currentFrequency.raw.FREQL;

    if (currentSsbStatus != 0)
    {
        currentFrequencyParams.arg.DUMMY1 = 0;
        currentFrequencyParams.arg.USBLSB = currentSsbStatus; // Set to LSB or USB
        currentFrequencyParams.arg.FAST = 0;                  // Used just on AM and FM
        currentFrequencyParams.arg.FREEZE = 0;                // Used just on FM
    }

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(currentTune);
    Wire.write(currentFrequencyParams.raw[0]); // Send a byte with FAST and  FREEZE information; if not FM must be 0;
    Wire.write(currentFrequencyParams.arg.FREQH);
    Wire.write(currentFrequencyParams.arg.FREQL);
    Wire.write(currentFrequencyParams.arg.ANTCAPH);
    // If current tune is not FM sent one more byte
    if (currentTune != FM_TUNE_FREQ)
        Wire.write(currentFrequencyParams.arg.ANTCAPL);
    Wire.endTransmission();
    delayMicroseconds(2000);
    currentWorkFrequency = freq; // check it
    waitToSend();                // Wait for the si473x is ready.
}

/* 
 * Set the current step value. 
 * @param step if you are using FM, 10 means 100KHz. If you are using AM 10 means 10KHz
 *             For AM, 1 (1KHz) to 10 (10KHz) are valid values.
 *             For FM 5 (50KHz) and 10 (100KHz) are valid values.  
 */
void SI4735::setFrequencyStep(uint8_t step)
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
    powerDown();
    setPowerUp(1, 1, 0, 1, 1, SI473X_ANALOG_AUDIO);
    analogPowerUp();
    setVolume(volume); // Set to previus configured volume
    currentSsbStatus = 0;
}

/*
 * Set the radio to FM function
 */
void SI4735::setFM()
{
    powerDown();
    setPowerUp(1, 1, 0, 1, 0, SI473X_ANALOG_AUDIO);
    analogPowerUp();
    setVolume(volume); // Set to previus configured volume
    currentSsbStatus = 0;
    disableFmDebug();
}

/*
 * Set the radio to AM (LW/MW/SW) function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency 
 * @param step step used to go to the next channel   
 */
void SI4735::setAM(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint8_t step)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setAM();

    currentWorkFrequency = initialFreq;

    setFrequency(currentWorkFrequency);
}

/*
 * Set the radio to FM function. 
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency (default frequency)
 * @param step step used to go to the next channel   
 */
void SI4735::setFM(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint8_t step)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setFM();

    currentWorkFrequency = initialFreq;
    setFrequency(currentWorkFrequency);
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
void SI4735::setBandwidth(uint8_t AMCHFLT, uint8_t AMPLFLT)
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
    waitToSend();
}

/*
 * Returns true (1) if the current function is FM (FM_TUNE_FREQ).
 */
bool SI4735::isCurrentTuneFM()
{
    return (currentTune == FM_TUNE_FREQ);
}

/*
 * Send (set) property to the SI47XX
 * This method is used for others to send generic properties and params to SI47XX
 */
void SI4735::sendProperty(uint16_t propertyValue, uint16_t parameter)
{

    si47x_property property;
    si47x_property param;

    property.value = propertyValue;
    param.value = parameter;
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);
    Wire.write(property.raw.byteHigh); // Send property - High byte - most significant first
    Wire.write(property.raw.byteLow);  // Send property - Low byte - less significant after
    Wire.write(param.raw.byteHigh);    // Send the argments. High Byte - Most significant first
    Wire.write(param.raw.byteLow);     // Send the argments. Low Byte - Less significant after
    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * Sets RSSI threshold for stereo blend (Full stereo above threshold, blend below threshold). 
 * To force stereo, set this to 0. To force mono, set this to 127.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 90. 
 */
void SI4735::setFmBlendStereoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_STEREO_THRESHOLD, parameter);
}

/*
 * Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). 
 * To force stereo set this to 0. To force mono set this to 127. Default value is 30 dBμV.
 *  See Si47XX PROGRAMMING GUIDE; AN332; page 56.
 */
void SI4735::setFmBlendMonoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_MONO_THRESHOLD, parameter);
}

/* 
 * Sets RSSI threshold for stereo blend. (Full stereo above threshold, blend below threshold.) 
 * To force stereo, set this to 0. To force mono, set this to 127. Default value is 49 dBμV.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 59. 
 */
void SI4735::setFmBlendRssiStereoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_RSSI_STEREO_THRESHOLD, parameter);
}

/*
 * Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). 
 * To force stereo, set this to 0. To force mono, set this to 127. Default value is 30 dBμV.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 59.  
 */
void SI4735::setFmBLendRssiMonoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_RSSI_MONO_THRESHOLD, parameter);
}

/*
 * Sets SNR threshold for stereo blend (Full stereo above threshold, blend below threshold). 
 * To force stereo, set this to 0. To force mono, set this to 127. Default value is 27 dB.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 59.  
 */
void SI4735::setFmBlendSnrStereoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_SNR_STEREO_THRESHOLD, parameter);
}

/*
 * Sets SNR threshold for mono blend (Full mono below threshold, blend above threshold). 
 * To force stereo, set this to 0. To force mono, set this to 127. Default value is 14 dB.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 59. 
 */
void SI4735::setFmBLendSnrMonoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_SNR_MONO_THRESHOLD, parameter);
}

/* 
 * Sets multipath threshold for stereo blend (Full stereo below threshold, blend above threshold). 
 * To force stereo, set this to 100. To force mono, set this to 0. Default value is 20.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 60.
 */
void SI4735::setFmBlendMultiPathStereoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_MULTIPATH_STEREO_THRESHOLD, parameter);
}

/*
 * Sets Multipath threshold for mono blend (Full mono above threshold, blend below threshold). 
 * To force stereo, set to 100. To force mono, set to 0. The default is 60.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 60.
 */
void SI4735::setFmBlendMultiPathMonoThreshold(uint8_t parameter)
{
    sendProperty(FM_BLEND_MULTIPATH_MONO_THRESHOLD, parameter);
}

/* 
 * Turn Off Stereo operation.
 */
void SI4735::setFmStereoOff()
{
    // TO DO
}

/* 
 * Turn Off Stereo operation.
 */
void SI4735::setFmStereoOn()
{
    // TO DO
}

/*
 * There is a debug feature that remains active in Si4704/05/3x-D60 firmware which can create periodic noise in audio.
 * Silicon Labs recommends you disable this feature by sending the following bytes (shown here in hexadecimal form):
 * 0x12 0x00 0xFF 0x00 0x00 0x00.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 299. 
 */
void SI4735::disableFmDebug()
{
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(0x12);
    Wire.write(0x00);
    Wire.write(0xFF);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    delayMicroseconds(2500);
}

/*
 * Gets the current frequency of the Si4735 (AM or FM)
 * The method status do it an more. See getStatus below. 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)
 */
uint16_t SI4735::getFrequency()
{
    si47x_frequency freq;
    getStatus(0, 1);
    freq.raw.FREQL = currentStatus.resp.READFREQL;
    freq.raw.FREQH = currentStatus.resp.READFREQH;

    currentWorkFrequency = freq.value;

    // getCurrentReceivedSignalQuality(0);

    return freq.value;
}

/*
 * Gets the current frequency saved in memory. 
 * Unlike getFrequency, this method gets the current frequency recorded after the last setFrequency command. 
 * This method avoids bus traffic and CI processing.
 * However, you can not get others status information like RSSI.
 */
uint16_t SI4735::getCurrentFrequency()
{
    return currentWorkFrequency;
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
 * @param uint8_t INTACK Seek/Tune Interrupt Clear. If set, clears the seek/tune complete interrupt status indicator;
 * @param uint8_t CANCEL Cancel seek. If set, aborts a seek currently in progress;
 * 
 */
void SI4735::getStatus(uint8_t INTACK, uint8_t CANCEL)
{
    si47x_tune_status status;
    uint8_t cmd = (currentTune == FM_TUNE_FREQ) ? FM_TUNE_STATUS : AM_TUNE_STATUS;

    waitToSend();

    status.arg.INTACK = INTACK;
    status.arg.CANCEL = CANCEL;

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(status.raw);
    Wire.endTransmission();
    // Reads the current status (including current frequency).
    do
    {
        waitToSend();
        Wire.requestFrom(SI473X_ADDR, 7);
        // Gets response information
        for (uint8_t i = 0; i < 7; i++)
            currentStatus.raw[i] = Wire.read();
    } while (currentStatus.resp.ERR); // If error, try it again
}

/*
 * Queries AGC STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 80; for AM page 142.
 * See AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18. 
 * After call this method, you can call isAgcEnabled to know the AGC status and getAgcGainIndex to know the gain index value.
 */
void SI4735::getAutomaticGainControl()
{
    uint8_t cmd;

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

    do
    {
        waitToSend();
        Wire.requestFrom(SI473X_ADDR, 3);
        currentAgcStatus.raw[0] = Wire.read(); // STATUS response
        currentAgcStatus.raw[1] = Wire.read(); // RESP 1
        currentAgcStatus.raw[2] = Wire.read(); // RESP 2
    } while (currentAgcStatus.refined.ERR);    // If error, try get AGC status again.
}

/* 
 * If FM, overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 
 * (minimum attenuation) and 26 (maximum attenuation);
 * If AM/SSB, Overrides the AM AGC setting by disabling the AGC and forcing the gain index that ranges between 0 
 * (minimum attenuation) and 37+ATTN_BACKUP (maximum attenuation);
 * 
 * @param uint8_t AGCDIS This param selects whether the AGC is enabled or disabled (0 = AGC enabled; 1 = AGC disabled);
 * @param uint8_t AGCIDX AGC Index (0 = Minimum attenuation (max gain); 1 – 36 = Intermediate attenuation); 
 *             > 37 - Maximum attenuation (min gain) ).
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 81; for AM page 143 
 */
void SI4735::setAutomaticGainControl(uint8_t AGCDIS, uint8_t AGCIDX)
{
    si47x_agc_overrride agc;

    uint8_t cmd;

    cmd = (currentTune == FM_TUNE_FREQ) ? FM_AGC_OVERRIDE : AM_AGC_OVERRIDE;

    agc.arg.AGCDIS = AGCDIS;
    agc.arg.AGCIDX = AGCIDX;

    waitToSend();

    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(agc.raw[0]);
    Wire.write(agc.raw[1]);
    Wire.endTransmission();

    waitToSend();
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
void SI4735::getCurrentReceivedSignalQuality(uint8_t INTACK)
{

    uint8_t arg;
    uint8_t cmd;
    int sizeResponse;

    if (currentTune == FM_TUNE_FREQ)
    { // FM TUNE
        cmd = FM_RSQ_STATUS;
        sizeResponse = 7;
    }
    else
    { // AM TUNE
        cmd = AM_RSQ_STATUS;
        sizeResponse = 5;
    }

    waitToSend();

    arg = INTACK;
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(cmd);
    Wire.write(arg); // send B00000001
    Wire.endTransmission();

    do
    {
        waitToSend();
        Wire.requestFrom(SI473X_ADDR, sizeResponse);
        // Gets response information
        for (uint8_t i = 0; i < sizeResponse; i++)
            currentRqsStatus.raw[i] = Wire.read();
    } while (currentRqsStatus.resp.ERR); // Try again if error found
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
void SI4735::getCurrentReceivedSignalQuality(void)
{
    getCurrentReceivedSignalQuality(0);
}

/*
 * Look for a station 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 55, 72, 125 and 137
 * 
 * @param SEEKUP Seek Up/Down. Determines the direction of the search, either UP = 1, or DOWN = 0. 
 * @param Wrap/Halt. Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
 */
void SI4735::seekStation(uint8_t SEEKUP, uint8_t WRAP)
{
    si47x_seek seek;

    // Check which FUNCTION (AM or FM) is working now
    uint8_t seek_start = (currentTune == FM_TUNE_FREQ) ? FM_SEEK_START : AM_SEEK_START;

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
 * Sets volume level
 * @param uint8_t volume (domain: 0 - 63) 
 */
void SI4735::setVolume(uint8_t volume)
{

    sendProperty(RX_VOLUME, volume);
    this->volume = volume;
}

/*
 * Gets the current volume level.
 */
uint8_t SI4735::getVolume()
{
    return this->volume;
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
    if (volume > 0)
        volume--;
    setVolume(volume);
}

/* 
 * RDS implementation  (UNDER CONSTRUCTION.....)
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
void SI4735::setRdsIntSource(uint8_t RDSNEWBLOCKB, uint8_t RDSNEWBLOCKA, uint8_t RDSSYNCFOUND, uint8_t RDSSYNCLOST, uint8_t RDSRECV)
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
    Wire.write(property.raw.byteHigh); // Send property - High byte - most significant first
    Wire.write(property.raw.byteLow);  // Low byte
    Wire.write(rds_int_source.raw[1]); // Send the argments. Most significant first
    Wire.write(rds_int_source.raw[0]);
    Wire.endTransmission();
    waitToSend();
}

/*
 * RDS COMMAND FM_RDS_STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78
 * @param INTACK Interrupt Acknowledge; 0 = RDSINT status preserved. 1 = Clears RDSINT.
 * @param MTFIFO 0 = If FIFO not empty, read and remove oldest FIFO entry; 1 = Clear RDS Receive FIFO.
 * @param STATUSONLY Determines if data should be removed from the RDS FIFO.
 */
void SI4735::getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY)
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

    do
    {
        waitToSend();
        // Gets response information
        Wire.requestFrom(SI473X_ADDR, 13);
        for (uint8_t i = 0; i < 13; i++)
            currentRdsStatus.raw[i] = Wire.read();
    } while (currentRdsStatus.resp.ERR);
}

/*
 * Gets RDS Status.
 * Same result of calling getRdsStatus(0,0,0);
 * Please, call getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY) instead getRdsStatus() 
 * if you want other behaviour
 */
void SI4735::getRdsStatus()
{
    getRdsStatus(0, 0, 0);
}

/*
 * Set RDS property  FM_RDS_CONFIG
 * 
 * @param uint8_t RDSEN RDS Processing Enable; 1 = RDS processing enabled.
 * @param uint8_t BLETHA Block Error Threshold BLOCKA.   
 * @param uint8_t BLETHB Block Error Threshold BLOCKB.  
 * @param uint8_t BLETHC Block Error Threshold BLOCKC.  
 * @param uint8_t BLETHD Block Error Threshold BLOCKD. 
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
void SI4735::setRdsConfig(uint8_t RDSEN, uint8_t BLETHA, uint8_t BLETHB, uint8_t BLETHC, uint8_t BLETHD)
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
    Wire.write(property.raw.byteHigh); // Send property - High byte - most significant first
    Wire.write(property.raw.byteLow);  // Low byte
    Wire.write(config.raw[1]);         // Send the argments. Most significant first
    Wire.write(config.raw[0]);
    Wire.endTransmission();
    delayMicroseconds(550);
}

// TO DO

// See inlines methods / functions on SI4735.h

/* 
 * Returns the programa type. 
 * Read the Block A content
 */
uint16_t SI4735::getRdsPI(void)
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
uint8_t SI4735::getRdsGroupType(void)
{
    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.groupType;
}

/*
 * Returns the current Text Flag A/B  
 */
uint8_t SI4735::getRdsFlagAB(void)
{
    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.textABFlag;
}

/*
 * Returns the address of the text segment.
 * 2A - Each text segment in version 2A groups consists of four characters. A messages of this group can be 
 *      have up to 64 characters. 
 * 2B - In version 2B groups, each text segment consists of only two characters. When the current RDS status is
 *      using this version, the maximum message length will be 32 characters.
 */

uint8_t SI4735::getRdsTextSegmentAddress(void)
{

    si47x_rds_blockb blkb;
    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.content;
}

/*
 * Gets the version code (extracted from the Block B)
 * Returns  0=A or 1=B
 */
uint8_t SI4735::getRdsVersionCode(void)
{
    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.versionCode;
}

/* 
 * Returns the Program Type (extracted from the Block B)
 */
uint8_t SI4735::getRdsProgramType(void)
{
    si47x_rds_blockb blkb;

    blkb.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
    blkb.raw.highValue = currentRdsStatus.resp.BLOCKBH;

    return blkb.refined.programType;
}

char *SI4735::getNext2Block(char *c)
{
    char raw[2];
    int i, j;

    raw[1] = currentRdsStatus.resp.BLOCKDL;
    raw[0] = currentRdsStatus.resp.BLOCKDH;

    for (i = j = 0; i < 2; i++)
    {
        if (raw[i] == 0xD)
        {
            c[j] = '\0';
            return;
        }
        if (raw[i] >= 32 and raw[i] <= 127)
        {
            c[j] = raw[i];
            j++;
        }
        else
        {
            c[i] = ' ';
        }
    }
}

char *SI4735::getNext4Block(char *c)
{
    char raw[4];
    int i, j;

    raw[0] = currentRdsStatus.resp.BLOCKCH;
    raw[1] = currentRdsStatus.resp.BLOCKCL;
    raw[2] = currentRdsStatus.resp.BLOCKDH;
    raw[3] = currentRdsStatus.resp.BLOCKDL;
    for (i = j = 0; i < 4; i++)
    {
        if (raw[i] == 0xD || raw[i] == 0xA)
        {
            c[j] = '\0';
            return;
        }
        if (raw[i] >= 32 and raw[i] <= 127)
        {
            c[j] = raw[i];
            j++;
        }
        else
        {
            c[i] = ' ';
        }
    }
}

/*
 * Gets the RDS Text when the message is of the Group Type 2 version A
 */
char *SI4735::getRdsText(void)
{

    // Needs to get the "Text segment address code".
    // Each message should be ended by the code 0D (Hex)

    if (rdsTextAdress2A >= 16)
        rdsTextAdress2A = 0;

    getNext4Block(&rds_buffer2A[rdsTextAdress2A * 4]);

    rdsTextAdress2A += 4;

    return rds_buffer2A;
}

char *SI4735::getRdsText0A(void)
{
    si47x_rds_blockb blkB;

    // getRdsStatus();

    // if (getRdsReceived())
    //{
        // if (getRdsNewBlockB())
        // {
        if (getRdsGroupType() == 0)
        {
            // Process group type 0
            blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
            blkB.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
            rdsTextAdress0A = blkB.group0.address;
            if (rdsTextAdress0A >= 0 && rdsTextAdress0A < 4)
            {
                getNext2Block(&rds_buffer0A[rdsTextAdress0A * 2]);
                rds_buffer0A[8] = '\0';
                return rds_buffer0A;
            }
        }
        // }
    // }
    return NULL;
}

char *SI4735::getRdsText2A(void)
{

    si47x_rds_blockb blkB;

    // getRdsStatus();

    // if (getRdsReceived())
    // {
    /*
        if (lastTextFlagAB != getRdsFlagAB())
        {
            lastTextFlagAB = getRdsFlagAB();
            clearRdsBuffer2A();
        }
        */
    //  if (getRdsNewBlockB())
    //  {
    if (getRdsGroupType() == 2 /* && getRdsVersionCode() == 0 */)
    {
        // Process group 2A
        // Decode B block information
        blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
        blkB.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
        rdsTextAdress2A = blkB.group2.address;
        if (rdsTextAdress2A >= 0 && rdsTextAdress2A < 16)
        {
            getNext4Block(&rds_buffer2A[rdsTextAdress2A * 4]);
            rds_buffer2A[63] = '\0';
            return rds_buffer2A;
        }
    }
    // }
    // }
    return NULL;
}

char *SI4735::getRdsText2B(void)
{
    si47x_rds_blockb blkB;

    // getRdsStatus();
    // if (getRdsReceived())
    // {
        // if (getRdsNewBlockB())
        // {
        if (getRdsGroupType() == 2 /* && getRdsVersionCode() == 1 */)
        {
            // Process group 2B
            blkB.raw.highValue = currentRdsStatus.resp.BLOCKBH;
            blkB.raw.lowValue = currentRdsStatus.resp.BLOCKBL;
            rdsTextAdress2B = blkB.group2.address;
            if (rdsTextAdress2B >= 0 && rdsTextAdress2B < 16)
            {
                getNext2Block(&rds_buffer2B[rdsTextAdress2B * 2]);
                return rds_buffer2B;
            }
        }
        //  }
    // }
    return NULL;
}

/* 
 * Gets the RDS time and date when the Group type is 4 
 */
char * SI4735::getRdsTime()
{
    // Under Test and construction
    // Need to check the Group Type before.
    si47x_rds_date_time dt;

    if (getRdsGroupType() == 4)
    {
        String s;
        uint16_t y, m, d;

        dt.raw[4] = currentRdsStatus.resp.BLOCKBL;
        dt.raw[5] = currentRdsStatus.resp.BLOCKBH;

        dt.raw[2] = currentRdsStatus.resp.BLOCKCL;
        dt.raw[3] = currentRdsStatus.resp.BLOCKCH;

        dt.raw[0] = currentRdsStatus.resp.BLOCKDL;
        dt.raw[1] = currentRdsStatus.resp.BLOCKDH;

        y = (unsigned)(dt.refined.mjd - 15078.2) / 365.25;
        m = ((unsigned)(dt.refined.mjd - 14956.1) - (unsigned)(y * 365.25)) / 30.6001;
        d = (unsigned)(dt.refined.mjd - 14956) - (unsigned)(y * 365.25) - (m * 30.6001);

        if (m > 13) {
            m = 1;
            y++;
        }

        y = y % 100;

        s = String(dt.refined.hour) + ":" + String(dt.refined.minute) + " - " + String(d) + "/" + String(m) +
            "/" + String(y) + "-" + String(dt.refined.offset);

        s.toCharArray(rds_time,19);
        rds_time[19] = '\0';
        return rds_time;
    }

    return NULL;
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
void SI4735::setSSBBfo(int offset)
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
void SI4735::setSSBConfig(uint8_t AUDIOBW, uint8_t SBCUTFLT, uint8_t AVC_DIVIDER, uint8_t AVCEN, uint8_t SMUTESEL, uint8_t DSP_AFCDIS)
{
    si47x_property property;

    if (currentTune == FM_TUNE_FREQ) // Only AM/SSB mode
        return;

    property.value = SSB_MODE;

    currentSSBMode.param.AUDIOBW = AUDIOBW;
    currentSSBMode.param.SBCUTFLT = SBCUTFLT;
    currentSSBMode.param.AVC_DIVIDER = AVC_DIVIDER;
    currentSSBMode.param.AVCEN = AVCEN;
    currentSSBMode.param.SMUTESEL = SMUTESEL;
    currentSSBMode.param.DUMMY1 = 0;
    currentSSBMode.param.DSP_AFCDIS = DSP_AFCDIS;

    sendSSBModeProperty();
}

/* 
 * Sets DSP AFC disable or enable
 * 0 = SYNC mode, AFC enable
 * 1 = SSB mode, AFC disable
 */
void SI4735::setSSBDspAfc(uint8_t DSP_AFCDIS)
{
    currentSSBMode.param.DSP_AFCDIS;
    sendSSBModeProperty();
}

/* 
 * Sets SSB Soft-mute Based on RSSI or SNR Selection:
 * 0 = Soft-mute based on RSSI (default).
 * 1 = Soft-mute based on SNR.
 */
void SI4735::setSSBSoftMute(uint8_t SMUTESEL)
{
    currentSSBMode.param.SMUTESEL;
    sendSSBModeProperty();
}

/*
 * Sets SSB Automatic Volume Control (AVC) for SSB mode
 * 0 = Disable AVC.
 * 1 = Enable AVC (default).
 */
void SI4735::setSSBAutomaticVolumeControl(uint8_t AVCEN)
{
    currentSSBMode.param.AVCEN = AVCEN;
    sendSSBModeProperty();
}

/*
 * Sets AVC Divider
 * for SSB mode, set divider = 0
 * for SYNC mode, set divider = 3 Other values = not allowed.
 */
void SI4735::setSSBAvcDivider(uint8_t AVC_DIVIDER)
{
    currentSSBMode.param.AVC_DIVIDER;
    sendSSBModeProperty();
}

/* 
 * Sets SBB Sideband Cutoff Filter for band pass and low pass filters:
 * 0 = Band pass filter to cutoff both the unwanted side band and high frequency components > 2.0 kHz of the wanted side band. (default)
 * 1 = Low pass filter to cutoff the unwanted side band. 
 * Other values = not allowed.
 */
void SI4735::setSBBSidebandCutoffFilter(uint8_t SBCUTFLT)
{
    currentSSBMode.param.SBCUTFLT;
    sendSSBModeProperty();
}

/*
 * SSB Audio Bandwidth for SSB mode
 * 
 * 0 = 1.2 kHz low-pass filter* . (default)
 * 1 = 2.2 kHz low-pass filter* .
 * 2 = 3.0 kHz low-pass filter.
 * 3 = 4.0 kHz low-pass filter.
 * 4 = 500 Hz band-pass filter for receiving CW signal, i.e. [250 Hz, 750 Hz]
 *     with center frequency at 500 Hz when USB is selected or [-250 Hz, -750 1Hz] with center 
 *     frequency at -500Hz when LSB is selected* .
 * 5 = 1 kHz band-pass filter for receiving CW signal, i.e. [500 Hz, 1500 Hz] with center 
 *     frequency at 1 kHz when USB is selected or [-500 Hz, -1500 1 Hz] with center frequency 
 *     at -1kHz when LSB is selected* .
 * Other values = reserved.
 * Note:
 *   If audio bandwidth selected is about 2 kHz or below, it is recommended to set SBCUTFLT[3:0] to 0 
 *   to enable the band pass filter for better high- cut performance on the wanted side band. 
 *   Otherwise, set it to 1.
 * 
 * See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 
 */
void SI4735::setSSBAudioBandwidth(uint8_t AUDIOBW)
{
    // Sets the audio filter property parameter
    currentSSBMode.param.AUDIOBW = AUDIOBW;
    sendSSBModeProperty();
}

/*
 * Set the radio to AM function. It means: LW MW and SW.
 */
void SI4735::setSSB(uint8_t usblsb)
{
    // Is it needed to load patch when switch to SSB?
    // powerDown();
    // It starts with the same AM parameters.
    setPowerUp(1, 1, 0, 1, 1, SI473X_ANALOG_AUDIO);
    analogPowerUp();
    // ssbPowerUp(); // Not used for regular operation
    setVolume(volume); // Set to previus configured volume
    currentSsbStatus = usblsb;
}

/*
 * Set the radio to SSB (LW/MW/SW) function. 
 * 
 * See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 13 and 14
 * 
 * @param fromFreq minimum frequency for the band
 * @param toFreq maximum frequency for the band
 * @param initialFreq initial frequency 
 * @param step step used to go to the next channel  
 * @param usblsb SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection; 
 *               value 2 (banary 10) = USB; 
 *               value 1 (banary 01) = LSB.   
 */
void SI4735::setSSB(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint8_t step, uint8_t usblsb)
{

    currentMinimumFrequency = fromFreq;
    currentMaximumFrequency = toFreq;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setSSB(usblsb);

    currentWorkFrequency = initialFreq;

    setFrequency(currentWorkFrequency);

    delayMicroseconds(550);
}

/* 
 * Just send the property SSB_MOD to the device. 
 * Internal use (privete method). 
 */
void SI4735::sendSSBModeProperty()
{
    si47x_property property;
    property.value = SSB_MODE;
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(SET_PROPERTY);
    Wire.write(0x00);                  // Always 0x00
    Wire.write(property.raw.byteHigh); // High byte first
    Wire.write(property.raw.byteLow);  // Low byte after
    Wire.write(currentSSBMode.raw[1]); // SSB MODE params; freq. high byte first
    Wire.write(currentSSBMode.raw[0]); // SSB MODE params; freq. low byte after

    Wire.endTransmission();
    delayMicroseconds(550);
}

/*
 * ****************
 * PATCH RESOURCES
 */

/*
   Call it first if you are applying a patch on SI4735. 
   Used to confirm if the patch is compatible with the internal device library revision.
   See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.

   @return a struct si47x_firmware_query_library (see it in SI4735.h)
*/
si47x_firmware_query_library
SI4735::queryLibraryId()
{
    si47x_firmware_query_library libraryID;

    powerDown(); // Is it necessary

    delay(500);

    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(0b00011111);          // Set to Read Library ID, disable interrupt; disable GPO2OEN; boot normaly; enable External Crystal Oscillator  .
    Wire.write(SI473X_ANALOG_AUDIO); // Set to Analog Line Input.
    Wire.endTransmission();

    do
    {
        waitToSend();
        Wire.requestFrom(SI473X_ADDR, 8);
        for (int i = 0; i < 8; i++)
            libraryID.raw[i] = Wire.read();
    } while (libraryID.resp.ERR); // If error found, try it again.

    delayMicroseconds(2500);

    return libraryID;
}

/*
 *  This method can be used to prepare the device to apply SSBRX patch
 *  Call queryLibraryId before call this method. 
 *  Powerup the device by issuing the POWER_UP command with FUNC = 1 (AM/SW/LW Receive) 
 *  See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220 and
 *  AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES; page 7.
 */
void SI4735::patchPowerUp()
{
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(0b00110001);          // Set to AM, Enable External Crystal Oscillator; Set patch enable; GPO2 output disabled; CTS interrupt disabled.
    Wire.write(SI473X_ANALOG_AUDIO); // Set to Analog Output
    Wire.endTransmission();
    delayMicroseconds(2500);
}

// Used for test
void SI4735::ssbPowerUp()
{
    waitToSend();
    Wire.beginTransmission(SI473X_ADDR);
    Wire.write(POWER_UP);
    Wire.write(0b00010001); // Set to AM/SSB, disable interrupt; disable GPO2OEN; boot normaly; enable External Crystal Oscillator  .
    Wire.write(0b00000101); // Set to Analog Line Input.
    Wire.endTransmission();
    delayMicroseconds(2500);

    powerUp.arg.CTSIEN = 0;          // 1 -> Interrupt anabled;
    powerUp.arg.GPO2OEN = 0;         // 1 -> GPO2 Output Enable;
    powerUp.arg.PATCH = 0;           // 0 -> Boot normally;
    powerUp.arg.XOSCEN = 1;          // 1 -> Use external crystal oscillator;
    powerUp.arg.FUNC = 1;            // 0 = FM Receive; 1 = AM/SSB (LW/MW/SW) Receiver.
    powerUp.arg.OPMODE = 0b00000101; // 0x5 = 00000101 = Analog audio outputs (LOUT/ROUT).
}

/*
 *  Transfers the content of a patch stored in a array of bytes to the SI4735 device. 
 *  You must mount an array as shown below and know the size of that array as well.
 *  See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.  
 * 
 *  It is importante to say  that patches to the SI4735 are distributed in binary form and 
 *  have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino).
 *  Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off 
 *  the system. Consequently, the content of the patch has to be transferred again to the device 
 *  each time after turn on the system or reset the device.
 * 
 *  The disadvantage of this approach is the amount of memory used by the patch content. 
 *  This may limit the use of other radio functions you want implemented in Arduino.
 * 
 *  Example of content:
 *  const PROGMEM uint8_t ssb_patch_content_full[] =
 *   { // SSB patch for whole SSBRX full download
 *       0x15, 0x00, 0x0F, 0xE0, 0xF2, 0x73, 0x76, 0x2F,
 *       0x16, 0x6F, 0x26, 0x1E, 0x00, 0x4B, 0x2C, 0x58,
 *       0x16, 0xA3, 0x74, 0x0F, 0xE0, 0x4C, 0x36, 0xE4,
 *          .
 *          .
 *          .
 *       0x16, 0x3B, 0x1D, 0x4A, 0xEC, 0x36, 0x28, 0xB7,
 *       0x16, 0x00, 0x3A, 0x47, 0x37, 0x00, 0x00, 0x00,
 *       0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9D, 0x29};   
 * 
 *  const int size_content_full = sizeof ssb_patch_content_full;
 * 
 *  @param ssb_patch_content point to array of bytes content patch.
 *  @param ssb_patch_content_size array size (number of bytes). The maximum size allowed for a patch is 15856 bytes
 * 
 *  @return false if an error is found.
 */
bool SI4735::downloadPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size)
{
    uint8_t content, cmd_status;
    int i, offset;
    // Send patch to the SI4735 device
    for (offset = 0; offset < ssb_patch_content_size; offset += 8)
    {
        Wire.beginTransmission(SI473X_ADDR);
        for (i = 0; i < 8; i++)
        {
            content = pgm_read_byte_near(ssb_patch_content + (i + offset));
            Wire.write(content);
        }
        Wire.endTransmission();
        waitToSend();
        Wire.requestFrom(SI473X_ADDR, 1);
        cmd_status = Wire.read();
        // The SI4735 issues a status after each 8 byte transfered.
        // Just the bit 7 (CTS) should be seted. if bit 6 (ERR) is seted, the system halts.
        if (cmd_status != 0x80)
            return false;
    }
    delayMicroseconds(2500);
    return true;
}

/*
 * Transfers the content of a patch stored in a eeprom to the SI4735 device.
 * 
 * TO USE THIS METHOD YOU HAVE TO HAVE A EEPROM WRITEN WITH THE PATCH CONTENT
 * See the sketch write_ssb_patch_eeprom.ino (TO DO)
 * 
 * @param eeprom_i2c_address 
 * @return false if an error is found.
 */
bool SI4735::downloadPatch(int eeprom_i2c_address)
{
    int ssb_patch_content_size;
    uint8_t content, cmd_status;
    int i, offset;
    uint8_t eepromPage[8];

    union {
        struct
        {
            uint8_t lowByte;
            uint8_t highByte;
        } raw;
        uint16_t value;
    } eeprom;

    // The first two bytes are the size of the patches
    // Set the position in the eeprom to read the size of the patch content
    Wire.beginTransmission(eeprom_i2c_address);
    Wire.write(0); // writes the most significant byte
    Wire.write(0); // writes the less significant byte
    Wire.endTransmission();
    Wire.requestFrom(eeprom_i2c_address, 2);
    eeprom.raw.highByte = Wire.read();
    eeprom.raw.lowByte = Wire.read();

    ssb_patch_content_size = eeprom.value;

    // the patch content starts on position 2 (the first two bytes are the size of the patch)
    for (offset = 2; offset < ssb_patch_content_size; offset += 8)
    {
        // Set the position in the eeprom to read next 8 bytes
        eeprom.value = offset;
        Wire.beginTransmission(eeprom_i2c_address);
        Wire.write(eeprom.raw.highByte); // writes the most significant byte
        Wire.write(eeprom.raw.lowByte);  // writes the less significant byte
        Wire.endTransmission();

        // Reads the next 8 bytes from eeprom
        Wire.requestFrom(eeprom_i2c_address, 8);
        for (i = 0; i < 8; i++)
            eepromPage[i] = Wire.read();

        // sends the page (8 bytes) to the SI4735
        Wire.beginTransmission(SI473X_ADDR);
        for (i = 0; i < 8; i++)
            Wire.write(eepromPage[i]);
        Wire.endTransmission();

        waitToSend();

        Wire.requestFrom(SI473X_ADDR, 1);
        cmd_status = Wire.read();
        // The SI4735 issues a status after each 8 byte transfered.
        // Just the bit 7 (CTS) should be seted. if bit 6 (ERR) is seted, the system halts.
        if (cmd_status != 0x80)
            return false;
    }
    delayMicroseconds(2500);
    return true;
}
