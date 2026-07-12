#ifndef CLICKABLE_ELEMENT_H
#define CLICKABLE_ELEMENT_H

#include "SDL3/SDL_rect.h"

typedef void (*action_callback)(void);

typedef struct _CLICKABLE_ELEMENT {
  SDL_FRect box;
  action_callback action;
  // TODO: maybe add hover variable
} CLICKABLE_ELEMENT;

CLICKABLE_ELEMENT* create_clickable_element(SDL_FRect box, action_callback function);
void destroy_clickable_element(CLICKABLE_ELEMENT* element);

#endif
