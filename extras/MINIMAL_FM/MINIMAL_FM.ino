/*
   Minimal FM radio
   By Ricardo Lima Caratti, Nov 2019
*/

#include <Wire.h>

#define RESET 12            // Connect Arduino Pin 12 or change it to another digital pin
#define SI473X_ADDR   0x11  // SI473X I2C buss address
#define POWER_UP_CMD  0x01  // Power Up command

// See Si47XX PROGRAMMING GUIDE; AN332; page 65.
#define SI473X_ANALOG_AUDIO B00000101  // Analog Audio Inputs

// See Si47XX PROGRAMMING GUIDE; AN332; page 55
#define FM_TUNE_FREQ 0x20

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
   Set the frequency
   BAND = FM_TUNE_FREQ = 0X20
*/
void setFrequency(byte band, unsigned freq)
{
  union {
    struct {
      byte FREQL;     // Tune Frequency High Byte.
      byte FREQH;     // Tune Frequency Low Byte.
    } raw;
    unsigned freq;
  } tune;

  tune.freq = freq;

  Wire.beginTransmission(SI473X_ADDR);
  Wire.write(band);
  Wire.write(0x00);
  Wire.write(tune.raw.FREQH);
  Wire.write(tune.raw.FREQL);
  Wire.write(0x00);
  Wire.endTransmission();

  delayMicroseconds(550);

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
  setFrequency(FM_TUNE_FREQ, 8100);
  delay(1000);

  Serial.println("Try keys 1 to 3 to select a station.");

 }
 
void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
      case '1':
        Serial.println("Trying 103.9 MHz ");
        setFrequency(FM_TUNE_FREQ, 10390); // FM -> 103.9 MHz
        break;
      case '2':
        Serial.println("Trying 106.5 MHz ");
        setFrequency(FM_TUNE_FREQ, 10650); // FM -> 106.5 MHz
        break;
      case '3':
        Serial.println("Trying 95.5 MHz ");
        setFrequency(FM_TUNE_FREQ, 9550); // FM -> 95.5 MHz
        break;
      case '+':
        setVolume(40);
        break;
      case '-':
        setVolume(10);
      case '=':
        setVolume(20);
      case 'p':
        powerUp(&pw);
        break;  
      default:
        break;
    }
  }
  delay(5);
}
