#include "boss.h"

u8 boss_spawning_flag=0;
u8 boss_live=0;
u8 boss_len=0, boss_level=1,boss_gen_col=0,boss_x_pos=0;
s16 boss_y_offset=0;
u16 boss_spawn_distance=0;
u8 boss_weapon_allowance=0;
u8 boss_weapon_count=0;
u8 boss_weapon_min=0;
u8 weapon_laser=0;
s32 boss_health=0;

struct boss_weapons manger_boss_weapons[BOSS_WEAPON_MAX];

unsigned short boss_map[BACK_X*32];

void boss_spawn()
{
	rand_stable_boss(boss_gen_col);

	// Shift boss left until
	++boss_x_pos;
	if (boss_x_pos <= 64)
	{
		return;
	}
	boss_x_pos=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_X;++y)
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
			default:
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
			default:
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
			default:
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2;

				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+BOSS_MAP_WEAPONS+2;

				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+BOSS_MAP_WEAPONS+2)|0x800;
				break;
		}
	} else if(boss_gen_col < boss_len){

		u8 v=ERAPI_RandMax(BOSS_MAP_WIDTH-3)+1;
		u8 weapon_spawn = (ERAPI_RandMax(boss_len-3) >= boss_weapon_count);
		switch(boss_level)
		{
			case 1:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				if(weapon_spawn)
				{
					u8 w=1+ERAPI_RandMax(BOSS_MAP_WEAPONS-1);
					boss_map[center] = w;
					boss_map[center+BACK_X*2] = w|0x800;

					manger_boss_weapons[boss_weapon_count].type = w;
					manger_boss_weapons[boss_weapon_count].x = boss_gen_col;
					manger_boss_weapons[boss_weapon_count].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);
					++boss_weapon_count;
				}
				break;
			case 2:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;
				if(weapon_spawn)
				{
					u8 w=1+ERAPI_RandMax(BOSS_MAP_WEAPONS-1);
					boss_map[center] = w;
					boss_map[center+BACK_X*3] = w|0x800;

					manger_boss_weapons[boss_weapon_count].type = w;
					manger_boss_weapons[boss_weapon_count].x = boss_gen_col;
					manger_boss_weapons[boss_weapon_count].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);;
					++boss_weapon_count;
				}

				break;
			default:
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+v)|0x800;
				if(weapon_spawn)
				{
					u8 w=2+ERAPI_RandMax(BOSS_MAP_WEAPONS-2+weapon_laser);
					if(w == BOSS_TILE_LASER) weapon_laser=0;
					boss_map[center] = w;
					boss_map[center+BACK_X*4] = w|0x800;

					manger_boss_weapons[boss_weapon_count].type = w;
					manger_boss_weapons[boss_weapon_count].x = boss_gen_col;
					manger_boss_weapons[boss_weapon_count].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);
					++boss_weapon_count;
				}

				break;
		}

	}

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

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_BOSS, &slide);
}

void boss_spawn_init()
{
	rand_stable_boss(boss_gen_col);
	boss_len = ERAPI_RandMax(boss_level * BOSS_SIZE_RATIO_MAX)+5;
	boss_health=(boss_len*level_count)*4;
	boss_gen_col=0;
	boss_x_pos=15;
	boss_y_offset=0;
	boss_weapon_count=0;
	weapon_laser=1;
	enemy_spawn_allowed=0;
	boss_spawning_flag = 1;
}

u8 boss_tile_hit_check(u8 x, u8 y)
{
	return (
		boss_map[ x/8 + (
			(y-boss_y_offset)/8

		) * BACK_X ]
	);
}

void boss_get_pos_on(u8* x, u8* y)
{
	*x = (BACK_X-2-boss_len+ERAPI_RandMax(boss_len))*8;
	*y = ((7+ERAPI_RandMax(boss_level+2))*8)-(boss_y_offset*-1)+vertical_offset + 4;
}

void boss_update()
{
	// Spawn boss if gone far enough, is called mul
	if(boss_spawning_flag && !(boss_live)) boss_spawn();

	if(!boss_spawning_flag && !boss_live) return;

	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_BOSS,boss_x_pos/8,(boss_y_offset*-1)+vertical_offset);

	if((8+boss_level + boss_y_offset/8) > tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) --boss_y_offset;

	if((8+boss_level + boss_y_offset/8) < tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) ++boss_y_offset;

	if(!boss_live) return;

	// fire weapons
	for(u8 i=0;i<BOSS_WEAPON_MAX;++i)
	{
		if(manger_boss_weapons[i].cooldown)
		{
			--manger_boss_weapons[i].cooldown;
			continue;
		}

		u8 by = ((7+manger_boss_weapons[i].alt+manger_boss_weapons[i].alt*boss_level)*8)-(boss_y_offset*-1)+vertical_offset + 4;
		manger_boss_weapons[i].alt = !manger_boss_weapons[i].alt;

		if(manger_boss_weapons[i].type == BOSS_TILE_SINGLE)
		{
			bullet_fire(0, 3, (BACK_X-boss_len-3+manger_boss_weapons[i].x)*8, by,3,BULLET_ENEMY);
			manger_boss_weapons[i].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);
		}

		if(manger_boss_weapons[i].type == BOSS_TILE_SPREAD)
		{
			bullet_fire(20, 3, (BACK_X-boss_len-3+manger_boss_weapons[i].x )*8, by,3,BULLET_ENEMY);
			bullet_fire(235, 3,(BACK_X-boss_len-3+manger_boss_weapons[i].x)*8, by,3,BULLET_ENEMY);
			manger_boss_weapons[i].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);
		}
	}

	if (boss_health < 0)
	{
		level_next();
		boss_live = 0;
		enemy_spawn_allowed=1;
		u8 x,y;
		for (u8 i= 0;i < 5;++i)
		{
			boss_get_pos_on(&x, &y);
			effect_explode(x,y);
		}

		boss_get_pos_on(&x, &y);
		powerup_droptype(x,y,POWERUP_COOLDOWN);
		boss_get_pos_on(&x, &y);
		powerup_droptype(x,y,POWERUP_SHIELD);
		boss_get_pos_on(&x, &y);
		powerup_droptype(x,y,POWERUP_SPREAD);

		enemy_clean();
		boss_init();
	}
}

void boss_damage(u8 damage)
{
	boss_health-=damage;
	// TODO - change palette to flash
}

void boss_init()
{
	boss_spawning_flag=0;
	boss_live=0;
	boss_len=0,boss_gen_col=0,boss_x_pos=0;
	boss_y_offset=0;
	boss_spawn_distance=0;
	boss_weapon_allowance=0;
	boss_weapon_count=0;
	boss_weapon_min=0;
	weapon_laser=0;
	boss_health=0;

	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_X;++y)
		{
			boss_map[x + (y * BACK_X) ]=0;
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

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_BOSS, &slide);
	ERAPI_LayerShow(BACKGROUND_LAYER_BOSS);

	for(u8 i=0;i<BOSS_WEAPON_MAX;++i)
	{
		manger_boss_weapons[i].type = 0;
		manger_boss_weapons[i].alt = 0;
	}

}
