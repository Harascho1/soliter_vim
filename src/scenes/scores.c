#include "../game.h"
#include "../res/font.h"
#include "SDL3/SDL_log.h"

SDL_Texture *tex_top_10_scores[10];
static int text_width;

bool
scores_lazy_load(const GAME *game) {
    FILE *saves = fopen("assets/bin/saves.bin", "rb+");
    if (saves == NULL) {
        SDL_Log("Unable to open file");
        return 1;
    }
    SDL_Color white_color = {255, 255, 255, 255};
    int status;

    char *scores_txt[10];
    int prev_pointer = 0;
    int i;
    for (i = 0; i < 10; i++) {
        int pointer = 0;
        char byte;
        fseek(saves, prev_pointer, SEEK_SET);
        while (fread(&byte, sizeof(char), 1, saves)) {
            pointer++;
            if (byte == '\0') {
                break;
            }
        }
        if (byte == EOF || pointer == 0) {
            SDL_Log("dobar je kod && pointer = %d\n", pointer);
            break;
        }
        SDL_Log("velicana stringa je %d\n", pointer); 
        fseek(saves, prev_pointer, SEEK_SET);
        scores_txt[i] = (char*)SDL_malloc(sizeof(char) * pointer);
        status = fread(scores_txt[i], sizeof(char), pointer, saves);
        if (status == 0) {
            SDL_Log("fread error\n");
        }
        scores_txt[i][pointer - 1] = '\0';
        SDL_Log("string je %s", scores_txt[i]); 
        SDL_Log("dobar je kod && i = %d\n", i);
        if (strcpy(scores_txt[i], scores_txt[i]) == NULL) {
            SDL_Log("jebeni strcpy ne radi...");
        }
        SDL_Log("dobar je kod\n");
        prev_pointer += pointer;
    }
    int font_size = game->field.text_font;

    status = get_text_size(
        game->font, 
        scores_txt[0], 
        font_size, 
        &text_width, 
        NULL
    );
    if (status == 0) {
        SDL_Log("get_text_size error in lazy load..\n");
        return 0;
    }

    for (int j = 0; j < i; j++) {
        tex_top_10_scores[j] = get_texture_from_text(
          game->font, game->renderer, scores_txt[j], font_size, &white_color);
    }
    fclose(saves);
    return 1;
}

void
scores_lazy_destroy(GAME *game) {
    for (int i = 0; i < 10; i++) {
        SDL_DestroyTexture(tex_top_10_scores[i]);
    }
}

bool
scores_event_hendler(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                push_user_event(g_change_scene_event_type, game_state_main_menu);
                break;
            default:
                break;
        }
    }
    return 1;
}

bool
scores_update(const GAME *game) {
    return true;
}

bool
scores_render(GAME *game) {
    int status = 0;

    status = SDL_RenderClear(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderClear error: %s", SDL_GetError());
        return 0;
    }

    status = SDL_RenderTexture(
        game->renderer, 
        game->background_texture, 
        NULL,
        NULL
    );
    if (status == 0) {
        SDL_Log("SDL_RenderTexture error: %s", SDL_GetError());
        return 0;
    }

    int text_height;
    status = get_text_size(
        game->font, 
        "normal mode", 
        game->field.text_font, 
        NULL, 
        &text_height
    );
    if (status == false) {
        return status;
    }

    SDL_Point pt = {
        .x = (game->field.screen_width - text_width) / 2,
        .y = 0 + text_height
    };

    for (int i = 0; i < 10; i++) {
        if (tex_top_10_scores[i] == NULL) {
            break;
        }
        status = render_text(
            game->renderer, 
            tex_top_10_scores[i], 
            &pt
        );
        pt.y += game->field.text_padding + text_height;
    }

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent error: %s", SDL_GetError());
        return 0;
    }
    return 1;
}

SCENE scores_scene = {
    .handle_events = scores_event_hendler,
    .update = scores_update,
    .render = scores_render,
    .lazy_load = scores_lazy_load,
    .lazy_destroy = scores_lazy_destroy
};

