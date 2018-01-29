#ifndef __MY_SPI
#define __MY_SPI

#include <avr/io.h>
#include "_common.h"
#define _MAX_SLAVE 8

#define SPI_DATA_AVAILABLE SPSR & 0x80
#define spi_read_one_byte SPDR

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
void config_spi_interrupt(BOOL enabled);
/*
 * set data transfer mode
 * */
void config_spi_data_mode(SPI_DATA_MODE data_mode);
void config_spi_data_order(SPI_DATA_ORDER _order);
void config_spi_enable(BOOL on);
// return _MAX_SLAVE means failure,
uint8_t add_slave(MEGA_PORT port, MEGA_PIN pin);
void remove_slave(uint8_t spi_id);
int8_t config_spi_child(uint8_t spi_id, uint8_t value);

/*
 * read flag and then read data register will clear the SPI interrutp flag.
 * */
uint8_t spi_clear_flag(void);

uint8_t spi_send_cmd(uint8_t spi_id, uint8_t value);
uint8_t spi_send_cmd_value(uint8_t spi_id, uint8_t cmd, uint8_t value);
void spi_send_buffer(uint8_t spi_id, const uint8_t data[], uint8_t len);

// send cmd with pack_len data, the data in pack_len only has the first data_len useful, but we still send the remaining as 0x00
uint8_t spi_send_cmd_buffer(uint8_t spi_id, uint8_t cmd, const uint8_t buf[], uint8_t data_len, uint8_t pack_len);
void spi_recv_buffer(uint8_t spi_id,  uint8_t data[], uint8_t len);

// send a cmd and recv a buffer of data_len
uint8_t spi_recv_cmd_buffer(uint8_t spi_id, uint8_t cmd, uint8_t data[], uint8_t data_len, uint8_t pack_len);
uint8_t spi_data_available(void);
#endif
