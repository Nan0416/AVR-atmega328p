#include "F_CPU.h"
#include "common.h"
#include "SPI.hpp"
#include "ST7735.hpp"
#include <avr/io.h>
#include <util/delay.h>

int main(){
	SPI spi;
	spi.dataRate(SCALE_2); // 16MHz / 2 = 8MHz
	spi.hardwareSetup();
	int slave_id = spi.addSlave(PinB2);
	ST7735 st7735(&spi, PinB1, PinB0, slave_id);
	spi.enable();
	st7735.hardwareReset();
	st7735.initDisplay();
	//st7735.rotation();
	//st7735.fillScreen(0x1111);
	st7735.drawFastVLine(64, 0, 160, 0x5562); 
	st7735.drawFastHLine(0, 80, 128, 0x5562);
	st7735.drawPixel(10, 10, 0x5562);  
	st7735.drawPixel(10, 11, 0x5562); 
	_delay_ms(500);
	//st7735.invertDisplay(1);
	//st7735.rotation();
	while(1);
}
