/*------------------------------------------------------
* uart_driver.c 
* implementation of functions
* init, transmit and receive data with uart
------------------------------------------------------*/

#include "uart_driver.h"
#include <avr/io.h>

#include "common.h"


#define BAUD 9600


void uart_init(unsigned long clk){

	/*Set baud rate*/
	unsigned int ubrr = (clk/(BAUD*16L))-1;

	UBRR0H = (unsigned char)(ubrr >> 8);

	UBRR0L = (unsigned char)ubrr;

	/* Set frame format: 8data, 2stop bit*/
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00);

	/* Enable receiver and transmitter*/
	set_bit(UCSR0B,RXEN0);
	set_bit(UCSR0B,TXEN0);

	fdevopen(uart_transmit, uart_receive);


}

//.. comment needed
int uart_transmit(char data, FILE * _not_used){
	(void) _not_used;

	/* Wait for empty transmit buffer*/
	wait_until_bit_is_set(UCSR0A,UDRE0 );

	/* Put data into buffer, sends the data*/
	UDR0 = data;
}

int uart_receive(FILE * _not_used){
	(void) _not_used;
	/*Wait for data to be received*/
	wait_until_bit_is_clear(UCSR0A,RXC0 );

	/*Get and return received data from buffer*/
	return UDR0;
}



