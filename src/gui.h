#ifndef GUI_H
#define GUI_H
#include "def.h"
#include "erapi.h"

#include "util.h"

extern ERAPI_HANDLE_REGION score_print;
extern ERAPI_HANDLE_REGION region_health;

extern s16 phealth_last;

void gui_init();

void gui_print_score(u32 score);
void gui_print_health(s16 health,u8 shield);

#endif
