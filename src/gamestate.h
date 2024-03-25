#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

#define BACK_X 32
#define BACK_Y 24

// Sprite types
#define SPRITE_PLAYER 0
#define SPRITE_ENEMY 1
#define SPRITE_PROJECTILE 2

// Sprite limits
#define ENEMY_MAX 8
#define BULLET_MAX 8

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"

extern u8 sysexit, win;


extern ERAPI_HANDLE_REGION score_print;



struct bullet_data
{
	u8 x;
	u8 y;
	u8 live;
	u8 angle;
	u8 speed;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct bullet_data manger_bullet[BULLET_MAX];



extern ERAPI_SPRITE sprite_bullet;
extern ERAPI_HANDLE_SPRITE the_bullet;


void gui_print_score(u32 score);
void bullet_fire(u8 angle, u8 speed, u8 x, u8 y);
void bullet_update();

#endif
