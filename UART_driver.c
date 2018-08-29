#include "UART_driver.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#define BAUD 9600



void UART_Transmit(unsigned char data ){
	/* Wait for empty transmit buffer*/
	while (!(UCSR0A & (1<<UDRE0)));

	/* Put data into buffer, sends the data*/
	UDR0 = data;
}

unsigned char UART_Receive(void)
{
	/*Wait for data to be received*/
	while (!(UCSR0A & (1<<RXC0)));

	/*Get and return received data from buffer*/
	return UDR0;
}

void UART_Init(unsigned long clk) {

	/*Set baud rate*/
	unsigned int ubrr = 31;//(clk/(BAUD*16L))-1;

	UBRR0H = (unsigned char)(ubrr >> 8);

	UBRR0L = (unsigned char)ubrr;

	/* Set frame format: 8data, 2stop bit*/
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00);

	/* Enable receiver and transmitter*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	fdevopen(UART_Transmit, UART_Receive);

}


