#include "game.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
#include "config.h"
#include "field.h"
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
    "Main Menu"
};

const char *setting_items[] = {
    "Options",
    "Cancel",
    "Macros"
};

static FILE *saves_file;

Uint32 g_change_scene_event_type = (Uint32) - 1;

int game_update = 0;
int g_game_win = 0;
CARD g_invisible_card[7];

int
load_game_field(DECK *deck, FIELD *field) {
    int count = 0;
    int x_coord = field->gameplay_screen_padding_width;

    for (int i = 0; i < number_of_cards_in_row; i++) {
        int y_coord = 2 * field->card_padding_height + field->card_height;
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

            y_coord += field->card_padding_height;
            count++;
        }
        x_coord += field->card_padding_width + field->card_width;
    }
    for (; count < 52; count++) {
        deck->cards[count].frame->x = field->gameplay_screen_padding_width;
        deck->cards[count].frame->y = field->gameplay_screen_padding_height;
        deck->cards[count].pos->col = 1;
        deck->cards[count].pos->row = 0;
        deck->cards[count].on_field = 0;
    }

    for (int suit = suit_clubs; suit <= suit_spades; suit++) {
        deck->sorted_cards[suit] = NULL;
    }

    return 1;
}

int
fullscree_mode(GAME *game) {
    int status;
    SDL_DisplayID id;
    //SDL_DisplayMode **mode = SDL_GetFullscreenDisplayModes(id, NULL);
    //if (mode == NULL) {
    //    SDL_Log("SDL_GetFullscreenDisplayModes error %s\n", SDL_GetError());
    //    return 0;
    //}
    status = SDL_SetWindowFullscreen(game->window, true);
    if (status == 0) {
        SDL_Log("SDL_SetWindowFullscreen error %s", SDL_GetError());
        return status;
    }
    return status;
}

int
game_init(GAME* game, const char *title, const RESOLUTION *res) {
    int status = 0;

    if (does_config_file_exist() == 0) {
        create_config_file();
    }
    if (does_option_file_exist() == 0) {
        create_option_file();
    }
    load_config();

    game->window = SDL_CreateWindow(title, 600, 600, 0);
    if (game->window == NULL) {
        SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return status;
    }

    status = fullscree_mode(game);
    if (status == 0) {
        SDL_Log("fullscreen is not set\n");
    }

    SDL_SyncWindow(game->window);

    game->font = create_font(
        "assets/font.ttf"
    );
    if (game->font == NULL) {
        SDL_Log("font_init failed.\n");
        game_quit(game);
        return status;
    }

    int w, h;
    status = SDL_GetWindowSizeInPixels(game->window, &w, &h);

    status = load_field(&game->field, w, h, game->font);
    if (status == 0) {
        SDL_Log("load_field error...\n");
        game_quit(game);
        return 0;
    }



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

    game->setting_menu = create_menu(
        setting_items,
        sizeof(setting_items) / sizeof(setting_items[0])
    );
    if (game->setting_menu == NULL) {
        SDL_Log("menu_init failed.\n");
        game_quit(game);
        return status;
    }

    game->timer = create_timer();
    if (game->timer == NULL) {
        SDL_Log("create_timer failed\n");
        game_quit(game);
        return status;
    }

    game->background_texture = create_texture_from_image(
        game->renderer,
        "assets/table.png"
    );
    if (game->background_texture == NULL) {
        SDL_Log("Background texture is NULL\n");
        game_quit(game);
        return status;
    }

    game->menu_texture = create_texture_from_image(
        game->renderer,
        "assets/logo.png"
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
    if (game->setting_menu != NULL) {
        destroy_menu(game->setting_menu);
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
    if (game->timer != NULL) {
        destroy_timer(game->timer);
    }
}

void
run_a_game(GAME *game) {
    if (game == NULL) {
        SDL_Log("game is NULL\n");
    }

    destroy_cursor(game->cursor);
    destroy_deck(game->deck);

    game->deck = create_deck(game->renderer);
    if (game->deck == NULL) {
        SDL_Log("create_deck error\n");
        game_quit(game);
    }

    game->cursor = create_cursor(&game->field, 1, 1);
    if (game->cursor == NULL) {
        SDL_Log("create_cursor error\n");
        game_quit(game);
    }

    g_game_win = 0;
    game_update = 0;
    load_game_field(game->deck, &game->field);
}

int
push_user_event(Uint32 type, Sint32 code) {
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.user.code = code;
    return SDL_PushEvent(&event);
}

void
save_score(GAME *game) {
    //if (g_game_win != 1) {
    //    return;
    //}

    saves_file = fopen("assets/saves.txt", "a+");

    if (saves_file == NULL) {
        SDL_Log("Nije uspeo da otvori fajl\n");
        return;
    }

    char buff[255];
    while (fgets(buff, 255, saves_file)) {

    }
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    char time_buff[80];

    strftime(time_buff, 80, "%Y-%m-%d %H:%M:%S", tm_info);

    sprintf(buff, "seconds: %d mode: normal time: %s\r\n", game->timer->time_elapsed, time_buff);

    fputs(buff, saves_file);

    fclose(saves_file);


}
