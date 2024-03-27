#ifndef MAP_H
#define MAP_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"


#define STAR_SPEED 4
extern u8 stars_offset;
extern u8 stars_offset_frames;

#define TUNNEL_SPEED 1
extern u8 tunnel_offset;
extern u8 tunnel_offset_frames;
#define TUNNEL_TILE_GROUP 4
#define TUNNEL_TILE_GROUP_COUNT 5
extern u8 tunnel_groups[2];
#define TUNNEL_HEIGHT_MIN 6
#define TUNNEL_HEIGHT_START 12
extern s8 tunnel_wall_top;
extern s8 tunnel_wall_bottom;

// Screen vertical offset
extern s8 vertical_offset;

extern unsigned short starslide[BACK_X*BACK_Y];
extern unsigned short tunnelslide[BACK_X*BACK_Y];

u8 tunnel_tile_pick();
void slide_tunnel();
void slide_stars();
void screen_init();
#endif
