/*

   RDS test
   Under development
*/

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define FM_FUNCTION 0

const unsigned min_fm = 8400;
const unsigned max_fm = 10900;
unsigned fm_freq = 10390; // 103.9 MHz - Change it to your local FM station with SDR service


/*
  // ORININAL UE
  String tabProgramType[] = {
  "No program definition type", "News", "Current affairs", "Information", "Sport", "Education", "Drama",
  "Culture", "Science", "Variable", "Popular Music (Pop)", "Rock Music", "Easy Listening", "Light Classical",
  "Serious Classical", "Other Music", "Weather", "Finance", "Children’s Programs", "Social Affairs","Religion",
  "Phone-in Talk", "Travel", "Leisure", "Jazz Music", "Country Music", "National Music", "Oldies Music",
  "Folk Music", "Documentary", "Alarm Test", "Alarm"
  };
*/

// UE
String tabProgramType[] = {
  "No prog. def.", "News", "Cur. affairs", "Inf.", "Sport", "Educ.", "Drama",
  "Culture", "Science", "Variable", "Pop", "Rock", "Easy List.", "L.Classical",
  "S.Classical", "Other Music", "Weather", "Finance", "Children’s Prog.", "Social Affairs", "Religion",
  "Talk", "Travel", "Leisure", "Jazz", "Country Music", "National Music", "Oldies Music",
  "Folk", "Documentary", "Alarm Test", "Alarm"
};

/*
  // EUA
  String tabProgramType[] = {
  "No program definition type", "News", "Information", "Sport", "Talk", "Rock", "Classic Rock",
  "Adult Hits", "Soft Rock", "Top 40", "Country Music", "Oldies Music", "Soft Music", "Nostalgia",
  "Jazz", "Classical", "Rhythm & Blues Music", "Soft Rhythm & Blues Music", "Language", "Religious Music", "Religious Talk",
  "Personality", "Public", "College", "Not assigned", "Not assigned", "Not assigned", "Not assigned",
  "Not assigned", "Weather", "Emergency Test", "Emergency"
  };
*/

SI4735 si4735;

void setup()
{
  Serial.begin(9600);
  Serial.println("SI4735 Arduino Library.");
  Serial.println("FM SDR test.");

  delay(500);

  si4735.setup(RESET_PIN, INTERRUPT_PIN, FM_FUNCTION);
  si4735.setVolume(45);
  delay(500);
  si4735.setFrequency(fm_freq);
  delay(100);
  showHelp();

  showCurrenteStatus();

  si4735.setRdsConfig(1, 2, 2, 2, 2);
}

void showHelp() {
  Serial.println("Type: S  to seek the next FM station.");
  Serial.println("      s  to seek the previous FM station.");
  Serial.println("      >  to increase the frequency");
  Serial.println("      <  to decreasee the frequency");
  Serial.println("      +  to volume up");
  Serial.println("      +  to volume down");
  Serial.println("      ?  to show this help");
  Serial.println("=============================================");
}

void showCurrenteStatus()
{
  fm_freq = si4735.getFrequency();
  delay(100);
  Serial.print("You are tuned on ");
  Serial.print( String(fm_freq / 100.0, 2));
  Serial.print(" MHz"); 
  Serial.print(" [SNR:" );
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");
  Serial.print(" Signal:" );
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");  
  Serial.println("================================");
}

void showRdsText()
{
  si4735.getRdsStatus();
  // TO DO
}


void showMessage() {
  Serial.println(">");
  for (int i = 0; i < 64; i++ ) {
    
    if ( si4735.rds_buffer[i] > 31 && si4735.rds_buffer[i] < 127 )
      Serial.print(si4735.rds_buffer[i]);
  }
  Serial.println("<");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    if (key == 'S')
    {
      si4735.seekStationUp(); // Look for the next station FM
      showCurrenteStatus();
    } else if (key == 's') {
      si4735.seekStationDown(); //Look for the previous station FM
      showCurrenteStatus();
    } else if ( key == '+' ) {
      si4735.volumeUp();
    } else if ( key == '-' ) {
      si4735.volumeDown();
    } else if ( key == '>' || key == '.' ) {
      fm_freq += 10;
      si4735.setFrequency(fm_freq);
      showCurrenteStatus();
    } else if ( key == '<' || key == ',' ) {
      fm_freq -= 10;
      si4735.setFrequency(fm_freq);
      showCurrenteStatus();
    } else if ( key == '?')  {
      showHelp();
    }
  }
  // RDS Test
  si4735.getRdsStatus();
  if (si4735.getRdsReceived())
  {
    unsigned gt = si4735.getRdsGroupType();
     if ( gt == 2 || gt == 4 ) {
      int i = si4735.getRdsProgramType();

      Serial.print("RDS GT: ");
      Serial.print(si4735.getRdsGroupType());

      Serial.print("-RDS V: ");
      Serial.print(si4735.getRdsVersionCode());

      Serial.print("-PTy: ");
      Serial.print(i);
      Serial.print("-");
      if ( i < 32 ) {
        Serial.print(tabProgramType[i]);
      }
      else {
        Serial.print("****");
      }
      if ( gt == 4 )  {
        Serial.print(" - ");
        Serial.print(si4735.getRdsTime());
      } else {
        Serial.print("-");
        Serial.print(si4735.getRdsText());
        // showMessage();
      }

      Serial.println("\n===================================================");
      delay(600);
    }
  }
  delay(100);
}
