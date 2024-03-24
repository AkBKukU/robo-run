#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF };

#define ENEMY_MAX 32

#define BACK_X 32
#define BACK_Y 24

struct enemy_data
{
	u8 live;
	u8 x;
	u8 y;
	u8 health;
	u8 t;
	u8 movement;
	u8 x_spawn;
	u8 y_spawn;
	u8 type;
	u8 cooldown;
};

u8 enemy_live=2;
struct enemy_data manger_enemy[ENEMY_MAX];

ERAPI_SPRITE sprite_player = { playerTiles, gfxSharedPal, 4, 2, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE h_player;

ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfxSharedPal, 2, 2, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE h_enemy_light[ENEMY_MAX];


u8 sysexit = 0, win = 0, px=160,py=80;
s8 vertical_offset = 16;
u32 key;

unsigned short mapslide[BACK_X*BACK_Y];


static inline void player_move(s8 x, s8 y)
{
	px+=x;
	py+=y;
	if (px < 16) px = 16;
	if (px > 240-16) px = 240-16;
	if (py < 8) py = 8;
	if (py > 160-8) py = 160-8;

	vertical_offset = ERAPI_Div(py,5);

	ERAPI_SetSpritePos( h_player, px, py);
}

static inline void enemy_update()
{
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;

		ERAPI_SetSpritePos(
			h_enemy_light[i],
			manger_enemy[i].x,
			manger_enemy[i].y-vertical_offset
		);
	}
}

static inline void player_hit_detect()
{
	// Check for contact with any enemies
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;
		if (ERAPI_CalcDistanceBetweenPoints(
			manger_enemy[i].x, manger_enemy[i].y,
			px, py
			) < 16)
		{
		ERAPI_SpriteHide( h_player);
		}

	}
	// TODO Check for hit against projectiles
}

static inline void player_control()
{
	key = ERAPI_GetKeyStateRaw();
	s8 dir_x = 0, dir_y = 0;
	if (key & ERAPI_KEY_UP)  dir_y = -1;
	if (key & ERAPI_KEY_DOWN)  dir_y = 1;
	if (key & ERAPI_KEY_LEFT) dir_x = -1;
	if (key & ERAPI_KEY_RIGHT) dir_x = 1;
	player_move(dir_x,dir_y);

}


static inline void init()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			mapslide[ (x) + (y * BACK_Y) ] = 0;
		}
	}

	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
	ERAPI_SetBackgroundOffset(3,8,0);

	h_player = ERAPI_SpriteCreateCustom( 0, &sprite_player);
	ERAPI_SetSpritePos( h_player, px, py);

	// Initialize enemy structs
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		manger_enemy[i].x = 0;
		manger_enemy[i].y = 0;
		manger_enemy[i].live = 0;
		h_enemy_light[i] = ERAPI_SpriteCreateCustom( 0, &sprite_enemy_light);
	}
	manger_enemy[0].x=px+60;
	manger_enemy[0].y=py;
	manger_enemy[0].live=1;

	manger_enemy[1].x=px+60;
	manger_enemy[1].y=py+32;
	manger_enemy[1].live=1;

	ERAPI_FadeIn( 1);
}

void slide_map()
{
	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_Y;++y)
	{
		mapslide[ (x-1) + (y * BACK_X) ] = mapslide[ (x) + (y * BACK_X)];
	}}

	// Set off screen tiles to random star tile
	for(u8 y=0;y<BACK_Y;++y)
	{
		u8 star_rand = ERAPI_Mod(ERAPI_Rand() , 30);
		// Limit amount of stars that are not blank
		u8 star=0;
		switch(star_rand)
		{
			case 1:
				star=1;
				break;
			case 2:
				star=2;
				break;
			case 3:
				star=3;
				break;
		}

		mapslide[ (31) + (y * BACK_X) ] = star ;
	}

	// Apply new background
	ERAPI_BACKGROUND slide =
	{
		starsTiles,
		mapSharedPal,
		mapslide,
		sizeof( starsTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( 3, &slide);
	ERAPI_SetBackgroundOffset(3,8,vertical_offset);
	ERAPI_FadeIn( 1);
}

int main()
{
	init();

	// Main Loop
	u8 background_loop=8;
	while (sysexit == 0)
	{
		++background_loop;
		ERAPI_SetBackgroundOffset(3,background_loop,vertical_offset);
		if (background_loop > 16)
		{
			background_loop=8;
			slide_map();
		}
		player_control();
		enemy_update();
		player_hit_detect();
		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
