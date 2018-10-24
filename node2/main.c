#define F_CPU 16000000


#include "util/delay.h"
#include <stdint.h>
#include "uart_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>


#include "SPI_driver.h"
#include "mcp_driver.h"
#include "MCP2515.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include "adc_driver_2.h"
#include "game_controller.h"


//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 

 
void ex6_uart_init(){

	unsigned long clock_speed = F_CPU;

	uart_init(clock_speed);

	stdout = &uart_stream;

	while(1){


		printf("Hello from node 2\r\n");

		_delay_ms(2000);

	}

}


// --------------------------------------------
// test spi interface - send and read test byte
// --------------------------------------------d
void ex6_spi_init(){


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

	
		/*if(can_send_message(message)){
			printf("Error in sending messages\r\n");
		}
		else{*/
		_delay_ms(1000);
		if(!can_receive_message(receive)){

			printf("Node 2 received message: ");
			
			for (int i = 0; i < receive->length; ++i){
				printf("%c", receive->data[i]);	
			}
			printf("\r\n");
		}
				
			
		//}

		//test mcp
		/*for (int i = 0; i < message->length; i++) {
			mcp_write( MCP_TXB0D0 + i, message->data[i]);
		}



		//read from that register
		for (int i = 0; i < 5; ++i)
		{
			receive->data[i] = mcp_read(MCP_RXB0D0 + i);
		}

		printf("receive->data%s\n",receive->data )*/
		

		/*clear_bit(PORTB, PB4);


		//send message
		SPI_write(0b10101010);


		char data = SPI_read();

		//deactivate slave
		set_bit(PORTB, PB4);

		printf("data received: %d\r\n", data);*/


		_delay_ms(2000);
	}

}


void ex7_pwm_pulse(){
	stdout = &uart_stream;
	can_init();
	init_game_controller();
	int pos = 0;


	while(1){
		process_game();
		

	}


}

void ex7_ir_value(){
	stdout = &uart_stream;
	adc_2_init();



	while(1){
	
		uint16_t irValue = get_IR_value();
		//printf("irVal: %d\r\n", irValue);


	}


}


int main(){
	
	unsigned long clock_speed = F_CPU;

	uart_init(clock_speed);



	//ex7_ir_value();


	//ex6_uart_init();


	//ex6_spi_init();

	//printf("is changing pulse\r\n");

	ex7_pwm_pulse();


	return 0;
}

