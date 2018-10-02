#ifndef OLED_MENU_H
#define OLED_MENU_H



 typedef struct menu
{
	char* name;
	struct menu* parent;
	struct menu* child;
	struct menu* sibling;
	
}menu_t;




//-------------------------------------------
// type enum: different menu items
typedef enum enMenuSel
{
	eMENU_MAIN,
	eMENU_PLAY,
	eMENU_SCORE


}enMenuSel;




void printMenu(enMenuSel menuSel);

void updateMenuSelection(enMenuSel currSelection);

menu_t* menuInit(void);

void menu_state_machine();


#endif