#ifndef GUI_H
#define GUI_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "util.h"

// Score Background
extern ERAPI_BACKGROUND_DRAW_NUMBER background_score;
#define GUI_SCORE_PALETTE 0
#define GUI_SCORE_STYLE 4120
#define GUI_SCORE_TEXTSHAD_PALINDEX 2
#define GUI_SCORE_TEXT_PALINDEX 12
#define GUI_SCORE_LENGTH 5


#define GUI_HEALTH_PALINDEX 0x05
#define GUI_HEALTH_LOW_PALINDEX 0x06
extern ERAPI_HANDLE_REGION region_health;
extern s16 phealth_last;

extern ERAPI_HANDLE_REGION region_screen;
#define GUI_SCREEN_COLOR 3
#define GUI_SCREEN_X 9
#define GUI_SCREEN_Y 9
#define GUI_SCREEN_W 12
#define GUI_SCREEN_H 2

void gui_init();
void gui_clean();
void gui_pause(u8 draw);

void gui_print_score(u32 score);
void gui_print_health(s16 health,u8 shield);

#endif
