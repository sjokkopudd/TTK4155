#include "game_controller.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include <stdio.h>
#include <stdlib.h>

static data_t* receive;

static void update_servo(uint8_t val){

	printf("update servo val: %d\n", val);
	//update servo position
	generate_pulse_servo(val);


}


void init_game_controller(){

	//initialize pwm to control the servo
	init_pwm();

	receive = malloc(sizeof(data_t));

}


void process_game(){

	//buffer for receiving data via can
	if(!can_receive_message(receive)){
		printf("in receive\r\n");
		switch(receive->id){
			case eID_JOY_X:
				printf("joy x\r\n");
				//update servo position
				update_servo(receive->data[0]);

				break;
			//TODO: add other cases
			default: break;
		}

	}
	
	
}


