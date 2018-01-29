#ifndef _MY_RS232__
#define _MY_RS232__
#include <avr/io.h>
#include <stdint.h>

#define RS232_EN_INT_RX 0x80
#define RS232_EN_INT_TX 0x40
#define RS232_EN_INT_DRE 0x20
#define RS232_EN_INT_MASK 0x1F

#define RS232_CLEAR_RX_F 0x80
#define RS232_CLEAR_TX_F 0x40
#define RS232_CLEAR_DR_F 0x20

#define RS232_EN_RX 0x10
#define RS232_EN_TX 0x08
#define RS232_EN_MASK 0xE7



#define B9600 207
#define B14400 138
#define B19200 103

#define MAX_DATA_LEN 16
/*
 * I defined a data packet that used to transfer data between devices.
 * The data packet contains three fields, 
 * The first field is a byte that is the length of the data array. The maximum length of data array is defined by MAX_DATA_LEN.
 * The second field is a byte that used to conut currently recevied bytes.
 * The third is a data array whose maxmimum size is defined by MAX_DATA_LEN
 * 
 * It is primarily used for a computer sending data to the microcontroller.
 * The computer must send the total size of the data, if the specific size is greater than MAX_DATA_LEN, the microcontroller only accept the first MAX_DATA_LEN.
 * */
struct RS232Data{
	uint8_t len;
	uint8_t offset;
	uint8_t data[MAX_DATA_LEN];
};
void init_rs232(void);
void enable_rs232(void);
void disable_rs232(void);
void send_byte(uint8_t data);
uint8_t recv_byte(void);
void send(struct RS232Data * data);

#endif // _MY_RS232__





























