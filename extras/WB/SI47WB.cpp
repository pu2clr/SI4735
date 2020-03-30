// Under construction.... 
#include <SI47WB.h>

/*
 * Set the Power Up parameters for si473X. 
 * Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
 * @see Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129
 * @param uint8_t CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled )
 * @param uint8_t GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled )
 * @param uint8_t PATCH  Used for firmware patch updates. Use it always 0 here. 
 * @param uint8_t XOSCEN sets external Crystal enabled or disabled 
 * @param uint8_t FUNC sets the receiver function have to be used [0 = FM Receive; 1 = AM (LW/MW/SW) and SSB (if SSB patch apllied)]
 * @param uint8_t OPMODE set the kind of audio mode you want to use.
 */
void SI47WB::setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE)
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
    else if (FUNC == 1)
    {
        currentTune = AM_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 0;
    }
    else
    {
        currentTune = WB_TUNE_FREQ;
        currentFrequencyParams.arg.FREEZE = 0;
    }

    currentFrequencyParams.arg.FAST = 1;
    currentFrequencyParams.arg.DUMMY1 = 0;
    currentFrequencyParams.arg.ANTCAPH = 0;
    currentFrequencyParams.arg.ANTCAPL = 1;
}

/*
 * Set the frequency to the corrent function of the Si4735 (AM, FM or WB)
 * You have to call setup or setPowerUp before call setFrequency.
 * 
 * @param uint16_t  freq Is the frequency to change. For example, FM => 10390 = 103.9 MHz; AM => 810 = 810 KHz. 
 */

void SI47WB::setFrequency(uint16_t freq)
{
    waitToSend(); // Wait for the si473x is ready.
    currentFrequency.value = freq;
    currentFrequencyParams.arg.FREQH = currentFrequency.raw.FREQH;
    currentFrequencyParams.arg.FREQL = currentFrequency.raw.FREQL;

    if (currentSsbStatus != 0)
    {
        currentFrequencyParams.arg.DUMMY1 = 0;
        currentFrequencyParams.arg.USBLSB = currentSsbStatus; // Set to LSB or USB
        currentFrequencyParams.arg.FAST = 1;                  // Used just on AM and FM
        currentFrequencyParams.arg.FREEZE = 0;                // Used just on FM
    }

    Wire.beginTransmission(deviceAddress);
    Wire.write(currentTune);
    Wire.write(currentFrequencyParams.raw[0]); // Send a byte with FAST and  FREEZE information; if not FM must be 0;
    Wire.write(currentFrequencyParams.arg.FREQH);
    Wire.write(currentFrequencyParams.arg.FREQL);
    if (currentTune != WB_TUNE_FREQ ) { 
        Wire.write(currentFrequencyParams.arg.ANTCAPH);
        // If current tune is AM or SSB sent one more byte
        if (currentTune != FM_TUNE_FREQ) 
            Wire.write(currentFrequencyParams.arg.ANTCAPL);
    }
    Wire.endTransmission();
    waitToSend();                         // Wait for the si473x is ready.
    currentWorkFrequency = freq;          // check it
    delay(MAX_DELAY_AFTER_SET_FREQUENCY); // For some reason I need to delay here.
} 

/*
 * Selects the tuning capacitor value.
 * 
 * For FM, Antenna Tuning Capacitor is valid only when using TXO/LPI pin as the antenna input.
 * 
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332; pages 71 and 136
 * 
 * 
 * @param capacitor If zero, the tuning capacitor value is selected automatically. 
 *                  If the value is set to anything other than 0:
 *                  AM - the tuning capacitance is manually set as 95 fF x ANTCAP + 7 pF. ANTCAP manual range is 1–6143;
 *                  FM - the valid range is 0 to 191.    
 *                  According to Silicon Labs, automatic capacitor tuning is recommended (value 0). 
 */
void SI47WB::setTuneFrequencyAntennaCapacitor(uint16_t capacitor)
{

    si47x_antenna_capacitor cap;

    cap.value = capacitor;

    currentFrequencyParams.arg.DUMMY1 = 0;

    if (currentTune == FM_TUNE_FREQ || currentTune == WB_TUNE_FREQ)
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
 * Sets the WB Receive to tune the frequency between 162.4 MHz and 162.55 MHz in 2.5 kHz units. 
 * For example 162.4 MHz = 64960 * 2.5 and 162.55 MHz = 65020 * 2.5.
 */
void SI47WB::setWB()
{
    powerDown();
    setPowerUp(1, 1, 0, 1, 3, SI473X_ANALOG_AUDIO);
    radioPowerUp();
    setVolume(volume); // Set to previus configured volume
    currentSsbStatus = 0;
    lastMode = WB_CURRENT_MODE;
}

/*
 * Sets the WB Receive to tune the frequency between 162.4 MHz and 162.55 MHz in 2.5 kHz units. 
 * For example 162.4 MHz = 64960 (* 2.5) and 162.55 MHz = 65020 (* 2.5).
 *
 * @param fromFreq minimum frequency for the band (162.4)
 * @param toFreq maximum frequency for the band (162.55)
 * @param initialFreq initial frequency (default frequency) (162.4)
 * @param step step used to go to the next channel 
 */
void SI47WB::setWB(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step)
{

    currentMinimumFrequency = fromFreq / 2.5;
    currentMaximumFrequency = toFreq / 2.5;
    currentStep = step;

    if (initialFreq < fromFreq || initialFreq > toFreq)
        initialFreq = fromFreq;

    setWB();

    currentWorkFrequency = initialFreq;
    setFrequency(currentWorkFrequency);
}
