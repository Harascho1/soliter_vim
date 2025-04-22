#include "SDL3/SDL_keycode.h"
#include "config.h"
#include "game.h"

static int selected_index = 0;
static SDL_Color white_color = {255, 255, 255, 255};
static SDL_Color green_color = {150, 255, 150, 255};

int
macro_settings_event_hendler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                push_user_event(g_change_scene_event_type, game_state_setting);
                break;
            case SDLK_W:
            case SDLK_UP:
                if (selected_index <= 0) {
                    break;
                }
                selected_index--;
                break;
            case SDLK_S:
            case SDLK_DOWN:
                if (selected_index > 15) {
                    break;
                }
                selected_index++;
                break;
            default:
                const char *character = SDL_GetKeyName(event->key.key);
                SDL_Log("Key: %s\n", character);
                break;
        }
    }
    return 1;
}

int
macro_settings_update(GAME *game) {
    return 1;
}

static char* text[14] = {
    "switch to normal mode",
    "selecting drawn card",
    "number 1",
    "number 2",
    "number 3",
    "num 4",
    "num 5",
    "num 6",
    "num 7",
    "num 8",
    "num 9",
    "num 0",
    "draw next card",
    "select card/cards"
};

static char title[] = "Macro";


int
macro_settings_render(GAME *game) {
    int status;
    if (game == NULL) {
        SDL_Log("game is NULL\n");
        return 0;
    }

    if (game->renderer == NULL) {
        SDL_Log("game->renderer is NULL\n");
        return 0;
    }
    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
    if (status == 0) {
        SDL_Log("SDL_GetWindowSizeInPixels error: %s\n", SDL_GetError());
        return status;
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

    int title_width, title_height;
    status = get_text_size(
        game->font,
        title,
        game->field.title_font,
        &title_width,
        &title_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    status = render_text(
        game->font,
        game->renderer,
        title,
        game->field.title_font,
        &(SDL_Point){
            .x = (width - title_width) / 2,
            .y = game->field.screen_padding
        },
        &white_color
    );
    if (status == 0) {
        SDL_Log("render_text error...\n");
        return 0;
    }


    int text_width, text_height;
    status = get_text_size(
        game->font,
        text[0],
        game->field.item_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    int y_pos = (title_height + game->field.screen_padding);
    int text_x_pos = game->field.screen_padding;
    int commands_x_pos = width - 4 * padding_of_card;
    int font = standard_font_size;
    for (int i = 0; i < 14; i++) {
        SDL_Color *color;
        int font = standard_font_size;
        int commands_y_pos;

        int commands_selected_height;
        if (selected_index == i) {
            status = get_text_size(
                game->font,
                commands_keys[i],
                selected_font_size,
                &text_width,
                &commands_selected_height
            );
            color = &green_color;
            font = selected_font_size;
            commands_y_pos = y_pos + (text_height - commands_selected_height) /2;
        } else {
            status = get_text_size(
                game->font,
                commands_keys[i],
                standard_font_size,
                &text_width,
                NULL
            );
            color = &white_color;
            font = standard_font_size;
            commands_y_pos = y_pos;
        }

        status = render_text(
            game->font,
            game->renderer,
            text[i],
            font,
            &(SDL_Point) {
                .x = text_x_pos,
                .y = commands_y_pos
            },
            &white_color
        );
        if (status == 0) {
            SDL_Log("render_text error...\n");
        }

        status = render_text(
            game->font,
            game->renderer,
            commands_keys[i],
            font,
            &(SDL_Point) {
                .x = commands_x_pos - text_width / 2,
                .y = commands_y_pos
            },
            color
        );
        if (status == 0) {
            SDL_Log("render_text error...\n");
        }

        //y_pos += text_height + padding_of_card / 2;
        y_pos += text_height + padding_of_card / 2;
    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}

SCENE macro_setting_scene = {
    .handle_events = macro_settings_event_hendler,
    .update = macro_settings_update,
    .render = macro_settings_render
};
