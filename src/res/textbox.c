#include "textbox.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include <ctype.h>
#include <string.h>
#include <uchar.h>

TEXTBOX* create_textbox(const int n) {
  TEXTBOX* textbox = (TEXTBOX*)SDL_malloc(sizeof(TEXTBOX));

  textbox->max_caracters = n;
  textbox->string = (char*)SDL_malloc(sizeof(char) * (n + 1));
  memset(textbox->string, 0, sizeof(char) * (n + 1));

  return textbox;
}

bool have_requaried_num_of_characters(const TEXTBOX* textbox, const int num) {
  if (strlen(textbox->string) >= num) {
    return true;
  }
  return false;
}

void destroy_textbox(TEXTBOX* textbox) {
  SDL_free(textbox->string);
  SDL_free(textbox);
}

bool insert_text(const TEXTBOX* textbox, const char* c) {
  if (strlen(textbox->string) >= textbox->max_caracters) {
    return false;
  }

  if (*c == ' ') {
    return true;
  }

  const char chr = (char)SDL_toupper(*c);
  textbox->string[strlen(textbox->string)] = chr;
  textbox->string[strlen(textbox->string) + 1] = 0;
  return true;
}

bool delete_text(const TEXTBOX* textbox) {
  if (strlen(textbox->string) == 0) {
    return false;
  }

  const unsigned int index = (unsigned int)strlen(textbox->string) - 1;
  textbox->string[index] = 0;
  return true;
}
