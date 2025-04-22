#include "gameplay.h"
#include "texture.h"

static const char* modes[4] = {
    "mode:normal",
    "mode:select",
    "mode:fly",
    "mode:fly-select"
};

int
render_commands(GAME *game) {
    if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
        return 0;
    }

    if (have_number_hover(game->cursor) == 0) {
        return 0;
    }

    //render
    int status;
    int text_width, text_height;

    if (strlen(buffer) == 0) {
        return 0;
    }
    status = get_text_size(
        game->font,
        buffer,
        title_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        return 0;
    }

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);

    status = render_text(
        game->font,
        game->renderer,
        buffer,
        title_size,
        &(SDL_Point){
            .x = (width - text_width) / 2,
            .y = (height - text_height - padding_of_card)
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        return 0;
    }
    return 1;

}

int
render_cursor(GAME *game) {
    int status = 0;

    const char *path = "assets/cursor.png";
    SDL_Texture *texture = create_texture_from_image(game->renderer, path);
    if (texture == NULL) {
        return 0;
    }

    if (game->cursor->cursor == NULL) {
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
sorted_card_render(GAME *game) {
    int status;
    int padding_width = 3 * (game->field.card_width + game->field.card_padding) + game->field.screen_padding;
    for (int suit = 0; suit < 4; suit++) {
        if (game->deck->sorted_cards[suit] == NULL) {
            status = SDL_RenderTexture(
                game->renderer,
                game->deck->empty_sorted_card,
                NULL,
                &(SDL_FRect) {
                    .x = padding_width,
                    .y = game->field.screen_padding,
                    .w = game->field.card_width,
                    .h = game->field.card_height 
                }
            );
            if (status == 0) {
                SDL_Log("SDL_RenderTexture error %s\n", SDL_GetError());
                return status;
            }
        } else {
            status = render_card(
                game->renderer,
                game->deck->sorted_cards[suit],
                //game->deck->sorted_cards[suit]->frame
                &(SDL_FPoint) {
                    .x = padding_width,
                    .y = game->field.screen_padding
                }
            );
            if (status == 0) {
                SDL_Log("render_card error...\n");
                return status;
            }
        }
        padding_width += game->field.card_width + game->field.card_padding;
    }

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
        game->background_texture = create_texture_from_image(game->renderer, "assets/background.png");
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

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        printf("ZASTOOOOO\n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    render_commands(game);

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);

    // *RENDERING SORTED_CARDS
    status = sorted_card_render(game);
    if (status == 0) {
        SDL_Log("sorted_card_render error...\n");
    }

    // *RENDERING DECK AND ONE MORE CARD ON LEFT RIGHT CORNER

    if (game->deck->deck_card != NULL) {
        status = render_card(
            game->renderer,
            game->deck->deck_card,
            game->deck->deck_card->frame
        );
        if (status == 0) {
            SDL_Log("render card error...\n");
            return 0;
        }
    }
    if (game->cursor->pos->col == 1 &&
        game->cursor->pos->row == 0) {

            status = render_cursor(game);
            if (status == 0) {
                SDL_Log("render_cursor error...\n");
                return 0;
            }
    }

    CARD *card = view_top_card_in_queue(game->deck->new_cards);
    if (card != NULL) {
        status = render_card(
            game->renderer,
            card,
            &(SDL_FPoint){
                .x = padding_of_card / 2 + padding_of_card + g_card_width,
                .y = padding_of_card / 2
            }
        );
        if (status == 0) {
            SDL_Log("render card error...\n");
        }
    }
    if (game->cursor->pos->col == 2 &&
        game->cursor->pos->row == 0) {

            status = render_cursor(game);
            if (status == 0) {
                SDL_Log("render_cursor error...\n");
                return 0;
            }
    }

    // * RENDERING TEXT THAT SHOWS WHAT IN WHAT MODE ARE U
    int mode = game->cursor->mode % 4;
    int text_width, text_height;
    status = get_text_size(
        game->font,
        modes[mode],
        standard_font_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
    }
    status = render_text(
        game->font,
        game->renderer,
        modes[mode],
        standard_font_size,
        &(SDL_Point){padding_of_card, height - text_height - padding_of_card},
        &(SDL_Color){255,255,255,255}
    );


    // * RENDERING GAME FIELD
    for (int i = 1; i <= number_of_cards_in_row; i++) {
        int j = 1;
        CARD *card;
        while ((card = find_card(game->deck, i, j)) != NULL) {
            render_card(
                game->renderer,
                card,
                card->frame
            );
            if (game->cursor->pos->col == i &&
                game->cursor->pos->row == j) {
                    status = render_cursor(game);
                    if (status == 0) {
                        SDL_Log("render_cursor error...\n");
                        return 0;
                    }
            }
            j++;
        }
        if (game->cursor->pos->col == i &&
            game->cursor->pos->row == j) {
                status = render_cursor(game);
                if (status == 0) {
                    SDL_Log("render_cursor error...\n");
                    return 0;
                }
        }
    }

    char timer[50];
    snprintf(timer, sizeof(timer), "seconds: %d", game->timer->time_elapsed);
    status = get_text_size(
        game->font,
        timer,
        standard_font_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
    }
    status = render_text(
        game->font,
        game->renderer,
        timer,
        standard_font_size,
        &(SDL_Point){width - padding_of_card - text_width, height - text_height - padding_of_card},
        &(SDL_Color){255,255,255,255}
    );

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}
