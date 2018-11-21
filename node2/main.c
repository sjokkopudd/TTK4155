#define F_CPU 16000000


#include "util/delay.h"
#include <stdint.h>
#include "uart_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "SPI_driver.h"
#include "mcp_driver.h"
#include "MCP2515.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include "adc_driver_2.h"
#include "game_controller.h"
#include "TWI_Master.h"
#include "dac_driver.h"
#include "motor_driver_v2.h"
#include "solenoid_driver.h"
#include "PID_controller.h"
#include "common.h"

//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 

unsigned long clock_speed = F_CPU;
 

int main(){
	
	uart_init(clock_speed);
	stdout = &uart_stream;

	can_init();
	dac_init();
	adc_2_init();
	init_pwm();
	init_game_controller();
	motor_init();
	PID_init();
	while(1){
		
		process_game();
	}

	return 0;
}

