#ifndef FIELD_H
#define FIELD_H

#include "font.h"

typedef struct _FIELD {
    int screen_width;
    int screen_height;
    int screen_padding;
    int gameplay_screen_padding_width;
    int gameplay_screen_padding_height;

    int card_padding_width;
    int card_padding_height;
    int card_width;
    int card_height;
    int cursor_padding;

    int title_font;
    int item_font;
    int hover_item_font;
    int text_font;

    int title_padding;
    int item_padding;
    int text_padding;
}FIELD;

int load_field(FIELD *field, int screen_width, int screen_height, FONT *font);

#endif
