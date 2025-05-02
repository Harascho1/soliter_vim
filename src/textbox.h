#ifndef TEXTBOX_H
#define TEXTBOX_H

typedef struct _TEXTBOX {
    int max_caracters;
    char *string;
} TEXTBOX;

TEXTBOX* create_textbox(int n);
void destroy_textbox(TEXTBOX *textbox);
int insert_text(TEXTBOX *textbox, const char *c);
int delete_text(TEXTBOX *textbox);
int have_requaried_num_of_characters(TEXTBOX *textbox, int num);

#endif
