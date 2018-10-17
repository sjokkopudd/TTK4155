#include "pinball_statemachine.h"
#include "oled_menu.h"
#include "joystick_driver.h"
#include "stdint.h"
#include "stdio.h"
#include "oled_driver.h"
#include "string.h"
#include "uart_driver.h"


/*#ifndef DEBUG
#define DEBUG
#endif*/

#define MINIMUM_DIFFICULTY 0
#define MAXIMUM_DIFFICULTY 9

//variable for the difficulty: 0 lowest and 9 highest
static uint8_t difficulty = 0;

static enStatePinball enCurrState;
static enMenuLeaf enMenuCurrLeaf;
static menu_t* currMenu;

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
//	eDIFF, 		
//	eSTART,
//	eSCORE,	

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
		//state menu 
		EvtNavigateBack, 
		EvtSelectMenuItem,
		EvtNavigateUp,
		EvtNavigateDown,
		DoNothing, 
		DoNothing,
		DoNothing,
		DoNothing
	},
	{
		//state diff
		EvtExitLeaf,
		DoNothing,
		EvtIncrementDiff, 
		EvtDecrementDiff,
		DoNothing, 
		DoNothing, //TODO -> for actually playing
		DoNothing, //TODO -> for actually playing
		DoNothing

	},
	{
		//state play
		EvtExitLeaf,
		DoNothing,
		DoNothing,
		DoNothing,
		DoNothing, 
		DoNothing,
		DoNothing,
		DoNothing
	},
	{
		//state score
		EvtExitLeaf,
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

	menuInit();
	//print menu to oled
	printMenu();

	return eMENU;
}

//-----------------------------------------------------
// navigate from selected menu item to parent menu item
// ----------------------------------------------------
enStatePinball EvtNavigateBack(){

	//call menu function to navigate back to parent
	menuNavigateBack(0);

	highlightMenu();

	return eMENU;

}

//-----------------------------------------------------
// select menu item
// ----------------------------------------------------
enStatePinball EvtSelectMenuItem(){
	
	enMenuLeaf currentLeaf = menuNavigationSelect();

	switch(currentLeaf){
		case eSET_DIFF: 
			printDifficulty();
			return eDIFF;
		case eSTART_GAME:
			return ePLAY;
		case eSEE_SCORE:
			return eSCORE;
		case eNOLEAF:
			return eMENU;
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


	return eMENU;

}

//-----------------------------------------------------
// navigate to lower menu item
// ----------------------------------------------------
enStatePinball EvtNavigateDown(){
	//call function of oled_menu to navigate down
	menuNavigateDown();

	//call function to highlight selected item in oled
	highlightMenu();

	return eMENU;
}

//TODO
enStatePinball EvtDoAnythingWithLeftBtn(){
	return ePLAY;
}

//TODO
enStatePinball EvtDoAnythingWithRightBtn(){
	return ePLAY;
}


// ---------------------------------------------
// quit game: go from state ePLAY to eMENU
// --------------------------------------------
enStatePinball EvtExitLeaf(void){
	//call navigation back with parameter 1 -> 
	//indicates that this is called from a leaf state
	//returning to underlying menu
	//for example: in state diff: Joystick left
	//prints menu: Play
	// 				 Difficulty
	//				 Start
	//
	menuNavigateBack(1);

	highlightMenu();

	return eMENU;

}



// ---------------------------------------------
// increment difficulty with Joystick up
// ---------------------------------------------
enStatePinball EvtIncrementDiff(void){

	if(difficulty < MAXIMUM_DIFFICULTY){
		difficulty++;
	}

	// update in oled_menu
	updateDifficulty(difficulty);

	return eDIFF;
}


// ---------------------------------------------
// decrement difficulty with Joystick down
// ---------------------------------------------
enStatePinball EvtDecrementDiff(void){
	if(difficulty > MINIMUM_DIFFICULTY){
		difficulty--;
	}

	// update in oled_menu
	updateDifficulty(difficulty);

	return eDIFF;
}


//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void InitPinballGame(){

	oled_init();

	oled_write_screen();

	enCurrState = eIDLE;
	
}

// -------------------------------------------------------
// calls function of joystick and button
// in order to detect joystick movement or button press
// ------------------------------------------------------


enStateEvent GetEvent(){
	static joy_direction_t lastDirection = eNEUTRAL;
	joy_direction_t currDirection;


	currDirection = joystick_get_direction();
	
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
	static uint8_t cnt = 0;
	enStateEvent currEvent;
	#ifndef DEBUG
		currEvent = GetEvent();
	#else
		currEvent = debug_events[cnt];
		cnt = (cnt +1)%5;
		//function call, pointed to by current state and event (matrix)
		printf("%s\n", testEventArray[currEvent]);
	#endif
	enCurrState = (* evtHndlTable[enCurrState][currEvent])();
	
	

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
