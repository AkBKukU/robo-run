
#include "screen.h"

u8 stars_offset = 0;
u8 stars_offset_frames = 0;
u8 tunnel_offset = 0;
u8 tunnel_offset_frames = 0;
u8 tunnel_groups[2] = {0,0};
s16 tunnel_wall_top = 3;
s16 tunnel_wall_bottom = 3;
s8 tunnel_height = TUNNEL_HEIGHT_START;
s16 tunnel_wall_top_hist[TUNNEL_MIN_CHANGE];
s16 tunnel_wall_bottom_hist[TUNNEL_MIN_CHANGE];

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
			starslide[ (x) + (y * BACK_X) ] = stars_pick();
			tunnelslide[ (x) + (y * BACK_X) ] = 0;
		}
	}

	// Apply new background
	ERAPI_BACKGROUND tunnel =
	{
		tunnelTiles,
		mapSharedPal,
		tunnelslide,
		sizeof( tunnelTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_TUNNEL, &tunnel);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_TUNNEL,8,vertical_offset);


	// Apply new background
	ERAPI_BACKGROUND stars =
	{
		starsTiles,
		mapSharedPal,
		starslide,
		sizeof( starsTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_STARS, &stars);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_STARS,8,vertical_offset);

	tunnel_wall_top = -50;
	tunnel_wall_bottom = -50;

	stars_offset=8;
	stars_offset_frames=0;
	tunnel_offset = 0;
	tunnel_offset_frames = 0;
	tunnel_groups[0] = 0;
	tunnel_groups[1] = 0;
	tunnel_height = TUNNEL_HEIGHT_START;
	vertical_offset = 16;

	for(u8 i=0;i<TUNNEL_MIN_CHANGE;++i)
	{
		tunnel_wall_top_hist[i] = 0;
		tunnel_wall_bottom_hist[i] = 0;
	}

}

void screen_clear()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			starslide[ (x) + (y * BACK_X) ] = 0;
			tunnelslide[ (x) + (y * BACK_X) ] = 0;
		}
	}

	// Apply new background
	ERAPI_BACKGROUND tunnel =
	{
		tunnelTiles,
		mapSharedPal,
		tunnelslide,
		sizeof( tunnelTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_TUNNEL, &tunnel);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_TUNNEL,0,0);


	// Apply new background
	ERAPI_BACKGROUND stars =
	{
		starsTiles,
		mapSharedPal,
		starslide,
		sizeof( starsTiles) >> 5,
		1
	};

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_STARS, &stars);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_STARS,0,0);
}
void tunnel_clear()
{
	rand_stable_map();
	tunnel_height = tunnel_height > TUNNEL_HEIGHT_MIN ? tunnel_height - 1 : tunnel_height;

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
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_TUNNEL,tunnel_offset,vertical_offset);
	if (tunnel_offset <= 16)
	{
		return;
	}
	++distance_tiles;
	++level_tiles;
	++player_score;
	tunnel_offset=8;
	tunnel_offset_frames=0;

	// Shift background tile map
	for(u8 x=1;x<BACK_X;++x){
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (x-1) + (y * BACK_X) ] = tunnelslide[ (x) + (y * BACK_X)];
	}}

	// Set off screen tiles to empty tunnel tile
	for(u8 y=0;y<BACK_Y;++y)
	{
		tunnelslide[ (31) + (y * BACK_X) ] = 0 ;
	}

	tunnel_generation();

	// Generate new tiles for top and bottom
	if (tunnel_wall_top > 0)
	{
		for(s8 wall = 0 ; wall < tunnel_wall_top; ++wall)
		{
			tunnelslide[ (31) + (wall * BACK_X) ] = tunnel_tile_pick() ;
		}
	}

	if (tunnel_wall_bottom > 0)
	{
		for(s8 wall = 0 ; wall < tunnel_wall_bottom; ++wall)
		{
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

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_TUNNEL, &slide);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_TUNNEL,8,vertical_offset);
}

void tunnel_generation()
{
	rand_stable_map();
	s16 last_top = 0;
	s16 last_bottom = 0;
	// Track tunnel shape history for checking viable path
	for(u8 i=1;i<TUNNEL_MIN_CHANGE;++i)
	{
		tunnel_wall_top_hist[i-1] = tunnel_wall_top_hist[i];
		tunnel_wall_bottom_hist[i-1] = tunnel_wall_bottom_hist[i];

		last_top = last_top < tunnel_wall_top_hist[i-1] ? tunnel_wall_top_hist[i-1] : last_top;
		last_bottom = last_bottom < tunnel_wall_bottom_hist[i-1] ? tunnel_wall_bottom_hist[i-1] : last_bottom;
	}
	tunnel_wall_top_hist[TUNNEL_MIN_CHANGE-1] = tunnel_wall_top;
	tunnel_wall_bottom_hist[TUNNEL_MIN_CHANGE-1] = tunnel_wall_bottom;

	last_top = last_top < tunnel_wall_top ? tunnel_wall_top : last_top;
	last_bottom = last_bottom < tunnel_wall_bottom ? tunnel_wall_bottom : last_bottom;

	if (level_progress_start+LEVEL_PROGRESS_1+(level_count*LEVEL_PROGRESS_INCREASE) > distance_tiles )
	{
		// Level Progress 0
		// Details:
		// - No tunnel
		return;

	}else if (level_progress_start+LEVEL_PROGRESS_2+(level_count*LEVEL_PROGRESS_INCREASE) > distance_tiles )
	{
		// Level Progress 1
		if (level_progress != 1)
		{
			// Reset tunnel wall positions
			level_progress = 1;
			tunnel_wall_top = -3;
			tunnel_wall_bottom = -3;
		}
		// Randomly but evenly move tunnel
		u8 max = 10 + level_count/2;
		if(ERAPI_RandMax(5) == 1)
		{
			tunnel_wall_top += ERAPI_RandMax(max)-(max/2);

		}
		if(ERAPI_RandMax(5) == 1)
		{
			tunnel_wall_bottom += ERAPI_RandMax(max)-(max/2);
		}
	}else{
		// Level Progress 2
		if (level_progress != 2)
		{
			level_progress = 2;
			 boss_spawn_init();
		}

		// Slowly move tunnel to center
		u8 max = 7 + level_count;
		if(ERAPI_RandMax(10) == 1)
		{
			tunnel_wall_top += ERAPI_RandMax(max+1)-(max/2);
		}
		if(ERAPI_RandMax(10) == 1)
		{
			tunnel_wall_bottom += ERAPI_RandMax(max+1)-(max/2);
		}
	}

	// Limit to tunnel height
	while ( (BACK_Y - tunnel_wall_top - tunnel_wall_bottom) < tunnel_height )
	{
		--tunnel_wall_bottom;
		--tunnel_wall_top;
	}

	// Prevent impassable overlapping of top and bottom
	if (tunnel_wall_bottom > BACK_Y-last_top-TUNNEL_MIN_CHANGE)
		tunnel_wall_bottom = BACK_Y-last_top-TUNNEL_MIN_CHANGE;
	if (tunnel_wall_top > BACK_Y-last_bottom-TUNNEL_MIN_CHANGE )
		tunnel_wall_top = BACK_Y-last_bottom-TUNNEL_MIN_CHANGE;


	// Prevent entire screen from filling and going too far away
	tunnel_wall_top = (tunnel_wall_top > BACK_Y - tunnel_height) ?  BACK_Y - tunnel_height : tunnel_wall_top;
	tunnel_wall_top = (tunnel_wall_top < -2) ? -2 : tunnel_wall_top;
	tunnel_wall_bottom = (tunnel_wall_bottom > BACK_Y - tunnel_height) ?  BACK_Y - tunnel_wall_bottom : tunnel_wall_bottom;
	tunnel_wall_bottom = (tunnel_wall_bottom < -2) ? -2 : tunnel_wall_bottom;
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

u8 stars_pick()
{
	u8 star_rand = ERAPI_RandMax(30);
	// Limit amount of stars that are not blank
	u8 star=1;
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
	return star;
}

void slide_stars()
{
	rand_stable_map();
	// Slowed background scrolling for parallax using modulos
	++stars_offset_frames;
	if(ERAPI_Mod(stars_offset_frames, STAR_SPEED)==0) ++stars_offset;
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_STARS,stars_offset,vertical_offset);
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

		starslide[ (31) + (y * BACK_X) ] = stars_pick();
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

	ERAPI_LoadBackgroundCustom( BACKGROUND_LAYER_STARS, &slide);
	ERAPI_SetBackgroundOffset(BACKGROUND_LAYER_STARS,8,vertical_offset);
}
