/*
  Under development...
  
  This sketch is an example of using the RDS functions impplemented byte the Si4735 Library for Arduino. 

  Prototype documentation: https://pu2clr.github.io/SI4735/
  PU2CLR Si47XX API documentation: https://pu2clr.github.io/SI4735/extras/apidoc/html/

  By Ricardo Lima Caratti
*/

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define FM_FUNCTION 0

#define ELAPSED_TIME 400

long rdsElapsedTime = millis();

const unsigned min_fm = 8400;
const unsigned max_fm = 10900;

// Setup some local FM Stations with RDS service
uint16_t rdsStations[] = {10390, 9550, 9290, 8090, 9130, 9070, 9910, 10230, 10430, 10570, 10650};
const int maxStations = (sizeof rdsStations / sizeof(uint16_t)) - 1;
int currentStation = 0;

bool showRdsStatus = true;

unsigned fm_freq;

// UE
/*
  char *
    tabProgramType[] = {
        "No program definition type", "News", "Current affairs", "Information", "Sport", "Education", "Drama",
        "Culture", "Science", "Variable", "Popular Music (Pop)", "Rock Music", "Easy Listening", "Light Classical",
        "Serious Classical", "Other Music", "Weather", "Finance", "Children’s Programs", "Social Affairs", "Religion",
        "Phone-in Talk", "Travel", "Leisure", "Jazz Music", "Country Music", "National Music", "Oldies Music",
        "Folk Music", "Documentary", "Alarm Test", "Alarm"};

  // USA - comment above and uncomment below if you are using USA
  char * tabProgramType[] = {
  "No program definition type", "News", "Information", "Sport", "Talk", "Rock", "Classic Rock",
  "Adult Hits", "Soft Rock", "Top 40", "Country Music", "Oldies Music", "Soft Music", "Nostalgia",
  "Jazz", "Classical", "Rhythm & Blues Music", "Soft Rhythm & Blues Music", "Language", "Religious Music", "Religious Talk",
  "Personality", "Public", "College", "Not assigned", "Not assigned", "Not assigned", "Not assigned",
  "Not assigned", "Weather", "Emergency Test", "Emergency"
  };
*/

SI4735 si4735;
char *rdsMsg2A;
char *rdsMsg2B;
char *stationInfo;
char *rdsTime;
void setup()
{
  Serial.begin(9600);
  Serial.println("SI4735 Arduino Library.");
  Serial.println("FM SDR test.");

  delay(500);

  si4735.setup(RESET_PIN, INTERRUPT_PIN, FM_FUNCTION);
  
  si4735.setVolume(45);

  delay(500);

  fm_freq = rdsStations[currentStation];

  si4735.setFrequency(fm_freq);
  delay(100);
  showHelp();
  showCurrenteStatus();
  si4735.setRdsConfig(1, 2, 2, 2, 2);
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
  fm_freq = si4735.getFrequency();
  Serial.print("You are tuned on ");
  Serial.print(String(fm_freq / 100.0, 2));
  Serial.print(" MHz");
  Serial.print(" [SNR:");
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");
  Serial.print(" Signal:");
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");
  Serial.println("================================");
}

void showCurrenteRdsStatus() {
  Serial.print("Sync Lost...: ");
  Serial.println(si4735.getRdsSyncLost());

  Serial.print("Sync Found..: ");
  Serial.println(si4735.getRdsSyncFound());

  Serial.print("Synchronized: ");
  Serial.println(si4735.getRdsSync());

  Serial.print("Groups Lost.: ");
  Serial.println(si4735.getGroupLost());

  Serial.print("FIFO Used...: ");
  Serial.println(si4735.getNumRdsFifoUsed());

  Serial.print("New Block A.: ");
  Serial.println(si4735.getRdsNewBlockA());

  Serial.print("New Block B.: ");
  Serial.println(si4735.getRdsNewBlockB());

  Serial.print("PI..........: ");
  Serial.println(si4735.getRdsPI());

  Serial.print("Version......: ");
  Serial.println(si4735.getRdsVersionCode());

  Serial.print("Flag A/B.....: ");
  Serial.println(si4735.getRdsFlagAB());

  int i = si4735.getRdsProgramType();
  Serial.print("Program Type.: ");
  Serial.print(i);

  Serial.print("\nGroup Type...: ");
  Serial.println(si4735.getRdsGroupType());
}


void showRdsText()
{
  si4735.getRdsStatus();
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
      si4735.seekStationUp(); // Look for the next station FM
      showCurrenteStatus();
    }
    else if (key == 's')
    {
      si4735.seekStationDown(); //Look for the previous station FM
      showCurrenteStatus();
    }
    else if (key == '+')
    {
      si4735.volumeUp();
    }
    else if (key == '-')
    {
      si4735.volumeDown();
    }
    else if (key == '>' || key == '.')
    {
      fm_freq += 10;
      si4735.setFrequency(fm_freq);
      showCurrenteStatus();
    }
    else if (key == '<' || key == ',')
    {
      fm_freq -= 10;
      si4735.setFrequency(fm_freq);
      showCurrenteStatus();
    } else if (key == 'r' || key == 'R')
    { // Goes to the next RDS Station that you configured in the table rdsStations
      currentStation++;
      if (currentStation > maxStations )
        currentStation = 0;
      fm_freq = rdsStations[currentStation];
      si4735.setFrequency(fm_freq);
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

  si4735.getRdsStatus(); // It needs to be called before any other RDS call function

  // Checks the RDS information each ELAPSED_TIME seconds
  // if ((millis() - rdsElapsedTime) > ELAPSED_TIME  && showRdsStatus )
  if ( si4735.getRdsReceived() )
  {
    si4735.getRdsStatus(); // It needs to be called before any other RDS call function

    rdsMsg2A = si4735.getRdsText2A();
    rdsMsg2B = si4735.getRdsText2B();
    stationInfo = si4735.getRdsText0A();
    rdsTime = si4735.getRdsTime();

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

    Serial.println("\n***********");
    delay(100);

    rdsElapsedTime = millis();
  }

  delay(20);
}
