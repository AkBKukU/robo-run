
#include "gamestate.h"
u8 sysexit = 0, win = 0;
u32 distance_tiles = 0, player_score=0,frame_count=0;

u16 base_seed = 24;
//u16 base_seed = 54;

void rand_stable_map()
{
	ERAPI_RandInit(base_seed+distance_tiles*10000);
}

void rand_true()
{
	ERAPI_RandInit(frame_count);
}

void rand_stable_boss(u8 boss_col)
{
	ERAPI_RandInit(base_seed+(boss_level+boss_col*10)*10000);
}
