#include "menu.h"

MENU* create_menu(const char* const* texts, const unsigned int count) {
  MENU* menu = (MENU*)malloc(sizeof(MENU));

  menu->items = (MENU_ITEM*)malloc(sizeof(MENU_ITEM) * count);
  if (menu->items == NULL) {
    free(menu);
    return NULL;
  }
  menu->count = count;
  menu->selected_item = 0;

  for (int i = 0; i < count; i++) {
    menu->items[i].text = (char*)malloc(strlen(texts[i]) + 1);
    const uint32_t size = sizeof(menu->items[i].text);
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
