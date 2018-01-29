#include "common.h"
#include "Adafruit_ST7735.hpp"
#include <limits.h>
#include "SPI.h"

inline uint16_t swapcolor(uint16_t x) { 
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}


Adafruit_ST7735(SPI * spi, uint8_t resetPin, uint8_t cdPin, uint8_t displayID): Adafruit_GFX(ST7735_TFTWIDTH_128, ST7735_TFTHEIGHT_160){
	_spi = spi;
	_rst = resetPin;
	_cdPin = cdPin;
	_displayID = displayID;
}



void Adafruit_ST7735::writecommand(uint8_t c) {
	pinSet(_cdPin, LOW);
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(data);
	_spi->selectSlave(_displayID, DISABLE);
	pinSet(_cdPin, HIGH);
}


void Adafruit_ST7735::writedata(uint8_t c) {
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(data);
	_spi->selectSlave(_displayID, DISABLE);
}

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
static const uint8_t PROGMEM
  

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

  Rcmd2green144[] = {              // Init for 7735R, part 2 (green 1.44 tab)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F },           //     XEND = 127

  Rcmd2green160x80[] = {              // Init for 7735R, part 2 (mini 160x80)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 79
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },           //     XEND = 159


  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    3,                        //  4 commands in list:
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
      10};                     //     10 ms delay
    //ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
     // 100 };                  //     100 ms delay


// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Adafruit_ST7735::commandList(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    writecommand(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      writedata(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if(ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}

void Adafruit_ST7735::hardwareReset(){
	pinMode(_resetPin, OUTPUT);
	pinMode(_cdPin, OUTPUT);
	pinSet(_cdPin, HIGH);
	pinSet(_resetPin, LOW);
	_delay_ms(1);
	pinSet(_resetPin, HIGH);
	_delay_ms(200);	
}




// Initialization for ST7735R screens (green or red tabs)
void Adafruit_ST7735::initR(uint8_t options) {
  commonInit(Rcmd1);
  if(options == INITR_GREENTAB) {
    commandList(Rcmd2green);
    colstart = 2;
    rowstart = 1;
  } else if(options == INITR_144GREENTAB) {
    _height = ST7735_TFTHEIGHT_128;
    _width = ST7735_TFTWIDTH_128;
    commandList(Rcmd2green144);
    colstart = 2;
    rowstart = 3;
  } else if(options == INITR_MINI160x80) {
    _height = ST7735_TFTHEIGHT_160;
    _width = ST7735_TFTWIDTH_80;
    commandList(Rcmd2green160x80);
    colstart = 24;
    rowstart = 0;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(Rcmd2red);
  }
  commandList(Rcmd3);

  // if black, change MADCTL color filter
  if ((options == INITR_BLACKTAB) || (options == INITR_MINI160x80)) {
    writecommand(ST7735_MADCTL);
    writedata(0xC0);
  }

  tabcolor = options;
  fillScreen(0x0000);
  displayOn();
  setRotation(0);
}


void Adafruit_ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1,
 uint8_t y1) {

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0+xstart);     // XSTART 
  writedata(0x00);
  writedata(x1+xstart);     // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0+ystart);     // YSTART
  writedata(0x00);
  writedata(y1+ystart);     // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}


void Adafruit_ST7735::pushColor(uint16_t color) {
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(color >> 8);
	_spi->transfer(color);
	_spi->selectSlave(_displayID, DISABLE);
}

void Adafruit_ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if((x < 0) ||(x >= ST7735_TFTWIDTH_128) || (y < 0) || (y >= ST7735_TFTWIDTH_128)) return;
	setAddrWindow(x,y,x+1,y+1);
	pushColor(color);
}


void Adafruit_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h,uint16_t color) {
	if((x >= ST7735_TFTWIDTH_128) || (y >= ST7735_TFTHEIGHT_160)){
		return;
	}
	uint16_t temp ;
	temp = y + h - 1;
	if(temp >= ST7735_TFTHEIGHT_160){
		h = ST7735_TFTHEIGHT_160 - y;
	}
	setAddrWindow(x, y, x, y+h-1);

	uint8_t hi = color >> 8, lo = color;
    
	_spi->selectSlave(_displayID, ENABLE);
	
	for(y=h; y>0; y--) {
		_spi->transfer(hi);
		_spi->transfer(lo);
	}
	_spi->selectSlave(_displayID, DISABLE);
}


void Adafruit_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w,uint16_t color) {
if((x >= ST7735_TFTWIDTH_128) || (y >= ST7735_TFTHEIGHT_160)){
		return;
	}
	uint16_t temp; // avoid overflow
	temp = x + w - 1;
	if(temp >= ST7735_TFTWIDTH_128){
		w = ST7735_TFTWIDTH_128  - x;
	}
	setAddrWindow(x, y, x+w-1, y);
	uint8_t hi = color >> 8, lo = color;
	_spi->selectSlave(_displayID, ENABLE);
	for(x=w; x>0; x--) {
		_spi->transfer(hi);
		_spi->transfer(lo);
	}
	_spi->selectSlave(_displayID, DISABLE);
}



void Adafruit_ST7735::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}



// fill a rectangle
void Adafruit_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	
  // rudimentary clipping (drawChar w/big text requires this)
	if((x >= ST7735_TFTWIDTH_128) || (y >= ST7735_TFTHEIGHT_160)){
		return;
	}
	uint16_t temp; // avoid overflow
	temp = x + w - 1;
	if(temp >= ST7735_TFTWIDTH_128){
		w = ST7735_TFTWIDTH_128  - x;
	}
	temp = y + h - 1;
	if(temp >= ST7735_TFTHEIGHT_160){
		h = ST7735_TFTHEIGHT_160 - y;
	}
	setAddrWindow(x, y, x+w-1, y+h-1);
	uint8_t hi = color >> 8, lo = color;
	_spi->selectSlave(_displayID, ENABLE);
	
	for(y=h; y>0; y--) {
		for(x=w; x>0; x--) {
			_spi->transfer(hi);
			_spi->transfer(lo);
		}
	}
	_spi->selectSlave(_displayID, DISABLE);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ST7735::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void Adafruit_ST7735::setRotation(uint8_t m) {

  writecommand(ST7735_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
     } else {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     }

     if (tabcolor == INITR_144GREENTAB) {
       _height = ST7735_TFTHEIGHT_128;
       _width  = ST7735_TFTWIDTH_128;
     } else if (tabcolor == INITR_MINI160x80)  {
       _height = ST7735_TFTHEIGHT_160;
       _width = ST7735_TFTWIDTH_80;
     } else {
       _height = ST7735_TFTHEIGHT_160;
       _width  = ST7735_TFTWIDTH_128;
     }
     xstart = colstart;
     ystart = rowstart;
     break;
   case 1:
     if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
     } else {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     }

     if (tabcolor == INITR_144GREENTAB)  {
       _width = ST7735_TFTHEIGHT_128;
       _height = ST7735_TFTWIDTH_128;
     } else if (tabcolor == INITR_MINI160x80)  {
       _width = ST7735_TFTHEIGHT_160;
       _height = ST7735_TFTWIDTH_80;
     } else {
       _width = ST7735_TFTHEIGHT_160;
       _height = ST7735_TFTWIDTH_128;
     }
     ystart = colstart;
     xstart = rowstart;
     break;
  case 2:
     if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
       writedata(MADCTL_RGB);
     } else {
       writedata(MADCTL_BGR);
     }

     if (tabcolor == INITR_144GREENTAB) {
       _height = ST7735_TFTHEIGHT_128;
       _width  = ST7735_TFTWIDTH_128;
     } else if (tabcolor == INITR_MINI160x80)  {
       _height = ST7735_TFTHEIGHT_160;
       _width = ST7735_TFTWIDTH_80;
     } else {
       _height = ST7735_TFTHEIGHT_160;
       _width  = ST7735_TFTWIDTH_128;
     }
     xstart = colstart;
     ystart = rowstart;
     break;
   case 3:
     if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
     } else {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     }

     if (tabcolor == INITR_144GREENTAB)  {
       _width = ST7735_TFTHEIGHT_128;
       _height = ST7735_TFTWIDTH_128;
     } else if (tabcolor == INITR_MINI160x80)  {
       _width = ST7735_TFTHEIGHT_160;
       _height = ST7735_TFTWIDTH_80;
     } else {
       _width = ST7735_TFTHEIGHT_160;
       _height = ST7735_TFTWIDTH_128;
     }
     ystart = colstart;
     xstart = rowstart;
     break;
  }
}


void Adafruit_ST7735::invertDisplay(boolean i) {
  writecommand(i ? ST7735_INVON : ST7735_INVOFF);
}





