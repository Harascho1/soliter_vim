#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include "cursor.h"
#include "gameplay.h"
#include "texture.h"
#include <stdio.h>

static const char* modes[4] = {
    "mode:normal",
    "mode:select",
    "mode:fly",
    "mode:fly-select"
};

static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color trensparent_green_color = {250, 55, 50, 255};

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
        game->field.text_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        return 0;
    }

    int width, height;
    width = game->field.screen_width;
    height = game->field.screen_height;

    status = render_text(
        game->font,
        game->renderer,
        buffer,
        game->field.text_font,
        &(SDL_Point){
            .x = (width - text_width) / 2,
            .y = (height - text_height - game->field.card_padding_height)
        },
        &white_color
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
    int padding_width = 3 * (game->field.card_width + game->field.card_padding_width) + game->field.gameplay_screen_padding_width;
    for (int suit = 0; suit < 4; suit++) {
        if (game->deck->sorted_cards[suit] == NULL) {
            status = SDL_RenderTexture(
                game->renderer,
                game->deck->empty_sorted_card,
                NULL,
                &(SDL_FRect) {
                    .x = padding_width,
                    .y = game->field.gameplay_screen_padding_height,
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
                &game->field,
                game->renderer,
                game->deck->sorted_cards[suit],
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
        padding_width += game->field.card_width + game->field.card_padding_width;
    }

    return status;
}

int
render_name_textbox(GAME *game) {
    int status;
    int height_indent, width_indent;

    int width = game->field.screen_width;
    int height = game->field.screen_height;


    int rect_width = game->field.name_textbox_width;
    int rect_height = game->field.name_textbox_height;

    height_indent = (height - rect_height) / 2;
    width_indent = game->field.gameplay_screen_padding_width;

    SDL_FRect rect = {
        .x = width_indent,
        .y = height_indent,
        .w = rect_width,
        .h = rect_height
    };

    SDL_Surface *surface = SDL_CreateSurface(rect_width, rect_height, SDL_PIXELFORMAT_RGBA32);
    if (surface == NULL) {
        SDL_Log("SDL_CreateSurface error: %s\n", SDL_GetError());
        return 0;
    }

    const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
    if (format_details == NULL) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_GetPixelFormatDetails error: %s\n", SDL_GetError());
        return 0;
    }

    Uint32 color = SDL_MapRGBA(format_details, NULL, 255, 255, 255, 255);

    status = SDL_FillSurfaceRect(surface, NULL, color);
    if (status == 0) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_FillSurfaceRect error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (texture == NULL) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        return 0;
    }

    status = SDL_RenderTexture(game->renderer, texture, NULL, &rect);
    if (status == 0) {
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
        SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    return 1;
    return 1;
}

int
render_fly_notaions(GAME *game) {
    if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
        return 1;
    }
    int status;
    int height_indent, width_indent;

    int width = game->field.screen_width;
    int height = game->field.screen_height;

    height_indent = game->field.gameplay_screen_padding_height
                    + game->field.card_height
                    + game->field.card_padding_height
                    + 2 * game->field.card_padding_height;
    width_indent = game->field.gameplay_screen_padding_width;

    int rect_width = width - 2 * game->field.gameplay_screen_padding_width;
    int rect_height = game->field.card_padding_height;

    SDL_Rect rect = {
        .x = width_indent,
        .y = height_indent,
        .w = rect_width,
        .h = rect_height
    };

    SDL_FRect frect;
    SDL_RectToFRect(&rect, &frect);

    SDL_Surface *surface = SDL_CreateSurface(rect_width, rect_height, SDL_PIXELFORMAT_RGBA32);
    if (surface == NULL) {
        SDL_Log("SDL_CreateSurface error: %s\n", SDL_GetError());
        return 0;
    }

    const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
    if (format_details == NULL) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_GetPixelFormatDetails error: %s\n", SDL_GetError());
        return 0;
    }

    Uint32 color = SDL_MapRGBA(format_details, NULL, 255, 255, 255, 25);

    status = SDL_FillSurfaceRect(surface, NULL, color);
    if (status == 0) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_FillSurfaceRect error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (texture == NULL) {
        SDL_DestroySurface(surface);
        SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        return 0;
    }


    for (int i = 0; i < 4; i++) {
        status = SDL_RenderTexture(game->renderer, texture, NULL, &frect);
        if (status == 0) {
            SDL_DestroySurface(surface);
            SDL_DestroyTexture(texture);
            SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
            return 0;
        }
        frect.y = frect.y + 3 * game->field.card_padding_height;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    return 1;
}

int
render_counting_time(GAME *game) {
    // Rendering timer in seconds
    int status;
    int width, height;
    width = game->field.screen_width;
    height = game->field.screen_height;

    char timer[20];
    int text_width, text_height;
    snprintf(timer, 20, "seconds: %d", game->timer->time_elapsed);
    status = get_text_size(
        game->font,
        timer,
        game->field.item_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return  status;
    }
    status = render_text(
        game->font,
        game->renderer,
        timer,
        game->field.item_font,
        &(SDL_Point){
            .x = width - game->field.screen_padding - text_width,
            .y = height - text_height - game->field.screen_padding
        },
        &white_color
    );
    if (status == 0) {
        SDL_Log("render_text error...\n");
        return  status;
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
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    status = render_fly_notaions(game);
    if (status == 0) {
        SDL_Log("render_fly_notaions failed... \n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    render_commands(game);

    int width, height;
    width = game->field.screen_width;
    height = game->field.screen_height;

    // *RENDERING SORTED_CARDS
    status = sorted_card_render(game);
    if (status == 0) {
        SDL_Log("sorted_card_render error...\n");
    }

    // *RENDERING DECK AND ONE MORE CARD ON LEFT RIGHT CORNER
    if (game->deck->deck_card != NULL) {
        status = render_card(
            &game->field,
            game->renderer,
            game->deck->deck_card,
            &(SDL_FPoint) {
                .x = game->field.gameplay_screen_padding_width,
                .y = game->field.gameplay_screen_padding_height
            }
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
            &game->field,
            game->renderer,
            card,
            &(SDL_FPoint){
                .x = game->field.gameplay_screen_padding_width + game->field.card_padding_width + game->field.card_width,
                .y = game->field.gameplay_screen_padding_height
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

    // * RENDERING TEXT THAT SHOWS WHAT IN WHAT MODE U ARE
    int mode = game->cursor->mode % 4;
    int text_width, text_height;
    status = get_text_size(
        game->font,
        modes[mode],
        game->field.text_font,
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
        game->field.text_font,
        &(SDL_Point) {
            .x = game->field.screen_padding,
            .y = height - text_height - game->field.screen_padding
        },
        &white_color
    );


    // * RENDERING GAME FIELD
    for (int i = 1; i <= number_of_cards_in_row; i++) {
        int j = 1;
        CARD *card;
        while ((card = find_card(game->deck, i, j)) != NULL) {
            render_card(
                &game->field,
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

    //TODO Kad ubacim lazy load valjda mu nece biti toliko tesko da loaduje
    //status = render_name_textbox(game);
    if (status == 0) {
        SDL_Log("render_name_textbox failed");
        return 0;
    }

    status = render_counting_time(game);
    if (status == 0) {
        SDL_Log("render_counting_time failed");
        return 0;
    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}
