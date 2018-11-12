#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

typedef enum collision_state
{
	eNo_Coll,
	eDebounce_Coll
	
}collision_state;

void init_game_controller();
void process_game();




#endif