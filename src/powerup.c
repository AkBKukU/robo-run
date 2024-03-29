
#include "powerup.h"

struct enemy_drops manager_cooldown;
struct enemy_drops manager_shield;
struct enemy_drops manager_spread;
ERAPI_SPRITE sprite_powerup_cooldown = { powerup_cooldownTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};
ERAPI_SPRITE sprite_powerup_shield = { powerup_shieldTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;
ERAPI_SPRITE sprite_powerup_spread = { powerup_spreadTiles, gfx_powerupSharedPal, 1, 1, 1, 4, 1, 1, 1};;

void powerup_update()
{// Power ups
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

void powerup_droptype(u8 x, u8 y,u8 type)
{
	switch (type)
	{
		case POWERUP_COOLDOWN:
			// Cooldown
			if(manager_cooldown.live == 1) return;
			manager_cooldown.live = 1;
			manager_cooldown.x = x*2;
			manager_cooldown.y = y*2;
			manager_cooldown.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_cooldown);
			ERAPI_SpriteSetType(manager_cooldown.handle,SPRITE_COOL);
			ERAPI_SetSpritePos(
				manager_cooldown.handle,
				manager_cooldown.x,
				manager_cooldown.y-vertical_offset
			);
			return;
		case POWERUP_SHIELD:
			// Shield
			if(manager_shield.live == 1) return;
			manager_shield.live = 1;
			manager_shield.x = x*2;
			manager_shield.y = y*2;
			manager_shield.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_shield);
			ERAPI_SpriteSetType(manager_shield.handle,SPRITE_SHIELD);
			ERAPI_SetSpritePos(
				manager_shield.handle,
				manager_shield.x,
				manager_shield.y-vertical_offset
			);
			return;
		case POWERUP_SPREAD:
			// Spread
			if(manager_spread.live == 1) return;
			manager_spread.live = 1;
			manager_spread.x = x*2;
			manager_spread.y = y*2;
			manager_spread.handle = ERAPI_SpriteCreateCustom( 2, &sprite_powerup_spread);
			ERAPI_SpriteSetType(manager_spread.handle,SPRITE_SPREAD);
			ERAPI_SetSpritePos(
				manager_spread.handle,
				manager_spread.x,
				manager_spread.y-vertical_offset
			);
			return;
	}
}

void powerup_drop(u8 x, u8 y)
{
	u8 type = 0;
	switch(ERAPI_RandMax(10))
	{
		case 1:
		case 2:
			powerup_droptype(x, y, POWERUP_COOLDOWN);
			return;
		case 3:
		case 4:
			powerup_droptype(x, y, POWERUP_SPREAD);
			return;
		case 5:
			powerup_droptype(x, y, POWERUP_SHIELD);
			return;
	}

}
