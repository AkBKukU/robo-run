#include "def.h"
#include "erapi.h"

#include "map/map.h"

#include "gui.h"
#include "boss.h"
#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"
#include "bullet.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF, 0x0000, 0x0FF0,0x001F,RGB(0x4986da) };



static inline void init()
{
	// System Setup
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( palette, 0x00, 0x06);

	ERAPI_RandInit(base_seed);

	// Component Setup
	gui_init();
	player_init();
	screen_init();
	enemy_init();
	bullet_init();
	effect_init();
	boss_init();
	ERAPI_FadeIn( 1);
}

int main()
{
	init();
	// Main Loop
	gui_print_health(phealth,player_sheild);
	while (sysexit == 0)
	{
		slide_tunnel();
		slide_stars();
		player_control();
		enemy_update();
		bullet_update();
		boss_update();
		effect_update();
		powerup_update();
		player_hit_detect();


 		gui_print_score(player_score);
		ERAPI_RenderFrame(1);
		++frame_count;
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
