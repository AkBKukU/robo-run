
#include "enemy.h"
struct enemy_data manger_enemy[ENEMY_MAX];
ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfxSharedPal, 2, 2, 6, 4, 0, 2, 1};

u32 enemy_spawn_next[ENEMY_TYPE_COUNT];
u8 enemy_spawn_count[ENEMY_TYPE_COUNT];
u8 enemy_spawn_max[ENEMY_TYPE_COUNT];

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
			// Check if already defeated
			if (manger_enemy[i].live > 1) return;
			// Damage enemy
			manger_enemy[i].health-=damage;
			// If defeated start death animation
			if (manger_enemy[i].health < 1)
			{
				manger_enemy[i].live = 2;
				manger_enemy[i].health=0;
				ERAPI_SetSpriteFrame(hit_sprite,3);
				ERAPI_SpriteAutoAnimate(hit_sprite,6,18);
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
	// Check to spawn new enemies
	if(enemy_spawn_next[ENEMY_TYPE_LIGHT] < distance_tiles) enemy_spawn(ENEMY_TYPE_LIGHT);
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
				--enemy_spawn_count[manger_enemy[i].type];
				ERAPI_SpriteFree(manger_enemy[i].handle);
			}
			continue;
		}

		// Check fire cooldown
		--manger_enemy[i].cooldown;
		if(!manger_enemy[i].cooldown)
		{
			bullet_fire(0, 3, manger_enemy[i].x-8, manger_enemy[i].y);
			manger_enemy[i].cooldown = ERAPI_RandMax(240);
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
			manger_enemy[i].x = 240-ERAPI_Div(manger_enemy[i].t,5);
			break;
	}
}

void enemy_spawn(u8 spawn_type)
{
	// Check all enemies
	if(enemy_spawn_count[spawn_type] >= enemy_spawn_max[spawn_type] ) return;
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		// If active, skip
		if (manger_enemy[i].live) continue;

		manger_enemy[i].x = 248;
		manger_enemy[i].y = 8+ERAPI_RandMax(172);
		manger_enemy[i].t = 0;
		manger_enemy[i].health = 3;
		manger_enemy[i].live = 1;
		manger_enemy[i].cooldown = ERAPI_RandMax(240);
		manger_enemy[i].type = spawn_type;
		manger_enemy[i].movement = ENEMY_PATH_STRAIGHT;

 		manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 0, &sprite_enemy_light);
 		ERAPI_SpriteSetType(manger_enemy[i].handle,SPRITE_ENEMY);
 		ERAPI_SetSpriteFrame(manger_enemy[i].handle,1);

		++enemy_spawn_count[spawn_type];
		// TODO - the 20 and 50 should scale with distance for difficulty
		enemy_spawn_next[spawn_type] = distance_tiles + 20+ERAPI_RandMax(50);
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
		enemy_spawn_next[i] = 0;
		enemy_spawn_count[i] = 0;
	}
	enemy_spawn_max[ENEMY_TYPE_LIGHT] = 5;
}
