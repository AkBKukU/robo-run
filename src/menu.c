
#include "menu.h"

//ERAPI_HANDLE_REGION score_print;
const u16 menu_seedf_pal[] = {
	0x0000,		RGB(0x004300) ,		RGB(0x007400)
};
const u16 menu_seed_pal[] = {
	0x0000,		RGB(0x007400) ,		RGB(0x00FF00)
};
s8 menu_digit_select = 0;
u8 menu_debounce = 0;

ERAPI_BACKGROUND_DRAW_NUMBER background_seed_select =
{
	MENU_SEEDF_LAYER-1,
	MENU_SEEDF_PALETTE+1,
	18, 9,
	MENU_SEEDF_STYLE,
	1,
	0, // Zero pad right
	0, // Zero pad left
	0,
	0
};

ERAPI_BACKGROUND_DRAW_NUMBER background_seed_fade =
{
	MENU_SEEDF_LAYER-1,
	MENU_SEEDF_PALETTE,
	9, 9,
	MENU_SEEDF_STYLE,
	MENU_SEEDF_LENGTH,
	0, // Zero pad right
	1, // Zero pad left
	0,
	0
};

void menu_init()
{

	// Apply new background
	ERAPI_BACKGROUND menu =
	{
		menuTiles,
		mapSharedPal,
		menuMap,
		sizeof( menuTiles) >> 5,
		1
	};
	ERAPI_LoadBackgroundCustom( 3, &menu);
	// Prep screen region
	gui_init();
	ERAPI_DrawText(region_screen,0,0,"Booting...");
	// Start menu music
	ERAPI_PlaySoundSystem(SND_MENU_MUSIC);
	// Fade in
	ERAPI_FadeIn( 100);
	ERAPI_RenderFrame(100);
	// Replace booting with numbers
	ERAPI_BackgroundDrawNumber(&background_seed_fade);
	ERAPI_BackgroundDrawNumber(&background_seed_select);
	ERAPI_SetBackgroundPalette( menu_seedf_pal, MENU_SEEDF_PALETTE*16, 3);
	ERAPI_SetBackgroundPalette( menu_seed_pal, (MENU_SEEDF_PALETTE+1)*16, 3);
}

void menu_start()
{
}

void menu_update()
{
	key = ERAPI_GetKeyStateRaw();
	gui_print_score(save.high_score);

	// Get direction of movement
	s8 dir_x = 0, dir_y = 0;
	u16 pow = 1;
	if (!menu_debounce)
	{
		// Change position of highlighted digit
		if (key & ERAPI_KEY_LEFT)
		{
			ERAPI_PlaySoundSystem(SND_MENU_CHANGE);
			menu_debounce = MENU_DEBOUNCE;
			++menu_digit_select;
		}
		if (key & ERAPI_KEY_RIGHT)
		{
			ERAPI_PlaySoundSystem(SND_MENU_CHANGE);
			menu_debounce = MENU_DEBOUNCE;
			--menu_digit_select;
		}
		if (menu_digit_select > 3) menu_digit_select = 0;
		if (menu_digit_select < 0) menu_digit_select = 3;

	}else{
		--menu_debounce;
	}

	// Get digit offset as multiple of 10
	u8 pow_count = menu_digit_select;
	while (pow_count)
	{
		--pow_count;
		pow = pow * 10;
	}

	if (!menu_debounce)
	{
		// Check to change value of seed based on position of selected digit
		if (key & ERAPI_KEY_UP)
		{
			ERAPI_PlaySoundSystem(SND_MENU_CHANGE);
			menu_debounce = MENU_DEBOUNCE;
			save.seed+=pow;
		}
		if (key & ERAPI_KEY_DOWN)
		{
			ERAPI_PlaySoundSystem(SND_MENU_CHANGE);
			menu_debounce = MENU_DEBOUNCE;
			save.seed-=pow;
		}
		if (save.seed > 9999) save.seed = 9999;
		if (save.seed < 0) save.seed = 0;
	}

	// Draw full seed to background
	ERAPI_BackgroundDrawNumberNewValue(&background_seed_fade, save.seed);

	// Get only digit to edit and draw it on top
	u8 digit = ERAPI_Mod(ERAPI_Div(save.seed, pow ),10);
	background_seed_select.x = 18-3*menu_digit_select;
	ERAPI_BackgroundDrawNumberNewValue(&background_seed_select, digit);

	// Set palettes to custom values
	ERAPI_SetBackgroundPalette( menu_seedf_pal, MENU_SEEDF_PALETTE*16, 3);
	ERAPI_SetBackgroundPalette( menu_seed_pal, (MENU_SEEDF_PALETTE+1)*16, 3);

	// Start with selected seed
	if (key & ERAPI_KEY_A)
	{
		gui_pause(0);
		game_init();
		ERAPI_SoundPause(SND_MENU_MUSIC);
		ERAPI_PlaySoundSystem(SND_MENU_ENTER);
		game_play = 1;
		return;
	}

	// Quit game
	if (key & ERAPI_KEY_B)
	{
		sysexit = 1;
	}
}
