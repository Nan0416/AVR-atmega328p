#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(){
	/*DDRB |= 0x10;
	DDRD &= 0xf7;
	while(1){
		uint8_t temp = PIND;
		if(temp & 0x08){
			PORTB |= 0x10;
			_delay_ms(1000);
			PORTB &= 0xef;
			_delay_ms(1000);
		}
	}
	*/
	DDRB |= 0x20;
	while(1){
		PORTB |= 0x20;
		_delay_ms(100);
		PORTB &= 0xdf;
		_delay_ms(500);
	}
}
