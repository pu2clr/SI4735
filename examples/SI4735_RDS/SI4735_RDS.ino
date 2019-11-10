/*
 * RDS test
 */

#include <SI4735.h>

#define INTERRUPT_PIN 2
#define RESET_PIN 12

#define FM_FUNCTION 0

const unsigned min_fm = 8400;
const unsigned max_fm = 10900;
unsigned fm_freq = 10390; // 103.9 MHz - Change it to your local FM station with SDR service

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

    showCurrenteStatus();

    si4735.setRdsConfig(1, 2, 2, 2, 2);
}

void showCurrenteStatus()
{
    Serial.println("==========================================================");
    Serial.print("You are tuned on ");
    Serial.print( String(fm_freq/100.0,2));
    Serial.println(" MHz");
    Serial.println("Wait for a RDS message or type S to seek another FM station.");
    Serial.println("==========================================================");
}

void showRdsText()
{
    si4735.getRdsStatus();
    // TO DO
}

void loop()
{
    if (Serial.available() > 0)
    {
        char key = Serial.read();
        if (key == 'S' || key == 's')
        {
            si4735.seekStation(1, 1); // // Look for the next station FM
            fm_freq = si4735.getFrequency();
            showCurrenteStatus();
        } else if ( key == '+' ) {
            si4735.volumeUp();
        } else if ( key == '-' ) {
            si4735.volumeDown();
        }   
    }
    si4735.getRdsStatus();
    if (si4735.getRdsReceived())
    {   
        Serial.print("Program Type: ");
        Serial.print(si4735.getRdsProgramType());
        Serial.println("\n==========================================================");
        delay(400);
    }
    delay(10);
}
