
# TO DO

## Things I need to finish

* Improve RDS/RBDS protocol implementation;
  * Support to text message scrolling (50% done);
  * Support to UTC and local time handling (75% done); 
  * More control on refresh text message (50% done);
* NBFM support (0% done)
* Write to and read from a eeprom,  Si4735 patches (SSB NBFM) (50% done)
* Exemples
  * Finish all of them with the coment "Under construction...." (75% done)
  * ATtiny85 
    * Make it to work with TM1638 Display and Keyboard (0% done)
    * Make it to work with EEPROM and SSB (25% done)
* Digital Audio support (25% done)
   

## Things I need to check

* Check the circuit using 10pF cap instead 22pF connected to the crystal (0% done);
* Check the circuit with GND and RF GND separated (50% done);
* Check the circuit with VA and VD powered with different sources (0% done);
* Internal varicap of AMI and FMI (method setTuneFrequencyAntennaCapacitor )
  * Check if it is possible to interact with exteranl front-end (0% done);
* Adjust and test the interrupt setup of the Si47XX device (75% done)

## Things I need to build

* Front-end
  * HF BPF controled by MCU (25% done);
  * RF Amplifier (0% done);
* Audio output 
  * Audio Amplifier (0% done)
  
  
  
  
