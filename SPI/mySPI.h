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

extern mySPI spi;

void init_spi();

void setup_spi(void);

void config_spi_child(uint8_t spi_id, uint8_t value);
void config_spi_enable(BOOL on);
void config_spi_interrupt(BOOL enabled);
void config_spi_data_mode(SPI_DATA_MODE data_mode);
void _config_spi_data_rate(SPI_DATA_RATE data_rate);
void config_spi_operating_mode(SPI_OPERATING_MODE _op_mode);
void spi_read_buffer2(uint8_t spi_id,  uint8_t data[], uint8_t len);
void config_spi_data_order(SPI_DATA_ORDER _order);
void _config_spi_slave(PIN_ADDR _ss[], uint8_t num);

uint8_t spi_clear_flag(void);

uint8_t spi_transfer(uint8_t data);
uint8_t spi_read_byte(void);

uint8_t spi_exchange_one_byte(uint8_t spi_id, uint8_t value);
uint8_t spi_exchange_two_bytes(uint8_t spi_id, uint8_t cmd, uint8_t value);

void spi_read_buffer(uint8_t spi_id,  uint8_t data[], uint8_t len);
void spi_write_buffer(uint8_t spi_id, const uint8_t data[], uint8_t len);

uint8_t spi_write_cmd_buffer(uint8_t spi_id, uint8_t cmd, const uint8_t buf[], uint8_t data_len, uint8_t pack_len);
uint8_t spi_read_cmd_buffer(uint8_t spi_id, uint8_t cmd, uint8_t data[], uint8_t data_len, uint8_t pack_len);

uint8_t spi_passive_receive(uint8_t buf[], uint8_t len);
uint8_t spi_data_available(void);

#endif
