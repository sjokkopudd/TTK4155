#ifndef PWM_MUSIC_H
#define PWM_MUSIC_H

#include <stdint.h>

void init_pwm();
void pwm_play_note(double note);
void buzzer_play_game_over();
void buzzer_play_highscore();
void buzzer_button_press();
#endif 