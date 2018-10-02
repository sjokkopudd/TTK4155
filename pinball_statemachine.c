#include "pinball_statemachine.h"
#include "oled_menu.h"
#include "joystick_driver.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

static enStatePinball enCurrState;
static enMenuSel enMenuSelected;
static menu_t* currMenu;


//--------------------------------------------------------
// state machine matrix 
// depending on current state each event that occurs different
// state machine functions are called 
//--------------------------------------------------------
fPtr const evtHndlTable [][MAX_EVENTS] = {
	{
		//state idle
		DoNothing,
		EvtStartApplication,
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
		DoNothing
	},
	{
		//state play
		EvtNavigateBack,
		DoNothing,
		DoNothing, 
		DoNothing,
		DoNothing, //TODO -> for actually playing
		DoNothing, //TODO -> for actually playing
		DoNothing

	}
	{
		//state score
		EvtNavigateBack,
		DoNothing,
		DoNothing,
		DoNothing,
		DoNothing,
		DoNothing,
		DoNothing
	}
};



/--------------------------------------------------------
// state machine functions
// -------------------------------------------------------

//-----------------------------------------------------
// DoNothing: returns current state
// ----------------------------------------------------
enStatePinball DoNothing(void*){
	return enCurrState;
}

//-----------------------------------------------------
// navigate from start screen to menu
// ----------------------------------------------------
enStatePinball EvtStartApplication(void*){
	menuInit();
	//print menu to oled
	printMenu();

	return eSELECT;
}

//-----------------------------------------------------
// navigate from selected menu item to parent menu item
// ----------------------------------------------------
enStatePinball EvtNavigateBack(void*){
	//as only one hierarchy is implemented go back to main menu
	enMenuSelected = eMENU_MAIN;

	printMenu();

	return eSELECT;

}

//-----------------------------------------------------
// select menu item
// ----------------------------------------------------
enStatePinball EvtSelectMenuItem(void*){
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
enStatePinball EvtNavigateUp(void*){
	
	if(enMenuSelected == eMENU_MAIN){
		enMenuSelected = eMENU_MAIN;
	}
	else{
		enMenuSelected--;
	}
	//call function to highlight selected item in oled
	highlightMenu();

	return eSELECT;

}

//-----------------------------------------------------
// navigate to lower menu item
// ----------------------------------------------------
enStatePinball EvtNavigateDown(void*){
	if(enMenuSelected == eMENU_SCORE){
		enMenuSelected = eMENU_SCORE;
	}
	else{
		enMenuSelected++;
	}
	//call function to highlight selected item in oled
	highlightMenu();

	return eSELECT;
}

//TODO
enStatePinball EvtDoAnythingWithLeftBtn(void*){
	return ePLAY;
}

//TODO
enStatePinball EvtDoAnythingWithRightBtn(void*){
	return ePLAY;
}

//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void InitPinballGame(void){

	enCurrState = eIDLE;
	enMenuSel = eMENU_MAIN;
}
enStateEvent GetEvent(void);
void PinballGameProcess(void);

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void printHighScore(uint16_t score){


}
void printAnimation(void*/*any animation: enum*/){
	
}
void printBestPlayers(void){

}
void printMenu(void){
	//call print function of menu
	swi

}
void printInitScreen(void);
void highlightMenu(void);
