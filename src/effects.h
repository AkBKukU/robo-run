#ifndef EFFECTS_H
#define EFFECTS_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"

extern ERAPI_SPRITE sprite_effect_explosion;


#define EFFECT_EXPLOSION 1

struct effect_data
{
	u8 live;
	u8 x;
	u8 y;
	u8 countdown;
	u8 type;
	ERAPI_HANDLE_SPRITE handle;
};
extern struct effect_data manger_effects[EFFECTS_MAX];


void effect_init();
void effect_update();
s8 effect_find_free();
void effect_explode(u8 x, u8 y, u8 delay);
void effect_clean();

#endif
