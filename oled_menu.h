#ifndef OLED_MENU_H
#define OLED_MENU_H

#include <stdint.h>

 typedef struct menu
{
	char* name;
	struct menu* parent;
	struct menu* child;
	struct menu* sibling;
	
}menu_t;

typedef struct currMenuSelected
{
	uint8_t row;
	menu_t* menuItem;

}currMenuSelected_t;




//-------------------------------------------
// type enum: different menu items
typedef enum enMenuSel
{
	eMENU_MAIN,
	eMENU_PLAY,
	eMENU_SCORE


}enMenuSel;

//-------------------------------------------------------
// functions that are called from pinball_statemachine
//------------------------------------------------------
void menuInit(void);
void printMenu(void);
void menuNavigateUp(void);
void menuNavigateDown(void);
void highlightMenu(void);



#endif