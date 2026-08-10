#include "macro_settings.h"
#include "../game.h"
#include "../res/config.h"
#include "../res/res.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"

static int selected_index = 0;
static int event_status = 0;

static char* text[14] = {"number 0",       "number 1",          "number 2",
                         "number 3",       "number 4",          "number 5",
                         "number 6",       "number 7",          "number 8",
                         "number 9",       "go to normal mode", "select drawn card",
                         "draw next card", "select card/cards"};

static const char* press_key = "Press key to bind";
static char title[] = "Macro";

static SDL_Texture* tex_hover_items[14];
static SDL_Texture* tex_items[14];
SDL_Texture* tex_hover_command_keys[14];
SDL_Texture* tex_command_keys[14];
static SDL_Texture* tex_title_menu;
SDL_Texture* tex_pop_out;

static bool lazy_load_config(const GAME* game) {
  for (int i = 0; i < 14; i++) {
    SDL_DestroyTexture(tex_command_keys[i]);
    SDL_DestroyTexture(tex_hover_command_keys[i]);
  }

  for (int i = 0; i < 14; i++) {
    int size = fonts.item_font;
    tex_command_keys[i] = get_texture_from_text(
      game->font, game->renderer, commands_keys[i], size, &colors.white
    );
    if (tex_items[i] == NULL) {
      SDL_Log("items[%d]  cannot be initiazlied...", i);
      return false;
    }
    size = fonts.item_hover_font;
    tex_hover_command_keys[i] = get_texture_from_text(
      game->font, game->renderer, commands_keys[i], size, &colors.green
    );
    if (tex_command_keys[i] == NULL) {
      SDL_Log("tex_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

static bool macro_setting_menu_lazy_load(const GAME* game) {

  int size = fonts.title_font;
  tex_title_menu =
    get_texture_from_text(game->font, game->renderer, title, size, &colors.white);
  if (tex_title_menu == NULL) {
    SDL_Log("title_menu cannot be initiazlied...");
    return false;
  }

  tex_pop_out =
    get_texture_from_text(game->font, game->renderer, press_key, size, &colors.white);
  if (tex_pop_out == NULL) {
    SDL_Log("tex_pop_out cannot be initiazlied...");
    return false;
  }

  size = fonts.item_font;
  for (int i = 0; i < 14; i++) {
    tex_items[i] =
      get_texture_from_text(game->font, game->renderer, text[i], size, &colors.white);
    if (tex_items[i] == NULL) {
      SDL_Log("items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_command_keys[i] = get_texture_from_text(
      game->font, game->renderer, commands_keys[i], size, &colors.white
    );
    if (tex_command_keys[i] == NULL) {
      SDL_Log("tex_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }

  size = fonts.item_hover_font;
  for (int i = 0; i < 14; i++) {
    tex_hover_items[i] =
      get_texture_from_text(game->font, game->renderer, text[i], size, &colors.green);
    if (tex_hover_items[i] == NULL) {
      SDL_Log("hover_items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_hover_command_keys[i] = get_texture_from_text(
      game->font, game->renderer, commands_keys[i], size, &colors.green
    );
    if (tex_hover_command_keys[i] == NULL) {
      SDL_Log("tex_hover_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

static void macro_settings_menu_lazy_destroy() {
  SDL_DestroyTexture(tex_title_menu);
  SDL_DestroyTexture(tex_pop_out);
  for (int i = 0; i < 14; i++) {
    SDL_DestroyTexture(tex_items[i]);
    SDL_DestroyTexture(tex_command_keys[i]);
    SDL_DestroyTexture(tex_hover_items[i]);
    SDL_DestroyTexture(tex_hover_command_keys[i]);
  }
}
static bool macro_settings_event_handler(GAME* game, const SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event_status == 1) {
      const unsigned int key = event->key.key;
      insert_command(key, selected_index);
      event_status = 0;
      return true;
    }
    bool status;
    switch (event->key.key) {
    case SDLK_ESCAPE:
      status = update_config_file();
      if (!status) {
        SDL_Log("Config file did not update");
        return false;
      }
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

static bool render_press_key_popout(const GAME* game) {
  int text_width, text_height;
  bool status = get_text_size(
    game->font, press_key,
    fonts.title_font, // ? nzm da li je font okej?
    &text_width, &text_height
  );
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_pop_out,
    &(SDL_FPoint){
      .x = (resolution.width - (float)text_width) / 2,
      .y = (resolution.height - (float)text_height) / 2,
    }
  );
  return status;
}

static int flag = 0;
static bool macro_settings_update(const GAME* game) {
  if (event_status == 1) {
    flag = 1;
  }
  if (event_status == 0 && flag == 1) {
    const bool status = lazy_load_config(game);
    if (!status) {
      SDL_Log("lazy_load_config\n");
      return false;
    }
  }
  return true;
}

static bool macro_settings_render(GAME* game) {
  if (game == NULL) {
    SDL_Log("game is NULL\n");
    return false;
  }

  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return false;
  }

  bool status = SDL_RenderClear(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    return false;
  }

  status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  int title_width, title_height;
  status =
    get_text_size(game->font, title, fonts.title_font, &title_width, &title_height);
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_title_menu,
    &(SDL_FPoint){
      .x = (resolution.width - (float)title_width) / 2,
      .y = screen_dimens.padding,
    }
  );
  if (!status) {
    SDL_Log("render_text error...\n");
    return false;
  }

  int text_width, text_height;
  status = get_text_size(
    game->font,
    "ht shift", // right indent for smth long like: right
                // shift, right ctrl, left ctrl...
    fonts.item_font, &text_width, &text_height
  );
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return false;
  }

  float y_pos = (float)title_height + screen_dimens.padding;
  const float text_x_pos = screen_dimens.padding;
  const float commands_x_pos = resolution.width - (float)text_width;
  for (int i = 0; i < 14; i++) {
    float commands_y_pos;

    int commands_selected_height;
    if (selected_index == i) {
      status = get_text_size(
        game->font, commands_keys[i], fonts.item_hover_font, &text_width,
        &commands_selected_height
      );
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return false;
      }
      commands_y_pos = y_pos + ((float)(text_height - commands_selected_height) / 2);
    } else {
      status =
        get_text_size(game->font, commands_keys[i], fonts.item_font, &text_width, NULL);
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return false;
      }
      commands_y_pos = y_pos;
    }

    SDL_Texture* item;

    if (selected_index == i) {
      item = tex_hover_items[i];
    } else {
      item = tex_items[i];
    }

    status = render_text(
      game->renderer, item,
      &(SDL_FPoint){
        .x = text_x_pos,
        .y = commands_y_pos,
      }
    );
    if (!status) {
      SDL_Log("render_text error...\n");
    }

    SDL_Texture* key;
    if (selected_index == i) {
      key = tex_hover_command_keys[i];
    } else {
      key = tex_command_keys[i];
    }

    status = render_text(
      game->renderer, key,
      &(SDL_FPoint){
        .x = commands_x_pos - ((float)text_width / 2),
        .y = commands_y_pos,
      }
    );
    if (!status) {
      SDL_Log("render_text error...\n");
    }

    y_pos += (float)text_height + (fonts.text_padding / 2);
  }

  if (event_status == 1) {
    status = render_press_key_popout(game);
    if (!status) {
      SDL_Log("render_press_key_popout error\n");
      return status;
    }
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  return true;
}

SCENE macro_setting_scene = {
  .handle_events = macro_settings_event_handler,
  .update = macro_settings_update,
  .render = macro_settings_render,
  .lazy_load = macro_setting_menu_lazy_load,
  .lazy_destroy = macro_settings_menu_lazy_destroy
};
