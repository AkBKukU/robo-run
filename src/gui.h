#ifndef GUI_H
#define GUI_H
#include "def.h"
#include "erapi.h"

extern ERAPI_HANDLE_REGION score_print;


void gui_init();

void gui_print_score(u32 score);

#endif
