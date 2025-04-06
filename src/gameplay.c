#include "game.h"
#include "texture.h"

int change_cursor_frame(GAME *game);

//TODO - ne znam zasto nece
CARD*
selected_card(DECK *deck) {
    for (int i = 0; i < 52; i++) {
        if (deck->cards[i].selected == 1) {
            return &deck->cards[i];
        }
    }
    return NULL;
}

int
select_a_card(GAME *game) {
    if (game->cursor->mode == 1) {
        for (int i = 0; i < 52; i++) {
            if (game->deck->cards[i].pos->col == game->cursor->pos->col
            && game->deck->cards[i].pos->row == game->cursor->pos->row) {
                game->cursor->mode = 0;
                CARD *card = selected_card(game->deck);
                if (card == NULL) {
                    SDL_Log("Ni jedna karta nije selektovana\n");
                    return 0;
                }
                SDL_Log("(%d, %d)\n", card->pos->row, card->pos->col);
                card->pos->col = game->cursor->pos->col;
                card->pos->row = game->deck->cards[i].pos->row + 1;
                card->frame->x = game->deck->cards[i].frame->x;
                card->frame->y = game->deck->cards[i].frame->y + padding_of_card;
                card->selected = 0;
                game->cursor->pos->row = card->pos->row;

                SDL_Log("(%d, %d)\n", card->pos->row, card->pos->col);
                
                change_cursor_frame(game);
                return 1;
            }

        }
        SDL_Log("PLS NE\n");
        game->cursor->mode = 0;
        return 0;

    }

    for (int i = 0; i < 52; i++) {
        if (game->deck->cards[i].pos->col == game->cursor->pos->col
        && game->deck->cards[i].pos->row == game->cursor->pos->row) {
            game->deck->cards[i].selected = !game->deck->cards[i].selected;
            game->cursor->mode = 1;
            return 1;
        }
    }
    game->cursor->mode = 0;
    return 0;
}

CARD*
is_there_a_card(GAME *game, int *changed_row, int *changed_col) {
    int col, row;
    col = game->cursor->pos->col;
    row = game->cursor->pos->row;
    if (changed_col != NULL) {
        col = *changed_col;
    } 
    if (changed_row != NULL) {
        row = *changed_row;
    }
    for (int i = 0; i < 52; i++) {
        if (game->deck->cards[i].pos->col == col &&
            game->deck->cards[i].pos->row == row) {
                return &game->deck->cards[i];
        }
    }
    return NULL;
}

int
change_cursor_frame(GAME *game) {
    int i = 0;
    for (i = 0; i < 52; i++) {
        if (game->deck->cards[i].pos->col == game->cursor->pos->col
        && game->deck->cards[i].pos->row == game->cursor->pos->row) {
            break;
        }
    }
    if (i == 52) {
        return 0;
    }
    game->cursor->cursor->x = game->deck->cards[i].frame->x - padding_of_card / 4; 
    game->cursor->cursor->y = game->deck->cards[i].frame->y - padding_of_card / 4; 
    return 1;
}

int
gamaplay_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                SDL_Log("Game Overing...\n");
                push_user_event(g_change_scene_event_type, game_state_game_over);
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                if (game->cursor->pos->col == 7) {
                    break;
                }
                int new_col = game->cursor->pos->col + 1;
                if (is_there_a_card(game, NULL, &(new_col)) == 0) {
                    break;
                }
                game->cursor->pos->col++;
                change_cursor_frame(game);
                break;                
            case SDLK_A:
            case SDLK_LEFT:
                if (game->cursor->pos->col == 1) {
                    break;
                }
                new_col = game->cursor->pos->col - 1;
                if (is_there_a_card(game, NULL, &new_col) == 0) {
                    if (new_col != 1) {
                        break;
                    }
                }
                game->cursor->pos->col--;
                change_cursor_frame(game);
                break;                
            case SDLK_S:
            case SDLK_DOWN:
                int new_row = game->cursor->pos->row + 1;
                if (is_there_a_card(game, &new_row, NULL) == 0) {
                    break;
                }
                game->cursor->pos->row++;
                change_cursor_frame(game);
                break;                
            case SDLK_W:
            case SDLK_UP:
                if (game->cursor->pos->row == 1) {
                    break;
                }
                new_row = game->cursor->pos->row - 1;
                if (is_there_a_card(game, &new_row, NULL) == 0) {
                    break;
                }
                game->cursor->pos->row--;
                change_cursor_frame(game);
                break;                
            case SDLK_RETURN:
                select_a_card(game);
                break;
            default:
                break;
        }
    }
    return 1;
}

int
gameplay_update(GAME* game) {
    //TODO - ako je karta okkrenuta poledjini a karte ispod nje nema onda treba da se otkije ta karta

    return 1;
}

int render_deck(GAME *game) {

    return 1;
}

int render_cursor(GAME *game) {
    int status = 0;

    const char *path = "../assets/cards/blank_front_with_num_boarders.png";
    SDL_Texture *texture = create_texture_from_image(game->renderer, path);
    if (texture == NULL) {
        SDL_Log("create_texture_from_image error...\n");
        return 0;
    }

    status = SDL_RenderTexture(
        game->renderer,
        texture,
        NULL,
        game->cursor->cursor
    );
    if (status == 0) {
        SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_DestroyTexture(texture);
    return status;
}

int
gameplay_render(GAME* game) {
    int status;
    
    if (game == NULL) {
        SDL_Log("game is NULL\n");
        return 0;
    }

    if (game->renderer == NULL) {
        SDL_Log("game->renderer is NULL\n");
        return 0;
    }

    if (game->background_texture == NULL) {
        SDL_Log("g_background_texture is NULL\n");
        game->background_texture = create_texture_from_image(game->renderer, "../assets/background.png");
        if (game->background_texture == NULL) {
            SDL_Log("Background texture is NULL\n");
            return 0;
        }
    }

    status = SDL_RenderClear(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
        return 0;
    }

    const char *path = "../assets/cards/blank_front_with_num_boarders_white.png";
    SDL_Texture *texture = create_texture_from_image(game->renderer, path);
    if (texture == NULL) {
        SDL_Log("create_texture_from_image failed...\n");
        return 0;
    }

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        printf("ZASTOOOOO\n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);

    int y_blank_coord = padding_of_card / 2;
    int x_blank_coord = width - padding_of_card / 2 - card_width;
    for (int i = 0; i < 4; i++) {
        status = SDL_RenderTexture(game->renderer, texture, NULL, &(SDL_FRect){x_blank_coord, y_blank_coord, card_width, card_height});
        x_blank_coord -= padding_of_card + card_width;
    }

    status = render_card(
        game->renderer,
        &game->deck->cards[51],
        &(SDL_FPoint){padding_of_card / 2, padding_of_card / 2}
    );
    if (status == 0) {
        SDL_Log("render card error...\n");
    }

    status = render_cursor(game);
    if (status == 0) {
        SDL_Log("render_cursor error...\n");
        return 0;
    }

    //TODO - posle zameni ovo
    int count = 0;
    for (int i = 0; i < number_of_cards_in_row; i++) {
        int j = 1;
        int tmp_i = i + 1;
        CARD *card;
        while ((card = is_there_a_card(game, &j, &tmp_i)) != NULL) {
            render_card(
                game->renderer,
                card,
                card->frame
            );
            j++;
        }

    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}

SCENE gameplay_scene = {
    .handle_events = gamaplay_event_handler,
    .update = gameplay_update,
    .render = gameplay_render
};