#ifndef MENU_H
#define MENU_H
#include "def.h"
#include "erapi.h"

#include "gamestate.h"
#include "map/map.h"


// Seed Background Fade
//ERAPI_BACKGROUND_DRAW_NUMBER background_seed_fade;
#define MENU_SEEDF_LAYER 1
#define MENU_SEEDF_PALETTE 1
#define MENU_SEEDF_STYLE 4137
#define MENU_SEEDF_TEXTSHAD_PALINDEX 2
#define MENU_SEEDF_TEXT_PALINDEX 12
#define MENU_SEEDF_LENGTH 4


void menu_init();
void menu_start();
void menu_update();

#endif
