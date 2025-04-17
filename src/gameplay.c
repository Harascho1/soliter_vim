#include "gameplay.h"
#include "texture.h"

char buffer[10] = "";

CARD*
top_deck_card(DECK *deck) {
    if (deck->deck_card == NULL) {
        return NULL;
    }
    if (deck->count >= 52) {
        return NULL;
    }
    while (deck->count < 52) {
        if (deck->cards[deck->count].on_field == 0) {
            return &deck->cards[deck->count];
        }
        deck->count++;
    }
    return NULL;
}

CARD*
next_deck_card(DECK *deck) {
    if (deck->count >= 52) {
        deck->count = -1;
        return NULL;
    }
    if (deck->deck_card == NULL) {
        pop_all(deck->new_cards);
    }
    while (deck->count < 52) {
        if (deck->cards[deck->count].on_field == 0) {
            return &deck->cards[deck->count];
        }
        deck->count++;
    }
    return NULL;
}

CARD*
draw_next_card(DECK *deck) {
    if (have_more_cards(deck) == 0) {
        return NULL;
    }
    CARD *next_new_card = top_deck_card(deck);

    if (next_new_card != NULL) {
        next_new_card->visible = visible;
        push(deck->new_cards, next_new_card);
    }

    deck->count++;
    deck->deck_card = next_deck_card(deck);
    return next_new_card;
}

int
reveal_card_below(GAME *game) {

    for (int i = 1; i <= number_of_cards_in_row; i++) {
        int j = 1;
        CARD *card;
        int last_card_visible = 0;
        while ((card = find_card(game->deck, i, j)) != NULL) {
            last_card_visible = card->visible;
            j++;
        }
        j--;
        if (j == 0) {
            continue;
        }
        if (last_card_visible == 0) {
            card = find_card(game->deck, i, j);
            card->visible = visible;
            return 1;
        } 
    }

    return 1;
}

int
selected_card(DECK *deck, CARD **selected_cards) {
    
    int count = 0;
    for (int i = 0; i < 52; i++) {
        if (deck->cards[i].selected == selected) {
            selected_cards[count++] = &deck->cards[i];
        }
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count ; j++) {
            if (selected_cards[j]->value > selected_cards[i]->value) {
                CARD *tmp;
                tmp = selected_cards[j];
                selected_cards[j] = selected_cards[i];
                selected_cards[i] = tmp;
            }
        }
    }
    return count;
}

int
place_king(CARD **card, int num, CURSOR *cursor) {
    int cursor_col = cursor->pos->col;
    int cursor_row = cursor->pos->row;
    int x_coord, y_coord;

    if ((*card)->value != 13) {
        return 0;
    }

    for (int i = 0; i < number_of_cards_in_row; i++) {
        if (cursor_col == g_invisible_card[i].pos->col) {
            x_coord = g_invisible_card[i].frame->x;
            y_coord = g_invisible_card[i].frame->y;

            card[0]->pos->col = cursor_col;
            card[0]->pos->row = cursor_row;
            card[0]->frame->x = x_coord;
            card[0]->frame->y = y_coord;
            card[0]->on_field = 1;

            game_update = 1;
            
            break;
        }
    }

    for (int i = 1; i < num; i++) {
        card[i]->pos->col = cursor_col;
        card[i]->pos->row = cursor_row + 1;
        card[i]->frame->x = x_coord;
        card[i]->frame->y = y_coord + padding_of_card;
        card[i]->on_field = 1;

        cursor_row++;
        y_coord += padding_of_card;

    }

    return 1;
}

int
place_a_card(GAME *game) {
    CARD *card = find_card(game->deck, game->cursor->pos->col, game->cursor->pos->row);

    CARD *s_card[14];
    int num_of_selected_cards;
    num_of_selected_cards = selected_card(game->deck, s_card);

    if (num_of_selected_cards == 0) {
        return 0;
    }

    if (card == NULL) {
        if (place_king(s_card, num_of_selected_cards, game->cursor) == 0) {
            game_update = 0;
            deselect_all_cards(game->deck);
            set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
            return 0;
        }
        if (same_card_selected(view_top_card_in_queue(game->deck->new_cards), *s_card) == 0) {
            game_update = 1;
        } else {
            pop_top(game->deck->new_cards);
        }
        deselect_all_cards(game->deck);
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        return 1;
    }

    if (card == not_visible) {
        deselect_all_cards(game->deck);
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        return 0;
    }

    if (same_card_selected(card, *s_card) == 1 && num_of_selected_cards == 1) {
        if (same_card_selected(card, game->deck->deck_card)) {
            CARD *next_card = draw_next_card(game->deck);
            if (next_card == NULL) {
            }
            deselect_all_cards(game->deck);
            set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
            return 1;

        }
        game_update = sort_a_card(*s_card, game->deck);

        CARD *top_card = view_top_card_in_queue(game->deck->new_cards);
        if (same_card_selected(*s_card, top_card) && game_update == 1) {
            pop_top(game->deck->new_cards);
        }
        deselect_all_cards(game->deck);
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        return 1;
    }       

    if (same_card_selected(*s_card, game->deck->deck_card)) {
        deselect_all_cards(game->deck);
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        return 0;
    }

    if (card->on_field == 0) {
        deselect_all_cards(game->deck);
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        return 0;
    }
    
    if (can_card_be_placed(*s_card, card) == 0) {
        set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
        deselect_all_cards(game->deck);
        return 0;
    }

    int col_of_cursor = game->cursor->pos->col;
    int row_of_cursor = game->cursor->pos->row;
    int x_card_pos = card->frame->x;
    int y_card_pos = card->frame->y;


    for (int i = 0; i < num_of_selected_cards; i++) {
        
        s_card[i]->pos->col = col_of_cursor;
        s_card[i]->pos->row = row_of_cursor + 1;
        s_card[i]->frame->x = x_card_pos;
        s_card[i]->frame->y = y_card_pos + padding_of_card;
        s_card[i]->on_field = 1;

        row_of_cursor++;
        y_card_pos += padding_of_card;
    }

    set_a_flag(game->cursor, CURSOR_NORMAL_MODE);

    CARD *new_card = view_top_card_in_queue(game->deck->new_cards);
    if (same_card_selected(*s_card, new_card)) {
        pop_top(game->deck->new_cards);
        deselect_all_cards(game->deck);
        game_update = 0;
        return 1;
    }

    deselect_all_cards(game->deck);
    game_update = 1;
    return 1;

}


int
select_a_card(GAME *game) {
    CARD *card;
    card = find_card(game->deck, game->cursor->pos->col, game->cursor->pos->row);
    if (card == NULL) {
        if (game->cursor->pos->row == 0 && game->cursor->pos->col == 1) {
            draw_next_card(game->deck);
            return 1;
        }
        return 0;
    }
    if (same_card_selected(card, game->deck->deck_card)) {
        if (have_more_cards(game->deck) == 0) {
            return 1;
        }
        card->selected = !card->selected;
        set_a_flag(game->cursor, CURSOR_SELECT_MODE);
        return 1;
    }
    if (same_card_selected(card, view_top_card_in_queue(game->deck->new_cards))) {
        card->selected = !card->selected;
        set_a_flag(game->cursor, CURSOR_SELECT_MODE);
        return 1;
    }
    if (card->visible == not_visible) {
        return 1;
    }
    card->selected = !card->selected;
    int selected_cards = select_card_below(card, game->deck);
    set_a_flag(game->cursor, CURSOR_SELECT_MODE);
    return 1;
}

int
change_cursor_frame(GAME *game) {
    CARD *card = find_card(
        game->deck,
        game->cursor->pos->col,
        game->cursor->pos->row
    );
    // * deck_card
    if (card == NULL && game->cursor->pos->row == 0) {
        game->cursor->cursor->x = padding_of_card / 4;
        game->cursor->cursor->y = padding_of_card / 4;
        return 1;
    }
    if (same_card_selected(card, view_top_card_in_queue(game->deck->new_cards)) == 1) {
        game->cursor->cursor->x = padding_of_card * 3/2 + g_card_width - padding_of_card / 4; 
        game->cursor->cursor->y = padding_of_card / 4;    
    } else {
        game->cursor->cursor->x = card->frame->x - padding_of_card / 4; 
        game->cursor->cursor->y = card->frame->y - padding_of_card / 4;    

    }
    return 1;
}

int
go_to_invisible_card(GAME *game, int col) {
    game->cursor->pos->col = col;
    game->cursor->cursor->x = g_invisible_card[col - 1].frame->x - padding_of_card / 4;
    game->cursor->cursor->y = g_invisible_card[col - 1].frame->y - padding_of_card / 4;
}

int
interact(GAME *game) {
    if (game->cursor->mode % 2 == 0) {
        select_a_card(game);
    } else if (game->cursor->mode % 2 == 1) {
        place_a_card(game);
    }
}

int
gameplay_update(GAME* game) {
    int status;
    if (game->timer->start_timer == 0) {
        start_timer(game->timer);
    }

    if (game_update == 1) {
        status = reveal_card_below(game);
        if (status != 0) {
            game_update = 0;
        }
    }

    int count = 0;
    for (int i = suit_clubs; i <= suit_spades; i++) {
        if (game->deck->sorted_cards[i] == NULL) {
            break;
        }
        if (game->deck->sorted_cards[i]->value == 13) {
            count++;
        }
    }

    if (count == 4) {
        g_game_win = 1;
        push_user_event(g_change_scene_event_type, game_state_game_over);
    }

    if (have_number_hover(game->cursor) == 0) {
        strcpy(buffer, "");
    }

    if (game->timer->time_elapsed >= 3600) {
        g_game_win = 0;
        push_user_event(g_change_scene_event_type, game_state_game_over);
    }

    return 1;
}


SCENE gameplay_scene = {
    .handle_events = gamaplay_event_handler,
    .update = gameplay_update,
    .render = gameplay_render
};