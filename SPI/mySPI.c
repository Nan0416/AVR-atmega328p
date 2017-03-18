#include "mySPI.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

/*
 * spi off, master mode, #slave = 1
 * irq off, rising-sample, msb scale 4
 * */
void init_spi(mySPI * spi){
	spi->spi_on = FALSE;
	spi->slave_num = 1;
	spi->_ss[0].port = PB;
	spi->_ss[0].pin = P2; 
	
	spi->irq_enabled = FALSE;
	spi->data_mode = RISING_SAMPLE;
	spi->op_mode = MASTER;
	spi->data_order = MSB;
	spi->data_rate = SCALE_4;
	
}

void setup_spi(mySPI * spi){
	config_spi_enable(spi, spi->spi_on);
	config_spi_operating_mode(spi, spi->op_mode);
	config_spi_interrupt(spi, spi->irq_enabled);
	config_spi_data_mode(spi, spi->data_mode);
	config_spi_data_order(spi, spi->data_order);
		
}

void config_spi_enable(mySPI * spi, BOOL on){
	if(on == TRUE){
		SPCR |= _BV(SPE);
	}else{
		SPCR &= ~_BV(SPE);
	}
	spi->spi_on = on;
	
}
void config_spi_interrupt(mySPI* spi, BOOL enabled){
	if(enabled == TRUE){
		SPCR |= _BV(SPIE);
	}else{
		SPCR &= ~_BV(SPIE);
	}
	spi->irq_enabled = enabled;
	
}

void config_spi_data_mode(mySPI* spi, SPI_DATA_MODE data_mode){
	switch(data_mode){
		case RISING_SAMPLE: SPCR &= 0xF3; break;
		case RISING_SETUP: SPCR &= 0xF7; SPCR |= 0x04; break;
		case FALLING_SAMPLE: SPCR |= 0x08; SPCR &= 0xFB; break;
		case FALLING_SETUP: SPCR |= 0x0C; break;
		default: SPCR &= 0xF3; break;
	}
	spi->data_mode = data_mode;
}

void _config_spi_data_rate(mySPI* spi, SPI_DATA_RATE data_rate){
	
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
	spi->data_rate = data_rate;
	
}

void config_spi_operating_mode(mySPI* spi, SPI_OPERATING_MODE _op_mode){
	if(_op_mode == MASTER){
		SPCR |= _BV(MSTR);
		_config_spi_data_rate(spi, spi->data_rate);
		_config_spi_slave(spi, spi->_ss, spi->slave_num);
		/* direction */
		DDRB |= 0x2C;
		DDRB &= 0xEF;
		PORTB |= 0x04;
	}else{
		SPCR &= ~_BV(MSTR);
		DDRB |= 0x10;
		DDRB &= 0xD3;
	}
	spi->op_mode = _op_mode;
}

void config_spi_data_order(mySPI* spi, SPI_DATA_ORDER _order){
	if(_order == MSB){
		SPCR &= ~_BV(DORD);
	}else{
		SPCR |= _BV(DORD);
	}
	spi->data_order = _order;
}

void _config_spi_slave(mySPI * spi, PIN_ADDR _ss[], uint8_t num){
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
		spi->_ss[i].port = _ss[i].port;
		spi->_ss[i].pin = _ss[i].pin;
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
uint8_t spi_transfer_byte(mySPI* spi, uint8_t to, uint8_t data){
	/* set low */
	if(to > _MAX_SLAVE){
		return 0;
	}
	switch(spi->_ss[to].port){
		case PB: DDRB |= _BV(spi->_ss[to].pin); PORTB &= ~_BV(spi->_ss[to].pin);break;
		case PC: DDRC |= _BV(spi->_ss[to].pin); PORTC &= ~_BV(spi->_ss[to].pin);break;
		case PD: DDRD |= _BV(spi->_ss[to].pin); PORTD &= ~_BV(spi->_ss[to].pin);break; 
		default: break;
	}
	_delay_us(50);
	SPDR = data;
	while (!(SPSR & 0x80));
	_delay_us(50);
	_delay_ms(1000);
	switch(spi->_ss[to].port){
		case PB: DDRB |= _BV(spi->_ss[to].pin); PORTB |= _BV(spi->_ss[to].pin);break;
		case PC: DDRC |= _BV(spi->_ss[to].pin); PORTC |= _BV(spi->_ss[to].pin);break;
		case PD: DDRD |= _BV(spi->_ss[to].pin); PORTD |= _BV(spi->_ss[to].pin);break; 
		default: break;
	}
	/* debug */
	
	

	return SPDR;
}

void spi_transfer_buffer(mySPI* spi, uint8_t to, uint8_t send[], uint8_t recv[], uint8_t len){
	/* set low */
	if(to > _MAX_SLAVE){
		return ;
	}
	switch(spi->_ss[to].port){
		case PB: DDRB |= _BV(spi->_ss[to].pin); PORTB &= ~_BV(spi->_ss[to].pin);break;
		case PC: DDRC |= _BV(spi->_ss[to].pin); PORTC &= ~_BV(spi->_ss[to].pin);break;
		case PD: DDRD |= _BV(spi->_ss[to].pin); PORTD &= ~_BV(spi->_ss[to].pin);break; 
		default: break;
	}
	_delay_us(50);
	int i = 0;
	for(;i < len; i++){
		SPDR = send[i];
		while (!(SPSR & 0x80));
		recv[i] = SPDR;
	}

	_delay_us(50);
	switch(spi->_ss[to].port){
		case PB: DDRB |= _BV(spi->_ss[to].pin); PORTB |= _BV(spi->_ss[to].pin);break;
		case PC: DDRC |= _BV(spi->_ss[to].pin); PORTC |= _BV(spi->_ss[to].pin);break;
		case PD: DDRD |= _BV(spi->_ss[to].pin); PORTD |= _BV(spi->_ss[to].pin);break; 
		default: break;
	}
	
	
}
uint8_t spi_read_byte(void){
	return SPDR;
}



