#ifndef BULLET_H
#define BULLET_H
#include "def.h"
#include "erapi.h"

#include "gfx/gfx_enemy.h"

#include "gamestate.h"


struct bullet_data
{
	u8 x;
	u8 y;
	u8 live;
	u8 angle;
	u8 speed;
	u8 damage;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct bullet_data manger_bullet[BULLET_MAX];


void bullet_fire(u8 angle, u8 speed, u8 x, u8 y, u8 damage);
void bullet_update();
void bullet_free(u8 i);
void bullet_init();

#endif
