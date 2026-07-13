#ifndef SOVIMTER_RES_H
#define SOVIMTER_RES_H
#include "font.h"
#include <SDL3/SDL_pixels.h>

typedef struct {
  SDL_Color white;
  SDL_Color green;
  // SDL_Color backgound;
  SDL_Color title;
  // SDL_Color selected;
} COLOR;
extern const COLOR colors;

typedef struct {
  const char* sovimter;
  const char* main_menu;
  const char* macro_settings;
  const char* option_settings;
  const char* gameplay_won;
  const char* gameplay_lost;
  const char* scores;
} TITLES;

typedef struct {
  const char* main_menu[4];
  const uint32_t count_main_menu;
  const char* game_over[3];
  const uint32_t count_game_over;
  const char* settings[3];
  const uint32_t count_settings;
} ITEMS;

typedef struct {
  TITLES titles;
  ITEMS items;
} TEXT;
extern const TEXT texts;

typedef struct {
  const char* background;
  const char* logo;
  const char* face_down_card;
  const char* selected_face_down_card;
  const char* empty_field;
  const char* cursor;
  // TODO: maybe add array of all 52 cards
  const char* cards;
} IMAGE_PATH;

typedef struct {
  const char* config;
  const char* option;
} BIN_PATH;

// typedef struct {
//   const char* click;
//   const char* select_click;
//   const char* draw_card;
//   const char* blip_sound;
//   const char* sort_sound;
//   const char* place_card_sound;
//   const char* move_sound;
// } SOUND_PATH;

typedef struct {
  IMAGE_PATH images;
  const char* sounds[7];
  BIN_PATH bins;
  const char* font;
} PATHS;
extern const PATHS paths;

// typedef struct {
//   SDL_Texture *background;
// } TEXTURE;

typedef struct {
  float width;
  float height;
} RESOLUTION;
extern RESOLUTION resolution;

typedef struct {
  float width_padding;
  float height_padding;
  float width;
  float height;
} CARD_DIMENS;
extern CARD_DIMENS card_dimens;

typedef struct {
  float width;
  float height;
  float padding;
} CURSOR_DIMENS;
extern CURSOR_DIMENS cursor_dimens;

typedef struct {
  float padding_width;
  float padding_height;
} GAMEPLAY_DIMENS;
extern GAMEPLAY_DIMENS game_dimens;

typedef struct {
  float width;
  float height;
  // NOTE: Possibly margin
  float margin;
} HERO_LOGO_DIMENS;
extern HERO_LOGO_DIMENS hero_logo_dimens;

typedef struct {
  float width;
  float height;
} TEXTBOX_DIMENS;
extern TEXTBOX_DIMENS textbox_dimens;

typedef struct {
  float padding;
} DEFAULT_SCREEN_DIMENS;
extern DEFAULT_SCREEN_DIMENS screen_dimens;

typedef struct {
  int title_font;
  float title_width;
  float title_height;
  float title_padding;

  int item_font;
  float item_padding;
  int item_hover_font;

  int text_font;
  float text_padding;
} FONT_DIMENS;
extern FONT_DIMENS fonts;

bool load_field(int width, int height, FONT* font);

#endif
