#include "main_menu.h"

#include "../game.h"
#include "../my_timer.h"
#include "../render.h"
#include "../res/font.h"
#include "../res/res.h"
#include "../res/sound.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "render_helper.h"

static SDL_Texture* tex_game_title;
static SDL_Texture* tex_menu_items[4];
static SDL_Texture* tex_hover_menu_items[4];
static SDL_Texture* tex_game_mode[2];
static SDL_Texture* tex_hover_game_mode[2];

void lazy_destroy_main_menu() {
  SDL_DestroyTexture(tex_game_title);
  for (int i = 0; i < 4; i++) {
    SDL_DestroyTexture(tex_menu_items[i]);
    SDL_DestroyTexture(tex_hover_menu_items[i]);
  }
  for (int i = 0; i < 2; i++) {
    SDL_DestroyTexture(tex_game_mode[i]);
    SDL_DestroyTexture(tex_hover_game_mode[i]);
  }
}

void action() {
  SDL_Log("Print");
}

bool lazy_load_main_menu(const GAME* game) {
  tex_game_title = get_texture_from_text(
    game->font, game->renderer, texts.titles.sovimter, fonts.title_font, &colors.title
  );
  if (tex_game_title == NULL) {
    SDL_Log("game_title cannot be initiazlied...");
    return false;
  }
  for (int i = 0; i < 4; i++) {
    tex_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, fonts.item_font,
      &colors.white
    );
    if (tex_menu_items[i] == NULL) {
      SDL_Log("menu_items[%d] cannot be initiazlied...", i);
      return false;
    }
  }
  for (int i = 0; i < 2; i++) {
    tex_game_mode[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, fonts.item_font,
      &colors.white
    );
    if (tex_game_mode[i] == NULL) {
      SDL_Log("tex_game_mode[%d] cannot be initiazlied...", i);
      return false;
    }
  }

  for (int i = 0; i < 4; i++) {
    tex_hover_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, fonts.item_hover_font,
      &colors.green
    );
    if (tex_hover_menu_items[i] == NULL) {
      SDL_Log("hover_menu_items[%d] cannot be initiazlied...", i);
      return false;
    }
  }
  for (int i = 0; i < 2; i++) {
    tex_hover_game_mode[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, fonts.item_hover_font,
      &colors.green
    );
    if (tex_hover_game_mode[i] == NULL) {
      SDL_Log("tex_hover_game_mode[%d] cannot be initiazlied...", i);
      return false;
    }
  }
  return true;
}

bool main_menu_event_handler(GAME* game, const SDL_Event* event) {
  if (event->type == SDL_EVENT_MOUSE_MOTION) {
  }
  if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    SDL_Log("x:%f, y:%f", event->motion.x, event->motion.y);
    if (event->motion.x <= 200 && event->motion.y <= 200) {
      SDL_Log("Radi");
    }
  }
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_UP:
    case SDLK_W:
      game->main_menu->selected_item--;
      if (game->main_menu->selected_item < 0) {
        game->main_menu->selected_item = texts.items.count_main_menu - 1;
      }
      break;
    case SDLK_DOWN:
    case SDLK_S:
      game->main_menu->selected_item++;
      if (game->main_menu->selected_item >= texts.items.count_main_menu) {
        game->main_menu->selected_item = 0;
      }
      break;
    case SDLK_RETURN:
    case SDLK_SPACE:
      play_sound(game->soundboard, 0);
      SDL_Delay(100);
      switch (game->main_menu->items[game->main_menu->selected_item].type) {
      case menu_item_type_play:
        run_a_game(game);
        push_user_event(g_change_scene_event_type, game_state_gameplay);
        break;
      case menu_item_type_settings:
        push_user_event(g_change_scene_event_type, game_state_setting);
        break;
      case menu_item_type_credits:
        push_user_event(g_change_scene_event_type, game_state_scores);
        break;
      case menu_item_type_exit:
        SDL_Event quit_event;
        quit_event.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit_event);
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

bool main_menu_update(const GAME* game) {
  if (game->timer->start_timer == 1) {
    reset_timer(game->timer);
  }
  return true;
}

bool main_menu_render(GAME* game) {
  if (game == NULL) {
    SDL_Log("game is NULL\n");
    return false;
  }

  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return false;
  }

  bool status = render_background(game);
  if (!status) {
    return false;
  }

  status = render_logo(game);
  if (!status) {
    return false;
  }

  status = render_text(
    game->renderer, tex_game_title,
    &(SDL_FPoint){
      .x = (resolution.width - fonts.title_width) / 2,
      .y = resolution.height / 4,
    }
  );
  if (!status) {
    SDL_Log("render_text_ failed...\n");
    return false;
  }

  status =
    render_menu(tex_menu_items, tex_hover_menu_items, game->renderer, game->main_menu);
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

SCENE main_menu_scene = {
  .handle_events = main_menu_event_handler,
  .update = main_menu_update,
  .render = main_menu_render,
  .lazy_load = lazy_load_main_menu,
  .lazy_destroy = lazy_destroy_main_menu
};
