#ifndef F_CPU 
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include "_common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "myLCD.h"


myLCD lcd;
int main(){
	init_lcd();
	
	setup_lcd();
	const char * data = "In computer user interfaces";
	config_show(1, FLUSH);
	
	lcd_show_data msg;
	msg.data = data;
	msg.beginning = 0;
	msg.data_len = strlen(data);
	
	lcd_update_line(&msg, LCD_LINE_2, LCD_RIGHT, 0);
	_delay_ms(500);
	uint8_t step = 2;
	while(1){
		
		while(msg.data_len - msg.beginning > step){	
			lcd_update_line(&msg, LCD_LINE_2, LCD_RIGHT, step);
			_delay_ms(500);
	    }
	    while(msg.beginning >= step){
			
			lcd_update_line(&msg, LCD_LINE_2, LCD_LEFT, step);
			_delay_ms(500);
		}
	}
	
	while(1);
	
}

