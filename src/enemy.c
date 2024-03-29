
#include "enemy.h"
struct enemy_data manger_enemy[ENEMY_MAX];
ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfx_enemySharedPal, 2, 2, 6, 4, 0, 2, 1};

struct enemy_drops manager_cooldown;
struct enemy_drops manager_shield;
struct enemy_drops manager_spread;
ERAPI_SPRITE sprite_powerup_cooldown = { powerup_cooldownTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};
ERAPI_SPRITE sprite_powerup_shield = { powerup_shieldTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;
ERAPI_SPRITE sprite_powerup_spread = { powerup_spreadTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;

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
			// Check if already defeated
			if (manger_enemy[i].live > 1) return;
			// Damage enemy
			manger_enemy[i].health-=damage;
			// If defeated start death animation
			if (manger_enemy[i].health < 1)
			{
				player_score+=10;
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
	if(enemy_spawn_allowed)
	{
		if(enemy_spawn_next[ENEMY_TYPE_LIGHT] < distance_tiles) enemy_spawn(ENEMY_TYPE_LIGHT);
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
				enemy_drop(i);
				--enemy_spawn_count[manger_enemy[i].type];
				ERAPI_SpriteFree(manger_enemy[i].handle);
			}
			continue;
		}

		// Check fire cooldown
		--manger_enemy[i].cooldown;
		if(!manger_enemy[i].cooldown)
		{
			bullet_fire(0, 3, manger_enemy[i].x-8, manger_enemy[i].y,manger_enemy[i].damage,BULLET_ENEMY);
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
	// Power ups
	if(manager_cooldown.live)
	{
		--manager_cooldown.x;
		ERAPI_SetSpritePos(
			manager_cooldown.handle,
			manager_cooldown.x/2,
			manager_cooldown.y/2-vertical_offset
		);
		if(manager_cooldown.x < -4)
		{
			manager_cooldown.live = 0;
			ERAPI_SpriteFree(manager_cooldown.handle);
		}
	}
	if(manager_spread.live)
	{
		--manager_spread.x;
		ERAPI_SetSpritePos(
			manager_spread.handle,
			manager_spread.x/2,
			manager_spread.y/2-vertical_offset
		);
		if(manager_spread.x < -4)
		{
			manager_spread.live = 0;
			ERAPI_SpriteFree(manager_spread.handle);
		}
	}
	if(manager_shield.live)
	{
		--manager_shield.x;
		ERAPI_SetSpritePos(
			manager_shield.handle,
			manager_shield.x/2,
			manager_shield.y/2-vertical_offset
		);
		if(manager_shield.x < -4)
		{
			manager_shield.live = 0;
			ERAPI_SpriteFree(manager_shield.handle);
		}
	}
}

void enemy_drop(u8 i)
{
	u8 type = 0;
	switch(ERAPI_RandMax(100))
	{
		case 1:
		case 2:
			// Cooldown
			if(manager_cooldown.live == 1) return;
			manager_cooldown.live = 1;
			manager_cooldown.x = manger_enemy[i].x*2;
			manager_cooldown.y = manger_enemy[i].y*2;
			manager_cooldown.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_cooldown);
			ERAPI_SpriteSetType(manager_cooldown.handle,SPRITE_COOL);
			ERAPI_SetSpritePos(
				manager_cooldown.handle,
				manager_cooldown.x,
				manager_cooldown.y-vertical_offset
			);
			return;
		case 3:
		case 4:
			// Spread
			if(manager_spread.live == 1) return;
			manager_spread.live = 1;
			manager_spread.x = manger_enemy[i].x*2;
			manager_spread.y = manger_enemy[i].y*2;
			manager_spread.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_spread);
			ERAPI_SpriteSetType(manager_spread.handle,SPRITE_SPREAD);
			ERAPI_SetSpritePos(
				manager_spread.handle,
				manager_spread.x,
				manager_spread.y-vertical_offset
			);
			return;
		case 5:
			// Shield
			if(manager_shield.live == 1) return;
			manager_shield.live = 1;
			manager_shield.x = manger_enemy[i].x*2;
			manager_shield.y = manger_enemy[i].y*2;
			manager_shield.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_shield);
			ERAPI_SpriteSetType(manager_shield.handle,SPRITE_SHIELD);
			ERAPI_SetSpritePos(
				manager_shield.handle,
				manager_shield.x,
				manager_shield.y-vertical_offset
			);
			return;
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

		// Prevent spawning on sold tile
		u8 y = (8+ERAPI_RandMax(172));
		while(tunnelslide[ (31) + (y/8 * BACK_X) ])
			y = (8+ERAPI_RandMax(172));

		// TODO - All these values shoudl scale with distance
		manger_enemy[i].x = 248;
		manger_enemy[i].y = y;
		manger_enemy[i].t = 0;
		manger_enemy[i].health = 3;
		manger_enemy[i].damage = 3;
		manger_enemy[i].live = 1;
		manger_enemy[i].cooldown = ERAPI_RandMax(240);
		manger_enemy[i].type = spawn_type;
		manger_enemy[i].movement = ENEMY_PATH_STRAIGHT;

 		manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 1, &sprite_enemy_light);
 		ERAPI_SpriteSetType(manger_enemy[i].handle,SPRITE_ENEMY);
 		ERAPI_SetSpriteFrame(manger_enemy[i].handle,1);

		++enemy_spawn_count[spawn_type];
		// TODO - the 20 and 50 should scale with distance for difficulty
		enemy_spawn_next[spawn_type] = distance_tiles + 20+ERAPI_RandMax(40);
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
