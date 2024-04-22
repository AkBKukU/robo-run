#ifndef BULLET_H
#define BULLET_H
#include "def.h"
#include "erapi.h"


#include "gfx/gfx_enemy.h"

#include "gamestate.h"

#define BULLET_PLAYER 1
#define BULLET_ENEMY 2
#define BULLET_LASER 3

#define BULLET_UPDATE_DELAY 6

#define LASER_LEN_COUNT 3
#define LASER_HITCHECK_DELAY 5


struct bullet_data
{
	u16 x;
	u16 y;
	u16 xu;
	u16 yu;
	u8 live;
	u8 type;
	u8 angle;
	u8 speed;
	u32 hitcheck;
	u8 damage;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct bullet_data manger_bullet[BULLET_MAX];

void bullet_fire(u8 angle, u8 speed, u8 x, u8 y, u8 damage, u8 type);

extern u8 bullet_count;

struct laser_data
{
	u16 x;
	u16 y;
	u8 live;
	u8 b_index;
	u8 type;
	u8 angle;
	u32 hitcheck;
	u8 damage;
};
extern struct laser_data manager_laser[LASER_MAX];

u8 laser_fire(u8 angle, u8 x, u8 y, u8 damage, u8 type);
void laser_update(u8 laser_id,  u8 x, u8 y, u8 angle);
void laser_relese(u8 laser_id);

void bullet_update();
void bullet_free(u8 i);
void bullet_free_sprite(ERAPI_HANDLE_SPRITE sprite);
void bullet_init();
void bullet_clean();



#endif
