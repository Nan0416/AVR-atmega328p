#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
//#define wdt_reset() __asm__ __volatile__ ("wdr")
#define powerDown() do {\
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);\
	sleep_mode();\
	}while(0);
	
void on_board_led(){
	PORTB |= 0x20;
	_delay_ms(1000);
	PORTB &= ~0x20;
	
}
ISR(WDT_vect){
	 // not necessary but safe
	 wdt_reset();
}
int main(){
	
	DDRB |= 0x20;
	uint8_t temp = (_BV(WDIE) | _BV(WDP3) | _BV(WDP0));
	
	WDTCSR |= (_BV(WDCE) | _BV(WDE)); // enable modification
	WDTCSR = temp; // enable interrupt and set prescale in one instruction
	
	if(WDTCSR == temp){
		//correct
		on_board_led();
	}
	sei(); // enable global interrupt
	while(1){
		powerDown();
		on_board_led();
	}
}
