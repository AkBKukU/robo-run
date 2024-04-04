
#include "gamestate.h"
u8 sysexit = 0, win = 0;
u32 distance_tiles = 0, player_score=0,frame_count=0;
u32 level_count =1;

u8 level_progress = 0;
u32 level_progress_start = 0;

u16 base_seed = 24;
//u16 base_seed = 54;

void game_init()
{
	ERAPI_FadeOut(100);
	ERAPI_RenderFrame(100);
	distance_tiles = 0;
	player_score=0;
	frame_count=0;
	level_count =1;

	level_progress = 0;
	level_progress_start = 0;

	player_init();
	screen_init();
	enemy_init();
	bullet_init();
	effect_init();
	boss_init();
	ERAPI_FadeIn( 100);
	ERAPI_RenderFrame(100);
}


void game_update()
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
}

void rand_stable_map()
{
	ERAPI_RandInit(base_seed+distance_tiles*10000);
}

void rand_true()
{
	ERAPI_RandInit(frame_count);
}

void rand_stable_boss(u8 boss_col)
{
	ERAPI_RandInit(base_seed+(level_count+boss_col*10)*10000);
}

void level_next()
{
	++level_count;
	tunnel_clear();
	player_sheild_max = (player_sheild_max + 1 >5 ? 5 : player_sheild_max+1);
	player_score+=100*level_count;
	level_progress = 0;
	level_progress_start = distance_tiles;
}
