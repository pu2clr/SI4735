// This sketch is a modified version of Mr. Goshante's sketch, which in 
// turn is based on a sketch I developed and made excellent interface 
// improvements. The changes made in this sketch are mainly enhancements 
// to the FM RDS functions. I would like to thank Mr. for the excellent 
// work done.
// Ricardo/PU2CLR 09.2024 
// The original comment about this sketch can be seeing below.
// ----------------------------------------------------------------------
// ATS_EX (Extended) Firmware for ATS-20 and ATS-20+ receivers.
// Based on PU2CLR sources.
// Inspired by closed-source swling.ru firmware.
// For more information check README file in my github repository:
// http://github.com/goshante/ats20_ats_ex
// ----------------------------------------------------------------------
// By Goshante
// 02.2024
// http://github.com/goshante
// ----------------------------------------------------------------------

#include <SI4735.h>
#include <EEPROM.h>
#include <Tiny4kOLED.h>
#include <PixelOperatorBold.h> 

#include "font14x24sevenSeg.h"
#include "Rotary.h"
#include "SimpleButton.h"
#include "patch_ssb_compressed.h"

#include "defs.h"
#include "globals.h"
#include "Utils.h"

void showStatus(bool cleanFreq = false);
void applyBandConfiguration(bool extraSSBReset = false);

bool isSSB()
{
    return g_currentMode > AM && g_currentMode < FM;
}

int getSteps()
{
    if (isSSB())
    {
        if (g_stepIndex >= g_amTotalSteps)
            return g_tabStep[g_stepIndex];

        return g_tabStep[g_stepIndex] * 1000;
    }

    if (g_stepIndex >= g_amTotalSteps)
        g_stepIndex = 0;

    return g_tabStep[g_stepIndex];
}

int getLastStep()
{
    if (isSSB())
        return g_amTotalSteps + g_ssbTotalSteps - 1;

    return g_amTotalSteps - 1;
}

// --------------------------
// ------- Main logic -------
// --------------------------

#define APP_VERSION 118

//Initialize controller
void setup()
{
    //We need to save more space with this
    DDRB |=  (1 << DDB5);   //13 pin
    DDRD &= ~(1 << ENCODER_PIN_A);
    PORTD |= (1 << ENCODER_PIN_A);
    DDRD &= ~(1 << ENCODER_PIN_B);
    PORTD |= (1 << ENCODER_PIN_B);
    g_voltagePinConnnected = analogRead(BATTERY_VOLTAGE_PIN) > 300;

    oled.begin(128, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
    oled.clear();
    oled.on();
    oled.setFont(DEFAULT_FONT);

    //Don't use digitalRead()
    //Registers save us more space
    if (!(PINC & (1 << (ENCODER_BUTTON - 14))))
    {
        saveAllReceiverInformation();
        oled.print("  EEPROM RESET");
        oled.setCursor(0, 2);
        for (uint8_t i = 0; i < 16; i++)
        {
            oled.print("-"); //Just fancy animation
            delay(60);
        }
    }
    else
    {
        oledPrint(" ATS-20 RECEIVER", 0, 0, DEFAULT_FONT, true);
        oledPrint("ATS_EX v1.18", 16, 2);
        oledPrint("Goshante 2024", 12, 4);
        oledPrint("Best firmware", 12, 6);
        delay(2000);
    }
    oled.clear();

    //Encoder interrupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), rotaryEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), rotaryEncoder, CHANGE);

    g_si4735.getDeviceI2CAddress(RESET_PIN);
    g_si4735.setup(RESET_PIN, MW_BAND_TYPE);

    delay(500);

    //Load settings from EEPROM
    if (EEPROM.read(EEPROM_VERSION_ADDRESS) == APP_VERSION && EEPROM.read(EEPROM_APP_ID_ADDRESS) == EEPROM_APP_ID)
        readAllReceiverInformation();
    else
        saveAllReceiverInformation();

    //Clock speed configuration
    noInterrupts(); //cli()
    CLKPR = 0x80;   //Allow edit CLKPR register
    CLKPR = g_Settings[SettingsIndex::CPUSpeed].param;
    interrupts();   //sei()

    //Initialize current band settings and read frequency
    applyBandConfiguration();
    g_currentFrequency = g_previousFrequency = g_si4735.getFrequency();
    g_si4735.setVolume(g_volume);

    //Draw main screen
    oled.clear();
    showStatus();
}

uint8_t volumeEvent(uint8_t event, uint8_t pin)
{
    if (g_muteVolume)
    {
        if (!BUTTONEVENT_ISDONE(event))
        {
            if ((BUTTONEVENT_SHORTPRESS != event) || (VOLUME_BUTTON == pin))
                doVolume(1);
        }
    }

    if (!g_muteVolume)
    {
#if (0 != VOLUME_DELAY)
#if (VOLUME_DELAY > 1)
        static uint8_t count;
        if (BUTTONEVENT_FIRSTLONGPRESS == event)
        {
            count = 0;
        }
#endif
        if (BUTTONEVENT_ISLONGPRESS(event))
            if (BUTTONEVENT_LONGPRESSDONE != event)
            {
#if (VOLUME_DELAY > 1)
                if (count++ == 0)
#endif
                    doVolume(VOLUME_BUTTON == pin ? 1 : -1);
#if (VOLUME_DELAY > 1)
                count = count % VOLUME_DELAY;
#endif
            }
#else
        if (BUTTONEVENT_FIRSTLONGPRESS == event)
            event = BUTTONEVENT_SHORTPRESS;
#endif
    }
    return event;
}

uint8_t simpleEvent(uint8_t event, uint8_t pin)
{
    if (BUTTONEVENT_FIRSTLONGPRESS == event)
        event = BUTTONEVENT_SHORTPRESS;
    return event;
}

//This looks like it's better to remove them and use only simpleEvent
//But it's a part of a hack that allows us to save more flash image size
uint8_t stepEvent(uint8_t event, uint8_t pin)
{
    return simpleEvent(event, pin);
}

uint8_t agcEvent(uint8_t event, uint8_t pin)
{
    return simpleEvent(event, pin);
}

uint8_t bandEvent(uint8_t event, uint8_t pin)
{
#if (0 != BAND_DELAY)
    static uint8_t count;
    if (BUTTONEVENT_ISLONGPRESS(event) && !g_settingsActive)
    {
        if (BUTTONEVENT_LONGPRESSDONE != event)
        {
            if (BUTTONEVENT_FIRSTLONGPRESS == event)
            {
                count = 0;
            }
            if (count++ == 0)
            {
                if (BAND_BUTTON == pin)
                {
                    if (g_bandIndex < g_lastBand)
                        bandSwitch(true);
                }
                else
                {
                    if (g_bandIndex)
                        bandSwitch(false);
                }
            }
            count = count % BAND_DELAY;
        }
    }
#else
    if (BUTTONEVENT_FIRSTLONGPRESS == event)
        event = BUTTONEVENT_SHORTPRESS;
#endif
    return event;
}

// Handle encoder direction
void rotaryEncoder()
{
    uint8_t encoderStatus = g_encoder.process();
    if (encoderStatus)
    {
        g_encoderCount = (encoderStatus == DIR_CW) ? 1 : -1;
        g_seekStop = true;
    }
}

//Saves more flash image size
void updateSSBCutoffFilter()
{
    // Auto mode: If SSB bandwidth 2 KHz or lower - it's better to enable cutoff filter
    if (g_Settings[SettingsIndex::CutoffFilter].param == 0 || g_currentMode == CW)
        g_si4735.setSSBSidebandCutoffFilter((g_bandwidthSSB[g_bwIndexSSB].idx == 0 || g_bandwidthSSB[g_bwIndexSSB].idx == 4 || g_bandwidthSSB[g_bwIndexSSB].idx == 5) ? 0 : 1);
    else
        g_si4735.setSSBSidebandCutoffFilter(g_Settings[SettingsIndex::CutoffFilter].param - 1);
}

//EEPROM Save
void saveAllReceiverInformation()
{
    uint8_t addr = EEPROM_DATA_START_ADDRESS;
    EEPROM.update(EEPROM_VERSION_ADDRESS, APP_VERSION);
    EEPROM.update(EEPROM_APP_ID_ADDRESS, EEPROM_APP_ID);

    EEPROM.update(addr++, g_muteVolume > 0 ? g_muteVolume : g_si4735.getVolume());
    EEPROM.update(addr++, g_bandIndex);
    EEPROM.update(addr++, g_currentMode);
    EEPROM.update(addr++, g_currentBFO >> 8);
    EEPROM.update(addr++, g_currentBFO & 0XFF);
    EEPROM.update(addr++, g_FMStepIndex);
    EEPROM.update(addr++, g_prevMode); 
    EEPROM.update(addr++, g_bwIndexSSB);

    for (uint8_t i = 0; i <= g_lastBand; i++)
    {
        EEPROM.update(addr++, (g_bandList[i].currentFreq >> 8));
        EEPROM.update(addr++, (g_bandList[i].currentFreq & 0xFF));
        EEPROM.update(addr++, ((i != FM_BAND_TYPE && g_bandList[i].currentStepIdx >= g_amTotalSteps) ? 0 : g_bandList[i].currentStepIdx));
        EEPROM.update(addr++, g_bandList[i].bandwidthIdx);
    }

    for (uint8_t i = 0; i < SettingsIndex::SETTINGS_MAX; i++)
        EEPROM.update(addr++, g_Settings[i].param);
}

//EEPROM Load
void readAllReceiverInformation()
{
    uint8_t addr = EEPROM_DATA_START_ADDRESS;
    int8_t bwIdx;
    g_volume = EEPROM.read(addr++);
    g_bandIndex = EEPROM.read(addr++);
    g_currentMode = EEPROM.read(addr++);
    g_currentBFO = (EEPROM.read(addr++) << 8) | EEPROM.read(addr++);
    g_FMStepIndex = EEPROM.read(addr++);
    g_prevMode = EEPROM.read(addr++);
    g_bwIndexSSB = EEPROM.read(addr++);

    for (uint8_t i = 0; i <= g_lastBand; i++)
    {
        g_bandList[i].currentFreq = (EEPROM.read(addr++) << 8) | EEPROM.read(addr++);
        g_bandList[i].currentStepIdx = EEPROM.read(addr++);
        g_bandList[i].bandwidthIdx = EEPROM.read(addr++);
    }

    for (uint8_t i = 0; i < SettingsIndex::SETTINGS_MAX; i++)
        g_Settings[i].param = EEPROM.read(addr++);

    oled.setContrast(uint8_t(g_Settings[SettingsIndex::Brightness].param) * 2);

    g_previousFrequency = g_currentFrequency = g_bandList[g_bandIndex].currentFreq;
    if (g_bandIndex == FM_BAND_TYPE)
        g_FMStepIndex = g_bandList[g_bandIndex].currentStepIdx;
    else
        g_stepIndex = g_bandList[g_bandIndex].currentStepIdx;
    bwIdx = g_bandList[g_bandIndex].bandwidthIdx;
    if (g_stepIndex >= g_amTotalSteps)
        g_stepIndex = 0;

    if (isSSB())
    {
        loadSSBPatch();
        g_si4735.setSSBAudioBandwidth(g_bandwidthSSB[g_bwIndexSSB].idx);
        updateSSBCutoffFilter();
    }
    else if (g_currentMode == AM)
    {
        g_bwIndexAM = bwIdx;
        g_si4735.setBandwidth(g_bandwidthAM[g_bwIndexAM].idx, 1);
    }
    else
    {
        g_bwIndexFM = bwIdx;
        g_si4735.setFmBandwidth(g_bwIndexFM);
    }

    applyBandConfiguration();
}

//For saving features
void resetEepromDelay()
{
    g_storeTime = millis();
    g_previousFrequency = 0;
}

//Draw frequency. 
//BFO and main frequency produce actual frequency that is displayed on LCD
//Too sensitive logic, do not change
void showFrequency(bool cleanDisplay = false)
{
    if (g_settingsActive)
        return;

    char unit[4];
    char freqDisplay[7];
    char ssbSuffix[4];
    static uint8_t prevLen = 0;
    uint16_t khzBFO, tailBFO;
    uint8_t off = (isSSB() ? -5 : 4) + 8;

    unit[0] = 'K';
    unit[1] = 'H';
    unit[2] = 'z';
    unit[3] = 0x0;

    ssbSuffix[0] = '.';
    ssbSuffix[1] = '0';
    ssbSuffix[2] = '0';
    ssbSuffix[3] = '\0';

    if (g_bandIndex == FM_BAND_TYPE)
    {
        convertToChar(freqDisplay, g_currentFrequency, 5, 3, '.', '/');
        unit[0] = 'M';
    }
    else
    {
        if (g_bandIndex == SW_BAND_TYPE)
            showBandTag();

        if (!isSSB())
        {
            bool swMhz = g_Settings[SettingsIndex::SWUnits].param == 1;
            convertToChar(freqDisplay, g_currentFrequency, 5, (g_bandIndex == SW_BAND_TYPE && swMhz) ? 2 : 0, '.', '/');
            if (g_bandIndex == SW_BAND_TYPE && swMhz)
                unit[0] = 'M';
        }
        else
        {
            splitFreq(khzBFO, tailBFO);
            //utoa(freqDisplay, khzBFO);
            convertToChar(freqDisplay, khzBFO, ilen(khzBFO));
        }
    }

    uint8_t len = isSSB() ? ilen(khzBFO) : ilen(g_currentFrequency);
    if (cleanDisplay)
    {
        oled.setCursor(0, 3);
        oledPrint("/////////", 0, 3, FONT14X24SEVENSEG); // This character is an empty space in my seven seg font.
    }
    else if (isSSB() && len > prevLen && len == 5)
        oledPrint("   ", 102, 4, DEFAULT_FONT);

    oledPrint(freqDisplay, off, 3, FONT14X24SEVENSEG);

    if (isSSB())
    {
        //utoa((ilen(tailBFO) == 1) ? &ssbSuffix[2] : &ssbSuffix[1], tailBFO);
        convertToChar((ilen(tailBFO) == 1) ? &ssbSuffix[2] : &ssbSuffix[1], tailBFO, ilen(tailBFO));
        ssbSuffix[3] = 0;
        oledPrint(ssbSuffix);
        if (len != prevLen && len < prevLen)
            oledPrint("/");
    }

    if (g_Settings[SettingsIndex::UnitsSwitch].param == 1 && (!isSSB() || isSSB() && len < 5))
        oledPrint(unit, 102, 4, DEFAULT_FONT);
        
    prevLen = len;
}

//This function is called by station seek logic
void showFrequencySeek(uint16_t freq)
{
    g_currentFrequency = freq;
    delay(10);
    if (g_currentMode == FM)
    {
        //Fix random 10th KHz fraction
        freq = (freq / 10) * 10;
        g_currentFrequency = freq;
        g_si4735.setFrequency(g_currentFrequency);
    }
    else
        g_currentFrequency = g_si4735.getFrequency();

    g_bandList[g_bandIndex].currentFreq = g_currentFrequency;
    showFrequency();
}

bool checkStopSeeking()
{
    return g_seekStop || !(PINC & (1 << (ENCODER_BUTTON - 14)));
}

void doSeek()
{
    if (g_seekDirection)
        g_si4735.frequencyUp();
    else
        g_si4735.frequencyDown();

#if USE_RDS
    if (g_displayRDS)
        oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
#endif
    g_seekStop = false;
    g_si4735.seekStationProgress(showFrequencySeek, checkStopSeeking, g_seekDirection);
}

//Update and draw main screen UI. 
//basicUpdate - update minimum as possible
//cleanFreq   - force clean frequency line
void showStatus(bool cleanFreq)
{
    showFrequency(cleanFreq);
    showModulation();
    showStep();
    showBandwidth();
    showCharge(true);
    showVolume();
}

void updateLowerDisplayLine()
{
    oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
    showModulation();
    showStep();
    showCharge(true);
}

//Converts settings value to UI value
void SettingParamToUI(char* buf, uint8_t idx)
{
    int8_t param = g_Settings[idx].param;
    switch (g_Settings[idx].type)
    {
    case SettingType::ZeroAuto:
        if (param == 0)
        {
            buf[0] = 'A';
            buf[1] = 'U';
            buf[2] = 'T';
            buf[3] = 0x0;
        }
        else
            convertToChar(buf, param, 3);

        break;

    case SettingType::Num:
        convertToChar(buf, abs(param), 3);
        if (param < 0)
            buf[0] = '-';
        break;

    case SettingType::SwitchAuto:
        if (param == 0)
        {
            buf[0] = 'A';
            buf[1] = 'U';
            buf[2] = 'T';       }
        else if (param == 1)
        {
            buf[0] = 'O';
            buf[1] = 'n';
            buf[2] = ' ';
        }
        else
        {
            buf[0] = 'O';
            buf[1] = 'f';
            buf[2] = 'f';
        }
        buf[3] = 0x0;
        break;

    case SettingType::Switch:
        if (idx == SettingsIndex::DeEmp)
        {
            if (param == 0)
            {
                buf[0] = '5';
                buf[1] = '0';
                buf[2] = 'u';
            }
            else
            {
                buf[0] = '7';
                buf[1] = '5';
                buf[2] = 'u';
            }
        }
        else if (idx == SettingsIndex::SWUnits)
        {
            if (param == 0)
                buf[0] = 'K';
            else
                buf[0] = 'M';
            buf[1] = 'H';
            buf[2] = 'z';
        }
        else if (idx == SettingsIndex::SSM)
        {
            if (param == 0)
            {
                buf[0] = 'R';
                buf[1] = 'S';
                buf[2] = 'S';
            }
            else
            {
                buf[0] = 'S';
                buf[1] = 'N';
                buf[2] = 'R';
            }
        }
        else if (idx == SettingsIndex::CWSwitch)
        {
            if (param == 0)
                buf[0] = 'L';
            else
                buf[0] = 'U';

            buf[1] = 'S';
            buf[2] = 'B';
        }
        else if (idx == SettingsIndex::CPUSpeed)
        {
            if (param == 0)
            {
                buf[0] = '1';
                buf[1] = '0';
                buf[2] = '0';
            }
            else
            {
                buf[0] = '5';
                buf[1] = '0';
                buf[2] = '%';
            }
        }
        else
        {
            if (param == 0)
            {
                buf[0] = 'O';
                buf[1] = 'f';
                buf[2] = 'f';
            }
            else
            {
                buf[0] = 'O';
                buf[1] = 'n';
                buf[2] = ' ';
            }
        }
        buf[3] = 0x0;
        break;
    }
}

// If full false - update only value
void DrawSetting(uint8_t idx, bool full)
{
    if (!g_settingsActive)
        return;

    char buf[5];
    uint8_t place = idx - ((g_SettingsPage - 1) * 6);
    uint8_t yOffset = place > 2 ? (place - 3) * 2 : place * 2;
    uint8_t xOffset = place > 2 ? 60 : 0;
    if (full)
        oledPrint(g_Settings[idx].name, 5 + xOffset, 2 + yOffset, DEFAULT_FONT, idx == g_SettingSelected && !g_SettingEditing);
    SettingParamToUI(buf, idx);
    oledPrint(buf, 35 + xOffset, 2 + yOffset, DEFAULT_FONT, idx == g_SettingSelected && g_SettingEditing);
}

//Update and draw settings UI
void showSettings()
{
    for (uint8_t i = 0; i < 6 && i + ((g_SettingsPage - 1) * 6) < SettingsIndex::SETTINGS_MAX; i++)
        DrawSetting(i + ((g_SettingsPage - 1) * 6), true);
}

void showSettingsTitle()
{
    oledPrint("   SETTINGS  ", 0, 0, DEFAULT_FONT, true);
    oled.invertOutput(true);
    oled.print(uint8_t(g_SettingsPage));
    oled.print("/");
    oled.print(uint8_t(g_SettingsMaxPages));
    oled.invertOutput(false);
}

void switchSettingsPage()
{
    g_SettingsPage++;
    g_SettingsPage = (g_SettingsPage > g_SettingsMaxPages) ? 1 : g_SettingsPage;
    g_SettingSelected = 6 * (g_SettingsPage - 1);
    g_SettingEditing = false;
    oled.clear();
    showSettingsTitle();
    showSettings();
}

//Switch between main screen and settings mode
void switchSettings()
{
    oled.clear();
    if (g_settingsActive)
    {
        g_SettingsPage = 1;
        showSettingsTitle();
        g_SettingSelected = 0;
        g_SettingEditing = false;
        showSettings();
    }
    else
    {
        saveAllReceiverInformation();
        showStatus();
    }
}

//Draw curremt modulation
void showModulation()
{
    oledPrint(g_bandModeDesc[g_currentMode], 0, 0, DEFAULT_FONT, g_cmdBand && g_currentMode == FM);
    oled.print(" ");
    if (isSSB() && g_Settings[SettingsIndex::Sync].param == 1)
        oledPrint("S", -1, -1, LastFont, true);
    else
        oled.print(" ");

    showBandTag();
}

//Draw current band
void showBandTag()
{
    if (g_sMeterOn || g_displayRDS)
        return;

    oledPrint((g_currentFrequency >= CB_LIMIT_LOW && g_currentFrequency < CB_LIMIT_HIGH)? "CB" : bandTags[g_bandIndex], 0, 6, DEFAULT_FONT, g_cmdBand && g_currentMode != FM);
}

//Draw volume level
void showVolume()
{
    if (g_settingsActive)
        return;

    char buf[3];
    if (g_muteVolume == 0)
        convertToChar(buf, g_si4735.getCurrentVolume(), 2, 0, 0);
    else
    {
        buf[0] = ' ';
        buf[1] = 'M';
        buf[2] = 0;
    }

    oledPrint(buf, (128 - (8 * 2) + 2 - 6), 0, DEFAULT_FONT, g_cmdVolume);
}

//Draw battery charge
//This feature requires hardware mod
//Voltage divider made of two 10 KOhm resistors between + and GND of Li-Ion battery
//Solder it to A2 analog pin
void showCharge(bool forceShow)
{
    if (!g_voltagePinConnnected)
        return;

    // mV, Percent
    //This values represent voltage values in ATMega328p analog units with reference voltage 3.30v
    //Voltage pin reads voltage from voltage divider, so it have to be 1/2 of Li-Ion battery voltage
    constexpr const uint8_t rows = 10;
    const uint16_t dischargeTable[rows][2] =
    {
        { 650, 100 },  //4.15v
        { 630, 95  },  //4.05v
        { 614, 90  },  //3.90v
        { 591, 80  },  //3.75v
        { 583, 60  },  //3.70v
        { 568, 40  },  //3.60v
        { 552, 20  },  //3.50v
        { 513, 15  },  //3.25v
        { 503, 5  },   //3.20v
        { 478, 0  },   //3.15v
    };

/*
    const uint16_t dischargeTable[rows][2] =
    {
        { 512, 100 },  //4.15v
        { 498, 95  },  //4.05v
        { 485, 90  },  //3.90v
        { 471, 80  },  //3.75v
        { 458, 60  },  //3.70v
        { 444, 40  },  //3.60v
        { 431, 20  },  //3.50v
        { 417, 15  },  //3.25v
        { 404, 5  },   //3.20v
        { 390, 0  },   //3.15v
    };
*/    

    auto getBatteryPercentage = [&](uint16_t currentSamples) -> uint8_t
    {
        if (currentSamples >= dischargeTable[0][0]) 
            return 100;

        if (currentSamples <= dischargeTable[rows - 1][0]) 
            return 0;

        for (uint8_t i = 0; i < rows - 1; ++i) 
        {
            if (currentSamples >= dischargeTable[i + 1][0] && currentSamples <= dischargeTable[i][0]) 
            {
                uint16_t voltageDiff = dischargeTable[i][0] - dischargeTable[i + 1][0];
                uint16_t percentageDiff = dischargeTable[i][1] - dischargeTable[i + 1][1];
                uint16_t voltageOffset = currentSamples - dischargeTable[i + 1][0];
                return dischargeTable[i + 1][1] + (percentageDiff * voltageOffset + voltageDiff / 2) / voltageDiff;
            }
        }

        return 0;
    };

    static uint32_t lastChargeShow = 0;
    static int16_t averageSamples = 0;

    int sample = analogRead(BATTERY_VOLTAGE_PIN);
    if (sample < 0)
        sample = averageSamples;

    if ((millis() - lastChargeShow) > 10000 || forceShow)
    {
        char buf[4];
        buf[3] = 0;
        int16_t percents = getBatteryPercentage(averageSamples);

        uint8_t il = ilen(percents) < 3 ? 2 : 3;
        convertToChar(buf, percents, il);

        if (il < 3)
            buf[2] = '%';

        if (!g_settingsActive && !g_sMeterOn && !g_displayRDS)
            oledPrint(buf, 102, 6, DEFAULT_FONT);
        lastChargeShow = millis();
        averageSamples = sample;
    }

    averageSamples = (averageSamples + sample) / 2;
}

#if USE_RDS
void showRDS()
{
    static uint16_t lastUpdatedFreq = 0;
    static uint32_t lastUpdatedTime = millis();
    static bool succeed = false;

    if (g_currentMode != FM || !g_displayRDS || g_settingsActive)
    { 
        lastUpdatedFreq = 0;
        g_rdsPrevLen = 0;
        succeed = false;
        g_rdsActiveInfo = 0;
        return;
    }

    if (millis() - lastUpdatedTime > 300)
        succeed = false;

    if (lastUpdatedFreq != g_currentFrequency || g_rdsSwitchPressed)
    {
        if (g_rdsSwitchPressed)
        {
            g_rdsActiveInfo++;
            if (g_rdsActiveInfo > RDSActiveInfo::ProgramInfo)
                g_rdsActiveInfo = RDSActiveInfo::StationName;
        }
        else
        {
            g_rdsActiveInfo = RDSActiveInfo::StationName;
            succeed = false;
        }
        g_rdsPrevLen = 0;
        oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
    }
    lastUpdatedFreq = g_currentFrequency;

    if (!succeed)
        g_si4735.getRdsStatus();

    if (!succeed && g_si4735.getRdsReceived() && g_si4735.getRdsSync() && g_si4735.getNumRdsFifoUsed() > 1)
    {
        g_RDSCells[RDSActiveInfo::StationName] = g_si4735.getRdsStationName();
        g_RDSCells[RDSActiveInfo::StationInfo] = g_si4735.getRdsStationInformation();
        g_RDSCells[RDSActiveInfo::ProgramInfo] = g_si4735.getRdsProgramInformation();
        g_RDSCells[RDSActiveInfo::StationInfo][17] = '\0';
        g_RDSCells[RDSActiveInfo::ProgramInfo][17] = '\0';
        succeed = true;
        lastUpdatedTime = millis();
    }
    else if (!g_rdsSwitchPressed && succeed)
        return;

    uint8_t len = strlen8(g_RDSCells[g_rdsActiveInfo]);

    if (len == 0 && !g_rdsSwitchPressed)
        return;

    oledPrint(g_RDSCells[g_rdsActiveInfo], 0, 6, DEFAULT_FONT);
    
    uint8_t toPrint = len == 0 ? 3 : (len < g_rdsPrevLen ? min(g_rdsPrevLen - len, 16 - len) : 0);
    char printChar = len == 0 ? '.' : ' ';
    for (uint8_t i = 0; i < toPrint; i++) 
        oled.print(printChar);

    g_rdsPrevLen = len;
    g_rdsSwitchPressed = false;
}
#endif

//Draw steps (with units)
void showStep()
{
    if (g_sMeterOn || g_displayRDS)
        return;

    char buf[5];
    if (g_currentMode == FM)
    {
        if (g_tabStepFM[g_FMStepIndex] == 100)
        {
            buf[0] = ' ';
            buf[1] = ' ';
            buf[2] = '1';
            buf[3] = 'M';
            buf[4] = 0x0;
        }
        else
        {
            convertToChar(buf, g_tabStepFM[g_FMStepIndex] * 10, 3);
            buf[3] = 'k';
            buf[4] = '\0';
        }
    }
    else
    {
        if (g_tabStep[g_stepIndex] == 1000)
        {
            buf[0] = ' ';
            buf[1] = ' ';
            buf[2] = '1';
            buf[3] = 'M';
            buf[4] = 0x0;
        }
        else if (isSSB() && g_stepIndex >= g_amTotalSteps)
            convertToChar(buf, g_tabStep[g_stepIndex], 4);
        else
        {
            convertToChar(buf, g_tabStep[g_stepIndex], 3);
            buf[3] = 'k';
            buf[4] = '\0';
        }
    }

    uint8_t off = 50;
    oledPrint("St:", off - 16, 6, DEFAULT_FONT, g_cmdStep);
    oledPrint(buf, off + 8, 6, DEFAULT_FONT, g_cmdStep);
}

void showSMeter()
{
    static uint32_t sMeterUpdated = 0;
    if (millis() - sMeterUpdated < 100)
        return;

    g_si4735.getCurrentReceivedSignalQuality();
    uint8_t rssi = g_si4735.getCurrentRSSI();
    rssi = rssi > 64 ? 64 : rssi;

    int sMeterValue = rssi / (64 / 16);
    char buf[17];
    for (uint8_t i = 0; i < sizeof(buf) - 1; i++)
        buf[i] = i < sMeterValue ? '|' : ' ';
    buf[sizeof(buf) - 1] = 0x0;

    oledPrint(buf, 0, 6, DEFAULT_FONT);
    sMeterUpdated = millis();
}

//Draw bandwidth (Ignored for CW mode)
void showBandwidth()
{
    char* bw;
    if (isSSB())
    {
        bw = (char*)g_bandwidthSSB[g_bwIndexSSB].desc;
        if (g_currentMode == CW)
            bw = "    ";
    }
    else if (g_currentMode == AM)
    {
        bw = (char*)g_bandwidthAM[g_bwIndexAM].desc;
    }
    else
    {
        bw = (char*)g_bandwidthFM[g_bwIndexFM];
    }

    oledPrint(bw, 45, 0, DEFAULT_FONT, g_cmdBw);
}

uint16_t getNextSWSuBband(bool up)
{
    uint16_t freq = g_currentFrequency;
    if (isSSB())
        freq += g_currentBFO / 1000;

    for (uint8_t i = 0; i < g_SWSubBandCount; i++)
    {
        uint8_t n = g_SWSubBandCount - 1 - i;
        if (!up && SWSubBands[n] < freq)
            return SWSubBands[n];
        else if (up && SWSubBands[i] > freq)
            return SWSubBands[i];
    }

    return 0;
}

void bandSwitch(bool up)
{
    uint16_t nextSW = getNextSWSuBband(up);
    
    if (g_bandIndex == SW_BAND_TYPE && nextSW != 0)
    {
        g_currentFrequency = nextSW;

        g_currentBFO = 0;
        if (isSSB())
            updateBFO();
        g_si4735.setFrequency(nextSW);
        agcSetFunc(); //Re-apply to remove noize
        showFrequency();
        showBandTag();
    }
    else
    {
        if (g_currentMode == FM)
            g_bandList[g_bandIndex].currentStepIdx = g_FMStepIndex;
        else
            g_bandList[g_bandIndex].currentStepIdx = g_stepIndex;

        if (up)
        {
            if (g_bandIndex < g_lastBand)
                g_bandIndex++;
            else
                g_bandIndex = 0;
        }
        else
        {
            if (g_bandIndex > 0)
                g_bandIndex--;
            else
                g_bandIndex = g_lastBand;
        }

        if (g_sMeterOn)
        {
            g_sMeterOn = false;
            oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
        }

#if USE_RDS
        if (g_displayRDS && g_currentMode != FM)
        {
            g_displayRDS = false;
            oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
        }
#endif

        g_currentBFO = 0;
        if (isSSB())
            updateBFO();
        applyBandConfiguration();
    }
}

// This function is required for using SSB. Si473x controllers do not support SSB by-default.
// But we can patch internal RAM of Si473x with special patch to make it work in SSB mode.
// Patch must be applied every time we enable SSB after AM or FM.
void loadSSBPatch()
{
    // This works, but i am not sure it's safe
    //g_si4735.setI2CFastModeCustom(700000);
    g_si4735.setI2CFastModeCustom(500000);
    g_si4735.queryLibraryId(); //Do we really need this? Research it.
    g_si4735.patchPowerUp();
    delay(50);
    g_si4735.downloadCompressedPatch(ssb_patch_content, sizeof(ssb_patch_content), cmd_0x15, sizeof(cmd_0x15));
    g_si4735.setSSBConfig(g_bandwidthSSB[g_bwIndexSSB].idx, 1, 0, 1, 0, 1);
    g_si4735.setI2CStandardMode();
    g_ssbLoaded = true;
    g_stepIndex = 0;
}

#if USE_RDS
void setRDSConfig(uint8_t bias)
{
    g_si4735.setRdsConfig(1, bias, bias, bias, bias);
}
#endif

//Update receiver settings after changing band and modulation
void applyBandConfiguration(bool extraSSBReset = false)
{
    g_si4735.setTuneFrequencyAntennaCapacitor(uint16_t(g_bandIndex == SW_BAND_TYPE));
    if (g_bandIndex == FM_BAND_TYPE)
    {
        g_currentMode = FM;
        g_si4735.setFM(g_bandList[g_bandIndex].minimumFreq,
            g_bandList[g_bandIndex].maximumFreq,
            g_bandList[g_bandIndex].currentFreq,
            g_tabStepFM[g_bandList[g_bandIndex].currentStepIdx]);
        g_si4735.setSeekFmLimits(g_bandList[g_bandIndex].minimumFreq, g_bandList[g_bandIndex].maximumFreq);
        g_si4735.setSeekFmSpacing(1);
        g_ssbLoaded = false;
#if USE_RDS
        setRDSConfig(g_Settings[SettingsIndex::RDSError].param);
#endif
        g_si4735.setFifoCount(1);
        g_bwIndexFM = g_bandList[g_bandIndex].bandwidthIdx;
        g_si4735.setFmBandwidth(g_bwIndexFM);
        g_si4735.setFMDeEmphasis(g_Settings[SettingsIndex::DeEmp].param == 0 ? 1 : 2);
    }
    else
    {
        uint16_t minFreq = g_bandList[g_bandIndex].minimumFreq;
        uint16_t maxFreq = g_bandList[g_bandIndex].maximumFreq;
        if (g_bandIndex == SW_BAND_TYPE)
        {
            minFreq = SW_LIMIT_LOW;
            maxFreq = SW_LIMIT_HIGH;
        }

        if (g_ssbLoaded)
        {
            g_currentBFO = 0;
            if (extraSSBReset)
                loadSSBPatch();

            //Call this before to call crazy volume after AM when SVC is off
            g_si4735.setSSBAutomaticVolumeControl(g_Settings[SettingsIndex::SVC].param);
            g_si4735.setSSB(minFreq,
                maxFreq,
                g_bandList[g_bandIndex].currentFreq,
                g_bandList[g_bandIndex].currentStepIdx >= g_amTotalSteps ? 0 : g_tabStep[g_bandList[g_bandIndex].currentStepIdx],
                g_currentMode == CW ? g_Settings[SettingsIndex::CWSwitch].param + 1 : g_currentMode);
            updateSSBCutoffFilter();
            g_si4735.setSSBAutomaticVolumeControl(g_Settings[SettingsIndex::SVC].param);
            g_si4735.setSSBDspAfc(g_Settings[SettingsIndex::Sync].param == 1 ? 0 : 1);
            g_si4735.setSSBAvcDivider(g_Settings[SettingsIndex::Sync].param == 0 ? 0 : 3); //Set Sync mode
            g_si4735.setAmSoftMuteMaxAttenuation(g_Settings[SettingsIndex::SoftMute].param);
            g_si4735.setSSBAudioBandwidth(g_currentMode == CW ? g_bandwidthSSB[0].idx : g_bandwidthSSB[g_bwIndexSSB].idx);
            updateBFO();
            g_si4735.setSSBSoftMute(g_Settings[SettingsIndex::SSM].param);
        }
        else
        {
            g_currentMode = AM;
            g_si4735.setAM(minFreq,
                maxFreq,
                g_bandList[g_bandIndex].currentFreq,
                g_bandList[g_bandIndex].currentStepIdx >= g_amTotalSteps ? 0 : g_tabStep[g_bandList[g_bandIndex].currentStepIdx]);
            g_si4735.setAmSoftMuteMaxAttenuation(g_Settings[SettingsIndex::SoftMute].param);
            g_bwIndexAM = g_bandList[g_bandIndex].bandwidthIdx;
            g_si4735.setBandwidth(g_bandwidthAM[g_bwIndexAM].idx, 1);
        }

        agcSetFunc();
        g_si4735.setAvcAmMaxGain(g_Settings[SettingsIndex::AutoVolControl].param);
        g_si4735.setSeekAmLimits(minFreq, maxFreq);
        g_si4735.setSeekAmSpacing((g_bandList[g_bandIndex].currentStepIdx >= g_amTotalSteps) ? 1 : g_tabStep[g_bandList[g_bandIndex].currentStepIdx]);
    }

    g_currentFrequency = g_bandList[g_bandIndex].currentFreq;
    if (g_currentMode == FM)
        g_FMStepIndex = g_bandList[g_bandIndex].currentStepIdx;
    else
        g_stepIndex = g_bandList[g_bandIndex].currentStepIdx;

    if ((g_bandIndex == LW_BAND_TYPE || g_bandIndex == MW_BAND_TYPE)
        && g_stepIndex > g_amTotalStepsSSB)
        g_stepIndex = g_amTotalStepsSSB;

    if (!g_settingsActive)
        showStatus(true);
    resetEepromDelay();
}

//Step value regulation
void doStep(int8_t v)
{
    if (g_currentMode == FM)
    {
        g_FMStepIndex = (v == 1) ? g_FMStepIndex + 1 : g_FMStepIndex - 1;
        if (g_FMStepIndex > g_lastStepFM)
            g_FMStepIndex = 0;
        else if (g_FMStepIndex < 0)
            g_FMStepIndex = g_lastStepFM;

        g_si4735.setFrequencyStep(g_tabStepFM[g_FMStepIndex]);
        g_bandList[g_bandIndex].currentStepIdx = g_FMStepIndex;
        g_si4735.setSeekFmSpacing(1);
        showStep();
    }
    else
    {
        g_stepIndex = (v == 1) ? g_stepIndex + 1 : g_stepIndex - 1;
        if (g_stepIndex > getLastStep())
            g_stepIndex = 0;
        else if (g_stepIndex < 0)
            g_stepIndex = getLastStep();

        //SSB Step limit
        else if (isSSB() && g_stepIndex >= g_amTotalStepsSSB && g_stepIndex < g_amTotalSteps)
            g_stepIndex = v == 1 ? g_amTotalSteps : g_amTotalStepsSSB - 1;
        
        //LW/MW Step limit
        else if ((g_bandIndex == LW_BAND_TYPE || g_bandIndex == MW_BAND_TYPE)
            && v == 1 && g_stepIndex > g_amTotalStepsSSB && g_stepIndex < g_amTotalSteps)
            g_stepIndex = g_amTotalSteps;
        else if ((g_bandIndex == LW_BAND_TYPE || g_bandIndex == MW_BAND_TYPE)
            && v != 1 && g_stepIndex > g_amTotalStepsSSB && g_stepIndex < g_amTotalSteps)
            g_stepIndex = g_amTotalStepsSSB;

        if (!isSSB() || isSSB() && g_stepIndex < g_amTotalSteps)
        {
            g_si4735.setFrequencyStep(g_tabStep[g_stepIndex]);
            g_bandList[g_bandIndex].currentStepIdx = g_stepIndex;
        }

        if (!isSSB())
            g_si4735.setSeekAmSpacing((g_bandList[g_bandIndex].currentStepIdx >= g_amTotalSteps) ? 1 : g_tabStep[g_bandList[g_bandIndex].currentStepIdx]);
        showStep();
    }
}

void updateBFO()
{
    //Actually to move frequency forward you need to move BFO backwards, so just * -1
    g_si4735.setSSBBfo((g_currentBFO + (g_Settings[SettingsIndex::BFO].param * 10)) * -1);
}

//Volume control
void doVolume(int8_t v)
{
    if (g_muteVolume)
    {
        g_si4735.setVolume(g_muteVolume);
        g_muteVolume = 0;
    }
    else
    {
        if (v == 1)
            g_si4735.volumeUp();
        else
            g_si4735.volumeDown();
    }
    showVolume();
}

//Helps to save more flash image size
void doSwitchLogic(int8_t& param, int8_t low, int8_t high, int8_t step)
{
    param += step;
    if (param < low)
        param = high;
    else if (param > high)
        param = low;
}

void agcSetFunc()
{
    uint8_t att = g_Settings[SettingsIndex::ATT].param;
    uint8_t disableAgc = att > 0;
    uint8_t agcNdx;
    if (att > 1) 
        agcNdx = att - 1;
    else
        agcNdx = 0;
    g_si4735.setAutomaticGainControl(disableAgc, agcNdx);
}

//Settings: Attenuation
void doAttenuation(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::ATT].param, 0, 37, v);
    agcSetFunc();
}

//Settings: Soft Mute
void doSoftMute(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::SoftMute].param, 0, 32, v);

    if (g_currentMode != FM)
        g_si4735.setAmSoftMuteMaxAttenuation(g_Settings[SettingsIndex::SoftMute].param);
}

//Settings: Brightness
void doBrightness(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::Brightness].param, 5, 125, v);
    oled.setContrast(uint8_t(g_Settings[SettingsIndex::Brightness].param) * 2);
}

//Settings: SSB AVC Switch
void doSSBAVC(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::SVC].param, 0, 1, v);

    if (isSSB())
    {
        g_si4735.setSSBAutomaticVolumeControl(g_Settings[SettingsIndex::SVC].param);
        applyBandConfiguration(true);
    }
}

//Settings: Automatic Volume Control
void doAvc(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::AutoVolControl].param, 12, 90, v);

    if (g_currentMode != FM)
        g_si4735.setAvcAmMaxGain(g_Settings[SettingsIndex::AutoVolControl].param);
}

//Settings: Sync switch
void doSync(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::Sync].param, 0, 1, v);

    if (isSSB())
    {
        g_si4735.setSSBDspAfc(g_Settings[SettingsIndex::Sync].param == 1 ? 0 : 1);
        g_si4735.setSSBAvcDivider(g_Settings[SettingsIndex::Sync].param == 0 ? 0 : 3); //Set Sync mode
        applyBandConfiguration(true);
    }
}

//Settings: FM DeEmp switch (50 or 75)
void doDeEmp(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::DeEmp].param, 0, 1, v);

    if (g_currentMode == FM)
        g_si4735.setFMDeEmphasis(g_Settings[SettingsIndex::DeEmp].param == 0 ? 1 : 2);
}

//Settings: SW Units
void doSWUnits(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::SWUnits].param, 0, 1, v);
}

//Settings: SW Units
void doSSBSoftMuteMode(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::SSM].param, 0, 1, v);

    if (isSSB())
        g_si4735.setSSBSoftMute(g_Settings[SettingsIndex::SSM].param);
}

//Settings: SSB Cutoff filter
void doCutoffFilter(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::CutoffFilter].param, 0, 2, v);

    if (isSSB())
        updateSSBCutoffFilter();
}

//Settings: CPU Frequency divider
void doCPUSpeed(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::CPUSpeed].param, 0, 1, v);

    noInterrupts();
    CLKPR = 0x80;
    CLKPR = g_Settings[SettingsIndex::CPUSpeed].param;
    interrupts();
}

//Settings: BFO Offset calibration
void doBFOCalibration(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::BFO].param, -60, 60, v);

    if (isSSB())
    {
#if USE_RDS
        setRDSConfig(g_Settings[SettingsIndex::BFO].param);
#endif
        updateBFO();
    }
}

//Settings: Tune Frequency Antenna Capacitor
void doUnitsSwitch(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::UnitsSwitch].param, 0, 1, v);
}

//Settings: Scan button switch
void doScanSwitch(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::ScanSwitch].param, 0, 1, v);
}

//Settings: CW mode switch
void doCWSwitch(int8_t v = 0)
{
    doSwitchLogic(g_Settings[SettingsIndex::CWSwitch].param, 0, 1, v);

    if (g_currentMode == CW)
        applyBandConfiguration(true);
}

#if USE_RDS
//Settings: RDS Error Level
void doRDSErrorLevel(int8_t v)
{
    doSwitchLogic(g_Settings[SettingsIndex::RDSError].param, 0, 3, v);

    if (g_currentMode == FM)
        setRDSConfig(g_Settings[SettingsIndex::RDSError].param);
}


void doRDS()
{
    g_displayRDS = !g_displayRDS;

    if (g_displayRDS)
    {
        g_sMeterOn = false;
        oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
        g_si4735.getRdsStatus();
        showRDS();
    }
    else
        updateLowerDisplayLine();
}
#endif

//Prevents repeatable code for flash image size saving
void doBandwidthLogic(int8_t& bwIndex, uint8_t upperLimit, uint8_t v)
{
    doSwitchLogic(bwIndex, 0, upperLimit, v);
    g_bandList[g_bandIndex].bandwidthIdx = bwIndex;
}

//Bandwidth regulation logic
void doBandwidth(uint8_t v)
{
    if (isSSB())
    {
        doSwitchLogic(g_bwIndexSSB, 0, g_bwSSBMaxIdx, v);
        g_si4735.setSSBAudioBandwidth(g_bandwidthSSB[g_bwIndexSSB].idx);
        updateSSBCutoffFilter();
    }
    else if (g_currentMode == AM)
    {
        doBandwidthLogic(g_bwIndexAM, g_maxFilterAM, v);
        g_bandList[g_bandIndex].bandwidthIdx = g_bwIndexAM;
        g_si4735.setBandwidth(g_bandwidthAM[g_bwIndexAM].idx, 1);
    }
    else
    {
        doBandwidthLogic(g_bwIndexFM, 4, v);
        g_bandList[g_bandIndex].bandwidthIdx = g_bwIndexFM;
        g_si4735.setFmBandwidth(g_bwIndexFM);
    }
    showBandwidth();
}

void switchCommand(bool* b, void (*showFunction)())
{
    static bool* prev = NULL;
    static void (*prevFunc)() = NULL;

    if (!b)
    {
        if (prev)
        {
            *prev = false;
            if (prevFunc)
                prevFunc();
            g_lastAdjustmentTime = 0;
            prev = NULL;
        }
        return;
    }

    bool last = *b;
    prev = b;
    prevFunc = showFunction;

    if (*b == false)
    {
        g_cmdVolume = false;
        g_cmdStep = false;
        g_cmdBw = false;
        g_cmdBand = false;
        g_lastAdjustmentTime = millis();
        showVolume();
        showStep();
        showBandwidth();
        showModulation();
    }
    else
        g_lastAdjustmentTime = 0;

    *b = !last;

    if (showFunction)
        showFunction();
}

bool clampSSBBand()
{
    uint16_t freq = g_currentFrequency + (g_currentBFO / 1000);
    auto bfoReset = [&]()
    {
        g_currentBFO = 0;
        updateBFO();
        showFrequency(true);
        showModulation();
    };

    bool upd = false;
    if (freq > g_bandList[g_bandIndex].maximumFreq)
    {
        g_currentFrequency = g_bandList[g_bandIndex].minimumFreq;
        upd = true;
    }
    else if (freq < g_bandList[g_bandIndex].minimumFreq)
    {
        g_currentFrequency = g_bandList[g_bandIndex].maximumFreq;
        upd = true;
    }

    if (upd)
    {
        g_bandList[g_bandIndex].currentFreq = g_currentFrequency;
        g_si4735.setFrequency(g_currentFrequency);
        bfoReset();
        return true;
    }

    return false;
}

void doFrequencyTune()
{
    g_seekDirection = g_encoderCount == 1 ? 1 : 0;

    //Update frequency
    g_previousFrequency = g_currentFrequency; //Force EEPROM update
    if (g_currentMode == FM)
    {
        g_currentFrequency += g_tabStepFM[g_FMStepIndex] * g_encoderCount; //g_si4735.getFrequency() is too slow
#if USE_RDS
        if (g_displayRDS)
            oledPrint(_literal_EmptyLine, 0, 6, DEFAULT_FONT);
#endif
    }
    else
        g_currentFrequency += g_tabStep[g_stepIndex] * g_encoderCount;
    uint16_t bMin = g_bandList[g_bandIndex].minimumFreq, bMax = g_bandList[g_bandIndex].maximumFreq;

    //Special logic for fast and responsive frequency surfing
    if (g_currentFrequency > bMax)
        g_currentFrequency = bMin;
    else if (g_currentFrequency < bMin)
        g_currentFrequency = bMax;

    g_bandList[g_bandIndex].currentFreq = g_currentFrequency;
    g_processFreqChange = true;
    g_lastFreqChange = millis();

    showFrequency();
}

void resetLowerLine()
{
    if (g_sMeterOn || g_displayRDS)
    {
        g_sMeterOn = false;
        g_displayRDS = false;
        updateLowerDisplayLine();
    }
}

//Special feature to make SSB feel like on expensive TECSUN receivers
//BFO is now part of main frequency in SSB mode
void doFrequencyTuneSSB()
{
    const int BFOMax = 16000;
    int step = g_encoderCount == 1 ? getSteps() : getSteps() * -1;
    int newBFO = g_currentBFO + step;
    int redundant = 0;

    if (newBFO > BFOMax)
    {
        redundant = (newBFO / BFOMax) * BFOMax;
        g_currentFrequency += redundant / 1000;
        newBFO -= redundant;
    }
    else if (newBFO < -BFOMax)
    {
        redundant = ((abs(newBFO) / BFOMax) * BFOMax);
        g_currentFrequency -= redundant / 1000;
        newBFO += redundant;
    }

    g_currentBFO = newBFO;
    updateBFO();

    if (redundant != 0)
    {
        g_si4735.setFrequency(g_currentFrequency);
        agcSetFunc(); //Re-apply to remove noize
        g_currentFrequency = g_si4735.getFrequency();
        g_bandList[g_bandIndex].currentFreq = g_currentFrequency;
    }

    g_bandList[g_bandIndex].currentFreq = g_currentFrequency + (g_currentBFO / 1000);
    g_lastFreqChange = millis();
    g_previousFrequency = 0; //Force EEPROM update
    if (!clampSSBBand()) //If we move outside of current band - switch it
        showFrequency();
}

void loop()
{
    uint8_t x;
    bool skipButtonEvents = false;
    bool frequencyRecentlyUpdated = millis() - g_lastFreqChange < 70;

    //Faster frequency tune
    if (g_processFreqChange && !isSSB())
    {
        if (!frequencyRecentlyUpdated && g_encoderCount == 0)
        {
            g_si4735.setFrequency(g_currentFrequency);
            g_processFreqChange = false;
        }
        else if (frequencyRecentlyUpdated && g_encoderCount != 0)
        {
            doFrequencyTune();
            g_encoderCount = 0;
            return;
        }
    }
    
    if (millis() - g_lastFreqChange >= 1000)
    {
#if USE_RDS
        showRDS();
#endif

        if (g_sMeterOn && !g_settingsActive)
            showSMeter();

        showCharge(false);
    }

    if (g_lastAdjustmentTime != 0 && millis() - g_lastAdjustmentTime > ADJUSTMENT_ACTIVE_TIMEOUT)
        switchCommand(NULL, NULL);

    //Encoder rotation check
    if (g_encoderCount != 0)
    {
        if (g_lastAdjustmentTime != 0)
            g_lastAdjustmentTime = millis();

        if (g_settingsActive)
        {
            if (!g_SettingEditing)
            {
                int8_t prev = g_SettingSelected;
                int8_t next = g_SettingSelected;

                next += g_encoderCount;

                uint8_t pageIdx = g_SettingsPage - 1;
                uint8_t maxOnThisPage = (pageIdx * 6) + 5;
                if (maxOnThisPage >= SettingsIndex::SETTINGS_MAX)
                    maxOnThisPage = SettingsIndex::SETTINGS_MAX - 1;

                if (next < pageIdx * 6)
                    g_SettingSelected = maxOnThisPage;
                else if (next > maxOnThisPage)
                    g_SettingSelected = pageIdx * 6;
                else
                    g_SettingSelected = next;

                DrawSetting(prev, true);
                DrawSetting(g_SettingSelected, true);
            }
            else
            {
                (*g_Settings[g_SettingSelected].manipulateCallback)(g_encoderCount);
                DrawSetting(g_SettingSelected, false);
                delay(MIN_ELAPSED_TIME);
            }
        }
        else if (g_cmdVolume)
            doVolume(g_encoderCount);
        else if (g_cmdStep)
            doStep(g_encoderCount);
        else if (g_cmdBw)
            doBandwidth(g_encoderCount);
        else if (g_cmdBand)
        {
            if (g_encoderCount == 1)
                bandSwitch(true);
            else
                bandSwitch(false);
        }
        else if (isSSB())
        {
            doFrequencyTuneSSB();
            skipButtonEvents = true;
        }
        else
        {
            doFrequencyTune();
            skipButtonEvents = true;
        }
        g_encoderCount = 0;
        resetEepromDelay();
    }

    if (skipButtonEvents)
        goto saveAttempt;

    //Command-checkers
    if (BUTTONEVENT_SHORTPRESS == btn_Bandwidth.checkEvent(simpleEvent))
    {
        if (!g_settingsActive && g_currentMode != CW)
            switchCommand(&g_cmdBw, showBandwidth);
    }
    if (BUTTONEVENT_SHORTPRESS == btn_BandUp.checkEvent(bandEvent))
    {
        if (!g_settingsActive)
        {
            resetLowerLine();
            switchCommand(&g_cmdBand, showModulation);
        }
        else
        {
            switchSettingsPage();
        }
    }
    if (BUTTONEVENT_SHORTPRESS == btn_BandDn.checkEvent(bandEvent))
    {
        if (!g_settingsActive)
            switchCommand(NULL, NULL);
        g_settingsActive = !g_settingsActive;
        switchSettings();
    }
    if (BUTTONEVENT_SHORTPRESS == btn_VolumeUp.checkEvent(volumeEvent))
    {
        if (!g_settingsActive && g_muteVolume == 0)
            switchCommand(&g_cmdVolume, showVolume);
    }
    if (BUTTONEVENT_SHORTPRESS == btn_VolumeDn.checkEvent(volumeEvent))
    {
        if (!g_cmdVolume)
        {
            uint8_t vol = g_si4735.getCurrentVolume();
            if (vol > 0 && g_muteVolume == 0)
            {
                g_muteVolume = vol;
                g_si4735.setVolume(0);
            }
            else if (g_muteVolume > 0)
            {
                g_si4735.setVolume(g_muteVolume);
                g_muteVolume = 0;
            }
            showVolume();
        }
    }
    if (BUTTONEVENT_SHORTPRESS == btn_Encoder.checkEvent(simpleEvent))
    {
        if (g_cmdBand)
        {
            g_cmdBand = false;
            showModulation();
        }
        else if (g_cmdStep)
        {
            g_cmdStep = false;
            showStep();
        }
        else if (g_cmdBw)
        {
            g_cmdBw = false;
            showBandwidth();
        }
        else if (g_cmdVolume)
        {
            g_cmdVolume = false;
            showVolume();
        }
        else if (g_settingsActive)
        {
            g_SettingEditing = !g_SettingEditing;
            DrawSetting(g_SettingSelected, true);
        }
        else if (g_displayRDS)
            g_rdsSwitchPressed = true;
        else if (isSSB() || g_Settings[SettingsIndex::ScanSwitch].param == 0)
        {
            if (!g_settingsActive)
            {
                switchCommand(&g_cmdStep, showStep);
                resetLowerLine();
            }
        }
        //Seek in SSB/CW is not allowed
        else if (g_currentMode == FM || g_currentMode == AM)
            doSeek();
    }

    //This is a hack, it allows SHORTPRESS and LONGPRESS events
    //Be processed without complicated overhead
    //It requires to save checkEvent result into a variable
    //That has exact same name as event processing function for this button
    uint8_t agcEvent = btn_AGC.checkEvent(agcEvent);
    if (BUTTONEVENT_SHORTPRESS == agcEvent)
    {
        if (!g_settingsActive || g_settingsActive && !g_displayOn)
        {
            g_displayOn = !g_displayOn;
            if (g_displayOn)
                oled.on();
            else
                oled.off();
        }
    }
    if (BUTTONEVENT_LONGPRESS == agcEvent)
    {
        if (!g_settingsActive)
        {
            if (isSSB())
                doSync(1);
        }
    }
    uint8_t stepEvent = btn_Step.checkEvent(stepEvent);
    if (BUTTONEVENT_SHORTPRESS == stepEvent)
    {
        if (!g_settingsActive)
        {
            switchCommand(&g_cmdStep, showStep);
            resetLowerLine();
        }
    }
    if (BUTTONEVENT_LONGPRESSDONE == stepEvent)
    {
        if (!g_settingsActive)
        {
            g_sMeterOn = !g_sMeterOn;
            if (g_sMeterOn)
            {
                g_displayRDS = false;
                showSMeter();
            }
            else
                updateLowerDisplayLine();
        }
    }
    if (BUTTONEVENT_SHORTPRESS == btn_Mode.checkEvent(simpleEvent))
    {
        if (!g_settingsActive)
        {
            //Do nothing on FM mode (unfortunately no NBFM patch), otherwise switch AM modulation
            if (g_currentMode != FM)
            {
                g_bandList[g_bandIndex].currentFreq = g_currentFrequency;
                g_prevMode = g_currentMode;
                switch (g_currentMode)
                {
                case AM:
                    //Patch Si473x memory every time when enabling SSB
                    loadSSBPatch();
                    g_processFreqChange = false;
                    //Allow pass through

                case LSB:
                    g_currentMode++;
                    g_bandList[g_bandIndex].currentFreq += g_currentBFO / 1000;
                    break;

                case USB:
                    g_currentMode++;
                    g_cmdBw = false;
                    g_bandList[g_bandIndex].currentFreq += g_currentBFO / 1000;
                    break;

                case CW:
                    g_currentMode = AM;
                    g_ssbLoaded = false;
                    if (g_stepIndex >= g_amTotalSteps)
                        g_stepIndex = 0;

                    g_currentFrequency += (g_currentBFO / 1000);
                    break;
                }

                g_bandList[g_bandIndex].currentStepIdx = g_stepIndex;
                applyBandConfiguration();
            }
#if USE_RDS
            else
                doRDS();
#endif
        }
    }

saveAttempt:
    //Save EEPROM if anough time passed and frequency changed
    if (g_currentFrequency != g_previousFrequency)
    {
        if ((millis() - g_storeTime) > STORE_TIME)
        {
            saveAllReceiverInformation();
            g_storeTime = millis();
            g_previousFrequency = g_currentFrequency;
        }
    }
}

//Overriding original main to save some space
int main(void)
{
    init();
    setup();
    while(1)
        loop();
    return 0;
}