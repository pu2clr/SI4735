/**
 * @brief SI4735 ARDUINO LIBRARY  
 * 
 * @details This is an Arduino library for the SI473X and SI474X, BROADCAST AM/FM/SW RADIO RECEIVER, IC from Silicon Labs for the 
 * @details Arduino development environment 
 * @details The communication used by this library is I2C.
 * @details This file contains: const (#define), Defined Data type and Methods declarations
 * @details You can see a complete documentation on <https://github.com/pu2clr/SI4735>
 * @details The are more than 30 examples on <https://github.com/pu2clr/SI4735/tree/master/examples>
 *   
 * @see [General Documentation](https://pu2clr.github.io/SI4735/)
 * @see [Schematics](https://pu2clr.github.io/SI4735/extras/schematic/)
 * @see Si47XX PROGRAMMING GUIDE AN332 (Rev 1.0): https://www.silabs.com/documents/public/application-notes/AN332.pdf
 * @see AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES
 * 
 * @author PU2CLR - Ricardo Lima Caratti 
 * @date  2019-2022
 */

#ifndef _SI4735_H // Prevent this file from being compiled more than once
#define _SI4735_H

#include <Arduino.h>
#include <Wire.h>

#define POWER_UP_FM 0  // FM
#define POWER_UP_AM 1  // AM and SSB (if patch applyed)
#define POWER_UP_WB 3  // Weather Band Receiver
#define POWER_PATCH 15 //

// SI473X commands (general)
#define SI473X_ADDR_SEN_LOW 0x11  // SI473X I2C bus address when the SEN pin (16) is set to low 0V.
#define SI473X_ADDR_SEN_HIGH 0x63 // SI473X I2C bus address when the SEN pin (16) is set to high +3.3V

#define POWER_UP 0x01       // Power up device and mode selection.
#define GET_REV 0x10        // Returns revision information on the device.
#define POWER_DOWN 0x11     // Power down device.
#define SET_PROPERTY 0x12   // Sets the value of a property.
#define GET_PROPERTY 0x13   // Retrieves a property’s value.
#define GET_INT_STATUS 0x14 // Read interrupt status bits.

// FM
#define FM_TUNE_FREQ 0x20
#define FM_SEEK_START 0x21 // Begins searching for a valid FM frequency.
#define FM_TUNE_STATUS 0x22
#define FM_AGC_STATUS 0x27
#define FM_AGC_OVERRIDE 0x28
#define FM_RSQ_STATUS 0x23
#define FM_RDS_STATUS 0x24 // Returns RDS information for current channel and reads an entry from the RDS FIFO.

#define FM_NB_DETECT_THRESHOLD 0x1900 // Sets the threshold for detecting impulses in dB above the noise floor. Default value is 16.
#define FM_NB_INTERVAL 0x1901         // Interval in micro-seconds that original samples are replaced by interpolated clean sam- ples. Default value is 24 μs.
#define FM_NB_RATE 0x1902             // Noise blanking rate in 100 Hz units. Default value is 64.
#define FM_NB_IIR_FILTER 0x1903       // Sets the bandwidth of the noise floor estimator Default value is 300.
#define FM_NB_DELAY 0x1904            // Delay in micro-seconds before applying impulse blanking to the original sam- ples. Default value is 133.

// FM RDS properties
#define FM_RDS_INT_SOURCE 0x1500
#define FM_RDS_INT_FIFO_COUNT 0x1501
#define FM_RDS_CONFIG 0x1502
#define FM_RDS_CONFIDENCE 0x1503

#define FM_DEEMPHASIS 0x1100
#define FM_BLEND_STEREO_THRESHOLD 0x1105
#define FM_BLEND_MONO_THRESHOLD 0x1106
#define FM_BLEND_RSSI_STEREO_THRESHOLD 0x1800
#define FM_BLEND_RSSI_MONO_THRESHOLD 0x1801
#define FM_BLEND_SNR_STEREO_THRESHOLD 0x1804
#define FM_BLEND_SNR_MONO_THRESHOLD 0x1805
#define FM_BLEND_MULTIPATH_STEREO_THRESHOLD 0x1808
#define FM_BLEND_MULTIPATH_MONO_THRESHOLD 0x1809
#define FM_CHANNEL_FILTER 0x1102
#define FM_SOFT_MUTE_MAX_ATTENUATION 0x1302

// FM SEEK Properties
#define FM_SEEK_BAND_BOTTOM 0x1400         // Sets the bottom of the FM band for seek
#define FM_SEEK_BAND_TOP 0x1401            // Sets the top of the FM band for seek
#define FM_SEEK_FREQ_SPACING 0x1402        // Selects frequency spacing for FM seek
#define FM_SEEK_TUNE_SNR_THRESHOLD 0x1403  // Sets the SNR threshold for a valid FM Seek/Tune
#define FM_SEEK_TUNE_RSSI_THRESHOLD 0x1404 // Sets the RSSI threshold for a valid FM Seek/Tune


// NBFM Commands
#define NBFM_TUNE_FREQ 0x50
#define NBFM_TUNE_STATUS 0x52
#define NBFM_RSQ_STATUS 0x53
#define NBFM_AGC_STATUS 0x57
#define NBFM_AGC_OVERRIDE 0x58


// NBFM Properties

#define NBFM_MAX_TUNE_ERROR 0x5108
#define NBFM_RSQ_INT_SOURCE 0x5200
#define NBFM_RSQ_SNR_HI_THRESHOLD 0x5201
#define NBFM_RSQ_SNR_LO_THRESHOLD 0x5202
#define NBFM_RSQ_RSSI_HI_THRESHOLD 0x5203
#define NBFM_RSQ_RSSI_LO_THRESHOLD 0x5204
#define NBFM_VALID_SNR_THRESHOLD 0x5403
#define NBFM_VALID_RSSI_THRESHOLD 0x5404


// AM command
#define AM_TUNE_FREQ 0x40    // Tunes to a given AM frequency.
#define AM_SEEK_START 0x41   // Begins searching for a valid AM frequency.
#define AM_TUNE_STATUS 0x42  // Queries the status of the already issued AM_TUNE_FREQ or AM_SEEK_START command.
#define AM_RSQ_STATUS 0x43   // Queries the status of the Received Signal Quality (RSQ) for the current channel.
#define AM_AGC_STATUS 0x47   // Queries the current AGC settings.
#define AM_AGC_OVERRIDE 0x48 // Overrides AGC settings by disabling and forcing it to a fixed value.
#define GPIO_CTL 0x80        // Configures GPO1, 2, and 3 as output or Hi-Z.
#define GPIO_SET 0x81        // Sets GPO1, 2, and 3 output level (low or high).

//SSB command (SAME AM CMD VALUES)
// See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 4 and 5
#define SSB_TUNE_FREQ 0x40    // Tunes to a given SSB frequency.
#define SSB_TUNE_STATUS 0x42  // Queries the status of the already issued SSB_TUNE_FREQ or AM_SEEK_START command.
#define SSB_RSQ_STATUS 0x43   // Queries the status of the Received Signal Quality (RSQ) for the current channel.
#define SSB_AGC_STATUS 0x47   // Queries the current AGC settings.
#define SSB_AGC_OVERRIDE 0x48 // Overrides AGC settings by disabling and forcing it to a fixed value.

// AM/SW/LW Receiver Property Summary
// See  Si47XX PROGRAMMING GUIDE AN332 (REV 1.0); page 125
#define DIGITAL_OUTPUT_FORMAT 0x0102                // Configure digital audio outputs.            
#define DIGITAL_OUTPUT_SAMPLE_RATE 0x0104           // Configure digital audio output sample rate
#define REFCLK_FREQ 0x0201                          //Sets frequency of reference clock in Hz. The range is 31130 to 34406 Hz, or 0 to disable the AFC. Default is 32768 Hz.
#define REFCLK_PRESCALE 0x0202                      // Sets the prescaler value for RCLK input.
#define AM_DEEMPHASIS 0x3100                        // Sets deemphasis time constant. Can be set to 50 μs. Deemphasis is disabled by default.
#define AM_CHANNEL_FILTER 0x3102                    // Selects the bandwidth of the channel filter for AM reception. The choices are 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). The default bandwidth is 2 kHz.
#define AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN 0x3103 // Sets the maximum gain for automatic volume control.
#define AM_MODE_AFC_SW_PULL_IN_RANGE 0x3104         // Sets the SW AFC pull-in range.
#define AM_MODE_AFC_SW_LOCK_IN_RANGE 0x3105         // Sets the SW AFC lock-in.
#define AM_RSQ_INTERRUPTS 0x3200                    // Same SSB - Configures interrupt related to Received Signal Quality metrics. All interrupts are disabled by default.
#define AM_RSQ_SNR_HIGH_THRESHOLD 0x3201            //Sets high threshold for SNR interrupt.
#define AM_RSQ_SNR_LOW_THRESHOLD 0x3202             // Sets low threshold for SNR interrupt.
#define AM_RSQ_RSSI_HIGH_THRESHOLD 0x3203           // Sets high threshold for RSSI interrupt.
#define AM_RSQ_RSSI_LOW_THRESHOLD 0x3204            // Sets low threshold for RSSI interrupt.
#define AM_SOFT_MUTE_RATE 0x3300                    // Sets the attack and decay rates when entering or leaving soft mute. The default is 278 dB/s.
#define AM_SOFT_MUTE_SLOPE 0x3301                   // Sets the AM soft mute slope. Default value is a slope of 1.
#define AM_SOFT_MUTE_MAX_ATTENUATION 0x3302         // Sets maximum attenuation during soft mute (dB). Set to 0 to disable soft mute. Default is 8 dB.
#define AM_SOFT_MUTE_SNR_THRESHOLD 0x3303           // Sets SNR threshold to engage soft mute. Default is 8 dB.
#define AM_SOFT_MUTE_RELEASE_RATE 0x3304            // Sets softmute release rate. Smaller values provide slower release, and larger values provide faster release.
#define AM_SOFT_MUTE_ATTACK_RATE 0x3305             // Sets software attack rate. Smaller values provide slower attack, and larger values provide faster attack.
#define AM_SEEK_BAND_BOTTOM 0x3400                  // Sets the bottom of the AM band for seek. Default is 520.
#define AM_SEEK_BAND_TOP 0x3401                     // Sets the top of the AM band for seek. Default is 1710.
#define AM_SEEK_FREQ_SPACING 0x3402                 // Selects frequency spacing for AM seek. Default is 10 kHz spacing.
#define AM_SEEK_SNR_THRESHOLD 0x3403                // Sets the SNR threshold for a valid AM Seek/Tune.
#define AM_SEEK_RSSI_THRESHOLD 0x3404               // Sets the RSSI threshold for a valid AM Seek/Tune.
#define AM_AGC_ATTACK_RATE 0x3702                   // Sets the number of milliseconds the high peak detector must be exceeded before decreasing gain.
#define AM_AGC_RELEASE_RATE 0x3703                  // Sets the number of milliseconds the low peak detector must not be exceeded before increasing the gain.
#define AM_FRONTEND_AGC_CONTROL 0x3705              // Adjusts AM AGC for frontend (external) attenuator and LNA.
#define AM_NB_DETECT_THRESHOLD 0x3900               // Sets the threshold for detecting impulses in dB above the noise floor
#define AM_NB_INTERVAL 0x3901                       // Interval in micro-seconds that original samples are replaced by interpolated clean samples
#define AM_NB_RATE 0x3902                           // Noise blanking rate in 100 Hz units. Default value is 64.
#define AM_NB_IIR_FILTER 0x3903                     // Sets the bandwidth of the noise floor estimator. Default value is 300.
#define AM_NB_DELAY 0x3904                          // Delay in micro-seconds before applying impulse blanking to the original samples

#define RX_VOLUME 0x4000
#define RX_HARD_MUTE 0x4001

// SSB properties
// See AN332 REV 0.8 Universal Programming Guide (Amendment for SI4735-D60 SSN and NBFM Patches)

#define GPO_IEN 0x0001                       // AM and SSB - Enable interrupt source
#define SSB_BFO 0x0100                       // Sets the Beat Frequency Offset (BFO) under SSB mode.
#define SSB_MODE 0x0101                      // Sets number of properties of the SSB mode.
#define SSB_RSQ_INTERRUPTS 0x3200            // Configure Interrupts related to RSQ
#define SSB_RSQ_SNR_HI_THRESHOLD 0x3201      // Sets high threshold for SNR interrupt
#define SSB_RSQ_SNR_LO_THRESHOLD 0x3202      // Sets low threshold for SNR interrupt
#define SSB_RSQ_RSSI_HI_THRESHOLD 0x3203     // Sets high threshold for RSSI interrupt
#define SSB_RSQ_RSSI_LO_THRESHOLD 0x3204     // Sets low threshold for RSSI interrupt
#define SSB_SOFT_MUTE_RATE 0x3300            // Sets the attack and decay rates when entering or leaving soft mute
#define SSB_SOFT_MUTE_MAX_ATTENUATION 0x3302 // Sets the maximum attenuation during soft mute (db); 0dB to disable soft mute; defaul 8dB;
#define SSB_SOFT_MUTE_SNR_THRESHOLD 0x3303   // Sets SNR threshould to engage soft mute. Defaul 8dB
#define SSB_RF_AGC_ATTACK_RATE 0x3700        // Sets the number of milliseconds the high RF peak detector must be exceeded before decreasing the gain. Defaul 4.
#define SSB_RF_AGC_RELEASE_RATE 0x3701       // Sets the number of milliseconds the low RF peak detector must be exceeded before increasing the gain. Defaul 24.
#define SSB_IF_AGC_RELEASE_RATE 0x3703       // Sets the number of milliseconds the low IF peak detector must not be exceeded before increasing the gain. Default value is 140 (approximately 40 dB / s).
#define SSB_IF_AGC_ATTACK_RATE 0x3702        // Sets the number of milliseconds the high IF peak detector must be exceeded before decreasing gain. Default value is 4 (approximately 1400 dB / s).

// SSB
#define SSB_RF_IF_AGC_ATTACK_RATE 0x3702  // Sets the number of milliseconds the high IF peak detector must be exceeded before decreasing gain. Defaul 4.
#define SSB_RF_IF_AGC_RELEASE_RATE 0x3703 // Sets the number of milliseconds the low IF peak detector must be exceeded before increasing the gain. Defaul 140.

// See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 12 and 13
#define LSB_MODE 1 // 01
#define USB_MODE 2 // 10

// Parameters
#define SI473X_RDS_OUTPUT_ONLY 0b00000000      // RDS output only (no audio outputs) Si4749 only
#define SI473X_ANALOG_AUDIO 0b00000101         // Analog Audio Inputs
#define SI473X_DIGITAL_AUDIO1 0b00001011       // Digital audio output (DCLK, LOUT/DFS, ROUT/DIO)
#define SI473X_DIGITAL_AUDIO2 0b10110000       // Digital audio outputs (DCLK, DFS, DIO)
#define SI473X_ANALOG_DIGITAL_AUDIO 0b10110101 // Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS,DIO)

// Other parameters
#define FM_CURRENT_MODE 0
#define AM_CURRENT_MODE 1
#define SSB_CURRENT_MODE 2
#define NBFM_CURRENT_MODE 3

#define SEEK_UP 1
#define SEEK_DOWN 0

#define MAX_DELAY_AFTER_SET_FREQUENCY 30 // In ms - This value helps to improve the precision during of getting frequency value
#define MAX_DELAY_AFTER_POWERUP 10       // In ms - Max delay you have to setup after a power up command.
#define MIN_DELAY_WAIT_SEND_LOOP 300     // In uS (Microsecond) - each loop of waitToSend sould wait this value in microsecond
#define MAX_SEEK_TIME 8000               // defines the maximum seeking time 8s is default.

#define DEFAULT_CURRENT_AVC_AM_MAX_GAIN 36 

#define XOSCEN_CRYSTAL 1 // Use crystal oscillator
#define XOSCEN_RCLK 0    // Use external RCLK (crystal oscillator disabled).

/** @defgroup group01 Union, Struct and Defined Data Types 
 * @section group01 Data Types 
 *  
 * @brief SI473X data representation
 * 
 * @details The goal of this approach is separating data from code. 
 * The Si47XX family works with many internal data that can be represented by data structure 
 * or defined data type in C/C++. These C/C++ resources have been used widely here.  
 * This approach made the library easier to build and maintain.  Each data structure created 
 * here has its reference (name of the document and page on which it was based). 
 * In other words, to make the SI47XX device easier to deal, some defined data types were 
 * created to handle byte and bits to process  commands, properties and responses.
 * These data types will be usefull to deal with SI473X 
 */

/**
 * @ingroup group01
 * 
 * @brief Power Up arguments data type 
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 64 and 65
 */
    typedef union
{
    struct
    {
        // ARG1
        uint8_t FUNC : 4;    //!<  Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
        uint8_t XOSCEN : 1;  //!<  Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
        uint8_t PATCH : 1;   //!<  Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
        uint8_t GPO2OEN : 1; //!<  GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
        uint8_t CTSIEN : 1;  //!<  CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).
        // ARG2
        uint8_t OPMODE; //!<  Application Setting. See page 65
    } arg;              //!<  Refined powerup parameters
    uint8_t raw[2];     //!<  Raw powerup parameters data. Same arg memory position. So, same content.
} si473x_powerup;

/**
 * @ingroup group01
 * 
 * @brief Data type for Enables output for GPO1, GPO2 and GPO3
 * 
 * @details GPO1, 2, and 3 can be configured for output (Hi-Z or active drive) by setting the GPO1OEN, GPO2OEN, and GPO3OEN bit. 
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 82 and 144
 */
typedef union
{
    struct
    {
        uint8_t DUMMY1 : 1;  //!< Always write 0.
        uint8_t GPO1OEN : 1; //!< GPO1 Output Enable.
        uint8_t GPO2OEN : 1; //!< GPO2 Output Enable.
        uint8_t GPO3OEN : 1; //!< GPO3 Output Enable.
        uint8_t DUMMY2 : 4;  //!< Always write 0.
    } arg;                   //!<  Refined powerup parameters
    uint8_t raw;
} si473x_gpio;

/**
 * @ingroup group01
 * 
 * @brief Data type for Configuring the sources for the GPO2/INT interrupt pin
 * 
 * @details Valid sources are the lower 8 bits of the STATUS byte, including CTS, ERR, RSQINT, and STCINT bits.
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 146
 */
typedef union
{
    struct
    {
        uint8_t STCIEN : 1; //!< Seek/Tune Complete Interrupt Enable (0 or 1).
        uint8_t DUMMY1 : 2; //!< Always write 0.
        uint8_t RSQIEN : 1; //!< RSQ Interrupt Enable (0 or 1).
        uint8_t DUMMY2 : 2; //!< Always write 0.
        uint8_t ERRIEN : 1; //!< ERR Interrupt Enable (0 or 1).
        uint8_t CTSIEN : 1; //!< CTS Interrupt Enable (0 or 1).
        uint8_t STCREP : 1; //!< STC Interrupt Repeat (0 or 1).
        uint8_t DUMMY3 : 2; //!< Always write 0.
        uint8_t RSQREP : 1; //!< RSQ Interrupt Repeat (0 or 1).
        uint8_t DUMMY4 : 4; //!< Always write 0.
    } arg;
    uint16_t raw;
} si473x_gpio_ien;

/**
 * @ingroup group01
 * 
 * @brief Represents how the  frequency is stored in the si4735.
 * @details It helps to convert frequency in uint16_t to two bytes (uint8_t) (FREQL and FREQH)  
 */
typedef union
{
    struct
    {
        uint8_t FREQL; //!<  Tune Frequency Low byte.
        uint8_t FREQH; //!<  Tune Frequency High byte.
    } raw;             //!<  Raw data that represents the frequency stored in the Si47XX device.
    uint16_t value;    //!<  frequency (integer value)
} si47x_frequency;

/**
 * @ingroup group01
 * @brief Antenna Tuning Capacitor data type manupulation 
 */
typedef union
{
    struct
    {
        uint8_t ANTCAPL; //!<  Antenna Tuning Capacitor High byte
        uint8_t ANTCAPH; //!<  Antenna Tuning Capacitor Low byte
    } raw;
    uint16_t value;
} si47x_antenna_capacitor;

/**
 * @ingroup group01
 * 
 * @brief AM Tune frequency data type command (AM_TUNE_FREQ command)
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 135
 */
typedef union
{
    struct
    {
        uint8_t FAST : 1;   //!<  ARG1 - FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.
        uint8_t FREEZE : 1; //!<  Valid only for FM (Must be 0 to AM)
        uint8_t DUMMY1 : 4; //!<  Always set 0
        uint8_t USBLSB : 2; //!<  SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection. 10 = USB is selected; 01 = LSB is selected.
        uint8_t FREQH;      //!<  ARG2 - Tune Frequency High byte.
        uint8_t FREQL;      //!<  ARG3 - Tune Frequency Low byte.
        uint8_t ANTCAPH;    //!<  ARG4 - Antenna Tuning Capacitor High byte.
        uint8_t ANTCAPL;    //!<  ARG5 - Antenna Tuning Capacitor Low byte. Note used for FM.
    } arg;
    uint8_t raw[5];
} si47x_set_frequency;

/** 
 * @ingroup group01 
 * 
 * @brief Seek frequency (automatic tuning). ARG1
 * 
 * @details Represents searching for a valid frequency data type AM and FM.
 * @details When AM, the searching data have to be complemented by si47x_seek_am_complement.
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 72 and 137
 * @see si47x_seek_am_complement
 */
typedef union
{
    struct
    {
        uint8_t RESERVED1 : 2;
        uint8_t WRAP : 1;   //!<  Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
        uint8_t SEEKUP : 1; //!<  Determines the direction of the search, either UP = 1, or DOWN = 0.
        uint8_t RESERVED2 : 4;
    } arg;
    uint8_t raw;
} si47x_seek;

/** 
 * @ingroup group01 
 * 
 * @brief Seek frequency (automatic tuning) AM complement (ARG2, ARG3, ARG4 and ARG5)
 * 
 * @details Represents AM complement searching information for a valid frequency data type.
 * 
 * @see  @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 72 and 137
 */
typedef struct
{
    uint8_t ARG2; // Always 0.
    uint8_t ARG3; // Always 0.
    uint8_t ANTCAPH;
    uint8_t ANTCAPL;
} si47x_seek_am_complement;

/** 
 * @ingroup group01 status response structure
 * 
 * @brief Status response data representation
 * 
 * @details Represents searching for a valid frequency data type.
 */
typedef union
{
    struct
    {
        uint8_t STCINT : 1; //!< 1 = Tune complete has been triggered.
        uint8_t DUMMY1 : 1; //!< Reserved (Values may vary).
        uint8_t RDSINT : 1; //!< 1 = Radio data system interrupt has been triggered.
        uint8_t RSQINT : 1; //!< 1 = Received Signal Quality measurement has been triggered.
        uint8_t DUMMY2 : 2; //!< Reserved (Values may vary).
        uint8_t ERR : 1;    //!< 1 = Error.
        uint8_t CTS : 1;    //!< 0 = Wait before sending next command; 1 = Clear to send next command.
    } refined;
    uint8_t raw;
} si47x_status;

/**  
 * @ingroup group01
 * 
 * @brief Response status command 
 * 
 * @details Response data from a query status command
 * 
 * @see Si47XX PROGRAMMING GUIDE; pages 73 and 
 */
typedef union
{
    struct
    {
        // Status
        uint8_t STCINT : 1; //!<  Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1; //!<  Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
        uint8_t RSQINT : 1; //!<  Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1; //!<  Error. 0 = No error 1 = Error
        uint8_t CTS : 1; //!<  Clear to Send.
        // RESP1
        uint8_t VALID : 1; //!<  Valid Channel
        uint8_t AFCRL : 1; //!<  AFC Rail Indicator
        uint8_t DUMMY3 : 5;
        uint8_t BLTF : 1; //!<  Reports if a seek hit the band limit
        // RESP2
        uint8_t READFREQH; //!<  Read Frequency High byte.
        // RESP3
        uint8_t READFREQL; //!<  Read Frequency Low byte.
        // RESP4
        uint8_t RSSI; //!<  Received Signal Strength Indicator (dBμV)
        // RESP5
        uint8_t SNR; //!<  This byte contains the SNR metric when tune is complete (dB).
        // RESP6
        uint8_t MULT; //!<  If FM, contains the multipath metric when tune is complete; IF AM READANTCAPH (tuning capacitor value high byte)
        // RESP7
        uint8_t READANTCAP; //!<  If FM, contains the current antenna tuning capacitor value; IF AM READANTCAPL (tuning capacitor value low byte)
    } resp;
    uint8_t raw[8]; //!<  Check it
} si47x_response_status;

/**
 * @ingroup group01
 * 
 * @brief Data representation for  Firmware Information (GET_REV)
 * 
 * @details The part number, chip revision, firmware revision, patch revision and component revision numbers. 
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 66 and 131 
 */
typedef union
{
    struct
    {
        // status ("RESP0")
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1;
        uint8_t CTS : 1;
        uint8_t PN;       //!<  RESP1 - Final 2 digits of Part Number (HEX).
        uint8_t FWMAJOR;  //!<  RESP2 - Firmware Major Revision (ASCII).
        uint8_t FWMINOR;  //!<  RESP3 - Firmware Minor Revision (ASCII).
        uint8_t PATCHH;   //!<  RESP4 - Patch ID High byte (HEX).
        uint8_t PATCHL;   //!<  RESP5 - Patch ID Low byte (HEX).
        uint8_t CMPMAJOR; //!<  RESP6 - Component Major Revision (ASCII).
        uint8_t CMPMINOR; //!<  RESP7 - Component Minor Revision (ASCII).
        uint8_t CHIPREV;  //!<  RESP8 - Chip Revision (ASCII).
        // RESP9 to RESP15 not used
    } resp;
    uint8_t raw[9];
} si47x_firmware_information;

/**
 * @ingroup group01
 * 
 * @brief Firmware Query Library ID response. 
 * 
 * @details Used to represent the response of a power up command with FUNC = 15 (patch)
 * 
 * To confirm that the patch is compatible with the internal device library revision, the library 
 * revision should be confirmed by issuing the POWER_UP command with Function = 15 (query library ID)
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 12 
 */
typedef union
{
    struct
    {
        // status ("RESP0")
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1;
        uint8_t CTS : 1;
        uint8_t PN;        //!<  RESP1 - Final 2 digits of Part Number (HEX).
        uint8_t FWMAJOR;   //!<  RESP2 - Firmware Major Revision (ASCII).
        uint8_t FWMINOR;   //!<  RESP3 - Firmware Minor Revision (ASCII).
        uint8_t RESERVED1; //!<  RESP4 - Reserved, various values.
        uint8_t RESERVED2; //!<  RESP5 - Reserved, various values.
        uint8_t CHIPREV;   //!<  RESP6 - Chip Revision (ASCII).
        uint8_t LIBRARYID; //!<  RESP7 - Library Revision (HEX).
        // RESP9 to RESP15 not used
    } resp;
    uint8_t raw[8];
} si47x_firmware_query_library;

/**
 * @ingroup group01
 * 
 * @brief Seek station status
 * 
 * @details Status of FM_TUNE_FREQ or FM_SEEK_START commands or Status of AM_TUNE_FREQ or AM_SEEK_START commands.
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 73 and 139
 */
typedef union
{
    struct
    {
        uint8_t INTACK : 1; //!<  If set, clears the seek/tune complete interrupt status indicator.
        uint8_t CANCEL : 1; //!<  If set, aborts a seek currently in progress.
        uint8_t RESERVED2 : 6;
    } arg;
    uint8_t raw;
} si47x_tune_status;

/**
 * @ingroup group01
 * 
 * @brief Data type to deal with SET_PROPERTY command
 * 
 * @details Property Data type (help to deal with SET_PROPERTY command on si473X)
 */
typedef union
{
    struct
    {
        uint8_t byteLow;
        uint8_t byteHigh;
    } raw;
    uint16_t value;
} si47x_property;

/** 
 * @ingroup group01
 * 
 * @brief  Radio Signal Quality data representation 
 * 
 * @details Data type for status information about the received signal quality (FM_RSQ_STATUS and AM_RSQ_STATUS)
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 75 and 
 */
typedef union
{
    struct
    {
        // status ("RESP0")
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1;
        uint8_t CTS : 1;
        // RESP1
        uint8_t RSSIILINT : 1; //!<  RSSI Detect Low.
        uint8_t RSSIHINT : 1;  //!<  RSSI Detect High.
        uint8_t SNRLINT : 1;   //!<  SNR Detect Low.
        uint8_t SNRHINT : 1;   //!<  SNR Detect High.
        uint8_t MULTLINT : 1;  //!<  Multipath Detect Low
        uint8_t MULTHINT : 1;  //!<  Multipath Detect High
        uint8_t DUMMY3 : 1;
        uint8_t BLENDINT : 1; //!<  Blend Detect Interrupt.
        // RESP2
        uint8_t VALID : 1; //!<  Valid Channel.
        uint8_t AFCRL : 1; //!<  AFC Rail Indicator.
        uint8_t DUMMY4 : 1;
        uint8_t SMUTE : 1; //!<  Soft Mute Indicator. Indicates soft mute is engaged.
        uint8_t DUMMY5 : 4;
        // RESP3
        uint8_t STBLEND : 7; //!<  Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).
        uint8_t PILOT : 1;   //!<  Indicates stereo pilot presence.
        // RESP4 to RESP7
        uint8_t RSSI;    //!<  RESP4 - Contains the current receive signal strength (0–127 dBμV).
        uint8_t SNR;     //!<  RESP5 - Contains the current SNR metric (0–127 dB).
        uint8_t MULT;    //!<  RESP6 - Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
        uint8_t FREQOFF; //!<  RESP7 - Signed frequency offset (kHz).
    } resp;
    uint8_t raw[8];
} si47x_rqs_status;

/**
 * @ingroup group01
 * @brief Adjusts the AM AGC for external front-end attenuator and external front-end cascode LNA.
 * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.0); page 168
 */
typedef union
{
    struct
    {
        uint8_t ATTN_BACKUP;
        uint8_t MIN_GAIN_INDEX;
    } field;
    uint16_t word;
} si47x_frontend_agc_control;

/**
 * @ingroup group01
 * 
 * @brief Data type for RDS Status command and response information 
 *
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 77 and 78 
 * @see Also https://en.wikipedia.org/wiki/Radio_Data_System
 */
typedef union
{
    struct
    {
        uint8_t INTACK : 1;     // Interrupt Acknowledge; 0 = RDSINT status preserved; 1 = Clears RDSINT.
        uint8_t MTFIFO : 1;     // Empty FIFO; 0 = If FIFO not empty; 1 = Clear RDS Receive FIFO.
        uint8_t STATUSONLY : 1; // Determines if data should be removed from the RDS FIFO.
        uint8_t dummy : 5;
    } arg;
    uint8_t raw;
} si47x_rds_command;

/**
 * @ingroup group01
 * 
 * @brief Response data type for current channel and reads an entry from the RDS FIFO.
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 77 and 78
 */
typedef union
{
    struct
    {
        // status ("RESP0")
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1;
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1;
        uint8_t CTS : 1;
        // RESP1
        uint8_t RDSRECV : 1;      //!<  RDS Received; 1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.
        uint8_t RDSSYNCLOST : 1;  //!<  RDS Sync Lost; 1 = Lost RDS synchronization.
        uint8_t RDSSYNCFOUND : 1; //!<  RDS Sync Found; 1 = Found RDS synchronization.
        uint8_t DUMMY3 : 1;
        uint8_t RDSNEWBLOCKA : 1; //!<  RDS New Block A; 1 = Valid Block A data has been received.
        uint8_t RDSNEWBLOCKB : 1; //!<  RDS New Block B; 1 = Valid Block B data has been received.
        uint8_t DUMMY4 : 2;
        // RESP2
        uint8_t RDSSYNC : 1; //!<  RDS Sync; 1 = RDS currently synchronized.
        uint8_t DUMMY5 : 1;
        uint8_t GRPLOST : 1; //!<  Group Lost; 1 = One or more RDS groups discarded due to FIFO overrun.
        uint8_t DUMMY6 : 5;
        // RESP3 to RESP11
        uint8_t RDSFIFOUSED; //!<  RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).
        uint8_t BLOCKAH;     //!<  RESP4 - RDS Block A; HIGH byte
        uint8_t BLOCKAL;     //!<  RESP5 - RDS Block A; LOW byte
        uint8_t BLOCKBH;     //!<  RESP6 - RDS Block B; HIGH byte
        uint8_t BLOCKBL;     //!<  RESP7 - RDS Block B; LOW byte
        uint8_t BLOCKCH;     //!<  RESP8 - RDS Block C; HIGH byte
        uint8_t BLOCKCL;     //!<  RESP9 - RDS Block C; LOW byte
        uint8_t BLOCKDH;     //!<  RESP10 - RDS Block D; HIGH byte
        uint8_t BLOCKDL;     //!<  RESP11 - RDS Block D; LOW byte
        // RESP12 - Blocks A to D Corrected Errors.
        // 0 = No errors;
        // 1 = 1–2 bit errors detected and corrected;
        // 2 = 3–5 bit errors detected and corrected.
        // 3 = Uncorrectable.
        uint8_t BLED : 2;
        uint8_t BLEC : 2;
        uint8_t BLEB : 2;
        uint8_t BLEA : 2;
    } resp;
    uint8_t raw[13];
} si47x_rds_status;

/**
 * @ingroup group01
 * 
 * @brief FM_RDS_INT_SOURCE property data type
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 103
 * @see also https://en.wikipedia.org/wiki/Radio_Data_System
 */
typedef union
{
    struct
    {
        uint8_t RDSRECV : 1;      //!<  If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
        uint8_t RDSSYNCLOST : 1;  //!<  If set, generate RDSINT when RDS loses synchronization.
        uint8_t RDSSYNCFOUND : 1; //!<  f set, generate RDSINT when RDS gains synchronization.
        uint8_t DUMMY1 : 1;       //!<  Always write to 0.
        uint8_t RDSNEWBLOCKA : 1; //!<  If set, generate an interrupt when Block A data is found or subsequently changed
        uint8_t RDSNEWBLOCKB : 1; //!<  If set, generate an interrupt when Block B data is found or subsequently changed
        uint8_t DUMMY2 : 5;       //!<  Reserved - Always write to 0.
        uint8_t DUMMY3 : 5;       //!<  Reserved - Always write to 0.
    } refined;
    uint8_t raw[2];
} si47x_rds_int_source;

/**
 * @ingroup group01
 * 
 * @brief Data type for FM_RDS_CONFIG Property
 * 
 * IMPORTANT: all block errors must be less than or equal the associated block error threshold for the group 
 * to be stored in the RDS FIFO. 
 * 0 = No errors; 1 = 1–2 bit errors detected and corrected; 2 = 3–5 bit errors detected and corrected; 3 = Uncorrectable.
 * Recommended Block Error Threshold options:
 *  2,2,2,2 = No group stored if any errors are uncorrected.
 *  3,3,3,3 = Group stored regardless of errors.
 *  0,0,0,0 = No group stored containing corrected or uncorrected errors.
 *  3,2,3,3 = Group stored with corrected errors on B, regardless of errors on A, C, or D.
 *  
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 58 and 104 
 */
typedef union
{
    struct
    {
        uint8_t RDSEN : 1; //!<  1 = RDS Processing Enable.
        uint8_t DUMMY1 : 7;
        uint8_t BLETHD : 2; //!<  Block Error Threshold BLOCKD
        uint8_t BLETHC : 2; //!<  Block Error Threshold BLOCKC.
        uint8_t BLETHB : 2; //!<  Block Error Threshold BLOCKB.
        uint8_t BLETHA : 2; //!<  Block Error Threshold BLOCKA.
    } arg;
    uint8_t raw[2];
} si47x_rds_config;

/**
 * @ingroup group01
 * 
 * @brief Block A data type
 */
typedef union
{
    struct
    {
        uint16_t pi;
    } refined;
    struct
    {
        uint8_t highValue; // Most Significant uint8_t first
        uint8_t lowValue;
    } raw;
} si47x_rds_blocka;

/**
 * @ingroup group01
 * 
 * @brief Block B data type
 * 
 * @details For GCC on System-V ABI on 386-compatible (32-bit processors), the following stands:
 * 
 * 1) Bit-fields are allocated from right to left (least to most significant).
 * 2) A bit-field must entirely reside in a storage unit appropriate for its declared type.
 *    Thus a bit-field never crosses its unit boundary.
 * 3) Bit-fields may share a storage unit with other struct/union members, including members that are not bit-fields.
 *    Of course, struct members occupy different parts of the storage unit.
 * 4) Unnamed bit-fields' types do not affect the alignment of a structure or union, although individual 
 *    bit-fields' member offsets obey the alignment constraints.   
 * 
 * @see also Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 78 and 79
 * @see also https://en.wikipedia.org/wiki/Radio_Data_System
 */
typedef union
{
    struct
    {
        uint16_t address : 2;            // Depends on Group Type and Version codes. If 0A or 0B it is the Text Segment Address.
        uint16_t DI : 1;                 // Decoder Controll bit
        uint16_t MS : 1;                 // Music/Speech
        uint16_t TA : 1;                 // Traffic Announcement
        uint16_t programType : 5;        // PTY (Program Type) code
        uint16_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint16_t groupType : 4;          // Group Type code.
    } group0;
    struct
    {
        uint16_t address : 4;            // Depends on Group Type and Version codes. If 2A or 2B it is the Text Segment Address.
        uint16_t textABFlag : 1;         // Do something if it chanhes from binary "0" to binary "1" or vice-versa
        uint16_t programType : 5;        // PTY (Program Type) code
        uint16_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint16_t groupType : 4;          // Group Type code.
    } group2;
    struct
    {
        uint16_t content : 4;            // Depends on Group Type and Version codes.
        uint16_t textABFlag : 1;         // Do something if it chanhes from binary "0" to binary "1" or vice-versa
        uint16_t programType : 5;        // PTY (Program Type) code
        uint16_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint16_t groupType : 4;          // Group Type code.
    } refined;
    struct
    {
        uint8_t lowValue;
        uint8_t highValue; // Most Significant byte first
    } raw;
} si47x_rds_blockb;

/*
 * 
 * 
 * Group type 4A ( RDS Date and Time)
 * When group type 4A is used by the station, it shall be transmitted every minute according to EN 50067.
 * This Structure uses blocks 2,3 and 5 (B,C,D)
 * 
 * Commented due to “Crosses boundary” on GCC 32-bit plataform.
 */
/*
typedef union {
    struct
    {
        uint32_t offset : 5;       // Local Time Offset
        uint32_t offset_sense : 1; // Local Offset Sign ( 0 = + , 1 = - )
        uint32_t minute : 6;       // UTC Minutes
        uint32_t hour : 5;         // UTC Hours
        uint32_t mjd : 17;        // Modified Julian Day Code
    } refined;
    uint8_t raw[6];
} si47x_rds_date_time;
*/

/**
 * @ingroup group01
 * 
 * Group type 4A ( RDS Date and Time)
 * When group type 4A is used by the station, it shall be transmitted every minute according to EN 50067.
 * This Structure uses blocks 2,3 and 5 (B,C,D)
 * 
 * ATTENTION: 
 * To make it compatible with 8, 16 and 32 bits platforms and avoid Crosses boundary, it was necessary to
 * split minute and hour representation. 
 */
typedef union
{
    struct
    {
        uint8_t offset : 5;       // Local Time Offset
        uint8_t offset_sense : 1; // Local Offset Sign ( 0 = + , 1 = - )
        uint8_t minute1 : 2;      // UTC Minutes - 2 bits less significant (void “Crosses boundary”).
        uint8_t minute2 : 4;      // UTC Minutes - 4 bits  more significant  (void “Crosses boundary”)
        uint8_t hour1 : 4;        // UTC Hours - 4 bits less significant (void “Crosses boundary”)
        uint8_t hour2 : 1;        // UTC Hours - 4 bits more significant (void “Crosses boundary”)
        uint16_t mjd1 : 15;        // Modified Julian Day Code - 15  bits less significant (void “Crosses boundary”)
        uint16_t mjd2 : 2;         // Modified Julian Day Code - 2 bits more significant (void “Crosses boundary”)
    } refined;
    uint8_t raw[6];
} si47x_rds_date_time;

/** 
 * @ingroup group01
 * 
 * AGC data types
 * FM / AM and SSB structure to AGC
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); For FM page 80; for AM page 142
 * @see AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18. 
 */
typedef union
{
    struct
    {
        // status ("RESP0")
        uint8_t STCINT : 1;
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1; // Not used for AM/SSB
        uint8_t RSQINT : 1;
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1;
        uint8_t CTS : 1;
        // RESP1
        uint8_t AGCDIS : 1; // This bit indicates if the AGC is enabled or disabled. 0 = AGC enabled; 1 = AGC disabled.
        uint8_t DUMMY : 7;
        // RESP2
        uint8_t AGCIDX; // For FM (5 bits - READ_LNA_GAIN_INDEX - 0 = Minimum attenuation (max gain)). For AM (8 bits). This byte reports the current AGC gain index.
    } refined;
    uint8_t raw[3];
} si47x_agc_status;

/** 
 * @ingroup group01
 * 
 * If FM, Overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 
 * (minimum attenuation) and 26 (maximum attenuation).
 * If AM, overrides the AGC setting by disabling the AGC and forcing the gain index that ranges between 0
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); For FM page 81; for AM page 143
 */
typedef union
{
    struct
    {
        // ARG1
        uint8_t AGCDIS : 1; // if set to 1 indicates if the AGC is disabled. 0 = AGC enabled; 1 = AGC disabled.
        uint8_t DUMMY : 7;
        // ARG2
        uint8_t AGCIDX; // AGC Index; If AMAGCDIS = 1, this byte forces the AGC gain index; 0 = Minimum attenuation (max gain)
    } arg;
    uint8_t raw[2];
} si47x_agc_overrride;

/** 
 * @ingroup group01
 * 
 * The bandwidth of the AM channel filter data type
 * AMCHFLT values: 0 = 6 kHz Bandwidth                    
 *                 1 = 4 kHz Bandwidth
 *                 2 = 3 kHz Bandwidth
 *                 3 = 2 kHz Bandwidth
 *                 4 = 1 kHz Bandwidth
 *                 5 = 1.8 kHz Bandwidth
 *                 6 = 2.5 kHz Bandwidth, gradual roll off
 *                 7–15 = Reserved (Do not use)
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 125 and 151
 */
typedef union
{
    struct
    {
        uint8_t AMCHFLT : 4; //!<  Selects the bandwidth of the AM channel filter.
        uint8_t DUMMY1 : 4;
        uint8_t AMPLFLT : 1; //!<  Enables the AM Power Line Noise Rejection Filter.
        uint8_t DUMMY2 : 7;
    } param;
    uint8_t raw[2];
} si47x_bandwidth_config; // AM_CHANNEL_FILTER

/** 
 * @ingroup group01
 * 
 * SSB - datatype for SSB_MODE (property 0x0101)
 * 
 * @see AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 
 */
typedef union
{
    struct
    {
        uint8_t AUDIOBW : 4;     //!<  0 = 1.2kHz (default); 1=2.2kHz; 2=3kHz; 3=4kHz; 4=500Hz; 5=1kHz
        uint8_t SBCUTFLT : 4;    //!<  SSB side band cutoff filter for band passand low pass filter
        uint8_t AVC_DIVIDER : 4; //!<  set 0 for SSB mode; set 3 for SYNC mode;
        uint8_t AVCEN : 1;       //!<  SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default);
        uint8_t SMUTESEL : 1;    //!<  SSB Soft-mute Based on RSSI or SNR
        uint8_t DUMMY1 : 1;      //!<  Always write 0;
        uint8_t DSP_AFCDIS : 1;  //!<  0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
    } param;
    uint8_t raw[2];
} si47x_ssb_mode;

/**
 * @ingroup group01
 * 
 * @brief Digital audio output format data structure (Property 0x0102. DIGITAL_OUTPUT_FORMAT).
 * 
 * @details Used to configure: DCLK edge, data format, force mono, and sample precision.
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 195. 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); chapter 9 - Digital Audio Interface
 */
typedef union
{
    struct
    {
        uint8_t OSIZE : 2; //!<  Digital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).
        uint8_t OMONO : 1; //!<  Digital Output Mono Mode (0=Use mono/stereo blend ).
        uint8_t OMODE : 4; //!<  Digital Output Mode (0000=I2S, 0110 = Left-justified, 1000 = MSB at second DCLK after DFS pulse, 1100 = MSB at first DCLK after DFS pulse).
        uint8_t OFALL : 1; //!<  Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge)
        uint8_t dummy : 8; //!<  Always 0.
    } refined;
    uint16_t raw;
} si4735_digital_output_format;

/**
 * @ingroup group01
 * @brief patch header stored in a eeprom 
 * @details This data type represents o header of a eeprom with a patch content 
 * @details This structure will be used to read an eeprom generated by leo sketch SI47XX_09_SAVE_SSB_PATCH_EEPROM.ino.
 * @details The sketch SI47XX_09_SAVE_SSB_PATCH_EEPROM can be found on Examples/SI47XX_TOOLS folder   
 */
typedef union
{
    struct
    {
        uint8_t reserved[8];  // Not used
        uint8_t status[8];    // Note used
        uint8_t patch_id[14]; // Patch name
        uint16_t patch_size;  // Patch size (in bytes)
    } refined;
    uint8_t raw[32];
} si4735_eeprom_patch_header;

/**
 * @ingroup group01
 * 
 * @brief Digital audio output sample structure (Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE).
 * 
 * @details Used to enable digital audio output and to configure the digital audio output sample rate in samples per second (sps).
 * 
 * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 196. 
 */
typedef struct
{
    uint16_t DOSR;                   // Digital Output Sample Rate(32–48 ksps .0 to disable digital audio output).
} si4735_digital_output_sample_rate; // Maybe not necessary


/********************************************************************** 
 * SI4735 Class definition
 **********************************************************************/

/**
 * @brief SI4735 Class 
 * 
 * @details This class implements all functions to help you to control the Si47XX devices. 
 * This library was built based on “Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0)”. 
 * It also can be used on all members of the SI473X family respecting, of course, the features available 
 * for each IC version.  These functionalities can be seen in the comparison matrix shown in 
 * table 1 (Product Family Function); pages 2 and 3 of the programming guide.
 * 
 * @author PU2CLR - Ricardo Lima Caratti 
 */
class SI4735
{
protected:
    char rds_buffer2A[65]; //!<  RDS Radio Text buffer - Program Information
    char rds_buffer2B[33]; //!<  RDS Radio Text buffer - Station Informaation
    char rds_buffer0A[9];  //!<  RDS Basic tuning and switching information (Type 0 groups)
    char rds_time[25];     //!<  RDS date time received information

    int rdsTextAdress2A; //!<  rds_buffer2A current position
    int rdsTextAdress2B; //!<  rds_buffer2B current position
    int rdsTextAdress0A; //!<  rds_buffer0A current position

    bool rdsEndGroupA = false;
    bool rdsEndGroupB = false;

    int16_t deviceAddress = SI473X_ADDR_SEN_LOW; //!<  Stores the current I2C bus address.

    // Delays
    uint16_t maxDelaySetFrequency = MAX_DELAY_AFTER_SET_FREQUENCY; //!< Stores the maximum delay after set frequency command (in ms).
    uint16_t maxDelayAfterPouwerUp = MAX_DELAY_AFTER_POWERUP;      //!< Stores the maximum delay you have to setup after a power up command (in ms).
    unsigned long maxSeekTime = MAX_SEEK_TIME;                     //!< Stores the maximum time (ms) for a seeking process. Defines the maximum seeking time.

    uint8_t lastTextFlagAB;
    uint8_t resetPin;     //!<  pin used on Arduino Board to RESET the Si47XX device

    uint8_t currentTune; //!<  tell the current tune (FM, AM or SSB)

    uint16_t currentMinimumFrequency; //!<  minimum frequency of the current band
    uint16_t currentMaximumFrequency; //!<  maximum frequency of the current band
    uint16_t currentWorkFrequency;    //!<  current frequency

    uint16_t currentStep; //!<  Stores the current step used to increment or decrement the frequency.

    uint8_t lastMode = -1; //!<  Stores the last mode used.

    uint8_t currentAvcAmMaxGain = DEFAULT_CURRENT_AVC_AM_MAX_GAIN; //!<  Stores the current Automatic Volume Control Gain for AM.
    uint8_t currentClockType = XOSCEN_CRYSTAL; //!< Stores the current clock type used (Crystal or REF CLOCK)
    uint8_t ctsIntEnable = 0;
    uint8_t gpo2Enable = 0;

    uint16_t refClock = 32768;     //!< Frequency of Reference Clock in Hz.
    uint16_t refClockPrescale = 1; //!< Prescaler for Reference Clock (divider).
    uint8_t refClockSourcePin = 0; //!< 0 = RCLK pin is clock source; 1 = DCLK pin is clock source.

    si47x_frequency currentFrequency; //!<  data structure to get current frequency
    si47x_set_frequency currentFrequencyParams;
    si47x_rqs_status currentRqsStatus;       //!<  current Radio SIgnal Quality status
    si47x_response_status currentStatus;     //!<  current device status
    si47x_firmware_information firmwareInfo; //!<  firmware information
    si47x_rds_status currentRdsStatus;       //!<  current RDS status
    si47x_agc_status currentAgcStatus;       //!<  current AGC status
    si47x_ssb_mode currentSSBMode;           //!<  indicates if USB or LSB

    si473x_powerup powerUp;

    uint8_t volume = 32; //!< Stores the current vlume setup (0-63).

    uint8_t currentAudioMode = SI473X_ANALOG_AUDIO; //!< Current audio mode used (ANALOG or DIGITAL or both)
    uint8_t currentSsbStatus;
    int8_t audioMuteMcuPin = -1;

    void waitInterrupr(void);
    si47x_status getInterruptStatus();

    // void setGpioCtl(uint8_t GPO1OEN, uint8_t GPO2OEN, uint8_t GPO3OEN);
    // void setGpio(uint8_t GPO1LEVEL, uint8_t GPO2LEVEL, uint8_t GPO3LEVEL);
    // void setGpioIen(uint8_t STCIEN, uint8_t RSQIEN, uint8_t ERRIEN, uint8_t CTSIEN, uint8_t STCREP, uint8_t RSQREP);

    void sendProperty(uint16_t propertyNumber, uint16_t param);

    void sendSSBModeProperty();
    void disableFmDebug();
    void clearRdsBuffer2A();
    void clearRdsBuffer2B();
    void clearRdsBuffer0A();
    void getSsbAgcStatus();

public:
    SI4735();
    void reset(void);
    void waitToSend(void);

    void setGpioCtl(uint8_t GPO1OEN, uint8_t GPO2OEN, uint8_t GPO3OEN);
    void setGpio(uint8_t GPO1LEVEL, uint8_t GPO2LEVEL, uint8_t GPO3LEVEL);
    void setGpioIen(uint8_t STCIEN, uint8_t RSQIEN, uint8_t ERRIEN, uint8_t CTSIEN, uint8_t STCREP, uint8_t RSQREP);

    void setup(uint8_t resetPin, uint8_t defaultFunction);
    void setup(uint8_t resetPin, uint8_t ctsIntEnable, uint8_t defaultFunction, uint8_t audioMode = SI473X_ANALOG_AUDIO, uint8_t clockType = XOSCEN_CRYSTAL, uint8_t gpo2Enable = 0);

    void setRefClock(uint16_t refclk);
    void setRefClockPrescaler(uint16_t prescale, uint8_t rclk_sel = 0);

    int32_t getProperty(uint16_t propertyValue);

    /**
     * @ingroup group10 Generic set and get property
     * 
     * @brief Sets a given SI47XX device property 
     * 
     * @details Sets the Si47XX device with a given attribute. 
     * @details You might need to use the bit operations or some bit field structure to set right the values.
     * @details Used this function instead of the sendProperty.
     * 
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 55, 69, 124 and  134.
     * @see getProperty, sendProperty
     * @param propertyNumber
     * @param param  pamameter value 
     */
    inline void setProperty(uint16_t propertyNumber, uint16_t param)
    {
        sendProperty(propertyNumber, param);
    };

    void sendCommand(uint8_t cmd, int parameter_size, const uint8_t *parameter);
    void getCommandResponse(int num_of_bytes, uint8_t *response);
    si47x_status getStatusResponse();

    void setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE);
    void radioPowerUp(void);
    void analogPowerUp(void);
    void powerDown(void);

    void setFrequency(uint16_t);

    void getStatus(uint8_t, uint8_t);

    uint16_t getFrequency(void);

    /** 
     * STATUS RESPONSE
     * Set of methods to get current status information. Call them after getStatus or getFrequency or seekStation
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 63
     */

    /**
     * @ingroup group08
     * @brief Get the Signal Quality Interrupt status
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 63            
     * @return RDSINT status
     */
    inline bool getSignalQualityInterrupt()
    {
        return currentStatus.resp.RSQINT;
    };

    /**
     * @ingroup group08
     * @brief Get the Radio Data System (RDS) Interrupt status
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 63
     * @return RDSINT status  
     */
    inline bool getRadioDataSystemInterrupt()
    {
        return currentStatus.resp.RDSINT;
    };

    /**
     * @ingroup group08
     * @brief Get the Tune Complete status
     * @details Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
     * @return STCINT status  
     */
    inline bool getTuneCompleteTriggered()
    {
        return currentStatus.resp.STCINT;
    };

    /**
     * @ingroup group08
     * @brief Get the Status Error 
     * @details Return the Error flag (true or false) of status of the least Tune or Seek
     * @return Error flag 
     */
    inline bool getStatusError()
    {
        return currentStatus.resp.ERR;
    };

    /**
     * @ingroup group08
     * @brief Gets the Error flag Clear to Send
     * 
     * @return CTS 
     */
    inline bool getStatusCTS() { return currentStatus.resp.CTS; };

    /**
     * @ingroup group08
     * @brief Returns true if the AFC rails (AFC Rail Indicator).
     * 
     * @return true 
     */
    inline bool getACFIndicator()
    {
        return currentStatus.resp.AFCRL;
    };

    /**
     * @ingroup group08
     * @brief Returns true if a seek hit the band limit 
     * 
     * @details (WRAP = 0 in FM_START_SEEK) or wrapped to the original frequency(WRAP = 1).
     * 
     * @return BLTF 
     */
    inline bool getBandLimit()
    {
        return currentStatus.resp.BLTF;
    };

    /**
     * @ingroup group08
     * @brief Gets the channel status
     * 
     * @details Returns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108)
     * 
     * @return true 
     * @return false 
     */
    inline bool getStatusValid()
    {
        return currentStatus.resp.VALID;
    };

    /**
     * @ingroup group08
     * @brief Returns the value of  Received Signal Strength Indicator (dBμV).
     * 
     * @return uint8_t 
     */
    inline uint8_t getReceivedSignalStrengthIndicator()
    {
        return currentStatus.resp.RSSI;
    };

    /**
     * @ingroup group08
     * @brief Gets the SNR metric when tune is complete (dB)
     * 
     * @details Returns the value  of the SNR metric when tune is complete (dB).
     * 
     * @return uint8_t 
     */
    inline uint8_t getStatusSNR()
    {
        return currentStatus.resp.SNR;
    };

    /**
     * @ingroup group08
     * @brief Get the Status the M U L T 
     * 
     * @details Returns the value containing the multipath metric when tune is complete.
     * 
     * @return uint8_t 
     */
    inline uint8_t getStatusMULT()
    {
        return currentStatus.resp.MULT;
    };

    /**
     * @ingroup group17
     * @brief Get the Antenna Tuning Capacitor value
     * @details Returns the current antenna tuning capacitor value. The tuning capacitance is 95 fF x READANTCAP + 7 pF.
     * @details ON AM or SSB mode, the MULT attribute sotores the high byte of READANTCAP and the attribute READANTCAP by itself stores the low byte.
     * 
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 74,74, 140 and 141.
     * @return uint8_t capacitance 
     */
    inline uint16_t getAntennaTuningCapacitor()
    {
        si47x_antenna_capacitor cap;

        if (currentTune == FM_TUNE_FREQ)
            return currentStatus.resp.READANTCAP;
        else
        {
            cap.raw.ANTCAPL = currentStatus.resp.READANTCAP; // On AM it is the low byte the READANTCAP value
            cap.raw.ANTCAPH = currentStatus.resp.MULT;       // On AM it is the high byte the READANTCAP value
            return cap.value;
        }
    };

    void getAutomaticGainControl(); //!<  Queries Automatic Gain Control STATUS

    /**
     * @ingroup group17
     * @brief Sets the Avc Am Max Gain to maximum gain (0x7800)
     */
    inline void setAvcAmMaxGain()
    {
        sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x7800);
        currentAvcAmMaxGain = 90;
    };

    /**
     * @ingroup group17
     * @brief Sets the Avc Am Max Gain to minimal gain (0x1000)
     */
    inline void setAvcAmMinGain()
    {
        sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x1000);
        currentAvcAmMaxGain = 12;
    };

    /**
     * @ingroup group17
     * @brief Sets the Avc Am Max Gain to default gain (0x2A80)
     */
    inline void setAvcAmDefaultGain()
    {
        sendProperty(AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN, 0x1543);
        currentAvcAmMaxGain = DEFAULT_CURRENT_AVC_AM_MAX_GAIN;
    };


    void setAvcAmMaxGain(uint8_t gain = 90); //!<  Sets the maximum gain for automatic volume control.

    /**
     * @ingroup group17
     * @brief Get the current Avc Am Max Gain 
     * 
     * @return uint8_t Current AVC gain index value
     */
    inline uint8_t getCurrentAvcAmMaxGain()
    {
        return currentAvcAmMaxGain;
    };

    /**
     * @ingroup group17
     * @brief Sets the Am Soft Mute Max Attenuation 
     * 
     * @details This function can be useful to disable Soft Mute. The value 0 disable soft mute.
     * @details Specified in units of dB. Default maximum attenuation is 8 dB. It works for AM and SSB.
     * 
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 158. 
     * 
     * @param smattn Maximum attenuation to apply when in soft mute
     */
    inline void setAmSoftMuteMaxAttenuation(uint8_t smattn = 0)
    {
        sendProperty(AM_SOFT_MUTE_MAX_ATTENUATION, smattn);
    };

    /**
     * @ingroup group17
     * @brief Sets the SSB Soft Mute Max Attenuation object
     * 
     * @details Sets maximum attenuation during soft mute (dB). Set to 0 to disable soft mute. 
     * @details Specified in units of dB. Default maximum attenuation is 8 dB.
     * @details You can use setAmSoftMuteMaxAttenuation instead. Same AM property values.  
     * @param smattn Maximum attenuation to apply when in soft mute.
     */
    inline void setSsbSoftMuteMaxAttenuation(uint8_t smattn = 0)
    {
        sendProperty(SSB_SOFT_MUTE_MAX_ATTENUATION, smattn);
    };

    /**
     * @ingroup group17
     * @brief Sets the number of milliseconds the low IF peak detector
     * 
     * @details Sets the number of milliseconds the low IF peak detector must not be exceeded before increasing the gain. Default value is 140 (approximately 40 dB / s).
     * @param param number of milliseconds ( from 4 to 248; step 4); default value 0x008C (140).
     */
    inline void setSsbIfAgcReleaseRate(uint8_t param = 140)
    {
        sendProperty(SSB_IF_AGC_RELEASE_RATE, param);
    };

    /**
     * @ingroup group17
     * @brief Sets the IF AGC attack rate
     * 
     * @details Large values provide slower attack, and smaller values provide faster attack
     * @param param number of milliseconds ( from 4 to 248; step 4); default value 4.
     */
    inline void setSsbIfAgcAttackRate(uint8_t param = 4)
    {
        sendProperty(SSB_IF_AGC_ATTACK_RATE, param);
    };

    /**
     * @ingroup group08
     * @brief Checks if the AGC is enabled
     *
     * @return true if the AGC is enabled 
     */
    inline bool isAgcEnabled()
    {
        return !currentAgcStatus.refined.AGCDIS;
    };

    /**
     * @ingroup group08
     * @brief Gets the current AGC gain index
     * 
     * @return uint8_t The current AGC gain index.
     */
    inline uint8_t getAgcGainIndex()
    {
        return currentAgcStatus.refined.AGCIDX;
    };

    void setAutomaticGainControl(uint8_t AGCDIS, uint8_t AGCIDX);

    /**
     * @ingroup group08 AGC
     *
     * @brief Automatic Gain Control setup (alternative name for setAutomaticGainControl )
     *
     * @details If FM, overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0
     * (minimum attenuation) and 26 (maximum attenuation).
     * @details If AM/SSB, Overrides the AGC setting by disabling the AGC and forcing the gain index that ranges between 0
     * (minimum attenuation) and 37+ATTN_BACKUP (maximum attenuation).
     *
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); For FM page 81; for AM page 143
     * @see setAutomaticGainControl
     *
     * @param uint8_t AGCDIS This param selects whether the AGC is enabled or disabled (0 = AGC enabled; 1 = AGC disabled);
     * @param uint8_t AGCIDX AGC Index (0 = Minimum attenuation (max gain); 1 – 36 = Intermediate attenuation);
     *                if >greater than 36 - Maximum attenuation (min gain) ).
     */
    inline void setAGC(uint8_t AGCDIS, uint8_t AGCIDX) { setAutomaticGainControl( AGCDIS, AGCIDX); };


    void setSsbAgcOverrite(uint8_t SSBAGCDIS, uint8_t SSBAGCNDX, uint8_t reserved = 0);

    void getCurrentReceivedSignalQuality(uint8_t INTACK);
    void getCurrentReceivedSignalQuality(void);

    // AM and FM

    /**
     * @ingroup group08
     * @brief Get the current receive signal strength (0–127 dBμV)
     * 
     * @return uint8_t a value between 0 to 127
     */
    inline uint8_t getCurrentRSSI()
    {
        return currentRqsStatus.resp.RSSI;
    };

    /**
     * @ingroup group08
     * @brief Gets the current SNR metric (0–127 dB).
     * 
     * @return uint8_t SNR value in dB (0-127)
     */
    inline uint8_t getCurrentSNR()
    {
        return currentRqsStatus.resp.SNR;
    };

    /**
     * @ingroup group08
     * @brief Checks if RSSI detected is LOW. 
     * 
     * @return true if RSSI is low
     */
    inline bool getCurrentRssiDetectLow()
    {
        return currentRqsStatus.resp.RSSIILINT;
    };

    /**
     * @ingroup group08
     * @brief Checks if RSSI detected is high 
     * 
     * @return true if RSSI detected is high
     */
    inline bool getCurrentRssiDetectHigh()
    {
        return currentRqsStatus.resp.RSSIHINT;
    };

    /**
     * @ingroup group08
     * @brief Checks if SNR detect is low
     * 
     * @return true if SNR detected is low
     */
    inline bool getCurrentSnrDetectLow()
    {
        return currentRqsStatus.resp.SNRLINT;
    };

    /**
     * @ingroup group08
     * @brief Checks if SNR detect is high
     * 
     * @return true  if SNR detect is high
     */
    inline bool getCurrentSnrDetectHigh()
    {
        return currentRqsStatus.resp.SNRHINT;
    };

    /**
     * @ingroup group08
     * @brief Checks if the current channel is valid
     * 
     * @return true if the current channel is valid
     */
    inline bool getCurrentValidChannel()
    {
        return currentRqsStatus.resp.VALID;
    };

    /**
     * @ingroup group08
     * @brief AFC Rail Indicator
     * 
     * @return true or false
     */
    inline bool getCurrentAfcRailIndicator()
    {
        return currentRqsStatus.resp.AFCRL;
    };

    /**
     * @ingroup group08
     * @brief Soft Mute Indicator. 
     * 
     * @details Indicates soft mute is engaged.
     * 
     * @return true  if soft mute indicates is engaged.
     */
    inline bool getCurrentSoftMuteIndicator()
    {
        return currentRqsStatus.resp.SMUTE;
    };

    // Just FM

    /**
     * @ingroup group08
     * @brief Gets the value of the amount of stereo blend in % (100 = full stereo, 0 = full mono).
     * 
     * @return uint8_t value (0 to 100)
     */
    inline uint8_t getCurrentStereoBlend()
    {
        return currentRqsStatus.resp.STBLEND;
    };

    /**
     * @ingroup group08
     * @brief Checks the current pilot 
     * 
     * @details Indicates stereo pilot presence.
     * 
     * @return true if stereo pilot presence has detected
     */
    inline bool getCurrentPilot()
    {
        return currentRqsStatus.resp.PILOT;
    };

    /**
     * @ingroup group08
     * @brief Gets the current Multipath
     *  
     * @details Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
     * 
     * @return uint8_t value (0 to 100)
     */
    inline uint8_t getCurrentMultipath()
    {
        return currentRqsStatus.resp.MULT;
    };

    /**
     * @ingroup group08
     * @brief Gets the Signed frequency offset (kHz).
     * 
     * @return uint8_t 
     */
    inline uint8_t getCurrentSignedFrequencyOffset() {
         return currentRqsStatus.resp.FREQOFF; 
    }; 

    /**
     * @ingroup group08
     * @brief Get Multipath Detect Low
     * 
     * @return true 
     * @return false 
     */
    inline bool getCurrentMultipathDetectLow() { 
        return currentRqsStatus.resp.MULTLINT; 
    };     

    /**
     * @ingroup group08
     * @brief Gets the Current Multipath Detect High 
     * 
     * @return true 
     * @return false 
     */
    inline bool getCurrentMultipathDetectHigh() { 
        return currentRqsStatus.resp.MULTHINT; 
    };   

    /**
     * @ingroup group08
     * @brief Gets the Current Blend Detect Interrupt
     * 
     * @return true 
     * @return false 
     */
    inline bool getCurrentBlendDetectInterrupt() { 
        return currentRqsStatus.resp.BLENDINT; 
    };    

    /*
     * FIRMWARE RESPONSE
     * 
     * See Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 66
     */

    /**
     * @ingroup group06
     * @brief Returns the Firmware Part Number
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwarePN() { 
        return firmwareInfo.resp.PN; 
    }; 

    /**
     * @ingroup group06
     * @brief Returns the Firmware F W M A J O R
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwareFWMAJOR() { 
        return firmwareInfo.resp.FWMAJOR; 
    };  

    /**
     * @ingroup group06
     * @brief Returns the Firmware F W M I N O R 
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwareFWMINOR() { 
        return firmwareInfo.resp.FWMINOR; 
    };   

    /**
     * @ingroup group06
     * @brief Returns the Firmware P A T C H  HIGH
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwarePATCHH() { 
        return firmwareInfo.resp.PATCHH; 
    }; 

    /**
     * @ingroup group06
     * @brief Returns the Firmware P A T C H  LOW
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwarePATCHL() { 
        return firmwareInfo.resp.PATCHL; 
    };   

    /**
     * @ingroup group06
     * @brief Get the Firmware C M P M A J O R object
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwareCMPMAJOR() { 
        return firmwareInfo.resp.CMPMAJOR; 
    }; //!<  RESP6 -  Returns the Component Major Revision (ASCII).

    /**
     * @ingroup group06
     * @brief Returns the Component Minor Revision (ASCII) (RESP7)
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwareCMPMINOR() { 
        return firmwareInfo.resp.CMPMINOR; 
    };

    /**
     * @ingroup group06
     * @brief RESP8 -  Returns the Chip Revision (ASCII)
     * 
     * @return uint8_t 
     */
    inline uint8_t getFirmwareCHIPREV() { 
        return firmwareInfo.resp.CHIPREV; 
    }; 

    void setVolume(uint8_t volume);
    uint8_t getVolume();
    void volumeDown();
    void volumeUp();

    /**
     * @ingroup group13 Audio volume
     * @brief Get the Current Volume 
     * @details Returns the current volume level.
     * @return uint8_t 
     */
    inline uint8_t getCurrentVolume() { return volume; };

    /**
     * @ingroup group13 Audio volume
     * @brief Set the Volume Up
     * @details Same volumeUp()
     * @see volumeUp
     */
    inline void setVolumeUp() { volumeUp(); };
    /**
     * @ingroup group13 Audio volume
     * @brief Set the Volume Down 
     * @details Same volumeDown()
     * @return voi 
     */
    inline void setVolumeDown() { volumeDown(); };

    /**
     * @ingroup group13 Digital Audio setup
     * @brief Sets the Audio Mode. See table below. 
     * @details If you want to change the audio mode, call this function before call setAM(), setFM() or setSSB(). 
     * @details Sets the Si47XX device to use ANALOG or DIGITAL audio output. The table below show the valid values. 
     * @details This function will only take effect after calling setAM(), setFM() or setSSB().
     * 
     * 
     * | Macro | Value (Binary) | Description |
     * | ----- | ----- | ----------- | 
     * | SI473X_ANALOG_AUDIO | 0b00000101 | Analog Audio Inputs |
     * | SI473X_DIGITAL_AUDIO1 | 0b00001011 | Digital audio output (DCLK, LOUT/DFS, ROUT/DIO) |
     * | SI473X_DIGITAL_AUDIO2  | 0b10110000 | Digital audio outputs (DCLK, DFS, DIO) |
     * | SI473X_DIGITAL_AUDIO3 | 0b10110101 | Analog and digital audio outputs (LOUT/ROUT and DCLK, DFS,DIO) |
     * 
     * @see setAM(), setFM(), setSSB().
     * @param audioMode One of the values options above
     */
    inline void setAudioMode(uint8_t audioMode)
    {
        currentAudioMode = audioMode;
    };

    /**
     * @ingroup group13 Audio Noise Blank Delay
     * @brief Sets the delay before applying impulse blanking 
     * @details Delay in micro-seconds before applying impulse blanking to the original samples. Default value is 172.
     * 
     * @param value Delay in micro-seconds
     */
    inline void setAmDelayNB(uint16_t value) {
        sendProperty(AM_NB_DELAY, value);
    } 

    void digitalOutputFormat(uint8_t OSIZE, uint8_t OMONO, uint8_t OMODE, uint8_t OFALL);
    void digitalOutputSampleRate(uint16_t DOSR);

    void setAudioMute(bool off); // if true mute the audio; else unmute

    void setAM();
    void setFM();
    void setAM(uint16_t fromFreq, uint16_t toFreq, uint16_t intialFreq, uint16_t step);
    void setFM(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step);

    /**
     * @ingroup group08
     * @brief Sets the FM Receive de-emphasis to 50 or 75 μs. 
     * @details valid parameters are 1 = 50 μs. Usedin Europe, Australia, Japan; 2 = 75 μs. Used in USA (default)
     * 
     * @param parameter 1 or 2 (default 1 - USA)
     */
    inline void setFMDeEmphasis(uint8_t parameter)
    {
        sendProperty(FM_DEEMPHASIS, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the Fm Soft Mute Max Attenuation 
     * 
     * @details This function can be useful to disable Soft Mute on FM mode. The value 0 disable soft mute.
     * @details Specified in units of dB. Default maximum attenuation is 8 dB. It works for AM and SSB.
     * 
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 97. 
     * 
     * @param smattn Maximum attenuation to apply when in soft mute
     */
    inline void setFmSoftMuteMaxAttenuation(uint8_t smattn = 0)
    {
        sendProperty(FM_SOFT_MUTE_MAX_ATTENUATION, smattn);
    };

    /**
    * @brief Set the Fm Noise Blank Threshold 
    * @details Sets the threshold for detecting impulses in dB above the noise floor. The CTS bit (and optional interrupt) is set when it is safe to send the next command.
    * @param parameter (from 0 to 90. default is 10)
    */
    inline void setFmNoiseBlankThreshold(uint16_t parameter)
    {
        sendProperty(FM_NB_DETECT_THRESHOLD, parameter);
    };

    /**
     * @brief Set the Fm Noise Blank 
     * @details Sets Noise blanking rate in 100 Hz units
     * @details Sets the Interval in micro-seconds that original samples are replaced by sample-hold clean samples.
     * @details Sets the bandwidth of the noise floor estimator.
     * 
     * @details ATTENTION: It works on SI474X. It may not work on SI473X devices. 
     * 
     * @param nb_rate Noise blanking rate in 100 Hz units. Default value is 64.
     * @param nb_interval Interval in micro-seconds that original samples are replaced by interpolated clean samples. Default value is 55 μs.
     * @param nb_irr_filter Sets the bandwidth of the noise floor estimator. Default value is 300.
     */
    inline void setFmNoiseBlank(uint16_t nb_rate = 64, uint16_t nb_interval = 55, uint16_t nb_irr_filter = 300)
    {
        sendProperty(FM_NB_RATE, nb_rate);
        sendProperty(FM_NB_INTERVAL, nb_interval);
        sendProperty(FM_NB_IIR_FILTER, nb_irr_filter);
    }

    /**
     * @brief Set the Fm Noise Blank Interval 
     * @details Interval in micro-seconds that original samples are replaced by interpolated clean samples.
     * @param parameter ( from 8 to 48. default value is 24)
     */
    inline void setFmNoiseBlankInterval(uint16_t parameter)
    {
        sendProperty(FM_NB_INTERVAL, parameter);
    };

    /**
     * @brief Set the Fm Noise Blank Rate
     * @details Noise blanking rate in 100 Hz units.
     * 
     * @param parameter ( from 1 to 64. default value is 64)
     */
    inline void setFmNoiseBlankRate(uint16_t parameter)
    {
        sendProperty(FM_NB_RATE, parameter);
    };

    /**
     * @brief Set the Fm Noise Blank Delay
     * @details Delay in micro-seconds before applying impulse blanking to the original samples.
     * @param parameter ( from 125 to 219. default value is 170)
     */
    inline void  setFmNoiseBlankDelay(uint16_t parameter)
    {
        sendProperty(FM_NB_DELAY, parameter);
    };

    /**
     * @brief Set the FmNoiseBlank IIR Filter 
     * @details Sets the bandwidth of the noise floor estimator.
     * @param parameter (from 300 to 1600. default value is 300)
     */
    inline void setFmNoiseBlank_IIR_Filter(uint16_t parameter)
    {
        sendProperty(FM_NB_IIR_FILTER, parameter);
    }

    /**
     * @ingroup group08
     * @brief Sets the AM Receive de-emphasis to 50 or disable. 
     * @details valid parameters are 1 = 50 μs. Usedin urope, Australia, Japan; 2 = 75 μs. Used in USA (default)
     * 
     * @param parameter 1 = enable or 0 = disable
     */
   inline void setAMDeEmphasis(uint8_t parameter)
    {
        sendProperty(AM_DEEMPHASIS, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the AM attenuation slope during soft mute
     * @details Configures attenuation slope during soft mute in dB attenuation per dB SNR below the soft mute SNR threshold.
     * @details Soft mute attenuation is the minimum of SMSLOPEx(SMTHR–SNR) and SMATTN.
     * @details The default slope is 1 dB/dB for AMRX component 5.0 or later and 2 dB/dB for AMRX component 3.0 or earlier.
     * 
     * @see setAmSoftMuteMaxAttenuation
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); 
     * @param parameter  the valid values are 1–5 (default 1).
     */
    inline void setAMSoftMuteSlop(uint8_t parameter)
    {
       sendProperty(AM_SOFT_MUTE_SLOPE, parameter);
    };


    /**
     * @ingroup group08
     * @brief Sets the attack and decay rates when entering or leaving soft mute.
     * @details The value specified is multiplied by 4.35 dB/s to come up with the actual attack rate
     * @details The default rate is 278 dB/s.
     * @see setAmSoftMuteMaxAttenuation
     * @see Si47XX PRORAMMING GUIDE; AN332 (REV 1.0); 
     * @param parameter  The valid values are 1-255  ( Default is ~64 - [64 x 4.35 = 278] )
     */
    inline void setAMSoftMuteRate(uint8_t parameter)
    {
       sendProperty(AM_SOFT_MUTE_RATE, parameter);
    };


    /**
     * @ingroup group08
     * @brief Sets the SNR threshold to engage soft mute
     * @details Whenever the SNR for a tuned frequency drops below this threshold the AM reception will go in soft mute,
     * @details provided soft mute max attenuation property is non-zero. The default value is 8dB
     * @see setAmSoftMuteMxAttenuation
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.0); 
     * @param parameter  0-63 (default is 8)
     */
    inline void setAMSoftMuteSnrThreshold(uint8_t parameter)
    {
       sendProperty(AM_SOFT_MUTE_SNR_THRESHOLD, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the soft mute release rate.
     * @details Smaller values provide slower release and larger values provide faster release. The default is 8192 (approximately 8000 dB/s).
     * @see setAmSoftMuteMxAttenuation
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.0); 
     * @param parameter  1–32767 
     */
    inline void setAMSoftMuteReleaseRate(uint8_t parameter)
    {
       sendProperty(AM_SOFT_MUTE_RELEASE_RATE, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the soft mute attack rate.
     * @details Smaller values provide slower attack and larger values provide faster attack. 
     * @see setAmSoftMuteMxAttenuation
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.0); 
     * @param parameter  1–32767 (The default is 8192 (approximately 8000 dB/s)
     */
    inline void setAMSoftMuteAttackRate(uint16_t parameter)
    {
       sendProperty(AM_SOFT_MUTE_ATTACK_RATE, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the AGC attack rate.
     * @details Large values provide slower attack, and smaller values provide faster attack.. 
     * @see setAmAgcAttackRate
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.2); page 167 
     * @param parameter Range: 4–248 (The default is 0x04) 
     */
    inline void setAmAgcAttackRate(uint16_t parameter)
    {
        sendProperty(AM_AGC_ATTACK_RATE, parameter);
    };

    /**
     * @ingroup group08
     * @brief Sets the AGC release rate.
     * @details  Larger values provide slower release, and smaller values provide faster release.
     * @see setAmAgcReleaseRate
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.2); page 168 
     * @param parameter Range: 4–248 (The default is 0x8C) 
     */
    inline void setAmAgcReleaseRate(uint16_t parameter)
    {
        sendProperty(AM_AGC_RELEASE_RATE, parameter);
    };

    /**
     * @ingroup group17
     * @brief Sets the AGC attack rate on SSB mode.
     * @details Large values provide slower attack, and smaller values provide faster attack.. 
     * @see setSsbAgcAttackRate
     * @see AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 29 
     * @param parameter Range: 4–248 (The default is 0x04) 
     */
    inline void setSsbAgcAttackRate(uint16_t parameter)
    {
        sendProperty(SSB_RF_AGC_ATTACK_RATE, parameter);
    };

    /**
     * @ingroup group17
     * @brief Sets the AGC Release rate on SSB mode.
     * @details Larger values provide slower release, and smaller values provide faster release. 
     * @see setSsbAgcAttackRate
     * @see AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 29 
     * @param parameter Range: 4–248 (The default is 0x18)
     */
    inline void setSsbAgcReleaseRate(uint16_t parameter)
    {
        sendProperty(SSB_RF_AGC_RELEASE_RATE, parameter);
    };

    /**
     * @ingroup group08
     * @brief Adjusts the AM AGC for external front-end attenuator and external front-end cascode LNA.
     * @details This property contains two fields: MIN_GAIN_INDEX and ATTN_BACKUP.
     * @details MIN_GAIN_INDEX impacts sensitivity and U/D performance. Lower values improve sensitivity, but degrade 
     * @details far away blocker U/D performance. 
     * @details Higher values degrade sensitivity, but improve U/D. With MIN_GAIN_INDEX=19 and Si4743 EVB reference 
     * @details design, the Si474x provides sensitivity of 28dBuV typical and U/D exceeding 55dB on far away blockers.
     * @details With MIN_GAIN_INDEX=24, the Si474x provides sensitivity of 34dBuV typical and U/D approaching 70dB on 
     * @details far away blockers. 
     * @see Si47XX PROAMMING GUIDE; AN332 (REV 1.0); page 168
     * @param MIN_GAIN_INDEX Values below 19 have minimal sensitivity improvement; Higher values degrade sensitivity, but improve U/D.
     * @param ATTN_BACKUP ??? 
     */
    inline void setAMFrontEndAgcControl(uint8_t MIN_GAIN_INDEX, uint8_t ATTN_BACKUP)
    {
        si47x_frontend_agc_control param;

        param.field.MIN_GAIN_INDEX = MIN_GAIN_INDEX;
        param.field.ATTN_BACKUP = ATTN_BACKUP;

        sendProperty(AM_FRONTEND_AGC_CONTROL, param.word);
    };

    /**
     * @brief Set the Am Noise Blank 
     * 
     * @details Sets Noise blanking rate in 100 Hz units
     * @details Sets the Interval in micro-seconds that original samples are replaced by sample-hold clean samples.
     * @details Sets the bandwidth of the noise floor estimator.
     * 
     * @details ATTENTION: It works on SI474X. It may not work on SI473X devices. 
     * 
     * @param nb_rate Noise blanking rate in 100 Hz units. Default value is 64.
     * @param nb_interval Interval in micro-seconds that original samples are replaced by interpolated clean samples. Default value is 55 μs.
     * @param nb_irr_filter Sets the bandwidth of the noise floor estimator. Default value is 300.
     * 
     */
    inline void setAmNoiseBlank(uint16_t nb_rate = 64, uint16_t nb_interval = 55, uint16_t nb_irr_filter = 300)
    {
        sendProperty(AM_NB_RATE, nb_rate);
        sendProperty(AM_NB_INTERVAL, nb_interval);
        sendProperty(AM_NB_IIR_FILTER, nb_irr_filter);
    }

    /* @ingroup group08 Check FM mode status 
     * @brief Returns true if the current function is FM (FM_TUNE_FREQ).
     * 
     * @return true if the current function is FM (FM_TUNE_FREQ).
     */
    inline bool isCurrentTuneFM()
    {
        return (currentTune == FM_TUNE_FREQ);
    }

    /**
     * @ingroup group08 Check AM mode status
     * 
     * @brief Returns true if the current function is AM (AM_TUNE_FREQ).
     * 
     * @return true if the current function is AM (AM_TUNE_FREQ).
     */
    inline bool isCurrentTuneAM()
    {
        return (currentTune == AM_TUNE_FREQ);
    }

    /**
     * @ingroup group08 Check SSB mode status
     * 
     * @brief Returns true if the current function is SSB (SSB_TUNE_FREQ).
     * 
     * @return true if the current function is SSB (SSB_TUNE_FREQ).
     */
    inline bool isCurrentTuneSSB()
    {
        return (currentTune == SSB_TUNE_FREQ);
    }

    void setBandwidth(uint8_t AMCHFLT, uint8_t AMPLFLT);

    /**
     * @brief Sets the Bandwith on FM mode
     * @details Selects bandwidth of channel filter applied at the demodulation stage. Default is automatic which means the device automatically selects proper channel filter. <BR>
     * @details | Filter  | Description |
     * @details | ------- | -------------|
     * @details |    0    | Automatically select proper channel filter (Default) |
     * @details |    1    | Force wide (110 kHz) channel filter |
     * @details |    2    | Force narrow (84 kHz) channel filter |
     * @details |    3    | Force narrower (60 kHz) channel filter |
     * @details |    4    | Force narrowest (40 kHz) channel filter |
     * 
     * @param filter_value 
     */
    inline void setFmBandwidth(uint8_t filter_value = 0)
    {
        sendProperty(FM_CHANNEL_FILTER, filter_value);
    }

    /**
     * @ingroup group08 Tune Frequency 
     * @brief Returns the FAST tuning status
     * 
     * @retrn uint8_t 
     */
    inline uint8_t getTuneFrequecyFast() { 
        return currentFrequencyParams.arg.FAST; 
    };  

    /**
     * @ingroup group08 Tune Frequency 
     * @brief Sets the FAST Tuning.  
     * @details If set, excutes fast and invalidated tune. Theune status will not be accurate
     * 
     * @param FAST 
     */
    inline void setTuneFrequencyFast (uint8_t FAST) { 
        currentFrequencyParams.arg.FAST = FAST; 
    };   

    /**
     * @ingroup group08 Tune Frequency 
     * @brief Returns the FREEZE status
     * 
     * @return unt8_t 
     */
    inline uint8_t getTuneFrequecyFreeze() { 
        return currentFrequencyParams.arg.FREEZE; 
    };  
    
    /**
     * @ingroup group08 Tune Frequency 
     * @brief Sets Freeze Metrics During Alternate Frequency Jum.
     * @details Only on FM mode
     * 
     * @param FREEZE 
     */
    inline void setTuneFrequencyFreze(uint8_t FREEZE) { 
        currentFrequencyParams.arg.FREEZE = FREEZE; 
    }; 
   
    
    void setTuneFrequencyAntennaCapacitor(uint16_t capacitor);

    void frequencyUp();
    void frequencyDown();

    /**
     * @ingroup group08 Tune Frequency 
     * @brief Set the FrequencyUp 
     * @details Same frequencyUp
     * @see frequencyUp
     */
    inline void setFrequencyUp() { frequencyUp(); };

    /**
     * @ingroup   group08 Tune Frequency 
     * @brief Set the Frequency Down 
     * @details same frequencyDown
     * @see frequencyDown
     */
    inline void setFrequencyDown() { frequencyDown(); };

    void getFirmware(void);

    void seekStation(uint8_t SEEKUP, uint8_t WRAP); // See WRAP parameter

    /**
     * @ingroup group08 Seek 
     * @brief Sets the maximum time in milliseconds for seeking. The default value is 8000ms (8s).
     * @details Depending on the bandwidth, your reception conditions or step configuration, the seek process can take a long time.
     * @details This function sets a time limit for seeking process and back the control to the system if the time runs out.
     * 
     * @addindex Seek
     * 
     * @param time_in_ms time in milliseconds. 
     */
    inline void setMaxSeekTime(long time_in_ms)
    {
        this->maxSeekTime = time_in_ms;
    };

    /**
    * @ingroup group08 Seek 
    * 
    * @brief Search for the next station
    * @details Seek a station up. Stop when a station is found or the frequency has reached the upper limit  
    * @see seekStation, seekStationProgress, setSeekAmLimits setSeekFmLimits 
    * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 124, 137, 139, 278
    */
    inline void seekStationUp()
    {
        seekStationProgress(NULL, SEEK_UP);
    };

    /**
    * @ingroup group08 Seek 
    * 
    * @brief Search the previous station
    * @details Seek a station Down. Stop when a station is found or the frequency has reached the lower limit  
    * @see seekStation, seekStationProgress, setSeekAmLimits, setSeekFmLimits
    * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 124, 137, 139, 278 
    */
    inline void seekStationDown()
    {
        seekStationProgress(NULL, SEEK_DOWN);
    };

    void seekNextStation();
    void seekPreviousStation();

    void seekStationProgress(void (*showFunc)(uint16_t f), uint8_t up_down);
    void seekStationProgress(void (*showFunc)(uint16_t f), bool (*stopSeking)(),  uint8_t up_down);

    // AM Seek property configurations
    void setSeekAmLimits(uint16_t bottom, uint16_t top);
    void setSeekAmSpacing(uint16_t spacing);

    /**
     * @ingroup group08 Seek
     * @brief Set the Seek Am Srn Threshold object
     * @deprecated Use setSeekAmSNRThreshold instead.
     * @see setSeekAmSNRThreshold
     * @param value 
     */
    void inline setSeekAmSrnThreshold(uint16_t value) { sendProperty(AM_SEEK_SNR_THRESHOLD, value); }; // Wrong name! Will be removed later  

    /**
     * @ingroup group08 Seek
     *
     * @brief Sets the SNR threshold for a valid AM Seek/Tune.
     *
     * @details If the value is zero then SNR threshold is not considered when doing a seek. Default value is 5 dB.
     * @see Si47XX PROGRAMMING GUIDE;  (REV 1.0); page 127
     */
    void inline setSeekAmSNRThreshold(uint16_t value) { sendProperty(AM_SEEK_SNR_THRESHOLD, value); }; // Fixing the function name

    void setSeekAmRssiThreshold(uint16_t value);

    // FM Seek property configurations
    void setSeekFmLimits(uint16_t bottom, uint16_t top);
    void setSeekFmSpacing(uint16_t spacing);

    /**
     * @ingroup group08 Seek
     * @brief Set the Seek Fm Srn Threshold object
     * @deprecated Use setSeekFmSNRThreshold instead.
     * @see setSeekFmSNRThreshold
     * @param value 
     */
    void inline setSeekFmSrnThreshold(uint16_t value) { sendProperty(FM_SEEK_TUNE_SNR_THRESHOLD, value); }; // Wrong name. Will be removed later
    
    /**
     * @ingroup group08 Seek
     *
     * @brief Sets the SNR threshold for a valid FM Seek/Tune.
     *
     * @details SNR Threshold which determines if a valid channel has been found during Seek/Tune. Specified in units of dB in 1 dB steps (0–127). Default is 3 dB
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); page 102
     *
     * @param value between 0 and 127.
     */
    void inline setSeekFmSNRThreshold(uint16_t value) { sendProperty(FM_SEEK_TUNE_SNR_THRESHOLD, value); }; // Fixing the function name
    
    void setSeekFmRssiThreshold(uint16_t value);

    void setFmBlendStereoThreshold(uint8_t parameter);
    void setFmBlendMonoThreshold(uint8_t parameter);
    void setFmBlendRssiStereoThreshold(uint8_t parameter);
    void setFmBLendRssiMonoThreshold(uint8_t parameter);
    void setFmBlendSnrStereoThreshold(uint8_t parameter);
    void setFmBLendSnrMonoThreshold(uint8_t parameter);
    void setFmBlendMultiPathStereoThreshold(uint8_t parameter);
    void setFmBlendMultiPathMonoThreshold(uint8_t parameter);
    void setFmStereoOn();
    void setFmStereoOff();

    void RdsInit();
    void setRdsIntSource(uint8_t RDSRECV, uint8_t RDSSYNCLOST, uint8_t RDSSYNCFOUND, uint8_t RDSNEWBLOCKA, uint8_t RDSNEWBLOCKB);
    void getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY);

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds Received FIFO
     * @details if FIFO is 1, it means the minimum number of groups was filled
     * @return true if minimum number of groups was filled.
     */
    inline bool getRdsReceived()
    {
        return currentRdsStatus.resp.RDSRECV;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds Sync Lost object
     * @details returns true (1) if Lost RDS synchronization is detected. 
     * @return true if Lost RDS synchronization detected. 
     */
    inline bool getRdsSyncLost()
    {
        return currentRdsStatus.resp.RDSSYNCLOST;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds Sync Found 
     * @details return true if found RDS synchronization
     * @return true if found RDS synchronization
     */
    inline bool getRdsSyncFound()
    {
        return currentRdsStatus.resp.RDSSYNCFOUND;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds New Block A 
     * 
     * @details Returns true if valid Block A data has been received.
     * @return true or false
     */
    inline bool getRdsNewBlockA()
    {
        return currentRdsStatus.resp.RDSNEWBLOCKA;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds New Block B 
     * @details Returns true if valid Block B data has been received.
     * @return true or false
     */
    inline bool getRdsNewBlockB()
    {
        return currentRdsStatus.resp.RDSNEWBLOCKB;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Rds Sync 
     * @details Returns true if RDS currently synchronized.
     * @return true or false
     */
    inline bool getRdsSync()
    {
        return currentRdsStatus.resp.RDSSYNC;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Group Lost 
     * @details Returns true if one or more RDS groups discarded due to FIFO overrun.
     * @return true or false
     */
    inline bool getGroupLost()
    {
        return currentRdsStatus.resp.GRPLOST;
    };

    /**
     * @ingroup group16 RDS
     * @brief Get the Num Rds Fifo Used 
     * @details Return the number of RDS FIFO used
     * @return uint8_t Total RDS FIFO used
     */
    inline uint8_t getNumRdsFifoUsed()
    {
        return currentRdsStatus.resp.RDSFIFOUSED;
    };

    /**
     * @ingroup group16 RDS
     * @brief Sets the minimum number of RDS groups stored in the RDS FIFO before RDSRECV is set. 
     * @details Return the number of RDS FIFO used
     * @param value from 0 to 25. Default value is 0. 
     */
    inline void setFifoCount(uint16_t value)
    {
        sendProperty(FM_RDS_INT_FIFO_COUNT, value);
    };

    /**
     * @ingroup group16 RDS
     * @brief Check if 0xD or 0xA special characters were received for group A
     * @see resetEndIndicatorGroupA
     * @return true or false
     */
    inline bool getEndIndicatorGroupA() {
        return rdsEndGroupA;
    }

    /**
     * @ingroup group16 RDS
     * @brief Resets 0xD or 0xA special characters condition (makes it false)
     * @see getEndIndicatorGroupA
     */
    inline void resetEndIndicatorGroupA() {
        rdsEndGroupA = false;
    }

    /**
     * @ingroup group16 RDS
     * @brief Check if 0xD or 0xA special characters were received for group B
     * @see resetEndIndicatorGroupB
     * @return true or false
     */
    inline bool getEndIndicatorGroupB()
    {
        return rdsEndGroupB;
    }

    /**
     * @ingroup group16 RDS
     * @brief Resets 0xD or 0xA special characters condition (makes it false)
     * @see getEndIndicatorGroupB
     */
    inline void resetEndIndicatorGroupB()
    {
        rdsEndGroupB = false;
    }

    /**
     * @ingroup group16 RDS status 
     * 
     * @brief Gets RDS Status.
     * 
     * @details Same result of calling getRdsStatus(0,0,0).
     * @details Please, call getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY) instead getRdsStatus() 
     * if you want other behaviour. 
     * 
     * @see SI4735::getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY)
     */
    inline void getRdsStatus()
    {
        getRdsStatus(0, 0, 0);
    }

    /**
     * @ingroup group16 RDS status 
     * @brief Empty FIFO
     * @details  Clear RDS Receive FIFO.
     * @see getRdsStatus
     */
    inline void rdsClearFifo()
    {
        getRdsStatus(0, 1, 0);
    }

    /**
     * @ingroup group16 RDS status 
     * @brief Clears RDSINT.
     * @details  INTACK Interrupt Acknowledge; 0 = RDSINT status preserved. 1 = Clears RDSINT.
     * @see getRdsStatus
     */
    inline void rdsClearInterrupt()
    {
        getRdsStatus(1, 0, 0);
    }

    void setRdsConfig(uint8_t RDSEN, uint8_t BLETHA, uint8_t BLETHB, uint8_t BLETHC, uint8_t BLETHD);
    uint16_t getRdsPI(void);
    uint8_t getRdsGroupType(void);
    uint8_t getRdsFlagAB(void);
    uint8_t getRdsVersionCode(void);
    uint8_t getRdsProgramType(void);
    uint8_t getRdsTextSegmentAddress(void);

    char *getRdsText(void);
    char *getRdsText0A(void); // Gets the Station name
    char *getRdsText2A(void); // Gets the Radio Text
    char *getRdsText2B(void);

    void mjdConverter(uint32_t mjd, uint32_t *year, uint32_t *month, uint32_t *day);
    char *getRdsTime(void);
    char *getRdsDateTime(void);
    bool getRdsDateTime(uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t * minute);

    void getNext2Block(char *);
    void getNext4Block(char *);

    void setSSBBfo(int offset);
    void setSSBConfig(uint8_t AUDIOBW, uint8_t SBCUTFLT, uint8_t AVC_DIVIDER, uint8_t AVCEN, uint8_t SMUTESEL, uint8_t DSP_AFCDIS);
    void setSSB(uint16_t fromFreq, uint16_t toFreq, uint16_t intialFreq, uint16_t step, uint8_t usblsb);
    void setSSB(uint8_t usblsb);
    void setSSBAudioBandwidth(uint8_t AUDIOBW);
    void setSSBAutomaticVolumeControl(uint8_t AVCEN);
    void setSSBSidebandCutoffFilter(uint8_t SBCUTFLT); // Fixing the function name

    /**
     * @ingroup group17 Patch and SSB support
     * @deprecated Use setSSBSidebandCutoffFilter instead.
     * @see setSSBSidebandCutoffFilter
     * @param SBCUTFLT 
     */
    void inline setSBBSidebandCutoffFilter(uint8_t SBCUTFLT) { setSSBSidebandCutoffFilter(SBCUTFLT); }; // Wrong name! will be removed later.

    void setSSBAvcDivider(uint8_t AVC_DIVIDER);
    void setSSBDspAfc(uint8_t DSP_AFCDIS);
    void setSSBSoftMute(uint8_t SMUTESEL);

    void setNBFM();
    void setNBFM(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint16_t step);
    void patchPowerUpNBFM();
    void loadPatchNBFM(const uint8_t *patch_content, const uint16_t patch_content_size);
    void setFrequencyNBFM(uint16_t freq);

    si47x_firmware_query_library queryLibraryId();
    void patchPowerUp();
    bool downloadPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size);
    bool downloadCompressedPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size, const uint16_t *cmd_0x15, const int16_t cmd_0x15_size);
    void loadPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size, uint8_t ssb_audiobw = 1);
    void loadCompressedPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size, const uint16_t *cmd_0x15, const int16_t cmd_0x15_size, uint8_t ssb_audiobw = 1);
    si4735_eeprom_patch_header downloadPatchFromEeprom(int eeprom_i2c_address);
    void ssbPowerUp();

    /**
     * @ingroup group06 Si47XX device Power Up 
     * @brief Set the Max Delay Power Up 
     * @details Sets the delay needed in ms after a powerup command (default is 10ms).
     * @details Some crystal oscillator might need more time to become stable (500 ms is the recommended).
     * @details Low values make the load SSB patch faster. However, it can make the system unstable.   
     * 
     * @see MAX_DELAY_AFTER_POWERUP  
     * @param ms delay in ms
     */
    inline void setMaxDelayPowerUp(uint16_t ms)
    {
        this->maxDelayAfterPouwerUp = ms;
    }

    /**
     * @ingroup   group08 Tune Frequency
     * @brief Set the Max Delay after Set Frequency 
     * 
     * @details After the set frequency command, the system need a time to get ready to the next set frequency (default value 30ms).
     * @details Why the waitToSend() does not work in this case? No idea for while! 
     * @details A low value makes the getFrequency command inaccurate. 
     * 
     * @see  MAX_DELAY_AFTER_POWERUP
     * @param ms 
     */
    inline void setMaxDelaySetFrequency(uint16_t ms)
    {
        this->maxDelaySetFrequency = ms;
    }

    /** 
     * @ingroup group08 Tune Frequency step
     * 
     * @brief Sets the current step value. 
     * 
     * @details This function does not check the limits of the current band. Please, don't take a step bigger than your legs.
     * @details Example:
     * @code
     * setFM(6400,10800,10390,10);
     * setFrequencyStep(100); // the step will be 1MHz (you are using FM mode)
     * .
     * .
     * .
     * setAM(7000,7600,7100,5); 
     * setFrequencyStep(1); // the step will be 1kHz (you are usin AM or SSB mode)  
     * @endcode 
     * 
     * @see setFM()
     * @see setAM()
     * @see setSSB()
     * 
     * @param step if you are using FM, 10 means 100kHz. If you are using AM 10 means 10kHz
     *             For AM, 1 (1kHz) to 1000 (1MHz) are valid values.
     *             For FM 5 (50kHz), 10 (100kHz) and 100 (1MHz) are valid values.  
     */
    inline void setFrequencyStep(uint16_t step)
    {
        this->currentStep = step;
    }

    /**
     * @ingroup group08 Frequency 
     * 
     * @brief Gets the current frequency saved in memory. 
     * 
     * @details Unlike getFrequency, this method gets the current frequency recorded after the last setFrequency command. 
     * @details This method avoids bus traffic and CI processing.
     * @details However, you can not get others status information like RSSI.
     * 
     * @see getFrequency()
     */
    inline uint16_t getCurrentFrequency()
    {
        return this->currentWorkFrequency;
    }

    /**
     * @ingroup group08 Si47XX device Status 
     * 
     * @brief Gets the current status  of the Si47XX (AM, FM or SSB)
     * @see Si47XX PROGRAMMING GUIDE; AN332 (REV 1.0); pages 73 (FM) and 139 (AM)
     */
    inline void getStatus()
    {
        getStatus(0, 1);
    }

    void setDeviceI2CAddress(uint8_t senPin);
    int16_t getDeviceI2CAddress(uint8_t resetPin);
    void setDeviceOtherI2CAddress(uint8_t i2cAddr);

    /******************************************************************************* 
     * The functions below modify the clock frequency for I2C communication. 
     * 100kHz  is usually the baseline.
     * Use one of these funcition if you have problem on you default configuration. 
     *******************************************************************************/

    /**
     * @ingroup group18 MCU I2C Speed 
     * @brief Sets I2C bus to 10kHz
     */
    inline void setI2CLowSpeedMode(void)
    {
        Wire.setClock(10000);
    };

    /**
     * @ingroup group18 MCU I2C Speed  
     * 
     * @brief Sets I2C bus to 100kHz
     */
    inline void setI2CStandardMode(void) { Wire.setClock(100000); };

    /**
     * @ingroup group18 MCU I2C Speed 
     * 
     * @brief Sets I2C bus to 400kHz
     */
    inline void setI2CFastMode(void)
    {
        Wire.setClock(400000);
    };

    /**
     * @ingroup group18 MCU I2C Speed  
     * 
     * @brief Sets the I2C bus to a given value.
     * ATTENTION: use this function with cation
     * 
     * @param value in Hz. For example: The values 500000 sets the bus to 500kHz.
     */
    inline void setI2CFastModeCustom(long value = 500000) { Wire.setClock(value); };

    /**
     * @ingroup group18 MCU External Audio Mute  
     * 
     * @brief Sets the Audio Mute Mcu Pin
     * @details This function sets the mcu digital pin you want to use to control the external audio mute circuit.
     * @details Some users may be uncomfortable with the loud popping of the speaker during some transitions caused by some SI47XX commands.  
     * @details This problem occurs during the transition from the power down to power up. 
     * @details Every time the user changes the mode (FM to AM or AM to FM) the power down and power up commands are required by the Si47XX devices.
     * @details If you have a extra circuit in your receiver to mute the audio on amplifier input, you can configure a MCU pin to control it by using this function.
     * 
     * @see setHardwareAudioMute
     * @param pin if 0 ou greater sets the MCU digital pin will be used to control de external circuit.  
     */
    inline void setAudioMuteMcuPin(int8_t pin)
    {
        audioMuteMcuPin = pin;
        pinMode(audioMuteMcuPin, OUTPUT);
    };

    /**
     * @ingroup group18 MCU External Audio Mute 
     * 
     * @brief Sets the Hardware Audio Mute
     * @details Turns the Hardware audio mute on or off
     * 
     * @see setAudioMuteMcuPin
     * 
     * @param on  True or false
     */
    inline void setHardwareAudioMute(bool on)
    {
        digitalWrite(audioMuteMcuPin, on);
        delayMicroseconds(300);
    }

    void convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros = true );
};
#endif // _SI4735_H
