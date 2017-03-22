#define F_CPU 16000000UL
#include <avr/io.h>
#include "myRS232.h"
#include <stdint.h>
#include <util/delay.h>
myRS232 rs232;
void led(){
	DDRB |= 0x20;
	PORTB |= 0x20;
	_delay_ms(100);
	PORTB &= ~0x20;
	_delay_ms(100);
}
int main(){
	uint8_t data[8] = {0x32, 0x87, 0x67, 0x23, 0x66, 0x14, 0x98, 0x92};
	init_rs232();
	setup_rs232();
	config_rs232_parity(rs232_parity_disable);
	config_rs232_stop_bit(1);
	config_rs232_baudrate(B9600);
	config_rs232_enable(RS232_EN_RX | RS232_EN_TX);
	_delay_us(100);
	led();
	led();
	recv_buffer(data, 8);
	led();
	while(1);
	return 0;
}
