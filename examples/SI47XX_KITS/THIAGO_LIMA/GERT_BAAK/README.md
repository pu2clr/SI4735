# SI4735-Radio-ESP32-2.8 inch TFT Touchscreen-Arduino
This sketch is using the SI4735 library developed by Ricardo PU2CLR.
The sketch is developed for running at a ESP32 WROOM-32, a 2.8 inch 240*320 Touchscreen with an ILI9341 controler and Rotary Encoder with Switch. For the TFT display the ESP_eSPI library is used. The ILI9341 config file is available.

There seems to be some confusion about the use of the TFT-eSPI library. 
This will solve the problem :

Move the file Setup1_ILI9341.h to the root or top directory of the TFT_eSPI library. Replace the file User_Setup_Select.h in same directory by the file User_Setup_Select.h in my github. The compiler runs after that smoothly.

Schematics for SI4732 added. Software 100 % compatible.

In line 74 and 75 of the sketch you may select now how to use the display, VERTICAL or HORIZONTAL.

The status of the radio is saved in the EEPROM of the ESP32. There are stories how many times you may write an EEPROM adress in the controler. I see figures between 100,000 and 1,000,000 times. Before writing, the program checks the content of the EEPROM. Only when the content is different with the new data , the program will write the data in the EEPROM. If after 3 years of intensive use ( about 100 times writing every day), unexpectedly a memory cell is defect you can simple use an other part of the EEPROM by changing the offsetEEPROM in line 683.

The manual controle of the radio is 100% done by the touchscreen and Rotary Encoder.



Program features :

    FM (VHF) support (64–108 MHz).
  
    AM (MW) band support (520–1710 kHz).
  
    SW band support (2.3–30.0 MHz).
  
    LW band support (153–279 kHz).
    
    S-meter
    
    SNR and RSSI indication.
    
    Volume indicator.
  
    FM Stereo and RDS. RDS has a SNR theshold and on / off button.
    
    AM and SSB with 1,5,9 & 10 KHz tuningstep.
  
    FM, AM and SSB (USB & LSB) modulation. SSB controlled by BFO.
    
    Preset for FM controlled by rotary encoder. 
    May be finished by rotary encoder switch or automatically by waking up the screen. 
  
    Band selection for Broadcast (17) and Ham bands (11).
    
    Prefered modulation for all Ham and Broadcast bands.
  
    Bandwidth control for AM and SSB.
  
    On screen Keyboard for frequency control.
  
    Tuningstep control for all broadcast and ham bands (except FM)
  
    Mute.
  
    Volume control by rotary encoder. Function is switching off after 30 sec automatically.
  
    AM/FM seek tuning.
  
    Automatic frequency control (AFC)
  
    Automatic gain control (AGC)
    Manual gain control by the rotary encoder.
 
    To save power from the battery, display is switching off after 5 minutes. 
    Display is waking up by the use of one of the controllers e.g. screen, rotary controller or rotary controller switch.
    
    Power consumption 120 mA and 80 mA when display is switched off.
