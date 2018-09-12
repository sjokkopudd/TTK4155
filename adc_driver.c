#include "adc_driver.h"

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"
#include "util/delay.h"


void adc_init(void){

	clear_bit(DDRB, PB3);

}

uint8_t adc_read_channel(adc_channel_t CHANNEL){

	_delay_us(1);
	uint16_t data_from_adc;
	volatile char *ext_ram = (char *) 0x1400;

	ext_ram[0x00] = CHANNEL;
	_delay_us(60);
	data_from_adc = ext_ram[0x00];
			

	return data_from_adc;

}


/*_delay_us(1);

	uint16_t data_from_adc;

	volatile char *ext_ram = (char *) 0x1000;
	ext_ram[0x400] = 0x4; //single ended ch1
	_delay_us(60);
	data_from_adc = ext_ram[0x400];
	printf(data_from_adc);*/