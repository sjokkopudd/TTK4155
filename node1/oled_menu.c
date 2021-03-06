#include "oled_menu.h"
#include "oled_driver.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "uart_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "joystick_driver.h"
#include "number_print.h"

#define MENU_HIGHLIGHT_INDENT 100
#define PLAYER_HIGHLIGHT_INDENT 100
#define PLAYER_SELECT_INDENT 80
#define PLAYER_HIGHLIGHT_ROW_OFFSET 2
#define MENU_PRINT_INDENT 10
#define NUM_PLAYERS 4

#define RESET_SCORE_SELECT_INDENT 80
#define RESET_SCORE_ROW_OFFSET 3

static menu_t* mainMenu;
static currMenuSelected_t* currMenuSelected;
static uint8_t lastRowIndex = 1;

static const char* STR_MENU_PLAY = "Play";
static const char* STR_MENU_START = "Start";
static const char* STR_MENU_SCORE= "Score";
static const char* STR_MENU_SETTING = "Reset";
static const char* STR_MENU_PLAYER= "Player";

static const char* PLAYER_NAMES[] = {"Link", "Sonic", "Waluigi", "Samus"};

static const char* STR_MENU_MAIN = "Main Menu";


//-----------------------------------------------------------
// create new menu item
// ----------------------------------------------------------
static menu_t * new_menu_item(menu_t* parent, const char* name){

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
static menu_t * add_sibling(menu_t* menu, const char* name){
	if (menu == NULL )
        return NULL;

    while (menu->sibling)
        menu = menu->sibling;

    return (menu->sibling = new_menu_item(menu->parent, name));
}


// ----------------------------------------------------------
// add a child to given parent
// ----------------------------------------------------------
static menu_t * add_child(menu_t* parent, const char* name){
	if (parent == NULL)
        return NULL;

    if (parent->child)
        return add_sibling(parent->child, name);
    else
        return (parent->child = new_menu_item(parent,name));
}

// -----------------------------------------------------
// regarding of current position in menu navigation
// an enum type is returned. 
// the current that is active is returned -> if the menu item
// is no leaf (has children), the enum value eNOLEAVE is returned
// as state changes in pinball_statemachine are only necessary if
// events based on a leaf are triggered 
// -----------------------------------------------------
static enMenuLeaf get_current_menu_leaf(void){
	//current selected menu item has no child -> is a leaf
	//check which leaf and return corresponding value
	if(currMenuSelected->menuItem->name == STR_MENU_PLAYER){
		return eSEL_PLAYER;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_SETTING){
		return eSET_SETTING;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_START){
		return eSTART_GAME;
	}
	else if(currMenuSelected->menuItem->name == STR_MENU_SCORE){
		return eSEE_SCORE;
	}
	else{
		return eNOLEAF;
	}

}


// ----------------------------------------------------------
// navigate to upper menu item
// if the first menu item is selected, the last menu item will
// be selected
// ----------------------------------------------------------
void oled_menu_navigate_up(void){
	
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
enMenuLeaf oled_menu_navigation_select(void){
	//step deeper in hierarchiy only if child is available
	if(currMenuSelected->menuItem->child != NULL){
		currMenuSelected->menuItem = currMenuSelected->menuItem->child;
		//set to row 1,as parent of this menu item will be printed in row 0
		currMenuSelected->row = 1;

		//print new menu
		oled_print_menu();

		return eNOLEAF;
	}

	//there is no other child, so it has to be a leaf
	//state change is needed
	return get_current_menu_leaf();
	
}



// ----------------------------------------------------------
// select current menu and step deeper in menu hierarchy
// ---------------------------------------------------------
void oled_menu_navigate_back(uint8_t isLeaf){ 	

	if(!isLeaf){
		if(currMenuSelected->menuItem->parent != NULL && currMenuSelected->menuItem->parent!=mainMenu){
			currMenuSelected->menuItem = currMenuSelected->menuItem->parent;
		}
	}
	currMenuSelected->row = 1;
	currMenuSelected->menuItem = currMenuSelected->menuItem->parent->child;
	//print new menu
	oled_print_menu();
}

// ----------------------------------------------------------
// navigate to the lower menu item
// if the last menu item is selected, the first menu item will 
// be selected 
// ----------------------------------------------------------
void oled_menu_navigate_down(void){
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
void oled_highlight_menu(void){
	// remember old row to clean
	static uint8_t idx_row = 1;
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
void oled_print_menu(void){

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

	oled_highlight_menu();
	
}


// -----------------------------------------------------------
// menu initialization 
// -----------------------------------------------------------
void oled_menu_init(void){

	//create menu tree
	mainMenu = new_menu_item(NULL, STR_MENU_MAIN);
    menu_t* play = add_child(mainMenu, STR_MENU_PLAY);
    add_child(play, STR_MENU_PLAYER);		 // add sub menu player to play menu
   	 // add sub menu difficulty to play menu
    add_child(play, STR_MENU_START);		 // add sub menu start to play menu

    add_child(mainMenu, STR_MENU_SCORE);	 // add score to main menu
    add_child(mainMenu, STR_MENU_SETTING);

    currMenuSelected = malloc(sizeof(currMenuSelected_t));
    currMenuSelected->menuItem = mainMenu->child;
	currMenuSelected->row = 1;



}

void oled_print_play_mode(){
	oled_reset();
	oled_pos(0, 0); 
	oled_print("Playing...");
	oled_pos(2, 0); 
	oled_print("Score:");

}

void oled_update_score(uint16_t score){
	char str[5];
	convert_int_to_string(score,str);
	oled_pos(4,0);
	oled_print(str);
}

void oled_reset_score(){

	oled_clear_line(4);

}

void oled_print_reset_high_scores(uint8_t sel){
	oled_reset();
	oled_pos(0,0);

	oled_print("Reset highscores");
	oled_pos(1,0);
	oled_print("Are you sure?");
	oled_pos(3,0);
	oled_print("No");
	oled_pos(4,0);
	oled_print("Yes");

	oled_update_reset_high_scores(0);
	
}

void oled_update_reset_high_scores(uint8_t sel){
	static uint8_t lastSel = 0;

	if(lastSel != sel){

		//delete old position
		oled_pos(lastSel+RESET_SCORE_ROW_OFFSET, PLAYER_HIGHLIGHT_INDENT);
		oled_print(" ");
		lastSel = sel;

	}
	
    oled_pos(sel+RESET_SCORE_ROW_OFFSET, PLAYER_HIGHLIGHT_INDENT);
	oled_print(">");
		
}

// ---------------------------------------------------
// prints all 4 player available
// ---------------------------------------------------
void oled_print_players(uint8_t active_player){
	oled_reset();
	oled_pos(0,0);
	oled_print("Select Player");
	oled_pos(2,0);
	oled_print(PLAYER_NAMES[0]);
	oled_pos(3,0);
	oled_print(PLAYER_NAMES[1]);
	oled_pos(4,0);
	oled_print(PLAYER_NAMES[2]);
	oled_pos(5,0);
	oled_print(PLAYER_NAMES[3]);

	oled_highlight_player(active_player);
}


// --------------------------------------------------
// highlight currently chosen player
// --------------------------------------------------
void oled_highlight_player(uint8_t player){
	static uint8_t lastPlayer = 0;

	if(lastPlayer != player){

		//delete old position
		oled_pos(lastPlayer+PLAYER_HIGHLIGHT_ROW_OFFSET, PLAYER_HIGHLIGHT_INDENT);
		oled_print(" ");
		lastPlayer = player;

	}
	
    oled_pos(player+PLAYER_HIGHLIGHT_ROW_OFFSET, PLAYER_HIGHLIGHT_INDENT);
    oled_print(">");

}


void oled_select_player(uint8_t player){
	static uint8_t lastPlayer = 1;

	if(lastPlayer != player){

		//delete old position
		oled_pos(lastPlayer+PLAYER_HIGHLIGHT_ROW_OFFSET, PLAYER_SELECT_INDENT);
		oled_print(" ");
		lastPlayer = player;

	}
	
    oled_pos(player+PLAYER_HIGHLIGHT_ROW_OFFSET, PLAYER_SELECT_INDENT);
    oled_print("*");



}

// --------------------------------------------------
// print the high score trophy here
// --------------------------------------------------
void oled_print_high_score(uint16_t score){

	oled_reset();

	oled_pos(0,0);
	oled_print(" NEW HIGHSCORE");

	oled_print_trophy();

}

// --------------------------------------------------
// print game over with scores
// --------------------------------------------------
void oled_print_game_over(uint16_t score, uint8_t player){
	oled_reset();
	oled_pos(0,0);

	oled_game_over();

	oled_pos(4,0);
	oled_print("Player:");
	oled_pos(4,60);
	oled_print(PLAYER_NAMES[player]);

	oled_pos(5,0);
	oled_print("Scores:");

    char str[5];
    convert_int_to_string(score,str);
    oled_pos(5,60);
   	oled_print(str);


}



void oled_print_best_players(uint16_t scores_players[], uint8_t length){

	oled_reset();
	oled_pos(0,0);

	oled_print("Scores");

	char str[5];
	oled_pos(2, 0);
	oled_print(PLAYER_NAMES[0]);
	convert_int_to_string(scores_players[0], str);
	oled_pos(2, 60);
	oled_print(str);

	oled_pos(3, 0);
	oled_print(PLAYER_NAMES[1]);
	convert_int_to_string(scores_players[1], str);
	oled_pos(3, 60);
	oled_print(str);

	oled_pos(4, 0);
	oled_print(PLAYER_NAMES[2]);
	convert_int_to_string(scores_players[2], str);
	oled_pos(4, 60);
	oled_print(str);

	oled_pos(5, 0);
	oled_print(PLAYER_NAMES[3]);
	convert_int_to_string(scores_players[3], str);
	oled_pos(5, 60);
	oled_print(str);

}


