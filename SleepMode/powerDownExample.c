#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/sleep.h>

void powerDown(void)
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}
void on_board_led(){
	PORTB |= 0x20;
	_delay_ms(1000);
	PORTB &= ~0x20;
	
}
ISR(INT0_vect){
	
}
int main(){
	
	DDRB |= 0x20;
	DDRD &= ~0x04; // INT0 pins PD2 output
	EICRA &= ~0x03; // INT0 level low trigger  
	EIMSK |= 0x01; // enable INT0 interrupt
	sei(); // enable global interrupt
	while(1){
		powerDown();
		on_board_led();
	}
}
