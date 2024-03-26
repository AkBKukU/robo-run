
#ifndef PLAYER_H
#define PLAYER_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx.h"

#define PLAYER_BUMP_FORCE 8
#define PLAYER_HIT_R 10

extern u8 px,py;
extern s8 fx,fy;
extern u8 fire_cooldown_max, fire_cooldown;

extern ERAPI_SPRITE sprite_player;
extern ERAPI_HANDLE_SPRITE h_player;

extern u32 key;


void player_move(s8 x, s8 y);
void player_bounce(u8 angle);
void player_hit_detect();
void player_control();
void player_init();
#endif