#ifndef OPTION_SETTINGS_H
#define OPTION_SETTINGS_H

typedef struct {
  SDL_Texture* tex_hover_items[3];
  SDL_Texture* tex_items[3];
  SDL_Texture* tex_title_menu;
  SDL_Texture* tex_hover_options_set[3];
  SDL_Texture* tex_options_set[3];
} TEX_OPTIONS_SETTINGS;

TEX_OPTIONS_SETTINGS* tex_opt;

#endif
