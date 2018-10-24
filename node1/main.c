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


//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 

static void ext_mem_init(void){
	//enable external memory
	set_bit(MCUCR, SRE);
	set_bit(SFIOR, XMM2);
	clear_bit(SFIOR, XMM1);
	clear_bit(SFIOR, XMM0);
}
                               
void ex1(){
	uint8_t data =10;
	/* Ex 1 - Test with writing on LED*/
	/*DDRC |= (1 << PC0);

	while(1){
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);
		_delay_ms(1000);
	}*/

	//-------------------------------------

	FILE* file = NULL;
	//Uart and Printf 
	while(1){
		/*uart_transmit('H', file);
		uart_transmit('\r', file);
		uart_transmit('\n', file);*/
		
		_delay_ms(100);
		//unsigned char data = UART_Receive();
		printf("Hello World: %d\n\r", data);
		
		printf("\r\n");
	}
	//------------------------------------


}

void ex2(){
/* Ex 2 - External Ram */
	
	//configure pins
	//enable external memory
	ext_mem_init();
	
	//run the test program
	sram_test();

	while(1){
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


	}
}

// --------------------------------------------------------
// test joystick
// -------------------------------------------------------
/*void ex3_joystick(){


	//test sram, adc and oled with the gal
	volatile char *ext_ram = (char *) 0x1000;
	uint8_t data = 55;

	joystick_init();
	joy_direction_t dir;
	joy_analog_pos analog_pos;

	volatile uint8_t adc_x = 0;
	volatile uint8_t adc_y = 0;
	while(1) {
		
		adc_channel_t CHANNEL;
		CHANNEL = JOYSTICK_X;
		
		adc_x = adc_read_channel(CHANNEL);
		_delay_ms(100);

		CHANNEL = JOYSTICK_Y;
		
		adc_y = adc_read_channel(CHANNEL);

		printf("dir_x: %d, dir_y: %d\r\n", adc_x, adc_y );
		
		
		dir = joystick_get_direction(adc_x, adc_y);
		analog_pos = joystick_get_analog_pos(adc_x, adc_y);
		printf("dir = %s \r\nperc x = %d%%, perc y = %d%%\r\n",enumStrings[dir], analog_pos.x, analog_pos.y); 
		

		uint8_t slider_left = get_slider_pos_left();
		uint8_t slider_right = get_slider_pos_right();

		uint8_t button_left = get_button(BTN_LEFT);
		uint8_t button_right = get_button(BTN_RIGHT);

		printf("slider left: %d, slider right: %d\r\n" ,slider_left , slider_right);
		printf("left button: %d, right button: %d\r\n\r\n\r\n", button_left, button_right);
		
		_delay_ms(2000);
		

	}
}*/

// -------------------------------------------
// first steps with oled -> init, 
// go to specified line or column and print
// test string
// ------------------------------------------
void ex4_oled_first_steps(){

	ext_mem_init();

	

	while(1){
	
		oled_init();

		oled_goto_line(3);

		oled_print("hello");

		_delay_ms(2000);


	}

	
}

// -------------------------------------------
// test oled and menu of oled
// -------------------------------------------
static void ex4_oled_menu(){
	ext_mem_init();
	can_init();

	InitPinballGame();
	
	while(1){

		PinballGameProcess();
	}
}


// --------------------------------------------
// test spi interface - send and read test byte
// --------------------------------------------d
void ex5_spi_init(){

	ext_mem_init();
	

	can_init();
	data_t* message = malloc(sizeof(data_t));
	data_t* receive = malloc(sizeof(data_t));

	message->id = 1;
	message-> length = 5;

	
	message->data[0] = 'H';
	message->data[1] = 'a';
	message->data[2] = 'l';
	message->data[3] = 'l';
	message->data[4] = 'o';

	stdout = &uart_stream;

	

	
	while(1){

	
		for (int i = 0; i < message->length; ++i){
			printf("%c", message->data[i]);	
		}
		printf("\r\n");

		if(can_send_message(message)){
			printf("Error in sending messages\r\n");
		}
		else{
			_delay_ms(1000);
			if(!can_receive_message(receive)){

				printf("received message: ");
				
				for (int i = 0; i < receive->length; ++i){
					printf("%c", receive->data[i]);	
				}
				printf("\r\n");
			}
				
			else{
				printf("Error in receiving message\r\n");
			}
		}

		//test mcp
		for (int i = 0; i < message->length; i++) {
			mcp_write( MCP_TXB0D0 + i, message->data[i]);
		}


		


		_delay_ms(2000);
	}

}

int main(){
	
	unsigned long clock_speed = F_CPU;

	uart_init(clock_speed);

	

	//ex1();
	//joystick_calibrate();
	adc_init();

	//ex2();
	//ex4();

	//ex5_can();

	//ex4_oled_first_steps();
	ex4_oled_menu();

	//ex5_spi_init();





	return 0;
}

