#ifndef CAN_COMMUNICATION_H
#define CAN_COMMUNICATION_H

#include <stdint.h>

#define CAN_BUFFER_SIZE 8

typedef struct data_t
{
	uint16_t id;
	uint8_t length;
	uint8_t data[CAN_BUFFER_SIZE];

}data_t;

void can_init();

uint8_t can_send_message(data_t* message);

data_t* can_receive_message();



#endif