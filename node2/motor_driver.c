#define F_CPU 16000000

#include "dac_driver.h"
#include "common.h"
#include "motor_driver.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define DEAD_ZONE 40


void motor_init(){
	//enable motor
	set_bit(DDRH, PH4);
}

void update_motor(uint8_t val){
	if (val > (128 + DEAD_ZONE)){

		//set direction
		clear_bit(DDRH, PH1);
		//distribute voltage span across the input range 
		uint8_t voltage = (val-(127 + DEAD_ZONE))*(double)(255/(160));
		unsigned char data = voltage;
		printf("voltage: %d\r\n", voltage);
		dac_send(data);
	}

	else if (val < (128 - DEAD_ZONE)){

		set_bit(DDRH, PH1);
		uint8_t voltage = ((127 - DEAD_ZONE)-val)*(double)(255/(160));
		unsigned char data = voltage;
		printf("voltage: %d\r\n", voltage);
		dac_send(data);
	}

	else{
		printf("under voltage\r\n");
		dac_send(0b0);
	}
}

uint16_t get_encoder_value(){
	//enable output of encoder
	clear_bit(DDRH, PH5);

	//get high bit
	clear_bit(DDRH, PH3);
	uint8_t high = PORTC;


	//get low bit
	set_bit(DDRH, PH3);
	uint8_t low = PORTC;

	//disable output of encoder
	set_bit(DDRH, PH5);

	//combine to 16bit output
	uint16_t result = ((uint16_t)high << 8) | low;
	return result;
}