#ifndef __MY_SPI
#define __MY_SPI


#include <avr/io.h>
#include "_common.h"
#define _MAX_SLAVE 8

#define SPI_DATA_AVAILABLE SPSR & _BV(SPIF)

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

typedef struct mySPI{
	uint8_t slave_num;
	BOOL spi_on;
	PIN_ADDR _ss[_MAX_SLAVE];
	BOOL irq_enabled;
	SPI_DATA_MODE data_mode;
	SPI_OPERATING_MODE op_mode;
	SPI_DATA_ORDER data_order;
	SPI_DATA_RATE data_rate;
	
}mySPI;



void init_spi(mySPI * spi);
void setup_spi(mySPI * spi);

void config_spi_enable(mySPI * spi, BOOL on);
void _config_spi_slave(mySPI * spi, PIN_ADDR _ss[], uint8_t num);
void config_spi_interrupt(mySPI* spi, BOOL enabled);
void config_spi_data_mode(mySPI* spi, SPI_DATA_MODE data_mode);
void _config_spi_data_rate(mySPI* spi, SPI_DATA_RATE data_rate);
void config_spi_operating_mode(mySPI* spi, SPI_OPERATING_MODE _op_mode);
void config_spi_data_order(mySPI* spi, SPI_DATA_ORDER _order);

/* read */
uint8_t spi_clear_flag(void);
//uint8_t spi_transfer(uint8_t data);
uint8_t spi_transfer_byte(mySPI* spi, uint8_t to, uint8_t data);
void spi_transfer_buffer(mySPI* spi, uint8_t to, uint8_t send[], uint8_t recv[], uint8_t len);
uint8_t spi_read_byte(void);



#endif
