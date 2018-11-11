#include "oled_menu.h"
#include "oled_driver.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "uart_driver.h"
/*#ifndef DEBUG
#define DEBUG
#endif*/

#ifndef DEBUG
#include <avr/io.h>
#include <avr/interrupt.h>
#include "joystick_driver.h"

#endif



// -------------------------------------
// DEBUG define only for testing oled_menu.h, 
// oled_menu.c, pinpall_statemachine.c and pinball_statemachine.h
// -----------------------------------


#define MENU_HIGHLIGHT_INDENT 100
#define MENU_PRINT_INDENT 10


static menu_t* mainMenu;
static currMenuSelected_t* currMenuSelected;
static uint8_t lastRowIndex = 1;

static const char* STR_MENU_PLAY = "Play";
static const char* STR_MENU_START = "Start";
static const char* STR_MENU_SCORE= "Score";
static const char* STR_MENU_DIFFICULTY= "Difficulty";
static const char* STR_MENU_MAIN = "Main Menu";
static uint8_t currDifficulty = 0;


//only for debug
//static FILE uart_stream  = FDEV_SETUP_STREAM (uart_transmit, NULL, _FDEV_SETUP_WRITE);


//-----------------------------------------------------------
// create new menu item
// ----------------------------------------------------------
static menu_t * newMenuItem(menu_t* parent, const char* name){

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
static menu_t * addSibling(menu_t* menu, const char* name){
	if (menu == NULL )
        return NULL;

    while (menu->sibling)
        menu = menu->sibling;

    return (menu->sibling = newMenuItem(menu->parent, name));
}


// ----------------------------------------------------------
// add a child to given parent
// ----------------------------------------------------------
static menu_t * addChild(menu_t* parent, const char* name){
	if (parent == NULL)
        return NULL;

    if (parent->child)
        return addSibling(parent->child, name);
    else
        return (parent->child = newMenuItem(parent,name));
}

// -----------------------------------------------------
// regarding of current position in menu navigation
// an enum type is returned. 
// the current that is active is returned -> if the menu item
// is no leaf (has children), the enum value eNOLEAVE is returned
// as state changes in pinball_statemachine are only necessary if
// events based on a leaf are triggered 
// -----------------------------------------------------
static enMenuLeaf getCurrentMenuLeaf(void){
	//current selected menu item has no child -> is a leaf
	//check which leaf and return corresponding value
	if(currMenuSelected->menuItem->name == STR_MENU_DIFFICULTY){
		return eSET_DIFF;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_START){
		return eSTART_GAME;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_SCORE){
		return eSEE_SCORE;
	}

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
		uint8_t cnt = 1;
		ptr = currMenuSelected->menuItem;
		while(ptr->sibling != NULL){
				ptr = ptr->sibling;
				cnt++;
		}
			currMenuSelected->menuItem= ptr;
			currMenuSelected->row = cnt;
	}
	
}

// ----------------------------------------------------------
// select current menu and step deeper in menu hierarchy
// ---------------------------------------------------------
enMenuLeaf menuNavigationSelect(void){
	//step deeper in hierarchiy only if child is available
	if(currMenuSelected->menuItem->child != NULL){
		currMenuSelected->menuItem = currMenuSelected->menuItem->child;
		//set to row 1,as parent of this menu item will be printed in row 0
		currMenuSelected->row = 1;

		//print new menu
		printMenu();

		return eNOLEAF;
	}

	//there is no other child, so it has to be a leaf
	//state change is needed
	return getCurrentMenuLeaf();
	
}



// ----------------------------------------------------------
// select current menu and step deeper in menu hierarchy
// ---------------------------------------------------------
void menuNavigateBack(uint8_t isLeaf){ 	

	if(!isLeaf){
		if(currMenuSelected->menuItem->parent != NULL && currMenuSelected->menuItem->parent!=mainMenu){
			currMenuSelected->menuItem = currMenuSelected->menuItem->parent;
		}
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
	// remember old row to clean
	static uint8_t idx_row = 1;
	//TODO: add function to write to oled -> invert font color
	// and background color at that row
	//oled(currMenuSelected->row;
	#ifndef DEBUG
		oled_pos(idx_row,MENU_HIGHLIGHT_INDENT);
		oled_print(" ");

		oled_pos(currMenuSelected->row,MENU_HIGHLIGHT_INDENT);
		oled_print(">");

		idx_row = currMenuSelected->row;
	#else
		printf("highlight row: %d\r\n", currMenuSelected->row);
	#endif



}


// ---------------------------------------------------------
// print menu starting at the current menu items parent 
// ---------------------------------------------------------
void printMenu(void){

	#ifndef DEBUG
		oled_reset();
		oled_pos(0,0);
	#endif

	uint8_t cnt = 1;
	
	
	//start at parent menu item of current menu item
	menu_t* ptr = currMenuSelected->menuItem->parent;
	#ifndef DEBUG
		oled_print(currMenuSelected->menuItem->parent->name);
	#else
		printf("%s\r\n", currMenuSelected->menuItem->parent->name);
	#endif
	if(ptr!= NULL){
		//step to first child
		ptr = ptr->child;
		if(ptr != NULL){
			#ifndef DEBUG
				//step to next row
				oled_pos(cnt++,MENU_PRINT_INDENT);
				//print name of child
				oled_print(ptr->name);
			#else
				printf("    %s\n\r", ptr->name);
			#endif
			//navigate to sibling
			ptr = ptr->sibling;
			while(ptr != NULL){
				#ifndef DEBUG
					//step to next row
					oled_pos(cnt++,MENU_PRINT_INDENT);
					//print name of sibling
					oled_print(ptr->name);
				#else
					printf("    %s\n\r", ptr->name);
				#endif
				//step to next sibling
				ptr = ptr->sibling;
			}
		}
	}

	highlightMenu();
	
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

    currMenuSelected = malloc(sizeof(currMenuSelected_t));
    currMenuSelected->menuItem = mainMenu->child;
	currMenuSelected->row = 1;



}

void printPlayMode(){
	oled_reset();
	oled_pos(0, 0); 
	oled_print("Playing...");
	oled_pos(2, 0); 
	oled_print("Score:");

}

void updateScore(uint16_t score){
	char str[2];
    sprintf(str, "%d",score);
	oled_pos(4,0);
	oled_print(str);
}

//--------------------------------------------------------
// prints difficulty to menu
//--------------------------------------------------------
void printDifficulty(void){
	#ifndef DEBUG
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
	#else
		printf("Set difficulty\r\n");
		printf("min = 0, max = 9\r\n");
		printf("JOY_UP: ++\r\n");
		printf("JOY_DOWN: --\r\n");
		printf("Diff = ");
	#endif

	updateDifficulty(currDifficulty);
	
}

// ----------------------------------------------------
// updates current difficulty in menu
// ----------------------------------------------------
void updateDifficulty(uint8_t diff){
	char str[2];
    sprintf(str, "%d",diff);

	#ifndef DEBUG
		oled_pos(5, 100);
		oled_print(str);
	#else  
  		printf("curr diff= %s\n", str);
  	#endif
	
	currDifficulty = diff;
}







