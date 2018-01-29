#define F_CPU 16000000UL
#include <avr/io.h>
#include "myRS232.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// 
struct RS232Data data;


ISR(USART_RX_vect){
	if(data.len == 0){
		data.len = UDR0;
		send_byte(data.len);
		if(data.len > MAX_DATA_LEN){
			data.len = MAX_DATA_LEN;
		}
	}else{
		uint8_t temp;
		temp = UDR0;
		data.data[data.offset] = temp;
		send_byte(temp);
		data.offset++;
	}
	if(data.offset >= MAX_DATA_LEN || data.offset >= data.len){
		//handle
		send(&data);
		data.len = 0;
		data.offset = 0;
	}
}

int main(){
	data.len = 0;
	data.offset = 0;
	//uint8_t data[8] = {0x32, 0x87, 0x67, 0x23, 0x66, 0x14, 0x98, 0x92};
	init_rs232();
	enable_rs232();
	sei();
	DDRB |= 0x10;
	PORTB |= 0x10;
	_delay_ms(50);
	PORTB &= ~0x10;
	while(1);
	
	return 0;
}


