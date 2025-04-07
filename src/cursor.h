#ifndef CURSOR_H
#define CURSOR_H

#include "game.h"

#define CURSOR_NORMAL_MODE 0x00000000
#define CURSOR_SELECT_MODE 0x00000001
#define CURSOR_FLY_MODE 0X0000002
#define CURSOR_HOVER_1 0x00000100
#define CURSOR_HOVER_2 0x00000200
#define CURSOR_HOVER_3 0x00000300
#define CURSOR_HOVER_4 0x00000400
#define CURSOR_HOVER_5 0x00000500
#define CURSOR_HOVER_6 0x00000600
#define CURSOR_HOVER_7 0x00000700

typedef struct _CURSOR {
    SDL_FRect *cursor;
    POSITION *pos;
    int mode;
} CURSOR;

CURSOR* create_cursor(int col, int row);
void destroy_cursor(CURSOR *cursor);

int have_a_flag(CURSOR *cursor, int flag);
int delete_hover_flag(CURSOR *cursor, int flag);

#endif