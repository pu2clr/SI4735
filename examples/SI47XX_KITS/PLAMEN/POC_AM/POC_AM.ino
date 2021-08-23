/*
 * Minimal test for Plamen's design on AM mode
 * Is is not working so far
 * ATTENTION: Even this sketch is not using the OLED, it have to be connected to the KIT to make the SI4732 starts (may be due to pull-up resistors on the I2C bus).
 */

#include <SI4735.h>
#include <si5351.h>

#define SI5351_CALIBRATION_OFFSET 39000 // SI5351 calibration factor
#define RESET_PIN PB12
#define IF_AMI_OFFSET 10700   

#define MFREQ 1000000ULL // For example: 65.700.000 * 100ULL

SI4735 rx;
Si5351 vfo(0x62); 

void setup()
{
  vfo.init(SI5351_CRYSTAL_LOAD_10PF, 27000000, SI5351_CALIBRATION_OFFSET);
  vfo.output_enable(SI5351_CLK0, 1);                  //1 - Enable / 0 - Disable CLK
  vfo.drive_strength(SI5351_CLK0, SI5351_DRIVE_4MA);  //Output current 2MA, 4MA, 6MA or 8MA
 
  vfo.update_status();
  
  delay(100);

  rx.getDeviceI2CAddress(RESET_PIN); 
  
  rx.setup(RESET_PIN, 1);
  // rx.reset();
  
  delay(300);

  rx.setAM(IF_AMI_OFFSET - 10, IF_AMI_OFFSET + 10, IF_AMI_OFFSET, 1);

  rx.setAvcAmMaxGain(48); // Sets the maximum gain for automatic volume control on AM/SSB mode (you can use values between 12 and 90dB).
  delay(100);
  
  // rx.setFrequency(IF_AMI_OFFSET);
  // rx.setBandwidth(2, 0);
  // rx.setAmSoftMuteMaxAttenuation(4); 
  // rx.setAutomaticGainControl(1, 0); 
   
  delay(100);
  rx.setVolume(15);


  // Trying Air band / 127,7 MHz 
  rx.setTuneFrequencyAntennaCapacitor(1); // Sets ATU for HIGH frequency 
  vfo.set_freq( (127700000 + (IF_AMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0); 
  delay(20000); // waits for 20 secounds
  // Trying SW / 11940kHz. 
  vfo.set_freq( ( 11940000 +  (IF_AMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0);
  delay(20000);
  // Trying MW / 810kHz. It is a strong local AM station 
   rx.setTuneFrequencyAntennaCapacitor(0); // Sets ATU for MW 
  vfo.set_freq( (810000 +  (IF_AMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0);
  delay(20000);
  rx.setAudioMute(true);
}


void loop() {
  delay(2);
}
