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

typedef enum{
	BTN_RIGHT = 0,
	BTN_LEFT = 1,
	BTN_JOY = 2
} button_t;

typedef struct Slider_Pos
{
	uint8_t x;
	uint8_t y;

}slider_pos;



void joystick_init(void);

joy_direction_t joystick_get_direction(uint8_t x, uint8_t y);
joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y);

uint8_t get_slider_pos_left(void);

uint8_t get_slider_pos_right(void);

uint8_t get_button(button_t button);
