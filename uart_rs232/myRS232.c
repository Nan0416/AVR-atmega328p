#include "myRS232.h"


/* default B19200, 8 bit, 2 stop bit, disenable functions, no interrupt, even parity */
void init_rs232(void){
	rs232.baud_rate = B19200;
	rs232.bit_num = 8;
	rs232.stop_bit_num = 2;
	rs232.enabled_mask = 0;
	rs232.interrupt_enabled_mask = 0;
	rs232.parity = rs232_parity_even;
	
}

void setup_rs232(void){
	config_rs232_baudrate(rs232.baud_rate);
	config_rs232_bit_num(rs232.bit_num);
	config_rs232_interrupt(rs232.interrupt_enabled_mask);
	config_rs232_parity(rs232.parity);
	config_rs232_stop_bit(rs232.stop_bit_num);
	config_rs232_enable(rs232.enabled_mask);
}

void config_rs232_interrupt(uint8_t interrupt){
	UCSR0B = (RS232_EN_INT_MASK & UCSR0B) | interrupt;
	rs232.interrupt_enabled_mask = interrupt;
}

void config_rs232_enable(uint8_t enable){
	UCSR0B = (RS232_EN_MASK & UCSR0B) | enable;
	rs232.enabled_mask = enable;
}

void config_rs232_bit_num(uint8_t bit_num){
	switch(bit_num){
		case 5: UCSR0C = (UCSR0C & 0xF9); UCSR0B = (UCSR0B & 0xFB); break;
		case 6: UCSR0C = (UCSR0C & 0xF9) | 0x02; UCSR0B = (UCSR0B & 0xFB); break;
		case 7: UCSR0C = (UCSR0C & 0xF9) | 0x04; UCSR0B = (UCSR0B & 0xFB); break;
		case 8: UCSR0C = (UCSR0C & 0xF9) | 0x06; UCSR0B = (UCSR0B & 0xFB); break;
		case 9: UCSR0C = (UCSR0C & 0xF9) | 0x06; UCSR0B = (UCSR0B & 0xFB) | 0x04; break;
		default: UCSR0C = (UCSR0C & 0xF9) | 0x06; UCSR0B = (UCSR0B & 0xFB); bit_num = 8 ;break; // default 8
	}
	rs232.bit_num = bit_num;
}

void config_rs232_parity(rs232_parity parity){
	switch(parity){
		case rs232_parity_even: UCSR0C = (UCSR0C & 0xCF) | 0x20; break;
		case rs232_parity_odd:  UCSR0C = (UCSR0C & 0xCF) | 0x30; break;
		case rs232_parity_disable:  UCSR0C = (UCSR0C & 0xCF); break;
		default: UCSR0C = (UCSR0C & 0xCF) | 0x20; parity = rs232_parity_even ;break; // default even
	}
	rs232.parity = parity;
}

void config_rs232_stop_bit(uint8_t stop_bit){
	if(stop_bit == 1){
		UCSR0C &= 0xF7;
	}else{
		// default 2
		UCSR0C |= 0x08;
		stop_bit = 2;
	}
	
	rs232.stop_bit_num = stop_bit;
}

void config_rs232_baudrate(rs232_baudrate baudrate){
	UBRR0L = baudrate;
	UBRR0H = 0x00;
	UCSR0A |= 0x02;
	rs232.baud_rate = baudrate;
}

void send_byte(uint8_t data){
	UDR0 = data;
	while(!(UCSR0A & 0x40));	
}

void send_buffer(uint8_t data[], uint8_t len){
	while(len--){
		UDR0 = *data++;
		while(!(UCSR0A & 0x40));
	}
}

uint8_t recv_byte(){
	while(!(UCSR0A & 0x80));
	return UDR0;	
}

void recv_buffer(uint8_t data[], uint8_t len){
	while(len--){
		while(!(UCSR0A & 0x80));
		*data++ = UDR0;
	}
}

void send_9bit(uint8_t data){
		
}

void send_buffer_9bit(uint8_t data[], uint8_t len){
	
}

uint8_t recv_9bit(){
	return 0;
}

void recv_buffer_9bit(uint8_t data[], uint8_t len){
	
}

void clear_rs232_irq_flg(uint8_t bit){
	if(UCSR0A & bit){
		UCSR0A |= bit;
	}
}
