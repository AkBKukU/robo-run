
#include "menu.h"

ERAPI_BACKGROUND_DRAW_NUMBER background_seed_fade =
{
	MENU_SEEDF_LAYER,
	MENU_SEEDF_PALETTE+2,
	9, 9,
	MENU_SEEDF_STYLE,
	MENU_SEEDF_LENGTH,
	0, // Zero pad right
	1, // Zero pad left
	0,
	0
};
void menu_init()
{
	// Apply new background
	ERAPI_BACKGROUND menu =
	{
		menuTiles,
		mapSharedPal,
		menuMap,
		sizeof( menuTiles) >> 5,
		1
	};

	ERAPI_BackgroundDrawNumber(&background_seed_fade);
	ERAPI_BackgroundDrawNumberNewValue(&background_seed_fade, base_seed);
	ERAPI_LoadBackgroundCustom( 3, &menu);

	ERAPI_FadeIn( 100);
	ERAPI_RenderFrame(100);
}

void menu_start()
{
}

void menu_update()
{
	key = ERAPI_GetKeyStateRaw();

	// Get direction of movement
	s8 dir_x = 0, dir_y = 0;
	if (key & ERAPI_KEY_UP)  ++base_seed;
	if (key & ERAPI_KEY_DOWN)  --base_seed;
	if (base_seed > 9999) base_seed = 9999;
	if (base_seed < 0) base_seed = 0;
	ERAPI_BackgroundDrawNumberNewValue(&background_seed_fade, base_seed);

	if (key & ERAPI_KEY_A)
	{
		game_play = 1;
		game_init();
		return;
	}

	if (key & ERAPI_KEY_B)
	{
		sysexit = 1;
	}
}
