#ifndef PWM_MUSIC_H
#define PWM_MUSIC_H

#include <stdint.h>

void init_pwm();
void pwm_start(uint8_t start);
void pwm_play_note(unsigned int count);
void button_sound();

#endif 