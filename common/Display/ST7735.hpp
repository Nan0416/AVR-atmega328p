#ifndef __ST7735_H__
#define __ST7735_H__
#include "F_CPU.h"
#include "SPI.hpp"
#include "common.h"

// some flags for initR() :(
#define INITR_GREENTAB   0x0
#define INITR_REDTAB     0x1
#define INITR_BLACKTAB   0x2

#define INITR_18GREENTAB    INITR_GREENTAB
#define INITR_18REDTAB      INITR_REDTAB
#define INITR_18BLACKTAB    INITR_BLACKTAB
#define INITR_144GREENTAB   0x1
#define INITR_MINI160x80    0x4


// for 1.44 and mini
#define ST7735_TFTWIDTH_128  128
// for mini
#define ST7735_TFTWIDTH_80   80
// for 1.44" display
#define ST7735_TFTHEIGHT_128 128
// for 1.8" and mini display
#define ST7735_TFTHEIGHT_160  160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04


#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
/**
 * Default layout is protrait (like a phone)
 * */

class ST7735:Adafruit_GFX{
private:
	SPI * _spi;
	uint8_t _resetPin;
	uint8_t _cdPin;
	uint8_t _displayID;
	void sendData(uint8_t data);
	void sendCmd(uint8_t data);
	void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void commandList(const uint8_t *addr);
public:
	ST7735(SPI * spi, uint8_t resetPin, uint8_t cdPin, uint8_t displayID);
	void hardwareReset();
	void initDisplay();
	void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
	inline void invertDisplay(boolean i) {
		sendCmd(i ? ST7735_INVON : ST7735_INVOFF);
	}
	inline void fillScreen(uint16_t color) {
		fillRect(0, 0, ST7735_TFTWIDTH_128, ST7735_TFTHEIGHT_160, color);
	}
	void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void pushColor(uint16_t color);
	//void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	//void setRotation(uint8_t r);
 
    
	inline void displayOn(){
		sendCmd(ST7735_DISPON);
	}
	inline void displayOff(){
		sendCmd(ST7735_DISPOFF);
	}
};






#endif
