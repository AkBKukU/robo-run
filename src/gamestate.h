#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "def.h"
#include "erapi.h"

#include "map/map.h"

#define BACK_X 32
#define BACK_Y 24

// Sprite types
#define SPRITE_PLAYER 0
#define SPRITE_ENEMY 1
#define SPRITE_PROJECTILE 2
#define SPRITE_COOL   10
#define SPRITE_SHIELD 11
#define SPRITE_SPREAD 12

// Sprite limits
#define ENEMY_MAX 8
#define BULLET_MAX 32
#define DROPS_MAX 3

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "util.h"
#include "boss.h"

extern u8 sysexit, win;
extern u32 distance_tiles, player_score,frame_count;

extern u16 base_seed;

void rand_stable_map();
void rand_stable_boss();

#endif
