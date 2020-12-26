
/*
  Program to control the "Adafruit Si5351A Clock Generator" or similar via Arduino.
  This program control the frequencies of two clocks (CLK) output of the Si5351A
  The CLK 0 can be used as a VFO (535kHz to 160MHz)
  The CLK 1 can be used as a BFO (400kHz to 500kHz)
  See on https://github.com/etherkit/Si5351Arduino  and know how to calibrate your Si5351
  See also the example Etherkit/si5251_calibration
  Author: Ricardo Lima Caratti (PU2CLR) -   April, 2019
*/

#include <si5351.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

// Enconder PINs
#define ENCODER_PIN_A 3 // 
#define ENCODER_PIN_B 2 // 

// OLED Diaplay constants
#define I2C_ADDRESS 0x3C
#define RST_PIN -1 // Define proper RST_PIN if required.

// Change this value below  (CORRECTION_FACTOR) to 0 if you do not know the correction factor of your Si5351A.
#define CORRECTION_FACTOR 0 // See how to calibrate your Si5351A (0 if you do not want).

#define BUTTON_STEP 4    // Control the frequency increment and decrement
#define BUTTON_BAND 5    // Controls the band
#define BUTTON_VFO_BFO 6 // Switch VFO to BFO

// BFO range for this project is 400kHz to 500kHz. The central frequency is 455kHz.
#define MAX_BFO 45800000LU    // BFO maximum frequency
#define CENTER_BFO 45500000LU // BFO centeral frequency
#define MIN_BFO 45200000LU    // BFO minimum frequency

#define CD2003GP_SWITCH_AM_FM 10 // Define Arduino Pin 10 to switch AM and FM on CD2003GO based radio (connected to PIN 14 on the CD2003GP)
#define CD2003GP_AM_LED 11  // Indicate that the radio is working in AM mode
#define CD2003GP_FM_LED 12  // Indicate that the radio is working in FM mode

#define STATUS_LED 13 // Arduino status LED Pin 10
#define STATUSLED(ON_OFF) digitalWrite(STATUS_LED, ON_OFF)
#define MIN_ELAPSED_TIME 300

// OLED - Declaration for a SSD1306 display connected to I2C (SDA, SCL pins)
SSD1306AsciiAvrI2c display;

// The Si5351 instance.
Si5351 si5351;

// Callback functions declarations
// Depending on the selected band, you may want to perform specific actions
// The functions declared below will do something for AM and FM BAND. See implementation later.
// You can implement callback function for other bands
void amBroadcast(); // See implementation later.
void fmBroadcast(); // See implementation later.


// Structure for Bands database
typedef struct
{
  char *name;
  uint64_t minFreq; // Min. frequency value for the band (unit 0.01Hz)
  uint64_t maxFreq; // Max. frequency value for the band (unit 0.01Hz)
  long long offset;
  char *unitFreq;         // MHz or kHz
  float divider;          // value that will be the divider of current clock (just to present on display)
  short decimals;         // number of digits after the comma
  short initialStepIndex; // Index to the initial step of incrementing
  short finalStepIndex;   // Index to the final step of incrementing
  short starStepIndex;    // Default Index for the band
  void (*f)(void);        // pointer to the function that handles specific things for the band
} Band;

// Band database. You can change the band ranges if you need.
// The unit of frequency here is 0.01Hz (1/100 Hz). See Etherkit Library at https://github.com/etherkit/Si5351Arduino
Band band[] = {
    {"CLK0", 3276800LLU, 99900000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL}, 
    {"CLK1", 6553600LLU, 1999000000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL},
    {"CLK2", 32768000LLU, 99900000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL},   
    {"CLK2", 327680000LLU, 99900000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL},   
    {"CLK3", 1300000000LLU, 2000000000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL},     
    {"CLK4", 1310720000LLU, 4000000000LLU, 0LU, "Hz", 100.0f, 0, 0, 9, 0, NULL}};

// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
short currentBand = 0;                                 // First band. For this case, AM is the current band.

// Struct for step database
typedef struct
{
  char *name; // step label: 50Hz, 100Hz, 500Hz, 1kHz, 5kHz, 10kHz and 500kHz
  long value; // Frequency value (unit 0.01Hz See documentation) to increment or decrement
} Step;

// Steps database. You can change the Steps and numbers of steps here if you need.
Step step[] = {
    {"1Hz", 100 },
    {"10Hz  ", 1000},
    {"100Hz ", 10000},
    {"500Hz ", 50000},
    {"1kHz  ", 100000},
    {"5kHz  ", 500000},
    {"10kHz  ",1000000},
    {"50kHz ", 5000000},
    {"100kHz", 10000000},
    {"500kHz", 50000000}};

// Calculate the index of last position of step[] array (in this case will be 8)
const short lastStepVFO = (sizeof step / sizeof(Step)) - 1; // index for max increment / decrement for VFO
short lastStepBFO = 3;                                      // index for max. increment / decrement for BFO. In this case will be is 1kHz
uint64_t bfoFreq = CENTER_BFO;                              // 455 kHz for this project

boolean isFreqChanged = true;
boolean clearDisplay = false;

// LW/MW is the default band
uint64_t vfoFreq = band[currentBand].minFreq;        // VFO starts on MW
short currentStep = band[currentBand].starStepIndex; // Step starts on default MW

// VFO is the Si5351A CLK0
// BFO is the Si5351A CLK1
short currentClock = 0; // If 0, then VFO will be controlled else the BFO will be

long elapsedButton = millis(); // will control the minimum time to process an interrupt action
long elapsedTimeEncoder = millis();

// Encoder variable control
unsigned char encoder_pin_a;
unsigned char encoder_prev = 0;
unsigned char encoder_pin_b;

void setup()
{
  // LED Pin
  pinMode(STATUS_LED, OUTPUT);
  pinMode(CD2003GP_AM_LED, OUTPUT);
  pinMode(CD2003GP_FM_LED, OUTPUT);

  // CD2003GP AM and FM controll switch
  pinMode(CD2003GP_SWITCH_AM_FM, OUTPUT);

  // Encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  // Si5351 contrtolers pins
  pinMode(BUTTON_BAND, INPUT);
  pinMode(BUTTON_STEP, INPUT);
  pinMode(BUTTON_VFO_BFO, INPUT);


  // The sistem is alive
  blinkLed(STATUS_LED, 100);
  STATUSLED(LOW);
  // Initiating the OLED Display
  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(System5x7);
  display.set2X();
  display.clear();
  display.print("\n PU2CLR");
  delay(3000);
  display.clear();
  displayDial();
  // Initiating the Signal Generator (si5351)
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  // Adjusting the frequency (see how to calibrate the Si5351 - example si5351_calibration.ino)
  si5351.set_correction(CORRECTION_FACTOR, SI5351_PLL_INPUT_XO);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(vfoFreq - band[currentBand].offset, SI5351_CLK0); // Start CLK0 (VFO)

  // To see later
  // si5351.set_freq(bfoFreq, SI5351_CLK1);
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.output_enable(SI5351_CLK2, 0);

  si5351.update_status();
  // Show the initial system information

  // Set defult Band
  currentBand = 0;
  vfoFreq = band[currentBand].minFreq;
  currentStep = band[currentBand].starStepIndex;
  if (band[currentBand].f != NULL) // Call callback function if exist something to do for the specific for the band
    (band[currentBand].f)();
  isFreqChanged = true;

  delay(100);
}

// Blink the STATUS LED
void blinkLed(int pinLed, int blinkDelay)
{
  for (int i = 0; i < 5; i++)
  {
    STATUSLED(HIGH);
    delay(blinkDelay);
    STATUSLED(LOW);
    delay(blinkDelay);
  }
}

// Show Signal Generator Information
// Verificar setCursor() em https://github.com/greiman/SSD1306Ascii/issues/53
void displayDial()
{
  double vfo = (double)vfoFreq / band[currentBand].divider;
  double bfo = (double)bfoFreq / 100000.0f;
  String mainFreq;
  String secoundFreq;
  String staticFreq;
  String dinamicFreq;
  String strAux;

  // Change the display behaviour depending on who is controlled, BFO or BFO.
  if (currentClock == 0)
  { // If the encoder is controlling the VFO
    mainFreq = String(vfo, band[currentBand].decimals);
    secoundFreq = String(bfo, 2);
    staticFreq = "BFO";
    dinamicFreq = "VFO";
  }
  else // encoder is controlling the VFO
  {
    mainFreq = String(bfo, 2);
    secoundFreq = String(vfo, band[currentBand].decimals);
    secoundFreq.concat(band[currentBand].unitFreq);
    staticFreq = "VFO";
    dinamicFreq = "BFO";
  }

  // Show Band information
  display.setCursor(0, 0);
  display.set1X();
  strAux = String(band[currentBand].name);
  display.print(strAux);
  strAux = band[currentBand].unitFreq;
  display.setCursor(29, 0);
  display.print(strAux);
  display.setCursor(55, 0);
  display.print(dinamicFreq);
  display.setCursor(80, 0);
  strAux = "S:" + String(step[currentStep].name);
  display.print(strAux);

  // Show main frequency (VFO or BFO)
  display.set2X();
  display.setCursor(0, 3);

  strAux = mainFreq + "     ";
  display.print(strAux);

  display.setCursor(0, 7);
  display.set1X();
  // Show VFO or BFO frequency
  strAux = staticFreq + ": " + secoundFreq;
  display.print(strAux);
}

// Change the frequency (increment or decrement)
// direction parameter is 1 (clockwise) or -1 (counter-clockwise)
void changeFreq(int direction)
{
  if (currentClock == 0)
  { // Check who will be chenged (VFO or BFO)
    vfoFreq += step[currentStep].value * direction;
    // Check the VFO limits
    if (vfoFreq > band[currentBand].maxFreq) // Max. VFO frequency for the current band
    {
      vfoFreq = band[currentBand].minFreq; // Go to min. frequency of the range
      blinkLed(STATUS_LED, 50);            // Alert the user that the range is over
    }
    else if (vfoFreq < band[currentBand].minFreq) // Min. VFO frequency for the band
    {
      vfoFreq = band[currentBand].maxFreq; // Go to max. frequency of the range
      blinkLed(STATUS_LED, 50);            // Alert the user that the range is over
    }
  }
  else
  {
    bfoFreq += step[currentStep].value * direction; // currentStep * direction;
    // Check the BFO limits
    if (bfoFreq > MAX_BFO || bfoFreq < MIN_BFO) // BFO goes to center if it is out of the limits
    {
      bfoFreq = CENTER_BFO;     // Go to center
      blinkLed(STATUS_LED, 50); // Alert the user that the range is over
    }
  }
  isFreqChanged = true;
}

// Callback implementation

// Doing something spefict for AM
// Example: set Pin 14 of the CD2003GP to LOW; switch filter, turn AM LED on etc
void amBroadcast()
{
  // TO DO
  digitalWrite(CD2003GP_SWITCH_AM_FM,LOW); // The CD2003GP is seted to AM
  digitalWrite(CD2003GP_AM_LED, HIGH);     // Turn ON the AM LED
  digitalWrite(CD2003GP_FM_LED, LOW);      // Turn OFF the FM LED

}

// Doing something spefict for FM
// Example: set Pin 14 of the CD2003GP to HIGH; turn FM LED on etc
void fmBroadcast()
{
  // TO DO
  digitalWrite(CD2003GP_SWITCH_AM_FM, HIGH); // The CD2003GP is seted to AM
  digitalWrite(CD2003GP_AM_LED, LOW);      // Turn OFF the AM LED
  digitalWrite(CD2003GP_FM_LED, HIGH);       // Turn ON the FM LED
}


// main loop
void loop()
{
  // Enconder action can be processed after 5 milisecounds
  if ((millis() - elapsedTimeEncoder) > 5)
  {
    encoder_pin_a = digitalRead(ENCODER_PIN_A);
    encoder_pin_b = digitalRead(ENCODER_PIN_B);
    if ((!encoder_pin_a) && (encoder_prev)) // has ENCODER_PIN_A gone from high to low?
    {                                       // if so,  check ENCODER_PIN_B. It is high then clockwise (1) else counter-clockwise (-1)
      changeFreq(((encoder_pin_b) ? 1 : -1));
    }
    encoder_prev = encoder_pin_a;
    elapsedTimeEncoder = millis(); // keep elapsedTimeEncoder updated
  }
  // check if some action changed the frequency
  if (isFreqChanged)
  {
    if (currentClock == 0)
    {
      si5351.set_freq(vfoFreq - band[currentBand].offset, SI5351_CLK0);
    }
    else
    {
      si5351.set_freq(bfoFreq, SI5351_CLK1);
    }
    isFreqChanged = false;
    displayDial();
  }

  // check if some button is pressed
  if (digitalRead(BUTTON_BAND) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    currentBand = (currentBand < lastBand) ? (currentBand + 1) : 0; // Is the last band? If so, go to the first band (AM). Else. Else, next band.
    vfoFreq = band[currentBand].minFreq;
    currentStep = band[currentBand].starStepIndex;

    // Call callback function if exist something to do for the specific band
    if (band[currentBand].f != NULL)
      (band[currentBand].f)();

    isFreqChanged = true;
    elapsedButton = millis();
  }
  else if (digitalRead(BUTTON_STEP) == HIGH && (millis() - elapsedButton) > MIN_ELAPSED_TIME)
  {
    if (currentClock == 0)                                                                                                     // Is VFO
      currentStep = (currentStep < band[currentBand].finalStepIndex) ? (currentStep + 1) : band[currentBand].initialStepIndex; // Increment the step or go back to the first
    else                                                                                                                       // Is BFO
      currentStep = (currentStep < lastStepBFO) ? (currentStep + 1) : 0;
    clearDisplay = true;
    elapsedButton = millis();
  }
  else if (digitalRead(BUTTON_VFO_BFO) && (millis() - elapsedButton) > MIN_ELAPSED_TIME == HIGH)
  {
    currentClock = !currentClock;
    currentStep = (currentClock == 0) ? band[currentBand].starStepIndex : 0;
    clearDisplay = true;
    elapsedButton = millis();
  }

  if (clearDisplay)
  {
    display.clear();
    displayDial();
    clearDisplay = false;
  }
}
