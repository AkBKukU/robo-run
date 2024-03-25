
#include "gamestate.h"
ERAPI_HANDLE_REGION score_print;
u8 sysexit = 0, win = 0;
ERAPI_SPRITE sprite_bullet = { bulletTiles, gfxSharedPal, 1, 1, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE the_bullet;
struct bullet_data manger_bullet[BULLET_MAX];
void gui_print_score(u32 score)
{
	char num_print[8];
	citoa(score,num_print,10);
	ERAPI_ClearRegion(score_print);
	ERAPI_DrawText( score_print, 0, 0, num_print);
}

void bullet_fire(u8 angle, u8 speed, u8 x, u8 y)
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		if (manger_bullet[i].live == 1)continue;
		manger_bullet[i].live = 1;
		manger_bullet[i].x = x;
		manger_bullet[i].y = y;
		manger_bullet[i].speed = speed;
		manger_bullet[i].angle = angle;
		manger_bullet[i].handle = ERAPI_SpriteCreateCustom( 0, &sprite_bullet);
		ERAPI_SpriteSetType(manger_bullet[i].handle,SPRITE_PROJECTILE);

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			x,
			y-vertical_offset
		);
		return;
	}
}

void bullet_update()
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		if (!manger_bullet[i].live) continue;

		manger_bullet[i].x += -ERAPI_Cos(manger_bullet[i].angle, manger_bullet[i].speed) >> 8;
		manger_bullet[i].y += ERAPI_Sin(manger_bullet[i].angle, manger_bullet[i].speed) >> 8;


		if (
			(manger_bullet[i].x < 0) ||
			(manger_bullet[i].x > 240) ||
			(manger_bullet[i].y < 0) ||
			(manger_bullet[i].y > 160)
		){
			manger_bullet[i].live = 0;
			ERAPI_SpriteFree(manger_bullet[i].handle);
			continue;
		}

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			manger_bullet[i].x,
			manger_bullet[i].y-vertical_offset
		);

		// Check for contact against enemies
		u16 dist = 0;
		ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(manger_bullet[i].handle,SPRITE_ENEMY, &dist);
		if (dist < 10)
		{
			enemy_damage(hit_sprite,1);
			manger_bullet[i].live = 0;
			ERAPI_SpriteFree(manger_bullet[i].handle);
			continue;
		}

	}
}
