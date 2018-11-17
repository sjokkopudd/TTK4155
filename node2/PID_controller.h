#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>

void PID_init(void);
void PID_update_reference(uint8_t pos);
void PID_controller();

#endif