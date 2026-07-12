//
// Created by luka on 7/12/26.
//
#include "res.h"

#define CARD_WIDTH_HEIGHT_RATIO (7/5.0)

const COLOR colors = {
  .white = {255, 255, 255, 255},
  .green = {150, 150, 150, 255},
  .title = {230, 230, 230, 255},
};

const TEXT texts = {
  .titles = {
    .main_menu = "MAIN MENU",
    .macro_settings = "MACRO SETTINGS",
    .option_settings = "OPTION SETTINGS",
    .gameplay_won = "YOU WON",
    .gameplay_lost = "YOU LOST",
    .scores = "SCORES",
  }
};

const PATHS paths = {
  .bins = {
    .config = "assets/bin/config.bin",
    .option = "assets/bin/option.bin",
  },
  .images = {
    .background = "assets/background.png",
    .cursor = "assets/cursor.png",
    .logo = "assests/logo.png",

  }
};

RESOLUTION res = {
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