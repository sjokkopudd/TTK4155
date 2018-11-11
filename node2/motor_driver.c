#define F_CPU 16000000

#include "dac_driver.h"
#include "common.h"
#include "motor_driver.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include "timer.h"

#define DEAD_ZONE 40

static uint8_t MAX;
static uint8_t MIN;
volatile uint16_t current_position;

void motor_init(){
	//enable motor
	set_bit(PORTH, PH4);

	//initialize encoder
	DDRK = 0;
	DDRH = 0b0111100;
	clear_bit(PORTH, PH6);
	_delay_us(20);
	set_bit(PORTH, PH6);
}

void update_motor(uint8_t val){

	/*if (val > (128 + DEAD_ZONE)){

		//set direction
		clear_bit(DDRH, PH1);
		//distribute voltage span across the input range 
		uint8_t voltage = (val-(127 + DEAD_ZONE))*(double)(255/(160));
		unsigned char data = voltage;
		//printf("voltage: %d\r\n", voltage);
		dac_send(data);
	}

	else if (val < (128 - DEAD_ZONE)){

		set_bit(DDRH, PH1);
		uint8_t voltage = ((127 - DEAD_ZONE)-val)*(double)(255/(160));
		unsigned char data = voltage;
		//printf("voltage: %d\r\n", voltage);
		dac_send(data);
	}

	else{
		//printf("under voltage\r\n");
		dac_send(0b0);
	}*/
}

void update_position(uint8_t pos){
	current_position = convert_to_16bit(pos);
}

uint16_t get_encoder_value(){
	//enable output of encoder
	clear_bit(PORTH, PH5);

	//get high bit
	clear_bit(PORTH, PH3);
	_delay_us(20);
	uint8_t high = PINK;

	//get low bit
	set_bit(PORTH, PH3);
	_delay_us(20);
	uint8_t low = PINK;

	//disable output of encoder
	set_bit(PORTH, PH5);

	//combine to 16bit output
	uint16_t result = ((uint16_t)high << 8) | low;
	return result;
}

void encoder_reset(){
	//drive the motor to both ends
	uint8_t data;
	// right
	uint8_t counter = 0;
	data = 100;
	clear_bit(DDRH, PH1);
	while(counter < 50){
		dac_send(data);
		counter ++;
	}
	MIN = get_encoder_value();
	_delay_us(100);

	// left
	counter = 0;
	data = 100;
	set_bit(DDRH, PH1);
	while(counter < 50){
		dac_send(data);
		counter ++;
	}
	MIN = get_encoder_value();

	printf("MAX: %d\r\n", MAX);
	printf("MIN: %d\r\n", MIN);

	//make sure that we don't get overflow error
	if (MAX < MIN){
		//drive the motor a little while letting the thread glide over the driveshaft 
		//in order to push the encoder to new values not in overflow range.
		while(counter < 15){
		dac_send(200);
		counter ++;
		}
		//try again
		encoder_reset();
	}	
}

uint16_t convert_to_16bit(uint8_t slider_pos){
	uint16_t interval = MAX-MIN;
	uint16_t increment = interval/255;
	uint16_t result = MIN + slider_pos*increment;
	return result;  
}

uint16_t get_current_position(){
	return current_position;
}