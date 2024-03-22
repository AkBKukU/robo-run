#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

ERAPI_HANDLE_REGION chooser;
extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

u8 sysexit = 0, win = 0;
u32 key;

#define BACK_X 32
#define BACK_Y 20

ERAPI_BACKGROUND background =
{
  starsTiles,
  mapSharedPal,
  starsMap,
  sizeof( starsTiles) >> 5,
  1
};


unsigned short mapslide[BACK_X*BACK_Y];


u8 color_loop=1;

static inline void init()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			mapslide[ (x) + (y * BACK_Y) ] = starsMap[ (x) + (y * BACK_Y)];
		}
	}

	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
	ERAPI_LoadBackgroundCustom( 3, &background);
	ERAPI_SetBackgroundOffset(3,8,0);
	ERAPI_FadeIn( 1);
}

void slide_map()
{
	for(u8 x=1;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			mapslide[ (x-1) + (y * BACK_X) ] = mapslide[ (x) + (y * BACK_X)];
		}
	}


	for(u8 y=0;y<BACK_Y;++y)
	{

		u8 star_rand = ERAPI_Mod(ERAPI_Rand() , 30);
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

	ERAPI_BACKGROUND slide =
	{
		starsTiles,
		mapSharedPal,
		mapslide,
		sizeof( starsTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( 3, &slide);
	ERAPI_SetBackgroundOffset(3,8,0);
	ERAPI_FadeIn( 1);
}

int main()
{
	init();
	// Main Loop
	u8 delay=0;
	u8 background_loop=8;
	ERAPI_LoadBackgroundCustom( 3, &background);
	while (sysexit == 0)
	{
		++background_loop;
		ERAPI_SetBackgroundOffset(3,background_loop,0);
		if (background_loop > 16)
		{
			background_loop=8;
			slide_map();
		}
		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
