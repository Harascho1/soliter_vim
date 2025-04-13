#include "cursor.h"

CURSOR*
create_cursor(int col, int row) {
    CURSOR *cursor = SDL_malloc(sizeof(CURSOR));
    
    cursor->pos = SDL_malloc(sizeof(POSITION));
    cursor->pos->col = col;
    cursor->pos->row = row;

    cursor->cursor = SDL_malloc(sizeof(SDL_FRect));
    cursor->cursor->x = padding_of_card / 2 - padding_of_card / 4;
    cursor->cursor->y = padding_of_card * 2 + card_height - padding_of_card / 4;
    cursor->cursor->w = g_card_width + padding_of_card / 2;
    cursor->cursor->h = card_height + padding_of_card / 2;

    cursor->mode = 0;

    return cursor;
}

void
destroy_cursor(CURSOR *cursor) {
    if (cursor != NULL) {
        SDL_free(cursor->cursor);
        SDL_free(cursor->pos);
        SDL_free(cursor);
    }
}

int
set_a_flag(CURSOR *cursor, int flag) {
    if (flag == 0) {
        cursor->mode = cursor->mode & ~1;
    } else {

        cursor->mode = cursor->mode & ~flag;
    }
    cursor->mode += flag;

}

int
have_a_flag(CURSOR *cursor, int flag) {
    if (cursor->mode & flag) {
        return 1;
    }
    return 0;
}

int
delete_hover_flag(CURSOR *cursor) {
    cursor->mode = cursor->mode % CURSOR_HOVER_1;
}

int
have_number_hover(CURSOR *cursor) {
    return (cursor->mode % CURSOR_HOVER_10) / CURSOR_HOVER_1;
}