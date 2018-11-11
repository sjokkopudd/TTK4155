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

static data_t* receive;
static data_t* message;
static uint16_t score = 0;

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

	update_servo(0);
	solenoid_init();

}


// -------------------------------------------------------------------
// receiving can messages and controlling the servo, motor, solenoid
// according to the message ids
// -------------------------------------------------------------------
void process_game(){

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
				update_motor(data);
				break;
			
			case eID_BTN_RIGHT:
				//shoot function
				solenoid_shoot();
				break;
			case eID_RESET:
				//reset score to zero
				score = 0;
				break;
			default: break;
		}

	}

	//update score
	if(check_collision()){
		score++;

		if(score & 0xffff){
			score = 0;
		}
	}

	message->id = eID_SCORE;
	message->length = 2;
	message->data[0] = (score & 0xff);
	message->data[1] = (score >> 8);
	can_send_message(message);

	printf("encoder value: %u\r\n", get_encoder_value());
	
	
}


