#ifndef FIELD_H
#define FIELD_H

typedef struct _FIELD {
    int screen_width;
    int screen_height;
}FIELD;

int load_field(FIELD *field, int screen_width, int screen_height);

#endif
