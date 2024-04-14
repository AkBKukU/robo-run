
#include "player.h"

u8 px=160,py=80;
s16 phealth=100;
s8 fx=0,fy=0;
u8 fire_cooldown_max = 20, fire_cooldown = 20;
u8 player_sheild_max = 1, player_sheild = 0;
u8 shot_spread = 0;
u8 player_iframes = 0;

ERAPI_SPRITE sprite_player = { playerTiles, gfx_playerSharedPal, 4, 2, 1, 4, 8, 8, 1};
ERAPI_HANDLE_SPRITE h_player;


void player_move(s8 x, s8 y)
{
	// Adjust player position
	px+=x;
	py+=y;

	// "Push" player based on active forces
	px+=fx;
	if (fx < 0) ++fx;
	if (fx > 0) --fx;

	py+=fy;
	if (fy < 0) ++fy;
	if (fy > 0) --fy;

	// Bound player to screen
	if (px < 16) px = 16;
	if (px > 240-16) px = 240-16;
	if (py < 8) py = 8;
	if (py > 164-8) py = 164-8;

	// Calculate vertical offset of world based on updated player position
	vertical_offset = ERAPI_Div(py,5);

	ERAPI_SetSpritePos( h_player, px, py);
}

// Add force to player to bouce towards angle
void player_bounce(u8 angle)
{
	fx += -ERAPI_Cos(angle, PLAYER_BUMP_FORCE) >> 8;
	fy += ERAPI_Sin(angle, PLAYER_BUMP_FORCE) >> 8;
	if (fx < -20) fx=-20;
	if (fx > 20) fx=20;
	if (fy < -20) fy=-20;
	if (fy > 20) fy=20;
}

void player_hit_detect()
{
	if(player_iframes)
	{
		ERAPI_SpriteToggleVisible(h_player);
		--player_iframes;
	}


	// Check for contact with nearest enemy
	u16 dist = 0;
	ERAPI_HANDLE_SPRITE hit_sprite = ERAPI_SpriteFindClosestSprite(h_player,SPRITE_ENEMY, &dist);


	if (dist > 0 && dist < PLAYER_HIT_R)
	{
		u8 angle = ERAPI_CalcAngleBetweenSprites(h_player,hit_sprite);
		enemy_damage(hit_sprite,1);
		player_bounce(angle);
		player_damage(1);
	}


	// Check tunnel hit
	// NOTE - This is sloppy and always pushes vertically, it might be good enough though
	u8 tile=tunnelslide[(px-tunnel_offset)/8+1+(((py+(vertical_offset)-2)/8)*BACK_X)];
	if (tile)
	{
		player_damage(20);
		if (py > (tunnel_center(px/8))*8)
		{
			fy=-7;
			fx=-3;

		}else{
			fy=7;
			fx=-3;

		}
	}

	// Check boss hit
	if(boss_tile_hit_check(px, py+vertical_offset))
	{
		player_damage(10);
		fx=-10;
	}

	hit_sprite = ERAPI_SpriteFindClosestSprite(h_player,SPRITE_COOL, &dist);
	if (dist > 0 && dist < PLAYER_HIT_R)
	{
		manager_cooldown.live = 0;
		ERAPI_SpriteFree(manager_cooldown.handle);
		save.cooldown = save.cooldown > 5 ? save.cooldown-1 : save.cooldown ;
	}

	hit_sprite = ERAPI_SpriteFindClosestSprite(h_player,SPRITE_SHIELD, &dist);
	if (dist > 0 && dist < PLAYER_HIT_R)
	{
		manager_shield.live = 0;
		ERAPI_SpriteFree(manager_shield.handle);
		if (save.shield < player_sheild_max)
			++save.shield;
		gui_print_health(save.health,save.shield);
	}

	hit_sprite = ERAPI_SpriteFindClosestSprite(h_player,SPRITE_SPREAD, &dist);
	if (dist > 0 && dist < PLAYER_HIT_R)
	{
		manager_spread.live = 0;
		ERAPI_SpriteFree(manager_spread.handle);
		save.spread += 2 ;
	}
}

void player_damage(u8 damage)
{

#ifdef DEBUG_MGBA
	//return;
#endif
	if(player_iframes)
	{
		return;
	}
	if (save.shield)
	{
		--save.shield;
	}else{
		save.health-=damage;
	}
	gui_print_health(save.health,save.shield);
	player_iframes=PLAYER_IFRAMES_MAX;
	if(save.health < 0)
	{
		effect_explode(px,py+vertical_offset);
		ERAPI_SpriteHide( h_player);
		save.health = -1;
		return;
	}
}

void player_control()
{
	key = ERAPI_GetKeyStateRaw();

	// Get direction of movement
	s8 dir_x = 0, dir_y = 0;
	if (key & ERAPI_KEY_UP)  dir_y = -1;
	if (key & ERAPI_KEY_DOWN)  dir_y = 1;
	if (key & ERAPI_KEY_LEFT) dir_x = -1;
	if (key & ERAPI_KEY_RIGHT) dir_x = 1;
	player_move(dir_x,dir_y);

	// Check if firing and can fire
	if (fire_cooldown) --fire_cooldown;
	if (key & ERAPI_KEY_A && !fire_cooldown)
	{
		// TODO - 1 is playeer damage that will scale with powerups
		rand_true();
		u32 rand = ERAPI_RandMax(save.spread);
		bullet_fire(128-rand, 2, px+12, py+vertical_offset,1,BULLET_PLAYER);
		if(save.spread)
			bullet_fire(128+rand, 2, px+12, py+vertical_offset,1,BULLET_PLAYER);
		fire_cooldown = save.cooldown;
	}

	if (key & ERAPI_KEY_B)
	{
		game_over();
	}
}

void player_init()
{
	px=120;
	py=80;
	fx=0;
	fy=0;
	fire_cooldown = PLAYER_COOLDOWN_START;
	player_sheild_max = 1;

	if (! (save.flags & SAVE_FLAG_RESUME))
	{
		save.spread = 0;
		save.shield = 0;
		save.health = 100;
		save.cooldown = PLAYER_COOLDOWN_START;
	}else{
		player_sheild_max = save.level;
		player_sheild_max = (player_sheild_max > 5?5:player_sheild_max);
	}
	player_iframes = 0;

	h_player = ERAPI_SpriteCreateCustom( 0, &sprite_player);
	ERAPI_SetSpritePos( h_player, -100, py);
	ERAPI_HANDLE_SpriteAutoScaleWidthUntilSize(h_player,0x30,1);
	ERAPI_SpriteAutoScaleHeightBySize(h_player,0x200,1);
}

void player_clean()
{
	ERAPI_SpriteFree(h_player);
}

