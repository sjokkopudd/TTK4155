#define F_CPU 16000000

#include "dac_driver.h"
#include "common.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "util/delay.h"


void dac_init(){
	cli();
	TWI_Master_Initialise();
	sei();
}

void dac_send(unsigned char data){
	unsigned char msg[3];
	msg[0] = 0b01010000;
	msg[1] = 0b00000000;
	msg[2] = data;
	//printf("before send \r\n");
	//TWCR = (0<<TWIE);
	TWI_Start_Transceiver_With_Data(msg, 3);
	_delay_us(100);

}