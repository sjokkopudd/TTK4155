#include "oled_menu.h"
#include "oled_driver.h"

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "joystick_driver.h"
#include "uart_driver.h"


#define MENU_HIGHLIGHT_INDENT 40
#define MENU_PRINT_INDENT 10


static menu_t* mainMenu;
static currMenuSelected_t* currMenuSelected;
static uint8_t lastRowIndex;

static const char* STR_MENU_PLAY = "Play";
static const char* STR_MENU_START = "Start";
static const char* STR_MENU_SCORE= "Score";
static const char* STR_MENU_DIFFICULTY= "Difficulty";
static const char* STR_MENU_MAIN = "Main Menu";
static uint8_t currDifficulty = 0;


//only for debug
static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);


//-----------------------------------------------------------
// create new menu item
// ----------------------------------------------------------
static menu_t * newMenuItem(menu_t* parent, char* name){

	menu_t * menu = malloc(sizeof(menu_t));

    if (menu){
    	menu->parent = parent;
        menu->sibling = NULL;
        menu->child = NULL;
        menu->name = name;
    }

    return menu;
}

//-----------------------------------------------------------
// add a sibling to a given menu item
// ----------------------------------------------------------
static menu_t * addSibling(menu_t* menu, char* name){
	if (menu == NULL )
        return NULL;

    while (menu->sibling)
        menu = menu->sibling;

    return (menu->sibling = newMenuItem(menu->parent, name));
}


// ----------------------------------------------------------
// add a child to given parent
// ----------------------------------------------------------
static menu_t * addChild(menu_t* parent, char* name){
	if (parent == NULL)
        return NULL;

    if (parent->child)
        return addSibling(parent->child, name);
    else
        return (parent->child = newMenuItem(parent,name));
}


// ----------------------------------------------------------
// navigate to upper menu item
// if the first menu item is selected, the last menu item will
// be selected
// ----------------------------------------------------------
void menuNavigateUp(void){
	
	//find left sibling of current menuitem
	menu_t* ptr = currMenuSelected->menuItem->parent->child;
	while(ptr->sibling != NULL && ptr->sibling != currMenuSelected->menuItem){
		ptr = ptr->sibling;
	}
	if(ptr->sibling == currMenuSelected->menuItem){
		//left sibling found
		currMenuSelected->menuItem = ptr;
		currMenuSelected->row--;
	}
	else{
		//if it is not found or if there is no sibling at its left
		//nagigate to the last sibling
		ptr = currMenuSelected->menuItem->sibling;
		if(ptr != NULL){
			while(ptr->sibling != NULL){
				ptr = ptr->sibling;
			}
			currMenuSelected->menuItem= ptr;
			currMenuSelected->row = lastRowIndex;
		}
	}
}

// ----------------------------------------------------------
// select current menu and step deeper in menu hierarchy
// ---------------------------------------------------------
void menuNavigationSelect(void){
	//only if child is available
	if(currMenuSelected->child != NULL){
		currMenuSelected = currMenuSelected->child;
	}

	//print new menu
	printMenu();
}

// ----------------------------------------------------------
// select current menu and step deeper in menu hierarchy
// ---------------------------------------------------------
void menuNavigationBack(void){
	//
	if(currMenuSelected->parent != NULL && currMenuSelected->parent!=mainMenu){
		currMenuSelected = currMenuSelected->parent;
	}

	//print new menu
	printMenu();
}

// ----------------------------------------------------------
// navigate to the lower menu item
// if the last menu item is selected, the first menu item will 
// be selected 
// ----------------------------------------------------------
void menuNavigateDown(void){
	//navigate to right sibling
	if(currMenuSelected->menuItem->sibling != NULL){
		currMenuSelected->menuItem = currMenuSelected->menuItem->sibling;
		currMenuSelected->row++;
	}
	else{
		//navigate to parent and to its child to go to the first menu item
		currMenuSelected->menuItem = currMenuSelected->menuItem->parent->child;
		currMenuSelected->row = 1;
	}

}

// ---------------------------------------------------------
// highlight current selected menu item 
// ---------------------------------------------------------
void highlightMenu(void){
	//TODO: add function to write to oled -> invert font color
	// and background color at that row
	//oled(currMenuSelected->row;
	oled_pos(currMenuSelected->row,MENU_HIGHLIGHT_INDENT);
	oled_print("<");



}


// ---------------------------------------------------------
// print menu starting at the current menu items parent 
// ---------------------------------------------------------
void printMenu(void){

	oled_reset();
	uint8_t cnt = 1;
	oled_pos(0,0);
	
	//start at parent menu item of current menu item
	menu_t* ptr = currMenuSelected->menuItem->parent;
	oled_print(currMenuSelected->menuItem->parent->name);
	if(ptr!= NULL){
		//print parent at indent 0
		oled_print(ptr->name);
		stdout = &uart_stream;
		printf("ptr->name: %d\n\r", ptr->name);
		//step to first child
		ptr = ptr->child;
		if(ptr != NULL){
			//step to next row
			oled_pos(cnt++,MENU_PRINT_INDENT);
			//print name of child
			oled_print(ptr->name);
			stdout = &uart_stream;
			printf("ptr->name: %d\n\r", ptr->name);
			//navigate to sibling
			ptr = ptr->sibling;
			while(ptr != NULL){
				//step to next row
				oled_pos(cnt++,MENU_PRINT_INDENT);
				//print name of sibling
				oled_print(ptr->name);
				stdout = &uart_stream;
				printf("ptr->name: %d\n\r", ptr->name);
				//step to next sibling
				ptr = ptr->sibling;
			}
		}
	}
	//set row number of last menu item
	lastRowIndex = cnt-1;
}


// -----------------------------------------------------------
// menu initialization 
// -----------------------------------------------------------
void menuInit(void){

	//set selected menu item to main item 
	//menuSel = eMENU_MAIN;
	
	//create menu entries
	//menu_t* start_screen = create_menu("Start Screen", NULL);

	//create menu tree
	mainMenu = newMenuItem(NULL, STR_MENU_MAIN);
    menu_t* play = addChild(mainMenu, STR_MENU_PLAY);
    addChild(play, STR_MENU_DIFFICULTY); // add sub menu difficulty to play menu
    addChild(play, STR_MENU_START);		 // add sub menu start to play menu

    addChild(mainMenu, STR_MENU_SCORE);	 // add score to main menu

    currMenuSelected->menuItem = mainMenu->child;
	currMenuSelected->row = 1;

    
	stdout = &uart_stream;
	printf("urrMenuSelected->menuItem->name: %d\n\r", currMenuSelected->menuItem->name);


}
//--------------------------------------------------------
//
//--------------------------------------------------------
void printDifficulty(void){
	oled_pos(0, 0); 
	oled_print("Set difficulty");
	oled_pos(1,0);
	oled_print("min = 0, max = 9");
	oled_pos(2,0);
	oled_print("JOY_UP: ++");
	oled_pos(3,0);
	oled_print("JOY_DOWN: --");
	oled_pos(5,0);
	oled_print("Diff = ");
	updateDifficulty(currDifficulty);
	
}

void updateDifficulty(uint8_t diff){
	oled_pos(5, 100);
	oled_print(diff);
	
	currDifficulty = diff;


}

// -----------------------------------------------------
// regarding of current position in menu navigation
// an enum type is returned. 
// the current that is active is returned -> if the menu item
// is no leaf (has children), the enum value eNOLEAVE is returned
// as state changes in pinball_statemachine are only necessary if
// events based on a leaf are triggered 
// -----------------------------------------------------
enMenuLeaf getCurrentMenuLeaf(void){
	//current selected menu item is a parent (no leaf)
	if(currMenuSelected->menuItem->child != NULL){
		return eNOLEAF;
	}

	//current selected menu item has no child -> is a leaf
	//check which leaf and return corresponding value
	if(currMenuSelected->menuItem->name == STR_MENU_DIFFICULTY){
		return eDIFF;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_START){
		return eSTART;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_SCORE){
		return eSCORE;
	}

}





