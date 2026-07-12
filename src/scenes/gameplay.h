#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "../game.h"
#include "../textbox.h"

extern char buffer[10];
extern TEXTBOX *textbox;

bool gameplay_lazy_load(const GAME *game);
void gameplay_lazy_destroy();
void change_cursor_frame(const GAME *game);
CARD *top_deck_card(DECK *deck);
CARD *next_deck_card(DECK *deck);
CARD *draw_next_card(DECK *deck);
int reveal_card_below(const GAME *game);
// Output a selected_cards or selected cards of 1 element if one card is
// selected and returns a number of selected cards
int selected_card(DECK *deck, CARD **selected_cards);
bool place_king(CARD **card, int num, const CURSOR *cursor, const FIELD *field);
int place_a_card(const GAME *game);
int select_a_card(GAME *game);
int go_to_invisible_card(GAME *game, int col);
int interact(GAME *game);
int normal_select_mode(GAME *game, const SDL_Event *event);
int fly_mode(GAME *game, const SDL_Event *event);
bool gameplay_event_handler(GAME *game, const SDL_Event *event);
bool gameplay_update(const GAME *game);
bool render_commands(GAME *game);
bool render_cursor(GAME *game);
int sorted_card_render(GAME *game);
bool gameplay_render(GAME *game);

#endif
