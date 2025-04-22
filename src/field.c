#include "field.h"

int
load_field(FIELD *field, int screen_width, int screen_height) {
    field->screen_height = screen_height;
    field->screen_width = screen_width;
    return 1;
}
