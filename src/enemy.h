

#ifndef ENEMY_H
#define ENEMY_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx.h"

struct enemy_data
{
	u8 live;
	u8 x;
	u8 y;
	s16 health;
	u8 t;
	u8 movement;
	u8 x_spawn;
	u8 y_spawn;
	u8 type;
	u8 cooldown;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct enemy_data manger_enemy[ENEMY_MAX];

extern ERAPI_SPRITE sprite_enemy_light;

void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage);
void enemy_update();
#endif
