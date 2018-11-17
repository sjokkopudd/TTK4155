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

typedef enum enMsgId
{
	eID_JOY_X = 0,
	eID_JOY_Y = 1,
	eID_BTN_RIGHT = 2,
	eID_BTN_LEFT = 3,
	eID_SLIDER_RIGHT = 4,
	eID_SLIDER_LEFT = 5, 
	eID_SCORE = 6,
	eID_START = 7,
	eID_GAME_OVER = 8,
	eID_EXIT_GAME = 9
}enMsgId;

void can_init();

uint8_t can_send_message(data_t* message);

int can_receive_message(data_t* message_to_receive);



#endif