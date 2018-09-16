#include "joystick_driver.h"
#include <stdint.h>
#include "adc_driver.h"

#define THRESH_DIR 20

#define INIT_POS 127

#ifndef DEBUG
#define DEBUG
#endif

static uint8_t init_pos_x = 127;
static uint8_t init_pos_y = 127;

static uint8_t max_pos_x = 255;
static uint8_t max_pos_y = 255;

static uint8_t min_pos_x = 0;
static uint8_t min_pos_y = 0;


static uint8_t joystick_fine_calibrate(adc_channel_t channel){
	uint8_t cnt = 0;
	uint8_t lastPos = adc_read_channel(channel);
	while(cnt < 3){
		uint8_t currPos = adc_read_channel(channel);
		if(currPos == lastPos){
			cnt++;
		}
		else{
			cnt = 0;
		}
		lastPos = currPos;
	}
	return lastPos;
}

void joystick_calibrate(void){
	/* each direction (left and right) of the joystick has to be calibrated
	   by software. */
	adc_channel_t channel_x = JOYSTICK_X;
	adc_channel_t channel_y = JOYSTICK_Y;

	//printf("joystick calibration %s\n", );
	//read each position until three same values are read in
	#ifdef DEBUG
		printf("init calibration of x and y- left\r\n");
	#endif

	init_pos_x = joystick_fine_calibrate(channel_x);
	init_pos_y = joystick_fine_calibrate(channel_y);

	/*#ifdef DEBUG
		printf("calibration of x - left\r\n");
	#endif

	delay_us(1000);
	min_pos_x = joystick_fine_calibrate(channel_x);
	

	#ifdef DEBUG
		printf("calibration of y - down\r\n");
	#endif

	delay_us(1000);
	min_pos_y = joystick_fine_calibrate(channel_y);

	#ifdef DEBUG
		printf("calibration of x - right\r\n");
	#endif

	delay_us(1000);
	max_pos_x = joystick_fine_calibrate(channel_x);
	

	#ifdef DEBUG
		printf("calibration of y - up\r\n");
	#endif

	delay_us(1000);
	max_pos_y = joystick_fine_calibrate(channel_y);*/


}



joy_direction_t joystick_get_direction(uint8_t x, uint8_t y){
	joy_direction_t direction;

	if(x <= init_pos_x - THRESH_DIR){
		direction = LEFT;
	}
	else if(x >= init_pos_x + THRESH_DIR){
		direction = RIGHT;
	}
	else if(y <= init_pos_y - THRESH_DIR){
		direction = DOWN;
	}
	else if(y >= init_pos_y + THRESH_DIR){
		direction = UP;
	}
	else{
		direction = NEUTRAL;
	}

	return direction;

}

joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y){
	joy_analog_pos position;
	int8_t x_percentage = ((int8_t)x - init_pos_x)*200/(max_pos_x-min_pos_x);
	int8_t y_percentage = ((int8_t)y - init_pos_y)*200/(max_pos_y-min_pos_y);

	position.x = x_percentage;
	position.y = y_percentage;

	return position;

}