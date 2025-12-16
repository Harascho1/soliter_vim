#include "clickable_element.h"
#include "SDL3/SDL_stdinc.h"

CLICKABLE_ELEMENT *create_clickable_element(SDL_FRect box,
                                            action_callback function) {
  CLICKABLE_ELEMENT *el = SDL_malloc(sizeof(CLICKABLE_ELEMENT));
  el->box = box;
  el->action = function;

  return el;
}

void destroy_clickable_element(CLICKABLE_ELEMENT *element) {
  SDL_free(element);
}
