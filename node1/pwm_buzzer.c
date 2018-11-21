#define F_CPU 4915200

#include "pwm_buzzer.h"
#include "common.h"
#include "notes.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>



void init_pwm(){


  //set mode 14: Fast PWM - writing to control registers
  set_bit(TCCR1A,WGM11);
  clear_bit(TCCR1A, WGM10); 
  set_bit(TCCR1B, WGM13);
  set_bit(TCCR1B, WGM12);

  //set output mode non-inverting
  set_bit(TCCR1A,COM1B1);
  clear_bit(TCCR1A,COM1B0);

  //enable output direction of pin 
  set_bit(DDRE, PE2);

  set_bit(TCCR1B, CS11);
  clear_bit(TCCR1B, CS12);
  clear_bit(TCCR1B, CS10);

  sei();
}

void pwm_play_note(double note){
  ICR1 = (F_CPU)/(note*16);
}

void buzzer_play_game_over(){
   //set volume
  OCR1B = 3;
  pwm_play_note(Gb5);
  _delay_ms(500);
  pwm_play_note(F5);
  _delay_ms(500);
  pwm_play_note(E5);
  _delay_ms(500);
  pwm_play_note(Eb5);
  _delay_ms(1500);
  pwm_play_note(0);
}

void buzzer_play_highscore(){
   //set volume
  OCR1B = 7;
  pwm_play_note(C5);
  _delay_ms(80);
  pwm_play_note(0);
  _delay_ms(80);
  pwm_play_note(C5);
  _delay_ms(80);
  pwm_play_note(0);
  _delay_ms(80);
  pwm_play_note(C5);
  _delay_ms(80);
  pwm_play_note(0);
  _delay_ms(80);
  pwm_play_note(C5);
  _delay_ms(480);
  pwm_play_note(Gs5);
  _delay_ms(480);
  pwm_play_note(As5);
  _delay_ms(480);
  pwm_play_note(C5);
  _delay_ms(160);
  pwm_play_note(0);
  _delay_ms(160);
  pwm_play_note(As5);
  _delay_ms(160);
  pwm_play_note(C5);
  _delay_ms(1440);
  pwm_play_note(0);
  
}

void buzzer_button_press(){
   //set volume
  OCR1B = 2;
  pwm_play_note(B5);
  _delay_ms(100);
  pwm_play_note(0);
}