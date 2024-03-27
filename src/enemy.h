

#ifndef ENEMY_H
#define ENEMY_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx_enemy.h"
#include "gfx/gfx_powerup.h"

#define ENEMY_PATH_STRAIGHT 0

#define ENEMY_TYPE_COUNT 1
#define ENEMY_TYPE_LIGHT 0
extern u32 enemy_spawn_next[ENEMY_TYPE_COUNT];
extern u8 enemy_spawn_count[ENEMY_TYPE_COUNT];
extern u8 enemy_spawn_max[ENEMY_TYPE_COUNT];

struct enemy_data
{
	u8 live;
	u8 x;
	u8 y;
	s16 health;
	u16 t;
	u8 movement;
	u8 damage;
	u8 x_spawn;
	u8 y_spawn;
	u8 type;
	u8 cooldown;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct enemy_data manger_enemy[ENEMY_MAX];

struct enemy_drops
{
	u8 live;
	s16 x;
	s16 y;
	u8 type;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct enemy_drops manager_cooldown;
extern struct enemy_drops manager_shield;
extern struct enemy_drops manager_spread;


extern ERAPI_SPRITE sprite_enemy_light;

extern ERAPI_SPRITE sprite_powerup_cooldown;
extern ERAPI_SPRITE sprite_powerup_shield;
extern ERAPI_SPRITE sprite_powerup_spread;

void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage);
void enemy_update();
void enemy_drop(u8 i);
void enemy_path(u8 i);
void enemy_spawn(u8 spawn_type);
void enemy_init();
#endif
