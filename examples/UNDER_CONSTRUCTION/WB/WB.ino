#include <SI4735.h>

// WB
#define WB_TUNE_FREQ 0x50
#define WB_TUNE_STATUS 0x52
#define WB_RSQ_STATUS 0x53
#define WB_SAME_STATUS 0x54
#define WB_ASQ_STATUS 0x55
#define WB_AGC_STATUS 0x57
#define WB_AGC_OVERRIDE 0x58

// WB properties
#define WB_MAX_TUNE_ERROR 0x5108
#define WB_RSQ_INT_SOURCE 0x5200
#define WB_RSQ_SNR_HI_THRESHOLD 0x5201
#define WB_RSQ_SNR_LO_THRESHOLD 0x5202
#define WB_RSQ_RSSI_HI_THRESHOLD 0x5203
#define WB_RSQ_RSSI_LO_THRESHOLD 0x5204
#define WB_VALID_SNR_THRESHOLD 0x5403
#define WB_VALID_RSSI_THRESHOLD 0x5404
#define WB_SAME_INTERRUPT_SOURCE 0x5500
#define WB_ASQ_INT_SOURCE 0x5600

#define WB_CURRENT_MODE 3

class WeatherBand : public SI4735 { // extending the original class SI4735
  public:

    void setWB(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step)
    {
        currentMinimumFrequency = fromFreq;
        currentMaximumFrequency = toFreq;
        currentStep = step;

        if (initialFreq < fromFreq || initialFreq > toFreq)
            initialFreq = fromFreq;

        setWB();

        currentWorkFrequency = initialFreq;
        setFrequency(currentWorkFrequency);
    }

    void setWB()
    {
        powerDown();
        setPowerUp(this->ctsIntEnable, this->gpo2Enable, 0, this->currentClockType, WB_CURRENT_MODE, this->currentAudioMode);
        radioPowerUp();
        setVolume(volume); // Set to previus configured volume
        currentSsbStatus = 0;
        disableFmDebug();
        lastMode = WB_CURRENT_MODE;
    }

    void getStatus(uint8_t INTACK, uint8_t CANCEL)
    {
        si47x_tune_status status;
        uint8_t cmd = FM_TUNE_STATUS;
        int limitResp = 8;

        if (currentTune == FM_TUNE_FREQ)
            cmd = FM_TUNE_STATUS;
        else if (currentTune == AM_TUNE_FREQ)
            cmd = AM_TUNE_STATUS;
        else if (currentTune == WB_TUNE_FREQ)
            cmd = WB_TUNE_STATUS;	
        else if (currentTune == NBFM_TUNE_FREQ)
        {
            cmd = NBFM_TUNE_STATUS;
            limitResp = 6;
        }

        waitToSend();

        status.arg.INTACK = INTACK;
        status.arg.CANCEL = CANCEL;
        status.arg.RESERVED2 = 0;

        Wire.beginTransmission(deviceAddress);
        Wire.write(cmd);
        Wire.write(status.raw);
        Wire.endTransmission();
        // Reads the current status (including current frequency).
        do
        {
            waitToSend();
            Wire.requestFrom(deviceAddress, limitResp); // Check it
            // Gets response information
            for (uint8_t i = 0; i < limitResp; i++)
                currentStatus.raw[i] = Wire.read();
        } while (currentStatus.resp.ERR); // If error, try it again
        waitToSend();
    }

};

WeatherBand wb; 

void setup()
{

}

void loop() {
  // Your code with your custom SI4735 library.
}

