#ifndef __MY_SPI
#define __MY_SPI
#include "F_CPU.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#define _MAX_SLAVE 8

#define SPI_DATA_AVAILABLE SPSR & 0x80

// TO-DO: provide the data order choice
/*
typedef enum SPI_DATA_ORDER{
	MSB,
	LSB
}SPI_DATA_ORDER;
*/
typedef enum SPI_OPERATING_MODE{
	MASTER,
	SLAVE
}SPI_OPERATING_MODE;

typedef enum SPI_DATA_MODE{
	RISING_SAMPLE,
	RISING_SETUP,
	FALLING_SAMPLE,
	FALLING_SETUP
}SPI_DATA_MODE;

typedef enum SPI_DATA_RATE{
	SCALE_2,
	SCALE_4,
	SCALE_8,
	SCALE_16,
	SCALE_32,
	SCALE_64,
	SCALE_128
}SPI_DATA_RATE;

/**
 * Compilation -std=c++11
 * The default constructor setup SPI as a master node with the pin setting according to Atmega328p.
 * SPI(PinB5->SCK, PinB3->MOSI, PinB4->MISO)
 * The default setting:
 * 		master
 * 		RISING_SAMPLE;
 *		dataOrder = MSB;
 * 		dataRate = SCALE_4;
 * */


class SPI{
private:
	uint8_t _sckPin;
	uint8_t _mosiPin;
	uint8_t _misoPin; 
	uint8_t _chipSelectPin;
	uint8_t _slaveNum;
	uint8_t _ss[_MAX_SLAVE];
	SPI_DATA_MODE _dataMode;
	SPI_OPERATING_MODE _opMode;
	//SPI_DATA_ORDER _dataOrder;
	SPI_DATA_RATE _dataRate;
	
public:
	SPI();
	SPI(uint8_t SCK, uint8_t MOSI, uint8_t MISO);
	SPI(uint8_t SCK, uint8_t MOSI, uint8_t MISO, uint8_t SS);
	void dataRate(SPI_DATA_RATE dataRate);
	void dataMode(SPI_DATA_MODE dataMode);
	void operatingMode(SPI_OPERATING_MODE opMode);

	inline void enableInterrupt(){
		SPCR |= _BV(SPIE);
	}
	inline void disableInterrrupt(){
		SPCR &= ~_BV(SPIE);
	}
	inline void MSB(){
		SPCR &= ~_BV(DORD);
	}
	inline void LSB(){
		SPCR |= _BV(DORD);
	}
	
	inline void enable(){
		SPCR |= _BV(SPE);
	}
	//void enable();
	inline void disable(){
		SPCR &= ~_BV(SPE);
	}
	void hardwareSetup();
	uint8_t dataAvailable(void);
	uint8_t clearFlag(void);
	uint8_t addSlave(uint8_t pin);
	void removeSlave(uint8_t slaveID);
	int8_t selectSlave(uint8_t slaveID, uint8_t value);
	uint8_t transfer(uint8_t data) ;
	uint16_t transfer16(uint16_t data);
	void transfer(void *buf, size_t count) ;
};


 


#endif
