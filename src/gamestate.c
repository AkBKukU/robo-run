
#include "gamestate.h"
u8 sysexit = 0, win = 0, game_play = 0, input_debounce=0;
u32 distance_tiles = 0,frame_count=0;

u8 level_progress = 0;
u32 level_progress_start = 0;

u16 level_tiles = 0;

u16 seed_fixed = 33875; // chosen by fair dice roll.
                        // guaranteed to be random.
//u16 base_seed = 54;
struct save_data save =
{
		0,
		0,
		0,

		0,
		0,
		0,
		0,

		0,
		0,

		{0,0,0,0}
};

u32 key;
void game_init()
{
	ERAPI_FadeOut(100);
	ERAPI_RenderFrame(100);
	distance_tiles = 0;
	frame_count=0;
	boss_level=1;
	level_tiles = 0;

	if (! (save.flags & SAVE_FLAG_RESUME))
	{
		save.level = 1;
		save.score = 0;
		save.health = 0;
		save.shield = 0;
		save.spread = 0;
		save.cooldown = PLAYER_COOLDOWN_START;
	}else{

		boss_level = save.level;
		boss_level = (boss_level > 3?3:boss_level);
	}

	level_progress = 0;
	level_progress_start = 0;

#ifdef DEBUG_MGBA
	mgba_print_string("Starting with seed:");
	char num_print[5]= "    ";

	citoa(save.seed,num_print,10);
	mgba_print_string(num_print);
#endif
	ERAPI_ClearSpritesAndBackgrounds();
	ERHACK_RegionListClearUS();
	player_init();
	screen_init();
	enemy_init();
	bullet_init();
	effect_init();
	boss_init();
	gui_init();
	gui_print_health(save.health,save.shield);
	ERAPI_FadeIn( 100);
	ERAPI_RenderFrame(75);
	ERAPI_HANDLE_SpriteAutoScaleWidthUntilSize(h_player,0x100,25);
	ERAPI_SpriteAutoScaleHeightBySize(h_player,0x100,25);
	ERAPI_SpriteSetPosAnimatedDuration(h_player,px,py,25);
	ERAPI_RenderFrame(25);
}

void game_clean()
{
	ERAPI_FadeOut(100);
	ERAPI_RenderFrame(100);
	boss_init();
	bullet_clean();
	enemy_clean();
	//gui_clean();
	player_clean();
	powerup_clean();
	screen_clear();
	effect_clean();
	for (u8 i = 0 ; i < 4 ; ++i)
	{
		ERAPI_SetBackgroundOffset(i,0,0);
	}
	ERAPI_ClearSpritesAndBackgrounds();
	ERHACK_RegionListClearUS();
}


void game_update()
{
	slide_tunnel();
	slide_stars();
	enemy_update();
	bullet_update();
	boss_update();
	effect_update();
	powerup_update();

	gui_print_score(save.score);
	player_hit_detect();
	player_control();

	if(save.health < 0)
		game_over();
}

void game_pause()
{
	gui_pause(1);
	key = ERAPI_GetKeyStateRaw();

	if (key & ERAPI_KEY_START && !input_debounce)
	{
		game_play = 1;
		gui_pause(0);
		input_debounce = DEBOUNCE_SET;
	}
	if (input_debounce)
		--input_debounce;
}

u16 stable_map_seed()
{
	// Requires a fixed seed starting point to create more entropy
	return seed_fixed+save.seed*seed_fixed+save.level*10000+level_tiles;
}
void rand_stable_map()
{
	ERAPI_RandInit(stable_map_seed());
}

void rand_stable_map_var(u8 var)
{
	ERAPI_RandInit(stable_map_seed()<<var);
}

void rand_true()
{
	ERAPI_RandInit(frame_count);
}

void rand_stable_boss(u8 boss_col)
{
	ERAPI_RandInit(save.seed+(save.level+boss_col*10)*10000);
}

void level_next()
{
	++boss_level;
	boss_level = (boss_level > 3?3:boss_level);
	++save.level;
	level_tiles = 0;
	tunnel_clear();
	player_sheild_max = (player_sheild_max + 1 >5 ? 5 : player_sheild_max+1);
	save.score+=100*save.level;
	level_progress = 0;
	level_progress_start = distance_tiles;
	save.flags|=SAVE_FLAG_RESUME;
	game_save();
}

void game_over()
{
	save.flags = save.flags  & ~SAVE_FLAG_RESUME;
	// Save new score if it is higher
	save.high_score = save.high_score < save.score ? save.score : save.high_score;
	save.score = save.high_score;
	game_save();
	game_clean();

	game_play=0;
	menu_init();
}

void game_save()
{
	ERAPI_FlashSaveUserData(SAVE_ID,&save);
}

void game_load()
{
	ERAPI_FlashLoadUserData(SAVE_ID,&save);
}
