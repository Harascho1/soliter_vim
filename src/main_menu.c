#include "main_menu.h"
#include "SDL3/SDL_events.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "font.h"
#include "game.h"
#include "sound.h"
#include "texture.h"
#include "my_timer.h"

static const char *title = "SoVIMter";
static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color title_color = {23, 150, 52, 255};
static SDL_Color green_color = {120, 255, 120, 255};

int
lazy_load_main_menu(GAME *game) {
    int size;
    size = game->field.title_font;
    game_title = get_texture_from_text(game->font, game->renderer, title, size, &title_color);
    if (game_title == NULL) {
        SDL_Log("game_title cannot be initiazlied...");
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        menu_items[i] = get_texture_from_text(game->font, game->renderer, title, size, &title_color);
        if (menu_items[i] == NULL) {
            SDL_Log("menu_items[%d] cannot be initiazlied...", i);
            return 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        hover_menu_items[i] = get_texture_from_text(game->font, game->renderer, title, size, &title_color);
        if (hover_menu_items[i] == NULL) {
            SDL_Log("hover_menu_items[%d] cannot be initiazlied...", i);
            return 0;
        }
    }
    return 1;
}

int
main_menu_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_UP:
            case SDLK_W:
                game->main_menu->selected_item--;
                if (game->main_menu->selected_item < 0) {
                    game->main_menu->selected_item = game->main_menu->count - 1;
                }
                break;
            case SDLK_DOWN:
            case SDLK_S:
                game->main_menu->selected_item++;
                if (game->main_menu->selected_item >= game->main_menu->count) {
                    game->main_menu->selected_item = 0;
                }
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                play_sound(game->soundboard, 0);
                SDL_Delay(200);
                switch (game->main_menu->items[game->main_menu->selected_item].type) {
                    case menu_item_type_play:
                        run_a_game(game);
                        push_user_event(g_change_scene_event_type, game_state_gameplay);
                        break;
                    case menu_item_type_settings:
                        push_user_event(g_change_scene_event_type, game_state_setting);
                        break;
                    case menu_item_type_credits:
                        printf("Looking at credits...\n");
                        break;
                    case menu_item_type_exit:
                        SDL_Event quit_event;
                        quit_event.type = SDL_EVENT_QUIT;
                        SDL_PushEvent(&quit_event);
                        break;
                    default:
                        break;
                }
                break;
            case SDLK_ESCAPE:
                push_user_event(SDL_EVENT_QUIT, 0);
                break;
            default:
                break;
        }
    }
    return 1;
}

int
main_menu_update(GAME* game) {
    if (game->timer->start_timer == 1) {
        reset_timer(game->timer);
    }
    return 1;
}

int
main_menu_render(GAME* game) {
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

    int width = game->field.screen_width;
    int height = game->field.screen_height;
    int text_width, text_height;

    status = get_text_size(
        game->font,
        title,
        game->field.title_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
    }

    status = render_text(
        game->font,
        game->renderer,
        title, 
        game->field.title_font,
        &(SDL_Point){.x = (width - text_width) / 2, .y = height / 4},
        &title_color
    );
    if (status == 0) {
        SDL_Log("render_text failed...\n");
        return 0;
    }

    status = get_text_size(
        game->font,
        game->main_menu->items[0].text,
        game->field.item_font,
        NULL,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
    }

    int y_coord = height / 2 + game->field.title_padding;

    for (int i = 0; i < game->main_menu->count; i++) {
        SDL_Color *color;
        int font_size;
        int selected_height;
        int render_coord_y;
        if (i == game->main_menu->selected_item) {
            color = &green_color;
            font_size = game->field.hover_item_font;
            status = get_text_size(
                game->font,
                game->main_menu->items[i].text,
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
                game->main_menu->items[i].text,
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
        status = render_text(
            game->font,
            game->renderer,
            game->main_menu->items[i].text, 
            font_size,
            &dst_rect,
            color
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

SCENE main_menu_scene = {
    .handle_events = main_menu_event_handler,
    .update = main_menu_update,
    .render = main_menu_render
};
