
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
// Frame error = 1
// when rx disable, the received char just flushed
// when tx disable, the writed char just flushed not shifted.

// write one to clear interrupt.
typedef enum rs232_baudrate{
	/* f = 16MHz, UBRRn, U2Xn error(%)*/
	//B2400, // 832  1      0.0
	//B4800, // 416  1,     -0.1
	B9600 = 207, // 207  1,     0.2
	B14400 = 138, // 138  1,     -0.1
	B19200 = 103,
	B28800 = 68,
	B38400 = 51,
	B57600 = 34, 
	B76800 = 25,
	B115200 = 16
}rs232_baudrate;
typedef enum rs232_parity{
	rs232_parity_odd,
	rs232_parity_even,
	rs232_parity_disable
}rs232_parity;

typedef struct myRS232{
	rs232_baudrate baud_rate;
	uint8_t bit_num;
	uint8_t stop_bit_num;
	uint8_t enabled_mask;
	uint8_t interrupt_enabled_mask; // three independent interrupt
	rs232_parity parity;
}myRS232;

extern myRS232 rs232;

void init_rs232(void);

void setup_rs232(void);

void config_rs232_interrupt(uint8_t interrupt);
void config_rs232_enable(uint8_t enable);
void config_rs232_bit_num(uint8_t bit_num);
void config_rs232_parity(rs232_parity parity);
void config_rs232_stop_bit(uint8_t stop_bit);
void config_rs232_baudrate(rs232_baudrate baudrate);

void send_byte(uint8_t data);
void send_buffer(uint8_t data[], uint8_t len);
uint8_t recv_byte();
void recv_buffer(uint8_t data[], uint8_t len);
void clear_rs232_irq_flg(uint8_t bit);

































