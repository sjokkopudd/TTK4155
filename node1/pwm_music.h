#ifndef PWM_MUSIC_H
#define PWM_MUSIC_H

void init_pwm();
void pwm_start();
void pwm_play_note(unsigned int count);
void button_sound();