#ifndef MENU_H
#define MENU_H

enum MENU_ITEM_STATE {
    menu_item_not_selected = 0,
    menu_item_selected
};

enum SETTINGS_MENU_ITEM_TYPE {
    settings_item_type_options,
    settings_item_type_cancel,
    settings_item_type_macro,
    settings_item_type_max,
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

typedef struct {
    char *text;
    int state;
    int type;
} MENU_ITEM;

typedef struct {
    MENU_ITEM *items;
    int selected_item;
    int count;
} MENU;

MENU* create_menu(const char *text[], int count);
void destroy_menu(MENU *menu);


#endif
