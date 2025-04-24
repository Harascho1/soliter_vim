#include "texture.h"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_render.h"

SDL_Texture* create_texture_from_image(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        SDL_Log("IMG_Load failed.\n");
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        return NULL;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_DestroySurface(surface);
    return texture;
}
