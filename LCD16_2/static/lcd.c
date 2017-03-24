#ifndef F_CPU
#define F_CPU 16000000UL
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

// PORTD 4-7 works as high 4 bits of data/command input/output
// PORTC 0-3 works as low 4 bits of data/commadnd input/output
// PC4 RS
// PC5 R/W 
// PD3 Enable 

void setupLCD(void){
	
	
	DDRD = 0xf8; // 1111 1000
	PORTD = 0x00;
	DDRC = 0x3f; // 0011 1111
	PORTC = 0x00;
	_writeInCMD(0x38);
	_delay_ms(64);
	_writeInCMD(0x38);
	_delay_ms(50);
	_writeInCMD(0x38);
	_delay_ms(20);
	_writeInCMD(0x0c);
	_delay_ms(20);
	_writeInCMD(0x01);  // clear the screen
	_delay_ms(100);
	
}
void clear(void){
	_writeInCMD(0x01);  // clear the screen
	_delay_ms(10);
}

void _writeInCMD(uint8_t CMD){
	PORTC = 0x00; // E = 0, RW = 0, RS = 0
	PORTD = 0x00;
	_delay_us(50);
	PORTD = CMD & 0xf0; // put data
	PORTC = CMD & 0x0f;
	_delay_us(50);
	PORTD = PORTD | 0x08; // E = 1, RW = 0, RS = 0
	_delay_us(50);
	PORTD &= 0xf7; // E = 0,
	_delay_us(50);	 
	PORTC = 0x00; // E = 0, RW = 0, RS = 0
	PORTD = 0x00;
	_delay_us(50);	
	
}

// PORTD 4-7 works as high 4 bits of data/command input/output
// PORTC 0-3 works as low 4 bits of data/commadnd input/output
// PC4 RS
// PC5 R/W 
// PD3 Enable 
void _writeInDATA(uint8_t DATA){
	PORTC = 0x10; // E = 0, RW = 0, RS = 1
	PORTD = 0x00;
	_delay_us(50);
	PORTD |= DATA & 0xf0; // put data
	PORTC |= DATA & 0x0f;
	_delay_us(50);
	PORTC |= 0x10; // E = 1, RW = 0, RS = 1
	PORTD |= 0x08;
	_delay_us(50);
	
	PORTD &= 0xf7;
	_delay_us(50);
	PORTC |= 0x10; // E = 0, RW = 0, RS = 1	
	_delay_us(50);
}
void showData(char * data,uint8_t len,uint8_t line){
	if(line == LINE_1){
		_writeInCMD(0x80);
	}else if(line == LINE_2){
		_writeInCMD(0xc0);
	}
	int i = 0;
	for(; i < len; i++){
		_writeInDATA(data[i]);
	}

}
