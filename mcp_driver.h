#ifndef MCP_DRIVER_H
#define MCP_DRIVER_H

#include <stdint.h>

uint8_t mcp_init(uint8_t mode);
uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t address, uint8_t data);
void mcp_request_to_send(uint8_t byte);
void mcp_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);
void mcp_reset();
uint8_t mcp_read_status();

#endif