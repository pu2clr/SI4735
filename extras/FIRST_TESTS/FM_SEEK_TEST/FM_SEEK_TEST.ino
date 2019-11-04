/*
 * The purpose of this code is to assist you in setting up the hardware (radio). 
 * It can help you to check if the Si473X wiring is correct during you bulding.
 * 
 * For a real radio, use the Si4735 Arduino Library instead this code. 
 *  
 * By Ricardo Lima Caratti, Nov 2019
 */

#include <Wire.h>

#define RESET 12          // Connect Arduino Pin 12 or change it to another digital pin
#define SI473X_ADDR 0x11  // SI473X I2C buss address
#define POWER_UP_CMD 0x01 // Power Up command

// See Si47XX PROGRAMMING GUIDE; AN332; page 65.
#define SI473X_ANALOG_AUDIO   B00000101  // Analog Audio Inputs
#define SI473X_DIGITAL_AUDIO  B00001011  // Digital audio inputs (DIN/DFS/DCLK)

// See Si47XX PROGRAMMING GUIDE; AN332; page 55
#define FM_TUNE_FREQ    0x20
#define FM_SEEK_START   0x21
#define FM_TUNE_STATUS  0x22

// First argument of Power Up command
// See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 65
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
   See Si47XX PROGRAMMING GUIDE; AN332;
   Reset the SI473X
*/
void resetDevice()
{
  pinMode(RESET, OUTPUT);
  delay(250);
  digitalWrite(RESET, LOW);
  delay(250);
  digitalWrite(RESET, HIGH);
  delay(500);
}

/*
 * Wait for Clear to Send status.
 * See Si47XX PROGRAMMING GUIDE; AN332; page 73.
 */
void wait() {
  do {
    delayMicroseconds(2000);
    Wire.requestFrom(SI473X_ADDR, 0x01);    
  } while ( !(Wire.read() & B10000000) );
}

/*
   Power Up command
   See Si47XX PROGRAMMING GUIDE; AN332;  page 259
   Table 51. Programming Example for the FM/RDS Transmitter
*/
void powerUp(si473x_powerup *powerup_args)
{
  resetDevice();

  // Initiate the I2C bus
  Wire.begin();
  delayMicroseconds(1000);

  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(POWER_UP_CMD);
  Wire.write(powerup_args->raw[0]); // Content of ARG1
  Wire.write(powerup_args->raw[1]); // COntent of ARG2
  Wire.endTransmission();
  // page 12 - Delay at least 500 ms between powerup command and first tune command.
  delayMicroseconds(550);
}

/*
 * See Si47XX PROGRAMMING GUIDE; AN332; page 72
 */
void seekStation(byte up_or_down, byte wrap) {

  union {
    struct {
      byte  RESERVED1: 2;
      byte  WRAP: 1;        // Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
      byte  SEEKUP: 1;      // Determines the direction of the search, either UP = 1, or DOWN = 0.
      byte  RESERVED2: 4;
    } arg;
    byte raw;
  } fm_seek;


  fm_seek.arg.SEEKUP = up_or_down;
  fm_seek.arg.WRAP = wrap;

  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(FM_SEEK_START);
  Wire.write(fm_seek.raw);
  Wire.endTransmission();

  delayMicroseconds(550);

}


/*
 *  Get the current Frequency
 * 
 */
unsigned getFrequency() {

  union {
    struct {
      byte  INTACK: 1;        // If set, clears the seek/tune complete interrupt status indicator.
      byte  CANCEL: 1;        // If set, aborts a seek currently in progress.     
      byte  RESERVED2: 6;
    } arg;
    byte raw;
  } status;

  wait();

  status.arg.INTACK = 0;
  status.arg.CANCEL = 0;

  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(FM_TUNE_STATUS);
  Wire.write(status.raw);
  Wire.endTransmission();

  wait();

  Wire.requestFrom(SI473X_ADDR, 0x4); 
  byte aux_status = Wire.read();
  byte resp1 = Wire.read();
  byte readFreqH = Wire.read();
  byte readFreqL = Wire.read();

  union {
     struct {
          byte freqL;
          byte freqH;
     } raw;

     unsigned currentFreq; 
     
  } freq;

  freq.raw.freqL = readFreqL;
  freq.raw.freqH = readFreqH;

  return freq.currentFreq;
  
}


/*
   Volume contrtol
*/
void setVolume(byte volume)
{
  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(0x00);
  Wire.write(0x40); // CMD1
  Wire.write(0x00); // CMD2
  Wire.write(0x00); // ARG1
  Wire.write(volume);   // ARG2
  Wire.endTransmission();
  delayMicroseconds(550);
}

// Power Up Args
si473x_powerup pw;

void setup()
{
  // RESET the SI473X
  resetDevice();

  Serial.begin(9600);
  Serial.println("The radio should alive in less than two minutes....");

  delay(1000);

  pw.arg.CTSIEN = 1;    // 1 -> Interrupt anabled;
  pw.arg.GPO2OEN = 1;   // 1 -> GPO2 Output Enable;
  pw.arg.PATCH = 0;     // 0 -> Boot normally;
  pw.arg.XOSCEN = 1;    // 1 -> Use external crystal oscillator;
  pw.arg.FUNC = 0;      // 0 = FM Receive.
  pw.arg.OPMODE = SI473X_ANALOG_AUDIO; // 0x5 = 00000101 = Analog audio outputs (LOUT/ROUT).
  powerUp(&pw);
  delay(200);
  help();

  displayFrequency();
  
}

/*
 * Show help
 */
void help() {
  Serial.println("+------------------------");
  Serial.println("Type ");
  Serial.println("S to scan station.");
  Serial.println("+ ou - to Volume Control.");
  Serial.println("+------------------------");
}


/* 
 *  Show the current frequency
 */
void displayFrequency() {
   Serial.print("Current Frequency: ");
   Serial.print(getFrequency());
   Serial.println(" MHz");
}



void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case '+':
        setVolume(20);
        break;
      case '-':
        setVolume(4);
        break;
      case '=':
        setVolume(10);
        break;
      case 'p':
        Serial.println("Power Up");
        powerUp(&pw);
        break;
      case 's':
      case 'S':
        seekStation(1, 1);
        delay(500);
        displayFrequency();
        break;
      default:
        break;
    }
  }
  delay(5);
}
