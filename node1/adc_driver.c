#define F_CPU 4915200

#include "adc_driver.h"
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"
#include <util/delay.h>
#include <stdio.h>


void adc_init(void){

	

	DDRB &= ~(1 << DDB3);


}

uint8_t adc_read_channel(adc_channel_t CHANNEL){

	_delay_us(1);
	uint8_t data_from_adc;
	volatile char *ext_ram = (char *) 0x1400;

	ext_ram[0x00] = CHANNEL;
	//loop_until_bit_is_set(PINB,PINB3);
	while((PINB & (1 << PB3)));
	
	data_from_adc = ext_ram[0x00];

	while(!(PINB & (1 << PB3)));		

	return data_from_adc;

}

