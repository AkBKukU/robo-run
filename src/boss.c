#include "boss.h"

u8 boss_spawning_flag=0;
u8 boss_live=0;
u8 boss_len=0, boss_level=1,boss_gen_col=0,boss_x_pos=0;
s16 boss_y_offset=0;
u8 boss_weapon_count=0;
u8 weapon_laser=0;
s32 boss_health=0;

struct boss_weapons manger_boss_weapons[BOSS_WEAPON_MAX];

unsigned short boss_map[BACK_X*32];

void boss_spawn()
{
	rand_stable_boss(boss_gen_col);

	// Shift boss left until tiles need slid
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

	// Generate a new column of boss
	if (boss_gen_col==0)
	{
		// Cockpit - First tile
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
		// Engine mount - Second to last tile
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
		// Engine - Last tile
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
	} else if(boss_gen_col < boss_len)
	{
		// Body - All middle tiles
		u8 v=ERAPI_RandMax(BOSS_MAP_WIDTH-3)+1;
		u8 weapon_spawn = (ERAPI_RandMax(boss_len-3) >= boss_weapon_count);
		switch(boss_level)
		{
			case 1:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				break;
			case 2:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;

				break;
			default:
			case 3:
				boss_map[center+BACK_X] = BOSS_MAP_WIDTH+v;
				boss_map[center+BACK_X*2] = BOSS_MAP_WIDTH*2+v;
				boss_map[center+BACK_X*3] = (BOSS_MAP_WIDTH+v)|0x800;

				break;
		}
		if(weapon_spawn)
		{
			u8 w=1+ERAPI_RandMax(BOSS_MAP_WEAPONS-1+weapon_laser);
			if(w == BOSS_TILE_LASER) weapon_laser=0;
			boss_map[center] = w;
			boss_map[center+BACK_X*(boss_level+1)] = w|0x800;

			manger_boss_weapons[boss_weapon_count].type = w;
			manger_boss_weapons[boss_weapon_count].x = boss_gen_col;
			manger_boss_weapons[boss_weapon_count].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);
			++boss_weapon_count;
		}

	}

	++boss_gen_col;
	// If boss is fully spawned change state
	if(boss_gen_col > boss_len+1)
	{
		boss_live = 1;
		boss_spawning_flag = 0;
		ERAPI_SoundPause(SND_BOSS_SPAWN);
		ERAPI_PlaySoundSystem(SND_MUSIC_BOSS);
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
	// Standard boss values
	boss_gen_col=0;
	boss_x_pos=15;
	boss_y_offset=0;
	boss_weapon_count=0;
	weapon_laser=1;
	boss_spawning_flag = 1;

	// Get boss size
	rand_stable_boss(boss_gen_col);
	boss_len = ERAPI_RandMax(boss_level * BOSS_SIZE_RATIO_MAX)+5;

	// Set health based on level
	boss_health=(boss_len*save.level)*4;

	// Disable enemy spawning
	enemy_spawn_allowed=0;

	// Change music
	ERAPI_SoundPause(SND_MUSIC_6);
	ERAPI_PlaySoundSystem(SND_BOSS_SPAWN);
}

u8 boss_tile_hit_check(u8 x, u8 y)
{
	// Returns the value of a boss tile after converting from pixels to tiles
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

	// Return if boss is inactive
	if(!boss_spawning_flag && !boss_live) return;

	// Set boss position using background layer
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_BOSS,boss_x_pos/8,(boss_y_offset*-1)+vertical_offset);

	// Position boss towards center of tunnel
	if((8+boss_level + boss_y_offset/8) > tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) --boss_y_offset;

	if((8+boss_level + boss_y_offset/8) < tunnel_center(31))
		if(!ERAPI_Mod(frame_count,8)) ++boss_y_offset;


	// Skip boss weapons
	if(!boss_live) return;

	// fire weapons
	for(u8 i=0;i<BOSS_WEAPON_MAX;++i)
	{
		// Check if ready to fire again
		if(manger_boss_weapons[i].cooldown)
		{
			--manger_boss_weapons[i].cooldown;
			continue;
		}
		// Player fire sound
		ERAPI_PlaySoundSystem(SND_ENEMY_FIRE);
		// Set next cooldown
		manger_boss_weapons[i].cooldown = BOSS_WEAPON_COOLDOWN_MIN+ERAPI_RandMax(BOSS_WEAPON_COOLDOWN_MAX);

		// Determine Y position for alternating firing
		u8 by = ((7+manger_boss_weapons[i].alt*(boss_level+1))*8)+boss_y_offset+vertical_offset + 4;
		manger_boss_weapons[i].alt = !manger_boss_weapons[i].alt;

		// Fire based on weapon type
		if(manger_boss_weapons[i].type == BOSS_TILE_SINGLE)
		{
			bullet_fire(0, 3, (BACK_X-boss_len-3+manger_boss_weapons[i].x)*8, by,3,BULLET_ENEMY);
		}

		if(manger_boss_weapons[i].type == BOSS_TILE_SPREAD)
		{
			bullet_fire(20, 3, (BACK_X-boss_len-3+manger_boss_weapons[i].x )*8, by,3,BULLET_ENEMY);
			bullet_fire(235, 3,(BACK_X-boss_len-3+manger_boss_weapons[i].x)*8, by,3,BULLET_ENEMY);
		}
	}
}

void boss_damage(u8 damage)
{
	boss_health-=damage;

	// Boss is defeated
	if (boss_health < 0)
	{
		// End boss music
		ERAPI_SoundPause(SND_MUSIC_BOSS);

		// Setup for next level
		level_next();

		// Spawn explosion effects over boss
		u8 x,y;
		for (u8 i= 0;i < 5;++i)
		{
			boss_get_pos_on(&x, &y);
			effect_explode(x,y,8*i);
		}

		// Drop all powerups over boss
		for (u8 i = 0; i < POWERUP_COUNT; ++i)
		{
			boss_get_pos_on(&x, &y);
			powerup_droptype(x,y,i);
		}

		// Play win sound
		ERAPI_PlaySoundSystem(SND_DEFEAT_BOSS);
		// Get new enemy spawn values
		enemy_clean();
		enemy_init();
		// Reset boss
		boss_init();
		// Restart normal music
		ERAPI_PlaySoundSystem(SND_MUSIC_6);
	}
}

void boss_init()
{
	// Set all control values
	boss_spawning_flag=0;
	boss_live=0;
	enemy_spawn_allowed=1;

	// Clear boss tilemap
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

	// Clear all boss weapons
	for(u8 i=0;i<BOSS_WEAPON_MAX;++i)
	{
		manger_boss_weapons[i].type = 0;
		manger_boss_weapons[i].alt = 0;
	}

}
