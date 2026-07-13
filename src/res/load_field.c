#include "res.h"

bool load_field(const int width, const int height, FONT* font) {
  resolution.height = (float)height;
  resolution.width = (float)width;

  const float card_width_height_ratio = 7.0F / 5.0F;
  card_dimens.width_padding = (float)height / 30;
  card_dimens.height_padding = (float)height / 35;
  screen_dimens.padding = resolution.width / 45;
  hero_logo_dimens.margin = (resolution.width - resolution.height) / 2;
  card_dimens.width =
    (resolution.width - 6 * card_dimens.width_padding - game_dimens.padding_width * 2) /
    7;
  card_dimens.height = card_width_height_ratio * card_dimens.width;
  cursor_dimens.padding = card_dimens.width_padding / 4;

  textbox_dimens.width = resolution.width - (2 * (game_dimens.padding_width));
  textbox_dimens.height = textbox_dimens.width / 7;

  fonts.title_font = height / 10;

  int tmp_text_height;
  int tmp_text_width;
  int status =
    get_text_size(font, texts.titles.sovimter, fonts.title_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.title_padding = (float)tmp_text_height / 2;
  fonts.item_font = height / 20;

  status =
    get_text_size(font, texts.titles.sovimter, fonts.item_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.item_padding = (float)tmp_text_height / 2;
  fonts.item_hover_font = height / 15;
  fonts.text_font = height / 30;

  status =
    get_text_size(font, texts.titles.sovimter, fonts.text_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.text_padding = (float)tmp_text_height / 2;
  return true;
}
