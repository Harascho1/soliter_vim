#include "game.h"


static const char *title[2] = {
    "GAME OVER",
    "YOU WIN"
};

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
game_over_menu_update(GAME *game) {
    if (game->timer->start_timer == 1) {
        reset_timer(game->timer);
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

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        printf("ZASTOOOOO\n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);
    if (status == 0) {
        SDL_Log("SDL_GetWindowSizeInPixels failed: %s\n", SDL_GetError());
        return 0;
    }
    int text_width, text_height;

    status = get_text_size(
        game->font,
        title[g_game_win],
        title_size,
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
        title[g_game_win], 
        title_size,
        &(SDL_Point){.x = (width - text_width) / 2, .y = height / 4},
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        SDL_Log("render_text failed...\n");
        return 0;
    }

    status = get_text_size(
        game->font,
        game->game_over_menu->items[0].text,
        standard_font_size,
        NULL,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size failed...\n");
        return 0;
    }

    int selected_text_height = text_height + 10;

    for (int i = 0; i < game->game_over_menu->count; i++) {

        SDL_Color color = {255, 255, 255, 255};
        int font_size;
        if (i == game->game_over_menu->selected_item) {
            color.r = 150;
            color.g = 255;
            color.b = 150;
            font_size = selected_font_size;
        } else {
            font_size = standard_font_size;
        }

        status = get_text_size(
            game->font,
            game->game_over_menu->items[i].text,
            font_size,
            &text_width,
            NULL
        );
        if (status == 0) {
            SDL_Log("get_text_size failed...\n");
            return 0;
        }
        SDL_Point dst_rect = {
            .x = (width - text_width) / 2,
            .y = (height - text_height) / 2 + i * (text_height + 10) 
        };
        status = render_text(
            game->font,
            game->renderer,
            game->game_over_menu->items[i].text, 
            font_size,
            &dst_rect,
            &color
        );
        if (status == 0) {
            SDL_Log("render_text failed...\n");
            return 0;
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

SCENE game_over_menu_scene = {
    .handle_events = game_over_menu_event_handler,
    .update = game_over_menu_update,
    .render = game_over_menu_render
};