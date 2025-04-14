// TODO moze da izabere da li hoce auto_sort da ukljuci
// TODO da promeni rezoluciju
// TODO SMANJI MOGUCU MUZIKU
// TODO mozes da menjas karte

#include "game.h"



static char* normal_mode = "NORMAL mode:\n\nW-A-S-D/ARROWS for moving cursor;\nSPACE/ENTER for selecting and placing a card;\nESC is for exiting a game";
static char* fly_mode = "FLY mode:\n\nUse your numbers to jump to the card. For an example:\nby typing 32 you will jump to 2nd card in the 3rd collumn.\nFirst number is collumn number and second is card in that collumn.\nIf you have more than 9 card in collumn you will type\n[collumn]00 for 10th card [collumn]01 for 11th card and so on.\nSPACE/ENTER for selecting and placing card.\nQ - Drawing cards from a deck.\nTAB - JUMP to the drawn card press.";
static char* universal_rule = "Going to the NORMAL mode is by pressing X\nGoint to the FLY mode is by pressing C";
static int width, height;

int
setting_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type != SDL_EVENT_KEY_DOWN) {
        return 1;
    }
    switch (event->key.key) {
        case SDLK_ESCAPE:
            push_user_event(g_change_scene_event_type, game_state_main_menu);
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
        standard_font_size
    );
    if (status == 0) {
        SDL_Log("get_text_size error\n");
        return 0;
    }
    status = render_wrapped_text(
        game->font,
        game->renderer,
        normal_mode,
        guide_font_size,
        &(SDL_Point) {
            .x = padding_of_card,
            .y = padding_of_card
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
        guide_font_size,
        &(SDL_Point) {
            .x = padding_of_card,
            .y = height / 1.5 + padding_of_card
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
        fly_mode,
        guide_font_size,
        &(SDL_Point) {
            .x = padding_of_card,
            .y = height / 4 + padding_of_card
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        SDL_Log("render_text error\n");
        return 0;
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