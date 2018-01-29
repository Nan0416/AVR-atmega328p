#include "F_CPU.h"
#include "SPI.hpp"
#include <util/delay.h>
#include "common.h"

SPI::SPI():SPI(PinB5, PinB3, PinB4){//C++11
	_opMode = MASTER;
}
SPI::SPI(uint8_t SCK, uint8_t MOSI, uint8_t MISO):SPI(SCK,  MOSI, MISO, UNDEFINED){
	_opMode = MASTER;
}
SPI::SPI(uint8_t SCK, uint8_t MOSI, uint8_t MISO, uint8_t SS):_sckPin(SCK), _mosiPin(MOSI), _misoPin(MISO), _chipSelectPin(SS){
	_dataMode = RISING_SAMPLE;
	_opMode = MASTER;
	//_dataOrder = MSB;
	_dataRate = SCALE_4;
	_slaveNum = 0;
	//initialize slave
	int i;
	for(i = 0; i< _MAX_SLAVE; i++){
		_ss[i] = UNDEFINED;
	}
	
}

void SPI::dataRate(SPI_DATA_RATE dataRate){
	switch(dataRate){
		case SCALE_2: SPSR |= 0x01; SPCR &= 0xFC; break;
		case SCALE_4: SPSR &= 0xFE; SPCR &= 0xFC; break;
		case SCALE_8: SPSR |= 0x01;  SPCR |= 0x01; SPCR &= 0xFD; break;
		case SCALE_16: SPSR &= 0xFE; SPCR |= 0x01; SPCR &= 0xFD; break;
		case SCALE_32: SPSR |= 0x01; SPCR &= 0xFE; SPCR |= 0x02; break;
		case SCALE_64: SPSR |= 0x01; SPCR |= 0x03; break;
		case SCALE_128: SPSR &= 0xFE; SPCR |= 0x03; break;
		default:  SPSR &= 0xFE; SPCR &= 0xFC; break;
	}
	this->_dataRate = dataRate;
}
void SPI::dataMode(SPI_DATA_MODE dataMode){
	switch(dataMode){
		case RISING_SAMPLE: SPCR &= 0xF3; break; // first-rising-first-sample
		case RISING_SETUP: SPCR &= 0xF7; SPCR |= 0x04; break; // first-rising-second-sample
		case FALLING_SAMPLE: SPCR |= 0x08; SPCR &= 0xFB; break; // first-falling-first-sample
		case FALLING_SETUP: SPCR |= 0x0C; break; //first-falling-second-sample
		default: SPCR &= 0xF3; break;
	}
	this->_dataMode = dataMode;
}
void SPI::operatingMode(SPI_OPERATING_MODE opMode){
	if(opMode == MASTER){
		SPCR |= _BV(MSTR);
		/* direction */
		//DDRB |= 0x28; // MOSI, SCK output
		//DDRB &= 0xEF; // MISO
		pinMode(_misoPin, INPUT);
		pinMode(_mosiPin, OUTPUT);
		pinMode(_sckPin, OUTPUT);
	}else{
		SPCR &= ~_BV(MSTR);
		//DDRB |= 0x10; // MISO output
		//DDRB &= 0xD3; // SS, SCK, MOSI input
		pinMode(_misoPin, OUTPUT);
		pinMode(_mosiPin, INPUT);
		pinMode(_sckPin, INPUT);
		pinMode(_chipSelectPin, INPUT);
	}
	this->_opMode = opMode;
}

void SPI::hardwareSetup(){
	uint8_t sreg = SREG;
	operatingMode(_opMode);
	if(_opMode == MASTER){
		dataRate(_dataRate);
	}
	dataMode(_dataMode);
	MSB();
	disableInterrrupt();
	SREG = sreg;
}



uint8_t SPI::dataAvailable(void){
	/* no available return 0, otherwise 0x80 */
	return SPSR & _BV(SPIF);
}
// read flag and then read data register will clear the SPI interrutp flag.
uint8_t SPI::clearFlag(void){
	uint8_t rs = 0;
	if(SPI_DATA_AVAILABLE){
		rs = SPDR;
	}
	rs = 0;
	return rs;
}


// return _MAX_SLAVE means failure,
uint8_t SPI::addSlave(uint8_t pin){
	//find next avaliable
	uint8_t i = _slaveNum;
	uint8_t flag = 0;
	for(; i < _slaveNum + _MAX_SLAVE; i++){
		if(_ss[i % _MAX_SLAVE] == UNDEFINED){
			_slaveNum = i % _MAX_SLAVE;
			flag = 1;
			break;
		}
	}
	if(flag == 0){
		return _MAX_SLAVE;
	}
	_ss[_slaveNum] = pin;
	pinMode(pin, OUTPUT);
	pinSet(pin, HIGH);
	return _slaveNum;
	
}
void SPI::removeSlave(uint8_t slaveID){
	if(slaveID < _MAX_SLAVE){
		_ss[slaveID] = UNDEFINED;
	}
}



int8_t SPI::selectSlave(uint8_t slaveID, uint8_t value){
	uint8_t pin = _ss[slaveID];
	if(pin == UNDEFINED){
		return -1;
	} 
	if(value == DISABLE){
		_delay_us(500);
		pinSet(pin, HIGH);
	}else{
		pinSet(pin, ENABLE);
		_delay_us(500);
	}
	return 1;
}


// Write to the SPI bus (MOSI pin) and also receive (MISO pin)
uint8_t SPI::transfer(uint8_t data) {
    SPDR = data;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
    return SPDR;
  }
uint16_t transfer16(uint16_t data) {
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
    in.val = data;
    if (!(SPCR & _BV(DORD))) {
      SPDR = in.msb;
      asm volatile("nop"); // See transfer(uint8_t) function
      while (!(SPSR & _BV(SPIF))) ;
      out.msb = SPDR;
      SPDR = in.lsb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.lsb = SPDR;
    } else {
      SPDR = in.lsb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.lsb = SPDR;
      SPDR = in.msb;
      asm volatile("nop");
      while (!(SPSR & _BV(SPIF))) ;
      out.msb = SPDR;
    }
    return out.val;
  }
void SPI::transfer(void *buf, size_t count) {
    if (count == 0) return;
    uint8_t *p = (uint8_t *)buf;
    SPDR = *p;
    while (--count > 0) {
      uint8_t out = *(p + 1);
      while (!(SPSR & _BV(SPIF))) ;
      uint8_t in = SPDR;
      SPDR = out;
      *p++ = in;
    }
    while (!(SPSR & _BV(SPIF))) ;
    *p = SPDR;
  }
