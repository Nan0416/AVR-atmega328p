#include "common.h"
#include <avr/io.h>
void pinBMode(uint8_t pin, uint8_t mode){
	switch(mode){
		case INPUT: DDRB &= ~_BV(pin); break;
		case OUTPUT: DDRB |= _BV(pin); break;
		case PULLUP_INPUT: DDRB &= ~_BV(pin);PORTB |= ~_BV(pin); break;
		default:break;
	}
}
void pinCMode(uint8_t pin, uint8_t mode){
	switch(mode){
		case INPUT: DDRC &= ~_BV(pin); break;
		case OUTPUT: DDRC |= _BV(pin); break;
		case PULLUP_INPUT: DDRC &= ~_BV(pin);PORTC |= ~_BV(pin); break;
		default:break;
	}
}
void pinDMode(uint8_t pin, uint8_t mode){
	switch(mode){
		case INPUT: DDRD &= ~_BV(pin); break;
		case OUTPUT: DDRD |= _BV(pin); break;
		case PULLUP_INPUT: DDRD &= ~_BV(pin);PORTD |= ~_BV(pin); break;
		default:break;
	}
}

void pinMode(uint8_t pin, uint8_t mode){
	if(pin < 0x08){
		//port b
		pinBMode(pin, mode);
	}else if(pin < 0x10){
		//port c
		pinCMode(pin & 0x07, mode);
	}else if(pin < 0x18){
		//port d
		pinDMode(pin & 0x07, mode);
	}
}
uint8_t pinGet(uint8_t pin){
	if(pin < 0x08){
		//port b
		return pinBGet(pin);
	}else if(pin < 0x10){
		//port c
		return pinCGet(pin & 0x07);
	}else if(pin < 0x18){
		//port d
		return pinDGet(0x07 & pin);
	}
	return 0;
}
void pinSet(uint8_t pin, uint8_t value){
	if(pin < 0x08){
		//port b
		pinBSet(pin, value);
	}else if(pin < 0x10){
		//port c
		pinCSet(pin & 0x07, value);
	}else if(pin < 0x18){
		//port d
		pinDSet(pin & 0x07, value);
	}
}

