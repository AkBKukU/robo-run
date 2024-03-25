
#include "screen.h"

u8 stars_offset = 0;
u8 stars_offset_frames = 0;
u8 tunnel_offset = 0;
u8 tunnel_offset_frames = 0;
u8 tunnel_groups[2] = {0,0};

// Screen vertical offset
s8 vertical_offset = 16;
unsigned short starslide[BACK_X*BACK_Y];
unsigned short tunnelslide[BACK_X*BACK_Y];

u8 tunnel_tile_pick()
{

	u8 group_slot = ERAPI_Mod(ERAPI_Rand() , 2);

	return tunnel_groups[group_slot] + ERAPI_Mod(ERAPI_Rand() , TUNNEL_TILE_GROUP)+1;
}
void slide_tunnel()
{
	++tunnel_offset_frames;
	++tunnel_offset;
	ERAPI_SetBackgroundOffset(2,tunnel_offset,vertical_offset);
	if (tunnel_offset <= 16)
	{
		return;
	}
	tunnel_offset=8;
	tunnel_offset_frames=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (x-1) + (y * BACK_X) ] = tunnelslide[ (x) + (y * BACK_X)];
	}}

	// Set off screen tiles to random star tile
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (31) + (y * BACK_X) ] = 0 ;
	}
	tunnelslide[ (31) ] = tunnel_tile_pick() ;
	tunnelslide[ (31) + ((BACK_Y-1) * BACK_X) ] = tunnel_tile_pick() ;


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

void slide_stars()
{
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
