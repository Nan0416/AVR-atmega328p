#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
int main(){
	while(1){
		DDRC |= 0x80;
		PORTC |= 0x80;
		_delay_ms(1000);
		PORTC &= ~0x7F;
		_delay_ms(1000);
		DDRC |= 0x20;
		PORTC |= 0x20;
		_delay_ms(1000);
		PORTC &= ~0x20;
		_delay_ms(1000);
	}
	return 0;
}
