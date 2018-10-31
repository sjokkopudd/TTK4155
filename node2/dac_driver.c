#include "dac_driver.h"
#include "common.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>


void dac_init(){
	sei();
	TWI_Master_Initialise();
}

void dac_send(unsigned char data){
	unsigned char msg[3];
	msg[0] = 0b01010000;
	msg[1] = 0b00000000;
	msg[2] = data;

	TWI_Start_Transceiver_With_Data(msg, 3);

}