#ifndef TEXTURE_H
#define TEXTURE_H

// Returns SDL_Texture for given renderer and path
SDL_Texture *create_texture_from_image(SDL_Renderer *renderer,
                                       const char *path);

#endif
