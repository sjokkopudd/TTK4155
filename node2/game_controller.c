#define F_CPU 16000000

#include "game_controller.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include "motor_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include "util/delay.h"

static data_t* receive;

static void update_servo(uint8_t val){

	//printf("update servo val: %d\n", val);
	//update servo position
	generate_pulse_servo(val);


}


void init_game_controller(){

	//initialize pwm to control the servo
	init_pwm();

	receive = malloc(sizeof(data_t));
	update_servo(0);

}


void process_game(){

	//buffer for receiving data via can
	if(!can_receive_message(receive)){
		//printf("in receive\r\n");
		uint8_t data;
		switch(receive->id){
			case eID_JOY_X:
				printf("joy x %d\r\n", receive->data[0]);
				//update servo position
				data = receive->data[0];
				update_servo(data);

				break;
		
			case eID_SLIDER_RIGHT:
				printf("slider pos %d\r\n", receive->data[0]);
				data = receive->data[0];
				update_motor(data);
				break;
			//TODO: add other cases
			default: break;
		}

	}
	
	
}


