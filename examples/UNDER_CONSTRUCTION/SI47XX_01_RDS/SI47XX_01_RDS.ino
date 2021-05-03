/*
  Under development...

  This sketch is an example of using the RDS functions impplemented byte the Si4735 Library for Arduino.

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti
*/

#include <SI4735.h>

#define RESET_PIN 12

#define FM_FUNCTION 0
#define ELAPSED_TIME 400

#define GPIO2_PIN 2 // Arduino PIN A2 is connected to the SI473X GPIO2
#define SI473X_INTERRUPT_ENABLE 1
#define GPIO2_ENABLE 1

long rdsElapsedTime = millis();

const unsigned min_fm = 8400;
const unsigned max_fm = 10900;

// Sets some local FM Stations with RDS service
uint16_t rdsStations[] = {10650, 10570, 9290, 9390, 9130, 9070, 9910, 10230, 9670, 9550, 10390};
const int maxStations = (sizeof rdsStations / sizeof(uint16_t)) - 1;
int currentStation = 0;

bool showRdsStatus = true;

unsigned fm_freq;

SI4735 rx;

char *rdsMsg2A;
char *rdsMsg2B;
char *stationInfo;
char *rdsTime;

volatile bool rdsEvent =  false;


void setup()
{

  pinMode(GPIO2_PIN, INPUT);
  
  Serial.begin(9600);
  Serial.println("SI4735 Arduino Library.");
  Serial.println("FM SDR test.");

  attachInterrupt(digitalPinToInterrupt(GPIO2_PIN), gpio2Process, RISING);

  rx.getDeviceI2CAddress(RESET_PIN); // Looks for the I2C bus address and set it.  Returns 0 if error

  delay(500);

  // Starts the SI473X with INTERRUPT ENABLE abd GPIO2 ENABLE
  rx.setup(RESET_PIN, SI473X_INTERRUPT_ENABLE, FM_FUNCTION, SI473X_ANALOG_AUDIO, XOSCEN_CRYSTAL, GPIO2_ENABLE);

  rx.setVolume(45);

  delay(500);

  fm_freq = rdsStations[currentStation];

  rx.setFrequency(fm_freq);
  delay(100);
  showHelp();
  showCurrenteStatus();

  rx.setGpioCtl(0, 1, 0);         // Enables GPIO2
  rx.setGpio(0, 0, 0);            // Sets GPIO2 HIGH
  rx.setRdsConfig(1, 2, 2, 2, 2); 
  rx.setFifoCount(1);     //

  // RDSRECV If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
  // RDSSYNCLOST If set, generate RDSINT when RDS loses synchronization.
  // RDSSYNCFOUND set, generate RDSINT when RDS gains synchronization.
  // RDSNEWBLOCKA If set, generate an interrupt when Block A data is found or subsequently changed
  // RDSNEWBLOCKB If set, generate an interrupt when Block B data is found or subsequently changed
  rx.setRdsIntSource(1, 0, 0, 0, 0); // Trigger an interrupt when the receiver gets RDS.

  Serial.println("FM SDR STARTED.");

}


void gpio2Process() {
  rx.getRdsStatus();
  rdsEvent = rx.getRdsSyncFound(); // True if RDS sync 
}



void showHelp()
{
  Serial.println("Type: S to seek the next FM station or s to seek the Previous.");
  Serial.println("      R to go to the next Station that you have configured in the table rdsStations");
  Serial.println("      >  to increase the frequency");
  Serial.println("      <  to decreasee the frequency");
  Serial.println("      +  to volume up");
  Serial.println("      +  to volume down");
  Serial.println("      ?  to show this help");
  Serial.println("=============================================");
}

void showCurrenteStatus()
{
  delay(250);
  fm_freq = rx.getFrequency();
  Serial.print("You are tuned on ");
  Serial.print(String(fm_freq / 100.0, 2));
  Serial.print(" MHz");
  Serial.print(" [SNR:");
  Serial.print(rx.getCurrentSNR());
  Serial.print("dB");
  Serial.print(" Signal:");
  Serial.print(rx.getCurrentRSSI());
  Serial.println("dBuV]");
  Serial.println("================================");
}

void showCurrenteRdsStatus() {
  Serial.print("Sync Lost...: ");
  Serial.println(rx.getRdsSyncLost());

  Serial.print("Sync Found..: ");
  Serial.println(rx.getRdsSyncFound());

  Serial.print("Synchronized: ");
  Serial.println(rx.getRdsSync());

  Serial.print("Groups Lost.: ");
  Serial.println(rx.getGroupLost());

  Serial.print("FIFO Used...: ");
  Serial.println(rx.getNumRdsFifoUsed());

  Serial.print("New Block A.: ");
  Serial.println(rx.getRdsNewBlockA());

  Serial.print("New Block B.: ");
  Serial.println(rx.getRdsNewBlockB());

  Serial.print("PI..........: ");
  Serial.println(rx.getRdsPI());

  Serial.print("Version......: ");
  Serial.println(rx.getRdsVersionCode());

  Serial.print("Flag A/B.....: ");
  Serial.println(rx.getRdsFlagAB());

  int i = rx.getRdsProgramType();
  Serial.print("Program Type.: ");
  Serial.print(i);

  Serial.print("\nGroup Type...: ");
  Serial.println(rx.getRdsGroupType());
}


void showRdsText()
{
  rx.getRdsStatus();
  // TO DO
}
//
void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    if (key == 'S')
    {
      rx.seekStationUp(); // Look for the next station FM
      showCurrenteStatus();
    }
    else if (key == 's')
    {
      rx.seekStationDown(); //Look for the previous station FM
      showCurrenteStatus();
    }
    else if (key == '+')
    {
      rx.volumeUp();
    }
    else if (key == '-')
    {
      rx.volumeDown();
    }
    else if (key == '>' || key == '.')
    {
      fm_freq += 10;
      rx.setFrequency(fm_freq);
      showCurrenteStatus();
    }
    else if (key == '<' || key == ',')
    {
      fm_freq -= 10;
      rx.setFrequency(fm_freq);
      showCurrenteStatus();
    } else if (key == 'r' || key == 'R')
    { // Goes to the next RDS Station that you configured in the table rdsStations
      currentStation++;
      if (currentStation > maxStations )
        currentStation = 0;
      fm_freq = rdsStations[currentStation];
      rx.setFrequency(fm_freq);
      delay(250);
      showCurrenteStatus();
    } else if ( key == 'M' || key == 'm' ) {
      showRdsStatus = !showRdsStatus;
    }
    else if (key == '?')
    {
      showHelp();
    }
  }


  if ( rdsEvent ) {
    // Checks the RDS information each ELAPSED_TIME seconds
    // if ((millis() - rdsElapsedTime) > ELAPSED_TIME  && showRdsStatus )

      rdsMsg2A = rx.getRdsText2A();
      rdsMsg2B = rx.getRdsText2B();
      stationInfo = rx.getRdsText0A();
      rdsTime = rx.getRdsTime();

      if ( stationInfo != NULL ) {
        Serial.print("\nTipo 0A.....: ");
        Serial.print(stationInfo);
      }

      if ( rdsMsg2A != NULL ) {
        Serial.print("\nTipo 2A.....: ");
        Serial.print(rdsMsg2A);
      }

      if ( rdsMsg2B != NULL ) {
        Serial.print("\nTipo 2B.....: ");
        Serial.print(rdsMsg2B);
      }

      if ( rdsTime  != NULL ) {
        Serial.print("\nTime........: ");
        Serial.print(rdsTime);
      }

  }

  delay(5);
}
