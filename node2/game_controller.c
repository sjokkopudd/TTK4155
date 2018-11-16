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
#include <inttypes.h>
#include "common.h"
#include "PID_controller.h"

static data_t* receive;
static data_t* message;
static uint16_t score = 0;
static uint8_t game_is_active = 0;
static const uint16_t INIT_DEBOUNCE_VAL = 0xffff;

//current collision state 
static collision_state curr_coll_state = eNo_Coll;

volatile uint16_t debounce_buffer;


// ----------------------------------------------------------------
// using timer 4 to debounce the score counting
// ---------------------------------------------------------------
static void timer4_init(){
	//disable global interrupts
	cli();

	//set timer to PWM fast mode
	clear_bit(TCCR4A, WGM40);
	set_bit(TCCR4A, WGM41);
	set_bit(TCCR4B, WGM42);
	set_bit(TCCR4B, WGM43);

	//set output mode non-inverting
	clear_bit(TCCR4A, COM4A0);
	set_bit(TCCR4A, COM4A1);
	

	//set clk prescalar fcpu/1024
	set_bit(TCCR4B, CS10);
	clear_bit(TCCR4B, CS11);
	set_bit(TCCR4B, CS12);

	//set_bit(TIMSK4, TOIE4);


	ICR4 = 3125; //timer overflow every 200ms

	
	//enable global interrupts
	sei(); 


}

static void timer4_start(){

	//reset scores
	score = 0;

	//enable overflow interrupt
	set_bit(TIMSK4, TOIE4);
}

static void timer4_stop(){

	clear_bit(TIMSK4, TOIE4);
}

static void timer5_init(){
	//disable global interrupts
	cli();

	//set timer to PWM fast mode
	clear_bit(TCCR5A, WGM50);
	set_bit(TCCR5A, WGM51);
	set_bit(TCCR5B, WGM52);
	set_bit(TCCR5B, WGM53);

	//set output mode non-inverting
	clear_bit(TCCR5A, COM5A0);
	set_bit(TCCR5A, COM5A1);
	

	//set clk prescalar fcpu/256
	set_bit(TCCR5B, CS10);
	clear_bit(TCCR5B, CS11);
	clear_bit(TCCR5B, CS12);

	

	//set_bit(TIMSK5, TOIE5);


	ICR5 = 6250; //timer overflow every 100ms
	
	
	//enable global interrupts
	sei(); 



}

static void timer5_start(){

	//reset scores
	debounce_buffer = 0xffff;

	printf("start timer 5\r\n");

	//enable overflow interrupt
	set_bit(TIMSK5, TOIE5);

}

static void timer5_stop(){

	printf("stop timer 5\n");

	clear_bit(TIMSK5, TOIE5);
}


ISR(TIMER5_OVF_vect){



	//printf("debounce_buffer: %u\r\n", debounce_buffer);

	debounce_buffer <<= (check_collision());


}


// ---------------------------------------------------------------
// timer 1 interrupt -> handling debouncing of ir-collision
// --------------------------------------------------------------
ISR(TIMER4_OVF_vect){
	
	//printf("in timer one for debouncing\r\n");
	//update debouncing buffer by getting the current ir collision value
	//store the retrieved value in the buffer
	score++;

	printf("score: %u\r\n", score);
	
	//printf("timer: %u\n", debounce_buffer );
	//
	//printf("coll %d, timer: %u\r\n", !check_collision(), debounce_buffer );

}



static void update_servo(uint8_t val){

	//printf("update servo val: %d\n", val);
	//update servo position
	generate_pulse_servo(val);


}


void init_game_controller(){

	//initialize pwm to control the servo
	init_pwm();
	timer4_init();
	timer5_init();


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
				//update reference for PID
				PID_update_reference(data);			
				break;
			
			case eID_BTN_RIGHT:
				//shoot function
				solenoid_shoot();
				break;
			case eID_START:
				printf("game started \r\n");
				//reset score to zero
				score = 0;

				//set game active
				game_is_active = 1;

				//start timer to count
				timer4_start();

				//TODO: init decoder !!!

				break;
			default: break;
		}

	}

	//update score only if there is a collision
	/*if(game_is_active){
		if(check_collision()){
			printf("collision\r\n");
			timer4_stop();
			message->id = eID_GAME_OVER;
			message->length = 1;
			message->data[0] = 0;


			can_send_message(message);

			game_is_active = 0;
		}

	}*/
	//check state of current collision
	//if there is a collision -> debounce it
	if(game_is_active){


		switch(curr_coll_state){
			case eNo_Coll:
				
				if(check_collision()){

				    printf("collision detected\r\n");
					//update state
					curr_coll_state = eDebounce_Coll;

					//reset debouncing buffer, start timer
					timer5_start();

				}

				else{
					//send score
					message->id = eID_SCORE;
					message->length = 2;
					message->data[0] = (score & 0xff);
					message->data[1] = (score >> 8);

					can_send_message(message);
				}
				break;
			case eDebounce_Coll:
				printf("eDebounce_Coll: buffer: %u\r\n", debounce_buffer);
				if(!debounce_buffer){
				//	printf("debounced\n");
					//stop timer for counting scores
					timer5_stop();

					//debounce_buffer = 0xffff;

					//send game over via can
					message->id = eID_GAME_OVER;
					message->length = 1;
					message->data[0] = 0;

					can_send_message(message);

					//change state
					curr_coll_state = eNo_Coll;


					game_is_active = 0;
				}

				break;
			default: break;
		}
	}
	
	
	

	//printf("encoder value: %u\r\n", get_encoder_value());
	
	
}


