#include "game.h"
#include "texture.h"

const char *main_menu_items[] = {
    "Play",
    "Settings",
    "Credits",
    "Exit"
};

const char *game_over_items[] = {
    "Restart",
    "Credits",
    "Exit"
};

Uint32 g_change_scene_event_type = (Uint32) - 1;

int game_update = 0;
CARD g_invisible_card[7];

int
load_game_field(DECK *deck) {
    int count = 0;
    int x_coord = padding_of_card / 2;
    
    for (int i = 0; i < number_of_cards_in_row; i++) {
        int y_coord = 2 * padding_of_card + card_height;
        g_invisible_card[i].frame = SDL_malloc(sizeof(SDL_FPoint));
        g_invisible_card[i].pos = SDL_malloc(sizeof(POSITION));
        g_invisible_card[i].pos->row = 1;
        g_invisible_card[i].pos->col = i + 1;
        g_invisible_card[i].frame->x = x_coord;
        g_invisible_card[i].frame->y = y_coord;
        g_invisible_card[i].value = 14;
        for (int j = 0; j < i + 1; j++) {
            if (i == j) {
                deck->cards[count].visible = visible;
            }
            deck->cards[count].frame->x = x_coord;
            deck->cards[count].frame->y = y_coord;
            deck->cards[count].pos->col = i + 1;
            deck->cards[count].pos->row = j + 1;
            deck->cards[count].on_field = 1;

            y_coord += padding_of_card;
            count++;
        }
        x_coord += padding_of_card + g_card_width;
    }
    for (; count < 52; count++) {
        deck->cards[count].frame->x = padding_of_card / 2;
        deck->cards[count].frame->y = padding_of_card / 2;
        deck->cards[count].pos->col = 1;
        deck->cards[count].pos->row = 0;
        deck->cards[count].on_field = 0;

    }

    int padding_width = 3 * (g_card_width + padding_of_card) + padding_of_card /2;
    for (int suit = suit_clubs; suit <= suit_spades; suit++) {
        deck->sorted_cards[suit] = SDL_malloc(sizeof(CARD));
        deck->sorted_cards[suit]->frame = SDL_malloc(sizeof(SDL_FPoint));
        deck->sorted_cards[suit]->frame->x = padding_width;
        deck->sorted_cards[suit]->frame->y = padding_of_card / 2;

        deck->sorted_cards[suit]->pos = SDL_malloc(sizeof(POSITION));
        deck->sorted_cards[suit]->pos->col = suit + 3;
        deck->sorted_cards[suit]->pos->row = 0;
        padding_width += padding_of_card + g_card_width;

        deck->sorted_cards[suit]->suit = suit;
        deck->sorted_cards[suit]->value = 0;
        deck->sorted_cards[suit]->visible = 1;
    }

    return 1;
}

int game_init(GAME* game, const char *title, const RESOLUTION *res) {
    int status = 0;

    game->window = SDL_CreateWindow(title, res->width, res->height, 0);
    if (game->window == NULL) {
        SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return status;
    }

    int width = res->width, height = res->height;

    float g_card_width_height_ratio = 7/5.0f;
    padding_of_card = width / 30;
    g_card_width = (width - number_of_cards_in_row * padding_of_card) / number_of_cards_in_row; 
    card_height = g_card_width * g_card_width_height_ratio;

    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        game_quit(game);
        return status;
    }

    game->main_menu = create_menu(
        main_menu_items,
        sizeof(main_menu_items) / sizeof(main_menu_items[0])
    );
    if (game->main_menu == NULL) {
        SDL_Log("menu_init failed.\n");
        game_quit(game);
        return status;
    }

    game->game_over_menu = create_menu(
        game_over_items,
        sizeof(game_over_items) / sizeof(game_over_items[0])
    );
    if (game->game_over_menu == NULL) {
        SDL_Log("menu_init failed.\n");
        game_quit(game);
        return status;
    }

    game->font = create_font(
        "../assets/font.ttf"
    );
    if (game->font == NULL) {
        SDL_Log("font_init failed.\n");
        game_quit(game);
        return status;
    }

    game->background_texture = create_texture_from_image(
        game->renderer,
        "../assets/background.png"
    );
    if (game->background_texture == NULL) {
        SDL_Log("Background texture is NULL\n");
        game_quit(game);
        return status;
    }

    Uint32 event_type = SDL_RegisterEvents(1);
    if (event_type == (Uint32) - 1) {
        SDL_Log("SDL_RegisterEvents failed: %s\n", SDL_GetError());
        game_quit(game);
        return status;
    }
    g_change_scene_event_type = event_type;

    game->deck = NULL;
    game->cursor = NULL;

    return 1;
}

void game_quit(GAME* game) {
    if (game->renderer != NULL) {
        SDL_DestroyRenderer(game->renderer);
    }
    if (game->window != NULL) {
        SDL_DestroyWindow(game->window);
    }
    if (game->main_menu != NULL) {
        destroy_menu(game->main_menu);
    }
    if (game->game_over_menu != NULL) {
        destroy_menu(game->game_over_menu);
    }
    if (game->font != NULL) {
        destroy_font(game->font);
    }
    if (game->deck != NULL) {
        destroy_deck(game->deck);
    }
    if (game->cursor != NULL) {
        destroy_cursor(game->cursor);
    }
    if (game->background_texture != NULL) {
        SDL_DestroyTexture(game->background_texture);
    }
}

void
run_a_game(GAME *game) {
    if (game == NULL) {
        SDL_Log("game is NULL\n");
    }

    destroy_cursor(game->cursor);
    destroy_deck(game->deck);

    game->deck = create_deck();
    if (game->deck == NULL) {
        SDL_Log("create_deck error\n");
        game_quit(game);
    }

    game->cursor = create_cursor(1, 1);
    if (game->cursor == NULL) {
        SDL_Log("create_cursor error\n");
        game_quit(game);
    }
    game_update = 0;
    load_game_field(game->deck);
}

int push_user_event(Uint32 type, Sint32 code) {
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.user.code = code;
    return SDL_PushEvent(&event);
}