/*
SoftwareSerial.h (formerly NewSoftSerial.h) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>
#include <HardwareSerial.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1052__) || defined(__IMXRT1062__)

class SoftwareSerial : public Stream
{
public:
	SoftwareSerial(uint8_t rxPin, uint8_t txPin, bool inverse_logic = false);
	~SoftwareSerial() { end(); }
	void begin(unsigned long speed);
	void end();
	bool listen() { return true; }
	bool isListening() { return true; }
	bool overflow() { bool ret = buffer_overflow; buffer_overflow = false; return ret; }
	virtual int available();
	virtual int read();
	int peek();
	virtual void flush();
	virtual size_t write(uint8_t byte);
	using Print::write;
private:
	HardwareSerial *port;
	uint32_t cycles_per_bit;
	#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
	volatile uint32_t *txreg;
	volatile uint32_t *rxreg;
	#else
	volatile uint8_t *txreg;
	volatile uint8_t *rxreg;
	#endif
	bool buffer_overflow;
	uint8_t txpin;
	uint8_t rxpin;
};

#else
class SoftwareSerial : public Stream
{
private:
  // per object data
  uint8_t _receivePin;
  uint8_t _receiveBitMask;
  volatile uint8_t *_receivePortRegister;
  uint8_t _transmitBitMask;
  volatile uint8_t *_transmitPortRegister;

  uint16_t _rx_delay_centering;
  uint16_t _rx_delay_intrabit;
  uint16_t _rx_delay_stopbit;
  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;

  // static data
  static char _receive_buffer[_SS_MAX_RX_BUFF]; 
  static volatile uint8_t _receive_buffer_tail;
  static volatile uint8_t _receive_buffer_head;
  static SoftwareSerial *active_object;

  // private methods
  void recv();
  uint8_t rx_pin_read();
  void tx_pin_write(uint8_t pin_state);
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);

  // private static method for timing
  static inline void tunedDelay(uint16_t delay);


public:
  // public methods
  SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
  ~SoftwareSerial();
  void begin(long speed);
  bool listen();
  void end();
  bool isListening() { return this == active_object; }
  bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
  int peek();

  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();
  
  using Print::write;

  // public only for easy access by interrupt handlers
  static inline void handle_interrupt();
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif

#endif
