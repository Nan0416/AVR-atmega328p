#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "myNRF24L01v2.h"
#include "mySPI.h"

void init_rf24l01p(){
	
	rf24.addr_width = 5;
	rf24.channel = 76;
	rf24.transfer_rate = RATE_250KBPS;
	rf24.gain_value = PA_0;
	rf24.crc_value = CRC_ENABLE_2;
	rf24.dynamic_payloads_enabled = FALSE;
	rf24.ack_enabled = TRUE;
	rf24.static_payload_length = 32;
	rf24.retran_interval= I_1500;
	rf24.irq_mask = IRQ_RX_DS;
	rf24.retran_times = 12;
	rf24.ce.port = PC;
	rf24.ce.pin = P3;
	rf24.spi_id = 0;
}

void setup_rf24l01p(){
	config_ce(LOW);
	config_spi_child(0, HIGH);
	_delay_ms( 5 );
	
	config_addr_width(rf24.addr_width);
	config_dynamic_payload(rf24.dynamic_payloads_enabled);
	config_CRC(rf24.crc_value);
	config_channel(rf24.channel);
	config_data_rate(rf24.transfer_rate);
	config_pa_level(rf24.gain_value);
	config_retransmit(rf24.retran_times, rf24.retran_interval);
	config_irq_mask(rf24.irq_mask);
	if(rf24.dynamic_payloads_enabled == FALSE){
		config_payload_length(rf24.static_payload_length);
	}
	CLEAR_IRQ;
	flush_rx();
	flush_tx();
	
	POWER_UP;
	/* enter standby-I */
	
	
}
void config_ce(uint8_t value){
	if(value == HIGH){
		switch(rf24.ce.port){
			case PB: DDRB |= _BV(rf24.ce.pin); PORTB |= _BV(rf24.ce.pin);break;
			case PC: DDRC |= _BV(rf24.ce.pin); PORTC |= _BV(rf24.ce.pin);break;
			case PD: DDRD |= _BV(rf24.ce.pin); PORTD |= _BV(rf24.ce.pin);break; 
			default: break;
		}
	}else{
		switch(rf24.ce.port){
			case PB: DDRB |= _BV(rf24.ce.pin); PORTB &= ~_BV(rf24.ce.pin);break;
			case PC: DDRC |= _BV(rf24.ce.pin); PORTC &= ~_BV(rf24.ce.pin);break;
			case PD: DDRD |= _BV(rf24.ce.pin); PORTD &= ~_BV(rf24.ce.pin);break; 
			default: break;
		}
	}
}

void config_irq_mask(uint8_t _irq_mask){
	_irq_mask = 0x70 & _irq_mask;
	if(_irq_mask & 0x40){
		/* rx_dr */
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) & 0xBF);
	}else{
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) | 0x40);
	}
	if(_irq_mask & 0x20){
		/* rx_dr */
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) & 0xDF);
	}else{
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) | 0x20);
	}
	if(_irq_mask & 0x10){
		/* rx_dr */
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) & 0xEF);
	}else{
		write_register(NRF_CONFIG, read_register(NRF_CONFIG) | 0x10);
	}
	rf24.irq_mask = _irq_mask;
}

void config_payload_length(uint8_t _sp){
	if(_sp > 32){
		_sp = 32;
	}
	/* only do pipe 0 */
	write_register(RX_PW_P0, _sp);
	rf24.static_payload_length = _sp;
}

void config_addr_width(uint8_t _addr_width){
	if(_addr_width != 3 || _addr_width != 4 || _addr_width != 5){
		_addr_width = 5;
	}
	
	if(_addr_width == 5){
		write_register(SETUP_AW, 0x03);
	}else if(_addr_width == 4){
		write_register(SETUP_AW, 0x02);
	}else{
		write_register(SETUP_AW, 0x01);
	}
	rf24.addr_width = _addr_width;
}

void config_retransmit(uint8_t _rt, RETRAN_INTERVAL _interval){
	/* fake */
	uint8_t retry = 0x00;
	switch(_interval){
	case I_250: retry = 0x00; break;	
	case I_500: retry = 0x10; break;
	case I_750: retry = 0x20; break;
	case I_1000: retry = 0x30; break;
	case I_1250: retry = 0x40; break;	
	case I_1500: retry = 0x50; break;
	case I_1750: retry = 0x60; break;
	case I_2000: retry = 0x70; break;
	case I_2250: retry = 0x80; break;	
	case I_2500: retry = 0x90; break;
	case I_2750: retry = 0xA0; break;
	case I_3000: retry = 0xB0; break;
	case I_3250: retry = 0xC0; break;	
	case I_3500: retry = 0xD0; break;
	case I_3750: retry = 0xE0; break;
	case I_4000: retry = 0xF0; break;
	}
	retry |= 0x0F & _rt;
	write_register(SETUP_RETR, retry);
	rf24.retran_interval = _interval;
	rf24.retran_times = _rt;
}

void config_pa_level(PA_LEVEL level){
	uint8_t setup = read_register(RF_SETUP) ;
	setup &= ~(_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH));
	
	if ( level == PA_0){
		setup |= 0x06 ;
	}else if ( level == PA_N_6 ){
		setup |= 0x04 ;
	}else if ( level == PA_N_12 ){
		setup |= 0x02;
	}else if ( level == PA_N_18 ){
		/* del*/
    }
	write_register( RF_SETUP, setup );
	rf24.gain_value = level;
}

void config_data_rate(RF_RATE speed)
{
	
	uint8_t setup = read_register(RF_SETUP) ;
	setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;
	if( speed == RATE_250KBPS ){
		setup |= 0x20 ;
	}else if(speed == RATE_2MBPS){
		setup |= 0x08;
    }else{
		/* 1Mbps */
    }
	write_register(RF_SETUP,setup);
	rf24.transfer_rate = speed;
}

void config_channel(uint8_t channel){
	channel &= CHANNEL_MASK;
	write_register(RF_CH,channel);
	rf24.channel = channel;
}

void config_CRC(CRC_DEF length){
	uint8_t config = read_register(NRF_CONFIG) & 0xF3 ;
	if ( length == CRC_DISABLE ){
		/* Do nothing, we turned it off above. */
	}else if ( length == CRC_ENABLE_1 ){
		config |= 0x08;
	}else{
		config |= _BV(EN_CRC);
		config |= _BV( CRCO );
	}
	write_register(NRF_CONFIG, config );
	rf24.crc_value = length;
}

void config_dynamic_payload(BOOL value){
	/* limitation: modify all pipes */
	
	if(value == TRUE){
		/* must enable auto ack */
		write_register(EN_AA, 0x3F);
		write_register(FEATURE, read_register(FEATURE) | ~_BV(EN_DPL));
		write_register(DYNPD,0x3F);
	}else{
		/* */
		write_register(FEATURE, read_register(FEATURE) & ~_BV(EN_DPL));
		write_register(DYNPD,0);
	}
	rf24.dynamic_payloads_enabled = value;
}

void config_destination_addr(uint8_t addr[]){
	config_ce(LOW);
	CLEAR_IRQ;
	_delay_us(150);
	write_buffer(TX_ADDR, addr, rf24.addr_width);
	if(rf24.ack_enabled == TRUE){
		/* enable pipe0 for receiving ack packet */
		write_buffer(RX_ADDR_P0, addr, rf24.addr_width);
		write_register(EN_RXADDR, read_register(EN_RXADDR) | 0x01);
	}
}

void config_listening_addr(uint8_t addr[]){
	/* rx mode */
	write_register(NRF_CONFIG, read_register(NRF_CONFIG) | 0x01);
	/* clear flag */
	CLEAR_IRQ;
	
	/* enable this pipe*/
	write_register(EN_RXADDR, read_register(EN_RXADDR) | 0x01);
	write_buffer(RX_ADDR_P0, addr, rf24.addr_width);
	if(rf24.ack_enabled == TRUE){
		/* enable ack */
		write_register(EN_AA, read_register(EN_AA) | 0x01);
	}
	POWER_UP;
	// wait for the radio to come up (130us actually only needed)
	_delay_us(130);
	config_ce(HIGH);
}

/*************************************************/

uint8_t send(const uint8_t data[], uint8_t len){
	flush_rx();
	flush_tx();
	
	spi_exchange_two_bytes(rf24.spi_id,  W_REGISTER | ( REGISTER_MASK & NRF_CONFIG ), read_register(NRF_CONFIG) & 0xFE); 
	POWER_UP; 
	_delay_us(150);
	CLEAR_IRQ;
	if(rf24.ack_enabled == TRUE){
		spi_write_cmd_buffer(rf24.spi_id, W_TX_PAYLOAD, data, len, rf24.static_payload_length);
	}else{
		spi_write_cmd_buffer(rf24.spi_id, W_TX_PAYLOAD_NO_ACK, data, len, rf24.static_payload_length);
		
	}
	
	config_ce(HIGH);
	/* sending ... */
	/* 250kbps 8 bits + 5 * 8 bits + 9 bits + 32 * 8 bits + 2 * 8 bits  = 329 bits */
	/* 329 / 250kbps*/
	_delay_ms(2);
	config_ce(LOW);
	
	POWER_DOWN;
	return len;
}


uint8_t recv(uint8_t data[], uint8_t len){
	CLEAR_IRQ;
	return spi_read_cmd_buffer(rf24.spi_id,R_RX_PAYLOAD, data, len, rf24.static_payload_length);
}


uint8_t read_buffer(uint8_t reg, uint8_t* buf, uint8_t len){
	return spi_read_cmd_buffer(rf24.spi_id,  R_REGISTER | ( REGISTER_MASK & reg ), buf, len,len);
}


uint8_t write_buffer(uint8_t reg, const uint8_t* buf, uint8_t len){
	return spi_write_cmd_buffer(rf24.spi_id, W_REGISTER | ( REGISTER_MASK & reg ), buf, len,len );
}










