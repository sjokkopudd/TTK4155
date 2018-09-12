

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


static void ext_mem_init(void){
	//enable external memory
	set_bit(MCUCR, SRE);
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR, XMM1);
	clear_bit(SFIOR, XMM0);
}


static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 
                               
void ex1(){
	/* Ex 1 - Test with writing on LED*/
	/*DDRC |= (1 << PC0);

	while(1){
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);
		_delay_ms(1000);
	}*/

	//-------------------------------------
	//Uart and Printf 
	/*while(1){
		uart_transmit('H');
		uart_transmit('\r');
		uart_transmit('\n');
		
		_delay_ms(100);
		//unsigned char data = UART_Receive();
		printf("Hello World");
		printf("\r\n");
	}*/
	//------------------------------------


}

void ex2(){
/* Ex 2 - External Ram */
	
	//configure pins
	//enable external memory
	ext_mem_init();
	
	//run the test program
	//sram_test();


	//test sram, adc and oled with the gal
	volatile char *ext_ram = (char *) 0x1000;
	uint8_t data = 55;

	joy_direction_t dir;
	joy_analog_pos analog_pos;

	while(1) {
		//write to OLED
		//ext_mem_write(0x300, data);

		//ext_ram[0x300] = data;
		//_delay_ms(3000);

		//ext_ram[0x400] = data;
		//write to ADC
		//sram_write(0x400, data);
		//_delay_ms(3000);
		
		//uint16_t addr = 0x500;
		//ext_ram[0x800]=d ata;
		//sram_write(addr, data);
		//ext_ram[0x800] = data;
		//write to SRAM
		//ext_mem_write(0x800,data);

		//_delay_ms(3000);
		uint8_t dir_x = adc_read_channel(JOYCON_X);
		uint8_t dir_y = adc_read_channel(JOYCON_Y);
		
		
		dir = joystick_get_direction(dir_x, dir_y);
		analog_pos = joystick_get_analog_pos(dir_x, dir_y);
		printf("dir = %s \r\nperc x = %d\\%, perc y = %d\\%\r\n",enumStrings[dir], analog_pos.x, analog_pos.y); 
	}
}


int main(){
	
	unsigned long clock_speed = F_CPU;

	uart_init(clock_speed);

	ex2();






	return 0;
}

