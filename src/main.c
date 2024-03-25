#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

#include "screen.h"
#include "enemy.h"
#include "player.h"
#include "gamestate.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF };







// A utility function to reverse a string
static inline void reverse(char str[], int length)
{
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}
}

// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
	int i = 0;

	/* Handle 0 explicitly, otherwise empty string is
	 * printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// Process individual digits
	while (num != 0) {
		int rem =ERAPI_Mod( num , base);
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num =ERAPI_Div( num , base);
	}

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	reverse(str, i);

	return str;
}



static inline void init()
{
	for(u8 x=0;x<BACK_X;++x)
	{
		for(u8 y=0;y<BACK_Y;++y)
		{
			starslide[ (x) + (y * BACK_Y) ] = 0;
			tunnelslide[ (x) + (y * BACK_Y) ] = 0;
		}
	}


	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
	ERAPI_SetBackgroundOffset(3,8,0);
	score_print = ERAPI_CreateRegion(0,0,0, 0,0xf, 0x03);
	ERAPI_SetTextColor( score_print, 0x01, 0x00);

	h_player = ERAPI_SpriteCreateCustom( 0, &sprite_player);
	ERAPI_SetSpritePos( h_player, px, py);

	the_bullet = ERAPI_SpriteCreateCustom( 0, &sprite_bullet);
	ERAPI_SetSpritePos( the_bullet, 255, 255);

	// Initialize enemy structs
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		manger_enemy[i].x = 0;
		manger_enemy[i].y = 0;
		manger_enemy[i].live = 0;
		manger_enemy[i].handle = ERAPI_SpriteCreateCustom( 0, &sprite_enemy_light);
		ERAPI_SpriteSetType(manger_enemy[i].handle,SPRITE_ENEMY);
		ERAPI_SetSpriteFrame(manger_enemy[i].handle,1);
	}
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		manger_bullet[i].live = 0;
	}
	manger_enemy[0].x=px;
	manger_enemy[0].y=py;
	manger_enemy[0].live=1;
	manger_enemy[0].health=3;

	manger_enemy[1].x=px+60;
	manger_enemy[1].y=py+32;
	manger_enemy[1].live=1;
	manger_enemy[1].health=3;

	ERAPI_FadeIn( 1);
}

int main()
{
	init();
	tunnel_groups[0] = ERAPI_Mod(ERAPI_Rand() , TUNNEL_TILE_GROUP_COUNT);
	tunnel_groups[1] = ERAPI_Mod(ERAPI_Rand() , TUNNEL_TILE_GROUP_COUNT);

	// Main Loop
	while (sysexit == 0)
	{
		slide_tunnel();
		slide_stars();
		player_control();
		enemy_update();
		bullet_update();
		player_hit_detect();
		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
