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
#include "util.h"

extern u8 sysexit, win;
extern u32 distance_tiles, player_score;


#endif
