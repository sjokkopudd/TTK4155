#include "timer.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "motor_driver.h"

#define K_p 1
#define K_i 0.005
#define K_d 0.01
#define T 0.02

volatile int16_t sampled_encoder_value = 0;
//volatile uint8_t time = 0;
volatile float integration_value = 0;

volatile int16_t saved_error = 0;

//initialize 16bit timer/counter3 to count at increments of 20ms, and enable flag
void timer_init(){

	printf("in init\r\n");
	//disable global interrupts
	cli();

	//enable overflow interrupt
	set_bit(TIMSK3, TOIE3);

	//set timer to PWM fast mode
	clear_bit(TCCR3A, WGM30);
	set_bit(TCCR3A, WGM31);
	set_bit(TCCR3B, WGM32);
	set_bit(TCCR3B, WGM33);

	//set output mode non-inverting
	clear_bit(TCCR3A, COM3A0);
	set_bit(TCCR3A, COM3A1);

	//set clk prescalar fcpu/1024
	set_bit(TCCR3B, CS30);
	clear_bit(TCCR3B, CS31);
	set_bit(TCCR3B, CS32);

	//set the counter to enable flag every 20ms
	ICR3 = 1250;

	//enable global interrupts
	sei();

}

ISR(TIMER3_OVF_vect){
	sampled_encoder_value = get_encoder_value();
	//printf("sampled_encoder_value: %u\r\n", sampled_encoder_value);
	discrete_PI_controller();
	//time += 20;
}

/*uint8_t get_time(){
	return time;
}*/

void discrete_PI_controller(){
	uint8_t target_value = get_current_position();
	printf("target value %d\r\n", target_value);
	int16_t error = target_value - convert_encoder_to_8bit(sampled_encoder_value);
	int16_t last_error = saved_error;
	saved_error = error;
	printf("encoder value: %d\n\r", convert_encoder_to_8bit(sampled_encoder_value));
	printf("error: %d\r\n", error);
	integration_value += error;

	int32_t output = K_p*error + T*K_i*integration_value + (K_d/T)*(error-last_error);
	update_motor_with_u(output);
	//printf("u: %d\r\n", output);
}


uint16_t convert_encoder_to_8bit(uint16_t value){
	uint16_t normalized_value = value - get_MIN();
	//printf("normalized_value: %u\r\n", normalized_value);
	uint16_t interval = get_MAX()-get_MIN();
	//printf("interval: %u\r\n", interval);
	double increment = 255./interval;
	//printf("increment: %d\r\n", increment);
	uint16_t result = normalized_value*increment;
	//printf("increment: %f\r\n", increment);
	//printf("result: %d\r\n", result);
	if(result > 300){
		result = 300;
	}
//	uint8_t result1 = (uint8_t)((normalized_value*255)./interval);
	//printf("result1: %d\r\n", result1);

	return result; 
}
