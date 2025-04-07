#ifndef CARD_H
#define CARD_H

enum {
    not_visible = 0,
    visible
};

extern int card_width;
extern int card_height;

typedef enum _SUIT {
    suit_clubs,
    suit_diamonds,
    suit_hearts,
    suit_spades
} SUIT;

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
    int selected; //0 if not selected, 1 if selected
    SDL_FPoint *frame;
    POSITION *pos;
} CARD;

typedef struct _HAND {
    CARD *cards;
    int count;
} HAND;

typedef struct _DECK {
    CARD cards[52];
    int count;
} DECK;

extern CARD invisible_card[7];

DECK* create_deck();
void destroy_deck(DECK *deck);

int render_card(SDL_Renderer *renderer, CARD *card, SDL_FPoint *point);
void deselect_all_cards(DECK *deck);

#endif //CARD_H