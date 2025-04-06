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

int
load_game_field(DECK *deck) {
    int count = 0;
    int x_coord = padding_of_card / 2;
    for (int i = 0; i < number_of_cards_in_row; i++) {
        int y_coord = 2 * padding_of_card + card_height;
        for (int j = 0; j < i + 1; j++) {
            if (i == j) {
                deck->cards[count].visible = visible;
            }
            deck->cards[count].frame->x = x_coord;
            deck->cards[count].frame->y = y_coord;
            deck->cards[count].pos->col = i + 1;
            deck->cards[count].pos->row = j + 1;


            y_coord += padding_of_card;
            count++;
        }
        x_coord += padding_of_card + card_width;
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

    float card_width_height_ratio = 7/5.0f;
    card_width = (width - number_of_cards_in_row * padding_of_card) / number_of_cards_in_row; 
    card_height = card_width * card_width_height_ratio;

    SDL_FRect *rectovic = SDL_malloc(sizeof(SDL_FRect));
    rectovic->x = (float)padding_of_card / 2.0f - padding_of_card / 4.0f;
    rectovic->y = (float)padding_of_card * 2 + card_height - padding_of_card / 4;
    rectovic->w = (float)card_width + padding_of_card / 2.0f;
    rectovic->h = (float)card_height + padding_of_card / 2.0f;

    game->cursor = SDL_malloc(sizeof(CURSOR));
    game->cursor->cursor = rectovic;
    if (game->cursor->cursor == NULL) {
        SDL_Log("nece da mi radi rectovic\n");
    }
    game->cursor->pos = SDL_malloc(sizeof(POSITION));
    game->cursor->pos->col = 1; 
    game->cursor->pos->row = 1;

    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        game_quit(game);
        return status;
    }

    game->main_menu = menu_init(
        main_menu_items,
        sizeof(main_menu_items) / sizeof(main_menu_items[0])
    );
    if (game->main_menu == NULL) {
        SDL_Log("menu_init failed.\n");
        game_quit(game);
        return status;
    }

    game->game_over_menu = menu_init(
        game_over_items,
        sizeof(game_over_items) / sizeof(game_over_items[0])
    );
    if (game->game_over_menu == NULL) {
        SDL_Log("menu_init failed.\n");
        game_quit(game);
        return status;
    }

    game->font = font_init(
        "../assets/font.ttf"
    );
    if (game->font == NULL) {
        SDL_Log("font_init failed.\n");
        game_quit(game);
        return status;
    }

    game->deck = create_deck();
    if (game->deck == NULL) {
        SDL_Log("create_deck failed\n");
        game_quit;
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


    load_game_field(game->deck);

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
        font_destroy(game->font);
    }
}

int push_user_event(Uint32 type, Sint32 code) {
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.user.code = code;
    SDL_Log("Code je %d\n", code);
    return SDL_PushEvent(&event);
}