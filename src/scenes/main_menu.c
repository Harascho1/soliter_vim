#include "main_menu.h"
#include "../game.h"
#include "../my_timer.h"
#include "../render.h"
#include "../res/font.h"
#include "../res/sound.h"
#include "../res/res.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

static const char* title = "SoVIMter";
static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color title_color = {23, 150, 52, 255};
static SDL_Color green_color = {120, 255, 120, 255};

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
  int size = fonts.title_font;
  tex_game_title =
    get_texture_from_text(game->font, game->renderer, title, size, &title_color);
  if (tex_game_title == NULL) {
    SDL_Log("game_title cannot be initiazlied...");
    return 0;
  }
  size = fonts.item_font;
  for (int i = 0; i < 4; i++) {
    tex_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, size, &white_color
    );
    if (tex_menu_items[i] == NULL) {
      SDL_Log("menu_items[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  for (int i = 0; i < 2; i++) {
    tex_game_mode[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, size, &white_color
    );
    if (tex_game_mode[i] == NULL) {
      SDL_Log("tex_game_mode[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  size = fonts.item_hover_font;
  for (int i = 0; i < 4; i++) {
    tex_hover_menu_items[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, size, &green_color
    );
    if (tex_hover_menu_items[i] == NULL) {
      SDL_Log("hover_menu_items[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  for (int i = 0; i < 2; i++) {
    tex_hover_game_mode[i] = get_texture_from_text(
      game->font, game->renderer, game->main_menu->items[i].text, size, &green_color
    );
    if (tex_hover_game_mode[i] == NULL) {
      SDL_Log("tex_hover_game_mode[%d] cannot be initiazlied...", i);
      return 0;
    }
  }
  return 1;
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
        game->main_menu->selected_item = game->main_menu->count - 1;
      }
      break;
    case SDLK_DOWN:
    case SDLK_S:
      game->main_menu->selected_item++;
      if (game->main_menu->selected_item >= game->main_menu->count) {
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
  return 1;
}

bool main_menu_update(const GAME* game) {
  if (game->timer->start_timer == 1) {
    reset_timer(game->timer);
  }
  return 1;
}

bool main_menu_render(GAME* game) {
  int status;
  if (game == NULL) {
    SDL_Log("game is NULL\n");
    return 0;
  }

  if (game->renderer == NULL) {
    SDL_Log("game->renderer is NULL\n");
    return 0;
  }

  if (render_background(game) == false)
    return 0;

  if (render_logo(game) == false)
    return 0;

  int width = resolution.width;
  int height = resolution.height;
  int text_width, text_height;
  status =
    get_text_size(game->font, title, fonts.title_font, &text_width, &text_height);
  if (status == 0) {
    SDL_Log("get_text_size failed...\n");
    return 0;
  }

  status = render_text(
    game->renderer, tex_game_title,
    &(SDL_Point){.x = (width - text_width) / 2, .y = height / 4}
  );
  if (status == 0) {
    SDL_Log("render_text_ failed...\n");
    return 0;
  }

  status = get_text_size(
    game->font, game->main_menu->items[0].text, fonts.item_font, NULL, &text_height
  );
  if (status == 0) {
    SDL_Log("get_text_size failed...\n");
    return 0;
  }

  int y_coord = height / 2 + fonts.title_padding;
  for (int i = 0; i < game->main_menu->count; i++) {
    int font_size;
    int selected_height;
    int render_coord_y;
    if (i == game->main_menu->selected_item) {
      font_size = fonts.item_hover_font;
      status = get_text_size(
        game->font, game->main_menu->items[i].text, font_size, &text_width,
        &selected_height
      );
      if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
      }
      render_coord_y = y_coord + (text_height - selected_height) / 2;
    } else {
      font_size = fonts.item_font;
      status = get_text_size(
        game->font, game->main_menu->items[i].text, font_size, &text_width, &text_height
      );
      if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
      }
      render_coord_y = y_coord;
    }
    SDL_Point dst_rect = {.x = (width - text_width) / 2, .y = render_coord_y};
    SDL_Texture* tmp = NULL;
    if (game->main_menu->selected_item == i) {
      tmp = tex_hover_menu_items[i];
    } else {
      tmp = tex_menu_items[i];
    }
    status = render_text(game->renderer, tmp, &dst_rect);
    if (status == 0) {
      SDL_Log("render_text failed...\n");
      return 0;
    }

    y_coord += text_height + fonts.item_padding;
  }

  status = SDL_RenderPresent(game->renderer);
  if (status == 0) {
    SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
    push_user_event(SDL_EVENT_QUIT, 0);
    return 0;
  }

  return 1;
}

SCENE main_menu_scene = {
  .handle_events = main_menu_event_handler,
  .update = main_menu_update,
  .render = main_menu_render,
  .lazy_load = lazy_load_main_menu,
  .lazy_destroy = lazy_destroy_main_menu
};
