#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "def.h"
#include "erapi.h"

#include "map/map.h"

// Save information
#define SAVE_ID 42

struct save_data // 16 bytes of data to allocate
{
	u8 flags;
#define SAVE_FLAG_VALID 0x1
#define SAVE_FLAG_RESUME 0x2
	u16 high_score;
	u16 seed;
	u16 score;

	u8 shield;
	u8 spread;
	u8 cooldown;
	s8 health;

	u8 level;

	u8 unused[4];
};
extern struct save_data save;

#define BACK_X 32
#define BACK_Y 24

// Sprite types
#define SPRITE_PLAYER 0
#define SPRITE_ENEMY 1
#define SPRITE_PROJECTILE 2
#define SPRITE_PROJECTILE_ENEMY 3
#define SPRITE_PROJECTILE_LASER 4
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

#define LASER_MAX 3

extern u8 level_progress;
extern u32 level_progress_start;

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "util.h"
#include "boss.h"
#include "gui.h"
#include "effects.h"

#define DEBOUNCE_SET 20
extern u8 sysexit, win, game_play, input_debounce,no_save;
extern u32 distance_tiles, frame_count;

extern u16 level_tiles,seed_fixed;

extern u32 key;

void game_init();
void game_update();
void game_clean();
u16 stable_map_seed();
void rand_stable_map();
void rand_stable_map_var(u8 var);
void rand_true();
void rand_stable_boss(u8 boss_col);
void game_over();
void game_save();
void game_load();
void game_pause();

void level_next();

#endif
