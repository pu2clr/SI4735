/*
 * SI4735 ARDUINO LIBRARY  
 * References: 
 *      Si47XX PROGRAMMING GUIDE AN332
 *      AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE
 * 
 * See documentation on https://github.com/pu2clr/SI4735
 *   
 * Ricardo Lima Caratti, Nov 2019  
 */

#include <Arduino.h>
#include <Wire.h>

#define SI473X_ADDR 0x11        // SI473X I2C buss address
#define POWER_UP 0x01           // Power up device and mode selection.
#define GET_REV 0x10            // Returns revision information on the device.
#define POWER_DOWN 0x11         // Power down device.
#define SET_PROPERTY 0x12       // Sets the value of a property.
#define GET_PROPERTY 0x13       // Retrieves a property’s value.
#define GET_INT_STATUS 0x14     // Read interrupt status bits.

#define FM_TUNE_FREQ  0x20
#define FM_SEEK_START 0x21      // Begins searching for a valid FM frequency.
#define FM_TUNE_STATUS 0x22

#define AM_TUNE_FREQ 0x40       // Tunes to a given AM frequency.
#define AM_SEEK_START 0x41      // Begins searching for a valid AM frequency.
#define AM_TUNE_STATUS 0x42     // Queries the status of the already issued AM_TUNE_FREQ or AM_SEEK_START command.
#define AM_RSQ_STATUS 0x43      // Queries the status of the Received Signal Quality (RSQ) for the current channel.
#define AM_AGC_STATUS 0x47      // Queries the current AGC settings.
#define AM_AGC_OVERRIDE 0x48    // Overrides AGC settings by disabling and forcing it to a fixed value.
#define GPIO_CTL 0x80           // Configures GPO1, 2, and 3 as output or Hi-Z.
#define GPIO_SET 0x81           // Sets GPO1, 2, and 3 output level (low or high).

#define TX_TUNE_FREQ 0x30   // CMD
#define SET_PROPERTY 0x12   // CMD
#define RX_VOLUME    0x4000

#define SI473X_ANALOG_AUDIO B00000101 // Analog Audio Inputs
#define SI473X_DIGITAL_AUDIO B00001011 // Digital audio output (DCLK, LOUT/DFS, ROUT/DIO)

/*****************************************************************
 * SI473X data types 
 * These data types will be usefull to deal with SI473X 
 *****************************************************************/

/*
 * Power Up arguments data type 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 65
 */ 
typedef union {
    struct
    {
        // ARG1
        byte FUNC       : 4;  // Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
        byte XOSCEN     : 1;  // Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
        byte PATCH      : 1;  // Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
        byte GPO2OEN    : 1;  // GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
        byte CTSIEN     : 1;  // CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).
        // ARG2
        byte OPMODE; // Application Setting. See page 65
    } arg;
    byte raw[2]; // same arg memory position, so same content.
} si473x_powerup;

/*
 * Represents how the  frequency is stored in the si4735.
 * It helps to convert frequency in unsigned int to two bytes (FREQL and FREQH)  
 */
typedef union {
    struct
    {
        byte FREQL; // Tune Frequency High Byte.
        byte FREQH; // Tune Frequency Low Byte.
    } raw;
    unsigned value;
} si47x_frequency;

/* 
 *  Represents searching for a valid frequency data type.
 */
typedef union {
    struct
    {
        byte RESERVED1 : 2;
        byte WRAP : 1;   // Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
        byte SEEKUP : 1; // Determines the direction of the search, either UP = 1, or DOWN = 0.
        byte RESERVED2 : 4;
    } arg;
    byte raw;
} si47x_seek;

/*
 * Status of FM_TUNE_FREQ or FM_SEEK_START commands or 
 * Status of AM_TUNE_FREQ or AM_SEEK_START commands.
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 and 139
 */
typedef union {
    struct
    {
        byte INTACK : 1; // If set, clears the seek/tune complete interrupt status indicator.
        byte CANCEL : 1; // If set, aborts a seek currently in progress.
        byte RESERVED2 : 6;
    } arg;
    byte raw;
} si47x_tune_status;

volatile static bool data_from_si4735;

static void interrupt_hundler()
{
    data_from_si4735 = true;
};

class SI4735
{

        private:
        byte  resetPin;
        byte  interruptPin;

        byte currentTune;

        si47x_frequency currentFrequency;
        si473x_powerup  powerUp;

        byte volume = 32;

        void reset(void);
        void waitInterrupr(void);
        void waitToSend(void);


    public :
        void setup(byte resetPin, byte interruptPin, byte defaultFunction);
        void setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE);
        void analogPowerUp(void);
        
        void setBand(byte new_band);
        unsigned getFrequency(void);
        void setFrequency(unsigned);
        void setVolume(byte volume);
        void volumeDown();
        void volumeUp();
        void setAM();
        void setFM();


        void getFirmware(void);


        void setFunction(byte FUNC);
        void seekStation(byte SEEKUP, byte WRAP);
};
