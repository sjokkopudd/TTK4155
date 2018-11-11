#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <stdint.h>

void motor_init();

void encoder_reset();

void update_motor(uint8_t val);

void update_position(uint8_t pos);

uint16_t get_encoder_value();

//converts the 8bit slider position value to the corrosponding 16bit encoder value
uint16_t convert_to_16bit(uint8_t slider_pos);

uint16_t get_current_position();

#endif