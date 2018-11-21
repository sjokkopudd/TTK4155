#include "can_communication.h"
#include "common.h"
#include "MCP2515.h"
#include <stdlib.h>
#include "mcp_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_driver.h"
#include <stdio.h>

static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

#define CAN_MAX_MESSAGE_LEN 8

volatile uint8_t rx_flag = 0;

ISR(INT2_vect){
	rx_flag = 1;
}

void can_init(){

	mcp_init(MODE_NORMAL);

	//disable global interrupts
	cli();

	//interrupt on falling edge
	set_bit(EICRA, ISC21);
	clear_bit(EICRA, ISC20);

	//enable interrupt on pd2
	set_bit(EIMSK, INT2);

	//enable global interrupts
	sei();


	//clear tx0 interrupt 
	mcp_bit_modify(MCP_CANINTE, MCP_TX0IF, 0);


	//disable rollover, accept all transmissions
	mcp_bit_modify(MCP_RXB0CTRL, 0b01100100, 0b01100000);

	//set rx0 interrupt
	mcp_bit_modify(MCP_CANINTE, MCP_RX0IF, 1);

}


// -----------------------------------------------------------------
// check if last transmission has completed - TXB0CTRL.TXREQ is clear 
// -----------------------------------------------------------------
static uint8_t can_transmission_complete(){

	
	//check third bit TXREQ of MCP_TXB0CTRL register 
	return !test_bit(mcp_read(MCP_TXB0CTRL), MCP_TXREQ);
}

uint8_t can_send_message(data_t* message){
	
	stdout = &uart_stream;

	//check on transmission complete before sending new message
	if(can_transmission_complete){

		//Set the higher message id 
		mcp_write(MCP_TXB0SIDH , (uint8_t) (message->id >> 3));

		//set lower message id and standard data frame (RTR = 0)
		mcp_bit_modify(MCP_TXB0SIDL, 0b11101000, (uint8_t)(message->id << 5));
		
		//Set data length 
		mcp_bit_modify(MCP_TXB0DLC, 0b00001111, message->length);

		//Set data bytes (max. 8 bytes)
		for (int i = 0; i < message->length; i++) {
			mcp_write( MCP_TXB0D0 + i, message->data[i]);
		}
		
		//Request to send via TX0
		mcp_request_to_send(MCP_RTS_TX0);

		return 0;
	}

	return 1;


}



int can_receive_message(data_t* message_to_receive){

	
	//check interrupt flag
	if(rx_flag){
		
		//get message id 
		message_to_receive->id = (mcp_read(MCP_RXB0SIDH) << 3 ) | (mcp_read(MCP_RXB0SIDL)>> 5);

		//get message length
		message_to_receive->length = (0x0F) & mcp_read(MCP_RXB0DLC);
		

		for (int i = 0; i < message_to_receive->length; ++i)
		{
			message_to_receive->data[i] = mcp_read(MCP_RXB0D0 + i);

		}

		rx_flag = 0;

		//clear interrupt flag to open register buffer for new receive
		mcp_bit_modify(MCP_CANINTF, 0x01, 0x00);

		return 0;

	}
	return 1;


}

