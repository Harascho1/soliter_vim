#include "menu.h"

MENU* create_menu(const char* const* texts, const unsigned int count) {
  MENU* menu = SDL_malloc(sizeof(MENU));

  menu->items = SDL_malloc(sizeof(MENU_ITEM) * count);
  if (menu->items == NULL) {
    free(menu);
    return NULL;
  }
  menu->count = count;
  menu->selected_item = 0;

  for (int i = 0; i < count; i++) {
    const uint32_t size = sizeof(char)*(strlen(texts[i])+1);
    menu->items[i].text = SDL_malloc(size);
    strlcpy(menu->items[i].text, texts[i], size);
    menu->items[i].state = menu_item_not_selected;
    menu->items[i].type = i;
  }

  return menu;
}

void destroy_menu(MENU* menu) {
  if (menu == NULL) {
    return;
  }
  for (int i = 0; i < menu->count; i++) {
    SDL_free(menu->items[i].text);
  }
  SDL_free(menu->items);
  SDL_free(menu);
}
