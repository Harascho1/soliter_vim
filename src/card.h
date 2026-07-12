#ifndef CARD_H
#define CARD_H

#include "res/field.h"

#define STACK_SIZE 30

enum { not_visible = 0, visible };

enum { not_selected = 0, selected };

typedef enum { suit_clubs, suit_spades, suit_diamonds, suit_hearts } SUIT;

typedef enum {
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

typedef struct {
  int row;
  int col;
} POSITION;

typedef struct {
  VALUE value;
  SUIT suit;
  int visible;
  int selected;
  SDL_FPoint* frame;
  POSITION* pos;
  int on_field;
} CARD;

typedef struct {
  int count;
  CARD* array[STACK_SIZE];
} CARD_STACK;

CARD* pop(CARD_STACK* stack);
// CARD *pop_top(CARD_STACK *stack);
bool push(CARD_STACK* stack, CARD* card);
bool is_empty(const CARD_STACK* stack);
bool is_full(const CARD_STACK* stack);

CARD* top_card(const CARD_STACK* stack);
void pop_all(CARD_STACK* queue);

typedef struct {
  // NOTE: whole deck
  CARD cards[52];
  // NOTE: number of cards in deck
  int count;
  // NOTE: cards that are drawn from the deck
  CARD_STACK drawn_cards;
  // NOTE: top card in deck
  CARD* deck_card;
  // NOTE: sorted small decks
  CARD* sorted_cards[4];
  // NOTE: texture for empty small deck
  SDL_Texture* empty_sorted_card;

} DECK;

extern CARD g_invisible_card[7];

DECK* create_deck(SDL_Renderer* renderer);
void destroy_deck(DECK* deck);

bool render_card(
  const FIELD* field, SDL_Renderer* renderer, const CARD* card, const SDL_FPoint* point
);
void deselect_all_cards(DECK* deck);

/*
  NOTE: returns a CARD pointer if it finds in deck for inserted col and row.
  Returns NULL if it doesn't find
*/
CARD* find_card(const DECK* deck, int col, int row);
bool can_card_be_placed(const CARD* card_below, const CARD* card_above);
int sort_a_card(CARD* card, DECK* deck);
int select_card_below(const CARD* card, const DECK* deck);
bool same_card_selected(const CARD* card1, const CARD* card2);
bool have_more_cards(const DECK* deck);

#endif // CARD_H
