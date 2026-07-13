#include "../game.h"
#include "../res/res.h"
#include "SDL3/SDL_render.h"

SDL_Texture* tex_settings_menu_items[3];
SDL_Texture* tex_hover_settings_menu_items[3];

// TODO ubaci u assets direktoriju .txt fajl koji uzima sav ovaj tekst
static char* normal_mode =
  "NORMAL mode:\n\nW-A-S-D/ARROWS - moving cursor;\nSPACE/ENTER - selecting "
  "and placing a card;\nESC - exiting a game";
static char* fly_mode =
  "FLY mode:\n\nUse numbers to jump to the card. For an example:\nby typing "
  "32 you will jump to 2nd card in the 3rd column.\nFirst number is column "
  "number and second is card in that column.\nIf you have more than 9 card "
  "in column you will type\n[column]00 for 10th card [column]01 for 11th "
  "card and so on.\nSPACE/ENTER - selecting and placing card.\nQ - Drawing "
  "cards from a deck.\nTAB - JUMP to the drawn card press.";
static char* universal_rule = "X - move to NORMAL mode\nC - move to FLY mode";

bool setting_event_handler(GAME* game, const SDL_Event* event) {
  if (event->type != SDL_EVENT_KEY_DOWN) {
    return 1;
  }
  switch (event->key.key) {
  case SDLK_W:
  case SDLK_UP:
  case SDLK_D:
  case SDLK_RIGHT:
    if (game->setting_menu->selected_item < settings_item_type_max - 1) {
      game->setting_menu->selected_item++;
    }
    break;
  case SDLK_S:
  case SDLK_DOWN:
  case SDLK_A:
  case SDLK_LEFT:
    if (game->setting_menu->selected_item > 0) {
      game->setting_menu->selected_item = game->setting_menu->selected_item - 1;
    }
    break;
  case SDLK_KP_ENTER:
  case SDLK_SPACE:
  case SDLK_RETURN:
    play_sound(game->soundboard, 0);
    SDL_Delay(100);
    switch (game->setting_menu->items[game->setting_menu->selected_item].type) {
    case settings_item_type_options:
      push_user_event(g_change_scene_event_type, game_state_option);
      break;
    case settings_item_type_cancel:
      push_user_event(g_change_scene_event_type, game_state_main_menu);
      break;
    case settings_item_type_macro:
      push_user_event(g_change_scene_event_type, game_state_macro);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return 1;
}

bool setting_update(const GAME* game) {
  return 1;
}

bool render_guide(const GAME* game) {

  int text_width, text_height;
  int status = set_font_size(game->font, fonts.text_font);
  if (!status) {
    SDL_Log("get_text_size error\n");
    return 0;
  }
  status = render_wrapped_text(
    game->font, game->renderer, normal_mode, fonts.text_font,
    &(SDL_FPoint){
      .x = screen_dimens.padding,
      .y = fonts.title_padding,
    },
    &(SDL_Color){255, 255, 255, 255}
  );
  if (!status) {
    SDL_Log("render_wrapped_text error\n");
    return 0;
  }

  status = render_wrapped_text(
    game->font, game->renderer, universal_rule, fonts.text_font,
    &(SDL_FPoint){
      .x = screen_dimens.padding,
      .y = (resolution.height / 1.5F) + fonts.title_padding,
    },
    &(SDL_Color){255, 255, 255, 255}
  );
  if (!status) {
    SDL_Log("render_wrapped_text error\n");
    return 0;
  }

  status = render_wrapped_text(
    game->font, game->renderer, fly_mode, fonts.text_font,
    &(SDL_FPoint){
      .x = screen_dimens.padding,
      .y = (resolution.height / 4.0F) + fonts.text_padding,
    },
    &(SDL_Color){255, 255, 255, 255}
  );
  if (!status) {
    SDL_Log("render_wrapped_text error\n");
    return 0;
  }

  status = get_text_size(
    game->font, game->setting_menu->items[0].text, fonts.item_font, &text_width,
    &text_height
  );
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  int selected_text_width, selected_text_height;
  status = get_text_size(
    game->font, game->setting_menu->items[0].text, fonts.item_hover_font,
    &selected_text_width, &selected_text_height
  );
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  SDL_Color selected_color = {150, 255, 150, 255};

  float not_selected_width = screen_dimens.padding;
  float selected_width = screen_dimens.padding;
  const float y_pos =
    resolution.height - (float)text_height - (screen_dimens.padding * 2);

  SDL_FPoint not_selected_point = (SDL_FPoint){
    .x = not_selected_width,
    .y = y_pos,
  };
  SDL_FPoint selected_point = (SDL_FPoint){
    .x = selected_width,
    .y = y_pos,
  };
  for (int i = 0; i < game->setting_menu->count; i++) {
    const SDL_FPoint* point = &not_selected_point;
    SDL_Texture* item = tex_settings_menu_items[i];

    if (i == game->setting_menu->selected_item) {
      point = &selected_point;
      item = tex_hover_settings_menu_items[i];
    }

    status = render_text(game->renderer, item, point);
    if (!status) {
      SDL_Log("render_text error...\n");
    }

    if (i != game->setting_menu->count - 1) {
      status = get_text_size(
        game->font, game->setting_menu->items[i + 1].text, fonts.item_font, &text_width,
        &text_height
      );
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return 0;
      }
      status = get_text_size(
        game->font, game->setting_menu->items[i + 1].text, fonts.item_hover_font,
        &selected_text_width, &selected_text_height
      );
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return 0;
      }
    }
    not_selected_width +=
      ((resolution.width - (float)text_width) / 2) - screen_dimens.padding;
    selected_width +=
      ((resolution.width - (float)selected_text_width) / 2) - screen_dimens.padding;
    not_selected_point.x = not_selected_width;
    selected_point.x = selected_width;
  }

  return status;
}

bool setting_render(GAME* game) {

  if (game->background_texture == NULL) {
    SDL_Log("background is NULL\n");
    return 0;
  }
  int status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (!status) {
    SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
    return status;
  }

  status = render_guide(game);
  if (!status) {
    SDL_Log("render_guide error\n");
    return status;
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent error %s\n", SDL_GetError());
    return 0;
  }

  return status;
}

bool setting_lazy_load(const GAME* game) {
  int size = fonts.item_font;
  for (int i = 0; i < 3; i++) {
    tex_settings_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->setting_menu->items[i].text, size, &colors.white
    );
    if (tex_settings_menu_items[i] == NULL) {
      SDL_Log("menu_items[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  size = fonts.item_hover_font;
  for (int i = 0; i < 3; i++) {
    tex_hover_settings_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->setting_menu->items[i].text, size, &colors.green
    );
    if (tex_hover_settings_menu_items[i] == NULL) {
      SDL_Log("menu_items[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  return 1;
}

void setting_lazy_destroy() {
  for (int i = 0; i < 3; i++) {
    SDL_DestroyTexture(tex_settings_menu_items[i]);
    SDL_DestroyTexture(tex_hover_settings_menu_items[i]);
  }
}

SCENE setting_scene = {
  .handle_events = setting_event_handler,
  .update = setting_update,
  .render = setting_render,
  .lazy_load = setting_lazy_load,
  .lazy_destroy = setting_lazy_destroy,
};
