//
// Created by luka on 7/12/26.
//
#include "res.h"
#include "sound.h"

#define CARD_WIDTH_HEIGHT_RATIO (7 / 5.0)

const COLOR colors = {
  .white = {255, 255, 255, 255},
  .green = {150, 150, 150, 255},
  .title = {230, 230, 230, 255},
};

const TEXT texts = {
  .titles =
    {
      .main_menu = "MAIN MENU",
      .macro_settings = "MACRO SETTINGS",
      .option_settings = "OPTION SETTINGS",
      .gameplay_won = "YOU WON",
      .gameplay_lost = "YOU LOST",
      .scores = "SCORES",
    },
  .items = {
    .main_menu =
      {
        "Play",
        "Settings",
        "Scores",
        "Exit",
      },
    .count_main_menu = 4,
    .game_over =
      {
        "Restart",
        "Scores",
        "Main Menu",
      },
    .count_game_over = 3,
    .settings =
      {
        "Options",
        "Cancel",
        "Macros",
      },
    .count_settings = 3,
  },
};

GAMEPLAY_DIMENS game_dimens = {
  .padding_width = 10.0F,
  .padding_height = 20.0F,
};

CURSOR_DIMENS cursor_dimens = {
  .width = 0.0F,
  .height = 0.0F,
  .padding = 7.0F,
};

CARD_DIMENS card_dimens = {
  .width_padding = 30,
  .height_padding = 25,
  .width = 100.0F,
  .height = 140.0F,
};

DEFAULT_SCREEN_DIMENS screen_dimens = {
  .padding = 20.0F
};

TEXTBOX_DIMENS textbox_dimens = {
  .width = 880,
  .height = 125,
};

const PATHS paths = {
  .bins =
    {
      .config = "assets/bin/config.bin",
      .option = "assets/bin/option.bin",
    },
  .images =
    {
      .background = "assets/background.png",
      .cursor = "assets/cursor.png",
      .logo = "assets/logo.png",
      .face_down_card = "assets/cards/back_red_basic_white.png",
      .selected_face_down_card = "assets/cards/back_red_basic.png",
      .empty_field = "assets/cards/total_blank_front_white.png",
    },
  .sounds =
    {
      [click_sound] = "assets/sounds/click_sound.wav",
      [select_card_sound] = "assets/sounds/select_card_sound.wav",
      [draw_card_sound] = "assets/sounds/draw_card_sound.wav",
      [blip_select_sound] = "assets/sounds/blip_select_sound.wav",
      [sort_card_sound] = "assets/sounds/sort_card_sound.wav",
      [place_card_sound] = "assets/sounds/place_card_sound.wav",
      [move_sound] = "assets/sounds/move_sound.wav",
    },
  .font = "assets/font.ttf",
};

RESOLUTION resolution = {
  .width = 900,
  .height = 900,
};

FONT_DIMENS fonts = {
  .title_font = 90,
  .title_padding = 45,
  .item_font = 45,
  .item_padding = 22,
  .item_hover_font = 66,
  .text_font = 30,
  .text_padding = 15,
};