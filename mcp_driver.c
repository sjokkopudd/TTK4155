#include "MCP_driver"
#include <stdint.h>
#include "common.h"


void mcp_activateSlave(){
	/* Activate Slave Select */
	clear_bit(PORTB, PB4);
}

void mcp_deactivateSlave(){
	/* Deactivate Slave Select */
	set_bit(PORTB, PB4);
}
