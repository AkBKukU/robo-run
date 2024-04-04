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

#include "util.h"
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
	game_init();
	ERAPI_FadeIn( 1);


#ifdef DEBUG_MGBA
// Emulator debug functions
	mgba_print_init();

	mgba_print_string("Starting with print ability");
	fire_cooldown_max = 5;
	shot_spread = 20;
#endif
}

int main()
{
	init();
	// Main Loop
	gui_print_health(phealth,player_sheild);
	while (sysexit == 0)
	{
		game_update();
		gui_print_score(level_progress);
		ERAPI_RenderFrame(1);
		++frame_count;
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
