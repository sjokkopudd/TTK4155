#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>


void SPI_send(char data);
uint8_t SPI_read(void);
void SPI_init(void);



#endif