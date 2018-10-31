#include "PWM_driver.h"
#include "common.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#define MIN_DUTY_CYCLE 4.5
#define MAX_DUTY_CYCLE 10.5
#define PWM_TOP 5000
#define MAX_PULSE_SERVO 525
//(PWM_TOP*MAX_DUTY_CYCLE)/100
#define MIN_PULSE_SERVO 225
//(PWM_TOP*MIN_DUTY_CYCLE)/100


void init_pwm(){


	//set mode 14: Fast PWM - writing to control registers
	set_bit(TCCR1A,WGM11);
	clear_bit(TCCR1A, WGM10); 
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B, WGM12);

	//set output mode non-inverting
	set_bit(TCCR1A,COM1A1);
	set_bit(TCCR1A,COM0A1);

	//enable output direction of pin 
	set_bit(DDRB, PB5);

	//set prescaler to fcpu/64
	clear_bit(TCCR1B, CS12);
	set_bit(TCCR1B, CS11);
	set_bit(TCCR1B, CS10);

	//set ICR1 5000 = 20 ms period (TOP)
	ICR1 = PWM_TOP;


}


void generate_pulse_servo(int pos){

	uint16_t pulse;

	printf("MIN: %d\r\n", MIN_PULSE_SERVO);
	printf("MAX: %d\r\n", MAX_PULSE_SERVO);
	
	//make sure that input is valid
	if(pos < 0){
		pulse = MIN_PULSE_SERVO;
	}
	else if(pos > 255){
		pulse = MAX_PULSE_SERVO;
	}

	else{

		//distribute position across servo range

		pulse = MIN_PULSE_SERVO + ((double)pos/255) * (MAX_PULSE_SERVO - MIN_PULSE_SERVO);
		
	}

	//make sure that output is valid 
	if(pulse < MIN_PULSE_SERVO){
		pulse = MIN_PULSE_SERVO;
	}
	else if(pulse > MAX_PULSE_SERVO){
		pulse = MAX_PULSE_SERVO;
	}


	printf("pulse: %d\n", pulse );
	

	OCR1A = pulse;

}