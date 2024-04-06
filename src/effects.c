
#include "effects.h"

struct effect_data manger_effects[EFFECTS_MAX];
ERAPI_SPRITE sprite_effect_explosion = { eff_0_explodeTiles, gfx_enemySharedPal, 4, 4, 3, 4, 0, 2, 1};


void effect_update()
{
	// Initialize effect structs
	for ( u8 i = 0; i < EFFECTS_MAX; ++i )
	{
		// Check if effect is active
		if (!manger_effects[i].live) continue;

		// Check if effect needs updated
		--manger_effects[i].countdown;

		ERAPI_SetSpritePos(
			manger_effects[i].handle,
			manger_effects[i].x,
			manger_effects[i].y-vertical_offset
		);

		if (manger_effects[i].countdown) continue;


		manger_effects[i].x = 0;
		manger_effects[i].y = 0;
		manger_effects[i].type = 0;
		manger_effects[i].live = 0;
		ERAPI_SpriteFree(manger_effects[i].handle);
	}
}

s8 effect_find_free()
{
	// Search effect structs
	for ( u8 i = 0; i < EFFECTS_MAX; ++i )
	{
		// Check if effect is active
		if (manger_effects[i].live) continue;

		// Save index if a free effect slot is found
		return i;
	}

	// Return negative for no index
	return -1;
}

void effect_explode(u8 x, u8 y)
{
	// Get free index
	s8 i = effect_find_free();
	if (i == -1) return;

	manger_effects[i].x = x;
	manger_effects[i].y = y;
	manger_effects[i].type = EFFECT_EXPLOSION;
	manger_effects[i].live = 1;
	manger_effects[i].countdown = 18;
	manger_effects[i].handle = ERAPI_SpriteCreateCustom( 1, &sprite_effect_explosion);

	ERAPI_SetSpritePos(
		manger_effects[i].handle,
		manger_effects[i].x,
		manger_effects[i].y-vertical_offset
	);
	ERAPI_SpriteAutoAnimate(manger_effects[i].handle,6,18);


}

void effect_init()
{
	// Initialize effect structs
	for ( u8 i = 0; i < EFFECTS_MAX; ++i )
	{
		manger_effects[i].x = 0;
		manger_effects[i].y = 0;
		manger_effects[i].type = 0;
		manger_effects[i].live = 0;
	}
}

void effect_clean()
{
	// Search effect structs
	for ( u8 i = 0; i < EFFECTS_MAX; ++i )
	{
		// Check if effect is active
		if (manger_effects[i].live)
		{
			ERAPI_SpriteFree(manger_effects[i].handle);
		}
	}
}
