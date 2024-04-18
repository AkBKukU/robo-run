
#include "bullet.h"

ERAPI_SPRITE sprite_bullet = { bulletTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};
ERAPI_SPRITE sprite_laser = {laserTiles , gfx_powerupSharedPal, 8, 1, 1, 1, 63, 8, 1};

struct bullet_data manger_bullet[BULLET_MAX];
struct laser_data manager_laser[LASER_MAX];

u8 bullet_count = 0;


u8 laser_fire(u8 angle, u8 x, u8 y, u8 damage, u8 type)
{

#ifdef DEBUG_MGBA
	mgba_print_string("firing ma laser");
#endif
	// Iterate over all lasers
	for ( u8 i = 0; i < LASER_MAX; ++i )
	{
		if(manager_laser[i].live) continue;

#ifdef DEBUG_MGBA
	mgba_print_string("found unused laser");
#endif
		s16 bi = -1;
		u8 laser_limit = BULLET_MAX-1-(LASER_LEN_COUNT * LASER_MAX);
		for (u8 b = BULLET_MAX-1-LASER_LEN_COUNT ; b > laser_limit; b-=LASER_LEN_COUNT )
		{

			if (manger_bullet[b].type == BULLET_LASER) continue;
			bi = b;
			break;
		}

		if (bi == -1) return 0;

		manager_laser[i].live = 1;
		manager_laser[i].x = x;
		manager_laser[i].y = y;
		manager_laser[i].angle = angle;
		manager_laser[i].b_index = bi;
#ifdef DEBUG_MGBA
	mgba_print_string("found bullet");
#endif

		// Clear and setup laser in bullet space
		for ( u8 b = bi; b < bi+LASER_LEN_COUNT; ++b )
		{
			// Free all bullets from i to laser count
			if(manger_bullet[b].live)
				bullet_free(b);
			// create new bullets of lasers
			manger_bullet[b].live = 1;
			manger_bullet[b].type = BULLET_LASER;
			manger_bullet[b].xu = 0;
			manger_bullet[b].yu = 0;
			++bullet_count;
			// create and stretch sprites
#ifdef DEBUG_MGBA
	mgba_print_string("creating sprite");
#endif
			manger_bullet[b].handle = ERAPI_SpriteCreateCustom( 2, &sprite_laser);
			//ERAPI_HANDLE_SpriteAutoScaleWidthUntilSize(manger_bullet[b].handle,1,1);
			ERAPI_SpriteSetType(manger_bullet[b].handle,SPRITE_PROJECTILE);
		}
		laser_update(i, x, y,manager_laser[i].angle);

		// Made Laser
		return i;
	}
}

void laser_update(u8 laser_id,  u8 x, u8 y, u8 angle)
{
	manager_laser[laser_id].angle = angle;
	manager_laser[laser_id].x = x;
	manager_laser[laser_id].y = y;
	// Clear and setup laser in bullet space
	for ( u8 b = manager_laser[laser_id].b_index; b < manager_laser[laser_id].b_index+LASER_LEN_COUNT; ++b )
	{
		// create new bullets of lasers
		manger_bullet[b].x = (x+32+(b-manager_laser[laser_id].b_index)*64); // FIXME - only fires right
		manger_bullet[b].x = manger_bullet[b].x > 240 ? 240<<8 :manger_bullet[b].x<<8 ;
		manger_bullet[b].y = y<<8;
		/*
		u8 dist =2;
		manger_bullet[b].x = manager_laser[laser_id].x<<8 - ERAPI_Cos(manager_laser[laser_id].angle, dist );
		manger_bullet[b].y = manager_laser[laser_id].y<<8 + ERAPI_Sin(manager_laser[laser_id].angle, dist );
		ERAPI_SpriteAutoRotateUntilAngle(manger_bullet[b].handle, angle,1);
		*/
	}
}

void laser_relese(u8 laser_id)
{
	// Clear and setup laser in bullet space
	for ( u8 b = manager_laser[laser_id].b_index; b < manager_laser[laser_id].b_index+LASER_LEN_COUNT; ++b )
	{
		// create new bullets of lasers
		bullet_free(b);
	}

	manager_laser[laser_id].live = 0;
}

void bullet_fire(u8 angle, u8 speed, u8 x, u8 y, u8 damage, u8 type)
{
	// Iterate over all bullets
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		// Continue if bullet is in use
		if (manger_bullet[i].live == 1) continue;

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
		switch(type)
		{
			case BULLET_PLAYER:
			ERAPI_SpriteSetType(manger_bullet[i].handle,SPRITE_PROJECTILE);
			break;
			case BULLET_ENEMY:
			ERAPI_SpriteSetType(manger_bullet[i].handle,SPRITE_PROJECTILE_ENEMY);
			break;
		}
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

		if (manger_bullet[i].type != BULLET_LASER)
		{
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
		}else{

			// Check for hit against enemy bullet
			u32 enemy; // FIXME - This may cause crashes by not handling simultaneous collisions
			if (ERAPI_SpriteFindCollisions(manger_bullet[i].handle,SPRITE_ENEMY,&enemy))
			{
				enemy_damage(enemy,1);
			}
		}

		// Dynamic delay based on number of bullets to prevent game lag
		if(manger_bullet[i].hitcheck > frame_count) continue;
		manger_bullet[i].hitcheck = frame_count + ERAPI_Div(bullet_count , BULLET_UPDATE_DELAY);

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
	}
}

void bullet_free(u8 i)
{
	// Set bullet as free to reuse
	manger_bullet[i].live = 0;
	manger_bullet[i].type = 0;

	// NOTE - This works for now, but alternate frame rendering will
	//        require a check for this in the future
	ERAPI_SpriteFree(manger_bullet[i].handle);
	--bullet_count;
}

void bullet_free_sprite(ERAPI_HANDLE_SPRITE sprite)
{
	// Search for bullet by sprite handle to free it
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		if(manger_bullet[i].handle == sprite)
		{
			bullet_free(i);
			return;
		}
	}
}

void bullet_init()
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		manger_bullet[i].live = 0;
		manger_bullet[i].type = 0;
	}

	// Iterate over all lasers
	for ( u8 i = 0; i < LASER_MAX; ++i )
	{
		manager_laser[i].live = 0;
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
