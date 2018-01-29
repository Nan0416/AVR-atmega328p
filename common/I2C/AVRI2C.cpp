#include "AVRI2C.hpp"

static TWIConfig twConfig;
  

static void twi_reply(uint8_t);
static void twi_stop(void);
static void twi_releaseBus(void);


/**
 * MT 
 * if no ack for sla+w, send stop condition, tw_error = TW_MT_SLA_NACK
 * if no ack for data, send stop condition, tw_error = TW_MT_DATA_NACK
 * if arbitration lost, release the bus, and go ready state
 * 
 * MR
 * master receiver determine to receive how many bytes (no slave gives but master ask)
 * to stop transmission, master won't ack the last byte.
 * 
 * SR
 * slave may not ack the master's data byte due to buffer is full. The last byte that is not ack is discarded.
 * after a data received not ack, the slave continue no ack, and master would stop, then the slave release bus
 * slave can be interrupt by the stop or repeated-start action sent by the master.
 * It also invoke the user-defined callback function after receiving master stop/repeated start. 
 * 				twi_onSlaveReceive(twi_rxBuffer, twi_rxBufferIndex);
 * 
 * ST
 * slave transmits data to master according to master's request.
 * Two stop condition: no data ack from master means master want to stop the transmission. 
 * 					no more data from slave, then slave set the twea in the last byte, master will always receives 0.
 * ST cannot know stop condition.
 * */



/* 
 * Function twi_stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
static void twi_stop(void)
{
  // send stop condition
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

  // The stop may not immediate sent due to bus busy
  // but the TWSTO will be cleared after the action is taken
  // TWINT is not set after a stop condition!
  while(TWCR & _BV(TWSTO)){
    continue;
  }

  // update twi state
  //twi_state = TWI_READY;
  twConfig.twi_state = TWI_READY;
}
/* 
 * Function twi_reply
 * Desc     sends byte or readys receive line
 * Input    ack: byte indicating to ack or to nack
 * Output   none
 */
static void twi_reply(uint8_t ack)
{
  // transmit master read ready signal, with or without ack
  if(ack){
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  }else{
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}



/* 
 * Function twi_releaseBus// mainly for set TWINT so that let the SCL go
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
static void twi_releaseBus(void)
{
  // release bus
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);

  // update twi state
  //twi_state = TWI_READY;
  twConfig.twi_state = TWI_READY;
}

ISR(TWI_vect){
	switch(TW_STATUS){
		/* For master: start condition sent; send SLA +R/W and disable start/stop in TWCR, enable TWEA*/
		case TW_START:     // sent start condition
		case TW_REP_START: // sent repeated start condition
			TWDR = twConfig.twi_slarw;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		/* For master transmit: sla+w or data is sent and acked; send more data from the driver buffer*/
		case TW_MT_SLA_ACK:  // slave receiver acked address
		case TW_MT_DATA_ACK: // slave receiver acked data
			// if there is data to send, send it, otherwise stop 
			if(twConfig.twi_masterBufferIndex < twConfig.twi_masterBufferLength){
				TWDR = twConfig.twi_masterBuffer[twConfig.twi_masterBufferIndex++];
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT); // | _BV(TWEA);
			}else{
				if(twConfig.twi_sendStop == 1){
					twi_stop();
				}else {
					twConfig.twi_inRepStart = true;  // we're gonna send the START
					
					// don't enable the interrupt. We'll generate the start, but we 
					// avoid handling the interrupt until we're in the next transaction,
					// at the point where we would normally issue the start.
					TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN);
					twConfig.twi_state = TWI_READY;
				}
			}
			break;
		case TW_MT_SLA_NACK:  // address sent, nack received
			twConfig.twi_error = TW_MT_SLA_NACK;
			twi_stop();
			break;
		case TW_MT_DATA_NACK: // data sent, nack received
			twConfig.twi_error = TW_MT_DATA_NACK;
			twi_stop();
			break;
		case TW_MT_ARB_LOST: // lost bus arbitration
			// It also handle TW_MR_ARB_LOST, same status code
			twConfig.twi_error = TW_MT_ARB_LOST;
			twi_releaseBus();
			break;

		/* For master receiver */
		
		case TW_MR_SLA_NACK: // address sent, nack received
			twi_stop();
			break;
		case TW_MR_DATA_ACK: // data received, ack sent
			// put byte into buffer
			twConfig.twi_masterBuffer[twConfig.twi_masterBufferIndex++] = TWDR;
		case TW_MR_SLA_ACK:  // address sent, ack received
			// Master determine how many bytes need to be transmit.
			// to stop the transmission, master won't ack the last byte.
			if(twConfig.twi_masterBufferIndex < twConfig.twi_masterBufferLength){
				 TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA); // ack 
			}else{
				 TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT); // don't ack the last byte
			}
			break;
		case TW_MR_DATA_NACK: // the final byte
				twConfig.twi_masterBuffer[twConfig.twi_masterBufferIndex++] = TWDR;
			if (twConfig.twi_sendStop){
				twi_stop();
			}else {
				twConfig.twi_inRepStart = true;  
				TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
				twConfig.twi_state = TWI_READY;
			}    
			break;
   
    

		/* Slave Receiver */
		case TW_SR_SLA_ACK:   // addressed, returned ack
		case TW_SR_GCALL_ACK: // addressed generally, returned ack
		case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
		case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
			// enter slave receiver mode
			twConfig.twi_state = TWI_SRX;
			// indicate that rx buffer can be overwritten and ack
			twConfig.twi_rxBufferIndex = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA); // ack 
			break;
		case TW_SR_DATA_ACK:       // data received, returned ack
		case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
			// if there is still room in the rx buffer
			if(twConfig.twi_rxBufferIndex < TWI_BUFFER_LENGTH){
			// put byte in buffer and ack
				twConfig.twi_rxBuffer[twConfig.twi_rxBufferIndex++] = TWDR;
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA); // ack 
			}else{
				// otherwise nack
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			}
			break;
		case TW_SR_STOP: // stop or repeated start condition received
			// ack future responses and leave slave receiver state
			twi_releaseBus();
			// put a null char after data if there's room
			if(twConfig.twi_rxBufferIndex < TWI_BUFFER_LENGTH){
				twConfig.twi_rxBuffer[twConfig.twi_rxBufferIndex] = '\0';
			}
			// callback to user defined callback
			twConfig.twi_onSlaveReceive(twConfig.twi_rxBuffer, twConfig.twi_rxBufferIndex);
			// since we submit rx buffer to "wire" library, we can reset it
			twConfig.twi_rxBufferIndex = 0;
			break;
		case TW_SR_DATA_NACK:       // data received, returned nack
		case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
			// nack back at master
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			break;
    
		/* Slave Transmitter */
		case TW_ST_SLA_ACK:          // addressed, returned ack
		case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
			// enter slave transmitter mode
			twConfig.twi_state = TWI_STX;
			// ready the tx buffer index for iteration
			twConfig.twi_txBufferIndex = 0;
			// set tx buffer length to be zero, to verify if user changes it
			twConfig.twi_txBufferLength = 0;
			// request for txBuffer to be filled and length to be set
			// note: user must call twi_transmit(bytes, length) to do this
			twConfig.twi_onSlaveTransmit();
			// if they didn't change buffer & length, initialize it
			if(0 == twConfig.twi_txBufferLength){
				twConfig.twi_txBufferLength = 1;
				twConfig.twi_txBuffer[0] = 0x00;
			}
			// no break;
			// transmit first byte from buffer, fall
		case TW_ST_DATA_ACK: // byte sent, ack returned
			// copy data to output register
			TWDR = twConfig.twi_txBuffer[twConfig.twi_txBufferIndex++];
			// if there is more to send, ack, otherwise nack
			if(twConfig.twi_txBufferIndex < twConfig.twi_txBufferLength){
				twi_reply(1);
			}else{
				twi_reply(0);
			}
			break;
		case TW_ST_DATA_NACK: // received nack, we are done 
		case TW_ST_LAST_DATA: // received ack, but we are done already!
			// ack future responses
			twi_reply(1);
			// leave slave receiver state
			twConfig.twi_state = TWI_READY;
			break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      twConfig.twi_error = TW_BUS_ERROR;
      twi_stop();
      break;
  }
}




void TWI::twi_init(void){
	//initialize pins
	sei();
	DDRC &= ~(_BV(5) | _BV(6));
	PORTC |= (_BV(5) | _BV(6));
	//prescale is set to 0 to ensure a higher speed
	TWSR &= 0xfc;
	// enable twi module, acks (for slave mode), and twi interrupt
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA); 
	twConfig.twi_state = TWI_READY;
	twConfig.twi_sendStop = true;
	twConfig.twi_inRepStart = false;
	
	
	twConfig.twi_masterBufferIndex = 0;
	twConfig.twi_masterBufferLength = 0;

	twConfig.twi_txBufferIndex = 0;
	twConfig.twi_txBufferLength = 0;

	twConfig.twi_rxBufferIndex = 0;
	twConfig.twi_error = 0xff;
}
/* 
 * Function twi_disable
 * Desc     disables twi pins
 * Input    none
 * Output   none
 */
void TWI::twi_disable(void)
{
  // disable twi module, acks, and twi interrupt
  TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

  // deactivate internal pullups for twi.
  
}


void TWIMaster::twi_setFrequency(uint8_t frequency){
		//SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
	TWBR = frequency;
}
/* 
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t TWIMaster::twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
	uint8_t i;
	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 1;
	}
	// twi_state can be modified by interrupt handler, 
	// wait until twi is ready, become master transmitter
	while(TWI_READY != twConfig.twi_state){
		continue;
	}
	// set status as master transmit mode: mtx includes phase of transmit sla + w and data.
	twConfig.twi_state = TWI_MTX;
	twConfig.twi_sendStop = sendStop;
	// reset error state (0xFF.. no error occured)
	twConfig.twi_error = 0xFF;
	
	
	// initialize buffer iteration vars
	twConfig.twi_masterBufferIndex = 0;
	twConfig.twi_masterBufferLength = length;
	// copy data to twi buffer
	for(i = 0; i < length; ++i){
		twConfig.twi_masterBuffer[i] = data[i];
	}
  
	// build sla+w, slave device address + w bit
	twConfig.twi_slarw = TW_WRITE;
	twConfig.twi_slarw |= address << 1;
	// if we're in a repeated start, then we've already sent the START
	// in the ISR. Don't do it again.
	
	if (true == twConfig.twi_inRepStart) {
		// The repeated start condition is sent during last TW_MT_DATA_ACK handler.
		// We need enable interrupt to receive the blocked interrupt and send the new sla+w/r
		twConfig.twi_inRepStart = false;
		do {
			TWDR = twConfig.twi_slarw;       
		} while(TWCR & _BV(TWWC));
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);  // enable INTs, but not START, start is sent
	}else{
		//send start condition, which will trigger the interrupt.
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA); // enable INTs
	}
	// wait for write operation to complete. During this waiting, (re-)start condition, sla+w/r, data is transmitted.
	while(wait && (TWI_MTX == twConfig.twi_state)){
		continue;
	}
  
	if(twConfig.twi_error == 0xFF){
		return 0; // success
	}else if(twi_error == TW_MT_SLA_NACK){
		return 2; // error: address send, nack received
	}else if(twi_error == TW_MT_DATA_NACK){
		return 3; // error: data send, nack received
	}else{
		return 4; // other twi error, e.g.lost arbitration
	}
}

/* 
 * Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: Boolean indicating whether to send a stop at the end
 * Output   number of bytes read
 */
uint8_t TWIMaster::twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
	uint8_t i;

	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 0;
	}

	// wait until twi is ready, become master receiver
	while(TWI_READY != twConfig.twi_state){
		continue;
	}
	twConfig.twi_state = TWI_MRX;
	twConfig.twi_sendStop = sendStop;
	// reset error state (0xFF.. no error occured)
	twConfig.twi_error = 0xFF;

	// initialize buffer iteration vars
	twConfig.twi_masterBufferIndex = 0;
	twConfig.twi_masterBufferLength = length-1;  // This is not intuitive, read on...
	//the -1 is used to stop transmission. For the last byte, interrupt routine would not ack it.
	//this will stop slave transmitter to transmit.

	// build sla+w, slave device address + w bit
	twConfig.twi_slarw = TW_READ;
	twConfig.twi_slarw |= address << 1;

	if (true == twConfig.twi_inRepStart) {
   
		twConfig.twi_inRepStart = false;     // remember, we're dealing with an ASYNC ISR
		do {
			TWDR = twConfig.twi_slarw;
		} while(TWCR & _BV(TWWC));
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);  // enable INTs, but not START
	}else{
		// send start condition
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
	}
	 // wait for read operation to complete
	//PORTB &= ~0x20;
	while(TWI_MRX == twConfig.twi_state){
		continue;
	}
	//PORTB |= 0x20;
	if (twConfig.twi_masterBufferIndex < length){
		length = twConfig.twi_masterBufferIndex;
	}
	for(i = 0; i < length; ++i){
		data[i] = twConfig.twi_masterBuffer[i];
	}
	return length;
}






uint8_t TWISlave::twi_setAddress(uint8_t address)
{
  // set twi slave address (skip over TWGCE bit)
  TWAR = address << 1;
  return address << 1;
}


/* 
 * Function twi_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
uint8_t TWISlave::twi_transmit(const uint8_t* data, uint8_t length)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < (twConfig.twi_txBufferLength+length)){
    return 1;
  }
  
  // ensure we are currently a slave transmitter
  if(TWI_STX != twConfig.twi_state){
    return 2;
  }
  
  // set length and copy data into tx buffer
  for(i = 0; i < length; ++i){
    twConfig.twi_txBuffer[twConfig.twi_txBufferLength+i] = data[i];
  }
  twConfig.twi_txBufferLength += length;
  
  return 0;
}

/* 
 * Function twi_attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
void TWISlave::twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twConfig.twi_onSlaveReceive = function;
}

/* 
 * Function twi_attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
 
void TWISlave::twi_attachSlaveTxEvent( void (*function)(void) )
{
  twConfig.twi_onSlaveTransmit = function;
}









