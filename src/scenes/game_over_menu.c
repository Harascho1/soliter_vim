#include "game_over_menu.h"

#include "../game.h"
#include "../my_timer.h"
#include "../render.h"
#include "../res/res.h"
#include "render_helper.h"

static const char* title[2] = {"GAME OVER", "YOU W0N"};

static char win_in_seconds[255];

static SDL_Texture* tex_game_result[2];
static SDL_Texture* tex_time;
static SDL_Texture* tex_game_over_items[3];
static SDL_Texture* tex_hover_game_over_items[3];

bool game_over_menu_lazy_load(const GAME* game) {

  if (game->timer->start_timer == 1) {
    if (g_game_win == 1) {
      sprintf(win_in_seconds, "time: %d", game->timer->time_elapsed);
    } else {
      win_in_seconds[0] = '\0';
    }
    reset_timer(game->timer);
  }

  int font_size = fonts.text_font;
  char buff[10];
  sprintf(buff, "time: %d", game->timer->time_elapsed);
  tex_time =
    get_texture_from_text(game->font, game->renderer, buff, font_size, &colors.white);
  if (tex_time == NULL) {
    SDL_Log("tex_time cannot be initialized...");
    return false;
  }

  font_size = fonts.title_font;
  for (int i = 0; i < 2; i++) {
    tex_game_result[i] = get_texture_from_text(
      game->font, game->renderer, title[i], font_size, &colors.title
    );
    if (tex_game_result[i] == NULL) {
      SDL_Log("tex_game_result[%d]  cannot be initialized...", i);
      return false;
    }
  }

  font_size = fonts.item_font;
  for (int i = 0; i < 3; i++) {
    tex_game_over_items[i] = get_texture_from_text(
      game->font, game->renderer, game->game_over_menu->items[i].text, font_size,
      &colors.white
    );
    if (tex_game_over_items[i] == NULL) {
      SDL_Log("tex_game_over_items[%d]  cannot be initialized...", i);
      return false;
    }
  }

  font_size = fonts.item_hover_font;
  for (int i = 0; i < 3; i++) {
    tex_hover_game_over_items[i] = get_texture_from_text(
      game->font, game->renderer, game->game_over_menu->items[i].text, font_size,
      &colors.green
    );
    if (tex_hover_game_over_items[i] == NULL) {
      SDL_Log("tex_hover_game_over_items[%d] cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

void game_over_menu_lazy_destroy() {
  SDL_DestroyTexture(tex_time);

  for (int i = 0; i < 2; i++) {
    SDL_DestroyTexture(tex_game_result[i]);
  }

  for (int i = 0; i < 3; i++) {
    SDL_DestroyTexture(tex_game_over_items[i]);
    SDL_DestroyTexture(tex_hover_game_over_items[i]);
  }
}

bool game_over_menu_event_handler(GAME* game, const SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_UP:
    case SDLK_W:
      if (game->game_over_menu->selected_item != 0) {
        game->game_over_menu->selected_item--;
      }
      break;
    case SDLK_DOWN:
    case SDLK_S:
      if (game->game_over_menu->selected_item != game_over_item_type_max - 1) {
        game->game_over_menu->selected_item++;
      }
      break;
    case SDLK_RETURN:
    case SDLK_SPACE:
      play_sound(game->soundboard, 0);
      SDL_Delay(150);
      switch (game->game_over_menu->selected_item) {
      case game_over_item_type_restart:
        run_a_game(game);
        push_user_event(g_change_scene_event_type, game_state_gameplay);
        break;
      case game_over_item_credits:
        push_user_event(g_change_scene_event_type, game_state_scores);
        break;
      case game_over_item_type_exit:
        push_user_event(g_change_scene_event_type, game_state_main_menu);
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
  return true;
}

bool game_over_menu_update(const GAME* game) {
  return true;
}

bool render_time(const GAME* game) {
  if (strlen(win_in_seconds) <= 0) {
    // SDL_Log("win_in_seconds buffer is empty\n");
    return true;
  }
  int text_width, text_height;

  bool status =
    get_text_size(game->font, win_in_seconds, fonts.text_font, &text_width, &text_height);
  if (!status) {
    SDL_Log("get_text_size error ...\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_time,
    &(SDL_FPoint){
      .x = (resolution.width - (float)text_width) / 2,
      .y = fonts.title_padding,
    }
  );
  if (!status) {
    SDL_Log("render_text error ...\n");
    return false;
  }
  return true;
}

bool game_over_menu_render(GAME* game) {
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
    printf("ZASTOOOOO\n");
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  render_logo(game);

  const SDL_FRect* rect = &(SDL_FRect){
    .x = 0,
    .y = 0,
    .w = resolution.height,
    .h = resolution.height,
  };

  status = SDL_RenderTexture(game->renderer, game->menu_texture, NULL, rect);
  if (!status) {
    SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  int text_width, text_height;
  status = render_time(game);
  if (!status) {
    SDL_Log("render_time failed: %s\n", SDL_GetError());
    return false;
  }

  status = get_text_size(
    game->font, title[g_game_win], fonts.title_font, &text_width, &text_height
  );
  if (!status) {
    SDL_Log("get_text_size failed...\n");
    return false;
  }

  status = render_text(
    game->renderer, tex_game_result[g_game_win],
    &(SDL_FPoint){.x = (resolution.width - (float)text_width) / 2, .y = resolution.height / 4}
  );
  if (!status) {
    SDL_Log("render_text failed...\n");
    return false;
  }

  status = get_text_size(
    game->font, game->game_over_menu->items[0].text, fonts.item_font, NULL, &text_height
  );
  if (!status) {
    SDL_Log("get_text_size failed...\n");
    return false;
  }

  status =
    render_menu(tex_game_over_items, tex_hover_game_over_items, game->renderer, game->game_over_menu);
  if (!status) {
    return false;
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return false;
  }

  return true;
}

SCENE game_over_menu_scene = {
  .handle_events = game_over_menu_event_handler,
  .update = game_over_menu_update,
  .render = game_over_menu_render,
  .lazy_load = game_over_menu_lazy_load,
  .lazy_destroy = game_over_menu_lazy_destroy
};
