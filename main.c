#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"

extern int __end[];
const u16 palette[] = { 0x0000, 0xFFFF };

#define ENEMY_MAX 8
#define BULLET_MAX 8
u8 fire_cooldown_max = 20, fire_cooldown = 0;

#define BACK_X 32
#define BACK_Y 24

#define PLAYER_BUMP_FORCE 8
#define PLAYER_HIT_R 10

#define SPRITE_PLAYER 0
#define SPRITE_ENEMY 1
#define SPRITE_PROJECTILE 2

#define STAR_SPEED 4
u8 stars_offset = 0;
u8 stars_offset_frames = 0;

#define TUNNEL_SPEED 1
u8 tunnel_offset = 0;
u8 tunnel_offset_frames = 0;
#define TUNNEL_TILE_GROUP 4
#define TUNNEL_TILE_GROUP_COUNT 5
u8 tunnel_groups[2];
#define TUNNEL_HEIGHT_MIN 8
#define TUNNEL_HEIGHT_START 12

ERAPI_HANDLE_REGION score_print;

struct enemy_data
{
	u8 live;
	u8 x;
	u8 y;
	s16 health;
	u8 t;
	u8 movement;
	u8 x_spawn;
	u8 y_spawn;
	u8 type;
	u8 cooldown;
	ERAPI_HANDLE_SPRITE handle;
};
struct enemy_data manger_enemy[ENEMY_MAX];


struct bullet_data
{
	u8 x;
	u8 y;
	u8 live;
	u8 angle;
	u8 speed;
	ERAPI_HANDLE_SPRITE handle;
};
struct bullet_data manger_bullet[BULLET_MAX];

ERAPI_SPRITE sprite_player = { playerTiles, gfxSharedPal, 4, 2, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE h_player;

ERAPI_SPRITE sprite_enemy_light = { emy_0_lightTiles, gfxSharedPal, 2, 2, 6, 4, 0, 2, 1};
ERAPI_SPRITE sprite_bullet = { bulletTiles, gfxSharedPal, 1, 1, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE the_bullet;


u8 sysexit = 0, win = 0, px=160,py=80;
s8 vertical_offset = 16, fx=-10,fy=0;
u32 key;

unsigned short starslide[BACK_X*BACK_Y];
unsigned short tunnelslide[BACK_X*BACK_Y];

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

void gui_print_score(u32 score)
{
	char num_print[8];
	citoa(score,num_print,10);
	ERAPI_ClearRegion(score_print);
	ERAPI_DrawText( score_print, 0, 0, num_print);
}

static inline void player_move(s8 x, s8 y)
{
	px+=x;
	py+=y;

	px+=fx;
	if (fx < 0) ++fx;
	if (fx > 0) --fx;

	py+=fy;
	if (fy < 0) ++fy;
	if (fy > 0) --fy;


	if (px < 16) px = 16;
	if (px > 240-16) px = 240-16;
	if (py < 8) py = 8;
	if (py > 164-8) py = 164-8;

	vertical_offset = ERAPI_Div(py,5);

	ERAPI_SetSpritePos( h_player, px, py);
}

void enemy_damage(ERAPI_HANDLE_SPRITE hit_sprite, u8 damage)
{
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;
		if (manger_enemy[i].handle == hit_sprite)
		{
			manger_enemy[i].health-=damage;
			if (manger_enemy[i].health < 1)
			{
				manger_enemy[i].health=0;
				ERAPI_SetSpriteFrame(hit_sprite,3);
				ERAPI_SpriteAutoAnimate(hit_sprite,6,18);
				continue;
			}else{
				ERAPI_SetSpriteFrame(hit_sprite,0);
				ERAPI_SpriteAutoAnimate(hit_sprite,3,6);
			}
		}
	}
}

static inline void enemy_update()
{
	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (!manger_enemy[i].live) continue;
		// TODO - Add logic to remove killed enemies after animation complete
		if(manger_enemy[i].health < 1)
		{
			--manger_enemy[i].health;
			if(manger_enemy[i].health == -24)
			{
				manger_enemy[i].live=0;
				ERAPI_SpriteHide(manger_enemy[i].handle);
				continue;
			}
		}

		ERAPI_SetSpritePos(
			manger_enemy[i].handle,
			manger_enemy[i].x,
			manger_enemy[i].y-vertical_offset
		);
	}
}

void bullet_fire(u8 angle, u8 speed, u8 x, u8 y)
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		if (manger_bullet[i].live == 1)continue;
		manger_bullet[i].live = 1;
		manger_bullet[i].x = x;
		manger_bullet[i].y = y;
		manger_bullet[i].speed = speed;
		manger_bullet[i].angle = angle;
		manger_bullet[i].handle = ERAPI_SpriteCreateCustom( 0, &sprite_bullet);
		ERAPI_SpriteSetType(manger_bullet[i].handle,SPRITE_PROJECTILE);

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			x,
			y-vertical_offset
		);
		return;
	}
}

void bullet_update()
{
	for ( u8 i = 0; i < BULLET_MAX; ++i )
	{
		if (!manger_bullet[i].live) continue;

		manger_bullet[i].x += -ERAPI_Cos(manger_bullet[i].angle, manger_bullet[i].speed) >> 8;
		manger_bullet[i].y += ERAPI_Sin(manger_bullet[i].angle, manger_bullet[i].speed) >> 8;


		if (
			(manger_bullet[i].x < 0) ||
			(manger_bullet[i].x > 240) ||
			(manger_bullet[i].y < 0) ||
			(manger_bullet[i].y > 160)
		){
			manger_bullet[i].live = 0;
			ERAPI_SpriteFree(manger_bullet[i].handle);
			continue;
		}

		ERAPI_SetSpritePos(
			manger_bullet[i].handle,
			manger_bullet[i].x,
			manger_bullet[i].y-vertical_offset
		);

		// Check for contact against enemies
		u16 dist = 0;
		ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(manger_bullet[i].handle,SPRITE_ENEMY, &dist);
		if (dist < 10)
		{
			enemy_damage(hit_sprite,1);
			manger_bullet[i].live = 0;
			ERAPI_SpriteFree(manger_bullet[i].handle);
			continue;
		}

	}
}


static inline void player_bounce(u8 angle)
{
	fx = -ERAPI_Cos(angle, PLAYER_BUMP_FORCE) >> 8;
	fy = ERAPI_Sin(angle, PLAYER_BUMP_FORCE) >> 8;
}

static inline void player_hit_detect()
{
	// Check for contact with any enemies
			char num_print[5];
	u16 dist = 0;
	ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(h_player,SPRITE_ENEMY, &dist);

	for ( u8 i = 0; i < ENEMY_MAX; ++i )
	{
		if (manger_enemy[i].handle == hit_sprite)
		{
			u8 angle = ERAPI_CalcAngleBetweenSprites(h_player,hit_sprite);
			if (dist < PLAYER_HIT_R)
			{

				enemy_damage(hit_sprite,1);
				player_bounce(angle);
			}
		}
	}

	// Check tunnel hit
	u8 tile=tunnelslide[(px-tunnel_offset)/8+1+(((py+(vertical_offset)-2)/8)*BACK_X)];
	if (tile)
	{
		if (py > 80)
		{
			fy=-10;

		}else{
			fy=10;

		}
	}
}

static inline void player_control()
{
	key = ERAPI_GetKeyStateRaw();
	s8 dir_x = 0, dir_y = 0;
	if (key & ERAPI_KEY_UP)  dir_y = -1;
	if (key & ERAPI_KEY_DOWN)  dir_y = 1;
	if (key & ERAPI_KEY_LEFT) dir_x = -1;
	if (key & ERAPI_KEY_RIGHT) dir_x = 1;

	if (fire_cooldown) --fire_cooldown;
	if (key & ERAPI_KEY_A && !fire_cooldown)
	{
		bullet_fire(127, 4, px, py+vertical_offset);
		fire_cooldown = fire_cooldown_max;
	}
	player_move(dir_x,dir_y);
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
