#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "mySPI.h"
#include <util/delay.h>


void init_spi(){
	spi.spi_on = FALSE;
	spi.slave_num = 1;
	spi._ss[0].port = PB;
	spi._ss[0].pin = P2; 
	
	spi.irq_enabled = FALSE;
	spi.data_mode = RISING_SAMPLE;
	spi.op_mode = MASTER;
	spi.data_order = MSB;
	spi.data_rate = SCALE_4;
	
}

void setup_spi(void){
	config_spi_enable(spi.spi_on);
	config_spi_operating_mode(spi.op_mode);
	config_spi_interrupt(spi.irq_enabled);
	config_spi_data_mode(spi.data_mode);
	config_spi_data_order(spi.data_order);
		
}

void config_spi_child(uint8_t spi_id, uint8_t value){
	if(value == HIGH){
		switch(spi._ss[spi_id].port){
			case PB: DDRB |= _BV(spi._ss[spi_id].pin); PORTB |= _BV(spi._ss[spi_id].pin);break;
			case PC: DDRC |= _BV(spi._ss[spi_id].pin); PORTC |= _BV(spi._ss[spi_id].pin);break;
			case PD: DDRD |= _BV(spi._ss[spi_id].pin); PORTD |= _BV(spi._ss[spi_id].pin);break; 
			default: break;
		}
	}else{
		switch(spi._ss[spi_id].port){
			case PB: DDRB |= _BV(spi._ss[spi_id].pin); PORTB &= ~_BV(spi._ss[spi_id].pin);break;
			case PC: DDRC |= _BV(spi._ss[spi_id].pin); PORTC &= ~_BV(spi._ss[spi_id].pin);break;
			case PD: DDRD |= _BV(spi._ss[spi_id].pin); PORTD &= ~_BV(spi._ss[spi_id].pin);break; 
			default: break;
		}
	}
}

void config_spi_enable(BOOL on){
	if(on == TRUE){
		/* SPE is not 6, !!!!!!!!! */
		SPCR |= _BV(6);
	}else{
		SPCR &= ~_BV(6);
	}
	spi.spi_on = on;
	
}
void config_spi_interrupt(BOOL enabled){
	if(enabled == TRUE){
		SPCR |= _BV(SPIE);
	}else{
		SPCR &= ~_BV(SPIE);
	}
	spi.irq_enabled = enabled;
	
}

void config_spi_data_mode(SPI_DATA_MODE data_mode){
	switch(data_mode){
		case RISING_SAMPLE: SPCR &= 0xF3; break;
		case RISING_SETUP: SPCR &= 0xF7; SPCR |= 0x04; break;
		case FALLING_SAMPLE: SPCR |= 0x08; SPCR &= 0xFB; break;
		case FALLING_SETUP: SPCR |= 0x0C; break;
		default: SPCR &= 0xF3; break;
	}
	spi.data_mode = data_mode;
}

void _config_spi_data_rate(SPI_DATA_RATE data_rate){
	
	switch(data_rate){
		case SCALE_2: SPSR |= 0x01; SPCR &= 0xFC; break;
		case SCALE_4: SPSR &= 0xFE; SPCR &= 0xFC; break;
		case SCALE_8: SPSR |= 0x01;  SPCR |= 0x01; SPCR &= 0xFD; break;
		case SCALE_16: SPSR &= 0xFE; SPCR |= 0x01; SPCR &= 0xFD; break;
		case SCALE_32: SPSR |= 0x01; SPCR &= 0xFE; SPCR |= 0x02; break;
		case SCALE_64: SPSR |= 0x01; SPCR |= 0x03; break;
		case SCALE_128: SPSR &= 0xFE; SPCR |= 0x03; break;
		default:  SPSR &= 0xFE; SPCR &= 0xFC; break;
	}
	spi.data_rate = data_rate;
	
}

void config_spi_operating_mode(SPI_OPERATING_MODE _op_mode){
	if(_op_mode == MASTER){
		SPCR |= _BV(MSTR);
		_config_spi_data_rate(spi.data_rate);
		_config_spi_slave(spi._ss, spi.slave_num);
		/* direction */
		DDRB |= 0x2C;
		DDRB &= 0xEF;
		PORTB |= 0x04;
	}else{
		SPCR &= ~_BV(MSTR);
		DDRB |= 0x10;
		DDRB &= 0xD3;
	}
	spi.op_mode = _op_mode;
}

void config_spi_data_order(SPI_DATA_ORDER _order){
	if(_order == MSB){
		SPCR &= ~_BV(DORD);
	}else{
		SPCR |= _BV(DORD);
	}
	spi.data_order = _order;
}

void _config_spi_slave(PIN_ADDR _ss[], uint8_t num){
	/* used only in master mode */
	if(num > _MAX_SLAVE){
		num = _MAX_SLAVE;
	}
	int i =0;
	for(;i < num; i++){
		/* set output */
		switch(_ss[i].port){
			case PB: DDRB |= _BV(_ss[i].pin); PORTB |= _BV(_ss[i].pin);break;
			case PC: DDRC |= _BV(_ss[i].pin); PORTC |= _BV(_ss[i].pin);break;
			case PD: DDRD |= _BV(_ss[i].pin); PORTD |= _BV(_ss[i].pin);break; 
			default: break;
		}
		
	}
	i = 0;
	for(; i < num; i++){
		spi._ss[i].port = _ss[i].port;
		spi._ss[i].pin = _ss[i].pin;
	}
}



uint8_t spi_clear_flag(void){
	uint8_t rs = 0;
	if(SPI_DATA_AVAILABLE){
		rs = SPDR;
	}
	rs = 0;
	return rs;
}
// uint8_t spi_transfer(uint8_t data);

uint8_t spi_transfer(uint8_t data){
	SPDR = data;
	while (!(SPSR & 0x80));
	return SPDR;
}

uint8_t spi_read_byte(void){
	return SPDR;
}

uint8_t spi_exchange_one_byte(uint8_t spi_id, uint8_t value){
	config_spi_child(spi_id, LOW);
	SPDR = value;
	while (!(SPSR & 0x80));
	value = SPDR;
	config_spi_child(spi_id, HIGH);
	return value;
}
uint8_t spi_exchange_two_bytes(uint8_t spi_id, uint8_t cmd, uint8_t value){
	config_spi_child(spi_id, LOW);
	SPDR = cmd;
	while (!(SPSR & 0x80));
	cmd = SPDR;
	SPDR = value;
	while (!(SPSR & 0x80));
	value = SPDR;
	config_spi_child(spi_id, HIGH);
	return value;
}

void spi_write_buffer(uint8_t spi_id, const uint8_t data[], uint8_t len){
	config_spi_child(spi_id, LOW);
	while(len--){
		SPDR = *data++;
		while (!(SPSR & 0x80));
	}
	config_spi_child(spi_id, HIGH);
}

void spi_read_buffer(uint8_t spi_id,  uint8_t data[], uint8_t len){
	config_spi_child(spi_id, LOW);
	while(len--){
		SPDR = NOP;
		while (!(SPSR & 0x80));
		*data++ = SPDR;
	}
	config_spi_child(spi_id, HIGH);
}

uint8_t spi_read_cmd_buffer(uint8_t spi_id, uint8_t cmd, uint8_t data[], uint8_t data_len, uint8_t pack_len){
	uint8_t rs;

	config_spi_child(spi_id, LOW);
	SPDR = cmd;
	while (!(SPSR & 0x80));
	rs = data_len;
	while(data_len--){
		SPDR = NOP;
		while (!(SPSR & 0x80));
		*data++ = SPDR;
	}
	pack_len -= data_len;
	while(pack_len--){
		SPDR = NOP;
		while (!(SPSR & 0x80));
	}	
	
	config_spi_child(spi_id, HIGH);
	return rs;

}

uint8_t spi_write_cmd_buffer(uint8_t spi_id, uint8_t cmd, const uint8_t buf[], uint8_t data_len, uint8_t pack_len){
	uint8_t status;
	pack_len -= data_len;
	config_spi_child(spi_id, LOW);
	SPDR = cmd;
	while (!(SPSR & 0x80));
	status = SPDR;
	while(data_len--){
		SPDR = *buf++;
		while (!(SPSR & 0x80));
	}
	while(pack_len--){
		SPDR = NOP;
		while (!(SPSR & 0x80));
	}
	config_spi_child(spi_id, HIGH);
	return status;
}

uint8_t spi_data_available(void){
	/* no available return 0, otherwise 0x80 */
	return SPSR & 0x80;
}
uint8_t spi_passive_receive(uint8_t buf[], uint8_t len){
	while(1){
		if(SPSR & 0x80){
			*buf++ = SPDR;
			/* spsr automatically clear by reading data */
			len -= 1;
		}
		if(len == 0){
			break;
		}
	}
	return len;
}
