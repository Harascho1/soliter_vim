#include "font.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

FONT*
create_font(const char *path) {
    FONT *font = (FONT *)malloc(sizeof(FONT));
    if (font == NULL) {
        return NULL;
    }

    font->font = TTF_OpenFont(path, 24);
    if (font->font == NULL) {
        free(font);
        return NULL;
    }

    font->size = 24;

    return font;
}

void
destroy_font(FONT *font) {
    if (font == NULL) {
        return;
    }
    TTF_CloseFont(font->font);
    free(font);
}

int
set_font_size(FONT *font, int size) {
    if (font == NULL) {
        return 0;
    }
    TTF_SetFontSize(font->font, size);
    font->size = size;
    return 1;
}

int
get_text_size(FONT *font, const char* text, int size, int *width, int *height) {
    if (font == NULL) {
        return 0;
    }

    int status = set_font_size(font, size);
    if (status == 0) {
        return status;
    }

    status = TTF_GetStringSize(font->font, text, 0, width, height);
    if (status == 0) {
        return status;
    }
    return status;
}

SDL_Texture*
get_texture_from_text(FONT *font, SDL_Renderer *render, const char* text, int size, SDL_Color *color) {
    if (font == NULL || render == NULL || text == NULL) {
        return NULL;
    }

    int status = set_font_size(font, size);
    if (status == 0) {
        SDL_Log("set_font_size failed...\n");
        return NULL;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font->font, text, 0, *color);
    if (surface == NULL) { 
        SDL_Log("TTF_RenderText_Solid failed: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    if (texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        return NULL;
    }

    SDL_DestroySurface(surface);
    return texture;
}

int
render_text(SDL_Renderer* render, SDL_Texture *texture, SDL_Point *point) {
    int status;
    float tex_width, tex_height;

    if (render == NULL) {
        SDL_Log("render is NULL\n");
        return 0;
    }

    if (texture == NULL) {
        SDL_Log("texture is NULL\n");
        return 0;
    }

    status = SDL_GetTextureSize(texture, &tex_width, &tex_height);
    if (status == 0) {
        SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return 0;
    }

    status = SDL_RenderTexture(render, texture, NULL, &(SDL_FRect){point->x, point->y, tex_width, tex_height});
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return 0;
    }

    return status;
}

int
render_wrapped_text(FONT *font, SDL_Renderer *render, const char *text, int size, SDL_Point *point, SDL_Color *color) {
    if (font == NULL || render == NULL || text == NULL) {
        return 0;
    }

    int status = set_font_size(font, size);
    if (status == 0) {
        SDL_Log("set_font_size failed...\n");
        return status;
    }

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font->font, text, 0, *color, 0);
    if (surface == NULL) { 
        SDL_Log("TTF_RenderText_Solid failed: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    if (texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        return 0;
    }

    float tex_width, tex_height;
    status = SDL_GetTextureSize(texture, &tex_width, &tex_height);
    if (status == 0) {
        SDL_Log("SDL_GetTextureSize failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
        return 0;
    }

    status = SDL_RenderTexture(render, texture, NULL, &(SDL_FRect){point->x, point->y, surface->w, surface->h});
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
        return 0;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);

    return status;
}
