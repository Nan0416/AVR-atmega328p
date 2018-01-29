#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "mySPI.h"
#include <util/delay.h>


void init_spi(){
	int i = 0;
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
	// disable interrupt for safety
	uint8_t sreg = SREG;
	config_spi_operating_mode(spi.op_mode);
	config_spi_data_mode(spi.data_mode);
	if(spi.data_order == MSB){
		SPI_MSB;
	}else{
		SPI_LSB;
	}
	SPI_DISABLE_INTERRUPT;
	SREG = sreg;
}
/*
 * set master / slave
 * */
void config_spi_operating_mode(SPI_OPERATING_MODE _op_mode){
	if(_op_mode == MASTER){
		SPCR |= _BV(MSTR);
		_config_spi_data_rate(spi.data_rate);
		/* direction */
		DDRB |= 0x28; // MOSI, SCK output
		DDRB &= 0xEF; // MISO
	}else{
		SPCR &= ~_BV(MSTR);
		DDRB |= 0x10; // MISO output
		DDRB &= 0xD3; // SS, SCK, MOSI input
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

int8_t spi_select_slave(uint8_t slave_id, uint8_t value){
	
	if(spi._ss[slave_id].port == UNDEFINED){
		return -1;
	} 
	if(value == HIGH){
		_delay_us(500);
		switch(spi._ss[slave_id].port){
			case PB: DDRB |= _BV(spi._ss[slave_id].pin); PORTB |= _BV(spi._ss[slave_id].pin);break;
			case PC: DDRC |= _BV(spi._ss[slave_id].pin); PORTC |= _BV(spi._ss[slave_id].pin);break;
			case PD: DDRD |= _BV(spi._ss[slave_id].pin); PORTD |= _BV(spi._ss[slave_id].pin);break; 
			default: break;
		}
	}else{
		switch(spi._ss[slave_id].port){
			case PB: DDRB |= _BV(spi._ss[slave_id].pin); PORTB &= ~_BV(spi._ss[slave_id].pin);break;
			case PC: DDRC |= _BV(spi._ss[slave_id].pin); PORTC &= ~_BV(spi._ss[slave_id].pin);break;
			case PD: DDRD |= _BV(spi._ss[slave_id].pin); PORTD &= ~_BV(spi._ss[slave_id].pin);break; 
			default: break;
		}
		_delay_us(500);
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

uint8_t spi_data_available(void){
	/* no available return 0, otherwise 0x80 */
	return SPSR & _BV(SPIF);
}

void spi_transfer_buffer(uint8_t slave_id, void *buf, size_t count){
    if (count == 0) return;
    spi_select_slave(slave_id, LOW);
    uint8_t *p = (uint8_t *)buf;
    SPDR = *p;
    while (--count > 0) {
	  _delay_us(20);
      uint8_t out = *(p + 1);
      while (!(SPSR & _BV(SPIF))) ;
      uint8_t in = SPDR;
      SPDR = out;
      *p++ = in;
    }
    while (!(SPSR & _BV(SPIF)));
    *p = SPDR;
    spi_select_slave(slave_id, HIGH);
 }


uint8_t spi_transfer_byte(uint8_t slave_id, uint8_t value){
	spi_select_slave(slave_id, LOW);
	SPDR = value;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
    value = SPDR;
	spi_select_slave(slave_id, HIGH);
	return value;
}
