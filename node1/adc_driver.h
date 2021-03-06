#pragma once

#include <stdint.h>


typedef enum{
    JOYSTICK_X       = 4,
    JOYSTICK_Y       = 5,
    SLIDER_LEFT    = 6,
    SLIDER_RIGHT   = 7
} adc_channel_t;


void adc_init(void);


uint8_t adc_read_channel(adc_channel_t CHANNEL);

