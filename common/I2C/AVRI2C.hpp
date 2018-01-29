#ifndef _I2C_H__
#define _I2C_H__

#include <inttypes.h>

#include <math.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
//#include <util/delay.h>

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4

static volatile uint8_t twi_error;
typedef struct TWIConfig{
	volatile uint8_t twi_state;
	volatile uint8_t twi_slarw;
	volatile uint8_t twi_sendStop;      // should the transaction end with a stop
	volatile uint8_t twi_inRepStart;     // in the middle of a repeated start
	void (*twi_onSlaveTransmit)(void);
	void (*twi_onSlaveReceive)(uint8_t*, int);

	uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
	volatile uint8_t twi_masterBufferIndex;
	volatile uint8_t twi_masterBufferLength;

	uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
	volatile uint8_t twi_txBufferIndex;
	volatile uint8_t twi_txBufferLength;

	uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
	volatile uint8_t twi_rxBufferIndex;
	volatile uint8_t twi_error;
	
}TWIConfig;

  


class TWI{
public:
	void twi_init();
	void twi_disable();
};
class TWIMaster: public TWI{
public:
	/* master functions */
	void twi_setFrequency(uint8_t frequency);
	uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop);
	uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop);
	
};
class TWISlave:public TWI{
public:
	/* slave functions */
	uint8_t twi_setAddress(uint8_t address);
	uint8_t twi_transmit(const uint8_t * data, uint8_t length); // prepare data
	void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) );
	void twi_attachSlaveTxEvent( void (*function)(void) );
};


#endif//
