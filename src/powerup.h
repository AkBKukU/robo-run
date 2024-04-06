#ifndef POWERUP_H
#define POWERUP_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx_powerup.h"

struct enemy_drops
{
	u8 live;
	s16 x;
	s16 y;
	u8 type;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct enemy_drops manager_cooldown;
extern struct enemy_drops manager_shield;
extern struct enemy_drops manager_spread;

#define POWERUP_COOLDOWN 1
#define POWERUP_SHIELD   2
#define POWERUP_SPREAD   3

#define POWERUP_DROP_RATE 50

extern ERAPI_SPRITE sprite_powerup_cooldown;
extern ERAPI_SPRITE sprite_powerup_shield;
extern ERAPI_SPRITE sprite_powerup_spread;

void powerup_update();
void powerup_drop(u8 x, u8 y);
u8 powerup_drop_chance();
void powerup_droptype(u8 x, u8 y,u8 type);
void powerup_clean();


#endif
