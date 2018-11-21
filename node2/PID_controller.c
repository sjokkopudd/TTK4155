#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "PID_controller.h"
#include "motor_driver_v2.h"
#include "common.h"
#include "dac_driver.h"
#include <stdio.h>
#include <stdlib.h>


volatile int16_t ref_position = 127;
static double Kp = 4;
static double Ki = 0.001;
static double Kd = 0.06;
volatile int16_t integral = 0;
volatile int16_t prev_error = 0;
static double dt = 0.012;

void PID_controller(){
    double encoder = motor_get_encoder_value()*(255./get_MAX());
	int16_t error = ref_position - encoder;
	integral += error;

	if (error < 10){
		integral = 0;
	}

	int16_t u = Kp*error + Ki*dt*integral + (Kd/dt)*(error - prev_error);

	prev_error = error;

	if (u < 0){
		//set direction to left
		motor_set_direction(eDIR_LEFT);
		if (abs(u) > 150){
			u = 150;
		}
		else if (abs(u) < 50 & abs(u) > 20){
			u = 30;
		}		
	}
	else{
		//set direction to right
		motor_set_direction(eDIR_RIGHT);
		if (abs(u) > 150){
			u = 150;
		}
		else if (abs(u) < 50 & abs(u) > 20){
			u = 50;
		}
	}

	dac_send(abs(u));
}

ISR(TIMER3_OVF_vect){

	PID_controller();

}

void PID_init(){

	 // Disable global interrupts
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

	//set the counter to enable flag every 40ms
	ICR3 = 750;

	//enable global interrupts
	sei();

	
}

void PID_update_reference(uint8_t pos){
	ref_position = (int16_t)pos;

}
