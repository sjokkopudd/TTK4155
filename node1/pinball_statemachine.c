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
#include "eeprom.h"
#include "pwm_buzzer.h"
#include "sprite_timer.h"
#include "util/delay.h"


#define NUM_PLAYERS 4
#define LAST_PLAYER 3
#define FIRST_PLAYER 0

//variable for the difficulty: 0 lowest and 9 highest
static uint8_t reset_high_score = 0;
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

static game_msg_node1_t curr_msg_node1 = {0};
static game_msg_node2_t curr_msg_node2 = {0};

static game_msg_node1_t msg_shoot = {0,0,1,0,0};
static game_msg_node1_t msg_start = {1,0,0,0,0};
static game_msg_node1_t msg_exit = {0,1,0,0,0};
static game_msg_node1_t msg_data = {0,0,0,0,0};


static void init_position(void);

//only for debug
//static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);




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
		//state setting
		evt_do_nothing,
		evt_do_nothing,
		evt_decrement_sel, 
		evt_increment_sel,
		evt_do_nothing, 
		evt_exit_leaf, //TODO -> for actually playing
		evt_sel_reset, //TODO -> for actually playing
		evt_do_nothing

	},
	{
		//state play
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing,
		evt_do_nothing, 
		evt_exit_play,
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

	//initialize the scores by fetching them from eeprom
	fetch_scores();
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
		//read data from receive buffer
		curr_msg_node2.game_over = receive->data[0];
		curr_msg_node2.game_score  = receive->data[1] | (receive->data[2] << 8);

		score = curr_msg_node2.game_score;

		//check if game over is set
		if(curr_msg_node2.game_over == 1){
			print_game_over(score);
			buzzer_play_game_over();
			_delay_ms(1000);

			if(score > highScores[player]){
				save_player_score(player, score);
				highScores[player] = score;
				//check if overall highScore
				if(score > currHighScore){
					//remember player 
					currLeader = player;
					currHighScore = score;
					print_high_score(score);
					sprite_timer_init();
										
					buzzer_play_highscore();
					_delay_ms(1000);
					disable_sprite_timer();

				}
			}
			
			return 0;

		}

		else{
			//update oled with current score
			oled_update_score(score);
		}
	}

	return 1;
}


// ----------------------------------------------------
// shoot function for solenoid
// ---------------------------------------------------
enStatePinball evt_shoot(){

	//update current message to 
	curr_msg_node1 = msg_shoot;

	return ePLAY;

}


//-----------------------------------------------------
// controlling servo -> sending x position of joystick
// ----------------------------------------------------
enStatePinball evt_control_game(){
	
	//getting current x position of joystick
	uint8_t x = joystick_get_x_pos();
	
	//getting right slider position 

	uint8_t x_slider = get_slider_pos_right();

	//update current message
	curr_msg_node1 = msg_data;
	curr_msg_node1.game_joy = x;
	curr_msg_node1.game_slider = x_slider;


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

		case eSET_SETTING: 
			oled_print_reset_high_scores(reset_high_score);
			return eSETTING;

		case eSTART_GAME:
			oled_reset_score();

			_delay_ms(70);
			while(!can_receive_message(receive));


			//reset score
			score = 0;

			curr_msg_node2.game_score = 0;

			//change current message to start 
			curr_msg_node1 = msg_start;
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

	oled_menu_navigate_back(1);

	oled_highlight_menu();

	return eMENU;
}


// ---------------------------------------------
// quit game: go from state ePLAY to eMENU
// --------------------------------------------
enStatePinball evt_exit_play(void){
	//send current message to exit game
	curr_msg_node1 = msg_exit;
	
	return ePLAY;

}

// ---------------------------------------------
// increment difficulty with Joystick up
// ---------------------------------------------
enStatePinball evt_increment_sel(void){

	if(!reset_high_score){
		reset_high_score = 1;
	}

	// update in oled_menu
	oled_update_reset_high_scores(reset_high_score);

	return eSETTING;
}


// ---------------------------------------------
// decrement difficulty with Joystick down
// ---------------------------------------------
enStatePinball evt_decrement_sel(void){
	if(reset_high_score){
		reset_high_score = 0;
	}

	// update in oled_menu
	oled_update_reset_high_scores(reset_high_score);

	return eSETTING;
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
				if (enCurrState != ePLAY){
					buzzer_button_press();
				}
				return eBUTTON_RIGHT;
			case eBTN_LEFT:
				buzzer_button_press();
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
	currEvent = get_event();
	//function call, pointed to by current state and event (matrix)
	enCurrState = (* evtHndlTable[enCurrState][currEvent])();

	if(enCurrState == ePLAY){
		//send can messages to node2
		message->id = eID_NODE1;
		message->length = 5;
		message->data[0] = curr_msg_node1.game_start;
		message->data[1] = curr_msg_node1.game_exit;
		message->data[2] = curr_msg_node1.game_shoot;
		message->data[3] = curr_msg_node1.game_joy;
		message->data[4] = curr_msg_node1.game_slider;


		can_send_message(message);

		_delay_ms(50);

		if(curr_msg_node1.game_exit){
			score = 0;
			curr_msg_node2.game_score = 0;

			enCurrState = evt_exit_leaf();

		}

		//check incoming can messages
		else if(!handle_responses()){
			//game over has been received
			//update current state as it is now the menu state instead of the play state
			enCurrState = evt_exit_leaf();	
		}
}
}

enStatePinball evt_sel_reset(void){
	//immediately return to state menu
	if(reset_high_score == 1){
		//reset high scores
		reset_eeprom();
		fetch_scores();
	}

	reset_high_score = 0;


	return evt_exit_leaf();

}

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void print_high_score(uint16_t score){

	oled_print_high_score(score);
}

void print_best_players(void){

	oled_print_best_players(highScores, NUM_PLAYERS);
}

void print_game_over(uint16_t score){

	oled_print_game_over(score, player);

}

void fetch_scores(){
	currHighScore = 0;
	for (int i = 0; i < 4; i++){
		highScores[i] = get_player_score(i);
		if(highScores[i] > currHighScore){
			currHighScore = highScores[i];
			currLeader = i;
		}
	}
}
