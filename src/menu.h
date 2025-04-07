#ifndef MENU_H
#define MENU_H

enum MENU_ITEM_STATE {
    menu_item_not_selected = 0,
    menu_item_selected
};

enum MAIN_MENU_ITEM_TYPE {
    menu_item_type_play,
    menu_item_type_settings,
    menu_item_type_credits,
    menu_item_type_exit,
    menu_item_type_max
};

enum GAME_OVER_ITEM_TYPE {
    game_over_item_type_restart,
    game_over_item_credits,
    game_over_item_type_exit,
    game_over_item_type_max
};

typedef struct _MENU_ITEM {
    char *text;
    int state;
    int type;
} MENU_ITEM;

typedef struct _MENU {
    MENU_ITEM *items;
    int selected_item;
    int count;
} MENU;

MENU* create_menu(const char *text[], int count);
void destroy_menu(MENU *menu);


#endif