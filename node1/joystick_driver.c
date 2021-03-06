#include "joystick_driver.h"
#include <stdint.h>
#include "adc_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "common.h"
#include "pwm_buzzer.h"

#define THRESH_DIR 50

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




// init function - with calibration
void joystick_init(void){
	
	//reset pins 0 and 1 at Port B to be input for button left and button right
	DDRB &= ~(1 << DDB0);
	DDRB &= ~(1 << DDB1);
	
	//initial read to calibrate joystick initial position
	init_pos_x = adc_read_channel(JOYSTICK_X);
	init_pos_y = adc_read_channel(JOYSTICK_Y);

}

// returns joystick direction: left, right, up, down or neutral
joy_direction_t joystick_get_direction(void){

	adc_channel_t CHANNEL;
	CHANNEL = JOYSTICK_X;
		
	uint8_t x = adc_read_channel(CHANNEL);
	//delay_ms(100); ->do we need it???
	CHANNEL = JOYSTICK_Y;
	uint8_t y = adc_read_channel(CHANNEL);

	joy_direction_t direction;
	joy_analog_pos joystick_position;

	joystick_position = joystick_get_analog_pos(x, y);

	// if x value is under threshold for direction left and x input is larger than y input
	if((x <= init_pos_x - THRESH_DIR) && (abs(joystick_position.x)>abs(joystick_position.y))){
		direction = eLEFT;
	}
	// if x value is over threshold for direction right and x input is larger than y input
	else if(x >= init_pos_x + THRESH_DIR && (abs(joystick_position.x)>abs(joystick_position.y))){
		direction = eRIGHT;
	}
	// if y value is under threshold for direction right and y input is larger than x input
	else if(y <= init_pos_y - THRESH_DIR && (abs(joystick_position.y)>abs(joystick_position.x))){
		direction = eDOWN;
	}
	// if y value is over threshold for direction up and y input is larger than x input
	else if(y >= init_pos_y + THRESH_DIR && (abs(joystick_position.y)>abs(joystick_position.x))){
		direction = eUP;
	}
	else{
		direction = eNEUTRAL;
	}

	return direction;

}

// returns position x and y of joystick in percentage
joy_analog_pos joystick_get_analog_pos(uint8_t x, uint8_t y){
	joy_analog_pos position;
	int8_t x_percentage = (x - init_pos_x +((init_pos_x-127)/2))*200/(max_pos_x-min_pos_x);
	int8_t y_percentage = (y - init_pos_y+ ((init_pos_y-127)/2))*200/(max_pos_y-min_pos_y);

	position.x = x_percentage;
	position.y = y_percentage;

	return position;

}


uint8_t joystick_get_x_pos(){
	adc_channel_t CHANNEL;
	CHANNEL = JOYSTICK_X;
		
	uint8_t x = adc_read_channel(CHANNEL);

	return x;
}

//returns left slider position
uint8_t get_slider_pos_left(void){


	return adc_read_channel(SLIDER_LEFT);

}

//returns right slider position
uint8_t get_slider_pos_right(void){
	return adc_read_channel(SLIDER_RIGHT);

}


button_t get_button(void){
	if(test_bit(PINB, eBTN_RIGHT)){
		return eBTN_RIGHT;
	}
	if(test_bit(PINB, eBTN_LEFT)){
		return eBTN_LEFT;
	}

	return eBTN_NO;
}

