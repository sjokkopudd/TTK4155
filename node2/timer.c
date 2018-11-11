#include "timer.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "motor_driver.h"

volatile uint8_t encoder_value = 0;
volatile uint8_t time = 0;

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
<<<<<<< HEAD
	encoder_value = get_encoder_value();
	//PI_controller();
	time += 20;
}

uint8_t get_time(){
	return time;
}

uint8_t get_integration_value(){
	return encoder_value;
}
=======
	printf("in timer interrupt\r\n");

}
>>>>>>> 13f9c942b72a376457979d82d1c0d154004a009c
