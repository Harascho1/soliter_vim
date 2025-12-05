#include "card.h"
#include "SDL3/SDL_log.h"
#include "texture.h"

int same_card_selected(CARD *card1, CARD *card2) { return card1 == card2; }

void deselect_all_cards(DECK *deck) {
  for (int i = 0; i < 52; i++) {
    deck->cards[i].selected = 0;
  }
}

CARD *find_card(DECK *deck, int col, int row) {
  if (col == 1 && row == 0) {
    return deck->deck_card;
  }

  if (col == 2 && row == 0) {
    return view_top_card_in_queue(deck->new_cards);
  }

  for (int i = 0; i < 52; i++) {
    if (deck->cards[i].pos->col == col && deck->cards[i].pos->row == row) {
      return &deck->cards[i];
    }
  }
  return NULL;
}

int select_card_below(CARD *card, DECK *deck) {
  if (card == NULL) {
    return 0;
  }
  if (card->visible == not_visible) {
    return 0;
  }
  CARD *tmp_card = find_card(deck, card->pos->col, card->pos->row + 1);
  if (tmp_card == NULL) {
    return 1;
  }
  tmp_card->selected = 1;
  return 1 + select_card_below(tmp_card, deck);
}

void shuffle_deck(DECK *deck) {
  if (deck == NULL) {
    SDL_Log("Deck is NULL and therefore cannot be shuffled");
    return;
  }

  srand((unsigned int)time(NULL));
  int j;
  int count = 0;
  for (int i = deck->count - 1; i >= 0; i--) {
    j = rand() % (i + 1);
    CARD temp = deck->cards[i];
    deck->cards[i] = deck->cards[j];
    deck->cards[j] = temp;
    count++;
  }
}

int sort_a_card(CARD *card, DECK *deck) {
  for (int suit = 0; suit < 4; suit++) {
    if (deck->sorted_cards[suit] == NULL && card->value == 1) {
      card->on_field = 1;
      card->pos->col = suit + 4;
      card->pos->row = 0;
      deck->sorted_cards[suit] = card;
      SDL_Log("sortirao\n");
      return 1;
    }
    if (deck->sorted_cards[suit] == NULL) {
      return 0;
    }
    if (card->suit == deck->sorted_cards[suit]->suit) {
      // SDL_Log("sortirao\n");
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

int pop_all(CARD_QUEUE *queue) {
  queue->p = 0;
  queue->q = 0;
  for (int i = 0; i < queue->max_items; i++) {
    if (queue->queue[i] == NULL) {
      continue;
    }
    if (queue->queue[i]->on_field == 0) {
      queue->queue[i]->visible = not_visible;
    }
    queue->queue[i] = NULL;
  }
  queue->count = 0;
  return 1;
}

int is_queue_empty(CARD_QUEUE *queue) { return queue->count == 0; }

int is_queue_full(CARD_QUEUE *queue) {
  return queue->count == queue->max_items;
}

int push(CARD_QUEUE *queue, CARD *card) {
  if (is_queue_full(queue)) {
    pop(queue);
  }

  queue->queue[queue->p++] = card;
  card->visible = visible;

  if (queue->p == queue->max_items) {
    queue->p = 0;
  }

  queue->count++;
  return 1;
}

CARD *pop(CARD_QUEUE *queue) {
  if (is_queue_empty(queue)) {
    return NULL;
  }
  int old_q = queue->q;
  queue->q++;
  if (queue->q == queue->max_items) {
    queue->q = 0;
  }
  queue->count--;
  queue->queue[old_q]->visible = not_visible;
  return queue->queue[old_q];
}

CARD *view_top_card_in_queue(CARD_QUEUE *queue) {
  if (queue->count == 0) {
    return NULL;
  }
  int tmp;
  if (queue->p == 0) {
    tmp = queue->max_items - 1;
  } else {
    tmp = queue->p - 1;
  }
  return queue->queue[tmp];
}

CARD *pop_top(CARD_QUEUE *queue) {
  if (queue->count == 0) {
    return NULL;
  }
  queue->count--;
  if (queue->p == 0) {
    queue->p = queue->max_items - 1;
  } else {
    queue->p--;
  }
  queue->queue[queue->p] = NULL;
  return NULL;
}

int can_card_be_placed(CARD *card_below, CARD *card_above) {
  if (card_below->value + 1 != card_above->value) {
    return 0;
  }
  if ((card_below->suit == suit_clubs || card_below->suit == suit_spades) &&
      (card_above->suit == suit_diamonds || card_above->suit == suit_hearts)) {
    return 1;
  } else if ((card_below->suit == suit_diamonds ||
              card_below->suit == suit_hearts) &&
             (card_above->suit == suit_clubs ||
              card_above->suit == suit_spades)) {
    return 1;
  }
  return 0;
}

CARD_QUEUE *create_card_queue(int max_items) {
  CARD_QUEUE *queue = SDL_malloc(sizeof(CARD_QUEUE));
  queue->max_items = max_items;
  queue->p = 0;
  queue->q = 0;
  queue->count = 0;
  queue->queue = (CARD **)SDL_malloc(sizeof(CARD *) * max_items);
  return queue;
}

DECK *create_deck(SDL_Renderer *renderer) {
  DECK *deck = NULL;
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
  deck->new_cards = create_card_queue(3);
  deck->deck_card = &deck->cards[deck->count];
  int status;

  deck->empty_sorted_card = create_texture_from_image(
      renderer, "assets/cards/total_blank_front_white.png");
  if (deck->empty_sorted_card == NULL) {
    SDL_Log("create_texture_from_image error\n");
    return NULL;
  }
  return deck;
}

void destroy_card(CARD *card) {
  if (card != NULL) {
    SDL_free(card->frame);
    SDL_free(card->pos);
  }
}

int have_more_cards(DECK *deck) {
  if (deck->deck_card != NULL) {
    return 1;
  }
  int count = 0;
  for (int i = 28; i < 52; i++) {
    if (deck->cards[i].on_field == 0) {
      count++;
    }
  }
  if (count > 1) {
    return 1;
  }
  return 0;
}

void destroy_deck(DECK *deck) {
  if (deck != NULL) {
    SDL_DestroyTexture(deck->empty_sorted_card);
    for (int i = 0; i < 52; i++) {
      destroy_card(&deck->cards[i]);
    }
    SDL_free(deck->new_cards);
    SDL_free(deck);
  }
}

char *find_path(CARD *card) {
  char *path;
  path = SDL_malloc(sizeof(char) * 100);

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

int render_card(FIELD *field, SDL_Renderer *renderer, CARD *card,
                SDL_FPoint *point) {
  if (renderer == NULL || card == NULL) {
    SDL_Log("renderer or card are NULL in render_card fun...\n");
    return 0;
  }

  char *path;
  path = find_path(card);
  if (path == NULL) {
    SDL_Log("path is NULL in render_card fun...\n");
    return 0;
  }

  SDL_Texture *texture = create_texture_from_image(renderer, path);
  if (texture == NULL) {
    SDL_Log("texture is NULL in render_card fun...\n");
    return 0;
  }

  int status = SDL_RenderTexture(
      renderer, texture, NULL,
      &(SDL_FRect){point->x, point->y, field->card_width, field->card_height});

  if (status == 0) {
    SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
    return status;
  }

  SDL_DestroyTexture(texture);
  SDL_free(path);
  return status;
}
