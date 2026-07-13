#ifndef FONT_H
#define FONT_H

#include "SDL3/SDL_render.h"
typedef struct {
  TTF_Font* font;
  int size;
} FONT;

FONT* create_font(const char* path);
void destroy_font(FONT* font);
bool get_text_size(FONT* font, const char* text, int size, int* width, int* height);
bool set_font_size(FONT* font, int size);
int render_wrapped_text(
  FONT* font, SDL_Renderer* render, const char* text, int size, const SDL_FPoint* point,
  const SDL_Color* color
);
SDL_Texture* get_texture_from_text(
  FONT* font, SDL_Renderer* render, const char* text, int size, const SDL_Color* color
);
bool render_text(SDL_Renderer* render, SDL_Texture* texture, const SDL_FPoint* point);

#endif // FONT_H
