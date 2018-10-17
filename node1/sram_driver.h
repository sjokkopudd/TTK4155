#pragma once

#include <stdint.h>


void sram_test(void);
int sram_write(uint16_t address, uint8_t data);
char sram_read(uint16_t address);