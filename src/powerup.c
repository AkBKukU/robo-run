
#include "powerup.h"

struct enemy_drops manager_drops[POWERUP_COUNT];
ERAPI_SPRITE sprite_powerup_cooldown = { powerup_cooldownTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};
ERAPI_SPRITE sprite_powerup_shield = { powerup_shieldTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;
ERAPI_SPRITE sprite_powerup_spread = { powerup_spreadTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;

void powerup_update()
{
	// Power ups
	for (u8 i = 0; i < POWERUP_COUNT; ++i)
	{
		if (!manager_drops[i].live) continue;

		// Move powerup until offscreen
		--manager_drops[i].x;
		if(manager_drops[i].x < -4)
		{
			manager_drops[i].live = 0;
			ERAPI_SpriteFree(manager_drops[i].handle);
			continue;
		}
		ERAPI_SetSpritePos(
				manager_drops[i].handle,
				manager_drops[i].x/2,
				manager_drops[i].y/2-vertical_offset
		);
	}
}

void powerup_droptype(u8 x, u8 y,u8 type)
{
	// Don't drop two of the same type
	if(manager_drops[type].live == 1 || type == POWERUP_COUNT) return;

	// Setup new powerup
	manager_drops[type].live = 1;
	manager_drops[type].x = x*2;
	manager_drops[type].y = y*2;
	switch(type)
	{
		case POWERUP_COOLDOWN:
			manager_drops[type].handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_cooldown);
			ERAPI_SpriteSetType(manager_drops[type].handle,SPRITE_COOL);
			break;
		case POWERUP_SPREAD:
			manager_drops[type].handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_spread);
			ERAPI_SpriteSetType(manager_drops[type].handle,SPRITE_SPREAD);
			break;
		case POWERUP_SHIELD:
			manager_drops[type].handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_shield);
			ERAPI_SpriteSetType(manager_drops[type].handle,SPRITE_SHIELD);
			break;
	}
	ERAPI_SetSpritePos(
			manager_drops[type].handle,
			manager_drops[type].x,
			manager_drops[type].y-vertical_offset
	);
	return;
}

void powerup_drop(u8 x, u8 y)
{
	// Get a powerup type to drop
	powerup_droptype(x, y, powerup_drop_chance());
}

u8 powerup_drop_chance()
{
	switch(ERAPI_RandMax(POWERUP_DROP_RATE))
	{
		case 1:
		case 2:
			return POWERUP_COOLDOWN;
		case 3:
		case 4:
			return POWERUP_SPREAD;
		case 5:
			return POWERUP_SHIELD;
	}
	return POWERUP_COUNT;
}

void powerup_clean()
{
	for (u8 i = 0; i < POWERUP_COUNT; ++i)
	{
		if(manager_drops[i].live)
		{
			manager_drops[i].live = 0;
			ERAPI_SpriteFree(manager_drops[i].handle);
		}else{
			manager_drops[i].live = 0;
		}
	}
}
