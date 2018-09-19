#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>


void oled_init(void); // PDF:“OLED LY190-128064” section 9.4

void oled_reset(void);

void oled_home(void);

void oled_goto_line(uint8_t line);

void OLED_clear_line(uint8_t line);

void oled_pos(uint8_t row, uint8_t column);

void oled_write_data(char data);

void oled_print(char* data);

void oled_set_brightness(uint8_t lvl);

#endif