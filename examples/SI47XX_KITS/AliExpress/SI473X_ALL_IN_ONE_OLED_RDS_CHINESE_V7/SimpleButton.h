// Simple button library for Arduino.
#include "Arduino.h"

#ifndef simplebutton_h
#define simplebutton_h

/* 

Defines a class SimpleButton which must be linked to a GPIO to act as a button handler for this pin.

SimpleButton will debounce the pin and is able to generate Shortpress- and (repeated) Longpress-Events.


To keep the code clean, there are some limitations:
- Pin is set to Input with Internal Pullup. If the controller does not have internal pullups on tha specified pin, you need to add one
  in hardware. 
- The switch attached to the pin is considered to drive the pin to LOW if pressed.
- Valid pin numbers range from 0..63.
- All Timings (Debounce- and Longpress-Events) are hardcoded (see BUTTONTIME_-#defines below) and can not be changed by API
- All Timings are significant as multiples of 16 only (i. e. 0..15 === 0, 16..31 === 16 etc.) and must not exceed 1023 (or 1008)

There are only two API-Calls:

SimpleButton::SimpleButton(uint8_t pin);

  - Constructor to create a "Button-Handler"
  - The given pin number must not exceed 63
  - The given pin is set to pinMode(INPUT_PULLUP). The allplication must not change the pinMode for the given pin!

uint8_t SimpleButton::event( void (*eventHandler)(uint8_t eventId, uint8_t pin)=NULL );
  - Must be called (frequently, i. e. in loop()) to process the button events
  - will return one the following defines
  - an optional callback-function can be passed as argument to SimpleButton::event():
     - signature of callback function is void (uint8_t eventId, uint8_t pin);
     - the first parameter is the event triggering the callback, equivalent to the return code of SimpleButton::event(), but only
       the events coded as BUTTONEVENT_*-defines will be reported to the callback function
     - the second parameter is the pin that is attached to the button. This information can be used to use the same callback function
       for different buttons (i. e. for Volume+ or Volume- where the logic is the same but only the direction of change differs)
 */


/* 
Return codes for SimpleButton::event() Do not mess around with the numbers used in the follwing defines!!!

Note that the values are somewhat bitcoded:
- if value is 0 (Zero), button is currently not pressed and no event is due...
- b0 is the "event" flag, if set, a button event has occured (pressed, longpressed)
- if b1 is set (currently only if b0 is set as well), this is a longpress event
- b2 is the "repeat" flag (currently only for continuous longpress, so b1 and b0 are set as well)
- b3 is the "done" flag. 
   * if no other bit is set, this signals the button is still active (pressed) but no event condition applies
   * if b1 and b0 are set, longpress has been concluded
*/

#define BUTTON_IDLE                     0         // Button is currently not pressed.
#define BUTTONEVENT_SHORTPRESS          1         // Shortpress-Event detected!
#define BUTTONEVENT_2PRESS              5
#define BUTTONEVENT_FIRSTLONGPRESS      3         // Button is longpressed (Longpress just started)
#define BUTTONEVENT_LONGPRESS           7         // Button is still longpressed (event will be generated every x ms as defined by 
                                                  // BUTTONTIME_LONGPRESSREPEAT (see below), if the  SimpleButton::event() is called 
                                                  // often enough.
                                                  // The application self must do something if longer period is needed (i. e. like
                                                  //  delaying calls to ::event() or react on only every other event to achieve 400ms)
#define BUTTONEVENT_LONGPRESSDONE      11         // Button is released after longpress. The application must not treat this event
                                                  // as if the button is still pressed but either use it for some cleanup (if needed)
                                                  // or simply ignore it.
#define BUTTON_PRESSED                  8         // No event, but the button is pressed (so either a BUTTONEVENT_SHORTPRESS or 
                                                  // any of the longpress-Events might follow but time for this is not yet due).

#define BUTTONEVENT_ISLONGPRESS(x)      (3 == (x & 3))                                                  
#define BUTTONEVENT_ISDONE(x)           (8 == (x & 8))                                                  


#define BUTTONTIME_PRESSDEBOUNCE      0*16        // How long to debounce falling slope of pin (in ms), i. e. Button going to pressed
#define BUTTONTIME_LONGPRESS1        20*16        // Time (ms) after debounce a button needs to be pressed to be considered longpressed
#define BUTTONTIME_LONGPRESSREPEAT   3*16         // Time (ms) between consecutive longpress events
#define BUTTONTIME_RELEASEDEBOUNCE    4*16        // How long to debounce rising slope of pin (in ms), i. e. Button going to released

class SimpleButton
{
  public:
    SimpleButton(uint8_t pin);
    uint8_t checkEvent(void (*_event)(uint8_t event, uint8_t pin) = NULL);
  private:
    uint16_t _PinDebounceState;
};
#endif
