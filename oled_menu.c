#include "oled_menu.h"
#include "oled_driver.h"

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "joystick_driver.h"

static uint8_t isActive = 0;

//static menu_t* Menu;


static ment_t* mainMenu;


void updateMenuSelection(enMenuSel currSelection){
	menuSel = currSelection;
}



menu_t * new_menu_item(char* name){

	menu_t * menu = malloc(sizeof(menu_t));

    if (menu){
        menu->sibling = NULL;
        menu->child = NULL;
        menu->name = name;
    }

    return menu;
}


menu_t * add_sibling(menu_t* menu, char* name){
	if (menu == NULL )
        return NULL;

    while (menu->sibling)
        menu = menu->next;

    return (menu->next = new_menu_item(name));
}


menu_t * add_child(menu_t* parent, char* name){
	if (parent == NULL)
        return NULL;

    if (parent->child)
        return add_sibling(parent->child, name);
    else
        return (parent->child = new_menu_item(name));
}


void printMenu(enMenuSel menuSel){

	/*menu_t* menu;
	char* name;
	switch(menuSel){
		case eMENU_MAIN:
			menu = mainMenu;
			break;
		case eMENU_PLAY:
			

			
	*/
	uint8_t cnt = 1;
	oled_pos(0,0),
	//oled_reset();
	oled_print(mainMenu->name);
	oled_pos(cnt++,0);



	oled_print(menu->child->name);
	menu_t* hlp = menu->child->sibling;
	while(hlp != NULL){
		oled_pos(cnt++,0);
		oled_print(hlp->name);
		hlp = hlp->child->sibling;
	}

}


menu_t* menuInit(void){

	//set selected menu item to main item 
	menuSel = eMENU_MAIN;

	//create menu entries
	//menu_t* start_screen = create_menu("Start Screen", NULL);

	//create menu tree
	mainMenu = new_menu_item("Main Menu");
    add_child(mainMenu, "Play");
    add_child(mainMenu, "Score");

	return mainMenu;

}


	//oled_print(play_menu->sibling->name);

	//print_menu(start_screen);
//}

void menu_state_machine(menu_t* current_menu){
	print_menu(current_menu);
	menu_t* hlp = menu->child;
	child_cnt = 0; //count number of children in menu
	while(hlp != NULL){
		child_cnt ++;
	}
	menu_t* child_index[7];

}






