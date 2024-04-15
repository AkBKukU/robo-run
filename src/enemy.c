
#include "enemy.h"
struct enemy_data manger_enemy[ENEMY_MAX];
ERAPI_SPRITE sprite_enemy_rock = { emy_0_rockTiles, gfx_enemySharedPal, 4, 4, 3, 4, 0, 2, 1};
ERAPI_SPRITE sprite_enemy_light = { emy_1_lightTiles, gfx_enemySharedPal, 2, 2, 6, 4, 0, 2, 1};


u32 enemy_spawn_next[ENEMY_TYPE_COUNT];
u8 enemy_spawn_count[ENEMY_TYPE_COUNT];
u8 enemy_spawn_max[ENEMY_TYPE_COUNT];

u8 enemy_spawn_allowed=1;

// Damage enemey using sprite handle as ID
void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage)
{
	// Check all enemies
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		// If not active, skip
		if (!manger_enemy[i].live) continue;
		// Find enemy to damage
		if (manger_enemy[i].handle == hit_sprite)
		{
			if(manger_enemy[i].type == ENEMY_TYPE_ROCK) return;
			// Check if already defeated
			if (manger_enemy[i].live > 1) return;
			// Damage enemy
			manger_enemy[i].health-=damage;
			// If defeated start death animation
			if (manger_enemy[i].health < 1)
			{
				save.score+=10;
				manger_enemy[i].live = 2;
				manger_enemy[i].health=0;
				ERAPI_SetSpriteFrame(hit_sprite,3);
				ERAPI_SpriteAutoAnimate(hit_sprite,6,18);
				ERAPI_PlaySoundSystem(SND_DEFEAT_ENEMY);
			// Flash if not defeated
			}else{
				ERAPI_SetSpriteFrame(hit_sprite,0);
				ERAPI_SpriteAutoAnimate(hit_sprite,3,6);
			}
			return;
		}
	}
}

void enemy_update()
{
	rand_stable_map();
	// Check to spawn new enemies
	if(enemy_spawn_allowed)
	{
		for ( u8 i = 0; i < ENEMY_TYPE_COUNT; ++i )
			if(enemy_spawn_next[i] < distance_tiles) enemy_spawn(i);
	}
	// Check all enemies
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		// If not active, skip
		if (!manger_enemy[i].live) continue;

		// If enemy defeated wait for death animation before removing
		if(manger_enemy[i].live == 2)
		{
			--manger_enemy[i].health;
			if(manger_enemy[i].health == -24)
			{
				// animation finished
				manger_enemy[i].live=0;
				powerup_droptype(manger_enemy[i].x, manger_enemy[i].y, manger_enemy[i].drop_type);
				--enemy_spawn_count[manger_enemy[i].type];
				ERAPI_SpriteFree(manger_enemy[i].handle);
			}
			continue;
		}

		// Check fire cooldown
		--manger_enemy[i].cooldown;
		if(!manger_enemy[i].cooldown && manger_enemy[i].type != ENEMY_TYPE_ROCK)
		{
			bullet_fire(0, 3, manger_enemy[i].x-8, manger_enemy[i].y,manger_enemy[i].damage,BULLET_ENEMY);
			ERAPI_PlaySoundSystem(SND_ENEMY_FIRE);
			manger_enemy[i].cooldown = ERAPI_RandMax(240);
		}

		// Check if enemy is off screen
		if (manger_enemy[i].x < -32)
		{
			manger_enemy[i].live=0;
			--enemy_spawn_count[manger_enemy[i].type];
			ERAPI_SpriteFree(manger_enemy[i].handle);
			continue;
		}

		// Update position
		// TODO - Add path movement
		enemy_path(i);
		ERAPI_SetSpritePos(
			manger_enemy[i].handle,
			manger_enemy[i].x,
			manger_enemy[i].y-vertical_offset
		);
	}

}


void enemy_path(u8 i)
{
	switch(manger_enemy[i].movement)
	{
		case ENEMY_PATH_STRAIGHT:
			++manger_enemy[i].t;
			if (manger_enemy[i].type == ENEMY_TYPE_ROCK)
			{
				manger_enemy[i].x = 240-ERAPI_Div(manger_enemy[i].t,8);
			} else {
				manger_enemy[i].x = 240-ERAPI_Div(manger_enemy[i].t,5);
			}
			break;
	}
}

void enemy_spawn(u8 spawn_type)
{
	// Check all enemies
	rand_stable_map();
	if(enemy_spawn_count[spawn_type] >= enemy_spawn_max[spawn_type] ) return;
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		// If active, skip
		if (manger_enemy[i].live) continue;

		// Prevent spawning on sold tile
		u8 tile_giveup = 10;
		u8 y = (8+ERAPI_RandMax(172));
		while(tunnelslide[ (31) + (y/8 * BACK_X) ] && tile_giveup)
		{
			y = (8+ERAPI_RandMax(172));
			--tile_giveup;
		}

		// TODO - All these values shoudl scale with distance
		manger_enemy[i].x = 250;
		manger_enemy[i].y = y;
		manger_enemy[i].t = 0;
		manger_enemy[i].damage = 3;
		manger_enemy[i].drop_type = powerup_drop_chance();
		manger_enemy[i].live = 1;
		manger_enemy[i].cooldown = ERAPI_RandMax(240);
		manger_enemy[i].type = spawn_type;
		manger_enemy[i].movement = ENEMY_PATH_STRAIGHT;

		switch(spawn_type)
		{
			case ENEMY_TYPE_ROCK:
				manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 1, &sprite_enemy_rock);
				manger_enemy[i].health = 100;
				rand_true();
				ERAPI_SetSpriteFrame(manger_enemy[i].handle,ERAPI_Mod(frame_count,3));
				ERAPI_SpriteMirrorToggle(manger_enemy[i].handle,ERAPI_RandMax(3));
				ERAPI_SpriteAutoRotateByTime(manger_enemy[i].handle,(40+ERAPI_RandMax(90))*(ERAPI_RandMax(3)-1),0);
				break;
			case ENEMY_TYPE_LIGHT:
				manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 1, &sprite_enemy_light);
				manger_enemy[i].health = 3*(save.level/2);
				ERAPI_SetSpriteFrame(manger_enemy[i].handle,1);
				// Debug, uncomment to show powerup type by flipping sprite
				ERAPI_SpriteMirrorToggle(manger_enemy[i].handle,manger_enemy[i].drop_type);
				break;
		}
 		ERAPI_SpriteSetType(manger_enemy[i].handle,SPRITE_ENEMY);

		++enemy_spawn_count[spawn_type];
		// TODO - the 20 and 50 should scale with distance for difficulty
		rand_stable_map_var(manger_enemy[i].type);
		enemy_spawn_next[spawn_type] = distance_tiles + 20+ERAPI_RandMax(20);
		return;
	}
}

void enemy_init()
{
	// Initialize enemy structs
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		manger_enemy[i].x = 0;
		manger_enemy[i].y = 0;
		manger_enemy[i].live = 0;
		manger_enemy[i].movement = ENEMY_PATH_STRAIGHT;
	}


	// Initialize enemy tracking structs
	for ( u8 i = 0; i < ENEMY_TYPE_COUNT; ++i )
	{
		rand_stable_map_var(i);
		enemy_spawn_next[i] = distance_tiles + 20+ERAPI_RandMax(20);
		enemy_spawn_count[i] = 0;
	}
	enemy_spawn_max[ENEMY_TYPE_LIGHT] = 5;
	enemy_spawn_max[ENEMY_TYPE_ROCK] = 3;
	enemy_spawn_allowed=1;
}

void enemy_clean()
{
	// Check all enemies
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		// If not active, skip
		if (!manger_enemy[i].live) continue;

		manger_enemy[i].x = 0;
		manger_enemy[i].y = 0;
		manger_enemy[i].live = 0;
		manger_enemy[i].movement = ENEMY_PATH_STRAIGHT;
		ERAPI_SpriteFree(manger_enemy[i].handle);
	}
	// Initialize enemy tracking structs
	rand_stable_map();
	for ( u8 i = 0; i < ENEMY_TYPE_COUNT; ++i )
	{
		rand_stable_map_var(i);
		enemy_spawn_next[i] = distance_tiles + 20+ERAPI_RandMax(20);
		enemy_spawn_count[i] = 0;
	}
}
