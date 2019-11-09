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
    si4735.setFrequency(fm_freq);
    si4735.setVolume(45);

    showCurrenteStatus();

}

void showCurrenteStatus()
{
    Serial.println("==========================================================");
    Serial.print("You are tuned on ");
    Serial.print(fm_freq);
    Serial.println(" MHz");
    Serial.print("Wait for a RDS message or type S to seek another FM station.");
    Serial.println("==========================================================");
}

void showRdsText() {
    si4735.getRdsStatus();
    // TO DO
}

void loop() {

    if (Serial.available() > 0)
    {
        char key = Serial.read();
        if ( key == 'S' || key == 's') {
            si4735.seekStation(1, 1); // // Look for the next station FM
            fm_freq = si4735.getFrequency();
            showCurrenteStatus();
        }
    }

    // check the RDS status 
    if ( si4735.getRadioDataSystemInterrupt() ) {
       // TO DO 
    } 
    delay(10);
}
