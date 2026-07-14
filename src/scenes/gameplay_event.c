#include "../cursor.h"
#include "../game.h"
#include "../res/config.h"
#include "../res/sound.h"
#include "../res/textbox.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "gameplay.h"

int normal_select_mode(GAME* game, const SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_ESCAPE:
      push_user_event(g_change_scene_event_type, game_state_game_over);
      break;
    case SDLK_C:
      deselect_all_cards(game->deck);
      game->cursor->mode = CURSOR_FLY_MODE;
      break;
    case SDLK_D:
    case SDLK_RIGHT:
      play_sound(game->soundboard, move_sound);
      if (game->cursor->pos->col == 7) {
        break;
      }
      if (game->cursor->pos->row == 0 && game->cursor->pos->col == 2) {
        break;
      }
      int new_col = game->cursor->pos->col + 1;
      if (find_card(game->deck, new_col, game->cursor->pos->row) == 0) {
        if (game->cursor->pos->row == 1) {
          go_to_invisible_card(game, new_col);
          break;
        }
        break;
      }
      game->cursor->pos->col++;
      change_cursor_frame(game);
      break;
    case SDLK_A:
    case SDLK_LEFT:
      play_sound(game->soundboard, move_sound);
      if (game->cursor->pos->col == 1) {
        break;
      }
      new_col = game->cursor->pos->col - 1;
      if (find_card(game->deck, new_col, game->cursor->pos->row) == 0) {
        if (game->cursor->pos->row == 1) {
          go_to_invisible_card(game, new_col);
          break;
        }
        if (game->cursor->pos->row != 0) {
          break;
        }
      }
      game->cursor->pos->col--;
      change_cursor_frame(game);
      break;
    case SDLK_S:
    case SDLK_DOWN:
      play_sound(game->soundboard, move_sound);
      int new_row = game->cursor->pos->row + 1;
      if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
        if (game->cursor->pos->row == 0) {
          go_to_invisible_card(game, game->cursor->pos->col);
          game->cursor->pos->row = new_row;
        }
        break;
      }
      game->cursor->pos->row++;
      change_cursor_frame(game);
      break;
    case SDLK_W:
    case SDLK_UP:
      play_sound(game->soundboard, move_sound);
      if (game->cursor->pos->row == 0) {
        break;
      }
      if (game->cursor->pos->row == 1) {
        if (game->cursor->pos->col != 1 && game->cursor->pos->col != 2) {
          break;
        }
      }
      new_row = game->cursor->pos->row - 1;
      if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
        if (game->cursor->pos->col != 1) {
          break;
        }
      }
      game->cursor->pos->row--;
      change_cursor_frame(game);
      break;
    case SDLK_RETURN:
    case SDLK_SPACE:
      interact(game);
      break;
    default:
      break;
    }
  }
  return 1;
}

int convert_controls(unsigned int key) {
  int i = 0;
  while (i < 14) {
    if (key == config_commands[i]) {
      return i;
    }
    i++;
  }
  return -1;
}

int fly_mode(GAME* game, const SDL_Event* event) {
  int status;
  if (event->type == SDL_EVENT_KEY_DOWN) {
    int tmp;
    const int key = convert_controls(event->key.key);
    switch (key) {
    case 0:
      tmp = 10;
      if ((status = have_number_hover(game->cursor)) == 0) {
        break;
      }
      if (have_a_flag(game->cursor, CURSOR_HOVER_10) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_10);
        break;
      }
      if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;

    case 1:
      tmp = 1;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_1);
        break;
      }
      if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      } else {
        if (tmp > 10) {

          break;
        }
        game->cursor->pos->row = tmp;
        go_to_invisible_card(game, status);
      }
      delete_hover_flag(game->cursor);
      break;
    case 2:
      tmp = 2;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_2);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 3:
      tmp = 3;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_3);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 4:
      tmp = 4;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_4);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 5:
      tmp = 5;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_5);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 6:
      tmp = 6;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_6);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 7:
      tmp = 7;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        set_a_flag(game->cursor, CURSOR_HOVER_7);
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 8:
      tmp = 8;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 9:
      tmp = 9;
      if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
        tmp = tmp + 10;
      }
      if ((status = have_number_hover(game->cursor)) == 0) {
        break;
      } else if (find_card(game->deck, status, tmp) != NULL) {
        game->cursor->pos->col = status;
        game->cursor->pos->row = tmp;
        delete_hover_flag(game->cursor);
        change_cursor_frame(game);
        play_sound(game->soundboard, move_sound);
      }
      delete_hover_flag(game->cursor);
      break;
    case 10:
      deselect_all_cards(game->deck);
      game->cursor->mode = CURSOR_NORMAL_MODE;
      break;
    case 11:
      deselect_all_cards(game->deck);
      status = have_number_hover(game->cursor);
      if (status != 0) {
        break;
      }
      game->cursor->pos->row = 0;
      game->cursor->pos->col = 2;
      change_cursor_frame(game);
      break;
    case 12:
      play_sound(game->soundboard, draw_card_sound);
      CARD* card = draw_next_card(game->deck);
      if (card == NULL) {
        SDL_Log("RECI MI\n");
      }
      deselect_all_cards(game->deck);
      set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
      break;
    case 13:
      status = have_number_hover(game->cursor);
      if (status != 0) {
        break;
      }
      interact(game);
      buffer[0] = '0';
      break;
    default:
      return 0;
      break;
    }
    if (key < 0 || key > 9) {
      return 1;
    }
    if (buffer[0] == '\0') {
      sprintf(buffer, "%d", key);
    } else {
      char tmp[10];
      sprintf(tmp, "%d", key);
      strcpy(buffer, tmp);
    }
  }
  return 1;
}

bool text_mode(const GAME* game, const SDL_Event* event) {
  bool status;
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_RETURN:
      if (!have_requaried_num_of_characters(textbox, 4)) {
        break;
      }
      save_score(game, textbox->string);
      push_user_event(g_change_scene_event_type, game_state_game_over);
      break;
    case SDLK_BACKSPACE:
      status = delete_text(textbox);
      if (!status) {
        SDL_Log("delete_text error...\n");
        return false;
      }
      break;
    default:
      break;
    }
  } else if (event->type == SDL_EVENT_TEXT_INPUT) {
    if (textbox == NULL) {
      SDL_Log("textbox je NULL\n");
      return false;
    }
    status = insert_text(textbox, event->text.text);
    if (!status) {
      SDL_Log("insert_text error...\n");
      return false;
    }
    SDL_Log("textbox value: %s", textbox->string);
  }
  return true;
}

bool gameplay_event_handler(GAME* game, const SDL_Event* event) {
  if (g_game_win == 1) {
    SDL_StartTextInput(game->window);
    text_mode(game, event);
  } else if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
    fly_mode(game, event);
  } else if (have_a_flag(game->cursor, CURSOR_SELECT_MODE)) {
    normal_select_mode(game, event);
  } else {
    normal_select_mode(game, event);
  }
  return 1;
}
