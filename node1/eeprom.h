#ifndef EEPROM_H
#define EEPROM_H

	uint8_t eeprom_read_byte(uint16_t address);
	void eeprom_write_byte(uint16_t address, uint8_t data);
	void reset_eeprom();
	uint16_t get_player_score(int player);
	void save_player_score(int player, uint16_t score);

#endif