#define F_CPU 4915200

#include "mcp_driver.h"
#include <stdint.h>
#include "common.h"
#include <avr/io.h>
#include "SPI_driver.h"
#include "MCP2515.h"
#include <stdio.h>
#include "util/delay.h"
// -------------------------------------------
// activate slave by setting ss bit low
// -------------------------------------------
static void mcp_activate_slave(){
	/* Activate Slave Select */
	clear_bit(PORTB, PB4);
}

// -------------------------------------------
// deactivate slave by setting ss bit high
// -------------------------------------------
static void mcp_deactivate_slave(){
	/* Deactivate Slave Select */
	set_bit(PORTB, PB4);
}


uint8_t mcp_read(uint8_t address){

	uint8_t result;

	//select can controller
	mcp_activate_slave();

	//send read instruction
	SPI_write(MCP_READ); 
	//send address
	SPI_write(address);

	//read result
	result = SPI_read();

	mcp_deactivate_slave();

	return result;

}

uint8_t mcp_init(uint8_t mode){
	uint8_t value;

	//initialize SPI
	SPI_init();

	//send reset command
	mcp_reset();

	/*//self test -> after reset, the can controller
	//has to be in configuration mode
	value = mcp_read(MCP_CANSTAT);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is NOT in configuration mode after reset\n");
		return 1;
	}*/

	//change from configuration mode to specified mode
	mcp_write(MCP_CANCTRL, mode);

	//check if the can controller is in correct mode
	value  = mcp_read(MCP_CANSTAT);
	if((value & MODE_MASK) != mode){
		printf("MCP2515 is NOT in correct mode after reset\n");
		return 1;
	}


	return 0;


}


// ------------------------------------------------------------
// 
// -----------------------------------------------------------
void mcp_write(uint8_t address, uint8_t data){
	mcp_activate_slave();

	SPI_write(MCP_WRITE);

	SPI_write(address);

	SPI_write(data);
	
	mcp_deactivate_slave();

}


// -------------------------------------------------------------
// 
// -------------------------------------------------------------
void mcp_request_to_send(uint8_t byte){
	mcp_activate_slave();
	
	SPI_write(byte);
	
	mcp_deactivate_slave();

}

// -------------------------------------------------------------
// modify individual bits in specific status and control register
// -------------------------------------------------------------
void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data){

	mcp_activate_slave();
	
	SPI_write(MCP_BITMOD);

	SPI_write(address);

	SPI_write(mask);

	SPI_write(data);
	
	mcp_deactivate_slave();

}

// ------------------------------------------------------------
// resets can controller 
// ------------------------------------------------------------
void mcp_reset(){
	mcp_activate_slave();

	SPI_write(MCP_RESET);

	mcp_deactivate_slave();

	_delay_ms(10);

}

// -----------------------------------------------------------
// reads current status of can controller
// -----------------------------------------------------------
uint8_t mcp_read_status(){
	mcp_activate_slave();
	
	SPI_write(MCP_READ_STATUS);

	uint8_t status = SPI_read();
	
	mcp_deactivate_slave();
	
	return status;

}




