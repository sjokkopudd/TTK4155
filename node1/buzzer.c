#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include "pwm_music.h"
#include "music.h"

uint16_t song, note;
uint16_t beat;
uint16_t whole_note, half_note, quarter_note, eight_note, sixteenth_note, triplets_note;
uint16_t three_sixteenths, three_eights;
//uint8_t music_on, game_song;

void BUZZER_init(void) {
	//Normal port operation, CTC mode, OCR3A as TOP

	clear_bit(TCCR3A, COM3A1);
	clear_bit(TCCR3A, COM3A0);
	clear_bit(TCCR3A, COM30);
	clear_bit(TCCR3A, WGM31);
	clear_bit(TCCR3A, WGM33);
	set_bit(TCCR3A, WGM32);
	

	//Set the compare match interrupt enable
	set_bit(ETIMSK, OCIE3A);
	
	init_pwm();
	
	//music_on = 1;
}

void BUZZER_start(uint8_t start) {
	if (start == 1) {
		//Start the PWM
		pwm_sound(1);
		//music_on = 1;
		//Set the prescaler (clk/256)
		set_bit(TCCR3B,CS32);
		clear_bit(TCCR3B, CS31);
		clear_bit(TCCR3B, CS30);
		} else {
		//Stop the PWM
		pwm_sound(0);
		//music_on = 0;
		//Stop the timer
		clear_bit(TCCR3B, CS32);
		clear_bit(TCCR3B, CS31);
		clear_bit(TCCR3B, CS30);
	}
}

void BUZZER_set_tempo(uint8_t tempo) {
	//Quarter note in seconds
	beat = (60*4915200)/((uint32_t)tempo*256);
	
	//Duration of notes in counts
	whole_note = beat*4;
	half_note = beat*2;
	quarter_note = beat;
	eight_note = beat/2;
	sixteenth_note = beat/4;
	triplets_note = beat*3/8;
	three_sixteenths = eight_note + sixteenth_note;
	three_eights = quarter_note + eight_note;
}

void BUZZER_play_song(uint8_t song_choice) {
	
		//if (music_on == 1) {
		//BUZZER_start(0);
		
		//Set tempo and song
		if (song_choice == 0) {
			BUZZER_set_tempo(80);
			song = 0;	
		} 
		else if (song_choice == 1) {
			BUZZER_set_tempo(120);
			song = 1;	
		}
		else if (song_choice == 2) {
			BUZZER_set_tempo(190);
			song = 2;	
		}
		else if (song_choice == 3) {
			BUZZER_set_tempo(152);
			song = 3;	
		}
		else if (song_choice == 4) {
			BUZZER_set_tempo(180);
			song = 4;	
		}
		else if (song_choice == 5) {
			song = 5;	
		}
		/*else if (strcmp(screen_name, "game") == 0) {
			switch (game_song) {
				case 1:
					BUZZER_set_tempo(120);
					break;
				case 2:
					BUZZER_set_tempo(190);
					break;
				case 3:
					BUZZER_set_tempo(152);
					break;
			}
			song = game_song;
		}  else if (strcmp(screen_name, "gameover") == 0) {
			BUZZER_set_tempo(180);
			song = 4;
		}  else if (strcmp(screen_name, "none") == 0) {
			song = 5;
		}*/
		note = 0;
		OCR3A = 1;
		
		BUZZER_start(1);	
}

void BUZZER_play_note(float note) {
	float note_ms;
	unsigned int count;
	
	if (note > 0) {
		note_ms = 1/note;
		count = (note_ms*4915200)/16;
	} 
	else {
		count = 0;
	}
	
	pwm_play_note(count);
}

void BUZZER_choose_game_song(uint8_t song_on) {
	game_song = song_on;
}

//Interrupt service routine for the Timer 3
ISR(TIMER3_COMPA_vect) {
	//Play note and set the delay
	if (song != 5) {
		BUZZER_play_note(pgm_read_float(&notes[song][note]));
		
		switch (pgm_read_byte(&notes_lengths[song][note])) {
			case 0:
			OCR3A = whole_note;
			break;
			case 1:
			OCR3A = half_note;
			break;
			case 2:
			OCR3A = quarter_note;
			break;
			case 3:
			OCR3A = eight_note;
			break;
			case 4:
			OCR3A = sixteenth_note;
			break;
			case 5:
			OCR3A = triplets_note;
			break;
			case 6:
			OCR3A = three_sixteenths;
			break;
			case 7:
			OCR3A = three_eights;
			break;
		}
		switch (song) {
			case 0:
			note = (note == 71) ? 0 : note+1;
			break;
			case 1:
			note = (note == 60) ? 0 : note+1;
			break;
			case 2:
			note = (note == 61) ? 0 : note+1;
			break;
			case 3:
			note = (note == 150) ? 0 : note+1;
			break;
			case 4:
			note = (note == 105) ? 0 : note+1;
			break;
		}
	} 
	else {
		BUZZER_play_note(0);
		OCR3A = whole_note;
	}
}