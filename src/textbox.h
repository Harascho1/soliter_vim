#ifndef TEXTBOX_H
#define TEXTBOX_H

typedef struct _TEXTBOX {
    int max_caracters;
    char *string;
} TEXTBOX;

TEXTBOX* create_textbox(int n);
void destroy_textbox(TEXTBOX *textbox);
bool insert_text(const TEXTBOX *textbox, const char *c);
bool delete_text(const TEXTBOX *textbox);
bool have_requaried_num_of_characters(const TEXTBOX *textbox, int num);

#endif
