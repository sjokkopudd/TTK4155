#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <stdint.h>

void motor_init();

void encoder_reset();

void update_motor(uint8_t val);

uint16_t get_encoder_value();

#endif