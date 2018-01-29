
#include "UI.hpp"
int16_t abs(int16_t x){
	if(x < 0){
		return -x;
	}
	return x;
}
UI::UI(int16_t width, int16_t height){
		this->_width = width;
		this->_height= height;
	}
void UI::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){}
void UI::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){}
void UI::drawPixel(int16_t x, int16_t y, uint16_t color){}
void UI::pushColor(uint16_t color){}
void UI::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){}


void UI::pixel(int16_t x, int16_t y, uint16_t color){
	drawPixel(x, y, color);
}


void UI::lineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
	for (; x0<=x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void UI::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    // Update in subclasses if desired!
    startWrite();
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if(y0 == y1){
        if(x0 > x1) _swap_int16_t(x0, x1);
        drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else  {
        lineHelper(x0, y0, x1, y1, color);
    }
    endWrite();
}
void UI::rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	startWrite();
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w, y, h, color);
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h, w, color);
	endWrite();
}
void UI::solidRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	startWrite();
	fillRect( x, y, w, h, color);
	endWrite();
}
// Draw a circle outline
void UI::circle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
	startWrite();
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    
    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
    endWrite();
   
}





// Used to do circles and roundrects
void UI::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
           drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
           drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2 && x >= 0) {
           drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}
void UI::solidCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color){
	startWrite();
    drawFastVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
    endWrite();
}


void UI::fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}
