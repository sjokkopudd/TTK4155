#define F_CPU 16000000

#include "game_controller.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include "motor_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include "util/delay.h"
#include "solenoid_driver.h"
#include "adc_driver_2.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "common.h"

static data_t* receive;
static data_t* message;
static uint16_t score = 0;

static const uint16_t INIT_DEBOUNCE_VAL = 0xffff;

//current collision state 
static collision_state curr_coll_state = eNo_Coll;

volatile uint16_t debounce_buffer;

// ----------------------------------------------------------------
// using timer 1 to debounce the score counting
// ---------------------------------------------------------------
static void timer1_init(){
	//disable global interrupts
	cli();

	//set clk prescalar fcpu/256
	set_bit(TCCR1B, CS10);
	clear_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS12);

	ICR1 = 6250; //timer overflow every 100ms

	//enable global interrupts
	sei(); 


}

static void timer1_start(){

	debounce_buffer = INIT_DEBOUNCE_VAL;

	//enable overflow interrupt
	set_bit(TIMSK1, TOIE1);
}

static void timer1_stop(){
	clear_bit(TIMSK1, TOIE1);
}

// ---------------------------------------------------------------
// timer 1 interrupt -> handling debouncing of ir-collision
// --------------------------------------------------------------
ISR(TIMER1_OVF_vect){
	//update debouncing buffer by getting the current ir collision value
	//store the retrieved value in the buffer
	debounce_buffer <<= (!check_collision());

}



static void update_servo(uint8_t val){

	//printf("update servo val: %d\n", val);
	//update servo position
	generate_pulse_servo(val);


}


void init_game_controller(){

	//initialize pwm to control the servo
	init_pwm();

	receive = malloc(sizeof(data_t));
	message = malloc(sizeof(data_t));
	score = 0;

	curr_coll_state = eNo_Coll;

	update_servo(0);
	solenoid_init();

}


// -------------------------------------------------------------------
// receiving can messages and controlling the servo, motor, solenoid
// according to the message ids
// -------------------------------------------------------------------
void process_game(){
	//message id and length for sending the score
	message->id = eID_SCORE;
	message->length = 2;


	//buffer for receiving data via can
	if(!can_receive_message(receive)){
		uint8_t data;
		switch(receive->id){
			case eID_JOY_X:
				//update servo position
				data = receive->data[0];
				update_servo(data);
				break;
			case eID_SLIDER_RIGHT:
				data = receive->data[0];
				//printf("data: %d\r\n", data);
				//update_motor(data);
				update_position(data);
				break;
			
			case eID_BTN_RIGHT:
				//shoot function
				solenoid_shoot();
				break;
			case eID_RESET:
				//reset score to zero
				score = 0;

				//TODO: init decoder !!!

				break;
			default: break;
		}

	}

	//update score only if there is a collision
	/*if(check_collision()){
		score++;

		if(score & 0xffff){
			score = 0;
		}

		message->data[0] = (score & 0xff);
		message->data[1] = (score >> 8);

		can_send_message(message);

	}
*/
	//check state of current collision
	//if there is a collision -> debounce it
	switch(curr_coll_state){
		case eNo_Coll:
			if(check_collision()){
				//update state
				curr_coll_state = eDebounce_Coll;

				//reset debouncing buffer, start timer
				timer1_start();

			}
			break;
		case eDebounce_Coll:
			if(!(debounce_buffer ^ (0xe000))){
				//is debounced -> increase score
				score++;

				//stop timer
				timer1_stop();

				//send score
				message->data[0] = (score & 0xff);
				message->data[1] = (score >> 8);

				can_send_message(message);

				curr_coll_state = eNo_Coll;
			}

			break;
		default: break;
	}
	
	
	

	//printf("encoder value: %u\r\n", get_encoder_value());
	
	
}


