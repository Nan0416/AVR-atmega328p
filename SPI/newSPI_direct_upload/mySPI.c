#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "mySPI.h"
#include <util/delay.h>
void led(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(100);
	PORTC &= 0xFE;
	_delay_ms(500);
}

void init_spi(){
	int i = 0;
	spi.spi_on = FALSE;
	spi.irq_enabled = FALSE;
	spi.data_mode = RISING_SAMPLE;
	spi.op_mode = MASTER;
	spi.data_order = MSB;
	spi.data_rate = SCALE_4;
	spi.slave_num = 0;
	//initialize slave
	for(i = 0; i< _MAX_SLAVE; i++){
		spi._ss[i].port = UNDEFINED;
	}
}

void setup_spi(void){
	config_spi_operating_mode(spi.op_mode);
	config_spi_interrupt(spi.irq_enabled);
	config_spi_data_mode(spi.data_mode);
	config_spi_data_order(spi.data_order);
	config_spi_enable(spi.spi_on);
		
}
/*
 * set master / slave
 * */
void config_spi_operating_mode(SPI_OPERATING_MODE _op_mode){
	if(_op_mode == MASTER){
		SPCR |= _BV(MSTR);
		_config_spi_data_rate(spi.data_rate);
		//_config_spi_slave(spi._ss, spi.slave_num);
		/* direction */
		DDRB |= 0x28; // MOSI, SCK output
		DDRB &= 0xEF; // MISO
	}else{
		SPCR &= ~_BV(MSTR);
		DDRB |= 0x10;
		DDRB &= 0xD3;
	}
	spi.op_mode = _op_mode;
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


void config_spi_interrupt(BOOL enabled){
	if(enabled == TRUE){
		SPCR |= _BV(SPIE);
	}else{
		SPCR &= ~_BV(SPIE);
	}
	spi.irq_enabled = enabled;
	
}
/*
 * set data transfer mode
 * */
void config_spi_data_mode(SPI_DATA_MODE data_mode){
	switch(data_mode){
		case RISING_SAMPLE: SPCR &= 0xF3; break; // first-rising-first-sample
		case RISING_SETUP: SPCR &= 0xF7; SPCR |= 0x04; break; // first-rising-second-sample
		case FALLING_SAMPLE: SPCR |= 0x08; SPCR &= 0xFB; break; // first-falling-first-sample
		case FALLING_SETUP: SPCR |= 0x0C; break; //first-falling-second-sample
		default: SPCR &= 0xF3; break;
	}
	spi.data_mode = data_mode;
}

void config_spi_data_order(SPI_DATA_ORDER _order){
	if(_order == MSB){
		SPCR &= ~_BV(DORD);
	}else{
		SPCR |= _BV(DORD);
	}
	spi.data_order = _order;
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

// return _MAX_SLAVE means failure,
uint8_t add_slave(MEGA_PORT port, MEGA_PIN pin){
	//find next avaliable
	uint8_t i = spi.slave_num;
	uint8_t flag = 0;
	for(; i < spi.slave_num + _MAX_SLAVE; i++){
		if(spi._ss[i % _MAX_SLAVE].port == UNDEFINED){
			spi.slave_num = i % _MAX_SLAVE;
			flag = 1;
			break;
		}
	}
	if(flag == 0){
		return _MAX_SLAVE;
	}
	spi._ss[spi.slave_num].port = port;
	spi._ss[spi.slave_num].pin = pin;
	switch(port){
		case PB: DDRB |= _BV(pin); PORTB |= _BV(pin);break; // set direction ,set as high
		case PC: DDRC |= _BV(pin); PORTC |= _BV(pin);break;
		case PD: DDRD |= _BV(pin); PORTD |= _BV(pin);break; 
		default: break;
	}
	return spi.slave_num;
	
}
void remove_slave(uint8_t spi_id){
	if(spi_id < _MAX_SLAVE){
		spi._ss[spi_id].port = UNDEFINED;
	}
}

int8_t config_spi_child(uint8_t spi_id, uint8_t value){
	
	if(spi._ss[spi_id].port == UNDEFINED){
		return -1;
	} 
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
	return 1;
}



/*
 * read flag and then read data register will clear the SPI interrutp flag.
 * */
uint8_t spi_clear_flag(void){
	uint8_t rs = 0;
	if(SPI_DATA_AVAILABLE){
		rs = SPDR;
	}
	rs = 0;
	return rs;
}

/*
uint8_t spi_transfer(uint8_t data){
	SPDR = data;
	while (!(SPSR & 0x80));
	return SPDR;
}

uint8_t spi_read_byte(void){
	return SPDR;
}*/


uint8_t spi_send_cmd(uint8_t spi_id, uint8_t value){
	config_spi_child(spi_id, LOW);
	SPDR = value;
	while (!(SPSR & 0x80));
	value = SPDR;
	config_spi_child(spi_id, HIGH);
	return value;
}
uint8_t spi_send_cmd_value(uint8_t spi_id, uint8_t cmd, uint8_t value){
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

void spi_send_buffer(uint8_t spi_id, const uint8_t data[], uint8_t len){
	led();
	config_spi_child(spi_id, LOW);
	uint8_t temp;
	while(len--){
		SPDR = *data++;
		// The SPSR's SPIF is cleared by first reading it and then acessing the SPI data register.
		while (!(SPSR & 0x80));
		temp = SPDR;
		
	}
	_delay_ms(1000);
	config_spi_child(spi_id, HIGH);
}
// send cmd with pack_len data, the data in pack_len only has the first data_len useful, but we still send the remaining as 0x00
uint8_t spi_send_cmd_buffer(uint8_t spi_id, uint8_t cmd, const uint8_t buf[], uint8_t data_len, uint8_t pack_len){
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

void spi_recv_buffer(uint8_t spi_id,  uint8_t data[], uint8_t len){
	config_spi_child(spi_id, LOW);
	while(len--){
		SPDR = NOP;
		while (!(SPSR & 0x80));
		*data++ = SPDR;
	}
	config_spi_child(spi_id, HIGH);
}
// send a cmd and recv a buffer of data_len
uint8_t spi_recv_cmd_buffer(uint8_t spi_id, uint8_t cmd, uint8_t data[], uint8_t data_len, uint8_t pack_len){
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


uint8_t spi_data_available(void){
	/* no available return 0, otherwise 0x80 */
	return SPSR & 0x80;
}

