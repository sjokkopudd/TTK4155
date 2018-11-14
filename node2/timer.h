#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

void timer_init();
uint8_t get_time();

void discrete_PI_controller();
uint16_t convert_encoder_to_8bit(uint16_t value);

#endif