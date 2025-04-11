#ifndef CARD_H
#define CARD_H

enum {
    not_visible = 0,
    visible
};

enum {
    not_selected = 0,
    selected
};


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
    int on_field;
} CARD;


typedef struct _CARD_QUEUE {
    int p;
    int q;
    int max_items;
    int count;
    CARD **queue;
} CARD_QUEUE;

CARD* pop(CARD_QUEUE *queue);
CARD* pop_top(CARD_QUEUE *queue);
int push(CARD_QUEUE *queue, CARD *card);
int is_queue_full(CARD_QUEUE *queue);
int is_queue_empty(CARD_QUEUE *queue);
CARD* view_top_card_in_queue(CARD_QUEUE *queue);
int pop_all(CARD_QUEUE *queue);

typedef struct _HAND {
    CARD *cards;
    int count;
} HAND;

typedef struct _DECK {
    CARD cards[52];
    int count;
    CARD_QUEUE *new_cards;
    CARD *deck_card;

    //TODO OVO MOZDA MOZE BOLJE DA SE OSMISLI
    CARD *sorted_cards[4];

} DECK;

extern int g_card_width;
extern int card_height;
extern CARD g_invisible_card[7];
extern int padding_of_card;

DECK* create_deck();
void destroy_deck(DECK *deck);

int render_card(SDL_Renderer *renderer, CARD *card, SDL_FPoint *point);
void deselect_all_cards(DECK *deck);
CARD* find_card(DECK *deck, int col, int row);
int can_card_be_placed(CARD *card_below, CARD *card_above);
int sort_a_card(CARD *card, DECK* deck);
int select_card_below(CARD *card, DECK *deck);
int same_card_selected(CARD *card1, CARD *card2);

#endif //CARD_H