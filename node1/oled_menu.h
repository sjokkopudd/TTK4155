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
	eSEL_PLAYER,
	eSET_DIFF,
	eSTART_GAME,
	eSEE_SCORE,
	eRESET_SCORE,
	eNOLEAF


}enMenuLeaf;

//-------------------------------------------------------
// functions that are called from pinball_statemachine
//------------------------------------------------------
void oled_menu_init(void);
void oled_print_menu(void);
void oled_menu_navigate_up(void);
void oled_menu_navigate_down(void);
void oled_menu_navigate_back(uint8_t isLeaf);

enMenuLeaf oled_menu_navigation_select(void);

void oled_highlight_menu(void);
void oled_update_difficulty(uint8_t diff);
void oled_print_difficulty(uint8_t difficulty);
void oled_print_play_mode(void);

void oled_update_score(uint16_t score);
void oled_print_reset_high_scores(uint16_t highscore, uint8_t player);
void oled_highlight_reset_high_score_selection(uint8_t sel);

void oled_reset_score();
void oled_print_high_score(uint16_t score);
void oled_print_game_over(uint16_t score, uint8_t player);
void oled_print_players(uint8_t active_player);
void oled_highlight_player(uint8_t player);
void oled_select_player(uint8_t player);
void oled_print_best_players(uint16_t scores_players[], uint8_t length);

#endif