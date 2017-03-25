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
	const char * data = "In computer user interfaces, a cursor is an indicator used to show the current position for user interaction on a computer monitor or other display device that will respond to input from a text input or pointing device. The mouse";// cursor is also called a pointer, owing to its resemblance in usage to a pointing stick.";
	config_show(1, FLUSH);
	
	lcd_show_data msg;
	msg.data = data;
	msg.beginning = 0;
	msg.data_len = strlen(data);
	
	while(msg.data_len - msg.beginning > 0 ){
		lcd_show_num(msg.data_len - msg.beginning, 1000);
		_delay_ms(1000);
		lcd_update_vertical(&msg);
		_delay_ms(2000);
	}
	while(1);
	
}

