#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "common.h"

#define START_ADDR 0

uint8_t eeprom_read_byte(uint16_t address) {

	cli();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address register */
	EEAR = address;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */

	sei();
	return EEDR;

}

void eeprom_write_byte(uint16_t address, uint8_t data){

	cli();
	/* 	Wait for completion of previous write*/
	while(EECR & (1<<EEWE))
	;
	/* Set up address and data registers */
	EEAR = address;
	EEDR = data;

	/* 	Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);

	sei();
}

uint16_t get_player_score(int player){
	uint8_t low = eeprom_read_byte(player*2);
	uint8_t high = eeprom_read_byte(player*2 + 1);

	uint16_t result = (high << 8) | low;
	return result;
}

void save_player_score(int player, uint16_t score){
	uint8_t high = (score >> 8);
	uint8_t low = (uint8_t) score;
	eeprom_write_byte(player*2, low);
	eeprom_write_byte(player*2 + 1, high);
}

void reset_eeprom(){
	for (int i = START_ADDR; i < (START_ADDR + 8); i++){
		eeprom_write_byte(i, 0);
	}
}