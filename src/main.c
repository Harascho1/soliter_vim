#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "game.h"
#include <stdbool.h>
static GAME_STATE g_current_game_state = game_state_main_menu;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

static SCENE* g_game_scenes[] = {
    &main_menu_scene,
    &gameplay_scene,
    &game_over_menu_scene,
    &setting_scene,
    &scores_scene,
    &macro_setting_scene,
    &option_setting_scene
};

static int g_scene_fps[] = {
    30,
    30,
    30,
    30,
    30,
    30,
    30
};

static int
is_period_pass(int period, int last_update_time) {

    int current_time = SDL_GetTicks();
    if ((current_time - last_update_time) >= period) {
        //SDL_Log("fps = %f\n", (float)current_time - last_update_time);
        return true;
    }
    return false;
}

int
sld_init() {
    int status;

    status = SDL_InitSubSystem(SDL_INIT_EVENTS);
    if (status == 0) {
        SDL_Log("SDL_InitSubSystem failed: %s\n", SDL_GetError());
        return status;
    }

    status = SDL_Init(SDL_INIT_VIDEO);
    if (status == 0) {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        return status;
    }

    status = SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (status == 0) {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        return status;
    }

    status = TTF_Init();
    if (status == 0) {
        SDL_Log("TTF_Init failed: %s\n", SDL_GetError());
        return status;
    }

    return status;
}

void cleanup() {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

int main() {

    int status;
    status = sld_init();
    if (status == 0) {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        return status;
    }

    GAME game;
    status = game_init(&game, "SoVIMter", &(RESOLUTION){900, 900});
    if (status == 0) {
        SDL_Log("game_init failed: %s\n", SDL_GetError());
        cleanup();
        return status;
    }

    SDL_Event event;
    int last_frame_time = 0;

    g_game_scenes[g_current_game_state]->lazy_load(&game);
    g_game_scenes[g_current_game_state]->render(&game);
    while (1) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_EVENT_PRIVATE0) {
                if (game.timer->time_elapsed >= 3600) {
                    g_game_scenes[g_current_game_state]->lazy_destroy();
                    g_current_game_state = game_state_game_over;

                    g_game_scenes[g_current_game_state]->lazy_load(&game);
                }
                g_game_scenes[g_current_game_state]->render(&game);
                continue;
            }
            if (event.type == SDL_EVENT_QUIT) {
                g_game_scenes[g_current_game_state]->lazy_destroy();
                break;
            } else if (event.type == g_change_scene_event_type) {
                g_game_scenes[g_current_game_state]->lazy_destroy();
                g_current_game_state = event.user.code;

                g_game_scenes[g_current_game_state]->lazy_load(&game);
                g_game_scenes[g_current_game_state]->update(&game);
                g_game_scenes[g_current_game_state]->render(&game);
            }
        }
        g_game_scenes[g_current_game_state]->handle_events(&game, &event);
        if (is_period_pass(1000 / g_scene_fps[g_current_game_state], last_frame_time) == 1) {
            g_game_scenes[g_current_game_state]->update(&game);
            g_game_scenes[g_current_game_state]->render(&game);
            last_frame_time = SDL_GetTicks();
        }
    }
    game_quit(&game);
    cleanup();
    return 0;
}
