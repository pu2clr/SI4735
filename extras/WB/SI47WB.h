

/*
 * 
 */ 

// WB Commands
#define WB_TUNE_FREQ 0x50    // Selects the WB tuning frequency.
#define WB_TUNE_STATUS 0x52  // Queries the status of previous WB_TUNE_FREQ or WB_SEEK_START command.
#define WB_RSQ_STATUS 0x53   // Queries the status of the Received Signal Quality (RSQ) of the current channel
#define WB_AGC_STATUS 0x57   // Queries the current AGC settings
#define WB_AGC_OVERRIDE 0x58 // Override AGC setting by disabling and forcing it to a fixed value
#define WB_SAME_STATUS 0x55  // Queries the status of the 1050 kHz alert tone in Weather Band.

#define WB_CURRENT_MODE 3

class SI47WB : public SI47WB
{

private:
    char rds_buffer2A[65]; // RDS Radio Text buffer - Program Information
    char rds_buffer2B[33]; // RDS Radio Text buffer - Station Informaation
    char rds_buffer0A[9];  // RDS Basic tuning and switching information (Type 0 groups)
    char rds_time[20];

    int rdsTextAdress2A;
    int rdsTextAdress2B;
    int rdsTextAdress0A;

    int16_t deviceAddress = SI473X_ADDR_SEN_LOW;

    uint8_t lastTextFlagAB;
    uint8_t resetPin;
    uint8_t interruptPin;

    uint8_t currentTune;

    uint16_t currentMinimumFrequency;
    uint16_t currentMaximumFrequency;
    uint16_t currentWorkFrequency;

    uint16_t currentStep;

    uint8_t lastMode = -1; // Store the last mode used.

    uint8_t currentAvcAmMaxGain = 48; // Automatic Volume Control Gain for AM - Default 48

    si47x_frequency currentFrequency;
    si47x_set_frequency currentFrequencyParams;
    si47x_rqs_status currentRqsStatus;
    si47x_response_status currentStatus;
    si47x_firmware_information firmwareInfo;
    si47x_rds_status currentRdsStatus;
    si47x_agc_status currentAgcStatus;
    si47x_ssb_mode currentSSBMode;

    si473x_powerup powerUp;

    uint8_t volume = 32;

    uint8_t currentSsbStatus;

    void waitInterrupr(void);
    void sendProperty(uint16_t propertyValue, uint16_t param);
    void sendSSBModeProperty(); // Sends SSB_MODE property to the device.
    void disableFmDebug();
    void clearRdsBuffer2A();
    void clearRdsBuffer2B();
    void clearRdsBuffer0A();

public:
    void setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE);
    void setTuneFrequencyAntennaCapacitor(uint16_t capacitor);
    void SI47WB::setWB();
    void SI47WB::setWB(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step);
};