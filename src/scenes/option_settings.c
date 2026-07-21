#include "option_settings.h"
#include "../game.h"
#include "../res/config.h"
#include "../res/res.h"
#include "../res/texture.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_render.h"

static int selected_index = 0;
static char* text[14] = {"fullscreen", "sfx", "volume"};

bool lazy_load_option(const GAME* game) {
  for (int i = 0; i < 3; i++) {
    SDL_DestroyTexture(tex_opt->tex_options_set[i]);
    SDL_DestroyTexture(tex_opt->tex_hover_options_set[i]);
  }

  for (int i = 0; i < 3; i++) {
    int size = fonts.item_font;
    tex_opt->tex_options_set[i] = get_texture_from_text(
      game->font, game->renderer, options_set[i], size, &colors.white
    );
    if (tex_opt->tex_options_set[i] == NULL) {
      SDL_Log("items[%d]  cannot be initiazlied...", i);
      return false;
    }
    size = fonts.item_hover_font;
    tex_opt->tex_hover_options_set[i] = get_texture_from_text(
      game->font, game->renderer, options_set[i], size, &colors.green
    );
    if (tex_opt->tex_hover_options_set[i] == NULL) {
      SDL_Log("tex_command_keys[%d]  cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

bool option_setting_lazy_load(const GAME* game) {
  tex_opt = SDL_malloc(sizeof(TEX_OPTIONS_SETTINGS));

  int size = fonts.title_font;
  tex_opt->tex_title_menu =
    get_texture_from_text(game->font, game->renderer, texts.titles.option_settings, size, &colors.white);
  if (tex_opt->tex_title_menu == NULL) {
    SDL_Log("title_menu cannot be initiazlied...");
    return false;
  }

  size = fonts.item_font;
  for (int i = 0; i < 3; i++) {
    tex_opt->tex_items[i] =
      get_texture_from_text(game->font, game->renderer, text[i], size, &colors.white);
    if (tex_opt->tex_items[i] == NULL) {
      SDL_Log("tex_items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_opt->tex_options_set[i] = get_texture_from_text(
      game->font, game->renderer, options_set[i], size, &colors.white
    );
    if (tex_opt->tex_options_set[i] == NULL) {
      SDL_Log("tex_options_set[%d]  cannot be initiazlied...", i);
      return false;
    }
  }

  size = fonts.item_hover_font;
  for (int i = 0; i < 3; i++) {
    tex_opt->tex_hover_items[i] =
      get_texture_from_text(game->font, game->renderer, text[i], size, &colors.green);
    if (tex_opt->tex_hover_items[i] == NULL) {
      SDL_Log("hover_items[%d]  cannot be initiazlied...", i);
      return false;
    }
    tex_opt->tex_hover_options_set[i] = get_texture_from_text(
      game->font, game->renderer, options_set[i], size, &colors.green
    );
    if (tex_opt->tex_hover_options_set[i] == NULL) {
      SDL_Log("tex_hover_options_set[%d]  cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

void option_setting_lazy_destroy() {
  SDL_DestroyTexture(tex_opt->tex_title_menu);
  for (int i = 0; i < 3; i++) {
    SDL_DestroyTexture(tex_opt->tex_items[i]);
    SDL_DestroyTexture(tex_opt->tex_options_set[i]);
    SDL_DestroyTexture(tex_opt->tex_hover_items[i]);
    SDL_DestroyTexture(tex_opt->tex_hover_options_set[i]);
  }
  SDL_free(tex_opt);
}

static int event_status = 0;

bool option_settings_event_hendler(GAME* game, const SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_ESCAPE:
      update_option_file();
      reload_window(game);
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
      if (selected_index >= 2) {
        break;
      }
      selected_index++;
      break;
    case SDLK_LEFT:
    case SDLK_A:
      if (selected_index != 2) {
        break;
      }
      if (config_options[selected_index] <= 0) {
        break;
      }
      event_status = 1;
      insert_option(--config_options[selected_index], selected_index);
      play_sound(game->soundboard, 0);
      break;
    case SDLK_RIGHT:
    case SDLK_D:
      if (selected_index != 2) {
        break;
      }
      if (config_options[selected_index] >= 100) {
        break;
      }
      event_status = 1;
      insert_option(++config_options[selected_index], selected_index);
      play_sound(game->soundboard, 0);
      break;
    case SDLK_SPACE:
    case SDLK_RETURN:
      play_sound(game->soundboard, 0);
      SDL_Delay(100);
      if (selected_index == 2) {
        break;
      }
      event_status = 1;
      insert_option(!config_options[selected_index], selected_index);
      break;
    default:
      break;
    }
  }
  return true;
}

bool option_settings_update(const GAME* game) {
  if (event_status == 1) {
    const bool status = lazy_load_option(game);
    if (!status) {
      SDL_Log("lazy_load_config");
      return false;
    }
  }
  return true;
}

bool option_settings_render(GAME* game) {
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

  status = render_text(
    game->renderer, tex_opt->tex_title_menu,
    &(SDL_FPoint){
      .x = (resolution.width - fonts.title_width) / 2.0F,
      .y = screen_dimens.padding,
    }
  );
  if (!status) {
    SDL_Log("render_text error...\n");
    return false;
  }

  int text_width, text_height;
  status = get_text_size(game->font, "shift", fonts.item_font, &text_width, &text_height);
  if (!status) {
    SDL_Log("get_text_size error...\n");
    return false;
  }

  float y_pos = fonts.title_height + screen_dimens.padding;
  const float text_x_pos = screen_dimens.padding;
  const float commands_x_pos = resolution.width - (float)text_width;
  for (int i = 0; i < 3; i++) {
    int commands_y_pos;

    int commands_selected_height;
    if (selected_index == i) {
      status = get_text_size(
        game->font, options_set[i], fonts.item_hover_font, &text_width,
        &commands_selected_height
      );
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return false;
      }
      commands_y_pos = y_pos + (text_height - commands_selected_height) / 2;
    } else {
      status =
        get_text_size(game->font, options_set[i], fonts.item_font, &text_width, NULL);
      if (!status) {
        SDL_Log("get_text_size error...\n");
        return false;
      }
      commands_y_pos = y_pos;
    }

    SDL_Texture* item;
    if (selected_index == i) {
      item = tex_opt->tex_hover_items[i];
    } else {
      item = tex_opt->tex_items[i];
    }

    status = render_text(
      game->renderer, item, &(SDL_FPoint){.x = text_x_pos, .y = commands_y_pos}
    );
    if (!status) {
      SDL_Log("render_text error...\n");
    }

    SDL_Texture* opt;
    if (selected_index == i) {
      opt = tex_opt->tex_hover_options_set[i];
    } else {
      opt = tex_opt->tex_options_set[i];
    }

    status = render_text(
      game->renderer, opt,
      &(SDL_FPoint){.x = commands_x_pos - text_width / 2, .y = commands_y_pos}
    );
    if (!status) {
      SDL_Log("render_text error...\n");
    }

    y_pos += text_height + fonts.item_padding;
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  return true;
}

SCENE option_setting_scene = {
  .handle_events = option_settings_event_hendler,
  .update = option_settings_update,
  .render = option_settings_render,
  .lazy_load = option_setting_lazy_load,
  .lazy_destroy = option_setting_lazy_destroy
};
