#ifndef CURSOR_H
#define CURSOR_H

#include "card.h"

#define CURSOR_NORMAL_MODE 0x0
#define CURSOR_SELECT_MODE 0x1
#define CURSOR_FLY_MODE 0X2
// #define CURSOR_HOVER_H 0x10
// #define CURSOR_HOVER_J 0x20
// #define CURSOR_HOVER_K 0x30
// #define CURSOR_HOVER_L 0x40
#define CURSOR_HOVER_1 0x100
#define CURSOR_HOVER_2 0x200
#define CURSOR_HOVER_3 0x300
#define CURSOR_HOVER_4 0x400
#define CURSOR_HOVER_5 0x500
#define CURSOR_HOVER_6 0x600
#define CURSOR_HOVER_7 0x700
#define CURSOR_HOVER_8 0x800
#define CURSOR_HOVER_9 0x900
#define CURSOR_HOVER_10 0x1000

typedef struct _CURSOR {
  SDL_FRect* cursor;
  POSITION* pos;
  int mode;
} CURSOR;

CURSOR* create_cursor(int col, int row);
void destroy_cursor(CURSOR* cursor);

int have_a_flag(CURSOR* cursor, int flag);
int delete_hover_flag(CURSOR* cursor);
int have_number_hover(CURSOR* cursor);
int set_a_flag(CURSOR* cursor, int flag);

#endif
