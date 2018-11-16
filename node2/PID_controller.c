#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "PID_controller.h"
#include "motor_driver_v2.h"
#include "common.h"

int16_t rot_max = -8800;
int16_t rot_min = 100;

int16_t ref_position = 0;
double motor_position_middle = 0;
double Kp = 1; //2;
double Ki = 0.005; //4;
double Kd = 0.01;//0.1;
double integral = 0;
int16_t prev_error = 0;
double dt = 0.02;//0.016;

ISR(TIMER3_OVF_vect){
	
	int16_t encoder_val = motor_get_encoder_value();

	double measured = (encoder_val + motor_position_middle) / (-motor_position_middle/100);
	
	int16_t ref = ref_position;
	
	int16_t error = ref - measured;
	integral = integral + error * dt;
	
	if (error < 1){
		integral = 0;
	}
	
	double derivative = -(error - prev_error)/dt;
	
	int16_t u = 0;
	u = Kp * error + Ki  * integral + Kd * derivative;

	prev_error = error;

	update_motor_with_u(u);
	
	

}

void PID_init(){
	
	 motor_position_middle = -(get_MAX() + get_MIN()) / 2;
	
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

	//set the counter to enable flag every 20ms
	ICR3 = 1250;

	//enable global interrupts
	sei();
	
}

void PID_update_reference(int16_t pos){
	ref_position = pos;
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