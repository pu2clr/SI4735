/***************************************************
  Arduino TFT graphics library targetted at the UNO
  and Mega boards.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics funtions and the
  proportional fonts.

  The larger fonts are Run Length Encoded to reduce their
  size.

 ****************************************************/

#include "TFT_ST7735.h"

#include <avr/pgmspace.h>
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

inline void spiWait17(void) __attribute__((always_inline));
inline void spiWait15(void) __attribute__((always_inline));
inline void spiWait14(void) __attribute__((always_inline));
inline void spiWait12(void) __attribute__((always_inline));
inline void spiWrite16(uint16_t data, int16_t count) __attribute__((always_inline));
inline void spiWrite16s(uint16_t data) __attribute__((always_inline));
inline void spiWrite16R(uint16_t data, int16_t count) __attribute__((always_inline));

/***************************************************************************************
** Function name:           TFT_ST7735
** Description:             Constructor , we must use hardware SPI pins
***************************************************************************************/
TFT_ST7735::TFT_ST7735(int16_t w, int16_t h)
{
  _cs   = TFT_CS;
  _dc   = TFT_DC;
  _rst  = TFT_RST;
  _mosi  = _sclk = 0;

  if (_rst > 0) {
    digitalWrite(_rst, LOW);
    pinMode(_rst, OUTPUT);
  }

  TFT_DC_D;
  pinMode(_dc, OUTPUT);

  TFT_CS_H;
  pinMode(_cs, OUTPUT);

  _width    = w;
  _height   = h;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textfont  = 1;
  textsize  = 1;
  textcolor   = 0xFFFF;
  textbgcolor = 0x0000;
  padX = 0;
  textwrap  = true;
  textdatum = 0; // Left text alignment is default
  fontsloaded = 0;

  addr_row = 0xFF;
  addr_col = 0xFF;

#ifdef LOAD_GLCD
  fontsloaded = 0x0002; // Bit 1 set
#endif

#ifdef LOAD_FONT2
  fontsloaded |= 0x0004; // Bit 2 set
#endif

#ifdef LOAD_FONT4
  fontsloaded |= 0x0010; // Bit 4 set
#endif

#ifdef LOAD_FONT6
  fontsloaded |= 0x0040; // Bit 6 set
#endif

#ifdef LOAD_FONT7
  fontsloaded |= 0x0080; // Bit 7 set
#endif

#ifdef LOAD_FONT8
  fontsloaded |= 0x0100; // Bit 8 set
#endif

}

/***************************************************************************************
** Function name:           spiwrite
** Description:             Write 8 bits to SPI port
***************************************************************************************/
void TFT_ST7735::spiwrite(uint8_t c)
{
  savedSPCR = SPCR;
  SPCR = mySPCR;
  SPDR = c;
  asm volatile( "nop\n\t" ::); // Sync SPIF
  while (!(SPSR & _BV(SPIF)));
  SPCR = savedSPCR;
}

/***************************************************************************************
** Function name:           writecommand
** Description:             Send an 8 bit command to the TFT
***************************************************************************************/
void TFT_ST7735::writecommand(uint8_t c)
{
  TFT_DC_C;
  TFT_CS_L;
  spiwrite(c);
  TFT_CS_H;
}

/***************************************************************************************
** Function name:           writedata
** Description:             Send a 8 bit data value to the TFT
***************************************************************************************/
void TFT_ST7735::writedata(uint8_t c)
{
  TFT_DC_D;
  TFT_CS_L;
  spiwrite(c);
  TFT_CS_H;
}

/***************************************************************************************
** Function name:           writeEnd
** Description:             Raise the Chip Select
***************************************************************************************/
void TFT_ST7735::writeEnd() {
  TFT_CS_H;
}

/***************************************************************************************
** Function name:           backupSPCR
** Description:             Save the SPCR register so it can be restored
***************************************************************************************/
void TFT_ST7735::backupSPCR() {
  savedSPCR = SPCR;
  SPCR = mySPCR;
}

/***************************************************************************************
** Function name:           restoreSPCR
** Description:             Restore the origional SPCR value
***************************************************************************************/
void TFT_ST7735::restoreSPCR() {
 SPCR = savedSPCR;
}

/***************************************************************************************
** Function name:           spi_begin
** Description:             Prepare for SPI communication to TFT
***************************************************************************************/
// If the SPI library has transaction support, these functions
// establish settings and protect from interference from other
// libraries.  Otherwise, they simply do nothing.

#ifdef SPI_HAS_TRANSACTION
  #ifdef SUPPORT_TRANSACTIONS

static inline void spi_begin(void) __attribute__((always_inline));

static inline void spi_begin(void) {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
}

static inline void spi_end(void) __attribute__((always_inline));

static inline void spi_end(void) {
  SPI.endTransaction();
}
  #else // we do not want to SUPPORT_TRANSACTIONS

#define spi_begin()
#define spi_end()

  #endif // SUPPORT_TRANSACTIONS

#else
#define spi_begin()
#define spi_end()
#endif

/***************************************************************************************
** Function name:           begin
** Description:             Included for backwards compatibility
***************************************************************************************/
void TFT_ST7735::begin(void)
{
 init();
}

/***************************************************************************************
** Function name:           init
** Description:             Reset, then initialise the TFT display registers
***************************************************************************************/
#define DELAY 0x80

void TFT_ST7735::init(void)
{
  SPI.begin();

  savedSPCR = SPCR;
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  mySPCR = SPCR;

  spi_end();

  // toggle RST low to reset
  if (_rst > 0) {
    digitalWrite(_rst, HIGH);
    delay(5);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(150);
  }


	// Initialization commands for ST7735 screens
  static const uint8_t PROGMEM
  Bcmd[] = {                  // Initialization commands for 7735B screens
    18,                       // 18 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
      255,                    //     255 = 500 ms delay
    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
      0x05,                   //     16-bit color
      10,                     //     10 ms delay
    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
      0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03,                   //     3 lines back porch
      10,                     //     10 ms delay
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,                   //     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02,                   //     Fix on VTL
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      0x0,                    //     Line inversion
    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
      0x02,                   //     GVDD = 4.7V
      0x70,                   //     1.0uA
      10,                     //     10 ms delay
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      0x05,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      0x01,                   //     Opamp current small
      0x02,                   //     Boost frequency
    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
      0x3C,                   //     VCOMH = 4V
      0x38,                   //     VCOML = -1.1V
      10,                     //     10 ms delay
    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                     //     10 ms delay
    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 2
      0x00, 0x81,             //     XEND = 129
    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 1
      0x00, 0x81,             //     XEND = 160
    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
      255 },                  //     255 = 500 ms delay

  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color

  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x9F+0x01 },      //     XEND = 159

  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },           //     XEND = 159

  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay


     tabcolor = TAB_COLOUR;

     if (tabcolor == INITB)
     {
       commandList(Bcmd);
     }
     else 
     {
	  commandList(Rcmd1);
       if (tabcolor == INITR_GREENTAB)
       {
         commandList(Rcmd2green);
         colstart = 2;
         rowstart = 1;
       }
       else if (tabcolor == INITR_GREENTAB2)
       {
         commandList(Rcmd2green);
         writecommand(ST7735_MADCTL);
         writedata(0xC0);
         colstart = 2;
         rowstart = 1;
       }
       else if (tabcolor == INITR_REDTAB)
       {
         commandList(Rcmd2red);
       }
       else if (tabcolor == INITR_BLACKTAB)
       {
         writecommand(ST7735_MADCTL);
         writedata(0xC0);
       }
       commandList(Rcmd3);
     }
}

/***************************************************************************************
** Function name:           commandList
** Description:             Get initialisation commands from FLASH and send to TFT
***************************************************************************************/
void TFT_ST7735::commandList (const uint8_t *addr)
{
	uint8_t  numCommands, numArgs;
	uint8_t  ms;

	spi_begin();
	numCommands = pgm_read_byte(addr++);            // Number of commands to follow
	while (numCommands--)                           // For each command...
	{
		writecommand(pgm_read_byte(addr++));    // Read, issue command
		numArgs = pgm_read_byte(addr++);        // Number of args to follow
		ms = numArgs & ST7735_INIT_DELAY;      // If hibit set, delay follows args
		numArgs &= ~ST7735_INIT_DELAY;         // Mask out delay bit
		while (numArgs--)                       // For each argument...
		{
			writedata(pgm_read_byte(addr++)); // Read, issue argument
		}

		if (ms)
		{
			ms = pgm_read_byte(addr++);     // Read post-command delay time (ms)
			delay( (ms==255 ? 500 : ms) );
		}
	}
	spi_end();
}

/***************************************************************************************
** Function name:           drawCircle
** Description:             Draw a circle outline
***************************************************************************************/
void TFT_ST7735::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = - r - r;
  int16_t x = 0;

  drawPixel(x0 + r, y0  , color);
  drawPixel(x0 - r, y0  , color);
  drawPixel(x0  , y0 - r, color);
  drawPixel(x0  , y0 + r, color);

  while (x < r) {
    if (f >= 0) {
      r--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + r, color);
    drawPixel(x0 - x, y0 + r, color);
    drawPixel(x0 - x, y0 - r, color);
    drawPixel(x0 + x, y0 - r, color);

    drawPixel(x0 + r, y0 + x, color);
    drawPixel(x0 - r, y0 + x, color);
    drawPixel(x0 - r, y0 - x, color);
    drawPixel(x0 + r, y0 - x, color);
  }
}

/***************************************************************************************
** Function name:           drawCircleHelper
** Description:             Support function for circle drawing
***************************************************************************************/
void TFT_ST7735::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;

  while (x < r) {
    if (f >= 0) {
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x8) {
      drawPixel(x0 - r, y0 + x, color);
      drawPixel(x0 - x, y0 + r, color);
    }
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + r, color);
      drawPixel(x0 + r, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + r, y0 - x, color);
      drawPixel(x0 + x, y0 - r, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - x, y0 - r, color);
      drawPixel(x0 - r, y0 - x, color);
    }

  }
}

/***************************************************************************************
** Function name:           fillCircle
** Description:             draw a filled circle
***************************************************************************************/
void TFT_ST7735::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  drawFastVLine(x0, y0 - r, r + r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

/***************************************************************************************
** Function name:           fillCircleHelper
** Description:             Support function for filled circle drawing
***************************************************************************************/
// Used to do circles and roundrects
void TFT_ST7735::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -r - r;
  int16_t x     = 0;

  delta++;
  while (x < r) {
    if (f >= 0) {
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0 + x, y0 - r, r + r + delta, color);
      drawFastVLine(x0 + r, y0 - x, x + x + delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0 - x, y0 - r, r + r + delta, color);
      drawFastVLine(x0 - r, y0 - x, x + x + delta, color);
    }
  }
}

/***************************************************************************************
** Function name:           drawEllipse
** Description:             Draw a ellipse outline
***************************************************************************************/
void TFT_ST7735::drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int16_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++)
  {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0)
    {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++)
  {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0)
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }
}

/***************************************************************************************
** Function name:           fillEllipse
** Description:             draw a filled ellipse
***************************************************************************************/
void TFT_ST7735::fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int16_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++)
  {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0)
    {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++)
  {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0)
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

}

/***************************************************************************************
** Function name:           fillScreen
** Description:             Clear the screen to defined colour
***************************************************************************************/
void TFT_ST7735::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
}

/***************************************************************************************
** Function name:           drawRect
** Description:             Draw a rectangle outline
***************************************************************************************/
// Draw a rectangle
void TFT_ST7735::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

/***************************************************************************************
** Function name:           drawRoundRect
** Description:             Draw a rounded corner rectangle outline
***************************************************************************************/
// Draw a rounded rectangle
void TFT_ST7735::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  drawFastHLine(x + r  , y    , w - r - r, color); // Top
  drawFastHLine(x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(x    , y + r  , h - r - r, color); // Left
  drawFastVLine(x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(x + r    , y + r    , r, 1, color);
  drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);
  drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);

}

/***************************************************************************************
** Function name:           fillRoundRect
** Description:             Draw a rounded corner filled rectangle
***************************************************************************************/
// Fill a rounded rectangle
void TFT_ST7735::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  // smarter version
  fillRect(x + r, y, w - r - r, h, color);

  // draw four corners
  fillCircleHelper(x + w - r - 1, y + r, r, 1, h - r - r - 1, color);
  fillCircleHelper(x + r    , y + r, r, 2, h - r - r - 1, color);
}

/***************************************************************************************
** Function name:           drawTriangle
** Description:             Draw a triangle outline using 3 arbitrary points
***************************************************************************************/
// Draw a triangle
void TFT_ST7735::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

/***************************************************************************************
** Function name:           fillTriangle 
** Description:             Draw a filled triangle using 3 arbitrary points
***************************************************************************************/
// Fill a triangle - original Adafruit function works well and code footprint is small
void TFT_ST7735::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    tftswap(y0, y1); tftswap(x0, x1);
  }
  if (y1 > y2) {
    tftswap(y2, y1); tftswap(x2, x1);
  }
  if (y0 > y1) {
    tftswap(y0, y1); tftswap(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)      a = x1;
    else if (x1 > b) b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;
    drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1;  // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) tftswap(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) tftswap(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }
}

/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
void TFT_ST7735::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{

  int16_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        drawPixel(x + i, y + j, color);
      }
    }
  }
}

/***************************************************************************************
** Function name:           setCursor
** Description:             Set the text cursor x,y position
***************************************************************************************/
void TFT_ST7735::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}

/***************************************************************************************
** Function name:           setCursor
** Description:             Set the text cursor x,y position and font
***************************************************************************************/
void TFT_ST7735::setCursor(int16_t x, int16_t y, uint8_t font)
{
  textfont = font;
  cursor_x = x;
  cursor_y = y;
}

/***************************************************************************************
** Function name:           setTextSize
** Description:             Set the text size multiplier
***************************************************************************************/
void TFT_ST7735::setTextSize(uint8_t s)
{
  if (s>7) s = 7; // Limit the maximum size multiplier so byte variables can be used for rendering
  textsize = (s > 0) ? s : 1; // Don't allow font size 0
}

/***************************************************************************************
** Function name:           setTextFont
** Description:             Set the font for the print stream
***************************************************************************************/
void TFT_ST7735::setTextFont(uint8_t f)
{
  textfont = (f > 0) ? f : 1; // Don't allow font 0
}

/***************************************************************************************
** Function name:           setTextColor
** Description:             Set the font foreground colour (background is transparent)
***************************************************************************************/
void TFT_ST7735::setTextColor(uint16_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

/***************************************************************************************
** Function name:           setTextColor
** Description:             Set the font foreground and background colour
***************************************************************************************/
void TFT_ST7735::setTextColor(uint16_t c, uint16_t b)
{
  textcolor   = c;
  textbgcolor = b;
}

/***************************************************************************************
** Function name:           setTextWrap
** Description:             Define if text should wrap at end of line
***************************************************************************************/
void TFT_ST7735::setTextWrap(boolean w)
{
  textwrap = w;
}

/***************************************************************************************
** Function name:           setTextDatum
** Description:             Set the text position reference datum
***************************************************************************************/
void TFT_ST7735::setTextDatum(uint8_t d)
{
  textdatum = d;
}

/***************************************************************************************
** Function name:           setTextPadding
** Description:             Define padding width (aids erasing old text and numbers)
***************************************************************************************/
void TFT_ST7735::setTextPadding(uint16_t x_width)
{
  padX = x_width;
}

/***************************************************************************************
** Function name:           getRotation
** Description:             Return the rotation value (as used by setRotation())
***************************************************************************************/
uint8_t TFT_ST7735::getRotation(void)
{
  return rotation;
}

/***************************************************************************************
** Function name:           width
** Description:             Return the pixel width of display (per current rotation)
***************************************************************************************/
// Return the size of the display (per current rotation)
int16_t TFT_ST7735::width(void)
{
  return _width;
}

/***************************************************************************************
** Function name:           height
** Description:             Return the pixel height of display (per current rotation)
***************************************************************************************/
int16_t TFT_ST7735::height(void)
{
  return _height;
}

/***************************************************************************************
** Function name:           textWidth
** Description:             Return the width in pixels of a string in a given font
***************************************************************************************/
int16_t TFT_ST7735::textWidth(char *string, int font)
{
  unsigned int str_width  = 0;
  char uniCode;
  char *widthtable;

  if (font>1 && font<9)
  widthtable = (char *)pgm_read_word( &(fontdata[font].widthtbl ) ) - 32; //subtract the 32 outside the loop

  while (*string)
  {
    uniCode = *(string++);
#ifdef LOAD_GLCD
    if (font == 1) str_width += 6;
    else
#endif
    str_width += pgm_read_byte( widthtable + uniCode); // Normally we need to subract 32 from uniCode
  }
  return str_width * textsize;
}

/***************************************************************************************
** Function name:           fontsLoaded
** Description:             return an encoded 16 bit value showing the fonts loaded
***************************************************************************************/
// Returns a value showing which fonts are loaded (bit N set =  Font N loaded)

uint16_t TFT_ST7735::fontsLoaded(void)
{
  return fontsloaded;
}


/***************************************************************************************
** Function name:           fontHeight
** Description:             return the height of a font
***************************************************************************************/
int16_t TFT_ST7735::fontHeight(int font)
{
  return pgm_read_byte( &fontdata[font].height ) * textsize;
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a single character in the Adafruit GLCD font
***************************************************************************************/
void TFT_ST7735::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
#ifdef LOAD_GLCD
  if ((x >= _width)            || // Clip right
      (y >= _height)           || // Clip bottom
      ((x + 6 * size - 1) < 0) || // Clip left
      ((y + 8 * size - 1) < 0))   // Clip top
    return;
  boolean fillbg = (bg != color);

spi_begin();

// This is about 5 times faster for textsize=1 with background (at 200us per character)
  if ((size==1) && fillbg)
  {
    byte column[6];
    byte mask = 0x1;
    setWindow(x, y, x+5, y+8);
    for (int8_t i = 0; i < 5; i++ ) column[i] = pgm_read_byte(font + (c * 5) + i);
    column[5] = 0;

    for (int8_t j = 0; j < 8; j++) {
      for (int8_t k = 0; k < 5; k++ ) {
        if (column[k] & mask) {
          while (!(SPSR & _BV(SPIF)));
          SPDR = color >> 8; asm volatile( "nop\n\t" ::); // Sync to SPIF bit
          while (!(SPSR & _BV(SPIF)));
          SPDR = color;
        }
        else {
          while (!(SPSR & _BV(SPIF)));
          SPDR = bg >> 8; asm volatile( "nop\n\t" ::);
          while (!(SPSR & _BV(SPIF)));
          SPDR = bg;
        }
      }

      mask <<= 1;
      while (!(SPSR & _BV(SPIF)));
      SPDR = bg >> 8; while (!(SPSR & _BV(SPIF)));
      SPDR = bg;
    }
    while (!(SPSR & _BV(SPIF)));
  }
  else
  {
    if (size == 1) // default size
    {
      for (int8_t i = 0; i < 5; i++ ) {
        uint8_t line = pgm_read_byte(font + c*5 + i);
        if (line & 0x1)  drawPixel(x + i, y, color);
        if (line & 0x2)  drawPixel(x + i, y + 1, color);
        if (line & 0x4)  drawPixel(x + i, y + 2, color);
        if (line & 0x8)  drawPixel(x + i, y + 3, color);
        if (line & 0x10) drawPixel(x + i, y + 4, color);
        if (line & 0x20) drawPixel(x + i, y + 5, color);
        if (line & 0x40) drawPixel(x + i, y + 6, color);
        if (line & 0x80) drawPixel(x + i, y + 7, color);
      }
    }
    else {  // big size
      for (int8_t i = 0; i < 5; i++ ) {
        uint8_t line = pgm_read_byte(font + c*5 + i);
        for (int8_t j = 0; j < 8; j++) {
          if (line & 0x1) fillRect(x + (i * size), y + (j * size), size, size, color);
          else if (fillbg) fillRect(x + i * size, y + j * size, size, size, bg);
          line >>= 1;
        }
      }
    }
  }

spi_end();

#endif // LOAD_GLCD
}

/***************************************************************************************
** Function name:           setAddrWindow
** Description:             define an area to receive a stream of pixels
***************************************************************************************/
// Chip select is high at the end of this function

void TFT_ST7735::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  spi_begin();
  setWindow(x0, y0, x1, y1);
  TFT_CS_H;
  while (!(SPSR & _BV(SPIF)));
  spi_end();
}

/***************************************************************************************
** Function name:           setWindow
** Description:             define an area to receive a stream of pixels
***************************************************************************************/
// Chip select stays low, use setAddrWindow() from sketches

void TFT_ST7735::setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{

  addr_row = 0xFF;
  addr_col = 0xFF;

  // Column addr set
  TFT_DC_C;
  TFT_CS_L;

  SPDR = ST7735_CASET;
  spiWait15();

  TFT_DC_D;
  SPDR = 0; spiWait14();
  SPDR = x0+colstart; spiWait17();
  SPDR = 0; spiWait14();
  SPDR = x1+colstart; spiWait14();

  // Row addr set
  TFT_DC_C;
  SPDR = ST7735_RASET; spiWait15();

  TFT_DC_D;
  SPDR = 0; spiWait14();
  SPDR = y0+rowstart; spiWait17();
  SPDR = 0; spiWait14();
  SPDR = y1+rowstart; spiWait14();

  // write to RAM
  TFT_DC_C;
  SPDR = ST7735_RAMWR; spiWait14();

  TFT_DC_D;

}

/***************************************************************************************
** Function name:           drawPixel
** Description:             push a single pixel at an arbitrary position
***************************************************************************************/
// Smarter version that takes advantage of often used orthogonal coordinate plots
// where either x or y does not change
void TFT_ST7735::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  // Faster range checking, possible because x and y are unsigned
  if ((x >= _width) || (y >= _height)) return;
  spi_begin();

  TFT_DC_C;
  TFT_CS_L;

if (addr_col != x) {
  SPDR = ST7735_CASET;
  spiWait12();
  addr_col = x;
  TFT_DC_D;
  SPDR = 0; spiWait14();
  SPDR = x+colstart; spiWait14(); // spiWait17() if we include the next 2 lines
  //SPDR = 0; spiWait14();
  //SPDR = x+colstart; spiWait14();

  TFT_DC_C;
}

if (addr_row != y) {
  SPDR = ST7735_RASET;
  spiWait12();
  addr_row = y;
  TFT_DC_D;
  SPDR = 0; spiWait14();
  SPDR = y+rowstart; spiWait14(); // spiWait17() if we include the next 2 lines
  //SPDR = 0; spiWait14();
  //SPDR = y+rowstart; spiWait14();

  TFT_DC_C;
}

  SPDR = ST7735_RAMWR; spiWait15();

  TFT_DC_D;

  SPDR = color >> 8; spiWait17();
  SPDR = color; spiWait14();

  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           pushColor
** Description:             push a single pixel
***************************************************************************************/
void TFT_ST7735::pushColor(uint16_t color)
{
  spi_begin();

  TFT_CS_L;

  //uint8_t backupSPCR =SPCR;
  //SPCR = mySPCR;

  SPDR = color>>8;
  while (!(SPSR & _BV(SPIF)));
  SPDR = color;
  while (!(SPSR & _BV(SPIF)));

  //SPCR = backupSPCR;

  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           pushColor
** Description:             push a single colour to "len" pixels
***************************************************************************************/
void TFT_ST7735::pushColor(uint16_t color, uint16_t len)
{
  spi_begin();

  TFT_CS_L;
  spiWrite16(color, len);
  TFT_CS_H;
  while (!(SPSR & _BV(SPIF)));

  spi_end();
}

/***************************************************************************************
** Function name:           pushColors
** Description:             push an aray of pixels for BMP image drawing
***************************************************************************************/
// Sends an array of 16-bit color values to the TFT; used
// externally by BMP examples.  Assumes that setAddrWindow() has
// previously been called to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).

void TFT_ST7735::pushColors(uint16_t *data, uint8_t len)
{
  uint16_t color;
  spi_begin();

  TFT_CS_L;

  while (len--) {
    color = *(data++);
    SPDR = color >> 8;
    spiWait17(); // Wait 17 clock cycles
    SPDR = color;
    // Wait 9 clock cycles
    asm volatile
    (
      "	rcall	1f     \n"	// 7
      "	rjmp 	2f     \n"	// 9
      "1:	ret    \n"	//
      "2:	       \n"	//
    );
  }
  while (!(SPSR & _BV(SPIF)));

  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           pushColors
** Description:             push an aray of pixels for 16 bit raw image drawing
***************************************************************************************/
// Assumed that setAddrWindow() has previously been called

void TFT_ST7735::pushColors(uint8_t *data, uint16_t len)
{
  spi_begin();
  len = len<<1;

  TFT_CS_L;
  while (len--) {
    SPDR = *(data++);
    // Wait 11 clock cycles
    asm volatile
    (
      "	adiw	r24,0  \n"	// 2
      "	rcall	1f     \n"	// 9
      "	rjmp 	2f     \n"	// 11
      "1:	ret    \n"	//
      "2:	       \n"	//
    );
  }
  TFT_CS_H;

  while (!(SPSR & _BV(SPIF)));
  spi_end();
}

/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
***************************************************************************************/

// Bresenham's algorithm - thx wikipedia - speed enhanced by Bodmer to use
// an eficient FastH/V Line draw routine for line segments of 2 pixels or more
// enhanced further using code from Xark and Spellbuilder (116 byte penalty)

// Select which version, fastest or compact
#ifdef FAST_LINE

void TFT_ST7735::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  spi_begin();

  int8_t steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep) {
		tftswap(x0, y0);
		tftswap(x1, y1);
	}

	if (x0 > x1) {
		tftswap(x0, x1);
		tftswap(y0, y1);
	}

	if (x1 < 0) return;

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int8_t ystep = (y0 < y1) ? 1 : (-1);

	if (steep)	// y increments every iteration (y0 is x-axis, and x0 is y-axis)
	{
	  if (x1 >= _height) x1 = _height - 1;

	  for (; x0 <= x1; x0++) {
		if ((x0 >= 0) && (y0 >= 0) && (y0 < _width)) break;
		err -= dy;
		if (err < 0) {
			err += dx;
			y0 += ystep;
		}
	  }

	  if (x0 > x1) return;

           setWindow(y0, x0, y0, _height);
		for (; x0 <= x1; x0++) {
			spiWrite16s(color);
			err -= dy;
			if (err < 0) {
				y0 += ystep;
				if ((y0 < 0) || (y0 >= _width)) break;
				err += dx;
			     //while (!(SPSR & _BV(SPIF))); // Safe, but can comment out and rely on delay
                     setWindow(y0, x0+1, y0, _height);
			}
		}
	}
	else	// x increments every iteration (x0 is x-axis, and y0 is y-axis)
	{
	  if (x1 >= _width) x1 = _width - 1;

	  for (; x0 <= x1; x0++) {
		if ((x0 >= 0) && (y0 >= 0) && (y0 < _height)) break;
		err -= dy;
		if (err < 0) {
			err += dx;
			y0 += ystep;
		}
	  }

	  if (x0 > x1) return;

           setWindow(x0, y0, _width, y0);
		for (; x0 <= x1; x0++) {
			spiWrite16s(color);
			err -= dy;
			if (err < 0) {
				y0 += ystep;
				if ((y0 < 0) || (y0 >= _height)) break;
				err += dx;
			     //while (!(SPSR & _BV(SPIF))); // Safe, but can comment out and rely on delay
                     setWindow(x0+1, y0, _width, y0);
			}
		}
	}
      TFT_CS_H;
  spi_end();
}

#else // FAST_LINE not defined so use more compact version

// Slower but more compact line drawing function
void TFT_ST7735::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    tftswap(x0, y0);
    tftswap(x1, y1);
  }

  if (x0 > x1) {
    tftswap(x0, x1);
    tftswap(y0, y1);
  }

  int16_t dx = x1 - x0, dy = abs(y1 - y0);;


  int16_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;
  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastHLine(xs, y0, dlen, color);
  }
}

#endif // FAST_LINE option

/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
***************************************************************************************/
void TFT_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
#ifdef CLIP_CHECK
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((y + h - 1) >= _height) h = _height - y;
#endif

  spi_begin();

  setWindow(x, y, x, _height);

  spiWrite16(color, h);
  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
***************************************************************************************/
void TFT_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
#ifdef CLIP_CHECK
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width - x;
#endif

  spi_begin();
  setWindow(x, y, _width, y);

  spiWrite16(color, w);
  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void TFT_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
#ifdef CLIP_CHECK
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x > _width) || (y > _height) || (w==0) || (h==0)) return;
  if ((x + w - 1) > _width)  w = _width  - x;
  if ((y + h - 1) > _height) h = _height - y;
#endif

  spi_begin();
  setWindow(x, y, x + w - 1, y + h - 1);

  if (h > w) tftswap(h, w);

  while (h--) spiWrite16(color, w);
  TFT_CS_H;

  spi_end();
}

/***************************************************************************************
** Function name:           color565
** Description:             convert three 8 bit RGB levels to a 16 bit colour value
***************************************************************************************/
uint16_t TFT_ST7735::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/***************************************************************************************
** Function name:           setRotation
** Description:             rotate the screen orientation m = 0-3 or 4-7 for BMP drawing
***************************************************************************************/

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void TFT_ST7735::setRotation(uint8_t m)
{
  byte spsr = SPSR;// We need this here for some reason...
  addr_row = 0xFF;
  addr_col = 0xFF;

  rotation = m % 4;
  spi_begin();
  writecommand(ST7735_MADCTL);
  switch (rotation) {
    case 0:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
       colstart = 2;
       rowstart = 1;
     } else {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     }
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 1:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
       colstart = 1;
       rowstart = 2;
     } else {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     }
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;
    case 2:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_RGB);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(MADCTL_RGB);
       colstart = 2;
       rowstart = 1;
     } else {
       writedata(MADCTL_BGR);
     }
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 3:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
     } else if(tabcolor == INITR_GREENTAB2) {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
       colstart = 1;
       rowstart = 2;
     } else {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     }
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;

  // These next rotations are for bottum up BMP drawing
  /*  case 4:
      writedata(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR);
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 5:
      writedata(ST7735_MADCTL_MV | ST7735_MADCTL_MX | ST7735_MADCTL_BGR);
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;
    case 6:
      writedata(ST7735_MADCTL_BGR);
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 7:
      writedata(ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR);
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;
  */
  }
  spi_end();
}

/***************************************************************************************
** Function name:           invertDisplay
** Description:             invert the display colours i = 1 invert, i = 0 normal
***************************************************************************************/
void TFT_ST7735::invertDisplay(boolean i)
{
  spi_begin();
  // Send the command twice as otherwise it does not always work!
  writecommand(i ? ST7735_INVON : ST7735_INVOFF);
  writecommand(i ? ST7735_INVON : ST7735_INVOFF);
  spi_end();
}

/***************************************************************************************
** Function name:           write
** Description:             draw characters piped through serial stream
***************************************************************************************/
size_t TFT_ST7735::write(uint8_t uniCode)
{
  if (uniCode == '\r') return 1;
  unsigned int width = 0;
  unsigned int height = 0;
  //Serial.print((char) uniCode); // Debug line sends all printed TFT text to serial port

#ifdef LOAD_FONT2
  if (textfont == 2)
  {
      // This is 20us faster than using the fontdata structure (0.443ms per character instead of 0.465ms)
      width = pgm_read_byte(widtbl_f16 + uniCode-32);
      height = chr_hgt_f16;
      // Font 2 is rendered in whole byte widths so we must allow for this
      width = (width + 6) / 8;  // Width in whole bytes for font 2, should be + 7 but must allow for font width change
      width = width * 8;        // Width converted back to pixles
  }
  #ifdef LOAD_RLE
  else
  #endif
#endif


#ifdef LOAD_RLE
  {
      // Uses the fontinfo struct array to avoid lots of 'if' or 'switch' statements
      // A tad slower than above but this is not significant and is more convenient for the RLE fonts
      // Yes, this code can be needlessly executed when textfont == 1...
      width = pgm_read_byte( pgm_read_word( &(fontdata[textfont].widthtbl ) ) + uniCode-32 );
      height= pgm_read_byte( &fontdata[textfont].height );
  }
#endif

#ifdef LOAD_GLCD
  if (textfont==1)
  {
      width =  6;
      height = 8;
  }
#else
  if (textfont==1) return 0;
#endif

  height = height * textsize;

  if (uniCode == '\n') {
    cursor_y += height;
    cursor_x  = 0;
  }
  else
  {
    if (textwrap && (cursor_x + width * textsize >= _width))
    {
      cursor_y += height;
      cursor_x = 0;
    }
    cursor_x += drawChar(uniCode, cursor_x, cursor_y, textfont);
  }
  return 1;
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
***************************************************************************************/
int TFT_ST7735::drawChar(unsigned int uniCode, int x, int y, int font)
{

  if (font==1)
  {
#ifdef LOAD_GLCD
      drawChar(x, y, uniCode, textcolor, textbgcolor, textsize);
      return 6 * textsize;
#else
      return 0;
#endif
  }

  unsigned int width  = 0;
  unsigned int height = 0;
  unsigned int flash_address = 0; // 16 bit address OK for Arduino if font files <60K
  uniCode -= 32;

#ifdef LOAD_FONT2
  if (font == 2)
  {
      // This is 20us faster than using the fontdata structure (0.413ms per character instead of 0.433ms)
      flash_address = pgm_read_word(&chrtbl_f16[uniCode]);
      width = pgm_read_byte(widtbl_f16 + uniCode);
      height = chr_hgt_f16;
  }
  #ifdef LOAD_RLE
  else
  #endif
#endif

#ifdef LOAD_RLE
  {
      // This is slower than above but is more convenient for the RLE fonts
      flash_address = pgm_read_word( pgm_read_word( &(fontdata[font].chartbl ) ) + uniCode*sizeof(void *) );
      width = pgm_read_byte( pgm_read_word( &(fontdata[font].widthtbl ) ) + uniCode );
      height= pgm_read_byte( &fontdata[font].height );
  }
#endif

  int w = width;
  int pX      = 0;
  int pY      = y;
  byte line = 0;

  byte tl = textcolor;
  byte th = textcolor >> 8;
  byte bl = textbgcolor;
  byte bh = textbgcolor >> 8;

#ifdef LOAD_FONT2 // chop out 962 bytes of code if we do not need it
  if (font == 2) {
    w = w + 6; // Should be + 7 but we need to compensate for width increment
    w = w / 8;
    if (x + width * textsize >= _width) return width * textsize ;

    if (textcolor == textbgcolor || textsize != 1) {

      for (int i = 0; i < height; i++)
      {
        if (textcolor != textbgcolor) fillRect(x, pY, width * textsize, textsize, textbgcolor);

        for (int k = 0; k < w; k++)
        {
          line = pgm_read_byte(flash_address + w * i + k);
          if (line) {
            if (textsize == 1) {
              pX = x + k * 8;
              if (line & 0x80) drawPixel(pX, pY, textcolor);
              if (line & 0x40) drawPixel(pX + 1, pY, textcolor);
              if (line & 0x20) drawPixel(pX + 2, pY, textcolor);
              if (line & 0x10) drawPixel(pX + 3, pY, textcolor);
              if (line & 0x08) drawPixel(pX + 4, pY, textcolor);
              if (line & 0x04) drawPixel(pX + 5, pY, textcolor);
              if (line & 0x02) drawPixel(pX + 6, pY, textcolor);
              if (line & 0x01) drawPixel(pX + 7, pY, textcolor);
            }
            else {
              pX = x + k * 8 * textsize;
              if (line & 0x80) fillRect(pX, pY, textsize, textsize, textcolor);
              if (line & 0x40) fillRect(pX + textsize, pY, textsize, textsize, textcolor);
              if (line & 0x20) fillRect(pX + 2 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x10) fillRect(pX + 3 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x08) fillRect(pX + 4 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x04) fillRect(pX + 5 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x02) fillRect(pX + 6 * textsize, pY, textsize, textsize, textcolor);
              if (line & 0x01) fillRect(pX + 7 * textsize, pY, textsize, textsize, textcolor);
            }
          }
        }
        pY += textsize;
      }
    }
    else
      // Faster drawing of characters and background using block write
    {
      spi_begin();
      setWindow(x, y, (x + w * 8) - 1, y + height - 1);

      byte mask;
      for (int i = 0; i < height; i++)
      {
        for (int k = 0; k < w; k++)
        {
          line = pgm_read_byte(flash_address + w * i + k);
          pX = x + k * 8;
          mask = 0x80;
          while (mask) {
            if (line & mask) {
              while (!(SPSR & _BV(SPIF)));
              SPDR = th; asm volatile( "nop\n\t" ::);
              while (!(SPSR & _BV(SPIF)));
              SPDR = tl;
            }
            else {
              while (!(SPSR & _BV(SPIF)));
              SPDR = bh; asm volatile( "nop\n\t" ::);
              while (!(SPSR & _BV(SPIF)));
              SPDR = bl;
            }
            mask = mask >> 1;
          }
        }
        pY += textsize;
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
      spi_end();
    }
  }

  #ifdef LOAD_RLE
  else
  #endif
#endif  //FONT2

#ifdef LOAD_RLE  //674 bytes of code
  // Font is not 2 and hence is RLE encoded
  {
    spi_begin();
    SPDR = 0; // Dummy write to ensure SPIF flag gets set for first check in while() loop
    w *= height; // Now w is total number of pixels in the character
    if ((textsize != 1) || (textcolor == textbgcolor)) {
      if (textcolor != textbgcolor) fillRect(x, pY, width * textsize, textsize * height, textbgcolor);
      int px = 0, py = pY, tpy = pY; // To hold character block start and end column and row values
      int pc = 0; // Pixel count
      byte np = textsize * textsize; // Number of pixels in a drawn pixel

      byte tnp = 0; // Temporary copy of np for while loop
      byte ts = textsize - 1; // Temporary copy of textsize
      // 16 bit pixel count so maximum font size is equivalent to 180x180 pixels in area
      // w is total number of pixels to plot to fill character block
      while (pc < w)
      {
        line = pgm_read_byte(flash_address);
        flash_address++; // 20 bytes smaller by incrementing here
        if (line & 0x80) {
          line &= 0x7F;
          line++;
          if (ts) {
            px = x + textsize * (pc % width); // Keep these px and py calculations outside the loop as they are slow
            py = y + textsize * (pc / width);
          }
          else {
            px = x + pc % width; // Keep these px and py calculations outside the loop as they are slow
            py = y + pc / width;
          }
          while (line--) { // In this case the while(line--) is faster
            pc++; // This is faster than putting pc+=line before while() as we use up SPI wait time
            while (!(SPSR & _BV(SPIF)));
            setWindow(px, py, px + ts, py + ts);

            if (ts) {
              tnp = np;
              while (tnp--) {
                while (!(SPSR & _BV(SPIF)));
                SPDR = th;
                while (!(SPSR & _BV(SPIF)));
                SPDR = tl;
              }
            }
            else {
              while (!(SPSR & _BV(SPIF)));
              SPDR = th;
              while (!(SPSR & _BV(SPIF)));
              SPDR = tl;
            }
            px += textsize;

            if (px >= (x + width * textsize))
            {
              px = x;
              py += textsize;
            }
          }
        }
        else {
          line++;
          pc += line;
        }
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
      spi_end();
    }
    else // Text colour != background && textsize = 1
         // so use faster drawing of characters and background using block write
    {
      spi_begin();
      setWindow(x, y, x + width - 1, y + height - 1);

      // Maximum font size is equivalent to 180x180 pixels in area
      while (w > 0)
      {
        line = pgm_read_byte(flash_address++); // 8 bytes smaller when incrementing here
        if (line & 0x80) {
          line &= 0x7F;
          line++; w -= line;
          spiWrite16(textcolor, line);
        }
        else {
          line++; w -= line;
          spiWrite16(textbgcolor, line);
        }
      }
      while (!(SPSR & _BV(SPIF)));
      writeEnd();
      spi_end();
    }
  }
  // End of RLE font rendering
#endif
  return width * textsize;    // x +
}

/***************************************************************************************
** Function name:           drawString
** Description :            draw string with padding if it is defined
***************************************************************************************/
int TFT_ST7735::drawString(char *string, int poX, int poY, int font)
{
  int16_t sumX = 0;
  uint8_t padding = 1;
  unsigned int cheight = 0;

  if (textdatum || padX)
  {
    // Find the pixel width of the string in the font
    unsigned int cwidth  = textWidth(string, font);

    // Get the pixel height of the font
    cheight = pgm_read_byte( &fontdata[font].height ) * textsize;

    switch(textdatum) {
      case TC_DATUM:
        poX -= cwidth/2;
        padding = 2;
        break;
      case TR_DATUM:
        poX -= cwidth;
        padding = 3;
        break;
      case ML_DATUM:
        poY -= cheight/2;
        padding = 1;
        break;
      case MC_DATUM:
        poX -= cwidth/2;
        poY -= cheight/2;
        padding = 2;
        break;
      case MR_DATUM:
        poX -= cwidth;
        poY -= cheight/2;
        padding = 3;
        break;
      case BL_DATUM:
        poY -= cheight;
        padding = 1;
        break;
      case BC_DATUM:
        poX -= cwidth/2;
        poY -= cheight;
        padding = 2;
        break;
      case BR_DATUM:
        poX -= cwidth;
        poY -= cheight;
        padding = 3;
        break;
    }
    // Check coordinates are OK, adjust if not
    if (poX < 0) poX = 0;
    if (poX+cwidth>_width)   poX = _width - cwidth;
    if (poY < 0) poY = 0;
    if (poY+cheight>_height) poY = _height - cheight;
  }

  while (*string) sumX += drawChar(*(string++), poX+sumX, poY, font);

//#define PADDING_DEBUG

#ifndef PADDING_DEBUG
  if((padX>sumX) && (textcolor!=textbgcolor))
  {
    int padXc = poX+sumX; // Maximum left side padding
    switch(padding) {
      case 1:
        fillRect(padXc,poY,padX-sumX,cheight, textbgcolor);
        break;
      case 2:
        fillRect(padXc,poY,(padX-sumX)>>1,cheight, textbgcolor);
        padXc = (padX-sumX)>>1;
        if (padXc>poX) padXc = poX;
        fillRect(poX - padXc,poY,(padX-sumX)>>1,cheight, textbgcolor);
        break;
      case 3:
        if (padXc>padX) padXc = padX;
        fillRect(poX + sumX - padXc,poY,padXc-sumX,cheight, textbgcolor);
        break;
    }
  }
#else

  // This is debug code to show text (green box) and blanked (white box) areas
  // to show that the padding areas are being correctly sized and positioned
  if((padX>sumX) && (textcolor!=textbgcolor))
  {
    int padXc = poX+sumX; // Maximum left side padding
    drawRect(poX,poY,sumX,cheight, TFT_GREEN);
    switch(padding) {
      case 1:
        drawRect(padXc,poY,padX-sumX,cheight, TFT_WHITE);
        break;
      case 2:
        drawRect(padXc,poY,(padX-sumX)>>1, cheight, TFT_WHITE);
        padXc = (padX-sumX)>>1;
        if (padXc>poX) padXc = poX;
        drawRect(poX - padXc,poY,(padX-sumX)>>1,cheight, TFT_WHITE);
        break;
      case 3:
        if (padXc>padX) padXc = padX;
        drawRect(poX + sumX - padXc,poY,padXc-sumX,cheight, TFT_WHITE);
        break;
    }
  }
#endif

return sumX;
}

/***************************************************************************************
** Function name:           drawCentreString
** Descriptions:            draw string centred on dX
***************************************************************************************/
int TFT_ST7735::drawCentreString(char *string, int dX, int poY, int font)
{
  byte tempdatum = textdatum;
  int sumX = 0;
  textdatum = TC_DATUM;
  sumX = drawString(string, dX, poY, font);
  textdatum = tempdatum;
  return sumX;
}

/***************************************************************************************
** Function name:           drawRightString
** Descriptions:            draw string right justified to dX
***************************************************************************************/
int TFT_ST7735::drawRightString(char *string, int dX, int poY, int font)
{
  byte tempdatum = textdatum;
  int sumX = 0;
  textdatum = TR_DATUM;
  sumX = drawString(string, dX, poY, font);
  textdatum = tempdatum;
  return sumX;
}

/***************************************************************************************
** Function name:           drawNumber
** Description:             draw a long integer
***************************************************************************************/
int TFT_ST7735::drawNumber(long long_num, int poX, int poY, int font)
{
  char str[12];
  ltoa(long_num, str, 10);
  return drawString(str, poX, poY, font);
}

/***************************************************************************************
** Function name:           drawFloat
** Descriptions:            drawFloat, prints 7 non zero digits maximum
***************************************************************************************/
// Adapted to assemble and print a string, this permits alignment relative to a datum
// looks complicated but much more compact and actually faster than using print class
int TFT_ST7735::drawFloat(float floatNumber, int dp, int poX, int poY, int font)
{
  char str[14];               // Array to contain decimal string
  uint8_t ptr = 0;            // Initialise pointer for array
  int8_t  digits = 1;         // Count the digits to avoid array overflow
  float rounding = 0.5;       // Round up down delta

  if (dp > 7) dp = 7; // Limit the size of decimal portion

  // Adjust the rounding value
  for (uint8_t i = 0; i < dp; ++i) rounding /= 10.0;

  if (floatNumber < -rounding)    // add sign, avoid adding - sign to 0.0!
  {
    str[ptr++] = '-'; // Negative number
    str[ptr] = 0; // Put a null in the array as a precaution
    digits = 0;   // Set digits to 0 to compensate so pointer value can be used later
    floatNumber = -floatNumber; // Make positive
  }

  floatNumber += rounding; // Round up or down

  // For error put ... in string and return (all TFT_ST7735 library fonts contain . character)
  if (floatNumber >= 2147483647) {
    strcpy(str, "...");
    return drawString(str, poX, poY, font);
  }
  // No chance of overflow from here on

  // Get integer part
  unsigned long temp = (unsigned long)floatNumber;

  // Put integer part into array
  ltoa(temp, str + ptr, 10);

  // Find out where the null is to get the digit count loaded
  while ((uint8_t)str[ptr] != 0) ptr++; // Move the pointer along
  digits += ptr;                  // Count the digits

  str[ptr++] = '.'; // Add decimal point
  str[ptr] = '0';   // Add a dummy zero
  str[ptr + 1] = 0; // Add a null but don't increment pointer so it can be overwritten

  // Get the decimal portion
  floatNumber = floatNumber - temp;

  // Get decimal digits one by one and put in array
  // Limit digit count so we don't get a false sense of resolution
  uint8_t i = 0;
  while ((i < dp) && (digits < 9)) // while (i < dp) for no limit but array size must be increased
  {
    i++;
    floatNumber *= 10;       // for the next decimal
    temp = floatNumber;      // get the decimal
    ltoa(temp, str + ptr, 10);
    ptr++; digits++;         // Increment pointer and digits count
    floatNumber -= temp;     // Remove that digit
  }
  
  // Finally we can plot the string and return pixel length
  return drawString(str, poX, poY, font);
}

/***************************************************************************************
** Function name:           spiWrite16
** Descriptions:            Delay based assembler loop for fast SPI write
***************************************************************************************/
inline void spiWrite16(uint16_t data, int16_t count)
{
// We can enter this loop with 0 pixels to draw, so we need to check this
// if(count<1) { Serial.print("#### Less than 1 ####"); Serial.println(count);}

  uint8_t temp;
  asm volatile
  (
    "	sbiw	%[count],0\n"			// test count
    //"	brmi	2f\n"					// if < 0 then done
    "	breq	2f\n"					// if == 0 then done

    "1:	out	%[spi],%[hi]\n"		// write SPI data
    "	rcall	3f      \n" // 7
    "	rcall	3f      \n" // 14
    "	rjmp 	4f      \n" // 16
    "3:	ret     \n" // 
    "4:	nop     \n" // 17

    "	out	%[spi],%[lo]\n"			// write SPI data

    "	adiw	r24,0	  \n"	// 2
    "	adiw	r24,0  \n"	// 4
    "	rcall	5f     \n"	// 11
    "	rjmp 	6f     \n"	// 13
    "5:	ret    \n"	// 
    "6:	       \n"

    "	sbiw	%[count],1 \n" // 15 decrement count
    "	brne	1b         \n" // 17 if != 0 then loop

    "2:\n"

    : [temp] "=d" (temp), [count] "+w" (count)
    : [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
    :
  );
}

/***************************************************************************************
** Function name:           spiWrite16s
** Descriptions:            Write 16 bits, do not wait after last byte sent
***************************************************************************************/
inline void spiWrite16s(uint16_t data)
{
  uint8_t temp;
  asm volatile
  (
    "out	%[spi],%[hi]\n"		// write SPI data
    "	rcall	3f      \n" // 7
    "	rcall	3f      \n" // 14
    "	rjmp 	4f      \n" // 16
    "3:	ret     \n" // 
    "4:	nop     \n" // 17

    "	out	%[spi],%[lo]\n"			// write SPI data
    "	nop         \n"	// 1

    " adiw  r24,0	  \n"	// 3
    " adiw  r24,0      \n"	// 5

    "5:\n"
    : [temp] "=d" (temp)
    : [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
    :
  );
}


/***************************************************************************************
** Function name:           spiWrite16R with hi<>lo reversed (not used)
** Descriptions:            Delay based assembler loop for fast SPI write
***************************************************************************************/
inline void spiWrite16R(uint16_t data, int16_t count)
{
// We can enter this loop with 0 pixels to draw, so we need to check this
// if(count<1) { Serial.print("#### Less than 1 ####"); Serial.println(count);}

  uint8_t temp;
  asm volatile
  (
    "sbiw %[count],0   \n"	// test count
    //"brmi 2f         \n"	// if < 0 then done, we use unsigned though
    " breq	2f          \n" // if == 0 then done

    "1:	out	%[spi],%[hi]\n"		// write SPI data

    " rcall	3f     \n" // 7
    " rcall	3f     \n" // 14
    " rjmp 	4f     \n" // 16
    "3:	ret         \n"	// 
    "4:	nop         \n"	// 17

    " out	%[spi],%[lo]\n"			// write SPI data

    " adiw  r24,0	  \n"	// 2
    " adiw  r24,0      \n"	// 4
    " rcall 5f         \n"	// 11
    " rjmp  6f         \n"	// 13
    "5:  ret           \n"	// 
    "6:                \n"

    "	sbiw	%[count],1  \n" // 15 decrement count
    "	brne	1b          \n"	// 17 if != 0 then loop

    "2:\n"

    : [temp] "=d" (temp), [count] "+w" (count)
    : [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)(data>>8)), [hi] "r" ((uint8_t)data)
    :
  );
}

/***************************************************************************************
** Function name:           spiWait
** Descriptions:            17 cycle delay
***************************************************************************************/
inline void spiWait17(void)
{
  asm volatile
  (
    "	rcall	1f    \n" // 7
    "	rcall	1f    \n" // 14
    "	rjmp 	2f    \n" // 16
    "1:	ret   \n" // 
    "2:	nop	 \n" // 17
  );
}

/***************************************************************************************
** Function name:           spiWait
** Descriptions:            15 cycle delay
***************************************************************************************/
inline void spiWait15(void)
{
  asm volatile
  (
    "	adiw	r24,0  \n"	// 2
    "	adiw	r24,0  \n"	// 4
    "	adiw	r24,0  \n"	// 6
    "	rcall	1f     \n"	// 13
    "	rjmp 	2f     \n"	// 15
    "1:	ret    \n"	//
    "2:	       \n"	//
  );
}

/***************************************************************************************
** Function name:           spiWait
** Descriptions:            14 cycle delay
***************************************************************************************/
inline void spiWait14(void)
{
  asm volatile
  (
    "	nop         \n"	// 1
    "	adiw	r24,0  \n"	// 3
    "	adiw	r24,0  \n"	// 5
    "	rcall	1f     \n"	// 12
    "	rjmp 	2f     \n"	// 14
    "1:	ret    \n"	//
    "2:	       \n"	//
  );
}

/***************************************************************************************
** Function name:           spiWait
** Descriptions:            12 cycle delay
***************************************************************************************/
inline void spiWait12(void)
{
  asm volatile
  (
    "	nop         \n"	// 1
    "	adiw	r24,0  \n"	// 3
    "	rcall	1f     \n"	// 10
    "	rjmp 	2f     \n"	// 12
    "1:	ret    \n"	//
    "2:	       \n"	//
  );
}

/***************************************************

  ORIGINAL LIBRARY HEADER

  This is our library for the Adafruit  ST7735 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

 ****************************************************/