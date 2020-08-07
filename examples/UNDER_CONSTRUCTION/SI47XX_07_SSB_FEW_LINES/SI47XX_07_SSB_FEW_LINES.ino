/**
 * Under construction...
 */
#include <SI4735.h>
#include "xxx"

#define RESET_PIN       12
#define LSB_USB_SWITCH   4
#define STEP_UP          5
#define STEP_DOWN        6

SI4735 rx;

void setup{
    
    digitalMode

    rx.setup(RESET_PIN);
    rx.loadPatch()
    rx.setFrequency()
    showFrequency();
}

void showFrequency() {

}

void loop() {
    // encoder
    rx.setFrequencyUp();
    rx.setFrequencyDown();

    LSB_USB_SWITCH



}



