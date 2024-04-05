#ifndef MAP_H
#define MAP_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "map/map.h"


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
#define TUNNEL_MIN_CHANGE 3
extern s8 tunnel_height;
extern s16 tunnel_wall_top;
extern s16 tunnel_wall_bottom;

extern s16 tunnel_wall_top_hist[TUNNEL_MIN_CHANGE];
extern s16 tunnel_wall_bottom_hist[TUNNEL_MIN_CHANGE];

// Screen vertical offset
extern s8 vertical_offset;

extern unsigned short starslide[BACK_X*BACK_Y];
extern unsigned short tunnelslide[BACK_X*BACK_Y];

void tunnel_clear();
u8 tunnel_tile_pick();
void slide_tunnel();
void tunnel_generation();
void slide_stars();
u8 stars_pick();
u8 tunnel_center(u8 col);
void screen_init();
void screen_clear();
#endif
