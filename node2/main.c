#define F_CPU 16000000


#include "util/delay.h"
#include <stdint.h>
#include "uart_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#include "SPI_driver.h"
#include "mcp_driver.h"
#include "MCP2515.h"
#include "can_communication.h"
#include "PWM_driver.h"
#include "adc_driver_2.h"
#include "game_controller.h"
#include "TWI_Master.h"
#include "dac_driver.h"
#include "motor_driver.h"
#include "solenoid_driver.h"
#include "timer.h"

//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);

static char* enumStrings[] = {"left", "right", "up", "down", "neutral"}; 

unsigned long clock_speed = F_CPU;
 
void ex6_uart_init(){

	//uart_init(clock_speed);

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
	dac_init();
	adc_2_init();
	init_pwm();
	init_game_controller();
	solenoid_init();
	motor_init();
	int pos = 0;


	while(1){
		process_game();

	}


}

void ex7_test(){
	stdout = &uart_stream;
	can_init();
	init_pwm();
	int pos;

	while(1){
		pos = 0;
		generate_pulse_servo(pos);
		_delay_ms(2000);
		pos = 127;
		generate_pulse_servo(pos);
		_delay_ms(2000);
		pos = 255;
		generate_pulse_servo(pos);
		_delay_ms(2000);
	}
}

void ex7_ir_value(){
	stdout = &uart_stream;
	adc_2_init();



	while(1){
	
		if (check_collision()){
			printf("collision \r\n");
		}
		
		_delay_ms(20);

	}


}

void ex8_dac(){
	dac_init();
	unsigned char data;
	while(1){
		data = 0b00000000;
		dac_send(data);
		_delay_ms(2000);
		data = 0b01111111;
		dac_send(data);
		_delay_ms(2000);
		data = 0b11111111;
		dac_send(data);
		_delay_ms(2000);
	}
}

void test_timer(){
	stdout = &uart_stream;
	timer_init();

	
	while(1){

	}
}

void test_encoder(){
	stdout = &uart_stream;
	motor_init();
	while(1){
		printf("encoder value: %d\r\n", get_encoder_value());
	}
}

void test() {
	stdout = &uart_stream;
	dac_init();
	uint8_t val = 60;
	while(1){
		for (int i = 0; i < 255; i++){
			update_motor(i);
			_delay_ms(20);
		}
		/*val = 0
		update_motor(val);
		_delay_ms(2000);
		val = 127;
		update_motor(val);
		_delay_ms(2000);
		val = 255;
		update_motor(val);
		_delay_ms(2000);*/

		/*unsigned char data = val;
		printf("data: %d\n", data);*/
	}
}

int main(){
	
	unsigned long clock_speed = F_CPU;

	uart_init(clock_speed);



	//ex7_ir_value();


	//ex6_uart_init();


	//ex6_spi_init();

	//printf("is changing pulse\r\n");

	
	//x7_pwm_pulse();
	//ex7_ir_value();
	//test_timer();
	ex7_pwm_pulse();


	//ex7_test();

	//ex8_dac();
	//test();

	return 0;
}

