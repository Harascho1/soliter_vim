#include "textbox.h"
#include "SDL3/SDL_stdinc.h"
#include <ctype.h>
#include <string.h>
#include <uchar.h>

TEXTBOX*
create_textbox(int n) {
    TEXTBOX *textbox = (TEXTBOX*)SDL_malloc(sizeof(TEXTBOX));

    textbox->max_caracters = n;
    textbox->string = (char*)SDL_malloc(sizeof(char) * (n + 1));
    memset(textbox->string, 0, sizeof(char) * (n + 1));

    return textbox;
}

int
have_requaried_num_of_characters(TEXTBOX *textbox, int num) {
    if (strlen(textbox->string) >= num) {
        return 1;
    }
    return 0;
}

void
destroy_textbox(TEXTBOX *textbox) {
    SDL_free(textbox->string);
    SDL_free(textbox);
}

int
insert_text(TEXTBOX *textbox, const char *c) {
    if (strlen(textbox->string) >= textbox->max_caracters) {
        return 0;
    }

    if (strcpy(" ", c) == 0) {
        return 1;
    }

    char chr = SDL_toupper(*c);
    strcat(textbox->string, &chr);
    SDL_Log("string je %s\n", textbox->string);
    return 1;
}

int
delete_text(TEXTBOX *textbox) {
    if (strlen(textbox->string) == 0) {
        return 0;
    }

    int index = strlen(textbox->string) - 1;
    textbox->string[index] = 0;
    return 1;
}
