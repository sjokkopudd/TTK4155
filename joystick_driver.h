#pragma once
#include <stdint.h>


typedef struct Joy_Analog_Pos
{
	int8_t x;
	int8_t y;
} joy_analog_pos;


typedef enum{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NEUTRAL = 4
} joy_direction_t;

void joystick_calibrate(void);

joy_direction_t joystick_get_direction(uint8_t x, uint8_t y);
joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y);