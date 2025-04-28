#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "SDL3/SDL_render.h"

extern SDL_Texture *game_title;
extern SDL_Texture *menu_items[4];
extern SDL_Texture *hover_menu_items[4];

int lazy_load_main_menu();
void lazy_destroy_main_menu();

#endif
