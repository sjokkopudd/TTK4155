#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <stdint.h>

void motor_init();

void update_motor(uint8_t val);

#endif