
#include "gamestate.h"
ERAPI_HANDLE_REGION score_print;
u8 sysexit = 0, win = 0;

void gui_print_score(u32 score)
{
	char num_print[8];
	citoa(score,num_print,10);
	ERAPI_ClearRegion(score_print);
	ERAPI_DrawText( score_print, 0, 0, num_print);
}
