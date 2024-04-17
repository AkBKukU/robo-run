

#ifndef ENEMY_H
#define ENEMY_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "gfx/gfx_enemy.h"
#include "powerup.h"

#define ENEMY_PATH_STRAIGHT 0

#define ENEMY_TYPE_COUNT 2
#define ENEMY_TYPE_ROCK 0
#define ENEMY_TYPE_LIGHT 1
extern u32 enemy_spawn_next[ENEMY_TYPE_COUNT];
extern u8 enemy_spawn_count[ENEMY_TYPE_COUNT];
extern u8 enemy_spawn_max[ENEMY_TYPE_COUNT];

extern u8 enemy_spawn_allowed;
#define ENEMY_DAMAGE_START 3
extern u8 enemy_bullet_damage;

struct enemy_data
{
	u8 live;
	s16 x;
	s16 y;
	s16 health;
	u16 t;
	u8 drop_type;
	u8 movement;
	u8 damage;
	u8 x_mov;
	u8 y_mov;
	u8 type;
	u8 cooldown;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct enemy_data manger_enemy[ENEMY_MAX];


extern ERAPI_SPRITE sprite_enemy_rock;
extern ERAPI_SPRITE sprite_enemy_light;

void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage);
void enemy_update();
void enemy_path(u8 i);
void enemy_spawn(u8 spawn_type);
void enemy_init();
void enemy_clean();
#endif
