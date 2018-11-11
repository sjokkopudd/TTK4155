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
#define K_p 1
#define K_i 1
#define T 0.02

static uint8_t MAX;
static uint8_t MIN;

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
	while(counter < 10000){
		dac_send(data);
		counter ++;
	}
	MIN = get_encoder_value();
	_delay_us(100);

	// left
	uint8_t counter = 0;
	data = 100;
	set_bit(DDRH, PH1);
	while(counter < 10000){
		dac_send(data);
		counter ++;
	}
	MIN = get_encoder_value();	

	printf("MAX: %d\r\n", MAX);
	printf("MIN: %d\r\n", MIN);
}