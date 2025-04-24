#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "font.h"
#include "card.h"
#include "cursor.h"
#include "texture.h"
#include "my_timer.h"
#include "field.h"

extern Uint32 g_change_scene_event_type;

typedef enum {
    game_state_main_menu = 0,
    game_state_gameplay,
    game_state_game_over,
    game_state_setting,
    game_state_macro,
    game_state_option,
    game_state_max
} GAME_STATE;

enum {
    number_of_cards_in_row = 7,
};

typedef struct _CURSOR CURSOR;

typedef struct _GAME{
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    MENU            *main_menu;
    MENU            *game_over_menu;
    MENU            *setting_menu;
    FONT            *font;
    SDL_Texture     *background_texture;
    SDL_Texture     *menu_texture;
    DECK            *deck;
    FIELD           field;
    CURSOR          *cursor;
    MY_TIMER        *timer;
} GAME;

typedef struct _RESOULUTION {
    int width;
    int height;
} RESOLUTION;

typedef int (*scene_handle_events_t)(GAME* game, const SDL_Event *event);
typedef int (*scene_update_t)(GAME *game);
typedef int (*scene_render_t)(GAME *game);

typedef struct _SCENE {
    scene_handle_events_t handle_events;
    scene_update_t update;
    scene_render_t render;
} SCENE;

extern SCENE main_menu_scene;
extern SCENE gameplay_scene;
extern SCENE game_over_menu_scene;
extern SCENE setting_scene;
extern SCENE macro_setting_scene;
extern SCENE option_setting_scene;

extern int game_update;
extern int g_game_win;

int game_init(GAME* game, const char *title, const RESOLUTION *res);
void game_quit(GAME* game);

int reload_window(GAME *game);
int load_game_field(DECK *game, FIELD *field);
void run_a_game(GAME *game);
void save_score(GAME *game);

int push_user_event(Uint32 type, Sint32 code);

#endif // GAME_H
