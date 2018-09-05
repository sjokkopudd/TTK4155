

#define F_CPU 4915200

#include "util/delay.h"
#include <stdint.h>
#include "uart_driver.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "sram.h"





int main(){
	/* Ex 1 - Test with writing on LED*/
	/*DDRC |= (1 << PC0);

	while(1){
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);
		_delay_ms(1000);
	}*/

	
	/* Ex 1 - Uart and Printf */
	
	unsigned long clock_speed = F_CPU;
	

	uart_init(clock_speed);

	/*while(1){
		uart_transmit('H');
		uart_transmit('\r');
		uart_transmit('\n');
		
		_delay_ms(100);
		//unsigned char data = UART_Receive();
		printf("Hello World");
		printf("\r\n");
	}*/

	//enable external memory
	set_bit(MCUCR, SRE);


	char data = 'c';
	volatile char *ext_ram = (char *) 0x1800;
	while(1){
			*ext_ram = data;

	}
	
	//ext_ram[0x7ff] = data;

	/* Ex 2 - External Ram */
	//char data = 'c';
	//volatile char* ext_ram = 0x1000;
	

	//sram_test();


	/*while(1) {
		ext_ram[0x000] = data;
		//printf("OLED command\n");
		_delay_ms(3000);
		ext_ram[0x300] = data;
		//printf("OLED data\n");
		_delay_ms(3000);
		ext_ram[0x400] = data;
		//printf("ADC\n");
		_delay_ms(3000);
		ext_ram[0x800] = data;
		//printf("SRAM\n");
		_delay_ms(3000);
	}*/






	return 0;
}

