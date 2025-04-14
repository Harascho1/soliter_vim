#ifndef FONT_H
#define FONT_H

typedef struct _FONT {
    TTF_Font *font;
    int size;
} FONT;

FONT* create_font(const char *path);
void destroy_font(FONT *font);
int get_text_size(FONT *font, const char *text, int size, int *width, int *height);
int set_font_size(FONT *font, int size);
int render_text(FONT *font, SDL_Renderer *render, const char *text, int size, SDL_Point *point, SDL_Color *color);
int render_wrapped_text(FONT *font, SDL_Renderer *render, const char *text, int size, SDL_Point *point, SDL_Color *color);

#endif // FONT_H