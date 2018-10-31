#include "adc_driver_2.h"
#include "common.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

void adc_2_init(){
	//enable adc
	set_bit(ADCSRA, ADEN);

	//Choose voltage reference AREF
	clear_bit(ADMUX, REFS0);
	clear_bit(ADMUX, REFS1);

	//decide input channel ADC0, single ended input
	clear_bit(ADMUX, MUX0);
	clear_bit(ADMUX, MUX1);
	clear_bit(ADMUX, MUX2);
	clear_bit(ADMUX, MUX3);
	clear_bit(ADMUX, MUX4);
	clear_bit(ADCSRB, MUX5);

	set_bit(ADCSRA, ADPS2);
	set_bit(ADCSRA, ADPS1);
	set_bit(ADCSRA, ADPS0);
	
	
	//set trigger mode, single conversions
	clear_bit(ADCSRA, ADATE);
}

uint16_t get_IR_value(){

	//start conversion
	set_bit(ADCSRA, ADSC);

	//wait for conversion to finish
	while (test_bit(ADCSRA,ADSC)){
	}

	//return result
	return ADC;
}

uint8_t check_collision(){

}