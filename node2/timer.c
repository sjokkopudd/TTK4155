#include "timer.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint8_t integration_value = 0;
volatile uint8_t time = 0;

//initialize 16bit timer/counter3 to count at increments of 20ms, and enable flag
void timer_init(){

	//enable global interrupts
	sei();

	//enable overflow interrupt
	set_bit(TIMSK3, TOIE3);

	//set timer to PWM fast mode
	clear_bit(TCCR3A, WGM30);
	set_bit(TCCR3A, WGM31);
	set_bit(TCCR3B, WGM32);
	set_bit(TCCR3B, WGM33);


	//set clk prescalar fcpu/1024
	set_bit(TCCR3B, CS30);
	clear_bit(TCCR3B, CS31);
	set_bit(TCCR3B, CS32);

	//set the counter to enable flag every 20ms
	ICR3 = 1250;

}

ISR(TIMER3_OVF_vect){


}

