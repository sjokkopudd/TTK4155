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

static int16_t MAX = 0;
static int16_t MIN = 0;
volatile uint8_t current_position = 0;


void motor_init(){
	//enable motor
	

	//initialize encoder
	DDRK = 0;
	//DDRH = 0b0111100;
	set_bit(DDRH, PH6);
	set_bit(DDRH, PH5);
	set_bit(DDRH, PH4);
	set_bit(DDRH, PH3);

	set_bit(PORTH, PH4);

	clear_bit(PORTH, PH6);
	_delay_us(20);
	set_bit(PORTH, PH6);
	printf("in motor init\n");
}

void update_motor(uint8_t val){



	if (val > (128 + DEAD_ZONE)){
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
	}
}

void update_position(uint8_t pos){
	current_position = pos;
}

int16_t get_encoder_value(){
	//enable output of encoder
	clear_bit(PORTH, PH5);

	//get high bit
	clear_bit(PORTH, PH3);
	_delay_us(40);
	uint8_t high = PINK;

	//get low bit
	set_bit(PORTH, PH3);
	_delay_us(40);
	uint8_t low = PINK;

	//disable output of encoder
	set_bit(PORTH, PH5);

	//combine to 16bit output
	int16_t result = ((int16_t)high << 8) | low;
	printf("result: %d\r\n", result );
	return result;
}

void encoder_reset(){
	//drive the motor to both ends
	uint8_t data;
	// right
	uint8_t counter = 0;
	data = 70;
	set_bit(DDRH, PH1);
	dac_send(data);
	_delay_ms(1000);

	//reset encoder
	clear_bit(PORTH, PH6);
	_delay_ms(20);
	set_bit(PORTH, PH6);

	MIN = get_encoder_value();
	_delay_us(100);

	// left
	counter = 0;
	data = 70;
	clear_bit(DDRH, PH1);
	dac_send(data);
	_delay_ms(1000);

	MAX = get_encoder_value();
	_delay_us(100);
	dac_send(0);


	printf("MAX: %d\r\n", MAX);
	printf("MIN: %d\r\n", MIN);	
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

int16_t get_MAX(){
	return MAX;
}

int16_t get_MIN(){
	return MIN;
}