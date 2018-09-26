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


menu_t* create_menu(char* name, menu_t* parent){
	menu_t* menu = malloc(sizeof(menu_t));
	menu->name = name;
	menu->parent = parent;
	menu->child = NULL;
	menu->sibling = NULL;
	if(parent != NULL && parent->child == NULL){
		parent->child = menu;
	}
	else if(parent != NULL){
		menu_t* hlp = parent->child->sibling;
		menu_t* prev = parent->child;
		

		while(hlp != NULL){
			prev = hlp;
			hlp = hlp->sibling;
			
		}
		prev->sibling = menu;
		
	}
	return menu;
}

static void print_menu(menu_t* menu){
	uint8_t cnt = 1;
	oled_pos(0,0),
	//oled_reset();
	oled_print(menu->name);
	oled_pos(cnt++,0);

	oled_print(menu->child->name);
	menu_t* hlp = menu->child->sibling;
	while(hlp != NULL){
		oled_pos(cnt++,0);
		oled_print(hlp->name);
		hlp = hlp->child->sibling;
	}

}

void oled_menu_init(void){
	//create menu entries
	menu_t* start_screen = create_menu("Start Screen", NULL);
	menu_t* main_menu =  create_menu("Main Menu", start_screen);
	menu_t* play_menu =  create_menu("Play", main_menu);
	menu_t* score_menu =  create_menu("Score", main_menu);


	oled_print(play_menu->sibling->name);

	print_menu(start_screen);
}

void menu_state_machine(menu_t* current_menu){
	print_menu(current_menu);
	menu_t* hlp = menu->child;
	child_cnt = 0; //count number of children in menu
	while(hlp != NULL){
		child_cnt ++;
	}
	menu_t* child_index[7];

}






