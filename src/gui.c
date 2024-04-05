
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
s16 phealth_last=0;
void gui_init()
{
	// Region for heath display
	region_health = ERAPI_CreateRegion(0,0,0, 19,0xf, 0x01);
	ERAPI_SetTextColor( region_health, 0x02, 0x00);
}
void gui_start()
{
	// Background score drawing
	ERAPI_BackgroundDrawNumber(&background_score);
}
void gui_clean()
{
	background_score.amount_digits = 0;
	ERAPI_BackgroundDrawNumber(&background_score);
	background_score.amount_digits = GUI_SCORE_LENGTH;

 	ERAPI_SetRegionColor( region_health, 0x00);
	ERAPI_ClearRegion(region_health);
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
	// Create print buffer and convert int to string
	char num_print[8];
	citoa(health,num_print,10);

	// Clear region and print score
 	ERAPI_SetRegionColor( region_health, 0x00);
	ERAPI_ClearRegion(region_health);
 	ERAPI_SetRegionColor( region_health, GUI_HEALTH_PALINDEX);
// 	ERAPI_SetPixel(region_health,1,0);
	health = (health < 0) ? 0 : health;
	u8 space = 0;
	if(phealth_last)
	{
		if (health > 16)
		{
			for (int i = 0;i < 4; ++i)
			{
				ERAPI_DrawLine(region_health,space,i,space+phealth_last,i,0);
			}
			ERAPI_SetRegionColor( region_health, 0x00);
			for (int i = 0;i < 4; ++i)
			{
				ERAPI_DrawLine(region_health,space+phealth_last,i,space+health,i,8);
			}
			ERAPI_SetTextColor( region_health, GUI_HEALTH_PALINDEX, 0x00);
			ERAPI_DrawText( region_health, 0, 0, num_print);
		}else{
			ERAPI_SetTextColor( region_health, GUI_HEALTH_LOW_PALINDEX, 0x00);
			ERAPI_DrawText( region_health, 0, 0, num_print);
		}
		phealth_last = health;
	}else{
		for (int i = 0;i < 4; ++i)
			ERAPI_DrawLine(region_health,space,i,space+health,i,30);
		phealth_last = health;
	}

 	ERAPI_SetRegionColor( region_health, GUI_HEALTH_PALINDEX);
	u8 shield_place = 20;
	for (int y = 0;y < shield; ++y)
	{
			ERAPI_DrawLine(region_health,shield_place,6,shield_place+13,6,0);
			shield_place += 16;

	}

}
