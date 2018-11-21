#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdlib.h>
#include <stdio.h>
/*------------------------------------------------------
* uart_driver.h 
* functions for init, transmit and receive data with uart
------------------------------------------------------*/

void uart_init(unsigned long clk);

int uart_transmit(char data, FILE * _not_used);

int uart_receive(FILE * _not_used);

#endif