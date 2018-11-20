#define F_CPU 4915200

#include "oled_driver.h"

#include <util/delay.h>

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>

volatile frame_counter = 0;

void sprite_timer_init(){

	//set timer to normal mode
	clear_bit(TCCR0, WGM00);
	clear_bit(TCCR0, WGM01);

	//OC0 disconnected
	clear_bit(TCCR0, COM0);
	clear_bit(TCCR0, COM01);

	//Set clock prescaler to Fcpu/1024
	set_bit(TCCR0, CS00);
	clear_bit(TCCR0, CS01);
	set_bit(TCCR0, CS02);

	//enable overlow interrupt for timer0
	set_bit(TIMSK, TOIE0);
	set_bit(TIFR, TOV0);

}

ISR(TIMER0_OVF_vect){
	if (frame_counter == 5){
		frame_counter = 0;
	}
	oled_print_firework(frame_counter);
}

void disable_sprite_timer(){
	clear_bit(TCCR0, CS00);
	clear_bit(TCCR0, CS01);
	clear_bit(TCCR0, CS02);
}