#include "macro_settings.h"
#include "../config.h"
#include "../game.h"
#include "../texture.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"

static int selected_index = 0;
static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color green_color = {150, 255, 150, 255};
static int event_status = 0;

static char *text[14] = {
    "number 0",       "number 1",          "number 2",
    "number 3",       "number 4",          "number 5",
    "number 6",       "number 7",          "number 8",
    "number 9",       "go to normal mode", "select drawn card",
    "draw next card", "select card/cards"};

static const char *press_key = "Press key to bind";
static char title[] = "Macro";

SDL_Texture *tex_hover_items[14];
SDL_Texture *tex_items[14];
SDL_Texture *tex_hover_command_keys[14];
SDL_Texture *tex_command_keys[14];
SDL_Texture *tex_title_menu;
SDL_Texture *tex_pop_out;

bool lazy_load_config(const GAME *game) {
  for (int i = 0; i < 14; i++) {
    SDL_DestroyTexture(tex_command_keys[i]);
    SDL_DestroyTexture(tex_hover_command_keys[i]);
  }

  for (int i = 0; i < 14; i++) {
    int size = game->field.item_font;
    tex_command_keys[i] = get_texture_from_text(
        game->font, game->renderer, commands_keys[i], size, &white_color);
    if (tex_items[i] == NULL) {
      SDL_Log("items[%d]  cannot be initiazlied...", i);
      return 0;
    }
    size = game->field.hover_item_font;
    tex_hover_command_keys[i] = get_texture_from_text(
        game->font, game->renderer, commands_keys[i], size, &green_color);
    if (tex_command_keys[i] == NULL) {
      SDL_Log("tex_command_keys[%d]  cannot be initiazlied...", i);
      return 0;
    }
  }
  return 1;
}

bool macro_setting_menu_lazy_load(const GAME *game) {

  int size = game->field.title_font;
  tex_title_menu = get_texture_from_text(game->font, game->renderer, title,
                                         size, &white_color);
  if (tex_title_menu == NULL) {
    SDL_Log("title_menu cannot be initiazlied...");
    return false;
  }

  tex_pop_out = get_texture_from_text(game->font, game->renderer, press_key,
                                      size, &white_color);
  if (tex_pop_out == NULL) {
    SDL_Log("tex_pop_out cannot be initiazlied...");
    return false;
  }

  size = game->field.item_font;
  for (int i = 0; i < 14; i++) {
    tex_items[i] = get_texture_from_text(game->font, game->renderer, text[i],
                                         size, &white_color);
    if (tex_items[i] == NULL) {
      SDL_Log("items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_command_keys[i] = get_texture_from_text(
        game->font, game->renderer, commands_keys[i], size, &white_color);
    if (tex_command_keys[i] == NULL) {
      SDL_Log("tex_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }

  size = game->field.hover_item_font;
  for (int i = 0; i < 14; i++) {
    tex_hover_items[i] = get_texture_from_text(game->font, game->renderer,
                                               text[i], size, &green_color);
    if (tex_hover_items[i] == NULL) {
      SDL_Log("hover_items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_hover_command_keys[i] = get_texture_from_text(
        game->font, game->renderer, commands_keys[i], size, &green_color);
    if (tex_hover_command_keys[i] == NULL) {
      SDL_Log("tex_hover_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }
  return 1;
}

void macro_settings_menu_lazy_destroy() {
  SDL_DestroyTexture(tex_title_menu);
  SDL_DestroyTexture(tex_pop_out);
  for (int i = 0; i < 14; i++) {
    SDL_DestroyTexture(tex_items[i]);
    SDL_DestroyTexture(tex_command_keys[i]);
    SDL_DestroyTexture(tex_hover_items[i]);
    SDL_DestroyTexture(tex_hover_command_keys[i]);
  }
}
bool macro_settings_event_handler(GAME *game, const SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event_status == 1) {
      const unsigned int key = event->key.key;
      insert_command(key, selected_index);
      event_status = 0;
      return 1;
    }
    switch (event->key.key) {
    case SDLK_ESCAPE:
      update_config_file();
      push_user_event(g_change_scene_event_type, game_state_setting);
      break;
    case SDLK_W:
    case SDLK_UP:
      if (selected_index <= 0) {
        break;
      }
      selected_index--;
      break;
    case SDLK_S:
    case SDLK_DOWN:
      if (selected_index >= 13) {
        break;
      }
      selected_index++;
      break;
    case SDLK_RETURN:
    case SDLK_SPACE:
      play_sound(game->soundboard, 0);
      SDL_Delay(100);
      event_status = 1;
      break;
    default:
      break;
    }
  }
  return true;
}

int render_press_key_popout(const GAME *game) {
  int width, height;
  int status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
  if (status == 0) {
    SDL_Log("SDL_GetWindowSizeInPixels error: %s\n", SDL_GetError());
    return status;
  }

  int text_width, text_height;
  status = get_text_size(game->font, press_key,
                         game->field.title_font, // ? nzm da li je font okej?
                         &text_width, &text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  status = render_text(game->renderer, tex_pop_out,
                       &(SDL_Point){.x = (width - text_width) / 2,
                                    .y = (height - text_height) / 2});
  return status;
}

static int flag = 0;
bool macro_settings_update(const  GAME *game) {
  if (event_status == 1) {
    flag = 1;
  }
  if (event_status == 0 && flag == 1) {
    const int status = lazy_load_config(game);
    if (status == 0) {
      SDL_Log("lazy_load_config\n");
      return 0;
    }
  }
  return 1;
}

bool macro_settings_render(GAME *game) {
  if (game == NULL) {
    SDL_Log("game is NULL\n");
    return 0;
  }

  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return 0;
  }
  int width, height;
  int status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
  if (status == 0) {
    SDL_Log("SDL_GetWindowSizeInPixels error: %s\n", SDL_GetError());
    return status;
  }

  if (game->background_texture == NULL) {
    SDL_Log("g_background_texture is NULL\n");
    game->background_texture =
        create_texture_from_image(game->renderer, "assets/background.png");
    if (game->background_texture == NULL) {
      SDL_Log("Background texture is NULL\n");
      return 0;
    }
  }

  status = SDL_RenderClear(game->renderer);
  if (status == 0) {
    SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    return 0;
  }

  status =
      SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (status == 0) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    printf("ZASTOOOOO\n");
    push_user_event(SDL_EVENT_QUIT, 0);
    return 0;
  }

  int title_width, title_height;
  status = get_text_size(game->font, title, game->field.title_font,
                         &title_width, &title_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  status = render_text(game->renderer, tex_title_menu,
                       &(SDL_Point){.x = (width - title_width) / 2,
                                    .y = game->field.screen_padding});
  if (status == 0) {
    SDL_Log("render_text error...\n");
    return 0;
  }

  int text_width, text_height;
  status = get_text_size(game->font,
                         "ht shift", // right indent for smth long like: right
                                     // shift, right ctrl, left ctrl...
                         game->field.item_font, &text_width, &text_height);
  if (status == 0) {
    SDL_Log("get_text_size error...\n");
    return 0;
  }

  int y_pos = (title_height + game->field.screen_padding);
  int text_x_pos = game->field.screen_padding;
  int commands_x_pos = width - text_width;
  int font = game->field.item_font;
  for (int i = 0; i < 14; i++) {
    SDL_Color *color;
    int font = game->field.item_font;
    int commands_y_pos;

    int commands_selected_height;
    if (selected_index == i) {
      status = get_text_size(game->font, commands_keys[i],
                             game->field.hover_item_font, &text_width,
                             &commands_selected_height);
      color = &green_color;
      font = game->field.hover_item_font;
      commands_y_pos = y_pos + (text_height - commands_selected_height) / 2;
    } else {
      status = get_text_size(game->font, commands_keys[i],
                             game->field.item_font, &text_width, NULL);
      color = &white_color;
      font = game->field.item_font;
      commands_y_pos = y_pos;
    }

    SDL_Texture *item;

    if (selected_index == i) {
      item = tex_hover_items[i];
    } else {
      item = tex_items[i];
    }

    status = render_text(game->renderer, item,
                         &(SDL_Point){.x = text_x_pos, .y = commands_y_pos});
    if (status == 0) {
      SDL_Log("render_text error...\n");
    }

    SDL_Texture *key;
    if (selected_index == i) {
      key = tex_hover_command_keys[i];
    } else {
      key = tex_command_keys[i];
    }

    status = render_text(game->renderer, key,
                         &(SDL_Point){.x = commands_x_pos - text_width / 2,
                                      .y = commands_y_pos});
    if (status == 0) {
      SDL_Log("render_text error...\n");
    }

    y_pos += text_height + game->field.text_padding / 2;
  }

  if (event_status == 1) {
    status = render_press_key_popout(game);
    if (status == 0) {
      SDL_Log("render_press_key_popout error\n");
      return status;
    }
  }

  status = SDL_RenderPresent(game->renderer);
  if (status == 0) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return 0;
  }

  return 1;
}

SCENE macro_setting_scene = {.handle_events = macro_settings_event_handler,
                             .update = macro_settings_update,
                             .render = macro_settings_render,
                             .lazy_load = macro_setting_menu_lazy_load,
                             .lazy_destroy = macro_settings_menu_lazy_destroy};
