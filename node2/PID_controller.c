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
//static double motor_position_middle = 0;
static double Kp = 1; //2;
static double Ki = 0.001; //4;
static double Kd = 0.001;//0.1;
volatile int16_t integral = 0;
volatile int16_t prev_error = 0;
static double dt = 0.02;//0.016;

void PID_controller(){
    double encoder = motor_get_encoder_value()*(255./get_MAX());
   // printf("encoder_raw: %d\r\n",motor_get_encoder_value());
    //printf("ref_position: %d\r\n", ref_position);
	int16_t error = ref_position - encoder;
	//printf("error: %d\r\n", error);
	integral += error;

	if (error < 1){
		integral = 0;
	}

	int16_t u = Kp*error + Ki*dt*integral + (Kd/dt)*(error - prev_error);

	prev_error = error;

	if (u < 0){
		//set direction to left
		motor_set_direction(eDIR_LEFT);
		if (abs(u) > 100){
			u = 100;
		}
		
	}
	else{
		//set direction to right
		motor_set_direction(eDIR_RIGHT);
		if (abs(u) > 100){
			u = 100;
		}
	}
	dac_send(u);
	//printf("u: %d\r\n", u);
}

ISR(TIMER3_OVF_vect){

//	printf("in interrupt\n");

	PID_controller();

	/*double encoder = motor_get_encoder_value()*(255./get_MAX());
   // printf("encoder_raw: %d\r\n",motor_get_encoder_value());
   // printf("ref_position: %d\r\n", ref_position);
	int16_t error = ref_position - encoder;
	//printf("error: %d\r\n", error);
	integral += error;

	if (error < 1){
		integral = 0;
	}

	int16_t u = Kp*error + Ki*dt*integral + (Kd/dt)*(error - prev_error);

	prev_error = error;

	if (u < 0){
		//set direction to left
		motor_set_direction(eDIR_LEFT);
		if (abs(u) > 100){
			u = 100;
		}
		
	}
	else{
		//set direction to right
		motor_set_direction(eDIR_RIGHT);
		if (abs(u) > 100){
			u = 100;
		}
	}
	dac_send(u);
	printf("u: %d\r\n", u);
	
	*/

}

void PID_init(){
	
	 //motor_position_middle = -(get_MAX() + get_MIN()) / 2;
	
	/*//-------------INITIALIZE TIMER INPUT-----------------
	
	// Disable global interrupts
	cli();
	
	// enable timer overflow interrupt for Timer2
	TIMSK2=(1<<TOIE2);
	
	// start timer2 with /1024 prescaler
	
	TCCR2B = (1<<CS20) | (1<<CS21) | (1<<CS22);
	
	// Enable global interrupts
	sei();
	
	//---------------------------------------------------*/

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
	ICR3 = 1250;

	printf("pid init done\n");

	//enable global interrupts
	sei();

	
}

void PID_update_reference(uint8_t pos){
	ref_position = (int16_t)pos;

}

/*void PID_update(difficulty_t difficulty){
	
	switch (difficulty){
		case EASY:
			Kp = 2.5;
			Ki = 2;
			Kd = 0.1;
			break;
		case MEDIUM:
			Kp = 2;
			Ki = 4;
			Kd = 0.1;
			break;
		case HARD:
			Kp = 2;
			Ki = 4;
			Kd = 0.1;
			break;
		}
}*/


