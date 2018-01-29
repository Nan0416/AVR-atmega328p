#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

void delay_ms(int time){
	int i =0 ;
	for(; i < time; i++){
		_delay_ms(1);
	}
}
volatile int time = 2000;
void on_board_led(const int time){
	PORTB |= 0x10;
	delay_ms(time);
	PORTB &= ~0x10;
	delay_ms(time);
}
ISR(INT0_vect, ISR_NOBLOCK){
	int i = 0;
	for(; i < 100; i++){
		on_board_led(time);
		EIFR |= 0x01;
	}
}
ISR(INT1_vect){
	on_board_led(50);
	on_board_led(50);
	EIFR |= 0x02;
	time = time >> 1;
	if(time < 50){
		time = 2000;
	}
}
int main(){
	DDRB |= 0x10;
	DDRD &= ~0x0C; // INT0/INT1 pins PD2/3 output
	EICRA |= 0x0f; // INT0 rising edge 
	EIMSK |= 0x03; // enable INT0 interrupt
	sei(); // enable global interrupt
	_delay_ms(1000); // delay 1000ms
	while(1);
}
