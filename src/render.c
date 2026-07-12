#include "render.h"
#include "SDL3/SDL_log.h"
#include "game.h"
#include "res/texture.h"

bool render_background(GAME *game) {
  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return 0;
  }

  bool status = SDL_RenderClear(game->renderer);
  if (status == 0) {
    SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    return 0;
  }

  status =
      SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (status == 0) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return 0;
  }
  return status;
}

bool render_logo(GAME *game) {
  if (game->menu_texture == NULL) {
    SDL_Log("menu_texture is NULL\n");
    game->menu_texture =
        create_texture_from_image(game->renderer, "assets/logo.png");
    if (game->menu_texture == NULL) {
      SDL_Log("menu_texture is NULL\n");
      return false;
    }
  }
  const SDL_FRect *rect = &(SDL_FRect){
      .x = game->field.square_screen_padding_width,
      .y = 0,
      .w = game->field.screen_height,
      .h = game->field.screen_height,
  };

  bool status =
      SDL_RenderTexture(game->renderer, game->menu_texture, NULL, rect);
  if (status == false) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }
  return status;
}
