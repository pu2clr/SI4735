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
        byte FUNC : 4;    // Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
        byte XOSCEN : 1;  // Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
        byte PATCH : 1;   // Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
        byte GPO2OEN : 1; // GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
        byte CTSIEN : 1;  // CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).
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
 * 
 */
typedef union {
    struct
    {
        byte ANTCAPL; // Antenna Tuning Capacitor High Byte
        byte ANTCAPH; // Antenna Tuning Capacitor Low Byte
    } raw;
    unsigned value;
} si47x_antenna_capacitor;

/*
 * AM_TUNE_FREQ data type command
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 135
 */
typedef union {
    struct
    {
        byte FAST : 1;   // ARG1 - FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.
        byte FREEZE : 1; // Valid only for FM (Must be 0 to AM)
        byte DUMMY1 : 4; // Always set 0
        byte USBLSB : 2; // SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection. 10 = USB is selected; 01 = LSB is selected.
        byte FREQH;      // ARG2 - Tune Frequency High Byte.
        byte FREQL;      // ARG3 - Tune Frequency Low Byte.
        byte ANTCAPH;    // ARG4 - Antenna Tuning Capacitor High Byte.
        byte ANTCAPL;    // ARG5 - Antenna Tuning Capacitor Low Byte. Note used for FM.
    } arg;
    byte raw[5];
} si47x_set_frequency;

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
 * Response status command 
 * See Si47XX PROGRAMMING GUIDE; pages 73 and 
 */
typedef union {
    struct
    {
        // Status
        byte STCINT : 1; // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
        byte DUMMY1 : 1;
        byte RDSINT : 1; // Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
        byte RSQINT : 1; // Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
        byte DUMMY2 : 2;
        byte ERR : 1; // Error. 0 = No error 1 = Error
        byte CTS : 1; // Clear to Send.
        // RESP1
        byte VALID : 1; // Valid Channel
        byte AFCRL : 1; // AFC Rail Indicator
        byte DUMMY3 : 5;
        byte BLTF : 1; // Reports if a seek hit the band limit
        // RESP2
        byte READFREQH; // Read Frequency High Byte.
        // RESP3
        byte READFREQL; // Read Frequency Low Byte.
        // RESP4
        byte RSSI; // Received Signal Strength Indicator (dBμV)
        // RESP5
        byte SNR; // This byte contains the SNR metric when tune is complete (dB).
        // RESP6
        byte MULT; // Contains the multipath metric when tune is complete
        // RESP7
        byte READANTCAP; // Contains the current antenna tuning capacitor value
    } resp;
    byte raw[7];
} si47x_response_status;

/*
 * Firmware Information (GET_ENV)
 */
typedef union {
    struct
    {
        // status ("RESP0")
        byte STCINT : 1;
        byte DUMMY1 : 1;
        byte RDSINT : 1;
        byte RSQINT : 1;
        byte DUMMY2 : 2;
        byte ERR : 1;
        byte CTS : 1;
        byte PN;       // RESP1 - Final 2 digits of Part Number (HEX).
        byte FWMAJOR;  // RESP2 - Firmware Major Revision (ASCII).
        byte FWMINOR;  // RESP3 - Firmware Minor Revision (ASCII).
        byte PATCHH;   // RESP4 - Patch ID High Byte (HEX).
        byte PATCHL;   // RESP5 - Patch ID Low Byte (HEX).
        byte CMPMAJOR; // RESP6 - Component Major Revision (ASCII).
        byte CMPMINOR; // RESP7 - Component Minor Revision (ASCII).
        byte CHIPREV;  // RESP8 - Chip Revision (ASCII).
        // RESP9 to RESP15 not used
    } resp;
    byte raw[9];
} si47x_firmware_information;

/*
 * Firmware Query Library ID response. 
 * Used to represent the response of a power up command with FUNC = 15
 */
typedef union {
    struct
    {
        // status ("RESP0")
        byte STCINT : 1;
        byte DUMMY1 : 1;
        byte RDSINT : 1;
        byte RSQINT : 1;
        byte DUMMY2 : 2;
        byte ERR : 1;
        byte CTS : 1;
        byte PN;        // RESP1 - Final 2 digits of Part Number (HEX).
        byte FWMAJOR;   // RESP2 - Firmware Major Revision (ASCII).
        byte FWMINOR;   // RESP3 - Firmware Minor Revision (ASCII).
        byte RESERVED1; // RESP4 - Reserved, various values.
        byte RESERVED2; // RESP5 - Reserved, various values.
        byte CHIPREV;   // RESP6 - Chip Revision (ASCII).
        byte LIBRARYID; // RESP7 - Library Revision (HEX).
        // RESP9 to RESP15 not used
    } resp;
    byte raw[8];
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
        byte INTACK : 1; // If set, clears the seek/tune complete interrupt status indicator.
        byte CANCEL : 1; // If set, aborts a seek currently in progress.
        byte RESERVED2 : 6;
    } arg;
    byte raw;
} si47x_tune_status;

/*
 * Property Data type (help to deal with SET_PROPERTY command on si473X)
 */
typedef union {
    struct
    {
        byte byteLow;
        byte byteHigh;
    } raw;
    unsigned value;
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
        byte STCINT : 1;
        byte DUMMY1 : 1;
        byte RDSINT : 1;
        byte RSQINT : 1;
        byte DUMMY2 : 2;
        byte ERR : 1;
        byte CTS : 1;
        // RESP1
        byte RSSIILINT : 1; // RSSI Detect Low.
        byte RSSIHINT : 1;  // RSSI Detect High.
        byte SNRLINT : 1;   // SNR Detect Low.
        byte SNRHINT : 1;   // SNR Detect High.
        byte MULTLINT : 1;  // Multipath Detect Low
        byte MULTHINT : 1;  // Multipath Detect High
        byte DUMMY3 : 1;
        byte BLENDINT : 1; // Blend Detect Interrupt.
        // RESP2
        byte VALID : 1; // Valid Channel.
        byte AFCRL : 1; // AFC Rail Indicator.
        byte DUMMY4 : 1;
        byte SMUTE : 1; // Soft Mute Indicator. Indicates soft mute is engaged.
        byte DUMMY5 : 4;
        // RESP3
        byte STBLEND : 7; // Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).
        byte PILOT : 1;   // Indicates stereo pilot presence.
        // RESP4 to RESP7
        byte RSSI;    // RESP4 - Contains the current receive signal strength (0–127 dBμV).
        byte SNR;     // RESP5 - Contains the current SNR metric (0–127 dB).
        byte MULT;    // RESP6 - Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
        byte FREQOFF; // RESP7 - Signed frequency offset (kHz).
    } resp;
    byte raw[8];
} si47x_rqs_status;

/*
 * FM_RDS_STATUS (0x24) command
 * Data type for command and response information 
 * See Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78
 */

// Command data type
typedef union {
    struct
    {
        byte INTACK : 1;     // Interrupt Acknowledge; 0 = RDSINT status preserved; 1 = Clears RDSINT.
        byte MTFIFO : 1;     // Empty FIFO; 0 = If FIFO not empty; 1 = Clear RDS Receive FIFO.
        byte STATUSONLY : 1; // Determines if data should be removed from the RDS FIFO.
        byte dummy : 5;
    } arg;
    byte raw;
} si47x_rds_command;

// Response data type for current channel and reads an entry from the RDS FIFO.
typedef union {
    struct
    {
        // status ("RESP0")
        byte STCINT : 1;
        byte DUMMY1 : 1;
        byte RDSINT : 1;
        byte RSQINT : 1;
        byte DUMMY2 : 2;
        byte ERR : 1;
        byte CTS : 1;
        // RESP1
        byte RDSRECV : 1;      // RDS Received; 1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.
        byte RDSSYNCLOST : 1;  // RDS Sync Lost; 1 = Lost RDS synchronization.
        byte RDSSYNCFOUND : 1; // RDS Sync Found; 1 = Found RDS synchronization.
        byte DUMMY3 : 1;
        byte RDSNEWBLOCKA : 1; // RDS New Block A; 1 = Valid Block A data has been received.
        byte RDSNEWBLOCKB : 1; // RDS New Block B; 1 = Valid Block B data has been received.
        byte DUMMY4 : 2;
        // RESP2
        byte RDSSYNC : 1; // RDS Sync; 1 = RDS currently synchronized.
        byte DUMMY5 : 1;
        byte GRPLOST : 1; // Group Lost; 1 = One or more RDS groups discarded due to FIFO overrun.
        byte DUMMY6 : 5;
        // RESP3 to RESP11
        byte RDSFIFOUSED; // RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).
        byte BLOCKAH;     // RESP4 - RDS Block A; HIGH byte
        byte BLOCKAL;     // RESP5 - RDS Block A; LOW byte
        byte BLOCKBH;     // RESP6 - RDS Block B; HIGH byte
        byte BLOCKBL;     // RESP7 - RDS Block B; LOW byte
        byte BLOCKCH;     // RESP8 - RDS Block C; HIGH byte
        byte BLOCKCL;     // RESP9 - RDS Block C; LOW byte
        byte BLOCKDH;     // RESP10 - RDS Block D; HIGH byte
        byte BLOCKDL;     // RESP11 - RDS Block D; LOW byte
        // RESP12 - Blocks A to D Corrected Errors.
        // 0 = No errors;
        // 1 = 1–2 bit errors detected and corrected;
        // 2 = 3–5 bit errors detected and corrected.
        // 3 = Uncorrectable.
        byte BLED : 2;
        byte BLEC : 2;
        byte BLEB : 2;
        byte BLEA : 2;
    } resp;
    byte raw[13];
} si47x_rds_status;

/*
 * FM_RDS_INT_SOURCE property data type
 * See Si47XX PROGRAMMING GUIDE; AN332; page 103
 */
typedef union {
    struct
    {
        byte RDSRECV : 1;      // If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
        byte RDSSYNCLOST : 1;  // If set, generate RDSINT when RDS loses synchronization.
        byte RDSSYNCFOUND : 1; // f set, generate RDSINT when RDS gains synchronization.
        byte DUMMY1 : 1;       // Always write to 0.
        byte RDSNEWBLOCKA : 1; // If set, generate an interrupt when Block A data is found or subsequently changed
        byte RDSNEWBLOCKB : 1; // If set, generate an interrupt when Block B data is found or subsequently changed
        byte DUMMY2 : 5;       // Reserved - Always write to 0.
        byte DUMMY3 : 5;       // Reserved - Always write to 0.
    } refined;
    byte raw[2];
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
        byte RDSEN : 1; // 1 = RDS Processing Enable.
        byte DUMMY1 : 7;
        byte BLETHD : 2; // Block Error Threshold BLOCKD
        byte BLETHC : 2; // Block Error Threshold BLOCKC.
        byte BLETHB : 2; // Block Error Threshold BLOCKB.
        byte BLETHA : 2; // Block Error Threshold BLOCKA.
    } arg;
    byte raw[2];
} si47x_rds_config;

/*
 * Block A data type
 */
typedef union {
    struct
    {
        unsigned pi;
    } refined;
    struct
    {
        byte lowValue;
        byte highValue; // Most Significant byte first
    } raw;
} si47x_rds_blocka;

/*
 * Block B data type
 * More about Group Type Contents see: https://github.com/pu2clr/SI4735/tree/master/examples/SI4735_RDS
 * See also Si47XX PROGRAMMING GUIDE; AN332; pages 78 and 79
 */
typedef union {
    struct
    {
        byte content : 5;            // Depends on Group Type and Version codes.
        byte programType : 5;        // PTY (Program Type) code
        byte trafficProgramCode : 1; // 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        byte versionCode : 1;        // 0=A; 1=B
        byte groupType : 4;          // Group Type code.

    } refined;
    struct
    {
        byte lowValue;
        byte highValue; // Most Significant Byte first
    } raw;
} si47x_rds_blockb;

typedef union {
    struct
    {
        byte offset : 5;
        byte offset_sense : 1; //
        byte minute : 6;       //
        byte hour : 4;         //
        unsigned mjd;
    } refined;
    byte raw[4];
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
        byte STCINT : 1;
        byte DUMMY1 : 1;
        byte RDSINT : 1; // Not used for AM/SSB
        byte RSQINT : 1;
        byte DUMMY2 : 2;
        byte ERR : 1;
        byte CTS : 1;
        // RESP1
        byte AGCDIS : 1; // This bit indicates if the AGC is enabled or disabled. 0 = AGC enabled; 1 = AGC disabled.
        byte DUMMY : 7;
        // RESP2
        byte AGCDX; // For FM (5 bits - READ_LNA_GAIN_INDEX - 0 = Minimum attenuation (max gain)). For AM (8 bits). This byte reports the current AGC gain index.
    } refined;
    byte raw[3];
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
        byte AGCDIS : 1; // if set to 1 indicates if the AGC is disabled. 0 = AGC enabled; 1 = AGC disabled.
        byte DUMMY : 7;
        // ARG2
        byte AGCDX; // AGC Index; If AMAGCDIS = 1, this byte forces the AGC gain index; 0 = Minimum attenuation (max gain)
    } arg;
    byte raw[2];
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
        byte AMCHFLT : 4; // Selects the bandwidth of the AM channel filter.
        byte DUMMY1 : 4;
        byte AMPLFLT : 1; // Enables the AM Power Line Noise Rejection Filter.
        byte DUMMY2 : 7;
    } param;
    byte raw[2];
} si47x_bandwidth_config; // AM_CHANNEL_FILTER

/* 
 * SSB - datatype for SSB_MODE (property 0x0101)
 * See AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 
 */
typedef union {
    struct
    {
        byte AUDIOBW : 4;     // 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz
        byte SBCUTFLT : 4;    // SSB side band cutoff filter for band passand low pass filter
        byte AVC_DIVIDER : 4; // set 0 for SSB mode; set 3 for SYNC mode;
        byte AVCEN : 1;       // SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default);
        byte SMUTESEL : 1;    // SSB Soft-mute Based on RSSI or SNR
        byte DUMMY1 : 1;      // Always write 0;
        byte DSP_AFCDIS : 1;  // 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.
    } param;
    byte raw[2];
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
    char rds_buffer[65];
    byte resetPin;
    byte interruptPin;

    byte currentTune;

    unsigned currentMinimumFrequency;
    unsigned currentMaximumFrequency;
    unsigned currentWorkFrequency;

    byte currentStep;

    si47x_frequency currentFrequency;
    si47x_set_frequency currentFrequencyParams;
    si47x_rqs_status currentRqsStatus;
    si47x_response_status currentStatus;
    si47x_firmware_information firmwareInfo;
    si47x_rds_status currentRdsStatus;
    si47x_agc_status currentAgcStatus;
    si47x_ssb_mode currentSSBMode;

    si473x_powerup powerUp;

    byte volume = 32;

    byte currentSsbStatus;

    void reset(void);
    void waitInterrupr(void);
    void sendSSBModeProperty(); // Sends SSB_MODE property to the device.

public:
    SI4735();
    void waitToSend(void); // Wait for Si4735 device ready to receive command
    void setup(byte resetPin, byte defaultFunction);
    void setup(byte resetPin, int interruptPin, byte defaultFunction);
    void setPowerUp(byte CTSIEN, byte GPO2OEN, byte PATCH, byte XOSCEN, byte FUNC, byte OPMODE);
    void analogPowerUp(void);
    void powerDown(void);

    void setFrequency(unsigned);

    // getStatus
    void getStatus();
    void getStatus(byte, byte);

    // Status response
    unsigned getFrequency(void);
    unsigned getCurrentFrequency(); // See documentation 

    /* STATUS RESPONSE
     * Set of methods to get current status information. Call them after getStatus or getFrequency or seekStation
     * See Si47XX PROGRAMMING GUIDE; AN332; pages 63
     */
        inline bool getSignalQualityInterrupt() { return currentStatus.resp.RSQINT; };        // Gets Received Signal Quality Interrupt(RSQINT)
        inline bool getRadioDataSystemInterrupt() { return currentStatus.resp.RDSINT; };      // Gets Radio Data System (RDS) Interrupt
        inline bool getTuneCompleteTriggered() { return currentStatus.resp.STCINT; };         // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
        inline bool getStatusError() { return currentStatus.resp.ERR; };                      // Return the Error flag (true or false) of status of the least Tune or Seek
        inline bool getStatusCTS() { return currentStatus.resp.CTS; };                        // Gets the Error flag of status response
        inline bool getACFIndicator() { return currentStatus.resp.AFCRL; };                   // Returns true if the AFC rails (AFC Rail Indicator).
        inline bool getBandLimit() { return currentStatus.resp.BLTF; };                       // Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or wrapped to the original frequency(WRAP = 1).
        inline bool getStatusValid() { return currentStatus.resp.VALID; };                    // eturns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108)
        inline byte getReceivedSignalStrengthIndicator() { return currentStatus.resp.RSSI; }; // Returns integer Received Signal Strength Indicator (dBμV).
        inline byte getStatusSNR() { return currentStatus.resp.SNR; };                        // returns integer containing the SNR metric when tune is complete (dB).
        inline byte getStatusMULT() { return currentStatus.resp.MULT; };                      // Returns integer containing the multipath metric when tune is complete.
        inline byte getAntennaTuningCapacitor() { return currentStatus.resp.READANTCAP; };    // Returns integer containing the current antenna tuning capacitor value.

        void getAutomaticGainControl();

        inline bool isAgcEnabled() { return !currentAgcStatus.refined.AGCDIS; };  // Returns true if the AGC is enabled
        inline byte getAgcGainIndex() { return currentAgcStatus.refined.AGCDX; }; // Returns the current AGC gain index.
        void setAutomaticGainControl(byte AGCDIS, byte AGCDX);                    // Overrides the AGC setting

        /* RQS STATUS RESPONSE 
     * 
     */
        void getCurrentReceivedSignalQuality(byte INTACK);
        // AM and FM
        inline byte getCurrentRSSI() { return currentRqsStatus.resp.RSSI; };               // current receive signal strength (0–127 dBμV).
        inline byte getCurrentSNR() { return currentRqsStatus.resp.SNR; };                 // current SNR metric (0–127 dB).
        inline bool getCurrentRssiDetectLow() { return currentRqsStatus.resp.RSSIILINT; }; // RSSI Detect Low.
        inline bool getCurrentRssiDetectHigh() { return currentRqsStatus.resp.RSSIHINT; }; // RSSI Detect High
        inline bool getCurrentSnrDetectLow() { return currentRqsStatus.resp.SNRLINT; };    // SNR Detect Low.
        inline bool getCurrentSnrDetectHigh() { return currentRqsStatus.resp.SNRHINT; };   // SNR Detect High
        inline bool getCurrentValidChannel() { return currentRqsStatus.resp.VALID; };      // Valid Channel.
        inline bool getCurrentAfcRailIndicator() { return currentRqsStatus.resp.AFCRL; };  // AFC Rail Indicator.
        inline bool getCurrentSoftMuteIndicator() { return currentRqsStatus.resp.SMUTE; }; // Soft Mute Indicator. Indicates soft mute is engaged.
        // Just FM
        inline bool getCurrentStereoBlend() { return currentRqsStatus.resp.STBLEND; };           // Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).
        inline bool getCurrentPilot() { return currentRqsStatus.resp.PILOT; };                   // Indicates stereo pilot presence.
        inline byte getCurrentMultipath() { return currentRqsStatus.resp.MULT; };                // Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
        inline byte getCurrentSignedFrequencyOffset() { return currentRqsStatus.resp.FREQOFF; }; // Signed frequency offset (kHz).
        inline bool getCurrentMultipathDetectLow() { return currentRqsStatus.resp.MULTLINT; };   // Multipath Detect Low.
        inline bool getCurrentMultipathDetectHigh() { return currentRqsStatus.resp.MULTHINT; };  // Multipath Detect High
        inline bool getCurrentBlendDetectInterrupt() { return currentRqsStatus.resp.BLENDINT; }; // Blend Detect Interrupt

        /*
     * FIRMWARE RESPONSE
     * 
     * See Si47XX PROGRAMMING GUIDE; AN332; page 66
     */
        inline byte
        getFirmwarePN()
        {
            return firmwareInfo.resp.PN;
        };                                                                        //  RESP1 - Part Number (HEX)
        inline byte getFirmwareFWMAJOR() { return firmwareInfo.resp.FWMAJOR; };   // RESP2 - Returns the Firmware Major Revision (ASCII).
        inline byte getFirmwareFWMINOR() { return firmwareInfo.resp.FWMINOR; };   // RESP3 - Returns the Firmware Minor Revision (ASCII).
        inline byte getFirmwarePATCHH() { return firmwareInfo.resp.PATCHH; };     // RESP4 -  Returns the Patch ID High Byte (HEX).
        inline byte getFirmwarePATCHL() { return firmwareInfo.resp.PATCHL; };     // RESP5 - Returns the Patch ID Low Byte (HEX).
        inline byte getFirmwareCMPMAJOR() { return firmwareInfo.resp.CMPMAJOR; }; // RESP6 -  Returns the Component Major Revision (ASCII).
        inline byte getFirmwareCMPMINOR() { return firmwareInfo.resp.CMPMINOR; }; // RESP7 - Returns the Component Minor Revision (ASCII).
        inline byte getFirmwareCHIPREV() { return firmwareInfo.resp.CHIPREV; };   // RESP8 -  Returns the Chip Revision (ASCII).

        // Volume control
        void setVolume(byte volume);
        byte getVolume();
        void volumeDown();
        void volumeUp();
        inline byte getCurrentVolume() { return volume; }; // Returns the current volume level.

        //
        void setAM();
        void setFM();
        void setAM(unsigned fromFreq, unsigned toFreq, unsigned intialFreq, byte step);
        void setFM(unsigned fromFreq, unsigned toFreq, unsigned initialFreq, byte step);

        void setBandwidth(byte AMCHFLT, byte AMPLFLT);

        void setFrequencyStep(byte step);

        inline void setTuneFrequencyFast(byte FAST) { currentFrequencyParams.arg.FAST = FAST; };         // FAST Tuning.  If set, executes fast and invalidated tune. The tune status will not be accurate
        inline void setTuneFrequencyFreeze(byte FREEZE) { currentFrequencyParams.arg.FREEZE = FREEZE; }; // Onlye FM. Freeze Metrics During Alternate Frequency Jump.
        void setTuneFrequencyAntennaCapacitor(unsigned capacitor);

        void frequencyUp();
        void frequencyDown();
        bool isCurrentTuneFM();
        void getFirmware(void);

        void setFunction(byte FUNC);
        void seekStation(byte SEEKUP, byte WRAP);
        void seekStationUp();
        void seekStationDown();

        // RDS implementation
        void setRdsIntSource(byte RDSNEWBLOCKB, byte RDSNEWBLOCKA, byte RDSSYNCFOUND, byte RDSSYNCLOST, byte RDSRECV);
        void getRdsStatus(byte INTACK, byte MTFIFO, byte STATUSONLY);
        void getRdsStatus();
        inline bool getRdsReceived() { return currentRdsStatus.resp.RDSRECV; };        // 1 = FIFO filled to minimum number of groups
        inline bool getRdsSyncLost() { return currentRdsStatus.resp.RDSSYNCLOST; };    // 1 = Lost RDS synchronization
        inline bool getRdsSyncFound() { return currentRdsStatus.resp.RDSSYNCFOUND; };  // 1 = Found RDS synchronization
        inline bool getRdsNewBlockA() { return currentRdsStatus.resp.RDSNEWBLOCKA; };  // 1 = Valid Block A data has been received.
        inline bool getRdsNewBlockB() { return currentRdsStatus.resp.RDSNEWBLOCKB; };  // 1 = Valid Block B data has been received.
        inline bool getRdsSync() { return currentRdsStatus.resp.RDSSYNC; };            // 1 = RDS currently synchronized.
        inline bool getGroupLost() { return currentRdsStatus.resp.GRPLOST; };          // 1 = One or more RDS groups discarded due to FIFO overrun.
        inline byte getNumRdsFifoUsed() { return currentRdsStatus.resp.RDSFIFOUSED; }; // // RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).

        void setRdsConfig(byte RDSEN, byte BLETHA, byte BLETHB, byte BLETHC, byte BLETHD);
        unsigned getRdsPI(void);
        unsigned getRdsGroupType(void);
        unsigned getRdsVersionCode(void);
        unsigned getRdsProgramType(void);
        String getRdsText(void);
        String getRdsTime(void);

        // Test
        char *getNext2Block(char *c);
        char *getNext4Block(char *);

        /*
     * SSB 
     */
        void setSSBBfo(int offset);
        void setSSBConfig(byte AUDIOBW, byte SBCUTFLT, byte AVC_DIVIDER, byte AVCEN, byte SMUTESEL, byte DSP_AFCDIS);
        void setSSB(unsigned fromFreq, unsigned toFreq, unsigned intialFreq, byte step, byte usblsb);
        void setSSB(byte usblsb);
        void setSSBAudioBandwidth(byte AUDIOBW);
        void setSSBAutomaticVolumeControl(byte AVCEN);
        void setSBBSidebandCutoffFilter(byte SBCUTFLT);
        void setSSBAvcDivider(byte AVC_DIVIDER);
        void setSSBDspAfc(byte DSP_AFCDIS);
        void setSSBSoftMute(byte SMUTESEL);

        /*
     * SSB PATCH
     */
        si47x_firmware_query_library queryLibraryId();
    void patchPowerUp(); // Used to apply SSB patch on SI4735
    bool downloadPatch(byte *ssb_patch_content, unsigned ssb_patch_content_size);
    bool downloadPatch(byte eeprom_i2c_address);
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
