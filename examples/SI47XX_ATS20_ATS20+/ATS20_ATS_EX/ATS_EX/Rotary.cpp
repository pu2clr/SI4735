#include "Arduino.h"
#include "Rotary.h"
#define R_START       0x0
#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN   0x1
#define R_CW_BEGIN    0x2
#define R_START_M     0x3
#define R_CW_BEGIN_M  0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M, R_CW_BEGIN, R_CCW_BEGIN, R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START, R_CCW_BEGIN, R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW, R_CW_BEGIN, R_START, R_START},
  // R_START_M (11)
  {R_START_M, R_CCW_BEGIN_M, R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M, R_START_M, R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M, R_CCW_BEGIN_M, R_START_M, R_START | DIR_CCW},
};
#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL  0x1
#define R_CW_BEGIN  0x2
#define R_CW_NEXT   0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT  0x6

const unsigned char ttable[7][4] = 
{
  // R_START
  {R_START, R_CW_BEGIN, R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT, R_START, R_CW_FINAL, R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT, R_CW_BEGIN, R_START, R_START},
  // R_CW_NEXT
  {R_CW_NEXT, R_CW_BEGIN, R_CW_FINAL, R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START, R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START, R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

// Constructor. Each arg is the pin number for each encoder contact
Rotary::Rotary(char _pin1, char _pin2) 
{
  // Assign variables
  pin1 = _pin1;
  pin2 = _pin2;
  // Set pins to input.
  DDRD &= ~(1 << pin1);
  DDRD &= ~(1 << pin2);

#ifdef ENABLE_PULLUPS
  PORTD |= (1 << pin1);
  PORTD |= (1 << pin2);
#endif
  // Initialise state
  state = R_START;
}

unsigned char Rotary::process() 
{
	// Grab state of input pins
	unsigned char pinstate = ((PIND & (1 << 3)) >> 2) | ((PIND & (1 << 2)) >> 2);
	// Determine new state from the pins and state table
	state = ttable[state & 0xf][pinstate];
	// Return emit bits, ie the generated event
	return state & 0x30;
}

