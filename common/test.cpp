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
	for(int i =8 ; i < 128 ;i += 16){
		st7735.solidCircle(i,8,5, ST7735_BLUE);
	}
	//st7735.solidCircle(56,8,2, ST7735_WHITE);
	int x = 120;
	while(1){
		
		for(int i = 8 ; i < 128 ;i += 16){
			st7735.solidCircle(x,8,5, ST7735_BLUE);
			st7735.solidCircle(i, 8, 2, ST7735_WHITE);
			x+=16;
			if(x >= 128){
				x = 8;
			}
			delay(24);
		}
	}
}
