
#include "enemy.h"
struct enemy_data manger_enemy[ENEMY_MAX];
ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfxSharedPal, 2, 2, 6, 4, 0, 2, 1};

void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage)
{
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;
		if (manger_enemy[i].handle == hit_sprite)
		{
			manger_enemy[i].health-=damage;
			if (manger_enemy[i].health < 1)
			{
				manger_enemy[i].health=0;
				ERAPI_SetSpriteFrame(hit_sprite,3);
				ERAPI_SpriteAutoAnimate(hit_sprite,6,18);
				continue;
			}else{
				ERAPI_SetSpriteFrame(hit_sprite,0);
				ERAPI_SpriteAutoAnimate(hit_sprite,3,6);
			}
		}
	}
}

void enemy_update()
{
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;
		// TODO - Add logic to remove killed enemies after animation complete
		if(manger_enemy[i].health < 1)
		{
			--manger_enemy[i].health;
			if(manger_enemy[i].health == -24)
			{
				manger_enemy[i].live=0;
				ERAPI_SpriteHide(manger_enemy[i].handle);
				continue;
			}
		}

		ERAPI_SetSpritePos(
			manger_enemy[i].handle,
			manger_enemy[i].x,
			manger_enemy[i].y-vertical_offset
		);
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
