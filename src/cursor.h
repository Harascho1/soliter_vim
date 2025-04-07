#ifndef CURSOR_H
#define CURSOR_H

#include "game.h"

typedef struct _CURSOR {
    SDL_FRect *cursor;
    POSITION *pos;
    int mode;
} CURSOR;

CURSOR* create_cursor(int col, int row);
void destroy_cursor(CURSOR *cursor);

#endif