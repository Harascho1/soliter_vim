// TODO moze da izabere da li hoce auto_sort da ukljuci
// TODO da promeni rezoluciju
// TODO SMANJI MOGUCU MUZIKU
// TODO mozes da menjas karte

#include "game.h"

static char* normal_mode = "NORMAL mode:\n\nW-A-S-D/ARROWS for moving cursor;\nSPACE/ENTER for selecting and placing a card;\nESC is for exiting a game";
static char* fly_mode = "FLY mode:\n\nUse your numbers to jump to the card. For an example:\nby typing 32 you will jump to 2nd card in the 3rd column.\nFirst number is column number and second is card in that column.\nIf you have more than 9 card in column you will type\n[column]00 for 10th card [column]01 for 11th card and so on.\nSPACE/ENTER for selecting and placing card.\nQ - Drawing cards from a deck.\nTAB - JUMP to the drawn card press.";
static char* universal_rule = "Going to the NORMAL mode is by pressing X\nGoint to the FLY mode is by pressing C";
static int width, height;

int
setting_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type != SDL_EVENT_KEY_DOWN) {
        return 1;
    }
    switch (event->key.key) {
        case SDLK_W:
        case SDLK_UP:
        case SDLK_D:
        case SDLK_RIGHT:
            if (game->setting_menu->selected_item < settings_item_type_max - 1) {
                game->setting_menu->selected_item++;
            }
            break;
        case SDLK_S:
        case SDLK_DOWN:
        case SDLK_A:
        case SDLK_LEFT:
            if (game->setting_menu->selected_item > 0) {
                game->setting_menu->selected_item = game->setting_menu->selected_item - 1;
            }
            break;
        case SDLK_KP_ENTER:
        case SDLK_SPACE:
        case SDLK_RETURN:
            switch (game->setting_menu->items[game->setting_menu->selected_item].type) {
                case settings_item_type_options:
                    push_user_event(g_change_scene_event_type, game_state_option);
                    break;
                case settings_item_type_cancel:
                    push_user_event(g_change_scene_event_type, game_state_main_menu);
                    break;
                case settings_item_type_macro:
                    push_user_event(g_change_scene_event_type, game_state_macro);
                    break;
                default:
                    break;
            }
            break;
        case SDLK_E:
            break;
        default:
            break;
    }
    return 1;
}

int
setting_update(GAME *game) {
    return 1;
}

int
render_guide(GAME *game) {
    int status;

    int text_width, text_height;
    status = set_font_size(
        game->font,
        game->field.text_font
    );
    if (status == 0) {
        SDL_Log("get_text_size error\n");
        return 0;
    }
    status = render_wrapped_text(
        game->font,
        game->renderer,
        normal_mode,
        game->field.text_font,
        &(SDL_Point) {
            .x = game->field.screen_padding,
            .y = game->field.title_padding,
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        SDL_Log("render_text error\n");
        return 0;
    }

    status = render_wrapped_text(
        game->font,
        game->renderer,
        universal_rule,
        game->field.text_font,
        &(SDL_Point) {
            .x = game->field.screen_padding,
            .y = height / 1.5 + game->field.title_padding 
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        SDL_Log("render_wrapped_text error\n");
        return 0;
    }

    status = render_wrapped_text(
        game->font,
        game->renderer,
        fly_mode,
        game->field.text_font,
        &(SDL_Point) {
            .x = game->field.screen_padding,
            .y = height / 4 + game->field.text_padding 
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        SDL_Log("render_wrapped_text error\n");
        return 0;
    }

    status = get_text_size(
        game->font,
        game->setting_menu->items[0].text,
        game->field.item_font,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    int selected_text_width, selected_text_height;
    status = get_text_size(
        game->font,
        game->setting_menu->items[0].text,
        game->field.hover_item_font,
        &selected_text_width,
        &selected_text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    SDL_Color selected_color = {150, 255, 150, 255};
    SDL_Color not_selected_color = {255, 255, 255, 255};

    int not_selected_width = game->field.screen_padding;
    int selected_width = game->field.screen_padding;
    int y_pos = height - text_height - game->field.screen_padding * 2;

    SDL_Point not_selected_point  = (SDL_Point) {
        .x = not_selected_width,
        .y = y_pos
    };
    SDL_Point selected_point  = (SDL_Point) {
        .x = selected_width,
        .y = y_pos
    };
    for (int i = 0; i < game->setting_menu->count; i++) {
        SDL_Color *color = &not_selected_color;
        SDL_Point* point = &not_selected_point;
        int font_size = game->field.item_font;

        if (i == game->setting_menu->selected_item) {
            color = &selected_color;
            font_size = game->field.hover_item_font;
            point = &selected_point;
        }

        status = render_text(
            game->font,
            game->renderer,
            game->setting_menu->items[i].text,
            font_size,
            point,
            color
        );
        if (status == 0) {
            SDL_Log("render_text error...\n");
        }
        if (i != game->setting_menu->count - 1) {
            status = get_text_size(
                game->font,
                game->setting_menu->items[i + 1].text,
                game->field.item_font,
                &text_width,
                &text_height
            );
            if (status == 0) {
                SDL_Log("get_text_size error...\n");
                return 0;
            }
            status = get_text_size(
                game->font,
                game->setting_menu->items[i + 1].text,
                game->field.hover_item_font,
                &selected_text_width,
                &selected_text_height
            );
            if (status == 0) {
                SDL_Log("get_text_size error...\n");
                return 0;
            }
        }
        not_selected_width += (width - text_width) / 2 - game->field.screen_padding;
        selected_width += (width - selected_text_width) / 2 - game->field.screen_padding;
        not_selected_point.x  = not_selected_width;
        selected_point.x  = selected_width;
    }

    return status;
}

int
setting_render(GAME *game) {


    int status = 0;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
    if (status == 0) {
        SDL_Log("SDL_GetWindowSizeInPixels error: %s\n", SDL_GetError());
        return status;
    }

    if (game->background_texture == NULL) {
        SDL_Log("background is NULL\n");
        return 0;
    }

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
        return status;
    }

    status = render_guide(game);
    if (status == 0) {
        SDL_Log("render_guide error\n");
        return status;
    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent error %s\n", SDL_GetError());
        return 0;
    }

    return status;
}

SCENE setting_scene = {
    .handle_events = setting_event_handler,
    .update = setting_update,
    .render = setting_render 
};
