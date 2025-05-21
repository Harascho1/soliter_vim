#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "game.h"
#include "textbox.h"

extern char buffer[10];
extern TEXTBOX *textbox;

int gameplay_lazy_load(GAME *game);
void gameplay_lazy_destroy();
int change_cursor_frame(GAME *game);
CARD* top_deck_card(DECK *deck);
CARD* next_deck_card(DECK *deck);
CARD* draw_next_card(DECK *deck);
int reveal_card_below(GAME *game);
int selected_card(DECK *deck, CARD **selected_cards);
int place_king(CARD **card, int num, CURSOR *cursor, FIELD *field);
int place_a_card(GAME *game);
int place_a_card(GAME *game);
int select_a_card(GAME *game);
int change_cursor_frame(GAME *game);
int go_to_invisible_card(GAME *game, int col);
int interact(GAME *game);
int normal_select_mode(GAME *game, const SDL_Event *event);
int fly_mode(GAME *game, const SDL_Event *event);
int gamaplay_event_handler(GAME *game, const SDL_Event *event);
int gameplay_update(GAME* game);
int render_commands(GAME *game);
int render_cursor(GAME *game);
int sorted_card_render(GAME *game);
int gameplay_render(GAME* game);

#endif
