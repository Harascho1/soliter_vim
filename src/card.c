#include "card.h"
#include "texture.h"

int card_width = 0;
int card_height = 0;

void
deselect_all_cards(DECK *deck) {
    for (int i = 0; i < 52; i++) {
        deck->cards[i].selected = 0;
    }
}

CARD*
find_card(DECK *deck, int col, int row) {
    for (int i = 0; i < 52; i++) {
        if (deck->cards[i].pos->col == col &&
            deck->cards[i].pos->row == row) {
            return &deck->cards[i];
        }
    }
    return NULL;
}

int
select_card_below(CARD *card, DECK *deck) {
    if (card == NULL) return 0;
    if (card->visible == not_visible) {
        return 1;
    }
    CARD *tmp_card = find_card(deck, card->pos->col, card->pos->row + 1);
    if (tmp_card == NULL) {
        return 1;
    }
    SDL_Log("moj brat\n");
    tmp_card->selected = 1;
    return select_card_below(tmp_card, deck);
}

void shuffle_deck(DECK *deck) {
    if (deck == NULL) {
        SDL_Log("Deck is NULL and therefore cannot be shuffled");
        return;
    }
    for (int i = 0; i < deck->count; i++) {
        int j = rand() % deck->count;
        CARD temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

int
sort_a_card(CARD *card, DECK *deck) {
    SDL_Log("SI USO\n");
    for (int suit = 0; suit <= suit_spades; suit++) {
        if (card->suit == deck->sorted_cards[suit]->suit) {
            SDL_Log("Alo momak\n");
            if (deck->sorted_cards[suit]->value + 1 != card->value) {
                return 0;
            }
            SDL_Log("Alo momak\n");
            int card_value = card->value;
            card->pos->col = deck->sorted_cards[suit]->pos->col;
            card->pos->row = deck->sorted_cards[suit]->pos->row;
            card->frame->x = deck->sorted_cards[suit]->frame->x;
            card->frame->y = deck->sorted_cards[suit]->frame->y;
            SDL_Log(
                "(%d, %d)",
                card->pos->col,
                card->pos->row
            );
            deck->sorted_cards[suit]->value = card_value;
            return 1;
        }
    }
    SDL_Log("SI IZASO\n");
    return 0;
}

int
can_card_be_placed(CARD *card_below, CARD *card_above) {
    if (card_below->value + 1 != card_above->value) {
        return 0;
    }
    if ((card_below->suit == suit_clubs || card_below->suit == suit_spades) &&
        (card_above->suit == suit_diamonds || card_above->suit == suit_hearts)) {
        return 1;
    } else if ((card_below->suit == suit_diamonds || card_below->suit == suit_hearts) &&
        (card_above->suit == suit_clubs || card_above->suit == suit_spades)) {
        return 1;
    }
    return 0;
}

DECK* create_deck() {
    DECK *deck = NULL;
    deck = SDL_malloc(sizeof(DECK));
    if (deck == NULL) {
        return deck;
    }
    deck->count = 28;
    int i = 0;
    for (int suit = suit_clubs; suit <= suit_spades; suit++) {
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
    deck->new_card = NULL;
    deck->deck_card = &deck->cards[28];
    shuffle_deck(deck);
    return deck;
}

void destroy_card(CARD *card) {
    if (card != NULL) {
        SDL_free(card->frame);
        SDL_free(card->pos);
    }
}

void destroy_deck(DECK *deck) {
    if (deck != NULL) {
        for (int i = 0; i < 52; i++) {
            destroy_card(&deck->cards[i]);
        }
        for (int i = 0; i < 4; i++) {
            destroy_card(deck->sorted_cards[i]);
            SDL_free(deck->sorted_cards[i]);
        }
        SDL_free(deck);
    }
}

char* find_path(CARD *card) {
    char *path;
    path = SDL_malloc(sizeof(char) * 100);

    if (card->value == 0) {
        sprintf(path, "../assets/cards/blank_front_with_num_boarders_white.png");
        return path;
    }

    if (card->visible == not_visible) {
        sprintf(path, "../assets/cards/back_red_basic_white.png");
        return path;
    }

    char suit[10];
    switch (card->suit) {
        case suit_clubs:
            strncpy(suit, "clubs", 6);
            break;
        case suit_diamonds:
            strncpy(suit, "diamonds", 9);
            break;
        case suit_hearts:
            strncpy(suit, "hearts", 7);
            break;
        case suit_spades:
            strncpy(suit, "spades", 7);
            break;
        default:
            SDL_Log("Unknown suit");
            return NULL;
    }

    char value[6];
    switch (card->value)
    {
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
        sprintf(path, "../assets/cards/%s_%s.png", value, suit);
        return path;

    }

    sprintf(path, "../assets/cards/%s_%s_white.png", value, suit);

    return path;
}

int
render_card(SDL_Renderer *renderer, CARD *card, SDL_FPoint *point) {
    if (renderer == NULL || card == NULL) {
        SDL_Log("renderer or card are NULL in render_card fun...\n");
        return 0;
    }

    char* path;
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
        renderer,
        texture,
        NULL,
        &(SDL_FRect){point->x, point->y, card_width, card_height}
    );

    if (status == 0) {
        SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
        return status;
    }

    SDL_DestroyTexture(texture);
    SDL_free(path);
    return status;
}

int render_hand(SDL_Renderer *renderer, HAND *hand, SDL_FRect *rect) {
    int status = 0;

    if (renderer == NULL || hand == NULL) {
        SDL_Log("Renderer or card is NULL");
        return status;
    }
    
    SDL_Surface *surface;
    SDL_Texture *texture;
    char *path;
    for (int i = 0; i < hand->count; i++) {
        path = find_path(&hand->cards[i]);
        if (path == NULL) {
            SDL_Log("Path is NULL");
            return status;
        }
        surface = IMG_Load(path);
        if (surface == NULL) {
            SDL_Log("IMG_Load error: %s", SDL_GetError());
            return status;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
            SDL_DestroySurface(surface);
            return status;
        }
        status = SDL_RenderTexture(renderer, texture, NULL, rect);
        if (status == 0) {
            SDL_Log("SDL_RenderTexture error: %s", SDL_GetError());
            return status;
        }
        rect->x += 60;
    }
    SDL_free(path);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    return status;
}