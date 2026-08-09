//
// Created by luka on 7/12/26.
//
#include "res.h"
#include "sound.h"
#include "../card.h"

const COLOR colors = {
  .white = {255, 255, 255, 255},
  .green = {150, 255, 150, 255},
  .title = {23, 150, 52, 255},
};

const TEXT texts = {
  .titles =
    {
      .sovimter = "SoVIMter",
      .main_menu = "MAIN MENU",
      .macro_settings = "MACRO SETTINGS",
      .option_settings = "OPTIONS",
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

HERO_LOGO_DIMENS hero_logo_dimens = {
  .width = 0,
  .height = 0,
  .margin = 0
};
DEFAULT_SCREEN_DIMENS screen_dimens = {.padding = 20.0F};

TEXTBOX_DIMENS textbox_dimens = {
  .width = 880,
  .height = 125,
};

const PATHS paths = {
  .bins =
    {
      .config = "assets/bin/config.bin",
      .option = "assets/bin/option.bin",
      .save = "assets/bin/saves.bin",
    },
  .images =
    {
      .background = "assets/background.png",
      .cursor = "assets/cursor.png",
      .logo = "assets/logo.png",
      .cards = {
        .face_down_card = "assets/cards/back_red_basic_white.png",
        .selected_face_down_card = "assets/cards/back_red_basic.png",
        .empty_field = "assets/cards/total_blank_front_white.png",
        .deck =
          {
            [suit_clubs] =
              {
                [value_ace] = "assets/cards/ace_clubs_white.png",
                [value_2] = "assets/cards/2_clubs_white.png",
                [value_3] = "assets/cards/3_clubs_white.png",
                [value_4] = "assets/cards/4_clubs_white.png",
                [value_5] = "assets/cards/5_clubs_white.png",
                [value_6] = "assets/cards/6_clubs_white.png",
                [value_7] = "assets/cards/7_clubs_white.png",
                [value_8] = "assets/cards/8_clubs_white.png",
                [value_9] = "assets/cards/9_clubs_white.png",
                [value_10] = "assets/cards/10_clubs_white.png",
                [value_jack] = "assets/cards/jack_clubs_white.png",
                [value_queen] = "assets/cards/queen_clubs_white.png",
                [value_king] = "assets/cards/king_clubs_white.png",
              },
            [suit_spades] =
              {
                [value_ace] = "assets/cards/ace_spades_white.png",
                [value_2] = "assets/cards/2_spades_white.png",
                [value_3] = "assets/cards/3_spades_white.png",
                [value_4] = "assets/cards/4_spades_white.png",
                [value_5] = "assets/cards/5_spades_white.png",
                [value_6] = "assets/cards/6_spades_white.png",
                [value_7] = "assets/cards/7_spades_white.png",
                [value_8] = "assets/cards/8_spades_white.png",
                [value_9] = "assets/cards/9_spades_white.png",
                [value_10] = "assets/cards/10_spades_white.png",
                [value_jack] = "assets/cards/jack_spades_white.png",
                [value_queen] = "assets/cards/queen_spades_white.png",
                [value_king] = "assets/cards/king_spades_white.png",
              },
            [suit_diamonds] =
              {
                [value_ace] = "assets/cards/ace_diamonds_white.png",
                [value_2] = "assets/cards/2_diamonds_white.png",
                [value_3] = "assets/cards/3_diamonds_white.png",
                [value_4] = "assets/cards/4_diamonds_white.png",
                [value_5] = "assets/cards/5_diamonds_white.png",
                [value_6] = "assets/cards/6_diamonds_white.png",
                [value_7] = "assets/cards/7_diamonds_white.png",
                [value_8] = "assets/cards/8_diamonds_white.png",
                [value_9] = "assets/cards/9_diamonds_white.png",
                [value_10] = "assets/cards/10_diamonds_white.png",
                [value_jack] = "assets/cards/jack_diamonds_white.png",
                [value_queen] = "assets/cards/queen_diamonds_white.png",
                [value_king] = "assets/cards/king_diamonds_white.png",
              },
            [suit_hearts] =
              {
                [value_ace] = "assets/cards/ace_hearts_white.png",
                [value_2] = "assets/cards/2_hearts_white.png",
                [value_3] = "assets/cards/3_hearts_white.png",
                [value_4] = "assets/cards/4_hearts_white.png",
                [value_5] = "assets/cards/5_hearts_white.png",
                [value_6] = "assets/cards/6_hearts_white.png",
                [value_7] = "assets/cards/7_hearts_white.png",
                [value_8] = "assets/cards/8_hearts_white.png",
                [value_9] = "assets/cards/9_hearts_white.png",
                [value_10] = "assets/cards/10_hearts_white.png",
                [value_jack] = "assets/cards/jack_hearts_white.png",
                [value_queen] = "assets/cards/queen_hearts_white.png",
                [value_king] = "assets/cards/king_hearts_white.png",
              },
          },
        .selected_deck =
          {
            [suit_clubs] =
              {
                [value_ace] = "assets/cards/ace_clubs.png",
                [value_2] = "assets/cards/2_clubs.png",
                [value_3] = "assets/cards/3_clubs.png",
                [value_4] = "assets/cards/4_clubs.png",
                [value_5] = "assets/cards/5_clubs.png",
                [value_6] = "assets/cards/6_clubs.png",
                [value_7] = "assets/cards/7_clubs.png",
                [value_8] = "assets/cards/8_clubs.png",
                [value_9] = "assets/cards/9_clubs.png",
                [value_10] = "assets/cards/10_clubs.png",
                [value_jack] = "assets/cards/jack_clubs.png",
                [value_queen] = "assets/cards/queen_clubs.png",
                [value_king] = "assets/cards/king_clubs.png",
              },
            [suit_spades] =
              {
                [value_ace] = "assets/cards/ace_spades.png",
                [value_2] = "assets/cards/2_spades.png",
                [value_3] = "assets/cards/3_spades.png",
                [value_4] = "assets/cards/4_spades.png",
                [value_5] = "assets/cards/5_spades.png",
                [value_6] = "assets/cards/6_spades.png",
                [value_7] = "assets/cards/7_spades.png",
                [value_8] = "assets/cards/8_spades.png",
                [value_9] = "assets/cards/9_spades.png",
                [value_10] = "assets/cards/10_spades.png",
                [value_jack] = "assets/cards/jack_spades.png",
                [value_queen] = "assets/cards/queen_spades.png",
                [value_king] = "assets/cards/king_spades.png",
              },
            [suit_diamonds] =
              {
                [value_ace] = "assets/cards/ace_diamonds.png",
                [value_2] = "assets/cards/2_diamonds.png",
                [value_3] = "assets/cards/3_diamonds.png",
                [value_4] = "assets/cards/4_diamonds.png",
                [value_5] = "assets/cards/5_diamonds.png",
                [value_6] = "assets/cards/6_diamonds.png",
                [value_7] = "assets/cards/7_diamonds.png",
                [value_8] = "assets/cards/8_diamonds.png",
                [value_9] = "assets/cards/9_diamonds.png",
                [value_10] = "assets/cards/10_diamonds.png",
                [value_jack] = "assets/cards/jack_diamonds.png",
                [value_queen] = "assets/cards/queen_diamonds.png",
                [value_king] = "assets/cards/king_diamonds.png",
              },
            [suit_hearts] =
              {
                [value_ace] = "assets/cards/ace_hearts.png",
                [value_2] = "assets/cards/2_hearts.png",
                [value_3] = "assets/cards/3_hearts.png",
                [value_4] = "assets/cards/4_hearts.png",
                [value_5] = "assets/cards/5_hearts.png",
                [value_6] = "assets/cards/6_hearts.png",
                [value_7] = "assets/cards/7_hearts.png",
                [value_8] = "assets/cards/8_hearts.png",
                [value_9] = "assets/cards/9_hearts.png",
                [value_10] = "assets/cards/10_hearts.png",
                [value_jack] = "assets/cards/jack_hearts.png",
                [value_queen] = "assets/cards/queen_hearts.png",
                [value_king] = "assets/cards/king_hearts.png",
              },
          },
      },
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