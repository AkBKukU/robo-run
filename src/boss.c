#include "boss.h"

u32 boss_level = 1;

unsigned short boss_map[BACK_X*BACK_Y];

void boss_spawn()
{
	boss_init();

	const u8 len = ERAPI_RandMax(boss_level *4)+2;

	for (u8 i;i<=len;++i)
	{
		if (i==0)
		{
			switch(boss_level)
			{
				case 1:
					boss_map[BACK_X] = BOSS_TILE_1_TALL;
					break;
				case 2:
					boss_map[BACK_X] = BOSS_MAP_WIDTH;
					boss_map[BACK_X*2] = BOSS_MAP_WIDTH*3;
					break;
				case 3:
					boss_map[BACK_X] = BOSS_MAP_WIDTH;
					boss_map[BACK_X*2] = BOSS_MAP_WIDTH*2;
					boss_map[BACK_X*3] = BOSS_MAP_WIDTH*3;
					break;
			}
		}else if(i == len)
		{
			switch(boss_level)
			{
				case 1:
					boss_map[BACK_X+4] = BOSS_MAP_WIDTH*2+4;
					boss_map[BACK_X+5] = BOSS_MAP_WIDTH*2+5;
					break;
				case 2:
					boss_map[BACK_X+4] = BOSS_MAP_WIDTH+4;
					boss_map[BACK_X+5] = BOSS_MAP_WIDTH+5;

					boss_map[BACK_X*2+4] = BOSS_MAP_WIDTH*3+4;
					boss_map[BACK_X*2+5] = BOSS_MAP_WIDTH*3+5;
					break;
				case 3:
					boss_map[BACK_X+4] = BOSS_MAP_WIDTH+4;
					boss_map[BACK_X+5] = BOSS_MAP_WIDTH+5;

					boss_map[BACK_X*2+4] = BOSS_MAP_WIDTH*2+4;
					boss_map[BACK_X*2+5] = BOSS_MAP_WIDTH*2+5;

					boss_map[BACK_X*3+4] = BOSS_MAP_WIDTH*3+4;
					boss_map[BACK_X*3+5] = BOSS_MAP_WIDTH*3+5;
					break;
			}
		} else {

			u8 v=ERAPI_RandMax(BOSS_MAP_WIDTH-3)+1;
			v=1;
			boss_map[i] = v;
			boss_map[BACK_X+i] = BOSS_MAP_WIDTH*2+v;
			boss_map[BACK_X*2+i] = v|0x800;


		}
	}
			boss_map[9] = 1;

	// Apply new background
	ERAPI_BACKGROUND slide =
	{
		map_bossTiles,
		map_bossSharedPal,
		boss_map,
		sizeof( map_bossTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( 1, &slide);
	ERAPI_SetBackgroundOffset(1,-60,-40);
}


void boss_init()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{

		boss_map[x + (y * BACK_Y) ]==4;
		}
	}
}
