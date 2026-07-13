#ifndef SOVIMTER_HELPER_H
#define SOVIMTER_HELPER_H
#include "../game.h"

bool render_menu(
  SDL_Texture** item_texture, SDL_Texture** hover_texture, SDL_Renderer* renderer,
  const MENU* menu
);

#endif
