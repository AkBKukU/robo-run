#ifndef BOSS_H
#define BOSS_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "map/map_boss.h"

#define BOSS_MAP_WIDTH   6

#define BOSS_TILE_1_TALL   BOSS_MAP_WIDTH*3
#define BOSS_TILE_X_SINGLE 1
#define BOSS_TILE_X_SPREAD 2
#define BOSS_TILE_X_LASER  3
// This sets the maximum dimensions before it will bump up to the next size
#define BOSS_SIZE_RATIO_MAX 4
#define BOSS_SIZE_HEIGH_MAX 3


extern u32 boss_level;

extern unsigned short boss_map[BACK_X*BACK_Y];

void boss_spawn();

#endif
