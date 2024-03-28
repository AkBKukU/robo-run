#include "boss.h"

u32 boss_level =2;
u8 boss_spawning_flag=0;
u8 boss_live=0;
u8 boss_len=0,boss_gen_col=0,boss_x_pos=0;
s8 boss_y_offset=0;
u16 boss_spawn_distance=0;
u8 boss_weapon_allowance=0;
u8 boss_weapon_min=0;
u8 weapon_laser=0;
u8 boss_kill_count=0;

struct boss_weapons manger_boss_weapons[BOSS_WEAPON_MAX];

unsigned short boss_map[BACK_X*32];

void boss_spawn()
{
	rand_stable_boss();
	if (!boss_spawning_flag)
	{
		boss_init();
		boss_len = ERAPI_RandMax(boss_level *BOSS_SIZE_RATIO_MAX)+5;
		boss_gen_col=0;
		boss_x_pos=15;
		boss_y_offset=0;
		boss_weapon_allowance=1+ERAPI_RandMax(boss_level*3);
		boss_weapon_min = boss_level+1;
		boss_weapon_allowance=boss_weapon_allowance> 10 ? 10 : boss_weapon_allowance;
		weapon_laser=1;
	}
	boss_spawning_flag = 1;

	// Shift boss left until
	++boss_x_pos;
	if (boss_x_pos <= 64)
	{
		return;
	}
	boss_x_pos=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<32;++y)
	{
		boss_map[ (x-1) + (y * BACK_X) ] = boss_map[ (x) + (y * BACK_X)];
	}}

	if (boss_gen_col==0)
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
	}else if(boss_gen_col== boss_len-1)
	{
		switch(boss_level)
		{
			case 1:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+BOSS_MAP_WEAPONS+1;
				break;
			case 2:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+1;

				boss_map[center+BACK_X*2] = (BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+1)|0x800;
				break;
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+1;

				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+BOSS_MAP_WEAPONS+1;

				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+1)|0x800;
				break;
		}
	}else if(boss_gen_col== boss_len)
	{
		switch(boss_level)
		{
			case 1:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+BOSS_MAP_WEAPONS+2;
				break;
			case 2:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2;

				boss_map[center+BACK_X*2] = (BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2)|0x800;
				break;
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2;

				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+BOSS_MAP_WEAPONS+2;

				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2)|0x800;
				break;
		}
	} else {

		u8 v=ERAPI_RandMax(BOSS_MAP_WIDTH-3)+1;
		u8 weapon_spawn = (ERAPI_RandMax(10) > 1 && boss_weapon_allowance);
		switch(boss_level)
		{
			case 1:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				if(weapon_spawn || boss_weapon_min)
				{
					u8 w=1+ERAPI_RandMax(BOSS_MAP_WEAPONS-1);
					boss_map[center] = w;
					boss_map[center+BACK_X*2] = w|0x800;
					--boss_weapon_allowance;
				}
				break;
			case 2:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;
				if(weapon_spawn || boss_weapon_min)
				{
					u8 w=1+ERAPI_RandMax(BOSS_MAP_WEAPONS-1+weapon_laser);
					if(w == BOSS_TILE_LASER) weapon_laser=0;
					boss_map[center] = w;
					boss_map[center+BACK_X*3] = w|0x800;
					--boss_weapon_allowance;
				}

				break;
			default:
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+v)|0x800;
				if(weapon_spawn || boss_weapon_min)
				{
					u8 w=2+ERAPI_RandMax(BOSS_MAP_WEAPONS-2+weapon_laser);
					if(w == BOSS_TILE_LASER) weapon_laser=0;
					boss_map[center] = w;
					boss_map[center+BACK_X*4] = w|0x800;
					--boss_weapon_allowance;
				}

				break;
		}

	}
	if(boss_weapon_min) --boss_weapon_min;

	++boss_gen_col;
	if(boss_gen_col > boss_len+1)
	{
		boss_live = 1;
		boss_spawning_flag = 0;
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
}

void boss_update()
{
	// Spawn boss if gone far enough, is called mul
	if(distance_tiles > boss_spawn_distance && !(boss_live)) boss_spawn();

	if(!boss_spawning_flag && !boss_live) return;

	ERAPI_SetBackgroundOffset(1,boss_x_pos/8,(boss_y_offset*-1)+vertical_offset);

	if((8+boss_level + boss_y_offset/8) > tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) --boss_y_offset;

	if((8+boss_level + boss_y_offset/8) < tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) ++boss_y_offset;

	if(!boss_live) return;

}

void boss_init()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<32;++y)
		{

		boss_map[x + (y * 32) ]=0;
		}
	}
// 	boss_spawn_distance = BOSS_SPAWN_DISTANCE_MAX+ERAPI_RandMax(boss_level*BOSS_SPAWN_DISTANCE_MAX);
 	boss_spawn_distance = BOSS_SPAWN_DISTANCE_MAX;
}
