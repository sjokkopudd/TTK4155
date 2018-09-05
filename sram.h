#pragma once

#include <stdint.h>

void sram_test(void);
int sram_write(uint16_t address, char data);
char sram_read(uint16_t address);