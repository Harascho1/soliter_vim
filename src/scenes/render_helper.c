#include "render_helper.h"
#include "../res/res.h"

bool render_menu(
  SDL_Texture** item_texture, SDL_Texture** hover_texture, SDL_Renderer* renderer,
  const MENU* menu
) {
  bool status;
  float y_coord = (resolution.height / 2) + fonts.title_padding;
  for (int i = 0; i < menu->count; i++) {
    float render_coord_y;
    float item_width;
    if (i == menu->selected_item) {
      status = SDL_GetTextureSize(hover_texture[i], &item_width, NULL);
      if (!status) {
        SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
        return false;
      }
      render_coord_y = y_coord - ((fonts.item_hover_height - fonts.item_height) / 2);
    } else {
      status = SDL_GetTextureSize(item_texture[i], &item_width, NULL);
      if (!status) {
        SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
        return false;
      }
      render_coord_y = y_coord;
    }
    SDL_FPoint dst_point = {
      .x = (resolution.width - item_width) / 2,
      .y = render_coord_y,
    };
    SDL_Texture* tmp = NULL;
    if (menu->selected_item == i) {
      tmp = hover_texture[i];
    } else {
      tmp = item_texture[i];
    }
    status = render_text(renderer, tmp, &dst_point);
    if (!status) {
      SDL_Log("render_text failed...\n");
      return false;
    }

    y_coord += fonts.item_height + fonts.item_padding;
  }
  return true;
}