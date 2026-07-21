#include "../game.h"
#include "../res/font.h"
#include "../res/res.h"
#include "SDL3/SDL_log.h"

SDL_Texture* tex_top_10_scores[10];

bool scores_lazy_load(const GAME* game) {
  FILE* saves = fopen(paths.bins.save, "rb+");
  if (saves == NULL) {
    SDL_Log("Unable to open file");
    return true;
  }

  char* scores_txt[10];
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
      break;
    }
    SDL_Log("Size in bytes of score buffer: %d\n", pointer);
    fseek(saves, prev_pointer, SEEK_SET);
    scores_txt[i] = SDL_malloc(sizeof(char) * pointer);
    const int status = (int)fread(scores_txt[i], sizeof(char), pointer, saves);
    if (!status) {
      fclose(saves);
      SDL_Log("fread error\n");
      return false;
    }
    scores_txt[i][pointer - 1] = '\0';
    SDL_Log("Score buffer is: %s", scores_txt[i]);
    const uint32_t size = SDL_strlen(scores_txt[i]) + 1;
    if (strlcpy(scores_txt[i], scores_txt[i], size) == 0) {
      fclose(saves);
      SDL_Log("strcpy error...");
      return false;
    }
    prev_pointer += pointer;
  }

  for (int j = 0; j < i; j++) {
    tex_top_10_scores[j] = get_texture_from_text(
      game->font, game->renderer, scores_txt[j], fonts.text_font, &colors.white
    );
  }
  fclose(saves);
  return true;
}

void scores_lazy_destroy() {
  for (int i = 0; i < 10; i++) {
    SDL_DestroyTexture(tex_top_10_scores[i]);
  }
}

bool scores_event_hendler(GAME* game, const SDL_Event* event) {
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

bool scores_update(const GAME* game) {
  return true;
}

bool scores_render(GAME* game) {
  bool status = SDL_RenderClear(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderClear error: %s", SDL_GetError());
    return false;
  }

  status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
  if (!status) {
    SDL_Log("SDL_RenderTexture error: %s", SDL_GetError());
    return false;
  }

  for (int i = 0; i < 10; i++) {
    if (tex_top_10_scores[i] == NULL) {
      break;
    }

    float tex_width;
    status = SDL_GetTextureSize(tex_top_10_scores[0], &tex_width, NULL);

    if (!status) {
      SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
      SDL_DestroyTexture(tex_top_10_scores[0]);
      return false;
    }

    SDL_FPoint pt = {
      .x = (resolution.width - tex_width) / 2,
      .y = fonts.text_height,
    };

    status = render_text(game->renderer, tex_top_10_scores[i], &pt);
    if (!status) {
      SDL_Log("render_text error: %s", SDL_GetError());
      return status;
    }
  }

  status = SDL_RenderPresent(game->renderer);
  if (!status) {
    SDL_Log("SDL_RenderPresent error: %s", SDL_GetError());
    return false;
  }
  return true;
}

SCENE scores_scene = {
  .handle_events = scores_event_hendler,
  .update = scores_update,
  .render = scores_render,
  .lazy_load = scores_lazy_load,
  .lazy_destroy = scores_lazy_destroy
};
