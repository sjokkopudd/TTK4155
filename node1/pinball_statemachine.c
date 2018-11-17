#define F_CPU 4915200
#include "pinball_statemachine.h"
#include "oled_menu.h"
#include "joystick_driver.h"
#include "stdint.h"
#include "stdio.h"
#include "oled_driver.h"
#include "string.h"
#include "uart_driver.h"
#include "can_communication.h"

#include "util/delay.h"


/*#ifndef DEBUG
#define DEBUG
#endif*/

#define MINIMUM_DIFFICULTY 0
#define MAXIMUM_DIFFICULTY 9

#define NUM_PLAYERS 4
#define LAST_PLAYER 3
#define FIRST_PLAYER 0

//variable for the difficulty: 0 lowest and 9 highest
static uint8_t difficulty = 0;
static uint16_t score = 0;

static uint16_t highScores[NUM_PLAYERS] = {0};

static uint8_t currLeader = 0;
static uint16_t currHighScore = 0;

static uint8_t player = 0;

static enStatePinball enCurrState;
static enMenuLeaf enMenuCurrLeaf;
static menu_t* currMenu;

//message buffer for can
static data_t* message;
static data_t* receive;

static void init_position(void);

//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);




// ---------------------------------------------------------------
// events for debugging
// 
//
// 1. nothing - start - play - diff - incr diff - incr diff
// static enStateEvent debug_events[]= {eJOYSTICK_LEFT, eJOYSTICK_RIGHT,eJOYSTICK_RIGHT,eJOYSTICK_UP, eJOYSTICK_UP};

// 2. 
static enStateEvent debug_events[]= {eJOYSTICK_RIGHT, eJOYSTICK_RIGHT,eJOYSTICK_LEFT,eJOYSTICK_UP, eJOYSTICK_UP};
static const char* testEventArray[] = {"eJOYSTICK_LEFT", "eJOYSTICK_RIGHT", "eJOYSTICK_UP", "eJOYSTICK_DOWN", "eJOYSTICK_NEUTRAL", "eBUTTON_LEFT", "eBUTTON_RIGHT", "eNO_EVENT"};


//--------------------------------------------------------
// state machine matrix 
// depending on current state each event that occurs different
// state machine functions are called 
// states:
//  eIDLE, 		//welcome screen on oled 
//	eMENU, 		//select menu points
//  ePLAYER,
//	eDIFF, 		
//	eSTART,
//	eSCORE,	





fPtr const evtHndlTable[][MAX_EVENTS] = {
	{
		//state idle
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,  
		evt_do_nothing, 
		evt_start_application,
		evt_do_nothing
	},
	{
		//state menu 
		evt_do_nothing, 
		evt_do_nothing,
		evt_navigate_up,
		evt_navigate_down,
		evt_do_nothing, 
		evt_navigate_back,
		evt_select_menu_item,
		evt_do_nothing
	},
	{
		//state player
		evt_do_nothing,
		evt_do_nothing,
		evt_decrement_player, 
		evt_increment_player,
		evt_do_nothing, 
		evt_exit_leaf, 
		evt_sel_player, 
		evt_do_nothing

	},
	{
		//state diff
		evt_do_nothing,
		evt_do_nothing,
		evt_increment_diff, 
		evt_decrement_diff,
		evt_do_nothing, 
		evt_exit_leaf, //TODO -> for actually playing
		evt_do_nothing, //TODO -> for actually playing
		evt_do_nothing

	},
	{
		//state play
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing, 
		evt_exit_leaf,
		evt_shoot,
		evt_control_game
	},
	{
		//state score
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing, 
		evt_exit_leaf,
		evt_do_nothing,
		evt_do_nothing
	}
};



//--------------------------------------------------------
// state machine functions
// -------------------------------------------------------

//-----------------------------------------------------
// DoNothing: returns current state
// ----------------------------------------------------
enStatePinball evt_do_nothing(){
	return enCurrState;
}

//-----------------------------------------------------
// navigate from start screen to menu
// ----------------------------------------------------
enStatePinball evt_start_application(){

	oled_menu_init();
	//print menu to oled
	oled_print_menu();

	return eMENU;
}

//-----------------------------------------------------
// navigate from selected menu item to parent menu item
// ----------------------------------------------------
enStatePinball evt_navigate_back(){

	//call menu function to navigate back to parent
	oled_menu_navigate_back(0);

	oled_highlight_menu();

	return eMENU;

}

// ------------------------------------------------------
// receives current score from node 2
// -----------------------------------------------------
static uint8_t handle_responses(){
	if(!can_receive_message(receive)){
		switch(receive->id){
			case eID_SCORE:
				score = receive->data[0] + (receive->data[1] << 8);
				oled_update_score(score);
				return 1;
				break;
			case eID_GAME_OVER: 
				print_game_over(score);
				_delay_ms(1000);

				if(score > highScores[player]){
					highScores[player] = score;
					//check if overall highScore
					if(score > currHighScore){
						//remember player 
						currLeader = player;
						currHighScore = score;
						print_high_score(score);

					}
				}
				_delay_ms(1000);

				 return 0;
				break;
			default: break;
		}

		

	}
	return 1;
}


// ----------------------------------------------------
// shoot function for solenoid
// ---------------------------------------------------
enStatePinball evt_shoot(){
	message->id = eID_BTN_RIGHT;
	message->length = 1;
	message->data[0] = 1;

	can_send_message(message);

	return ePLAY;

}
//-----------------------------------------------------
// controlling servo -> sending x position of joystick
// ----------------------------------------------------
enStatePinball evt_control_game(){
	//static uint8_t lstVal = 128;
	//(lstVal-x) > 5 || (x-lstVal)>5
	
	//getting current x position of joystick
	uint8_t x = joystick_get_x_pos();

	
	//lstVal = x;
	message->id = eID_JOY_X;
	message->length = 1;
	message->data[0] = x;

	can_send_message(message);
	
	
	//getting right slider position and sending it over the CAN bus

	uint8_t x_slider = get_slider_pos_right();

	message->id = eID_SLIDER_RIGHT;
	message->length = 1;
	message->data[0] = x_slider;

	can_send_message(message);

	_delay_ms(50);
	if(!handle_responses()){
		//game over has been received

		return evt_exit_leaf(); 
		//return eMENU;
		

		//
	}


	return ePLAY;

}

//-----------------------------------------------------
// select menu item
// ----------------------------------------------------
enStatePinball evt_select_menu_item(){
	
	enMenuLeaf currentLeaf = oled_menu_navigation_select();

	switch(currentLeaf){
		case eSEL_PLAYER:
			oled_print_players(player);
			oled_select_player(player);
			return ePLAYER;
		case eSET_DIFF: 
			oled_print_difficulty(difficulty);
			return eDIFF;
		case eSTART_GAME:
			init_position();
			oled_print_play_mode();
			return ePLAY;
		case eSEE_SCORE:
			print_best_players();
			return eSCORE;
		case eNOLEAF:
			return eMENU;
	}

}

//-----------------------------------------------------
// navigate to upper menu item
// ----------------------------------------------------
enStatePinball evt_navigate_up(){
	
	//call function of oled_menu to navigate up
	oled_menu_navigate_up();

	//call function to highlight
	oled_highlight_menu();


	return eMENU;

}

//-----------------------------------------------------
// navigate to lower menu item
// ----------------------------------------------------
enStatePinball evt_navigate_down(){
	//call function of oled_menu to navigate down
	oled_menu_navigate_down();

	//call function to highlight selected item in oled
	oled_highlight_menu();

	return eMENU;
}


// ---------------------------------------------
// quit game: go from state ePLAY to eMENU
// --------------------------------------------
enStatePinball evt_exit_leaf(void){
	//call navigation back with parameter 1 -> 
	//indicates that this is called from a leaf state
	//returning to underlying menu
	//for example: in state diff: Joystick left
	//prints menu: Play
	// 				 Difficulty
	//				 Start
	//
	oled_menu_navigate_back(1);

	oled_highlight_menu();

	return eMENU;

}


// ---------------------------------------------
// increment difficulty with Joystick up
// ---------------------------------------------
enStatePinball evt_increment_diff(void){

	if(difficulty < MAXIMUM_DIFFICULTY){
		difficulty++;
	}

	// update in oled_menu
	oled_update_difficulty(difficulty);

	return eDIFF;
}


// ---------------------------------------------
// decrement difficulty with Joystick down
// ---------------------------------------------
enStatePinball evt_decrement_diff(void){
	if(difficulty > MINIMUM_DIFFICULTY){
		difficulty--;
	}

	// update in oled_menu
	oled_update_difficulty(difficulty);

	return eDIFF;
}


enStatePinball evt_increment_player(void){
	if(player < LAST_PLAYER){
		player++;
	}
	// update in oled_menu
	oled_highlight_player(player);

	return ePLAYER;
}

enStatePinball evt_decrement_player(void){
	if(player > FIRST_PLAYER){
		player--;
	}
	// update in oled_menu
	oled_highlight_player(player);

	return ePLAYER;
}

enStatePinball evt_sel_player(void){
	oled_select_player(player);

	return ePLAYER;

}


//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void init_pinball_game(){

	oled_init();

	oled_write_screen();

	//send the postion via CAN to node 2
	message = malloc(sizeof(data_t));
	receive = malloc(sizeof(data_t));

	enCurrState = eIDLE;
	
}

// -------------------------------------------------------
// calls function of joystick and button
// in order to detect joystick movement or button press
// ------------------------------------------------------


enStateEvent get_event(){
	static joy_direction_t lastDirection = eNEUTRAL;
	static button_t lastBtn = eBTN_NO;

	joy_direction_t currDirection;
	button_t btn_input;


	currDirection = joystick_get_direction();
	btn_input = get_button();

	if(lastBtn != btn_input){
		lastBtn = btn_input;
		switch(btn_input){
			case eBTN_RIGHT:
				return eBUTTON_RIGHT;
			case eBTN_LEFT:
				return eBUTTON_LEFT;
		}
	}
	
	//do anything only if current directory is different from last
	if(lastDirection != currDirection){
		//set new direction 
		lastDirection = currDirection;
		switch(currDirection){
			case eLEFT:
				return eJOYSTICK_LEFT;
			case eRIGHT:
				return eJOYSTICK_RIGHT;
			case eUP:
				return eJOYSTICK_UP;
			case eDOWN:
				return eJOYSTICK_DOWN;
			case eNEUTRAL:
				return eJOYSTICK_NEUTRAL;
			
		}
	}


	return eNO_EVENT;

}


// ------------------------------------------------------
// main pinball processs, gets current event and according
// to state and event a state change is executed or not 
// -----------------------------------------------------
void pinball_game_process(void){
	static uint8_t cnt = 0;
	enStateEvent currEvent;
	#ifndef DEBUG
		currEvent = get_event();
	#else
		currEvent = debug_events[cnt];
		cnt = (cnt +1)%5;
		//function call, pointed to by current state and event (matrix)
	#endif
	enCurrState = (* evtHndlTable[enCurrState][currEvent])();
	_delay_ms(100);

	
	

}

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void print_high_score(uint16_t score){

	oled_print_high_score(score);

}
void print_animation(void* any/*any animation: enum*/){
	
}
void print_best_players(void){

	oled_print_best_players(highScores);
}

void print_init_screen(void){

}

void print_game_over(uint16_t score){

	oled_print_game_over(score, player);

}


void update_play_screen(){
	//print oled score

}

// ------------------------------------------------------
// sends initial start values for servo and motor
// ------------------------------------------------------
static void init_position(void){
	uint8_t x_pos = 0;
	uint8_t x_slider_pos = 0;

	message->id = eID_JOY_X;
	message->length = 1;
	message->data[0] = x_pos;

	can_send_message(message);

	_delay_ms(50);

	message->id = eID_SLIDER_RIGHT;
	message->length = 1;
	message->data[0] = x_slider_pos;

	can_send_message(message);

	_delay_ms(50);

	//reset scores 
	message->id = eID_START;
	can_send_message(message);

	_delay_ms(50);

}

