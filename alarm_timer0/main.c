
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include "alarm.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <avr/io.h>
ISR(TIMER0_COMPA_vect){
	alarm_pause();
	DDRB |= 0x20;
	PORTB |= 0x20;
	_delay_ms(1000);
	PORTB &= ~0x20;
	alarm_s(4);
	
}
void led_fast(){
	DDRB |= 0x20;
	PORTB |= 0x20;
	_delay_ms(100);
	PORTB &= ~0x20;
	_delay_ms(100);
}
int main(){
	clear_flag();
	sei();
	alarm_s(4);
	
	while(1){
		led_fast();
	}
	return 0;	
}
