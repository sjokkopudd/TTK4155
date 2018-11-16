#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>

void PID_init(void);
void PID_update_reference(int16_t pos);


#endif