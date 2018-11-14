#define F_CPU 4915200

#include "pwm_music.h"
#include "common.h"
#include "music.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void init_pwm(){


  //set mode 14: Fast PWM - writing to control registers
  set_bit(TCCR1A,WGM11);
  set_bit(TCCR1A, WGM10); 
  //clear_bit(TCCR1A, WGM10); 
  set_bit(TCCR1B, WGM13);
  set_bit(TCCR1B, WGM12);

  //set output mode non-inverting
  clear_bit(TCCR1A,COM1A1);
  //set_bit(TCCR1A,COM1A1);
  set_bit(TCCR1A,COM1A0);

  //enable output direction of pin 
  set_bit(DDRD, PD5);

  //set ICR1 5000 = 20 ms period (TOP)
  ICR1 = 2000;
}

void pwm_start(uint8_t start){
  if (start == 1) {
    //set prescaler to fcpu/8
    set_bit(TCCR1B, CS11);
    clear_bit(TCCR1B, CS12);
    clear_bit(TCCR1B, CS10);
  }
  else{
    //Stop
    clear_bit(TCCR1B, CS12);
    clear_bit(TCCR1B, CS11);
    clear_bit(TCCR1B, CS10);
  }
}


void pwm_play_note(unsigned int count){
  if (count > 0){
    pwm_start(0);
    _delay_ms(20);
    OCR1A = count;
    pwm_start(1);
  }
  else{
    pwm_start(0);
  }
}

void button_sound(){
  for (int i = 0; i < 10; i++) {
    pwm_play_note(3000);
  }
  pwm_start(0);
}