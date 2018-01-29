#ifndef _UI_hpp_
#define _UI_hpp_


#include <stdint.h>
#include "common.h"
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

class UI{
protected:
	int16_t _width;
	int16_t _height;
	
public:
	//maximum area : uint16
	UI(int16_t width, int16_t height);
	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
	virtual void pushColor(uint16_t color);
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	
	void lineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	
	
	
	
	
	
	
	
	void pixel(int16_t x, int16_t y, uint16_t color);
	void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void solidRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void solidCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	
	
	void startWrite(){
	}
	void endWrite(){
		
	}
	void fillScreen(uint16_t color);
	
};
#endif //_UI_hpp_
