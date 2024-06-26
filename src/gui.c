
#include "gui.h"

//ERAPI_HANDLE_REGION score_print;
const u16 palette[] = {
	0x0000,		0x0000, 	0x0000,		0x0FF0,
	0x001F,		RGB(0x4986da) , 0x0000,		0x0000,
	0x0000,		0x0000,		0x0000,		0x0000,
	0xFFFF,		0x0000,		0x0000,		0x0000
};

ERAPI_BACKGROUND_DRAW_NUMBER background_score =
{
	BACKGROUND_LAYER_UI,
	GUI_SCORE_PALETTE,
	0, 0,
	GUI_SCORE_STYLE,
	GUI_SCORE_LENGTH,
	0, // Zero pad right
	1, // Zero pad left
	0,
	0
};

ERAPI_HANDLE_REGION region_health;
ERAPI_HANDLE_REGION region_screen;
s16 phealth_last=0;
void gui_init()
{
	// Background score drawing
	ERAPI_BackgroundDrawNumber(&background_score);

	// Region for screen
	region_screen = ERAPI_CreateRegion(0,0,GUI_SCREEN_X,GUI_SCREEN_Y,GUI_SCREEN_W,GUI_SCREEN_H);
	ERAPI_SetRegionColor(region_screen,0);
	ERAPI_SetTextColor( region_screen, GUI_SCREEN_COLOR, 0x00);

	// Region for heath display
	region_health = ERAPI_CreateRegion(0,0,0, 19,0xf, 0x01);
	ERAPI_SetTextColor( region_health, 0x02, 0x00);
}
void gui_clean()
{
 	ERAPI_SetRegionColor( region_health, 0x00);
	ERAPI_ClearRegion(region_health);
	ERAPI_ClearRegion(region_screen);
	background_score.amount_digits = 0;
	background_score.amount_digits = GUI_SCORE_LENGTH;
	ERAPI_BackgroundDrawNumber(&background_score);
}

void gui_pause(u8 draw)
{
	if (draw)
	{
		ERAPI_DrawText(region_screen,0,0,"pause");
	}else{
		ERAPI_ClearRegion(region_screen);
	}
}

void gui_print_score(u32 score)
{
	// Print score to background
	ERAPI_BackgroundDrawNumberNewValue(&background_score, score);
	// Replace palette with custom one
	ERAPI_SetBackgroundPalette( palette, 0x00, 0x06);
}

void gui_print_health(s16 health,u8 shield)
{
	// Limit parameters
	shield = shield  > 5 ? 5 : shield;
	health = (health < 0) ? 0 : health;
	// Clear region and print score
 	ERAPI_SetRegionColor( region_health, 0x00);
	ERAPI_ClearRegion(region_health);

	if (health > 20)
	{
		ERAPI_SetTextColor( region_health, GUI_HEALTH_PALINDEX, 0x00);
		ERAPI_SetRegionColor( region_health, GUI_HEALTH_PALINDEX);
	}else{
		ERAPI_SetTextColor( region_health, GUI_HEALTH_LOW_PALINDEX, 0x00);
		ERAPI_SetRegionColor( region_health, GUI_HEALTH_LOW_PALINDEX);
	}
	ERAPI_RegionDrawRect(region_health,
			0,0,
			health,3,
			1);

	// Create print buffer and convert int to string
	char num_print[8];
	citoa(health,num_print,10);
	ERAPI_DrawText( region_health, 0, 0, num_print);

	u8 shield_place = 20;
	for (int y = 0;y < shield; ++y)
	{
			ERAPI_DrawLine(region_health,shield_place,6,shield_place+13,6,0);
			shield_place += 16;

	}
	// Replace palette with custom one
	ERAPI_SetBackgroundPalette( palette, 0x00, 0x06);
}
