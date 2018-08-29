#ifndef UART_DRIVER
#define UART_DRIVER


void UART_Init(unsigned long clk);

void UART_Transmit(unsigned char data);

unsigned char UART_Receive(void);

#endif
