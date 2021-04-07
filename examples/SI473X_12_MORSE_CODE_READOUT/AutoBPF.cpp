#include "AutoBPF.h"


/**
 * @brief Configures the Arduino pins used to control the bandpass filter circuit 
 * 
 * @param pin_s0  Audiono pin connected to the input s0
 * @param pin_s1  Audiono pin connected to the input s1
 */
void AutoBPF::setup(uint8_t pin_s0, uint8_t pin_s1) {
    this->s0 = pin_s0;
    this->s1 = pin_s1;
    pinMode(this->s0, OUTPUT);
    pinMode(this->s1, OUTPUT);
}

/**
 * @brief Sets the bandpass filter
 * @details Selects the BPF
 * 
 * @param filter the valid values are 0,1,2 and 3.
 */
void AutoBPF::setFilter(uint8_t filter) {
    if (filter < 4) {
        digitalWrite(this->s0, (filter & 1));  // Sets the S0 HIGH or LOW
        digitalWrite(this->s1, (filter & 2));     // Sets the S1 HIGH or LOW
        this->currentFilter = filter;
    }
};

/**
 * @brief Switches to the next BPF
 * @details if the current filter is the last (filter 3), the first filter will be select (filter 0).
 */
void AutoBPF::setNext() {

    if (this->currentFilter == 3) 
      this->currentFilter = 0;
    else 
       this->currentFilter++;

    this->setFilter(currentFilter);
};

/**
 * @brief Switches to the previous BPF
 * @details if the current filter is the first (filter 0), the last filter will be select (filter 3).
 */
void AutoBPF::setPrevious() {
    if (this->currentFilter == 0)
        this->currentFilter = 3;
    else
        this->currentFilter--;

    this->setFilter(currentFilter);
};
