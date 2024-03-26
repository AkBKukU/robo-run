#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

#include "gui.h"
#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "bullet.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF };



static inline void init()
{
	// System Setup
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);

	ERAPI_RandInit(1337);

	// Component Setup
	gui_init();
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
	gui_print_health(phealth);
	while (sysexit == 0)
	{
		gui_print_score(player_score);
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
