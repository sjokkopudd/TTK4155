#pragma once
#include <stdint.h>


typedef struct Joy_Analog_Pos
{
	int8_t x;
	int8_t y;
} joy_analog_pos;


typedef enum{
	eLEFT = 0,
	eRIGHT = 1,
	eUP = 2,
	eDOWN = 3,
	eNEUTRAL = 4
} joy_direction_t;

typedef enum{
	eBTN_RIGHT = 0,
	eBTN_LEFT = 1,
	eBTN_NO = 2
} button_t;

typedef struct Slider_Pos
{
	uint8_t x;
	uint8_t y;

}slider_pos;



void joystick_init(void);

joy_direction_t joystick_get_direction();
joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y);

uint8_t joystick_get_x_pos();

uint8_t get_slider_pos_left(void);

uint8_t get_slider_pos_right(void);

button_t get_button(void);

