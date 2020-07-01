#include <SI4735.h>
SI4735 r;
void setup() {
    for (int i = 5; i <= 8; i++) pinMode(i, INPUT_PULLUP); // Starts 4 Arduino digital pins (from 5 to 8) with pullup resource.
    r.setup(12,0); // Starts the receiver. The Arduino digital pin 12 is used to reset the Si4735. 
    r.setFM(6400,10800,10750,10); // Sets the FM mode, band limits and default station.
    r.setVolume(50); // Optional, sets the voume level when the receiver starts 
}
void loop() {
    if (digitalRead(5) == LOW) r.setAM(520,1790,810,10);
    if (digitalRead(6) == LOW) r.setFM(6400, 10800, 10750, 10);
    if (digitalRead(7) == LOW) r.seekNextStation();
    if (digitalRead(8) == LOW) r.seekPreviousStation();
    delay(150);
}
