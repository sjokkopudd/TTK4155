#include "SPI_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "common.h"

void SPI_send(char data){

	clear_bit(PORTB, PB4);

	/* Start transmission */
	SPDR = data;
	
	/* Wait for transmission complete */
	loop_until_bit_is_set(SPSR,SPIF);


}


uint8_t SPI_read(){
	// Remember that to read something from the slave, the master must transmit a dummy byte
	SPDR = 0x00;

	//receive
	/* Wait for reception complete */
	loop_until_bit_is_set(SPSR, SPIF);

	return SPDR;
} 



void SPI_init(){

	/* Set SS, MOSI and SCK output, all others input */
	DDRB = (1<<PB4)|(1<<PB5)|(1<<PB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)/*|(1<<DORD)*/;

	//set SS high
	set_bit(PORTB, PB4);
	
}