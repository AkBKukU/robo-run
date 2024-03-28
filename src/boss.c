#include "boss.h"

u32 boss_level = 3;

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
					boss_map[center+BACK_X] = BOSS_TILE_1_TALL;
					break;
				case 2:
					boss_map[center+BACK_X] = BOSS_MAP_WIDTH;
					boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH|0x800;
					break;
				case 3:
					boss_map[center+BACK_X] = BOSS_MAP_WIDTH;
					boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2;
					boss_map[center+BACK_X*3] = BOSS_MAP_WIDTH|0x800;
					break;
			}
		}else if(i == len)
		{
			switch(boss_level)
			{
				case 1:
					boss_map[center+BACK_X+len] = BOSS_MAP_WIDTH*2+4;
					boss_map[center+BACK_X+len+1] = BOSS_MAP_WIDTH*2+5;
					break;
				case 2:
					boss_map[center+BACK_X+len] = BOSS_MAP_WIDTH+4;
					boss_map[center+BACK_X+len+1] = BOSS_MAP_WIDTH+5;

					boss_map[center+BACK_X*2+len] = (BOSS_MAP_WIDTH+4)|0x800;
					boss_map[center+BACK_X*2+len+1] = (BOSS_MAP_WIDTH+5)|0x800;
					break;
				case 3:
					boss_map[center+BACK_X+len] = BOSS_MAP_WIDTH+4;
					boss_map[center+BACK_X+len+1] = BOSS_MAP_WIDTH+5;

					boss_map[center+BACK_X*2+len] = BOSS_MAP_WIDTH*2+4;
					boss_map[center+BACK_X*2+len+1] = BOSS_MAP_WIDTH*2+5;

					boss_map[center+BACK_X*3+len] = (BOSS_MAP_WIDTH+4)|0x800;
					boss_map[center+BACK_X*3+len+1] = (BOSS_MAP_WIDTH+5)|0x800;
					break;
			}
		} else {

			u8 v=ERAPI_RandMax(BOSS_MAP_WIDTH-3)+1;
			switch(boss_level)
			{
				case 1:
					boss_map[center+i] = v;
					boss_map[center+BACK_X+i] = BOSS_MAP_WIDTH*2+v;
					boss_map[center+BACK_X*2+i] = v|0x800;
					break;
				case 2:
					boss_map[center+i] = v;
					boss_map[center+BACK_X+i] = BOSS_MAP_WIDTH*2+v;
					boss_map[center+BACK_X*2+i] = BOSS_MAP_WIDTH*2+v;
					boss_map[center+BACK_X*3+i] = v|0x800;

					break;
				case 3:
					boss_map[center+i] = v;
					boss_map[center+BACK_X+i] = BOSS_MAP_WIDTH+v;
					boss_map[center+BACK_X*2+i] = BOSS_MAP_WIDTH*2+v;
					boss_map[center+BACK_X*3+i] = (BOSS_MAP_WIDTH+v)|0x800;
					boss_map[center+BACK_X*4+i] = v|0x800;

					break;
			}

		}
	}

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

void boss_update()
{
	ERAPI_SetBackgroundOffset(1,0,vertical_offset);
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
