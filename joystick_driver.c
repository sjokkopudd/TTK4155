#include "joystick_driver.h"
#include <stdint.h>

#define X_THRESH_LEFT 107
#define X_THRESH_RIGHT 147
#define Y_THRESH_UP 147
#define Y_THRESH_DOWN 107

#define NEUTRAL_VAL 127

joy_direction_t joystick_get_direction(uint8_t x, uint8_t y){
	joy_direction_t direction;

	if(x <= X_THRESH_LEFT){
		direction = LEFT;
	}
	else if(x >= X_THRESH_RIGHT){
		direction = RIGHT;
	}
	else if(y <= Y_THRESH_DOWN){
		direction = DOWN;
	}
	else if(y >= Y_THRESH_UP){
		direction = UP;
	}
	else{
		direction = NEUTRAL;
	}



}

joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y){
	joy_analog_pos position;
	int8_t x_percentage = ((int8_t)x - NEUTRAL_VAL)*100/NEUTRAL_VAL;
	int8_t y_percentage = ((int8_t)y - NEUTRAL_VAL)*100/NEUTRAL_VAL;
	position.x = x_percentage;
	position.y = y_percentage;

}