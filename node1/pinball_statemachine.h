/*



*/

#ifndef PINBALL_STATEMACHINE_H
#define PINBALL_STATEMACHINE_H

#include <stdint.h>

#define MAX_EVENTS 8

// --------------------------------------------------------
// enum type: states of pinball game 
// --------------------------------------------------------
typedef enum enStatePinball {
	eIDLE, 		//welcome screen on oled 
	eMENU, 	    //select menu points
	ePLAYER,	//select player
	eSETTING, 		//play game
	ePLAY,
	eSCORE,	//display highscores
} enStatePinball;

// --------------------------------------------------------
// enum type: events of pinball game 
// --------------------------------------------------------
typedef enum enStateEvent {
	eJOYSTICK_LEFT,			/* "back" navigation of a selected menu item */
	eJOYSTICK_RIGHT,		/* select menu item */
	eJOYSTICK_UP,			/* navigate up in menu  */
	eJOYSTICK_DOWN,			/* navigate down in menu */
	eJOYSTICK_NEUTRAL,
	eBUTTON_LEFT,			/* */
	eBUTTON_RIGHT,			/* */
	eNO_EVENT				/* no interaction */

} enStateEvent;

//--------------------------------------------------------
// typedef function pointer for pinball game
// -------------------------------------------------------
typedef enStatePinball (*fPtr)(void);


//--------------------------------------------------------
// state machine functions
// -------------------------------------------------------
enStatePinball evt_do_nothing(void);
enStatePinball evt_start_application(void);
enStatePinball evt_navigate_back(void);
enStatePinball evt_select_menu_item(void);
enStatePinball evt_navigate_up(void);
enStatePinball evt_navigate_down(void);
enStatePinball evt_exit_leaf(void);
enStatePinball evt_increment_sel(void);
enStatePinball evt_decrement_sel(void);
enStatePinball evt_increment_player(void);
enStatePinball evt_decrement_player(void);
enStatePinball evt_sel_player(void);
enStatePinball evt_control_game(void);
enStatePinball evt_shoot(void);
enStatePinball evt_exit_play(void);
enStatePinball evt_sel_reset(void);
//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void init_pinball_game(void);
enStateEvent get_event(void);
void pinball_game_process(void);

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void print_high_score(uint16_t score);
void print_game_over(uint16_t score);
void print_best_players(void);
void fetch_scores(void);



#endif