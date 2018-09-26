#ifndef OLED_MENU_H
#define OLED_MENU_H



 typedef struct menu
{
	char* name;
	struct menu* parent;
	struct menu* child;
	struct menu* sibling;
	
}menu_t;



void oled_menu_init(void);

void menu_state_machine();


#endif