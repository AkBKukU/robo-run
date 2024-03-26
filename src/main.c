#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "bullet.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF };







static inline void init()
{
	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
	ERAPI_SetBackgroundOffset(3,8,0);
	score_print = ERAPI_CreateRegion(0,0,0, 0,0xf, 0x03);
	ERAPI_SetTextColor( score_print, 0x01, 0x00);


	player_init();
	screen_init();
	enemy_init();
	bullet_init();

	ERAPI_FadeIn( 1);
}

int main()
{
	init();
	// Main Loop
	while (sysexit == 0)
	{
		slide_tunnel();
		slide_stars();
		player_control();
		enemy_update();
		bullet_update();
		player_hit_detect();
		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
