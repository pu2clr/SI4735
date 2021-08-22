/*
 * Minimal test for Plamen's design on FM mode
 * Is is working very well 
 * ATTENTION: Even this sketch is not using the OLED, it have to be connected  to the KIT to make the SI4732 starts (may be due to pull-up resistors on the I2C bus).
 */

#include <SI4735.h>
#include <si5351.h>

#define SI5351_CALIBRATION_OFFSET 39000 // SI5351 calibration factor
#define RESET_PIN PB12

#define IF_FMI_OFFSET 65400                 // 65.4MHz
#define IF_FMI_SI4735 IF_FMI_OFFSET / 10    // Use 6540 to tune SI473X on 65.4MHz

SI4735 rx;
Si5351 vfo(0x62); 

void setup()
{
  vfo.init(SI5351_CRYSTAL_LOAD_10PF, 27000000, SI5351_CALIBRATION_OFFSET);
  vfo.output_enable(SI5351_CLK0, 1);                  //1 - Enable / 0 - Disable CLK
  vfo.drive_strength(SI5351_CLK0, SI5351_DRIVE_4MA);  //Output current 2MA, 4MA, 6MA or 8MA
  delay(100);

  rx.getDeviceI2CAddress(RESET_PIN); 
  rx.setup(RESET_PIN, 1);
  rx.setFM(IF_FMI_SI4735 - 10, IF_FMI_SI4735 + 10, IF_FMI_SI4735, 1);
  rx.setVolume(22);
  
  // Trying two FM local stations. IT IS WORKING WELL!
  vfo.set_freq( (106500000 + (IF_FMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0); // NOVA BRASIL STATION - 106.5MHZ - OK
  delay(20000); // Waits for 20 secounds.
  vfo.set_freq( (93900000 + (IF_FMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0); // VERDES MARES STATION - 93.9MHz - OK
  delay(20000); // Waits for 20 secounds.
  vfo.set_freq( (89900000 + (IF_FMI_OFFSET * 1000ULL)) * 100ULL, SI5351_CLK0); //  89.9MHz - OK
  delay(20000); // Waits for 20 secounds.
  rx.setAudioMute(true);  

}


void loop() {
  delay(2);
}
