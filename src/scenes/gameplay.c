#include "gameplay.h"
#include "../card.h"
#include "../cursor.h"
#include "../game.h"
#include "../my_timer.h"
#include "../res/res.h"
#include "../res/sound.h"
#include "SDL3/SDL_log.h"

char buffer[10] = "";

void change_cursor_frame(const GAME* game) {
  const CARD* card =
    find_card(game->deck, game->cursor->pos->col, game->cursor->pos->row);

  // * deck_card
  if (card == NULL && game->cursor->pos->row == 0) {
    game->cursor->cursor->x = game_dimens.padding_width - cursor_dimens.padding;
    game->cursor->cursor->y = game_dimens.padding_height - cursor_dimens.padding;
    return;
  }

  if (card == NULL) {
    SDL_Log("card is NULL and cursor pos isn't on first row\n");
    return;
  }

  // other scenarios
  if (same_card_selected(card, top_card(&game->deck->drawn_cards)) == 1) {
    const float right_indent = g_invisible_card[1].frame->x;
    game->cursor->cursor->x = right_indent - cursor_dimens.padding;
    game->cursor->cursor->y = game_dimens.padding_height - cursor_dimens.padding;
  } else {
    game->cursor->cursor->x = card->frame->x - cursor_dimens.padding;
    game->cursor->cursor->y = card->frame->y - cursor_dimens.padding;
  }
}

CARD* top_deck_card(DECK* deck) {
  if (deck->deck_card == NULL) {
    return NULL;
  }
  if (deck->count >= 52) {
    return NULL;
  }
  while (deck->count < 52) {
    if (deck->cards[deck->count].on_field == 0) {
      return &deck->cards[deck->count];
    }
    deck->count++;
  }
  return NULL;
}

CARD* next_deck_card(DECK* deck) {
  if (deck->count >= 52) {
    deck->count = -1;
    return NULL;
  }
  if (deck->deck_card == NULL) {
    pop_all(&deck->drawn_cards);
  }
  while (deck->count < 52) {
    if (deck->cards[deck->count].on_field == 0) {
      return &deck->cards[deck->count];
    }
    deck->count++;
  }
  return NULL;
}

CARD* draw_next_card(DECK* deck) {
  if (have_more_cards(deck) == 0) {
    return NULL;
  }
  CARD* next_new_card = top_deck_card(deck);

  if (next_new_card != NULL) {
    next_new_card->visible = visible;
    push(&deck->drawn_cards, next_new_card);
  }

  deck->count++;
  deck->deck_card = next_deck_card(deck);
  return next_new_card;
}

int reveal_card_below(const GAME* game) {
  for (int i = 1; i <= number_of_cards_in_row; i++) {
    int j = 1;
    CARD* card;
    int last_card_visible = 0;
    while ((card = find_card(game->deck, i, j)) != NULL) {
      last_card_visible = card->visible;
      j++;
    }
    j--;
    if (j == 0) {
      continue;
    }
    if (last_card_visible == 0) {
      card = find_card(game->deck, i, j);
      card->visible = visible;
    }
  }

  return 1;
}

int selected_card(DECK* deck, CARD** selected_cards) {
  int count = 0;
  for (int i = 0; i < 52; i++) {
    if (deck->cards[i].selected == selected) {
      selected_cards[count++] = &deck->cards[i];
    }
  }
  for (int i = 0; i < count - 1; i++) {
    for (int j = i + 1; j < count; j++) {
      if (selected_cards[j]->value > selected_cards[i]->value) {
        CARD* tmp = selected_cards[j];
        selected_cards[j] = selected_cards[i];
        selected_cards[i] = tmp;
      }
    }
  }
  return count;
}

bool place_king(CARD** card, const int num, const CURSOR* cursor) {
  const int cursor_col = cursor->pos->col;
  int cursor_row = cursor->pos->row;
  float x_coord, y_coord;

  if (cursor_row == 0) {
    SDL_Log("Can not");
    return false;
  }

  if ((*card)->value != 13) {
    return false;
  }

  for (int i = 0; i < number_of_cards_in_row; i++) {
    if (cursor_col == g_invisible_card[i].pos->col) {
      x_coord = g_invisible_card[i].frame->x;
      y_coord = g_invisible_card[i].frame->y;

      card[0]->pos->col = cursor_col;
      card[0]->pos->row = cursor_row;
      card[0]->frame->x = x_coord;
      card[0]->frame->y = y_coord;
      card[0]->on_field = 1;

      game_update = 1;
      break;
    }
  }

  for (int i = 1; i < num; i++) {
    card[i]->pos->col = cursor_col;
    card[i]->pos->row = cursor_row + 1;
    card[i]->frame->x = x_coord;
    card[i]->frame->y = y_coord + (float)card_dimens.height_padding;
    card[i]->on_field = 1;

    cursor_row++;
    y_coord += (float)card_dimens.height_padding;
  }

  return true;
}

bool place_a_card(const GAME* game) {
  const CARD* card =
    find_card(game->deck, game->cursor->pos->col, game->cursor->pos->row);

  CARD* s_card[14];
  const int num_of_selected_cards = selected_card(game->deck, s_card);
  // SDL_Log("Num of seleceted cards: %d", num_of_selected_cards);
  // SDL_Log("same cards %b", same_card_selected(card, *s_card));
  if (card != NULL) {
    SDL_Log("row: %d and col: %d\n", card->pos->row, card->pos->row);
  }

  if (num_of_selected_cards == 0) {
    return false;
  }

  if (card == NULL) {
    if (place_king(s_card, num_of_selected_cards, game->cursor) == false) {
      game_update = 0;
      deselect_all_cards(game->deck);
      set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
      return false;
    }
    if (same_card_selected(top_card(&game->deck->drawn_cards), *s_card) == 0) {
      game_update = 1;
    } else {
      pop(&game->deck->drawn_cards);
    }
    deselect_all_cards(game->deck);
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    return true;
  }

  // if (card->visible == not_visible) {
  //   deselect_all_cards(game->deck);
  //   set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
  //   return 0;
  // }

  // DRAW a new card
  if (same_card_selected(card, *s_card) == 1 && num_of_selected_cards == 1) {
    if (same_card_selected(card, game->deck->deck_card)) {
      play_sound(game->soundboard, draw_card_sound);
      const CARD* next_card = draw_next_card(game->deck);
      if (next_card == NULL) {
      }
      deselect_all_cards(game->deck);
      set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
      return true;
    }
    game_update = sort_a_card(*s_card, game->deck);
    if (game_update == 1) {
      play_sound(game->soundboard, sort_card_sound);
    }

    const CARD* top_card_ptr = top_card(&game->deck->drawn_cards);
    if (same_card_selected(*s_card, top_card_ptr) && game_update == 1) {
      pop(&game->deck->drawn_cards);
    }
    deselect_all_cards(game->deck);
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    return true;
  }

  if (same_card_selected(*s_card, game->deck->deck_card)) {
    deselect_all_cards(game->deck);
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    return false;
  }

  if (card->on_field == 0) {
    deselect_all_cards(game->deck);
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    return false;
  }

  if (can_card_be_placed(*s_card, card) == 0) {
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    deselect_all_cards(game->deck);
    return false;
  }

  if (find_card(game->deck, card->pos->col, card->pos->row + 1) != NULL) {
    SDL_Log("komso\n");
    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
    deselect_all_cards(game->deck);
    return false;
  }
  play_sound(game->soundboard, place_card_sound);
  const int col_of_cursor = game->cursor->pos->col;
  int row_of_cursor = game->cursor->pos->row;
  const float x_card_pos = card->frame->x;
  float y_card_pos = card->frame->y;

  for (int i = 0; i < num_of_selected_cards; i++) {
    s_card[i]->pos->col = col_of_cursor;
    s_card[i]->pos->row = row_of_cursor + 1;
    s_card[i]->frame->x = x_card_pos;
    s_card[i]->frame->y = y_card_pos + card_dimens.height_padding;
    s_card[i]->on_field = 1;

    row_of_cursor++;
    y_card_pos += card_dimens.height_padding;
  }

  set_a_flag(game->cursor, CURSOR_NORMAL_MODE);

  const CARD* new_card = top_card(&game->deck->drawn_cards);
  if (same_card_selected(*s_card, new_card)) {
    pop(&game->deck->drawn_cards);
    deselect_all_cards(game->deck);
    game_update = 0;
    return true;
  }

  deselect_all_cards(game->deck);
  game_update = 1;
  return true;
}

int select_a_card(GAME* game) {
  CARD* card;
  card = find_card(game->deck, game->cursor->pos->col, game->cursor->pos->row);
  if (card == NULL) {
    if (game->cursor->pos->row == 0 && game->cursor->pos->col == 1) {
      draw_next_card(game->deck);
      return 1;
    }
    play_sound(game->soundboard, blip_select_sound);
    return 0;
  }
  if (same_card_selected(card, game->deck->deck_card)) {
    if (have_more_cards(game->deck) == 0) {
      return 1;
    }
    card->selected = !card->selected;
    set_a_flag(game->cursor, CURSOR_SELECT_MODE);
    return 1;
  }
  if (same_card_selected(card, top_card(&game->deck->drawn_cards))) {
    card->selected = !card->selected;
    set_a_flag(game->cursor, CURSOR_SELECT_MODE);
    return 1;
  }
  if (card->visible == not_visible) {
    return 1;
  }
  card->selected = !card->selected;
  int selected_cards = select_card_below(card, game->deck);
  set_a_flag(game->cursor, CURSOR_SELECT_MODE);
  return 1;
}

int auto_sortable(const CARD* card, CARD** sorted_cards) {
  const int card_suit = card->suit;
  if (card->value == 1)
    return 1;

  if (card->value == 2) {
    const CARD* card_same_suit = NULL;
    for (int i = 0; i < 4; ++i) {
      if (sorted_cards[i] == NULL) {
        continue;
      }
      if (card_suit == sorted_cards[i]->suit) {
        card_same_suit = sorted_cards[i];
        break;
      }
    }
    if (card_same_suit == NULL) {
      return 0;
    }
    if (card_same_suit->value == 1) {
      return 1;
    }
  }

  // potential bug
  if (card->value >= 3 && card->value < 14) {
    CARD* card_other_suit[2];
    int other_suits = (card_suit + 2) % 4;
    if (card_suit % 2 == 1) {
      other_suits--;
    }
    SDL_Log("card suit is %d", card_suit);
    SDL_Log("other suit is %d", other_suits);
    int count = 0;
    for (int i = 0; i < 4; ++i) {
      if (sorted_cards[i] == NULL) {
        continue;
      }

      if ((other_suits == sorted_cards[i]->suit) ||
          (other_suits + 1 == sorted_cards[i]->suit)) {
        card_other_suit[count++] = sorted_cards[i];
      }
    }
    if (count != 2) {
      return 0;
    }
    SDL_Log("card is %d, and suit is %d\n\n\n", card->value, card->suit);
    for (int i = 0; i < count; ++i) {
      SDL_Log(
        "card is %d, and suit is %d\n\n\n", card_other_suit[i]->value,
        card_other_suit[i]->suit
      );
      if (card_other_suit[i]->value + 1 != card->value &&
          card->value > card_other_suit[i]->value) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

int auto_solve(const GAME* game) {
  if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
    return 1;
  }

  DECK* deck = game->deck;
  for (int i = 1; i <= number_of_cards_in_row; ++i) {
    int j = 1;
    CARD* card;
    do {
      card = find_card(deck, i, j);
      if (card == NULL) {
        break;
      }
      if (find_card(deck, i, j + 1)) {
        ++j;
        continue;
      }
      if (card->visible == visible) {
        if (auto_sortable(card, deck->sorted_cards)) {
          game_update |= sort_a_card(card, deck);
        }
      }
      ++j;
    } while (card != NULL);
  }
  if (game_update != 0) {
    play_sound(game->soundboard, sort_card_sound);
  }
  return 1;
}

int go_to_invisible_card(GAME* game, int col) {
  game->cursor->pos->col = col;
  game->cursor->cursor->x = g_invisible_card[col - 1].frame->x - cursor_dimens.padding;
  game->cursor->cursor->y = g_invisible_card[col - 1].frame->y - cursor_dimens.padding;
  return 1;
}

int interact(GAME* game) {
  if (game->cursor->mode % 2 == 0) {
    play_sound(game->soundboard, select_card_sound);
    select_a_card(game);
  } else if (game->cursor->mode % 2 == 1) {
    const int status = place_a_card(game);
    if (!status) {
      play_sound(game->soundboard, blip_select_sound);
    }
  }
  return 1;
}

bool gameplay_update(const GAME* game) {

  auto_solve(game);

  if (game_update == 1) {
    const int status = reveal_card_below(game);
    if (status != 0) {
      game_update = 0;
    }
  }

  if (have_number_hover(game->cursor) == 0) {
    buffer[0] = '\0';
  }

  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (game->deck->sorted_cards[i] == NULL) {
      break;
    }
    if (game->deck->sorted_cards[i]->value == 13) {
      count++;
    }
  }

  if (count == 4) {
    g_game_win = 1;
    stop_timer(game->timer);
  }

  return 1;
}

SCENE gameplay_scene = {
  .handle_events = gameplay_event_handler,
  .update = gameplay_update,
  .render = gameplay_render,
  .lazy_load = gameplay_lazy_load,
  .lazy_destroy = gameplay_lazy_destroy
};
