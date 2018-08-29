

#define F_CPU 4915200
#include "util/delay.h"
#include <stdint.h>
#include "UART_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>



int main(){
	/*DDRC |= (1 << PC0);

	while(1){
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);
		_delay_ms(1000);
	}*/


	unsigned long clock_speed = F_CPU;
	


	UART_Init(clock_speed);

	while(1){
		/*UART_Transmit('c');
		UART_Transmit('\n');
		_delay_ms(100);*/
		//unsigned char data = UART_Receive();
		printf("kdjfkj");



}



	return 0;
}

