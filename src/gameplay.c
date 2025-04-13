#include "game.h"
#include "texture.h"

int change_cursor_frame(GAME *game);
static char buffer[10] = "";

static const char* modes[4] = {
    "mode:normal",
    "mode:select",
    "mode:fly",
    "mode:fly-select"
};

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

int
have_more_cards(DECK *deck) {
    if (deck->deck_card != NULL) {
        return 1;
    } 
    int count = 0;
    for (int i = 28; i < 52; i++) {
        if (deck->cards[i].on_field == 0) {
            count++;
        }
    }
    if (count > 1) {
        return 1;
    }
    return 0;
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
        if (game_update != 0) {
            if (same_card_selected(*s_card, view_top_card_in_queue(game->deck->new_cards))) {
                pop_top(game->deck->new_cards);
            }
            deselect_all_cards(game->deck);
            set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
            return 1;
        }
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
normal_select_mode(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                push_user_event(g_change_scene_event_type, game_state_game_over);
                break;
            case SDLK_C:
                game->cursor->mode = CURSOR_FLY_MODE;
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                if (game->cursor->pos->col == 7) {
                    break;
                }
                if (game->cursor->pos->row == 0 &&
                    game->cursor->pos->col == 2) {
                        break;
                    }
                int new_col = game->cursor->pos->col + 1;
                if (find_card(game->deck, new_col, game->cursor->pos->row) == 0) {
                    if (game->cursor->pos->row == 1) {
                        go_to_invisible_card(game, new_col); 
                        break;
                    } 
                    break;
                }
                game->cursor->pos->col++;
                change_cursor_frame(game);
                break;                
            case SDLK_A:
            case SDLK_LEFT:
                if (game->cursor->pos->col == 1) {
                    break;
                }
                new_col = game->cursor->pos->col - 1;
                if (find_card(game->deck, new_col, game->cursor->pos->row) == 0) {
                    if (game->cursor->pos->row == 1) {
                        go_to_invisible_card(game, new_col);
                        break;
                    }
                    if (game->cursor->pos->row != 0) {
                        break;

                    }
                }
                game->cursor->pos->col--;
                change_cursor_frame(game);
                break;                
            case SDLK_S:
            case SDLK_DOWN:
                int new_row = game->cursor->pos->row + 1;
                if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
                    if (game->cursor->pos->row == 0) {
                        go_to_invisible_card(game, game->cursor->pos->col);
                        game->cursor->pos->row = new_row;
                    }
                    break;
                }
                game->cursor->pos->row++;
                change_cursor_frame(game);
                break;                
            case SDLK_W:
            case SDLK_UP:
                if (game->cursor->pos->row == 0) {
                    break;
                }
                if (game->cursor->pos->row == 1) {
                    if (game->cursor->pos->col != 1 &&
                        game->cursor->pos->col != 2) {
                        break;
                    }
                }
                new_row = game->cursor->pos->row - 1;
                if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
                    if (game->cursor->pos->col != 1) {
                        break;
                    }
                }
                game->cursor->pos->row--;
                change_cursor_frame(game);
                break;                
            case SDLK_N:
                CARD *card = draw_next_card(game->deck);
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                interact(game);
                break;
            default:
                break;

        }
    }
    return 1;
}

int
fly_mode(GAME *game, const SDL_Event *event) {
    int tmp;
    int status;
    int num;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch ((num = event->key.key)) {
            case SDLK_X:
                game->cursor->mode = CURSOR_NORMAL_MODE;
                break;

            case SDLK_TAB:
                status = have_number_hover(game->cursor);
                if (status != 0) {
                    break;
                }
                game->cursor->pos->row = 0;
                game->cursor->pos->col = 2;
                change_cursor_frame(game);
                break;
            
            case SDLK_1:
                tmp = 1;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_1);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                } else {
                    if (tmp > 10) {

                        break;
                    }
                    game->cursor->pos->row = tmp;
                    go_to_invisible_card(game, status);

                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_2:
                tmp = 2;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_2);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_3:
                tmp = 3;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_3);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_4:
                tmp = 4;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_4);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_5:
                tmp = 5;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_5);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_6:
                tmp = 6;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_6);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_7:
                tmp = 7;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_7);
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_8:
                tmp = 8;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_9:
                tmp = 9;
                if (have_a_flag(game->cursor, CURSOR_HOVER_10)) {
                    tmp = tmp + 10;
                }
                if ((status = have_number_hover(game->cursor)) == 0) {
                    break;
                } else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_0:
                tmp = 10;
                if ((status = have_number_hover(game->cursor)) == 0) {
                    break;
                } else if (have_a_flag(game->cursor, CURSOR_HOVER_10) == 0) {
                    set_a_flag(game->cursor, CURSOR_HOVER_10);
                    break;
                }
                else if (find_card(game->deck, status, tmp) != NULL) {
                    game->cursor->pos->col = status;
                    game->cursor->pos->row = tmp;
                    delete_hover_flag(game->cursor);
                    change_cursor_frame(game);
                }
                delete_hover_flag(game->cursor);
                break;
            case SDLK_Q:
                CARD *card = draw_next_card(game->deck);
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                interact(game);
                buffer[0] = '0';
                break;
            default:
                break;
        }
        num = num - 0x30u;
        if (num >= 0 && num <= 9) {
            char slovo[2];
            sprintf(slovo, "%d", num);
            printf("%c\n", *slovo);
            strcat(buffer, slovo);
            if (strlen(buffer) > 3) {
                strcpy(buffer, "");
            }
        }
    }
    return 1;
}

int
gamaplay_event_handler(GAME *game, const SDL_Event *event) {
    int status;
    if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
        fly_mode(game, event);

    } else if (have_a_flag(game->cursor, CURSOR_SELECT_MODE)) {
        normal_select_mode(game, event);

    } else {
        normal_select_mode(game, event);

    }


    return 1;
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
    // TODO - auto sort da update sam sortira karte koje su na polju

    int count = 0;
    for (int i = suit_clubs; i <= suit_spades; i++) {
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



    return 1;
}

int
render_commands(GAME *game) {
    if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
        return 0;
    }

    if (have_number_hover(game->cursor) == 0) {
        return 0;
    }

    //render
    int status;
    int text_width, text_height;

    if (strlen(buffer) == 0) {
        return 0;
    }
    
    status = get_text_size(
        game->font,
        buffer,
        title_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        return 0;
    }

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);

    status = render_text(
        game->font,
        game->renderer,
        buffer,
        title_size,
        &(SDL_Point){
            .x = (width - text_width) / 2,
            .y = (height - text_height - padding_of_card)
        },
        &(SDL_Color){255, 255, 255, 255}
    );
    if (status == 0) {
        return 0;
    }

}

int render_cursor(GAME *game) {
    int status = 0;

    const char *path = "assets/cursor.png";
    SDL_Texture *texture = create_texture_from_image(game->renderer, path);
    if (texture == NULL) {
        return 0;
    }

    if (game->cursor->cursor == NULL) {
    } 

    status = SDL_RenderTexture(
        game->renderer,
        texture,
        NULL,
        game->cursor->cursor
    );
    if (status == 0) {
        SDL_Log("SDL_RenderTexture error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_DestroyTexture(texture);
    return status;
}

int
gameplay_render(GAME* game) {
    int status;
    
    if (game == NULL) {
        SDL_Log("game is NULL\n");
        return 0;
    }

    if (game->renderer == NULL) {
        SDL_Log("game->renderer is NULL\n");
        return 0;
    }

    if (game->background_texture == NULL) {
        SDL_Log("g_background_texture is NULL\n");
        game->background_texture = create_texture_from_image(game->renderer, "assets/background.png");
        if (game->background_texture == NULL) {
            SDL_Log("Background texture is NULL\n");
            return 0;
        }
    }

    status = SDL_RenderClear(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
        return 0;
    }

    status = SDL_RenderTexture(game->renderer, game->background_texture, NULL, NULL);
    if (status == 0) {
        SDL_Log("SDL_RenderTexture failed: %s\n", SDL_GetError());
        printf("ZASTOOOOO\n");
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    render_commands(game);

    int width, height;
    status = SDL_GetWindowSizeInPixels(game->window, &width, &height);

    // *RENDERING SORTED_CARDS
    for (int i = 0; i < 4; i++) {
        status = render_card(
            game->renderer,
            game->deck->sorted_cards[i],
            game->deck->sorted_cards[i]->frame
        );
    }

    // *RENDERING DECK AND ONE MORE CARD ON LEFT RIGHT CORNER

    if (game->deck->deck_card != NULL) {
        status = render_card(
            game->renderer,
            game->deck->deck_card,
            game->deck->deck_card->frame
        );
        if (status == 0) {
            SDL_Log("render card error...\n");
            return 0;
        }
    }
    if (game->cursor->pos->col == 1 &&
        game->cursor->pos->row == 0) {

            status = render_cursor(game);
            if (status == 0) {
                SDL_Log("render_cursor error...\n");
                return 0;
            }
    }

    CARD *card = view_top_card_in_queue(game->deck->new_cards);
    if (card != NULL) {
        status = render_card(
            game->renderer,
            card,
            &(SDL_FPoint){
                .x = padding_of_card / 2 + padding_of_card + g_card_width,
                .y = padding_of_card / 2
            }
        );
        if (status == 0) {
            SDL_Log("render card error...\n");
        }
    }
    if (game->cursor->pos->col == 2 &&
        game->cursor->pos->row == 0) {

            status = render_cursor(game);
            if (status == 0) {
                SDL_Log("render_cursor error...\n");
                return 0;
            }
    }

    // * RENDERING TEXT THAT SHOWS WHAT IN WHAT MODE ARE U
    int mode = game->cursor->mode % 4;
    int text_width, text_height;
    status = get_text_size(
        game->font,
        modes[mode],
        standard_font_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
    }
    status = render_text(
        game->font,
        game->renderer,
        modes[mode],
        standard_font_size,
        &(SDL_Point){padding_of_card, height - text_height - padding_of_card},
        &(SDL_Color){255,255,255,255}
    );


    // * RENDERING GAME FIELD
    for (int i = 1; i <= number_of_cards_in_row; i++) {
        int j = 1;
        CARD *card;
        while ((card = find_card(game->deck, i, j)) != NULL) {
            render_card(
                game->renderer,
                card,
                card->frame
            );
            if (game->cursor->pos->col == i &&
                game->cursor->pos->row == j) {
                    status = render_cursor(game);
                    if (status == 0) {
                        SDL_Log("render_cursor error...\n");
                        return 0;
                    }
            }
            j++;
        }
        if (game->cursor->pos->col == i &&
            game->cursor->pos->row == j) {
                status = render_cursor(game);
                if (status == 0) {
                    SDL_Log("render_cursor error...\n");
                    return 0;
                }
        }
    }

    char timer[50];
    snprintf(timer, sizeof(timer), "seconds: %d", game->timer->time_elapsed);
    status = get_text_size(
        game->font,
        timer,
        standard_font_size,
        &text_width,
        &text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
    }
    status = render_text(
        game->font,
        game->renderer,
        timer,
        standard_font_size,
        &(SDL_Point){width - padding_of_card - text_width, height - text_height - padding_of_card},
        &(SDL_Color){255,255,255,255}
    );

    status = SDL_RenderPresent(game->renderer);
    if (status == 0) {
        SDL_Log("SDL_RenderPresent failed: %s\n", SDL_GetError());
        push_user_event(SDL_EVENT_QUIT, 0);
        return 0;
    }

    return 1;
}

SCENE gameplay_scene = {
    .handle_events = gamaplay_event_handler,
    .update = gameplay_update,
    .render = gameplay_render
};