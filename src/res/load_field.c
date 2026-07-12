#include "res.h"

bool load_field(const int screen_width, const int screen_height, FONT* font) {
  SDL_Log("width: %d & height: %d", screen_width, screen_height);
  resolution.height = (float)screen_height;
  resolution.width = (float)screen_width;

  const float card_width_height_ratio = 7.0F / 5.0F;
  card_dimens.width_padding = (float)screen_height / 30;
  SDL_Log("card_padding_width: %f", card_dimens.width_padding);
  card_dimens.height_padding = (float)screen_height / 35;
  SDL_Log("card_padding_height: %f", card_dimens.height_padding);
  screen_dimens.padding = resolution.width / 45;
  SDL_Log("screen_padding: %f", screen_dimens.padding);
  // SDL_Log("square_screen_padding_width: %f", field->square_screen_padding_width);
  // field->square_screen_padding_width = (screen_width - screen_height) / 2;
  card_dimens.width = (resolution.width - 6 * card_dimens.width_padding -
                       game_dimens.padding_width * 2) /
                      7;
  SDL_Log("card_width: %f", card_dimens.width);
  card_dimens.height = card_width_height_ratio * card_dimens.width;
  SDL_Log("card_height: %f", card_dimens.height);
  cursor_dimens.padding = card_dimens.width_padding / 4;
  SDL_Log("cursor_padding: %f", cursor_dimens.padding);

  textbox_dimens.width = resolution.width - (2 * (game_dimens.padding_width));
  SDL_Log("name_textbox_width: %f", textbox_dimens.width);
  textbox_dimens.height = textbox_dimens.width / 7;
  SDL_Log("name_textbox_height: %f", textbox_dimens.height);

  int tmp_text_height;
  int tmp_text_width;

  fonts.title_font = screen_height / 10;
  SDL_Log("title_font: %d", fonts.title_font);

  int status =
    get_text_size(font, "soVIMter", fonts.title_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.title_padding = (float)tmp_text_height / 2;
  SDL_Log("title_padding: %f", fonts.title_padding);

  fonts.item_font = screen_height / 20;
  SDL_Log("item_font: %d", fonts.item_font);

  status =
    get_text_size(font, "soVIMter", fonts.item_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.item_padding = (float)tmp_text_height / 2;
  SDL_Log("item_padding: %f", fonts.item_padding);

  fonts.item_hover_font = screen_height / 15;
  SDL_Log("hover_item_font: %d", fonts.item_hover_font);

  fonts.text_font = screen_height / 30;
  SDL_Log("text_font: %d", fonts.text_font);

  status =
    get_text_size(font, "soVIMter", fonts.text_font, &tmp_text_width, &tmp_text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  fonts.text_padding = (float)tmp_text_height / 2;
  SDL_Log("text_padding: %f", fonts.text_padding);

  return 1;
}
