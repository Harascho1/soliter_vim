#include "game.h"
#include "my_timer.h"
#include "game_over_menu.h"

static const char *title[2] = {
    "GAME OVER",
    "YOU W0N"
};

static char win_in_seconds[255];

static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color green_color = {150, 255, 150, 255};
static SDL_Color title_color = {23, 150, 52, 255};

static SDL_Texture *tex_game_result[2];
static SDL_Texture *tex_time;
static SDL_Texture *tex_game_over_items[3];
static SDL_Texture *tex_hover_game_over_items[3];

int
game_over_menu_lazy_load(GAME *game) {
    int size;

    if (game->timer->start_timer == 1) {
        if (g_game_win == 1) {
            sprintf(win_in_seconds, "time: %d", game->timer->time_elapsed);
            SDL_Log("U win_in_seconds sam napisao %s\n", win_in_seconds);
        } else {
            win_in_seconds[0] = '\0';
        }
        reset_timer(game->timer);
    }

    size = game->field.text_font;
    char buff[10];
    sprintf(buff, "time: %d", game->timer->time_elapsed);
    tex_time = get_texture_from_text(
        game->font,
        game->renderer,
        buff,
        size,
        &white_color
    );
    if (tex_time == NULL) {
        SDL_Log("tex_time cannot be initiazlied...");
        return 0;
    }

    size = game->field.title_font;
    for (int i = 0; i < 2; i++) {
        tex_game_result[i] = get_texture_from_text(
            game->font,
            game->renderer,
            title[i],
            size,
            &title_color
        );
        if (tex_game_result[i] == NULL) {
            SDL_Log("tex_game_result[%d]  cannot be initiazlied...", i);
            return 0;
        }
    }

    size = game->field.item_font;
    for (int i = 0; i < 3; i++) {
        tex_game_over_items[i] = get_texture_from_text(
            game->font,
            game->renderer,
            game->game_over_menu->items[i].text,
            size,
            &white_color
        );
        if (tex_game_over_items[i] == NULL) {
            SDL_Log("tex_game_over_items[%d]  cannot be initiazlied...", i);
            return 0;
        }
    }

    size = game->field.hover_item_font;
    for (int i = 0; i < 3; i++) {
        tex_hover_game_over_items[i] = get_texture_from_text(
            game->font,
            game->renderer,
            game->game_over_menu->items[i].text,
            size,
            &green_color
        );
        if (tex_hover_game_over_items[i] == NULL) {
            SDL_Log("tex_hover_game_over_items[%d] cannot be initiazlied...", i);
            return 0;
        }
    }
    return 1;
}

void
game_over_menu_lazy_destroy() {
    SDL_DestroyTexture(tex_time);
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(tex_game_result[i]);
    }
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(tex_game_over_items[i]);
        SDL_DestroyTexture(tex_hover_game_over_items[i]);
    }
    return;
}

int
game_over_menu_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_UP:
            case SDLK_W:
                if (game->game_over_menu->selected_item != 0) {
                        game->game_over_menu->selected_item--;
                }
                break;
            case SDLK_DOWN:
            case SDLK_S:
                if (game->game_over_menu->selected_item != game_over_item_type_max - 1) {
                    game->game_over_menu->selected_item++;
                }
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                play_sound(game->soundboard, 0);
                SDL_Delay(150);
                switch (game->game_over_menu->selected_item) {
                    case game_over_item_type_restart:
                        run_a_game(game);
                        push_user_event(g_change_scene_event_type, game_state_gameplay);
                        break;
                    case game_over_item_credits:
                        SDL_Log("Looking at credits...\n");
                        break;
                    case game_over_item_type_exit:
                        push_user_event(g_change_scene_event_type, game_state_main_menu);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    return 1;
}

int
game_over_menu_update(GAME *game) {
    return 1; 
}

int
render_time(GAME *game) {
    if (strlen(win_in_seconds) <= 0) {
        return 0;
    }
    int status;
    int text_witdh, text_height;

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
    if (status == 0) {
        SDL_Log("SDL_GetWindowSizeInPixels failed: %s\n", SDL_GetError());
        return 0;
    }

    status = get_text_size(
        game->font,
        win_in_seconds,
        game->field.text_font,
        &text_witdh,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error ...\n");
        return 0;
    }

    status = render_text(
        game->renderer,
        tex_time,
        &(SDL_Point){
            .x = (width - text_witdh) / 2,
            .y = game->field.title_padding
        }
    );
    if (status == 0) {
        SDL_Log("render_text error ...\n");
        return 0;
    }
    return 1;
}

int
game_over_menu_render(GAME *game) {

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

    if (game->menu_texture == NULL) {
        SDL_Log("menu_texture is NULL\n");
        game->menu_texture = create_texture_from_image(game->renderer, "assets/logo.png");
        if (game->menu_texture == NULL) {
            SDL_Log("menu_texture is NULL\n");
            return 0;
        }
    }

    SDL_FRect *rect = &(SDL_FRect) {
        .x = game->field.square_screen_padding_width,
        .y = 0,
        .w = game->field.screen_height,
        .h = game->field.screen_height,
    };

    status = SDL_RenderTexture(game->renderer, game->menu_texture, NULL, rect);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    int width, height;
    width = game->field.screen_width;
    height = game->field.screen_height;
    int text_width, text_height;

    status = render_time(game);

    status = get_text_size(
        game->font,
        title[g_game_win],
        game->field.title_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
    }

    status = render_text(
        game->renderer,
        tex_game_result[g_game_win],
        &(SDL_Point){.x = (width - text_width) / 2, .y = height / 4}
    );
    if (status == 0) {
        SDL_Log("render_text failed...\n");
        return 0;
    }

    status = get_text_size(
        game->font,
        game->game_over_menu->items[0].text,
        game->field.item_font,
        NULL,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
    }

    int y_coord = height / 2 + game->field.title_padding;

    for (int i = 0; i < game->game_over_menu->count; i++) {

        SDL_Color *color;
        int font_size;
        int selected_height;
        int render_coord_y;
        if (i == game->game_over_menu->selected_item) {
            color = &green_color;
            font_size = game->field.hover_item_font;
            status = get_text_size(
                game->font,
                game->game_over_menu->items[i].text,
                font_size,
                &text_width,
                &selected_height
            );
            if (status == 0) {
                SDL_Log("get_text_size failed...\n");
                return 0;
            } 
            render_coord_y = y_coord + (text_height - selected_height) / 2;
        } else {
            color = &white_color;
            font_size = game->field.item_font;
            status = get_text_size(
                game->font,
                game->game_over_menu->items[i].text,
                font_size,
                &text_width,
                &text_height
            );
            if (status == 0) {
                SDL_Log("get_text_size failed...\n");
                return 0;
            }
            render_coord_y = y_coord;
        }

        SDL_Point dst_rect = {
            .x = (width - text_width) / 2,
            .y = render_coord_y
        };

        SDL_Texture *item = NULL;
        if (i == game->game_over_menu->selected_item) {
            item = tex_hover_game_over_items[i];
        } else {
            item = tex_game_over_items[i];
        }

        status = render_text(
            game->renderer,
            item,
            &dst_rect
        );
        if (status == 0) {
            SDL_Log("render_text failed...\n");
            return 0;
        }

        y_coord += text_height + game->field.item_padding;

    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}

SCENE game_over_menu_scene = {
    .handle_events = game_over_menu_event_handler,
    .update = game_over_menu_update,
    .render = game_over_menu_render,
    .lazy_load = game_over_menu_lazy_load,
    .lazy_destroy = game_over_menu_lazy_destroy
};
