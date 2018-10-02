/*



*/

#ifndef PINBALL_STATEMACHINE_H
#define PINBALL_STATEMACHINE_H

#include <stdint.h>

#define MAX_EVENTS 7

// --------------------------------------------------------
// enum type: states of pinball game 
// --------------------------------------------------------
typedef enum enStatePinball {
	eIDLE, 		//welcome screen on oled 
	eSELECT, 	//select menu points
	ePLAY, 		//play game
	eSCORE		//display highscores
} enStatePinball;

// --------------------------------------------------------
// enum type: events of pinball game 
// --------------------------------------------------------
typedef enum enStateEvent {
	eJOYSTICK_LEFT,			/* "back" navigation of a selected menu item */
	eJOYSTICK_RIGHT,		/* select menu item */
	eJOYSTICK_UP,			/* navigate up in menu  */
	eJOYSTICK_DOWN,			/* navigate down in menu */
	eBUTTON_LEFT,			/* */
	eBUTTON_RIGHT,			/* */
	eNO_EVENT				/* no interaction */

} enStateEvent;

//--------------------------------------------------------
// typedef function pointer for pinball game
// -------------------------------------------------------
typedef enStatePinball (*fPtr)(void*);


//--------------------------------------------------------
// state machine functions
// -------------------------------------------------------
enStatePinball DoNothing(void*);
enStatePinball EvtStartApplication(void*);
enStatePinball EvtNavigateBack(void*);
enStatePinball EvtSelectMenuItem(void*);
enStatePinball EvtNavigateUp(void*);
enStatePinball EvtNavigateDown(void*);
enStatePinball EvtDoAnythingWithLeftBtn(void*);
enStatePinball EvtDoAnythingWithRightBtn(void*);

//--------------------------------------------------------
// pinball game functions
// -------------------------------------------------------
void InitPinballGame(void);
enStateEvent GetEvent(void);
void PinballGameProcess(void);

// ------------------------------------------------------
//print functions to oled
// ------------------------------------------------------
void printHighScore(uint16_t score);
void printAnimation(void*/*any animation: enum*/);
void printBestPlayers(void);
void printMenu(void); 
void printInitScreen(void);
void highlightScreenLine(void);



#endif