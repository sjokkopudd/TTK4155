#pragma once

/*------------------------------------------------------
* uart_driver.h 
* functions for init, transmit and receive data with uart
------------------------------------------------------*/

void uart_init(unsigned long clk);

void uart_transmit(unsigned char data);

unsigned char uart_receive(void);

