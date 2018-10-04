#ifndef OLED_MENU_H
#define OLED_MENU_H

#include <stdint.h>



 typedef struct menu
{
	const char* name;
	struct menu* parent;
	struct menu* child;
	struct menu* sibling;
	
}menu_t;

typedef struct currMenuSelected
{
	uint8_t row;
	menu_t* menuItem;

}currMenuSelected_t;




// ---------------------------------
// common typedef for menu leafes in order to change state
// --------------------------------
typedef enum enMenuLeaf
{
	eSET_DIFF,
	eSTART_GAME,
	eSEE_SCORE,
	eNOLEAF


}enMenuLeaf;

//-------------------------------------------------------
// functions that are called from pinball_statemachine
//------------------------------------------------------
void menuInit(void);
void printMenu(void);
void menuNavigateUp(void);
void menuNavigateDown(void);
void menuNavigateBack(uint8_t isLeaf);
enMenuLeaf menuNavigationSelect(void);

void highlightMenu(void);
void updateDifficulty(uint8_t diff);
void printDifficulty(void);



#endif