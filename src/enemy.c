
#include "enemy.h"
struct enemy_data manger_enemy[ENEMY_MAX];
ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfxSharedPal, 2, 2, 6, 4, 0, 2, 1};

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
				ERAPI_SpriteHide(manger_enemy[i].handle);
				// TODO - Add logic to remove killed enemies after animation complete
				continue;
			}
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
	gui_print_score(manger_enemy[0].x);
	switch(manger_enemy[i].movement)
	{
		case ENEMY_PATH_STRAIGHT:
			++manger_enemy[i].t;
			manger_enemy[i].x = 240-ERAPI_Div(manger_enemy[i].t,5);
			break;
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
		manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 0, &sprite_enemy_light);
		ERAPI_SpriteSetType(manger_enemy[i].handle,SPRITE_ENEMY);
		ERAPI_SetSpriteFrame(manger_enemy[i].handle,1);
	}

	manger_enemy[0].x=px;
	manger_enemy[0].y=py;
	manger_enemy[0].live=1;
	manger_enemy[0].health=3;

	manger_enemy[1].x=px+60;
	manger_enemy[1].y=py+32;
	manger_enemy[1].live=1;
	manger_enemy[1].health=3;
}
