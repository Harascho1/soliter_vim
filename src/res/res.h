//
// Created by luka on 7/12/26.
//

#ifndef SOVIMTER_RES_H
#define SOVIMTER_RES_H
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
  int width;
  int height;
} RESOLUTION;
extern RESOLUTION resolution;

typedef struct {
  int width_padding;
  int height_padding;
  int width;
  int height;
} CARD_DIMENS;

typedef struct {
  int width;
  int height;
  int padding;
} CURSOR_DIMENS;

typedef struct {
  int padding_width;
  int padding_height;
} GAMEPLAY_DIMENS;

typedef struct {
  int width;
  int height;
  // NOTE: Possibly margin
  int padding;
} HERO_LOGO_DIMENS;

typedef struct {
  int width;
  int height;
} TEXTBOX_DIMENS;

typedef struct {
  float padding;
} DEFAULT_SCREEN_DIMENS;

typedef struct {
  float padding;
} GAMEPLAY_SCREEN_DIMENS;

typedef struct {
  int title_font;
  int title_width;
  int title_height;
  int title_padding;

  int item_font;
  int item_padding;
  int item_hover_font;

  int text_font;
  int text_padding;
} FONT_DIMENS;
extern FONT_DIMENS fonts;

#endif // SOVIMTER_RES_H
