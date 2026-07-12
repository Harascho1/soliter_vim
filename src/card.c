#include "card.h"
#include "SDL3/SDL_log.h"
#include "res/texture.h"
#include "scenes/gameplay.h"
#include <string.h>

bool same_card_selected(const CARD* card1, const CARD* card2) {
  return card1 == card2;
}

void deselect_all_cards(DECK* deck) {
  for (int i = 0; i < 52; i++) {
    deck->cards[i].selected = 0;
  }
}

CARD* find_card(const DECK* deck, int col, int row) {
  if (col == 1 && row == 0) {
    // NOTE: Deck card can be NULL.
    return deck->deck_card;
  }

  if (col == 2 && row == 0) {
    // NOTE: This card can be NULL
    return top_card(&deck->drawn_cards);
  }

  for (int i = 0; i < 52; i++) {
    if (deck->cards[i].pos->col == col && deck->cards[i].pos->row == row) {
      // NOTE: This card can be null
      // TODO: need to make function that explicitly return `CARD*` from stack
      return (CARD*)&deck->cards[i];
    }
  }
  return NULL;
}

int select_card_below(const CARD* card, const DECK* deck) {
  if (card == NULL) {
    return 0;
  }
  if (card->visible == not_visible) {
    return 0;
  }
  CARD* tmp_card = find_card(deck, card->pos->col, card->pos->row + 1);
  if (tmp_card == NULL) {
    return 1;
  }
  tmp_card->selected = 1;
  return 1 + select_card_below(tmp_card, deck);
}

void shuffle_deck(DECK* deck) {
  if (deck == NULL) {
    SDL_Log("Deck is NULL and therefore cannot be shuffled");
    return;
  }

  srand((unsigned int)time(NULL));
  int count = 0;
  for (int i = deck->count - 1; i >= 0; i--) {
    int j = rand() % (i + 1);
    const CARD temp = deck->cards[i];
    deck->cards[i] = deck->cards[j];
    deck->cards[j] = temp;
    count++;
  }
}

int sort_a_card(CARD* card, DECK* deck) {
  for (int suit = 0; suit < 4; suit++) {
    if (deck->sorted_cards[suit] == NULL && card->value == 1) {
      card->on_field = 1;
      card->pos->col = suit + 4;
      card->pos->row = 0;
      deck->sorted_cards[suit] = card;
      SDL_Log("Sorted\n");
      return 1;
    }
    if (deck->sorted_cards[suit] == NULL) {
      return 0;
    }
    if (card->suit == deck->sorted_cards[suit]->suit) {
      if (deck->sorted_cards[suit]->value + 1 != card->value) {
        return 0;
      }
      card->on_field = 1;
      card->pos->col = 0;
      card->pos->row = 0;
      deck->sorted_cards[suit] = card;
      return 1;
    }
  }
  return 0;
}

void pop_all(CARD_STACK* stack) {
  for (int i = 0; i < stack->count; i++) {
    // NOTE: return cards to the deck face down
    stack->array[i]->visible = not_visible;
  }
  memset(stack->array, 0, STACK_SIZE);
  stack->count = 0;
}

bool is_empty(const CARD_STACK* stack) {
  return stack->count == 0;
}

bool is_full(const CARD_STACK* stack) {
  return stack->count == STACK_SIZE;
}

bool push(CARD_STACK* stack, CARD* card) {
  if (is_full(stack)) {
    return false;
  }
  stack->array[stack->count++] = card;
  card->visible = visible;
  return true;
}

CARD* pop(CARD_STACK* stack) {
  if (is_empty(stack)) {
    return NULL;
  }
  return stack->array[--stack->count];
}

CARD* top_card(const CARD_STACK* stack) {
  if (stack == NULL) {
    return NULL;
  }
  if (is_empty(stack)) {
    return NULL;
  }
  return stack->array[stack->count - 1];
}

bool can_card_be_placed(const CARD* card_below, const CARD* card_above) {
  if (card_below->value + 1 != card_above->value) {
    return false;
  }
  if ((card_below->suit == suit_clubs || card_below->suit == suit_spades) &&
      (card_above->suit == suit_diamonds || card_above->suit == suit_hearts)) {
    return true;
  }
  if ((card_below->suit == suit_diamonds || card_below->suit == suit_hearts) &&
      (card_above->suit == suit_clubs || card_above->suit == suit_spades)) {
    return true;
  }
  return true;
}

void create_card_stack(CARD_STACK* stack) {
  stack->count = 0;
}

DECK* create_deck(SDL_Renderer* renderer) {
  DECK* deck = NULL;
  deck = SDL_malloc(sizeof(DECK));
  if (deck == NULL) {
    return deck;
  }
  deck->count = 52;
  int i = 0;
  for (int suit = suit_clubs; suit <= suit_hearts; suit++) {
    for (int value = value_ace; value <= value_king; value++) {
      deck->cards[i].suit = suit;
      deck->cards[i].value = value;
      deck->cards[i].visible = not_visible;
      deck->cards[i].selected = 0;
      deck->cards[i].on_field = 0;
      deck->cards[i].frame = SDL_malloc(sizeof(SDL_FPoint));
      deck->cards[i].pos = SDL_malloc(sizeof(POSITION));
      i++;
    }
  }
  shuffle_deck(deck);
  deck->count = 28;
  create_card_stack(&deck->drawn_cards);
  deck->deck_card = &deck->cards[deck->count];

  deck->empty_sorted_card =
    create_texture_from_image(renderer, "assets/cards/total_blank_front_white.png");
  if (deck->empty_sorted_card == NULL) {
    SDL_Log("create_texture_from_image error\n");
    return NULL;
  }

  return deck;
}

void destroy_card(const CARD* card) {
  if (card != NULL) {
    SDL_free(card->frame);
    SDL_free(card->pos);
  }
}

bool have_more_cards(const DECK* deck) {
  if (deck->deck_card != NULL) {
    return true;
  }
  int count = 0;
  for (int i = 28; i < 52; i++) {
    if (deck->cards[i].on_field == 0) {
      count++;
    }
  }
  if (count > 1) {
    return true;
  }
  return true;
}

void destroy_deck(DECK* deck) {
  if (deck != NULL) {
    SDL_DestroyTexture(deck->empty_sorted_card);
    for (int i = 0; i < 52; i++) {
      destroy_card(&deck->cards[i]);
    }
    SDL_free(deck);
  }
}

char* find_path(const CARD* card) {
  char* path = SDL_malloc(sizeof(char) * 100);

  if (card->visible == not_visible) {
    if (card->selected == selected) {
      sprintf(path, "assets/cards/back_red_basic.png");
      return path;
    }
    sprintf(path, "assets/cards/back_red_basic_white.png");
    return path;
  }

  char suit[10];
  switch (card->suit) {
  case suit_clubs:
    strncpy(suit, "clubs", 6);
    break;
  case suit_spades:
    strncpy(suit, "spades", 7);
    break;
  case suit_diamonds:
    strncpy(suit, "diamonds", 9);
    break;
  case suit_hearts:
    strncpy(suit, "hearts", 7);
    break;
  default:
    SDL_Log("Unknown suit");
    return NULL;
  }

  char value[6];
  switch (card->value) {
  case 1:
    strncpy(value, "ace", 4);
    break;
  case 11:
    strncpy(value, "jack", 5);
    break;
  case 12:
    strncpy(value, "queen", 6);
    break;
  case 13:
    strncpy(value, "king", 5);
    break;
  default:
    SDL_itoa(card->value, value, 10);
    break;
  }

  if (card->selected == 1) {
    sprintf(path, "assets/cards/%s_%s.png", value, suit);
    return path;
  }

  sprintf(path, "assets/cards/%s_%s_white.png", value, suit);
  return path;
}

bool render_card(
  const FIELD* field, SDL_Renderer* renderer, const CARD* card, const SDL_FPoint* point
) {
  if (renderer == NULL || card == NULL) {
    SDL_Log("renderer or card are NULL in render_card fun...\n");
    return 0;
  }

  char* path = find_path(card);
  if (path == NULL) {
    SDL_Log("path is NULL in render_card fun...\n");
    return 0;
  }

  SDL_Texture* texture = create_texture_from_image(renderer, path);
  if (texture == NULL) {
    SDL_Log("texture is NULL in render_card fun...\n");
    return 0;
  }

  const int status = SDL_RenderTexture(
    renderer, texture, NULL,
    &(SDL_FRect){point->x, point->y, (float)field->card_width, (float)field->card_height}
  );

  if (status == 0) {
    SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
    return status;
  }

  SDL_DestroyTexture(texture);
  SDL_free(path);
  return status;
}
