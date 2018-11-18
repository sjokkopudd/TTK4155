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

static game_msg_node1_t curr_msg_node1 = {0};
static game_msg_node2_t curr_msg_node2 = {0};

static game_msg_node2_t msg_reset ={0};


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


// ---------------------------------------------------------------
// timer 4 interrupt upcountes the scores
// --------------------------------------------------------------
ISR(TIMER4_OVF_vect){
	//update score
	score++;

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


	receive = malloc(sizeof(data_t));
	message = malloc(sizeof(data_t));
	score = 0;

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

		curr_msg_node1.game_start = receive->data[0];
		curr_msg_node1.game_exit = receive->data[1];
		curr_msg_node1.game_shoot = receive->data[2];
		curr_msg_node1.game_joy = receive->data[3];
		curr_msg_node1.game_slider = receive->data[4];

		if(curr_msg_node1.game_start){
			printf("node 2 got start\r\n");
			//reset score
			score = 0;

			//reset scores and game over
			curr_msg_node2 = msg_reset;

			printf("curr_msg_node2.game over: %d\r\n", curr_msg_node2.game_over);
			printf("curr_msg_node2.score: %d\r\n", curr_msg_node2.game_score);

			//set game is active flag
			game_is_active = 1;

			//start timer 4 to count scores
			timer4_start();

		}
		else if(curr_msg_node1.game_exit){
			//set game inactive
			game_is_active = 0; 

			//stop timer to count scores
			timer4_stop();

		}
		else if(curr_msg_node1.game_shoot){
			//shoot
			solenoid_shoot();

		}
		else{
			//processing of joystick and slider data

			//update servo
			update_servo(curr_msg_node1.game_joy);

			//update pid controller for controlling motor
			PID_update_reference(curr_msg_node1.game_slider);

		}

	}

	//updating score and check if collision has been detected > only if 
	if(game_is_active){
		printf("ir_value: %u\r\n", get_IR_value());
		if(check_collision()){
			printf("coll detected\r\n");
			game_is_active = 0;

			//stop timer to upcount scores
			timer4_stop();

			//set flag to indicate the game over
			curr_msg_node2.game_over = 1;

			
		}
		//update current score
		curr_msg_node2.game_score = score;

		message->id = eID_NODE2;
		message->length = 3;
		message->data[0] = curr_msg_node2.game_over;
		message->data[1] = (curr_msg_node2.game_score & 0xff);
		message->data[2] = (curr_msg_node2.game_score  >> 8);

		can_send_message(message);

		printf("node 2 sending \r\n");
		

	}
	
}


