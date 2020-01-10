/*
 * SI4735 ARDUINO LIBRARY  
 * Const, Data type and Methods definitions
 * References: 
 *      Si47XX PROGRAMMING GUIDE AN332
 *      AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE
 * 
 * See documentation on https://github.com/pu2clr/SI4735
 * Ricardo Lima Caratti, Nov 2019  
 */

#include <Arduino.h>
#include <Wire.h>

#define POWER_UP_FM 0  // FM
#define POWER_UP_AM 1  // AM and SSB (if patch applyed)
#define POWER_UP_WB 3  // Weather Band Receiver
#define POWER_PATCH 15 //

// SI473X commands (general)
#define SI473X_ADDR 0x11    // SI473X I2C buss address
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

// FM RDS properties
#define FM_RDS_INT_SOURCE 0x1500
#define FM_RDS_INT_FIFO_COUNT 0x1501
#define FM_RDS_CONFIG 0x1502
#define FM_RDS_CONFIDENCE 0x1503

#define FM_BLEND_STEREO_THRESHOLD 0x1105
#define FM_BLEND_MONO_THRESHOLD 0x1106
#define FM_BLEND_RSSI_STEREO_THRESHOLD 0x1800
#define FM_BLEND_RSSI_MONO_THRESHOLD 0x1801
#define FM_BLEND_SNR_STEREO_THRESHOLD 0x1804
#define FM_BLEND_SNR_MONO_THRESHOLD 0x1805
#define FM_BLEND_MULTIPATH_STEREO_THRESHOLD 0x1808
#define FM_BLEND_MULTIPATH_MONO_THRESHOLD 0x1809

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
// See  Si47XX PROGRAMMING GUIDE AN332; page 125
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

// SSB
#define SSB_RF_IF_AGC_ATTACK_RATE 0x3702  // Sets the number of milliseconds the high IF peak detector must be exceeded before decreasing gain. Defaul 4.
#define SSB_RF_IF_AGC_RELEASE_RATE 0x3703 // Sets the number of milliseconds the low IF peak detector must be exceeded before increasing the gain. Defaul 140.

// See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 12 and 13
#define LSB_MODE 1 // 01
#define USB_MODE 2 // 10

// Parameters
#define SI473X_ANALOG_AUDIO B00000101  // Analog Audio Inputs
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
        uint8_t FUNC : 4;    // Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
        uint8_t XOSCEN : 1;  // Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
        uint8_t PATCH : 1;   // Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
        uint8_t GPO2OEN : 1; // GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
        uint8_t CTSIEN : 1;  // CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).
        // ARG2
        uint8_t OPMODE; // Application Setting. See page 65
    } arg;
    uint8_t raw[2]; // same arg memory position, so same content.
} si473x_powerup;

/*
 * Represents how the  frequency is stored in the si4735.
 * It helps to convert frequency in uint16_t to two bytes (uint8_t) (FREQL and FREQH)  
 */
typedef union {
    struct
    {
        uint8_t FREQL; // Tune Frequency High byte.
        uint8_t FREQH; // Tune Frequency Low byte.
    } raw;
    uint16_t value;
} si47x_frequency;

/*
 * 
 */
typedef union {
    struct
    {
        uint8_t ANTCAPL; // Antenna Tuning Capacitor High byte
        uint8_t ANTCAPH; // Antenna Tuning Capacitor Low byte
    } raw;
    uint16_t value;
} si47x_antenna_capacitor;

/*
 * AM_TUNE_FREQ data type command
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 135
 */
typedef union {
    struct
    {
        uint8_t FAST : 1;   // ARG1 - FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.
        uint8_t FREEZE : 1; // Valid only for FM (Must be 0 to AM)
        uint8_t DUMMY1 : 4; // Always set 0
        uint8_t USBLSB : 2; // SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection. 10 = USB is selected; 01 = LSB is selected.
        uint8_t FREQH;      // ARG2 - Tune Frequency High byte.
        uint8_t FREQL;      // ARG3 - Tune Frequency Low byte.
        uint8_t ANTCAPH;    // ARG4 - Antenna Tuning Capacitor High byte.
        uint8_t ANTCAPL;    // ARG5 - Antenna Tuning Capacitor Low byte. Note used for FM.
    } arg;
    uint8_t raw[5];
} si47x_set_frequency;

/* 
 *  Represents searching for a valid frequency data type.
 */
typedef union {
    struct
    {
        uint8_t RESERVED1 : 2;
        uint8_t WRAP : 1;   // Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
        uint8_t SEEKUP : 1; // Determines the direction of the search, either UP = 1, or DOWN = 0.
        uint8_t RESERVED2 : 4;
    } arg;
    uint8_t raw;
} si47x_seek;

/*  
 * Response status command 
 * See Si47XX PROGRAMMING GUIDE; pages 73 and 
 */
typedef union {
    struct
    {
        // Status
        uint8_t STCINT : 1; // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
        uint8_t DUMMY1 : 1;
        uint8_t RDSINT : 1; // Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
        uint8_t RSQINT : 1; // Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
        uint8_t DUMMY2 : 2;
        uint8_t ERR : 1; // Error. 0 = No error 1 = Error
        uint8_t CTS : 1; // Clear to Send.
        // RESP1
        uint8_t VALID : 1; // Valid Channel
        uint8_t AFCRL : 1; // AFC Rail Indicator
        uint8_t DUMMY3 : 5;
        uint8_t BLTF : 1; // Reports if a seek hit the band limit
        // RESP2
        uint8_t READFREQH; // Read Frequency High byte.
        // RESP3
        uint8_t READFREQL; // Read Frequency Low byte.
        // RESP4
        uint8_t RSSI; // Received Signal Strength Indicator (dBμV)
        // RESP5
        uint8_t SNR; // This byte contains the SNR metric when tune is complete (dB).
        // RESP6
        uint8_t MULT; // Contains the multipath metric when tune is complete
        // RESP7
        uint8_t READANTCAP; // Contains the current antenna tuning capacitor value
    } resp;
    uint8_t raw[7];
} si47x_response_status;

/*
 * Firmware Information (GET_ENV)
 */
typedef union {
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
        uint8_t PN;       // RESP1 - Final 2 digits of Part Number (HEX).
        uint8_t FWMAJOR;  // RESP2 - Firmware Major Revision (ASCII).
        uint8_t FWMINOR;  // RESP3 - Firmware Minor Revision (ASCII).
        uint8_t PATCHH;   // RESP4 - Patch ID High byte (HEX).
        uint8_t PATCHL;   // RESP5 - Patch ID Low byte (HEX).
        uint8_t CMPMAJOR; // RESP6 - Component Major Revision (ASCII).
        uint8_t CMPMINOR; // RESP7 - Component Minor Revision (ASCII).
        uint8_t CHIPREV;  // RESP8 - Chip Revision (ASCII).
        // RESP9 to RESP15 not used
    } resp;
    uint8_t raw[9];
} si47x_firmware_information;

/*
 * Firmware Query Library ID response. 
 * Used to represent the response of a power up command with FUNC = 15
 */
typedef union {
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
        uint8_t PN;        // RESP1 - Final 2 digits of Part Number (HEX).
        uint8_t FWMAJOR;   // RESP2 - Firmware Major Revision (ASCII).
        uint8_t FWMINOR;   // RESP3 - Firmware Minor Revision (ASCII).
        uint8_t RESERVED1; // RESP4 - Reserved, various values.
        uint8_t RESERVED2; // RESP5 - Reserved, various values.
        uint8_t CHIPREV;   // RESP6 - Chip Revision (ASCII).
        uint8_t LIBRARYID; // RESP7 - Library Revision (HEX).
        // RESP9 to RESP15 not used
    } resp;
    uint8_t raw[8];
} si47x_firmware_query_library;

/*
 * Status of FM_TUNE_FREQ or FM_SEEK_START commands or 
 * Status of AM_TUNE_FREQ or AM_SEEK_START commands.
 * 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 73 and 139
 */
typedef union {
    struct
    {
        uint8_t INTACK : 1; // If set, clears the seek/tune complete interrupt status indicator.
        uint8_t CANCEL : 1; // If set, aborts a seek currently in progress.
        uint8_t RESERVED2 : 6;
    } arg;
    uint8_t raw;
} si47x_tune_status;

/*
 * Property Data type (help to deal with SET_PROPERTY command on si473X)
 */
typedef union {
    struct
    {
        uint8_t byteLow;
        uint8_t byteHigh;
    } raw;
    uint16_t value;
} si47x_property;

/*
 ********************** RDS Data types *******************************
 */

/* 
 * Data type for status information about the received signal quality
 * FM_RSQ_STATUS and AM_RSQ_STATUS
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 75 and 
 */
typedef union {
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
        uint8_t RSSIILINT : 1; // RSSI Detect Low.
        uint8_t RSSIHINT : 1;  // RSSI Detect High.
        uint8_t SNRLINT : 1;   // SNR Detect Low.
        uint8_t SNRHINT : 1;   // SNR Detect High.
        uint8_t MULTLINT : 1;  // Multipath Detect Low
        uint8_t MULTHINT : 1;  // Multipath Detect High
        uint8_t DUMMY3 : 1;
        uint8_t BLENDINT : 1; // Blend Detect Interrupt.
        // RESP2
        uint8_t VALID : 1; // Valid Channel.
        uint8_t AFCRL : 1; // AFC Rail Indicator.
        uint8_t DUMMY4 : 1;
        uint8_t SMUTE : 1; // Soft Mute Indicator. Indicates soft mute is engaged.
        uint8_t DUMMY5 : 4;
        // RESP3
        uint8_t STBLEND : 7; // Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).
        uint8_t PILOT : 1;   // Indicates stereo pilot presence.
        // RESP4 to RESP7
        uint8_t RSSI;    // RESP4 - Contains the current receive signal strength (0–127 dBμV).
        uint8_t SNR;     // RESP5 - Contains the current SNR metric (0–127 dB).
        uint8_t MULT;    // RESP6 - Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
        uint8_t FREQOFF; // RESP7 - Signed frequency offset (kHz).
    } resp;
    uint8_t raw[8];
} si47x_rqs_status;

/*
 * FM_RDS_STATUS (0x24) command
 * Data type for command and response information 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78
 * See Also https://en.wikipedia.org/wiki/Radio_Data_System
 */

// Command data type
typedef union {
    struct
    {
        uint8_t INTACK : 1;     // Interrupt Acknowledge; 0 = RDSINT status preserved; 1 = Clears RDSINT.
        uint8_t MTFIFO : 1;     // Empty FIFO; 0 = If FIFO not empty; 1 = Clear RDS Receive FIFO.
        uint8_t STATUSONLY : 1; // Determines if data should be removed from the RDS FIFO.
        uint8_t dummy : 5;
    } arg;
    uint8_t raw;
} si47x_rds_command;

// Response data type for current channel and reads an entry from the RDS FIFO.
typedef union {
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
        uint8_t RDSRECV : 1;      // RDS Received; 1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.
        uint8_t RDSSYNCLOST : 1;  // RDS Sync Lost; 1 = Lost RDS synchronization.
        uint8_t RDSSYNCFOUND : 1; // RDS Sync Found; 1 = Found RDS synchronization.
        uint8_t DUMMY3 : 1;
        uint8_t RDSNEWBLOCKA : 1; // RDS New Block A; 1 = Valid Block A data has been received.
        uint8_t RDSNEWBLOCKB : 1; // RDS New Block B; 1 = Valid Block B data has been received.
        uint8_t DUMMY4 : 2;
        // RESP2
        uint8_t RDSSYNC : 1; // RDS Sync; 1 = RDS currently synchronized.
        uint8_t DUMMY5 : 1;
        uint8_t GRPLOST : 1; // Group Lost; 1 = One or more RDS groups discarded due to FIFO overrun.
        uint8_t DUMMY6 : 5;
        // RESP3 to RESP11
        uint8_t RDSFIFOUSED; // RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).
        uint8_t BLOCKAH;     // RESP4 - RDS Block A; HIGH byte
        uint8_t BLOCKAL;     // RESP5 - RDS Block A; LOW byte
        uint8_t BLOCKBH;     // RESP6 - RDS Block B; HIGH byte
        uint8_t BLOCKBL;     // RESP7 - RDS Block B; LOW byte
        uint8_t BLOCKCH;     // RESP8 - RDS Block C; HIGH byte
        uint8_t BLOCKCL;     // RESP9 - RDS Block C; LOW byte
        uint8_t BLOCKDH;     // RESP10 - RDS Block D; HIGH byte
        uint8_t BLOCKDL;     // RESP11 - RDS Block D; LOW byte
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

/*
 * FM_RDS_INT_SOURCE property data type
 * See Si47XX PROGRAMMING GUIDE; AN332; page 103
 * See also https://en.wikipedia.org/wiki/Radio_Data_System
 */
typedef union {
    struct
    {
        uint8_t RDSRECV : 1;      // If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
        uint8_t RDSSYNCLOST : 1;  // If set, generate RDSINT when RDS loses synchronization.
        uint8_t RDSSYNCFOUND : 1; // f set, generate RDSINT when RDS gains synchronization.
        uint8_t DUMMY1 : 1;       // Always write to 0.
        uint8_t RDSNEWBLOCKA : 1; // If set, generate an interrupt when Block A data is found or subsequently changed
        uint8_t RDSNEWBLOCKB : 1; // If set, generate an interrupt when Block B data is found or subsequently changed
        uint8_t DUMMY2 : 5;       // Reserved - Always write to 0.
        uint8_t DUMMY3 : 5;       // Reserved - Always write to 0.
    } refined;
    uint8_t raw[2];
} si47x_rds_int_source;

/*
 * Data type for FM_RDS_CONFIG Property
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
 */
typedef union {
    struct
    {
        uint8_t RDSEN : 1; // 1 = RDS Processing Enable.
        uint8_t DUMMY1 : 7;
        uint8_t BLETHD : 2; // Block Error Threshold BLOCKD
        uint8_t BLETHC : 2; // Block Error Threshold BLOCKC.
        uint8_t BLETHB : 2; // Block Error Threshold BLOCKB.
        uint8_t BLETHA : 2; // Block Error Threshold BLOCKA.
    } arg;
    uint8_t raw[2];
} si47x_rds_config;

/*
 * Block A data type
 */
typedef union {
    struct
    {
        uint16_t pi;
    } refined;
    struct
    {
        uint8_t lowValue;
        uint8_t highValue; // Most Significant uint8_t first
    } raw;
} si47x_rds_blocka;

/*
 * Block B data type
 * See also Si47XX PROGRAMMING GUIDE; AN332; pages 78 and 79
 * See also https://en.wikipedia.org/wiki/Radio_Data_System
 */
typedef union {
    struct {
        uint8_t address : 2;            // Depends on Group Type and Version codes. If 2A or 2B it is the Text Segment Address.
        uint8_t DI:1;                    // Decoder Controll bit
        uint8_t MS : 1;                  // Music/Speech
        uint8_t TA : 1;                 // Traffic Announcement
        uint8_t programType : 5;        // PTY (Program Type) code
        uint8_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint8_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint8_t groupType : 4;          // Group Type code.
    } group0;
    struct
    {
        uint8_t address : 4;            // Depends on Group Type and Version codes. If 2A or 2B it is the Text Segment Address.
        uint8_t textABFlag : 1;         // Do something if it chanhes from binary "0" to binary "1" or vice-versa
        uint8_t programType : 5;        // PTY (Program Type) code
        uint8_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint8_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint8_t groupType : 4;          // Group Type code.

    } group2;
    struct
    {
        uint8_t content : 4;            // Depends on Group Type and Version codes. If 2A or 2B it is the Text Segment Address.
        uint8_t textABFlag : 1;         // Do something if it chanhes from binary "0" to binary "1" or vice-versa
        uint8_t programType : 5;        // PTY (Program Type) code
        uint8_t trafficProgramCode : 1; // (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint8_t versionCode : 1;        // (B0) => 0=A; 1=B
        uint8_t groupType : 4;          // Group Type code.
    } refined;
    struct
    {
        uint8_t lowValue;
        uint8_t highValue; // Most Significant byte first
    } raw;
} si47x_rds_blockb;

/*
 * Group type 4A ( RDS Date and Time)
 * When group type 4A is used by the station, it shall be transmitted every minute according to EN 50067.
 * This Structure uses blocks 2,3 and 5 (B,C,D)
 */
typedef union {
    struct
    {
        uint8_t offset : 5;       // Local Time Offset
        uint8_t offset_sense : 1; // Local Offset Sign ( 0 = + , 1 = - )
        uint8_t minute : 6;       // UTC Minutes
        uint8_t hour : 5;         // UTC Hours
        uint32_t mjd : 17;        // Modified Julian Day Code
    } refined;
    uint8_t raw[6];
} si47x_rds_date_time;

/* AGC data types
 * FM / AM and SSB structure to AGC
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 80; for AM page 142
 * See AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18. 
 */
typedef union {
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

/* 
 * If FM, Overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 
 * (minimum attenuation) and 26 (maximum attenuation).
 * If AM, overrides the AGC setting by disabling the AGC and forcing the gain index that ranges between 0
 * See Si47XX PROGRAMMING GUIDE; AN332; For FM page 81; for AM page 143
 */
typedef union {
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

/* 
 * The bandwidth of the AM channel filter data type
 * AMCHFLT values: 0 = 6 kHz Bandwidth                    
 *                 1 = 4 kHz Bandwidth
 *                 2 = 3 kHz Bandwidth
 *                 3 = 2 kHz Bandwidth
 *                 4 = 1 kHz Bandwidth
 *                 5 = 1.8 kHz Bandwidth
 *                 6 = 2.5 kHz Bandwidth, gradual roll off
 *                 7–15 = Reserved (Do not use)
 */
typedef union {
    struct
    {
        uint8_t AMCHFLT : 4; // Selects the bandwidth of the AM channel filter.
        uint8_t DUMMY1 : 4;
        uint8_t AMPLFLT : 1; // Enables the AM Power Line Noise Rejection Filter.
        uint8_t DUMMY2 : 7;
    } param;
    uint8_t raw[2];
} si47x_bandwidth_config; // AM_CHANNEL_FILTER

/* 
 * SSB - datatype for SSB_MODE (property 0x0101)
 * See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 
 */
typedef union {
    struct
    {
        uint8_t AUDIOBW : 4;     // 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz
        uint8_t SBCUTFLT : 4;    // SSB side band cutoff filter for band passand low pass filter
        uint8_t AVC_DIVIDER : 4; // set 0 for SSB mode; set 3 for SYNC mode;
        uint8_t AVCEN : 1;       // SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default);
        uint8_t SMUTESEL : 1;    // SSB Soft-mute Based on RSSI or SNR
        uint8_t DUMMY1 : 1;      // Always write 0;
        uint8_t DSP_AFCDIS : 1;  // 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
    } param;
    uint8_t raw[2];
} si47x_ssb_mode;

/************************ Deal with Interrupt  *************************/
volatile static bool data_from_si4735;

static void interrupt_hundler()
{
    data_from_si4735 = true;
};

/************************* SI4735 Class definition ********************/

class SI4735
{

private:
    char rds_buffer2A[65]; // RDS Radio Text buffer - Program Information
    char rds_buffer2B[33]; // RDS Radio Text buffer - Station Informaation
    char rds_buffer0A[9];  // RDS Basic tuning and switching information (Type 0 groups)
    char rds_time[20];
    int rdsTextAdress2A;
    int rdsTextAdress2B;
    int rdsTextAdress0A;

    uint8_t lastTextFlagAB;
    uint8_t resetPin;
    uint8_t interruptPin;

    uint8_t currentTune;

    uint16_t currentMinimumFrequency;
    uint16_t currentMaximumFrequency;
    uint16_t currentWorkFrequency;

    uint8_t currentStep;

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
    SI4735();
    void reset(void);
    void waitToSend(void); // Wait for Si4735 device ready to receive command
    void setup(uint8_t resetPin, uint8_t defaultFunction);
    void setup(uint8_t resetPin, int interruptPin, uint8_t defaultFunction);
    void setPowerUp(uint8_t CTSIEN, uint8_t GPO2OEN, uint8_t PATCH, uint8_t XOSCEN, uint8_t FUNC, uint8_t OPMODE);
    void analogPowerUp(void);
    void powerDown(void);

    void setFrequency(uint16_t);

    // getStatus
    void getStatus();
    void getStatus(uint8_t, uint8_t);

    // Status response
    uint16_t getFrequency(void);
    uint16_t getCurrentFrequency(); // See documentation

    /* STATUS RESPONSE
     * Set of methods to get current status information. Call them after getStatus or getFrequency or seekStation
     * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
     */
    inline bool getSignalQualityInterrupt() { return currentStatus.resp.RSQINT; };           // Gets Received Signal Quality Interrupt(RSQINT)
    inline bool getRadioDataSystemInterrupt() { return currentStatus.resp.RDSINT; };         // Gets Radio Data System (RDS) Interrupt
    inline bool getTuneCompleteTriggered() { return currentStatus.resp.STCINT; };            // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
    inline bool getStatusError() { return currentStatus.resp.ERR; };                         // Return the Error flag (true or false) of status of the least Tune or Seek
    inline bool getStatusCTS() { return currentStatus.resp.CTS; };                           // Gets the Error flag of status response
    inline bool getACFIndicator() { return currentStatus.resp.AFCRL; };                      // Returns true if the AFC rails (AFC Rail Indicator).
    inline bool getBandLimit() { return currentStatus.resp.BLTF; };                          // Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or wrapped to the original frequency(WRAP = 1).
    inline bool getStatusValid() { return currentStatus.resp.VALID; };                       // eturns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108)
    inline uint8_t getReceivedSignalStrengthIndicator() { return currentStatus.resp.RSSI; }; // Returns integer Received Signal Strength Indicator (dBμV).
    inline uint8_t getStatusSNR() { return currentStatus.resp.SNR; };                        // returns integer containing the SNR metric when tune is complete (dB).
    inline uint8_t getStatusMULT() { return currentStatus.resp.MULT; };                      // Returns integer containing the multipath metric when tune is complete.
    inline uint8_t getAntennaTuningCapacitor() { return currentStatus.resp.READANTCAP; };    // Returns integer containing the current antenna tuning capacitor value.

    void getAutomaticGainControl();

    inline bool isAgcEnabled() { return !currentAgcStatus.refined.AGCDIS; };      // Returns true if the AGC is enabled
    inline uint8_t getAgcGainIndex() { return currentAgcStatus.refined.AGCIDX; }; // Returns the current AGC gain index.
    void setAutomaticGainControl(uint8_t AGCDIS, uint8_t AGCIDX);                 // Overrides the AGC setting

    /* RQS STATUS RESPONSE 
     * 
     */
    void getCurrentReceivedSignalQuality(uint8_t INTACK);
    void getCurrentReceivedSignalQuality(void);

    // AM and FM
    inline uint8_t getCurrentRSSI() { return currentRqsStatus.resp.RSSI; };            // current receive signal strength (0–127 dBμV).
    inline uint8_t getCurrentSNR() { return currentRqsStatus.resp.SNR; };              // current SNR metric (0–127 dB).
    inline bool getCurrentRssiDetectLow() { return currentRqsStatus.resp.RSSIILINT; }; // RSSI Detect Low.
    inline bool getCurrentRssiDetectHigh() { return currentRqsStatus.resp.RSSIHINT; }; // RSSI Detect High
    inline bool getCurrentSnrDetectLow() { return currentRqsStatus.resp.SNRLINT; };    // SNR Detect Low.
    inline bool getCurrentSnrDetectHigh() { return currentRqsStatus.resp.SNRHINT; };   // SNR Detect High
    inline bool getCurrentValidChannel() { return currentRqsStatus.resp.VALID; };      // Valid Channel.
    inline bool getCurrentAfcRailIndicator() { return currentRqsStatus.resp.AFCRL; };  // AFC Rail Indicator.
    inline bool getCurrentSoftMuteIndicator() { return currentRqsStatus.resp.SMUTE; }; // Soft Mute Indicator. Indicates soft mute is engaged.
    // Just FM
    inline uint8_t getCurrentStereoBlend() { return currentRqsStatus.resp.STBLEND; };           // Indicates amount of stereo blend in % (100 = full stereo, 0 = full mono).
    inline bool getCurrentPilot() { return currentRqsStatus.resp.PILOT; };                      // Indicates stereo pilot presence.
    inline uint8_t getCurrentMultipath() { return currentRqsStatus.resp.MULT; };                // Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
    inline uint8_t getCurrentSignedFrequencyOffset() { return currentRqsStatus.resp.FREQOFF; }; // Signed frequency offset (kHz).
    inline bool getCurrentMultipathDetectLow() { return currentRqsStatus.resp.MULTLINT; };      // Multipath Detect Low.
    inline bool getCurrentMultipathDetectHigh() { return currentRqsStatus.resp.MULTHINT; };     // Multipath Detect High
    inline bool getCurrentBlendDetectInterrupt() { return currentRqsStatus.resp.BLENDINT; };    // Blend Detect Interrupt

    /*
     * FIRMWARE RESPONSE
     * 
     * See Si47XX PROGRAMMING GUIDE; AN332; page 66
     */
    inline uint8_t
    getFirmwarePN()
    {
        return firmwareInfo.resp.PN;
    };                                                                           //  RESP1 - Part Number (HEX)
    inline uint8_t getFirmwareFWMAJOR() { return firmwareInfo.resp.FWMAJOR; };   // RESP2 - Returns the Firmware Major Revision (ASCII).
    inline uint8_t getFirmwareFWMINOR() { return firmwareInfo.resp.FWMINOR; };   // RESP3 - Returns the Firmware Minor Revision (ASCII).
    inline uint8_t getFirmwarePATCHH() { return firmwareInfo.resp.PATCHH; };     // RESP4 -  Returns the Patch ID High byte (HEX).
    inline uint8_t getFirmwarePATCHL() { return firmwareInfo.resp.PATCHL; };     // RESP5 - Returns the Patch ID Low byte (HEX).
    inline uint8_t getFirmwareCMPMAJOR() { return firmwareInfo.resp.CMPMAJOR; }; // RESP6 -  Returns the Component Major Revision (ASCII).
    inline uint8_t getFirmwareCMPMINOR() { return firmwareInfo.resp.CMPMINOR; }; // RESP7 - Returns the Component Minor Revision (ASCII).
    inline uint8_t getFirmwareCHIPREV() { return firmwareInfo.resp.CHIPREV; };   // RESP8 -  Returns the Chip Revision (ASCII).

    // Volume control
    void setVolume(uint8_t volume);
    uint8_t getVolume();
    void volumeDown();
    void volumeUp();
    inline uint8_t getCurrentVolume() { return volume; }; // Returns the current volume level.

    //
    void setAM();
    void setFM();
    void setAM(uint16_t fromFreq, uint16_t toFreq, uint16_t intialFreq, uint8_t step);
    void setFM(uint16_t fromFreq, uint16_t toFreq, uint16_t initialFreq, uint8_t step);

    void setBandwidth(uint8_t AMCHFLT, uint8_t AMPLFLT);

    void setFrequencyStep(uint8_t step);

    inline void setTuneFrequencyFast(uint8_t FAST) { currentFrequencyParams.arg.FAST = FAST; };         // FAST Tuning.  If set, executes fast and invalidated tune. The tune status will not be accurate
    inline void setTuneFrequencyFreeze(uint8_t FREEZE) { currentFrequencyParams.arg.FREEZE = FREEZE; }; // Onlye FM. Freeze Metrics During Alternate Frequency Jump.
    void setTuneFrequencyAntennaCapacitor(uint16_t capacitor);

    void frequencyUp();
    void frequencyDown();
    bool isCurrentTuneFM();
    void getFirmware(void);

    void setFunction(uint8_t FUNC);
    void seekStation(uint8_t SEEKUP, uint8_t WRAP);
    void seekStationUp();
    void seekStationDown();

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

    // RDS implementation
    void setRdsIntSource(uint8_t RDSNEWBLOCKB, uint8_t RDSNEWBLOCKA, uint8_t RDSSYNCFOUND, uint8_t RDSSYNCLOST, uint8_t RDSRECV);
    void getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY);
    void getRdsStatus();
    inline bool getRdsReceived() { return currentRdsStatus.resp.RDSRECV; };           // 1 = FIFO filled to minimum number of groups
    inline bool getRdsSyncLost() { return currentRdsStatus.resp.RDSSYNCLOST; };       // 1 = Lost RDS synchronization
    inline bool getRdsSyncFound() { return currentRdsStatus.resp.RDSSYNCFOUND; };     // 1 = Found RDS synchronization
    inline bool getRdsNewBlockA() { return currentRdsStatus.resp.RDSNEWBLOCKA; };     // 1 = Valid Block A data has been received.
    inline bool getRdsNewBlockB() { return currentRdsStatus.resp.RDSNEWBLOCKB; };     // 1 = Valid Block B data has been received.
    inline bool getRdsSync() { return currentRdsStatus.resp.RDSSYNC; };               // 1 = RDS currently synchronized.
    inline bool getGroupLost() { return currentRdsStatus.resp.GRPLOST; };             // 1 = One or more RDS groups discarded due to FIFO overrun.
    inline uint8_t getNumRdsFifoUsed() { return currentRdsStatus.resp.RDSFIFOUSED; }; // RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).

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

    char *getRdsTime(void);

    // Test
    char *getNext2Block(char *);
    char *getNext4Block(char *);

    /*
     * SSB 
     */
    void setSSBBfo(int offset);
    void setSSBConfig(uint8_t AUDIOBW, uint8_t SBCUTFLT, uint8_t AVC_DIVIDER, uint8_t AVCEN, uint8_t SMUTESEL, uint8_t DSP_AFCDIS);
    void setSSB(uint16_t fromFreq, uint16_t toFreq, uint16_t intialFreq, uint8_t step, uint8_t usblsb);
    void setSSB(uint8_t usblsb);
    void setSSBAudioBandwidth(uint8_t AUDIOBW);
    void setSSBAutomaticVolumeControl(uint8_t AVCEN);
    void setSBBSidebandCutoffFilter(uint8_t SBCUTFLT);
    void setSSBAvcDivider(uint8_t AVC_DIVIDER);
    void setSSBDspAfc(uint8_t DSP_AFCDIS);
    void setSSBSoftMute(uint8_t SMUTESEL);

    /*
     * SSB PATCH
     */
    si47x_firmware_query_library queryLibraryId();
    void patchPowerUp(); // Used to apply SSB patch on SI4735
    bool downloadPatch(const uint8_t *ssb_patch_content, const uint16_t ssb_patch_content_size);
    bool downloadPatch(int eeprom_i2c_address);
    void ssbPowerUp();

    /* 
     * This functions below modify the clock frequency for I2C communication. 
     * 100KHz  is usually the baseline.
     * Use one of these funcition if you have problem on you default configuration. 
     */
    inline void setI2CLowSpeedMode(void) { Wire.setClock(10000); };
    inline void setI2CStandardMode(void) { Wire.setClock(100000); };
    inline void setI2CFastMode(void) { Wire.setClock(400000); };
};
