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
#include "menu.h"

#include "util.h"
extern int __end[];



static inline void init()
{
	// System Setup
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);

	// Component Setup
	game_load();
	//gui_init();

	//gui_print_score(save.score);

	if (save.flags & SAVE_FLAG_RESUME)
	{
		game_init();
		game_play = 1;
		save.flags  = save.flags  & ~SAVE_FLAG_RESUME;
		game_save();

	}else{
		menu_init();
	}


#ifdef DEBUG_MGBA
// Emulator debug functions
	mgba_print_init();

	mgba_print_string("Starting with print ability");
	//fire_cooldown_max = 5;
	//shot_spread = 20;
#endif
}

int main()
{
	init();
	// Main Loop
	while (sysexit == 0)
	{
		if (game_play)
		{
			game_update();
		} else {
			menu_update();
		}
		ERAPI_RenderFrame(1);
		++frame_count;
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
