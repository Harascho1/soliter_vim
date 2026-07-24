#include "render.h"
#include "SDL3/SDL_log.h"
#include "game.h"
#include "res/res.h"
#include "res/texture.h"

void render_background(const GAME* game) {
  const bool status =
    SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
  }
}

void render_logo(const GAME* game) {
  const SDL_FRect* rect = &(SDL_FRect){
    .x = hero_logo_dimens.margin,
    .y = 0,
    .w = resolution.height,
    .h = resolution.height,
  };

  const bool status = SDL_RenderTexture(game->renderer, game->menu_texture, NULL, rect);
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
  }
}
