
#ifndef PLAYER_H
#define PLAYER_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx_player.h"

#define PLAYER_BUMP_FORCE 8
#define PLAYER_HIT_R 10
#define PLAYER_MAX 5
#define PLAYER_IFRAMES_MAX 16

extern u8 px,py;
extern s16 phealth;
extern s8 fx,fy;
extern u8 fire_cooldown_max, fire_cooldown;
extern u8 player_sheild_max, player_sheild;
extern u8 shot_spread;
extern u8 player_iframes;

extern ERAPI_SPRITE sprite_player;
extern ERAPI_HANDLE_SPRITE h_player;

extern u32 key;


void player_move(s8 x, s8 y);
void player_bounce(u8 angle);
void player_hit_detect();
void player_damage(u8 damage);
void player_control();
void player_init();
#endif
