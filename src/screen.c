
#include "screen.h"

u8 stars_offset = 0;
u8 stars_offset_frames = 0;
u8 tunnel_offset = 0;
u8 tunnel_offset_frames = 0;
u8 tunnel_groups[2] = {0,0};
s16 tunnel_wall_top = 3;
s16 tunnel_wall_bottom = 3;
s8 tunnel_height = TUNNEL_HEIGHT_START;

// Screen vertical offset
s8 vertical_offset = 16;
unsigned short starslide[BACK_X*BACK_Y];
unsigned short tunnelslide[BACK_X*BACK_Y];

void screen_init()
{
	tunnel_clear();
	// Initialize background tile maps to empty
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			starslide[ (x) + (y * BACK_Y) ] = 0;
			tunnelslide[ (x) + (y * BACK_Y) ] = 0;
		}
	}
	tunnel_wall_top = 0;
	tunnel_wall_bottom = 0;

}

void tunnel_clear()
{
	rand_stable_boss(0);
	tunnel_height = tunnel_height < TUNNEL_HEIGHT_MIN ? tunnel_height - 1 : tunnel_height;
	// Block types for tunnel
	tunnel_groups[0] = ERAPI_RandMax(TUNNEL_TILE_GROUP_COUNT);
	tunnel_groups[1] = ERAPI_RandMax(TUNNEL_TILE_GROUP_COUNT);

	tunnel_wall_top = 0;
	tunnel_wall_bottom = 0;
}

// Pick a tile from the currently selected groups
// TODO - Doesn't seem to be working right
u8 tunnel_tile_pick()
{
	u8 group_slot = ERAPI_RandMax(2);

	return tunnel_groups[group_slot]*4 + ERAPI_RandMax(4)+1;
}

// Shifts all tiles to the left for the tunnel
void slide_tunnel()
{
	rand_stable_map();
	++tunnel_offset_frames;
	++tunnel_offset;
	ERAPI_SetBackgroundOffset(2,tunnel_offset,vertical_offset);
	if (tunnel_offset <= 16)
	{
		return;
	}
	++distance_tiles;
	++player_score;
	tunnel_offset=8;
	tunnel_offset_frames=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (x-1) + (y * BACK_X) ] = tunnelslide[ (x) + (y * BACK_X)];
	}}

	// Set off screen tiles to random tunnel tile
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (31) + (y * BACK_X) ] = 0 ;
	}

	if(ERAPI_RandMax(10) == 1)
	{
		tunnel_wall_top += ERAPI_RandMax(6)-2;
		tunnel_wall_top = (tunnel_wall_top < -2) ? -2 : tunnel_wall_top;
	}
	if(ERAPI_RandMax(10) == 1)
	{

		tunnel_wall_bottom += ERAPI_RandMax(6)-2;
		tunnel_wall_bottom = (tunnel_wall_bottom < -2) ? -2 : tunnel_wall_bottom;
	}

	while ( (20 - tunnel_wall_top - tunnel_wall_bottom) < tunnel_height )
	{
		--tunnel_wall_bottom;
		--tunnel_wall_top;
	}

	// Generate new tiles for top and bottom
	if (tunnel_wall_top > 0)
	{
		s8 wall = tunnel_wall_top;
		while(wall)
		{
			--wall;
			tunnelslide[ (31) + (wall * BACK_X) ] = tunnel_tile_pick() ;
		}
	}

	if (tunnel_wall_bottom > 0)
	{
		s8 wall = tunnel_wall_bottom;
		while(wall)
		{
			--wall;
			tunnelslide[ (31) + ((BACK_Y-1-wall) * BACK_X) ] = tunnel_tile_pick() ;
		}
	}


	// Apply new background
	ERAPI_BACKGROUND slide =
	{
		tunnelTiles,
		mapSharedPal,
		tunnelslide,
		sizeof( tunnelTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( 2, &slide);
	ERAPI_SetBackgroundOffset(2,8,vertical_offset);
}

u8 tunnel_center(u8 col)
{
	u8 ytop = 0;
	u8 top = tunnelslide[ (col) ];
	while(top)
	{
		++ytop;
		top = tunnelslide[ (col) + (ytop * BACK_X) ];
	}

	u8 ybot = BACK_Y-1;
	u8 bottom = tunnelslide[ (col) + (ybot * BACK_X)  ];
	while(bottom)
	{
		--ybot;
		bottom = tunnelslide[ (col) + (ybot * BACK_X) ];
	}

	return ytop+(ybot - ytop)/2;
}

void slide_stars()
{
	rand_stable_map();
	// Slowed background scrolling for parallax using modulos
	++stars_offset_frames;
	if(ERAPI_Mod(stars_offset_frames, STAR_SPEED)==0) ++stars_offset;
	ERAPI_SetBackgroundOffset(3,stars_offset,vertical_offset);
	if (stars_offset <= 16)
	{
		return;
	}

	stars_offset=8;
	stars_offset_frames=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_Y;++y)
	{
		starslide[ (x-1) + (y * BACK_X) ] = starslide[ (x) + (y * BACK_X)];
	}}

	// Set off screen tiles to random star tile
	for(u8 y=0;y<BACK_Y;++y)
	{
		u8 star_rand = ERAPI_RandMax(30);
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

		starslide[ (31) + (y * BACK_X) ] = star ;
	}

	// Apply new background
	ERAPI_BACKGROUND slide =
	{
		starsTiles,
		mapSharedPal,
		starslide,
		sizeof( starsTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( 3, &slide);
	ERAPI_SetBackgroundOffset(3,8,vertical_offset);
}
