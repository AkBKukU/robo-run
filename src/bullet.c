
#include "bullet.h"

ERAPI_SPRITE sprite_bullet = { bulletTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 8, 8, 1};

struct bullet_data manger_bullet[BULLET_MAX];


void bullet_fire(u8 angle, u8 speed, u8 x, u8 y, u8 damage, u8 type)
{
	// Iterate over all bullets
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		// Continue if bullet is in use
		if (manger_bullet[i].live == 1)continue;

		// Prepare new bullet
		manger_bullet[i].live = 1;
		manger_bullet[i].x = x<<8;
		manger_bullet[i].y = y<<8;
		manger_bullet[i].speed = speed;
		manger_bullet[i].type = type;
		manger_bullet[i].angle = angle;
		manger_bullet[i].damage = damage;
		manger_bullet[i].handle = ERAPI_SpriteCreateCustom( 2, &sprite_bullet);
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
	// Iterate over all bullets
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		// Continue if bullet is not in use
		if (!manger_bullet[i].live) continue;

		// Calculate next position based on angle
		manger_bullet[i].x += -ERAPI_Cos(manger_bullet[i].angle, manger_bullet[i].speed);
		manger_bullet[i].y += ERAPI_Sin(manger_bullet[i].angle, manger_bullet[i].speed);

		// Check if bullet is in bounds
		if (
			(manger_bullet[i].x < 0) ||
			(manger_bullet[i].x > 240<<8) ||
			(manger_bullet[i].y < 0) ||
			(manger_bullet[i].y > (BACK_Y * 8)<<8)
		){
			bullet_free(i);
			continue;
		}


		// Update drawn position
		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			manger_bullet[i].x / 256,
			(manger_bullet[i].y /256)-vertical_offset
		);

		// Check for contact against enemies
		u16 dist = 0;
		if(manger_bullet[i].type == BULLET_PLAYER)
		{
			ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(manger_bullet[i].handle,SPRITE_ENEMY, &dist);
			if (dist > 0 && dist < 10)
			{
				enemy_damage(hit_sprite,1);
				bullet_free(i);
				continue;
			}
		}

		// Check for contact against player
		if(manger_bullet[i].type == BULLET_ENEMY)
		{
			ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(manger_bullet[i].handle,SPRITE_PLAYER, &dist);
			if (dist > 0 && dist < 10)
			{
				player_damage(manger_bullet[i].damage);
				bullet_free(i);
				continue;
			}
		}
	}
}

void bullet_free(u8 i)
{
	// Set bullet as free to reuse
	manger_bullet[i].live = 0;

	// NOTE - This works for now, but alternate frame rendering will
	//        require a check for this in the future
	ERAPI_SpriteFree(manger_bullet[i].handle);
}

void bullet_init()
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		manger_bullet[i].live = 0;
	}
}
