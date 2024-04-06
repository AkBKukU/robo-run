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

// Background Layer Usage
#define BACKGROUND_LAYER_UI 0
#define BACKGROUND_LAYER_BOSS 1
#define BACKGROUND_LAYER_TUNNEL 2
#define BACKGROUND_LAYER_STARS 3

// Sprite limits
#define ENEMY_MAX 8
#define BULLET_MAX 32
#define DROPS_MAX 3
#define EFFECTS_MAX 5

// Level Progression
#define LEVEL_PROGRESS_INCREASE 50
#define LEVEL_PROGRESS_1 150
#define LEVEL_PROGRESS_2 350

extern u32 level_count;
extern u8 level_progress;
extern u32 level_progress_start;

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "util.h"
#include "boss.h"

extern u8 sysexit, win, game_play;
extern u32 distance_tiles, level_tiles, player_score,frame_count;

extern s16 base_seed;

extern u32 key;

void game_init();
void game_update();
void game_clean();
void rand_stable_map();
void rand_stable_map_var(u8 var);
void rand_true();
void rand_stable_boss(u8 boss_col);

void level_next();

#endif
