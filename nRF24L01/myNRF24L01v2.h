
#ifndef _MYNRF24L01_V2_
#define _MYNRF24L01_V2_

#include "nRF24L01.h"
#include "_common.h"

#define STATUS 0x07

#define CE_ENABLE PORTD |= 0x80
#define CE_DISABLE PORTD &= 0x7F

#define CLEAR_IRQ write_register(STATUS,0x70)
#define POWER_UP write_register(NRF_CONFIG,read_register(NRF_CONFIG) | 0x02)
#define POWER_DOWN write_register(NRF_CONFIG,read_register(NRF_CONFIG) & 0xFD)

#define IRQ_RX_DS 0x40
#define IRQ_TX_DS 0x20
#define IRQ_MAX_RT 0x10
#define IRQ_DISABLE 0x00

#define write_register(reg, value) spi_exchange_two_bytes(rf24.spi_id, W_REGISTER | ( REGISTER_MASK & reg ), value)
#define read_register(reg) spi_exchange_two_bytes(rf24.spi_id, R_REGISTER | ( REGISTER_MASK & reg ), NOP)
#define get_status() spi_exchange_one_byte(rf24.spi_id, NOP)

#define available2() (!(read_register(FIFO_STATUS) & _BV(RX_EMPTY)))

#define flush_rx() spi_exchange_one_byte(rf24.spi_id, FLUSH_RX)
#define flush_tx() spi_exchange_one_byte(rf24.spi_id, FLUSH_TX)

typedef enum PA_LEVEl{
	PA_0,
	PA_N_6,
	PA_N_12,
	PA_N_18
}PA_LEVEL;

typedef enum RF_RATE{
	RATE_1MBPS,
	RATE_2MBPS,
	RATE_250KBPS
}RF_RATE;

typedef enum CRC_DEF{
	CRC_DISABLE,
	CRC_ENABLE_2,
	CRC_ENABLE_1
}CRC_DEF;

typedef enum PIPE_NUM{
	PIPE0,
	PIPE1,
	PIPE2,
	PIPE3,
	PIPE4,
	PIPE5,
	PIPE_ALL
}PIPE_NUM;

typedef enum RETRAN_INTERVAL{
	I_250, I_500,
	I_750, I_1000,
	I_1250, I_1500,
	I_1750, I_2000,
	I_2250, I_2500,
	I_2750, I_3000,
	I_3250, I_3500,
	I_3750, I_4000
}RETRAN_INTERVAL;

typedef struct NRF24L01P{
	BOOL dynamic_payloads_enabled;
	RF_RATE transfer_rate;
	PA_LEVEL gain_value;
	CRC_DEF crc_value;
	BOOL ack_enabled;
	uint8_t channel;
	uint8_t static_payload_length;
	uint8_t retran_times;
	RETRAN_INTERVAL retran_interval;
	uint8_t addr_width;
	uint8_t irq_mask;
	PIN_ADDR ce;
	uint8_t spi_id;
}NRF24L01P;
extern NRF24L01P rf24;

void init_rf24l01p();

void setup_rf24l01p();

void config_ce( uint8_t value);
void config_irq_mask( uint8_t _irq_mask);
void config_payload_length(uint8_t _sp);
void config_addr_width(uint8_t _addr_width);
void config_retransmit(uint8_t _rt, RETRAN_INTERVAL _interval);
void config_pa_level(PA_LEVEL level);
void config_data_rate(RF_RATE speed);
void config_channel(uint8_t channel);
void config_CRC( CRC_DEF length);
void config_dynamic_payload(BOOL value);
void config_destination_addr( uint8_t addr[]);
void config_listening_addr( uint8_t addr[]);

uint8_t send(const uint8_t data[], uint8_t len);
uint8_t recv(uint8_t data[], uint8_t len);

uint8_t read_buffer(uint8_t reg, uint8_t* buf, uint8_t len);
uint8_t write_buffer(uint8_t reg, const uint8_t* buf, uint8_t len);

#endif // 
