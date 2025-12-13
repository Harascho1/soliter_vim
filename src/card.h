#ifndef CARD_H
#define CARD_H

#include "field.h"

#define STACK_SIZE 30

enum { not_visible = 0, visible };

enum { not_selected = 0, selected };

typedef enum _SUIT { suit_clubs, suit_spades, suit_diamonds, suit_hearts } SUIT;

typedef enum _VALUE {
  value_ace = 1,
  value_2,
  value_3,
  value_4,
  value_5,
  value_6,
  value_7,
  value_8,
  value_9,
  value_10,
  value_jack,
  value_queen,
  value_king
} VALUE;

typedef struct _POSITION {
  int row;
  int col;
} POSITION;

typedef struct _CARD {
  VALUE value;
  SUIT suit;
  int visible;
  int selected;
  SDL_FPoint *frame;
  POSITION *pos;
  int on_field;
} CARD;

typedef struct _CARD_STACK {
  int count;
  CARD *array[STACK_SIZE];
} CARD_STACK;

CARD *pop(CARD_STACK *stack);
// CARD *pop_top(CARD_STACK *stack);
bool push(CARD_STACK *stack, CARD *card);
bool is_empty(CARD_STACK *stack);
bool is_full(CARD_STACK *stack);

CARD *top_card(CARD_STACK *queue);
void pop_all(CARD_STACK *queue);

typedef struct _DECK {
  // NOTE: whole deck
  CARD cards[52];
  // NOTE: number of cards in deck
  int count;
  // NOTE: cards that are drown from deck
  CARD_STACK drawn_cards;
  // NOTE: top card in deck
  CARD *deck_card;
  // NOTE: sorted small decks
  CARD *sorted_cards[4];
  // NOTE: texture for empty small deck
  SDL_Texture *empty_sorted_card;

} DECK;

extern CARD g_invisible_card[7];

DECK *create_deck(SDL_Renderer *renderer);
void destroy_deck(DECK *deck);

int render_card(FIELD *field, SDL_Renderer *renderer, CARD *card,
                SDL_FPoint *point);
void deselect_all_cards(DECK *deck);

/*
  NOTE: returns CARD pointer if find in deck for inserted col and row. Returns
  NULL if dont find
*/
CARD *find_card(DECK *deck, int col, int row);
int can_card_be_placed(CARD *card_below, CARD *card_above);
int sort_a_card(CARD *card, DECK *deck);
int select_card_below(CARD *card, DECK *deck);
int same_card_selected(CARD *card1, CARD *card2);
int have_more_cards(DECK *deck);

#endif // CARD_H
