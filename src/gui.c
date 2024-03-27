
#include "gui.h"

ERAPI_HANDLE_REGION score_print;
ERAPI_HANDLE_REGION region_health;
s16 phealth_last=0;
void gui_init()
{
	// Setup new region with white on transparent text
	score_print = ERAPI_CreateRegion(0,0,0, 0,0xf, 0x03);
	ERAPI_SetTextColor( score_print, 0x01, 0x00);

	region_health = ERAPI_CreateRegion(0,0,0, 19,0xf, 0x01);
	ERAPI_SetTextColor( region_health, 0x02, 0x00);
	ERAPI_DrawText( region_health, 0, 0, "Health");
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

void gui_print_health(s16 health,u8 shield)
{
	// Create print buffer and convert int to string
	char num_print[8];
	citoa(health,num_print,10);

	// Clear region and print score
 	ERAPI_SetRegionColor( region_health, 0x00);
	ERAPI_ClearRegion(region_health);
 	ERAPI_SetRegionColor( region_health, 0x05);
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
			ERAPI_SetTextColor( region_health, 0x05, 0x00);
			ERAPI_DrawText( region_health, 0, 0, num_print);
		}else{
			ERAPI_SetTextColor( region_health, 0x04, 0x00);
			ERAPI_DrawText( region_health, 0, 0, num_print);
		}
		phealth_last = health;
	}else{
		for (int i = 0;i < 4; ++i)
			ERAPI_DrawLine(region_health,space,i,space+health,i,30);
		phealth_last = health;
	}

 	ERAPI_SetRegionColor( region_health, 0x05);
	u8 shield_place = 20;
	for (int y = 0;y < shield; ++y)
	{
			ERAPI_DrawLine(region_health,shield_place,6,shield_place+13,6,0);
			shield_place += 16;

	}

}
