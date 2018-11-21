#define F_CPU 4915200

#include "util/delay.h"
#include <stdint.h>
#include "uart_driver.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "sram_driver.h"
#include "adc_driver.h"
#include "joystick_driver.h"
#include "oled_driver.h"
#include "oled_menu.h"
#include "pinball_statemachine.h"
#include "SPI_driver.h"
#include "mcp_driver.h"
#include "MCP2515.h"
#include "can_communication.h"
#include "pwm_buzzer.h"

//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 
unsigned long clock_speed = F_CPU;

//enable external memory
static void ext_mem_init(void){
	set_bit(MCUCR, SRE);
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR, XMM1);
	clear_bit(SFIOR, XMM0);
}
                               

int main(){
	
	uart_init(clock_speed);
	adc_init();
	ext_mem_init();
	can_init();
	init_pwm();

	init_pinball_game();
	
	while(1){

		pinball_game_process();
	}

	return 0;
}

