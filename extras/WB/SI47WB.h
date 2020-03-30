/*
 * The SI47WB class extends the Si4735 class and includes Weather Band features supported by some Si47XX Devices. 
 * 
 * Si4736, Si4737, Si4738, Si4739, Si4742 and Si4743
 * 
 * This code is being developed in partnership with Dave. 
 *  
 */

#include <SI4735.h>

// WB Commands
#define WB_TUNE_FREQ 0x50    // Selects the WB tuning frequency.
#define WB_TUNE_STATUS 0x52  // Queries the status of previous WB_TUNE_FREQ or WB_SEEK_START command.
#define WB_RSQ_STATUS 0x53   // Queries the status of the Received Signal Quality (RSQ) of the current channel
#define WB_AGC_STATUS 0x57   // Queries the current AGC settings
#define WB_AGC_OVERRIDE 0x58 // Override AGC setting by disabling and forcing it to a fixed value
#define WB_SAME_STATUS 0x55  // Queries the status of the 1050 kHz alert tone in Weather Band.

#define WB_CURRENT_MODE 3

class SI47WB : public SI4735
{

public:
    void setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE);
    void setTuneFrequencyAntennaCapacitor(uint16_t capacitor);
    void setWB();
    void setWB(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step);
    void setFrequency(uint16_t freq)
};