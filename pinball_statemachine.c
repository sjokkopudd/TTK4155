#include "pinball_statemachine.h"
#include "oled_menu.h"
#include "joystick_driver.h"
#include "stdint.h"
#include "stdio.h"
#include "oled_driver.h"
#include "string.h"
#include "uart_driver.h"

static enStatePinball enCurrState;
static enMenuSel enMenuSelected;
static menu_t* currMenu;

//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);


//--------------------------------------------------------
// state machine matrix 
// depending on current state each event that occurs different
// state machine functions are called 
//--------------------------------------------------------
fPtr const evtHndlTable[][MAX_EVENTS] = {
	{
		//state idle
		DoNothing,
		EvtStartApplication,
		DoNothing,
		DoNothing,
		DoNothing,  
		DoNothing, 
		DoNothing,
		DoNothing
	},
	{
		//state menu select
		DoNothing, 
		EvtSelectMenuItem,
		EvtNavigateUp,
		EvtNavigateDown,
		DoNothing, 
		DoNothing,
		DoNothing,
		DoNothing
	},
	{
		//state play
		EvtNavigateBack,
		DoNothing,
		DoNothing, 
		DoNothing,
		DoNothing, 
		DoNothing, //TODO -> for actually playing
		DoNothing, //TODO -> for actually playing
		DoNothing

	},
	{
		//state score
		EvtNavigateBack,
		DoNothing,
		DoNothing,
		DoNothing,
		DoNothing, 
		DoNothing,
		DoNothing,
		DoNothing
	}
};



//--------------------------------------------------------
// state machine functions
// -------------------------------------------------------

//-----------------------------------------------------
// DoNothing: returns current state
// ----------------------------------------------------
enStatePinball DoNothing(){
	return enCurrState;
}

//-----------------------------------------------------
// navigate from start screen to menu
// ----------------------------------------------------
enStatePinball EvtStartApplication(){
	stdout = &uart_stream;
	printf("event start app");


	menuInit();
	//print menu to oled
	printMenu();

	stdout = &uart_stream;
	printf("after print menu");

	return eSELECT;
}

//-----------------------------------------------------
// navigate from selected menu item to parent menu item
// ----------------------------------------------------
enStatePinball EvtNavigateBack(){
	//as only one hierarchy is implemented go back to main menu
	enMenuSelected = eMENU_MAIN;

	printMenu();

	return eSELECT;

}

//-----------------------------------------------------
// select menu item
// ----------------------------------------------------
enStatePinball EvtSelectMenuItem(){
	
	menuNavigationSelect();

	switch(enMenuSelected){
		case eMENU_PLAY: 
			return ePLAY;
		case eMENU_SCORE:
			return eSCORE;
		default:
			return eMENU_MAIN;
	}

}

//-----------------------------------------------------
// navigate to upper menu item
// ----------------------------------------------------
enStatePinball EvtNavigateUp(){
	
	//call function of oled_menu to navigate up
	menuNavigateUp();

	//call function to highlight
	highlightMenu();

	return eSELECT;
	/*if(enMenuSelected == eMENU_MAIN){
		enMenuSelected = eMENU_MAIN;
	}
	else{
		enMenuSelected--;
	}
	//call function to highlight selected item in oled
	highlightMenu();

	return eSELECT*/

}

//-----------------------------------------------------
// navigate to lower menu item
// ----------------------------------------------------
enStatePinball EvtNavigateDown(){
	//call function of oled_menu to navigate down
	menuNavigateDown();

	//call function to highlight selected item in oled
	highlightMenu();

	return eSELECT;
}

//TODO
enStatePinball EvtDoAnythingWithLeftBtn(){
	return ePLAY;
}

//TODO
enStatePinball EvtDoAnythingWithRightBtn(){
	return ePLAY;
}

//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void InitPinballGame(){

	enCurrState = eIDLE;
	
}

// -------------------------------------------------------
// calls function of joystick and button
// in order to detect joystick movement or button press
// ------------------------------------------------------
enStateEvent GetEvent(){
	//remember last joystick direction
	static joy_direction_t lastDirection = eNEUTRAL;
	joy_direction_t currDirection = joystick_get_direction();
	

	//debug
	stdout = &uart_stream;
	
	

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
void PinballGameProcess(void){
	enStateEvent currEvent;
	currEvent = GetEvent();
	//function call, pointed to by current state and event (matrix)
	enCurrState = (* evtHndlTable[enCurrState][currEvent])();

	stdout = &uart_stream;
	printf("current state %d\r\n: ", enCurrState);

}

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void printHighScore(uint16_t score){


}
void printAnimation(void* any/*any animation: enum*/){
	
}
void printBestPlayers(void){

}

void printInitScreen(void){

}

