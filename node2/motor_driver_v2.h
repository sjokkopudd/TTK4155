#ifndef MOTOR_DRIVER_V2_H
#define MOTOR_DRIVER_V2_H

#include <stdint.h>

typedef enum dir_t
{
	eDIR_LEFT = 0,
	eDIR_RIGHT = 1
}dir_t;



void motor_init(void);
void motor_reset();
void motor_set_direction(dir_t dir);
void motor_calibrate();
void motor_set_speed(uint8_t speed);
int16_t motor_get_encoder_value(void);
void update_motor_with_u(int16_t val);
int16_t get_MAX(void);
int16_t get_MIN(void);

#endif