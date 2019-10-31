/*
 * SI4735 ARDUINO LIBRARY  
 * References: 
 *      Si47XX PROGRAMMING GUIDE AN332
 *      AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE
 *  
 * Ricardo Lima Caratti, Oct 2019  
 *  
 * Code Under construction
 * 
 */

#define POWER_UP 0x01           // Power up device and mode selection.
#define GET_REV 0x10            // Returns revision information on the device.
#define POWER_DOWN 0x11         // Power down device.
#define SET_PROPERTY 0x12       // Sets the value of a property.
#define GET_PROPERTY 0x13       // Retrieves a property’s value.
#define GET_INT_STATUS 0x14     // Read interrupt status bits.

#define AM_TUNE_FREQ 0x40       // Tunes to a given AM frequency.
#define AM_SEEK_START 0x41      // Begins searching for a valid frequency.
#define AM_TUNE_STATUS 0x42     // Queries the status of the already issued AM_TUNE_FREQ or AM_SEEK_START command.
#define AM_RSQ_STATUS 0x43      // Queries the status of the Received Signal Quality (RSQ) for the current channel.
#define AM_AGC_STATUS 0x47      // Queries the current AGC settings.
#define AM_AGC_OVERRIDE 0x48    // Overrides AGC settings by disabling and forcing it to a fixed value.
#define GPIO_CTL 0x80           // Configures GPO1, 2, and 3 as output or Hi-Z.
#define GPIO_SET 0x81           // Sets GPO1, 2, and 3 output level (low or high).



#define TX_TUNE_FREQ 0x30   // CMD
#define SET_PROPERTY 0x12   // CMD




// Table 4. FM / RDS Transmitter Command Summary


class SI4735
{

    private:

    public:

    void getFrequency(void);
    void setFrequency(unsifned);




}
