#ifndef F_CPU 
#define F_CPU 16000000UL
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../myExternal.h"

myEXT ext;
void on_board_led(void){
	DDRB |= 0x10;
	PORTB |= 0x10;
	_delay_ms(1000);
	PORTB &= ~0x10;
	
	
}
ISR(INT0_vect){
	on_board_led();
	/* do not accumulate interrupt */
	clear_ext_irq_flag(&ext);
}
int main(){
	/* default use INT0, rising edge */
	
	init_ext(&ext, EXT_INT_0);
	
	setup_ext(&ext);
	config_ext_enable(&ext, TRUE);
	
	sei();
	while(1);
}
