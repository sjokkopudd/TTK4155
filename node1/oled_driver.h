#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>


void oled_init(void); 

void oled_reset(void);

void oled_home(void);

void oled_goto_line(uint8_t line);

void oled_goto_column(uint8_t column);

void oled_clear_line(uint8_t line);

void oled_pos(uint8_t row, uint8_t column);

void oled_print(const char* data);

void oled_set_brightness(uint8_t lvl);

void oled_write_screen();

void oled_print_trophy();

void oled_game_over();

void oled_print_firework(int frame);

#endif