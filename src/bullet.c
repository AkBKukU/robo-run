
#include "bullet.h"

ERAPI_SPRITE sprite_bullet = { bulletTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};

struct bullet_data manger_bullet[BULLET_MAX];

u8 bullet_count = 0;

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
		manger_bullet[i].angle = angle;
		manger_bullet[i].speed = speed;
		manger_bullet[i].hitcheck = frame_count;

		// Store next position based on angle
		manger_bullet[i].xu = -ERAPI_Cos(manger_bullet[i].angle, manger_bullet[i].speed);
		manger_bullet[i].yu = ERAPI_Sin(manger_bullet[i].angle, manger_bullet[i].speed);

		manger_bullet[i].type = type;
		manger_bullet[i].damage = damage;
		manger_bullet[i].handle = ERAPI_SpriteCreateCustom( 2, &sprite_bullet);
		ERAPI_SpriteSetType(manger_bullet[i].handle,SPRITE_PROJECTILE);

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			x,
			y-vertical_offset
		);
		++bullet_count;
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

		// Update bullet path
		manger_bullet[i].x += manger_bullet[i].xu;
		manger_bullet[i].y += manger_bullet[i].yu;

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

		// Dynamic delay based on number of bullets to prevent game lag
		if(manger_bullet[i].hitcheck > frame_count) continue;
		manger_bullet[i].hitcheck = frame_count + (bullet_count / BULLET_UPDATE_DELAY);

		// Update drawn position
		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			manger_bullet[i].x / 256,
			(manger_bullet[i].y /256)-vertical_offset
		);

		// Check for contact
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

			if(boss_live && boss_tile_hit_check(manger_bullet[i].x/256, manger_bullet[i].y/256))
			{
				boss_damage(manger_bullet[i].damage);
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
	--bullet_count;
}

void bullet_init()
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		manger_bullet[i].live = 0;
	}
	bullet_count = 0;
}

void bullet_clean()
{
	// Iterate over all bullets
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		// Continue if bullet is not in use
		if (!manger_bullet[i].live) continue;

		ERAPI_SpriteFree(manger_bullet[i].handle);
	}

	bullet_init();
}
