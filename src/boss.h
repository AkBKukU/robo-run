#ifndef BOSS_H
#define BOSS_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "bullet.h"
#include "map/map_boss.h"

#define BOSS_MAP_WIDTH   6
#define BOSS_MAP_WEAPONS   3
#define BOSS_TILE_SINGLE 1
#define BOSS_TILE_SPREAD 2
#define BOSS_TILE_LASER   3

#define BOSS_TILE_1_TALL   BOSS_MAP_WIDTH*3
#define BOSS_SIZE_RATIO_MAX 4
#define BOSS_SIZE_HEIGHT_MAX 3

#define BOSS_WEAPON_MAX 10
#define BOSS_WEAPON_COOLDOWN_MIN 30
#define BOSS_WEAPON_COOLDOWN_MAX 250

#define BOSS_SPAWN_DISTANCE_MAX 150



extern u8 boss_spawning_flag;
extern u8 boss_live;
extern u8 boss_len, boss_level, boss_gen_col,boss_x_pos;
extern s16 boss_y_offset;
extern u8 boss_weapon_count;
extern u8 weapon_laser;
extern s32 boss_health;


struct boss_weapons
{
	u8 x;
	u8 type;
	u8 alt;
	u8 cooldown;
};
extern struct boss_weapons manger_boss_weapons[BOSS_WEAPON_MAX];


extern unsigned short boss_map[ BACK_X * BACK_X ];

// TODO - This value could be eliminated by just putting the boss in the upper
// left of the map and specifiying a static offset instead which would be
// used much less often

#define center (BACK_X *9 + 30)

void boss_spawn();
void boss_spawn_init();
void boss_damage(u8 damage);
void boss_update();
void boss_stop_laser(s8 w);
void boss_get_pos_on(u8* x, u8* y);
void boss_init();
u8 boss_tile_hit_check(u8 x, u8 y);

#endif
