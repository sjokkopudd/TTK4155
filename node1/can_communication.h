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

typedef struct game_msg_node1_t
{
	uint8_t game_start;
	uint8_t game_exit;
	uint8_t game_shoot;
	uint8_t game_joy;
	uint8_t game_slider;
}game_msg_node1_t;

typedef struct game_msg_node2_t
{
	uint8_t game_over;
	uint16_t game_score;	
}game_msg_node2_t;


typedef enum enMsgId
{
	eID_NODE1 = 0,
	eID_NODE2 = 1
}enMsgId;


void can_init();

uint8_t can_send_message(data_t* message);

int can_receive_message(data_t* message_to_receive);



#endif