#ifndef FIELD_H
#define FIELD_H

#include "font.h"

typedef struct _FIELD {
    int screen_width;
    int screen_height;
    int screen_padding;

    int card_padding;
    int card_width;
    int card_height;

    int title_font;
    int item_font;
    int hover_item_font;

    int title_padding;
    int item_padding;
}FIELD;

int load_field(FIELD *field, int screen_width, int screen_height, FONT *font);

#endif
