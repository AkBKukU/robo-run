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
#define EFFECTS_MAX 5

// Level Progression
#define LEVEL_PROGRESS_INCREASE 50
#define LEVEL_PROGRESS_1 150
#define LEVEL_PROGRESS_2 350

extern u8 level_progress;
extern u32 level_progress_start;

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "util.h"
#include "boss.h"

extern u8 sysexit, win;
extern u32 distance_tiles, player_score,frame_count;

extern u16 base_seed;

void game_init();
void game_update();
void rand_stable_map();
void rand_true();
void rand_stable_boss(u8 boss_col);

void level_next();

#endif
