#include "myRS232.h"


/* default B19200, 8 bit, 2 stop bit,  even parity */
void init_rs232(void){
	//set working mode: asynchronous
	UCSR0C &= 0x3f;
	//baudrate
	UBRR0L = B19200;
	UBRR0H = 0x00;
	UCSR0A |= 0x02;
	// bit_num = 8;
	UCSR0C = (UCSR0C & 0xF9) | 0x06; 
	UCSR0B = (UCSR0B & 0xFB);
	// stop bit 2
	UCSR0C |= 0x08;
	// even parity
	UCSR0C = (UCSR0C & 0xCF) | 0x20;
	
	
}
void enable_rs232(void){
	UCSR0B |= 0x18;
}
void disable_rs232(void){
	UCSR0B &= 0xe7;
}

void send_byte(uint8_t data){
	UDR0 = data;
	while(!(UCSR0A & 0x20));
	//_delay_ms(1);	
}

void send(struct RS232Data * data){
	int i = 0;
	send_byte(data->len);
	for(i =0 ;i < data->len; i++){
		send_byte(data->data[i]);
	}
}

uint8_t recv_byte(void){
	while(!(UCSR0A & 0x80));
	return UDR0;
}

