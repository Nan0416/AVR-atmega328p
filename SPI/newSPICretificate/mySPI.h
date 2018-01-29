#ifndef __MY_SPI
#define __MY_SPI

#include <avr/io.h>
#include "_common.h"
#define _MAX_SLAVE 8

#define SPI_DATA_AVAILABLE SPSR & 0x80
#define SPI_ENABLE SPCR |= _BV(SPE)
#define SPI_DISABLE SPCR &= ~_BV(SPE)

#define SPI_MSB SPCR &= ~_BV(DORD)
#define SPI_LSB SPCR |= _BV(DORD)

#define SPI_ENABLE_INTERRUPT  SPCR |= _BV(SPIE)
#define SPI_DISABLE_INTERRUPT  SPCR &= ~_BV(SPIE)


//when setting as master, we use 

typedef enum SPI_DATA_ORDER{
	MSB,
	LSB
}SPI_DATA_ORDER;

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

typedef struct SPISettings{
	uint8_t slave_num;
	PIN_ADDR _ss[_MAX_SLAVE];
	SPI_DATA_MODE data_mode;
	SPI_OPERATING_MODE op_mode;
	SPI_DATA_ORDER data_order;
	SPI_DATA_RATE data_rate;
	
}SPISettings;

extern SPISettings spi;


#define spi_exchange(spi_id, value) spi_send_cmd(spi_id, value)

/*
 * disable SPI, disable interrupt, data mode rising_sample, master mode, MSB, clock scale 4, no slave
 * */
void init_spi();
void setup_spi(void);

/*
 * set master / slave
 * */
void config_spi_operating_mode(SPI_OPERATING_MODE _op_mode);
void _config_spi_data_rate(SPI_DATA_RATE data_rate);
/*
 * set data transfer mode
 * */
void config_spi_data_mode(SPI_DATA_MODE data_mode);

// return _MAX_SLAVE means failure,
uint8_t add_slave(MEGA_PORT port, MEGA_PIN pin);
void remove_slave(uint8_t spi_id);
int8_t spi_select_slave(uint8_t spi_id, uint8_t value);

// read flag and then read data register will clear the SPI interrutp flag.
uint8_t spi_clear_flag(void);
uint8_t spi_data_available(void);


uint8_t spi_transfer_byte(uint8_t slave_id, uint8_t value);
void spi_transfer_buffer(uint8_t slave_id, void *buf, size_t count);

#endif
