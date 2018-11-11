#include "timer.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "motor_driver.h"

#define K_p 1
#define K_i 1
#define T 0.02

volatile uint16_t sampled_encoder_value = 0;
//volatile uint8_t time = 0;
volatile uint8_t integration_value = 0;

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
	discrete_PI_controller();
	//time += 20;
}

/*uint8_t get_time(){
	return time;
}*/

void discrete_PI_controller(){
	uint8_t target_value = get_current_position();
	uint8_t error = target_value - convert_encoder_to_8bit(sampled_encoder_value);
	integration_value += error;

	uint16_t output = K_p*error + T*K_i*integration_value;

	printf("u: %u\r\n", output);
}


uint8_t convert_encoder_to_8bit(uint16_t value){
	uint16_t normalized_value = value - MIN;
	uint16_t interval = MAX-MIN;
	int increment = 255/interval;
	uint8_t result = normalized_value*increment;
	return result;  
}
