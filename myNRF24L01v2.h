
#ifndef _MYNRF24L01_V2_
#define _MYNRF24L01_V2_

#include "nRF24L01.h"


#define STATUS 0x07


#define LOW 0
#define HIGH 1
#define CE_ENABLE PORTD |= 0x80
#define CE_DISABLE PORTD &= 0x7F


#define CLEAR_IRQ write_register(STATUS,0x70)
#define POWER_UP write_register(NRF_CONFIG,read_register(NRF_CONFIG) | 0x02)
#define POWER_DOWN write_register(NRF_CONFIG,read_register(NRF_CONFIG) & 0xFD)

#define IRQ_RX_DS 0x40
#define IRQ_TX_DS 0x20
#define IRQ_MAX_RT 0x10
#define IRQ_DISABLE 0x00

#define _min(a,b) ((a<b)?(a):(b))



typedef unsigned char uint8_t;
typedef unsigned char uchar;
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


typedef enum BOOL_{
	TRUE,
	FALSE
}BOOL;




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
}NRF24L01P;




void init_rf24l01p(NRF24L01P * rf24);


void setup_rf24l01p(NRF24L01P * rf24);

void config_irq_mask(NRF24L01P * rf24, uint8_t _irq_mask);
void config_payload_length(NRF24L01P * rf24, uint8_t _sp);
void config_addr_width(NRF24L01P * rf24, uint8_t _addr_width);
void config_retransmit(NRF24L01P * rf24, uint8_t _rt, RETRAN_INTERVAL _interval);
void config_pa_level(NRF24L01P * rf24, PA_LEVEL level);
void config_data_rate(NRF24L01P * rf24, RF_RATE speed);
void config_channel(NRF24L01P * rf24, uint8_t channel);

void config_CRC(NRF24L01P * rf24, CRC_DEF length);

void config_dynamic_payload(NRF24L01P * rf24, BOOL value);



/*************************************************************************/

void config_destination_addr(const NRF24L01P*rf, uint8_t addr[]);
void config_listening_addr(const NRF24L01P*rf, uint8_t addr[]);

uint8_t send(const NRF24L01P*rf, uint8_t data[], uint8_t len);
uint8_t recv(const NRF24L01P*rf, uint8_t data[], uint8_t len);

uint8_t available2();

uint8_t flush_rx(void);
uint8_t flush_tx(void);


uint8_t read_buffer(uint8_t reg, uint8_t* buf, uint8_t len);
uint8_t read_register(uint8_t reg);
uint8_t write_buffer(uint8_t reg, const uint8_t* buf, uint8_t len);
uint8_t write_register(uint8_t reg, uint8_t value);
// uint8_t write_payload(const uint8_t* buf, uint8_t len);

//uint8_t read_payload(uint8_t* buf, uint8_t len);
// void toggle_features(void);

void csn(int mode);
uint8_t get_status(void);


#endif // 
