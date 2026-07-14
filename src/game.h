#ifndef GAME_H
#define GAME_H

#include "SDL3/SDL_stdinc.h"
#include "card.h"
#include "cursor.h"
#include "menu.h"
#include "my_timer.h"
#include "res/font.h"
#include "res/sound.h"

extern Uint32 g_change_scene_event_type;
extern Uint8 game_c;

typedef enum {
  game_state_main_menu = 0,
  game_state_gameplay,
  game_state_game_over,
  game_state_setting,
  game_state_scores,
  game_state_macro,
  game_state_option,
  game_state_max
} GAME_STATE;

enum {
  number_of_cards_in_row = 7,
};

typedef struct _CURSOR CURSOR;

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  MENU* main_menu;
  MENU* game_over_menu;
  MENU* setting_menu;
  FONT* font;
  SDL_Texture* background_texture;
  SDL_Texture* menu_texture;
  MIX_Mixer* mixer;
  SOUNDBOARD* soundboard;
  DECK* deck;
  CURSOR* cursor;
  MY_TIMER* timer;
} GAME;

typedef bool (*scene_handle_events_t)(GAME* game, const SDL_Event* event);
typedef bool (*scene_update_t)(const GAME* game);
typedef bool (*scene_render_t)(GAME* game);
typedef bool (*lazy_load_t)(const GAME* game);
typedef void (*lazy_destroy_t)();

typedef struct {
  scene_handle_events_t handle_events;
  scene_update_t update;
  scene_render_t render;
  lazy_load_t lazy_load;
  lazy_destroy_t lazy_destroy;
} SCENE;

extern SCENE main_menu_scene;
extern SCENE gameplay_scene;
extern SCENE game_over_menu_scene;
extern SCENE setting_scene;
extern SCENE scores_scene;
extern SCENE macro_setting_scene;
extern SCENE option_setting_scene;

extern int game_update;
extern int g_game_win;

bool game_init(GAME* game, const char* title);
void game_quit(const GAME* game);

bool reload_window(const GAME* game);
bool load_game_field(DECK* deck);
void run_a_game(GAME* game);
void save_score(const GAME* game, const char* name);
bool render_counting_time(const GAME* game);
int push_user_event(Uint32 type, Sint32 code);

#endif // GAME_H
