#include "SPI.hpp"
#include "common.h"
#include <avr/pgmspace.h>
#include "ST7735.hpp"
#include <util/delay.h>
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
      0x05 };              //     16-bit color
      
void ST7735::commandList(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    sendCmd(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
	while(numArgs--) {                   //   For each argument...
		sendData(pgm_read_byte(addr++));  //     Read, issue argument
	}
	if(ms) {
		ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
		if(ms == 255){
			ms = 500;
		};     // If 255, delay for 500 ms
		delay(ms);
    }
  }
}
   
      
ST7735::ST7735(SPI * spi, uint8_t resetPin, uint8_t cdPin, uint8_t displayID){
	_spi = spi;
	_resetPin = resetPin;
	_cdPin = cdPin;
	_displayID = displayID;
}
void ST7735::hardwareReset(){
	pinMode(_resetPin, OUTPUT);
	pinMode(_cdPin, OUTPUT);
	pinSet(_cdPin, HIGH);
	pinSet(_resetPin, LOW);
	_delay_ms(1);
	pinSet(_resetPin, HIGH);
	_delay_ms(200);	
}
void ST7735::sendData(uint8_t data){
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(data);
	_spi->selectSlave(_displayID, DISABLE);
}
void ST7735::sendCmd(uint8_t data){
	pinSet(_cdPin, LOW);
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(data);
	_spi->selectSlave(_displayID, DISABLE);
	pinSet(_cdPin, HIGH);
}
void ST7735::initDisplay(){
	commandList(Rcmd1);
	fillScreen(0x0000);
	displayOn();
}

void ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	sendCmd(ST7735_CASET);
	sendData(0x00);
	sendData(x0);
	sendData(0x00);
	sendData(x1);
	sendCmd(ST7735_RASET);
	sendData(0x00);
	sendData(y0);
	sendData(0x00);
	sendData(y1);
	sendCmd(ST7735_RAMWR);
}




// fill a rectangle
void ST7735::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
	
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
//draw a vertical line
void ST7735::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

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


void ST7735::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){

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
void ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if((x < 0) ||(x >= ST7735_TFTWIDTH_128) || (y < 0) || (y >= ST7735_TFTWIDTH_128)) return;
	setAddrWindow(x,y,x+1,y+1);
	pushColor(color);
}

void ST7735::pushColor(uint16_t color) {
	_spi->selectSlave(_displayID, ENABLE);
	_spi->transfer(color >> 8);
	_spi->transfer(color);
	_spi->selectSlave(_displayID, DISABLE);
}













