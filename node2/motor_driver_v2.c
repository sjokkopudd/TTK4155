#define F_CPU 16000000

#include "dac_driver.h"
#include "common.h"
#include "motor_driver_v2.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include "timer.h"
#include <stdio.h>

static int16_t MAX = 0;
static int16_t MIN = 0;


void motor_init(){
	// enable motor

	// Enable motor
	set_bit(DDRH, PH4);
	set_bit(PORTH, PH4);
	
	// Set DIR pin to output
	set_bit(DDRH, PH1);
	
	// Set !OE
	set_bit(DDRH, PH5);
	
	// Set SEL 
	set_bit(DDRH, PH3);
	
	// Set !RST

	set_bit(DDRH, PH6);

	// Reset encoder
	motor_reset();
	
	motor_calibrate();


}

// ---------------------------------------
// reset encoder !RST
// ---------------------------------------
void motor_reset(){
	//toggle !RST
	clear_bit(PORTH, PH6);
	_delay_us(200);
	set_bit(PORTH, PH6);

}

// ------------------------------------------
// set direction of motor by writing on DIR pin
// -----------------------------------------
void motor_set_direction(dir_t dir){
	switch (dir){
		case eDIR_RIGHT:
			set_bit(PORTH, PH1);
			break;
		case eDIR_LEFT:
			clear_bit(PORTH, PH1);
			break;
		default: break;
	}
}
// ---------------------------------------
// calibrates encoder -> left and right position 
// ---------------------------------------
void motor_calibrate(){
	motor_set_direction(eDIR_LEFT);
	motor_set_speed(80);
	_delay_ms(1600);
	motor_set_speed(0);

	motor_reset();

	MIN = motor_get_encoder_value();
	
	_delay_us(100);

	// go to the right
	motor_set_direction(eDIR_RIGHT);
	motor_set_speed(80);
	_delay_ms(1600);


	MAX = motor_get_encoder_value();


	_delay_us(100);
	motor_set_speed(0);

	//printf("MIN: %d\r\n", MIN);
	//printf("MAX: %d\r\n", MAX);

	/*int16_t prev_rot = cur_rot+200;
	while(prev_rot != cur_rot) {
		prev_rot = cur_rot;
		_delay_ms(40);
		cur_rot = motor_read_rotation(0);
	}
	motor_reset_encoder();
	motor_set_speed(0);


	//drive the motor to both ends
	uint8_t data;
	// left
	uint8_t counter = 0;
	data = 100;
	set_bit(DDRH, PH1);
	dac_send(data);
	_delay_ms(1600);
	dac_send(0);

	//reset encoder
	clear_bit(PORTH, PH6);
	_delay_ms(20);
	set_bit(PORTH, PH6);

	MIN = get_encoder_value();
	_delay_us(100);

	// right
	counter = 0;
	data = 100;
	clear_bit(DDRH, PH1);
	dac_send(data);
	_delay_ms(1600);

	MAX = get_encoder_value();
	_delay_us(100);
	dac_send(0);


	printf("MAX: %d\r\n", MAX);
	printf("MIN: %d\r\n", MIN);	*/



}

// ----------------------------------------------
// read current motor value
// ----------------------------------------------
int16_t motor_get_encoder_value(){

	//enable output of encoder
	clear_bit(PORTH, PH5);

	//get high bit
	clear_bit(PORTH, PH3);

	//wait a few us
	_delay_us(40);

	//read MSB
	uint8_t high = PINK;

	//get low bit
	set_bit(PORTH, PH3);

	//wait a feq us
	_delay_us(40);

	uint8_t low = PINK;

	//disable output of encoder
	set_bit(PORTH, PH5);

	//combine to 16bit output
	int16_t result = (int16_t)((high << 8) | low);
	//printf("result: %d\r\n", result );
	return result;


}


void motor_set_speed(uint8_t speed){
	printf("in speed, val: %d\r\n", speed);
	dac_send(speed);

}

// ----------------------------------------
// update motor with controlled u
// ----------------------------------------
void update_motor_with_u(int16_t val){
		printf("in update motor\r\n");
	if (val < 0){
		//set direction to left
		motor_set_direction(eDIR_LEFT);
		if (abs(val) > 200){
			val = 200;
		}
		
	}
	else{
		//set direction to right
		motor_set_direction(eDIR_RIGHT);
		if (abs(val) > 200){
			val = 200;
		}
	}
	unsigned char data = abs(val);
	printf("before set speed, value: %d\r\n", data);
	//send speed
	//motor_set_speed(abs(val));
	dac_send(data);
	printf("after set speed\r\n");


}


int16_t get_MAX(){
	return MAX;
}

int16_t get_MIN(){
	return MIN;
}