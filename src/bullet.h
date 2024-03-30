#ifndef BULLET_H
#define BULLET_H
#include "def.h"
#include "erapi.h"


#include "gfx/gfx_enemy.h"

#include "gamestate.h"

#define BULLET_PLAYER 0
#define BULLET_ENEMY 1
#define BULLET_UPDATE_DELAY 8


struct bullet_data
{
	u16 x;
	u16 y;
	u16 xu;
	u16 yu;
	u8 live;
	u8 type;
	u8 angle;
	u8 speed;
	u32 hitcheck;
	u8 damage;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct bullet_data manger_bullet[BULLET_MAX];

extern u8 bullet_count;


void bullet_fire(u8 angle, u8 speed, u8 x, u8 y, u8 damage, u8 type);
void bullet_update();
void bullet_free(u8 i);
void bullet_init();




#endif
