#include "font.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

FONT* create_font(const char* path) {
  FONT* font = (FONT*)malloc(sizeof(FONT));
  if (font == NULL) {
    return NULL;
  }

  font->font = TTF_OpenFont(path, 24);
  if (font->font == NULL) {
    free(font);
    return NULL;
  }

  font->size = 24;
  return font;
}

void destroy_font(FONT* font) {
  if (font == NULL) {
    return;
  }
  TTF_CloseFont(font->font);
  free(font);
}

bool set_font_size(FONT* font, const int size) {
  if (font == NULL) {
    return false;
  }
  TTF_SetFontSize(font->font, (float)size);
  font->size = size;
  return true;
}

bool get_text_size(
  FONT* font, const char* text, const int size, int* width, int* height
) {
  bool status = set_font_size(font, size);
  if (!status) {
    return status;
  }

  status = TTF_GetStringSize(font->font, text, 0, width, height);
  return status;
}

SDL_Texture* get_texture_from_text(
  FONT* font, SDL_Renderer* render, const char* text, const int size,
  const SDL_Color* color
) {
  if (render == NULL || text == NULL) {
    SDL_Log("render or/and text is/are NULL\n");
    return NULL;
  }

  const bool status = set_font_size(font, size);
  if (!status) {
    SDL_Log("set_font_size failed...\n");
    return NULL;
  }

  SDL_Surface* surface = TTF_RenderText_Solid(font->font, text, 0, *color);
  if (surface == NULL) {
    SDL_Log("TTF_RenderText_Solid failed: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
  if (texture == NULL) {
    SDL_Log("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    return NULL;
  }

  SDL_DestroySurface(surface);
  return texture;
}

bool render_text(SDL_Renderer* render, SDL_Texture* texture, const SDL_FPoint* point) {
  if (render == NULL || texture == NULL) {
    SDL_Log("render or/and text is/are NULL\n");
    return false;
  }

  float tex_width, tex_height;
  bool status = SDL_GetTextureSize(texture, &tex_width, &tex_height);
  if (!status) {
    SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
    SDL_DestroyTexture(texture);
    return false;
  }
  status = SDL_RenderTexture(
    render, texture, NULL,
    &(SDL_FRect){
      .x = point->x,
      .y = point->y,
      .w = tex_width,
      .h = tex_height,
    }
  );
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    SDL_DestroyTexture(texture);
    return false;
  }

  return status;
}

bool render_wrapped_text(
  FONT* font, SDL_Renderer* render, const char* text, const int size,
  const SDL_FPoint* point, const SDL_Color* color
) {
  if (render == NULL || text == NULL) {
    SDL_Log("render or/and text is/are NULL\n");
    return false;
  }

  bool status = set_font_size(font, size);
  if (!status) {
    SDL_Log("set_font_size failed...\n");
    return status;
  }

  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font->font, text, 0, *color, 0);
  if (surface == NULL) {
    SDL_Log("TTF_RenderText_Solid failed: %s\n", SDL_GetError());
    return false;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
  if (texture == NULL) {
    SDL_Log("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
    SDL_DestroySurface(surface);
    return false;
  }

  float tex_width, tex_height;
  status = SDL_GetTextureSize(texture, &tex_width, &tex_height);
  if (!status) {
    SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    return false;
  }

  status = SDL_RenderTexture(
    render, texture, NULL,
    &(SDL_FRect){
      point->x,
      point->y,
      (float)surface->w,
      (float)surface->h,
    }
  );
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    return false;
  }

  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);

  return status;
}
