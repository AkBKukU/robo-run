
#include "gui.h"

ERAPI_HANDLE_REGION score_print;

void gui_init()
{
	// Setup new region with white on transparent text
	score_print = ERAPI_CreateRegion(0,0,0, 0,0xf, 0x03);
	ERAPI_SetTextColor( score_print, 0x01, 0x00);
}

void gui_print_score(u32 score)
{
	// Create print buffer and convert int to string
	char num_print[8];
	citoa(score,num_print,10);

	// Clear region and print score
	ERAPI_ClearRegion(score_print);
	ERAPI_DrawText( score_print, 0, 0, num_print);
}
