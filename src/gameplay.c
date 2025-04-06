#include "game.h"
#include "texture.h"

int
gamaplay_event_handler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                SDL_Log("Game Overing...\n");
                push_user_event(g_change_scene_event_type, game_state_game_over);
                break;
            default:
                break;
        }
    }
    return 1;
}

int
gameplay_update(GAME* game) {
    return 1;
}

int render_deck(GAME *game) {

    return 1;
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

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        printf("ZASTOOOOO\n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    //TODO - posle zameni ovo
    status = render_card(
        game->renderer,
        &(CARD){value_ace, suit_spades, visible, 0},
        &(SDL_Point){10, 10}
    );

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