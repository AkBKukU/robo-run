
#include "bullet.h"

ERAPI_SPRITE sprite_bullet = { bulletTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};
ERAPI_SPRITE sprite_laser = {
		laserTiles , gfx_powerupSharedPal,
		LASER_LEN_PX/8, LASER_LEN_PY/8,
		1,
		1,
		(LASER_LEN_PX)-1, 8
		, 1};

struct bullet_data manger_bullet[BULLET_MAX];
struct laser_data manager_laser[LASER_MAX];

u8 bullet_count = 0;
/*
unsigned char laserTilesGen[LASER_LEN_PX*(8*3)];
unsigned char laser_color_order[LASER_LEN_PY]={
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,

		LASER_PX_1,
		LASER_PX_2,
		LASER_PX_3,
		LASER_PX_4,
		LASER_PX_4,
		LASER_PX_3,
		LASER_PX_2,
		LASER_PX_1,

		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0,
		LASER_PX_0
};
void laser_sprite_build()
{
	for (u8 t = 0; t < LASER_LEN_PX/8 ; t+=(LASER_LEN_PY*8))
	{
		for (u8 y = 0; y < LASER_LEN_PY ; ++y)
		{
			for (u8 x = 0; x < 8 ; ++x)
			{
				laserTilesGen[i]=LASER_PX_0;
				laserTilesGen[i+1]=LASER_PX_0;
			}
		}
	}
}
*/

u8 laser_fire(u8 angle, u8 x, u8 y, u8 damage, u8 type)
{
	// Iterate over all lasers
	for ( u8 i = 0; i < LASER_MAX; ++i )
	{
		if(manager_laser[i].live) continue;
		s16 bi = -1;
		u8 laser_limit = BULLET_MAX-(LASER_LEN_COUNT * LASER_MAX);
		for (u8 b = BULLET_MAX-LASER_LEN_COUNT ; b >= laser_limit; b-=LASER_LEN_COUNT )
		{

			if (manger_bullet[b].type == BULLET_LASER) continue;
			bi = b;
			break;
		}

		if (bi == -1) return 0;


#ifdef DEBUG_MGBA
	mgba_print_string("Laser bullet found:");
	mgba_print_num(bi);
#endif
		manager_laser[i].live = 1;
		manager_laser[i].x = x;
		manager_laser[i].y = y;
		manager_laser[i].hitcheck = LASER_HITCHECK_DELAY;
		manager_laser[i].type = type;
		manager_laser[i].angle = angle;
		manager_laser[i].damage = damage;
		manager_laser[i].b_index = bi;

		// Clear and setup laser in bullet space
		for ( u8 b = bi; b < bi+LASER_LEN_COUNT; ++b )
		{
			// Free all bullets from i to laser count
			if(manger_bullet[b].live)
				bullet_free(b);
			// create new bullets of lasers
			manger_bullet[b].live = 1;
			manger_bullet[b].damage = damage;
			manger_bullet[b].type = BULLET_LASER;
			manger_bullet[b].xu = 0;
			manger_bullet[b].yu = 0;
			++bullet_count;
			// create and stretch sprites
			manger_bullet[b].handle = ERAPI_SpriteCreateCustom( 2, &sprite_laser);
			ERAPI_SpriteShow(manger_bullet[b].handle);
			ERAPI_SpriteAutoRotateByTime(manger_bullet[b].handle,0,0);
			//ERAPI_HANDLE_SpriteAutoScaleWidthUntilSize(manger_bullet[b].handle,1,1);
			ERAPI_SpriteSetType(manger_bullet[b].handle,SPRITE_PROJECTILE);
		}
		laser_update(i, x, y,manager_laser[i].angle);

		// Return laser index for later updating
		return i;
	}
}

void laser_update(u8 laser_id,  u8 x, u8 y, u8 angle)
{
	// Update laser manager
	manager_laser[laser_id].angle = angle;
	manager_laser[laser_id].x = x;
	manager_laser[laser_id].y = y;

	--manager_laser[laser_id].hitcheck;

	// Go over all bullets for the laser
	for ( u8 b = manager_laser[laser_id].b_index; b < manager_laser[laser_id].b_index+LASER_LEN_COUNT; ++b )
	{
		// Determine the max distance a laser can be placed
		s16 dist_max =LASER_LEN_PX/2+((b-manager_laser[laser_id].b_index)*LASER_LEN_PX);
		s16 dist =LASER_LEN_PX/2;
		u8 bounds_check=1;
		manger_bullet[b].angle = -angle;
		// Check laser position is valid and move it farther until it isn't
		// NOTE - This is a workaround for preventing lasers from wrapping around the screen
		while(bounds_check)
		{
			manger_bullet[b].xu = -ERAPI_Cos(manager_laser[laser_id].angle, dist );
			manger_bullet[b].yu =  ERAPI_Sin(manager_laser[laser_id].angle, dist );
			manger_bullet[b].x = (manager_laser[laser_id].x<<8) - manger_bullet[b].xu;
			manger_bullet[b].y = (manager_laser[laser_id].y<<8) + manger_bullet[b].yu;
			if(
				// Bounds checks inset slightly
				(manger_bullet[b].x>>8 < 240) &&
				(manger_bullet[b].x>>8 > 16) &&
				((manger_bullet[b].y>>8)-vertical_offset < 160) &&
				((manger_bullet[b].y>>8)-vertical_offset > 8) &&
				dist < dist_max
			){
				// Move laser farther
				dist+=8;
			}else{
				// At max viable distance
				bounds_check=0;
			}
		}

		// Rotate sprite to match fire angle
		ERAPI_SpriteAutoRotateUntilAngle(manger_bullet[b].handle, -angle,1);
		ERAPI_SpriteShow(manger_bullet[b].handle);
		// Update drawn position
		// NOTE - This bypasses the bullet drawing delay to make the sprites all line up properly
		/*
		ERAPI_SetSpritePos(
			manger_bullet[b].handle,
			(manger_bullet[b].x / 256),
			((manger_bullet[b].y /256)-vertical_offset)
		);
		*/

		if(manager_laser[laser_id].hitcheck == 0)
		{
			if(manager_laser[laser_id].type == BULLET_PLAYER)
			{
				// Check for hit against enemy bullet
				u32 enemy; // FIXME - This may cause crashes by not handling simultaneous collisions
				if (ERAPI_SpriteFindCollisions(manger_bullet[b].handle,SPRITE_ENEMY,&enemy))
				{
					enemy_damage(enemy,1);
				}
			}
			if(manager_laser[laser_id].type == BULLET_ENEMY)
			{
				// Check for hit against enemy bullet
				u32 enemy; // FIXME - This may cause crashes by not handling simultaneous collisions
				if (ERAPI_SpriteFindCollisions(manger_bullet[b].handle,SPRITE_PLAYER,&enemy))
				{
					player_damage(enemy_bullet_damage);
				}
				manager_laser[laser_id].hitcheck = LASER_HITCHECK_DELAY;
			}
		}
	}
	if(manager_laser[laser_id].type == BULLET_PLAYER)
	{
		if(manager_laser[laser_id].hitcheck == 0)
		{
			if(boss_live && boss_tile_hit_check(224, manager_laser[laser_id].y))
			{
				boss_damage(manager_laser[laser_id].damage);
			}
			manager_laser[laser_id].hitcheck = LASER_HITCHECK_DELAY;
		}
	}
}

void laser_relese(u8 laser_id)
{

#ifdef DEBUG_MGBA
	mgba_print_string("Laser removal:");
	mgba_print_num(laser_id);
#endif
	// Clear and setup laser in bullet space
	for ( u8 b = manager_laser[laser_id].b_index; b < manager_laser[laser_id].b_index+LASER_LEN_COUNT; ++b )
	{
		// create new bullets of lasers
		bullet_free(b);
#ifdef DEBUG_MGBA
	mgba_print_string("bullet removal:");
	mgba_print_num(b);
#endif
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
		manger_bullet[i].handle = ERAPI_SpriteCreateCustom( 2|0x80, &sprite_bullet);
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
				(manger_bullet[i].x > 240<<8) ||
				(manger_bullet[i].y > (BACK_Y * 8)<<8)
			){
				bullet_free(i);
				continue;
			}
		}

		/*
		if (ERAPI_Mod(frame_count,2) == 1 && ERAPI_Mod(i,2) == 1 )
		{
			ERAPI_SpriteShow(manger_bullet[i].handle);
		}else if (ERAPI_Mod(frame_count,2) == 1 && ERAPI_Mod(i,2) == 0 )
		{
			ERAPI_SpriteHide(manger_bullet[i].handle);
		}else if (ERAPI_Mod(frame_count,2) == 0 && ERAPI_Mod(i,2) == 0 )
		{
			ERAPI_SpriteShow(manger_bullet[i].handle);
		}else if (ERAPI_Mod(frame_count,2) == 0 && ERAPI_Mod(i,2) == 1 )
		{
			ERAPI_SpriteHide(manger_bullet[i].handle);
		}
		*/

		// Dynamic delay based on number of bullets to prevent game lag
		if(manger_bullet[i].hitcheck > frame_count) continue;
		manger_bullet[i].hitcheck = frame_count + ERAPI_Div(bullet_count , BULLET_UPDATE_DELAY);

		/*
		// Update drawn position for non-lasers
		if (manger_bullet[i].type == BULLET_LASER)
		{
#ifdef DEBUG_MGBA
	mgba_print_string("laser angle:");
	mgba_print_num(manger_bullet[i].angle);
#endif
			ERAPI_SpriteAutoRotateUntilAngle(manger_bullet[i].handle, manger_bullet[i].angle,1);
		}
		*/

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			(manger_bullet[i].x / 256),
			((manger_bullet[i].y /256)-vertical_offset)
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
		manger_bullet[i].live = 0;

		ERAPI_SpriteFree(manger_bullet[i].handle);
	}

	// Iterate over all lasers
	for ( u8 i = 0; i < LASER_MAX; ++i )
	{
		// Continue if bullet is not in use
		if (manager_laser[i].live)
			laser_relese(i);
	}

	bullet_init();
}
