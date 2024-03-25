
#include "player.h"

u8 px=160,py=80;
s8 fx=-10,fy=0;
u8 fire_cooldown_max = 20, fire_cooldown = 0;

ERAPI_SPRITE sprite_player = { playerTiles, gfxSharedPal, 4, 2, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE h_player;

u32 key;
void player_move(s8 x, s8 y)
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


void player_bounce(u8 angle)
{
	fx = -ERAPI_Cos(angle, PLAYER_BUMP_FORCE) >> 8;
	fy = ERAPI_Sin(angle, PLAYER_BUMP_FORCE) >> 8;
}

void player_hit_detect()
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

void player_control()
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
