#include "../card.h"
#include "../cursor.h"
#include "../game.h"
#include "../res/font.h"
#include "../res/res.h"
#include "../res/textbox.h"
#include "../res/texture.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "gameplay.h"

#include <string.h>

static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color green_color = {150, 255, 150, 255};
static SDL_Color trensparent_green_color = {250, 55, 50, 255};

static const char* modes[4] = {
  "mode:normal", "mode:normal-select", "mode:fly", "mode:fly-select"
};

static SDL_Texture* tex_modes[4];
static SDL_Texture* tex_small_buffer;
static SDL_Texture* tex_timer_text;
static SDL_Texture* tex_text_box;
static SDL_Texture* tex_fly_notaions;
static SDL_Texture* tex_text_name_a_record;

TEXTBOX* textbox;

bool gameplay_lazy_load(const GAME* game) {
  textbox = create_textbox(4);
  start_timer(game->timer);

  int size = fonts.text_font;
  for (int i = 0; i < 4; i++) {
    tex_modes[i] =
      get_texture_from_text(game->font, game->renderer, modes[i], size, &white_color);
    if (tex_modes[i] == NULL) {
      SDL_Log("tex_modes[%d]  cannot be initiazlied...", i);
      return false;
    }
  }

  size = fonts.item_font;
  for (int i = 0; i < 4; i++) {
    tex_text_name_a_record = get_texture_from_text(
      game->font, game->renderer, "Name a Record", size, &white_color
    );
    if (tex_text_name_a_record == NULL) {
      SDL_Log("tex_text_name_a_record cannot be initiazlied...");
      return false;
    }
  }

  const float height = screen_dimens.padding;

  const float rect_width = textbox_dimens.width;
  const float rect_height = textbox_dimens.height;

  const float height_indent = (height - rect_height) / 2;
  const float width_indent = game_dimens.padding_width;

  SDL_FRect rect = {
    .x = width_indent, .y = height_indent, .w = rect_width, .h = rect_height
  };

  SDL_Surface* surface =
    SDL_CreateSurface((int)rect_width, (int)rect_height, SDL_PIXELFORMAT_RGBA32);
  if (surface == NULL) {
    SDL_Log("SDL_CreateSurface error: %s\n", SDL_GetError());
    return false;
  }

  const SDL_PixelFormatDetails* format_details =
    SDL_GetPixelFormatDetails(surface->format);
  if (format_details == NULL) {
    SDL_DestroySurface(surface);
    SDL_Log("SDL_GetPixelFormatDetails error: %s\n", SDL_GetError());
    return false;
  }

  const Uint32 color = SDL_MapRGBA(format_details, NULL, 255, 255, 255, 255);

  bool status = SDL_FillSurfaceRect(surface, NULL, color);
  if (!status) {
    SDL_DestroySurface(surface);
    SDL_Log("SDL_FillSurfaceRect error: %s\n", SDL_GetError());
    return false;
  }

  tex_text_box = SDL_CreateTextureFromSurface(game->renderer, surface);
  if (tex_text_box == NULL) {
    SDL_DestroySurface(surface);
    SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
    return false;
  }

  SDL_DestroySurface(surface);

  surface = SDL_CreateSurface((int)rect_width, (int)rect_height, SDL_PIXELFORMAT_RGBA32);
  if (surface == NULL) {
    SDL_Log("SDL_CreateSurface error: %s\n", SDL_GetError());
    return false;
  }

  // const SDL_PixelFormatDetails *format_details =
  // SDL_GetPixelFormatDetails(surface->format); if (format_details == NULL) {
  //     SDL_DestroySurface(surface);
  //     SDL_Log("SDL_GetPixelFormatDetails error: %s\n", SDL_GetError());
  //     return 0;
  // }

  const Uint32 transparent_color = SDL_MapRGBA(format_details, NULL, 255, 255, 255, 25);

  status = SDL_FillSurfaceRect(surface, NULL, transparent_color);
  if (!status) {
    SDL_DestroySurface(surface);
    SDL_Log("SDL_FillSurfaceRect error: %s\n", SDL_GetError());
    return false;
  }

  tex_fly_notaions = SDL_CreateTextureFromSurface(game->renderer, surface);
  if (tex_fly_notaions == NULL) {
    SDL_DestroySurface(surface);
    SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
    return false;
  }

  SDL_DestroySurface(surface);
  return true;
}

void gameplay_lazy_destroy() {
  // TODO mozda treba da se posalje tekst pre nego sto se obrise da bi moglo da
  // se sacuva
  destroy_textbox(textbox);
  for (int i = 0; i < 2; i++) {
    SDL_DestroyTexture(tex_modes[i]);
  }
  SDL_DestroyTexture(tex_text_name_a_record);
  SDL_DestroyTexture(tex_text_box);
  SDL_DestroyTexture(tex_fly_notaions);
}

bool render_commands(const GAME* game) {
  if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
    return true;
  }

  if (have_number_hover(game->cursor) == 0) {
    return true;
  }

  int text_width, text_height;
  if (strlen(buffer) == 0) {
    return true;
  }
  bool status =
    get_text_size(game->font, buffer, fonts.text_font, &text_width, &text_height);
  if (!status) {
    return false;
  }

  tex_small_buffer = get_texture_from_text(
    game->font, game->renderer, buffer, fonts.text_font, &white_color
  );
  if (tex_small_buffer == NULL) {
    SDL_Log("tex_small_buffer is NULL\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_small_buffer,
    &(SDL_FPoint){
      .x = (resolution.width - (float)text_width) / 2,
      .y = (resolution.height - (float)text_height - card_dimens.height_padding),
    }
  );
  if (!status) {
    SDL_Log("render_text error...\n");
    return false;
  }
  SDL_DestroyTexture(tex_small_buffer);
  return true;
}

bool render_cursor(const GAME* game) {
  const char* path = paths.images.cursor;
  SDL_Texture* texture = create_texture_from_image(game->renderer, path);
  if (texture == NULL) {
    return false;
  }

  if (game->cursor->cursor == NULL) {
    return false;
  }

  const bool status =
    SDL_RenderTexture(game->renderer, texture, NULL, game->cursor->cursor);
  if (!status) {
    SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
    return false;
  }

  SDL_DestroyTexture(texture);
  return true;
}

bool sorted_card_render(const GAME* game) {
  bool status;
  float padding_width =
    (3.0F * (card_dimens.width + card_dimens.width_padding)) + game_dimens.padding_width;
  for (int suit = 0; suit < 4; suit++) {
    if (game->deck->sorted_cards[suit] == NULL) {
      status = SDL_RenderTexture(
        game->renderer, game->deck->empty_sorted_card, NULL,
        &(SDL_FRect){
          .x = padding_width,
          .y = game_dimens.padding_height,
          .w = card_dimens.width,
          .h = card_dimens.height,
        }
      );
      if (!status) {
        SDL_Log("SDL_RenderTexture error %s\n", SDL_GetError());
        return status;
      }
    } else {
      status = render_card(
        game->renderer, game->deck->sorted_cards[suit],
        &(SDL_FPoint){.x = padding_width, .y = game_dimens.padding_height}
      );
      if (!status) {
        SDL_Log("render_card error...\n");
        return status;
      }
    }
    padding_width += card_dimens.width + card_dimens.width_padding;
  }

  return true;
}

bool render_name_textbox(const GAME* game) {
  if (g_game_win == 0) {
    return true;
  }

  int text_width, text_height;
  bool status =
    get_text_size(game->font, "GGGG", fonts.title_font, &text_width, &text_height);
  if (!status) {
    SDL_Log("get_text_size in render_name_textbox\n");
    return false;
  }

  const float rect_width = (float)text_width;
  const float rect_height = (float)text_height;
  const float width_indent = (resolution.width - (float)text_width) / 2;
  const float height_indent = (resolution.height - (float)rect_height) / 2;

  status = get_text_size(
    game->font, "Name a Record", fonts.item_font, &text_width, &text_height
  );
  if (!status) {
    SDL_Log("get_text_size in render_name_textbox\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_text_name_a_record,
    &(SDL_FPoint){
      .x = (resolution.width - (float)text_width) / 2,
      .y = height_indent - fonts.title_padding,
    }
  );
  if (!status) {
    SDL_Log("render_text error: %s\n", SDL_GetError());
    return false;
  }

  const SDL_FRect rect = {
    .x = width_indent,
    .y = height_indent,
    .w = rect_width,
    .h = rect_height,
  };

  status = SDL_RenderTexture(game->renderer, tex_text_box, NULL, &rect);
  if (!status) {
    SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
    return false;
  }

  if (strlen(textbox->string) == 0) {
    return true;
  }

  SDL_Texture* tex_text_in_textbox = get_texture_from_text(
    game->font, game->renderer, textbox->string, fonts.title_font,
    &trensparent_green_color
  );
  if (tex_text_in_textbox == NULL) {
    SDL_Log("tex_text_in_textbox is NULL & get_texture_from_text error...\n");
    return false;
  }

  const SDL_FPoint point = {
    .x = width_indent,
    .y = height_indent,
  };

  status = render_text(game->renderer, tex_text_in_textbox, &point);
  if (!status) {
    SDL_Log("render_text error... in render_name_textbox\n");
    SDL_DestroyTexture(tex_text_in_textbox);
    return false;
  }

  SDL_DestroyTexture(tex_text_in_textbox);
  return true;
}

bool render_fly_notaions(const GAME* game) {
  if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
    return true;
  }
  const float height_indent = game_dimens.padding_height + card_dimens.height +
                              card_dimens.height_padding + 2 * card_dimens.height_padding;
  const float width_indent = game_dimens.padding_width;

  const float rect_width = resolution.width - (2 * game_dimens.padding_width);
  const float rect_height = card_dimens.height_padding;

  SDL_FRect frect = {
    .x = width_indent, .y = height_indent, .w = rect_width, .h = rect_height
  };

  for (int i = 0; i < 4; i++) {
    const bool status = SDL_RenderTexture(game->renderer, tex_fly_notaions, NULL, &frect);
    if (!status) {
      SDL_DestroyTexture(tex_fly_notaions);
      SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
      return false;
    }
    frect.y = frect.y + (3 * card_dimens.height_padding);
  }

  return true;
}

bool render_counting_time(const GAME* game) {
  // Rendering timer in seconds
  char timer[20];
  int text_width, text_height;
  snprintf(timer, 20, "seconds: %d", game->timer->time_elapsed);
  bool status =
    get_text_size(game->font, timer, fonts.item_font, &text_width, &text_height);
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return status;
  }

  tex_timer_text = get_texture_from_text(
    game->font, game->renderer, timer, fonts.item_font, &white_color
  );
  if (tex_timer_text == NULL) {
    SDL_Log("tex_timer_text is NULL\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_timer_text,
    &(SDL_FPoint){
      .x = resolution.width - screen_dimens.padding - (float)text_width,
      .y = resolution.height - screen_dimens.padding - (float)text_height,
    }
  );
  if (!status) {
    SDL_Log("render_text error...\n");
    return status;
  }
  SDL_DestroyTexture(tex_timer_text);
  return status;
}

bool gameplay_render(GAME* game) {
  if (game == NULL) {
    SDL_Log("game is NULL\n");
    return false;
  }

  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return false;
  }

  bool status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    return false;
  }

  status = render_fly_notaions(game);
  if (!status) {
    SDL_Log("render_fly_notaions failed... \n");
    return false;
  }

  status = render_commands(game);
  if (!status) {
    SDL_Log("render_commands error...\n");
    return false;
  }

  // *RENDERING SORTED_CARDS
  status = sorted_card_render(game);
  if (!status) {
    SDL_Log("sorted_card_render error...\n");
    return false;
  }

  // *RENDERING DECK AND ONE MORE CARD ON LEFT RIGHT CORNER
  if (game->deck->deck_card != NULL) {
    status = render_card(
      game->renderer, game->deck->deck_card,
      &(SDL_FPoint){.x = game_dimens.padding_width, .y = game_dimens.padding_height}
    );
    if (!status) {
      SDL_Log("render card error...\n");
      return false;
    }
  }
  if (game->cursor->pos->col == 1 && game->cursor->pos->row == 0) {
    status = render_cursor(game);
    if (!status) {
      SDL_Log("render_cursor error...\n");
      return false;
    }
  }

  const CARD* card = top_card(&game->deck->drawn_cards);
  if (card != NULL) {
    status = render_card(
      game->renderer, card,
      &(SDL_FPoint){.x = game_dimens.padding_width + card_dimens.width_padding +
                         card_dimens.width,
                    .y = game_dimens.padding_height}
    );
    if (!status) {
      SDL_Log("render card error...\n");
    }
  }
  if (game->cursor->pos->col == 2 && game->cursor->pos->row == 0) {

    status = render_cursor(game);
    if (!status) {
      SDL_Log("render_cursor error...\n");
      return false;
    }
  }

  // * RENDERING TEXT THAT SHOWS WHAT IN WHAT MODE YOU'RE
  const int mode = game->cursor->mode % 4;
  int text_width, text_height;
  status =
    get_text_size(game->font, modes[mode], fonts.text_font, &text_width, &text_height);
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_modes[mode],
    &(SDL_FPoint){.x = screen_dimens.padding,
                  .y = resolution.height - (float)text_height - screen_dimens.padding}
  );
  if (!status) {
    SDL_Log("render_text error...\n");
    return false;
  }

  // * RENDERING GAME FIELD
  for (int i = 1; i <= number_of_cards_in_row; ++i) {
    int j = 1;
    CARD* tmp_card;
    while ((tmp_card = find_card(game->deck, i, j)) != NULL) {
      status = render_card(game->renderer, tmp_card, tmp_card->frame);
      if (!status) {
        SDL_Log("render_card error...\n");
        return false;
      }

      if (game->cursor->pos->col == i && game->cursor->pos->row == j) {

        status = render_cursor(game);
        if (!status) {
          SDL_Log("render_cursor error...\n");
          return false;
        }
      }
      j++;
    }
    if (game->cursor->pos->col == i && game->cursor->pos->row == j) {

      status = render_cursor(game);
      if (!status) {
        SDL_Log("render_cursor error...\n");
        return false;
      }
    }
  }

  status = (bool)render_name_textbox(game);
  if (!status) {
    SDL_Log("render_name_textbox failed");
    return false;
  }

  status = render_counting_time(game);
  if (!status) {
    SDL_Log("render_counting_time failed");
    return false;
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    return false;
  }

  return true;
}
