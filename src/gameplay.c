#include "game.h"
#include "texture.h"

CARD* is_there_a_card(GAME *game, int *row, int *col);
int change_cursor_frame(GAME *game);

static const char* modes[3] = {
    "mode:normal",
    "mode:select",
    "mode:fly"
};

CARD*
top_deck_card(DECK *deck) {
    if (deck->count >= 52) {
        SDL_Log("deck->count je veci ili jednak sa 52\n");
        deck->count = 28;
        pop_all(deck->new_cards);
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
        SDL_Log("deck->count je veci ili jednak sa 52\n");
        deck->count = 28;
    }
    while (deck->count < 52) {
        if (deck->cards[deck->count].on_field == 0) {
            return &deck->cards[deck->count];
        }
        deck->count++;
    }
    return NULL;
}

//TODO OVO JE PROBLEM ZA KASNIJE
CARD*
draw_next_card(DECK *deck) {
    CARD *next_new_card = top_deck_card(deck);

    if (next_new_card == NULL) {
        SDL_Log("Nije uspeo da izvuce novu kartu\n");
        return NULL;
    } else {
        next_new_card->visible = visible;
        push(deck->new_cards, next_new_card);
    }

    //SDL_Log(
    //    "%d %d",
    //    next_new_card->suit,
    //    next_new_card->value
    //);

    deck->count++;
    deck->deck_card = next_deck_card(deck);
    //SDL_Log("broj karte za deck_card: %d", deck->count);
    return next_new_card;
}

int
reveal_card_below(GAME *game) {
    //TODO dodati da moze da vidi ako je postavio new card da otkrije kartu ispod nje ako je ima

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
            SDL_Log("j je 0\n");
            continue;
        }
        if (last_card_visible == 0) {
            //SDL_Log("sad cemo da okrenemo kartu\n");
            card = find_card(game->deck, i, j);
            if (card == NULL) {
                SDL_Log("card je NULL\n");
            }
            card->visible = visible;
            return 1;
        } 
    }
    SDL_Log("NISAM USPEO DA OTKRIJEM KARTU\n");

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
    //for (int i = 0; i < count; i++) {
    //    SDL_Log(
    //        "karta je: %d\n",
    //        selected_cards[i]->value
    //    );
    //}
    return count;
}

int
place_king(CARD **card, int num, CURSOR *cursor) {
    int cursor_col = cursor->pos->col;
    int cursor_row = cursor->pos->row;
    int x_coord, y_coord;

    for (int i = 0; i < number_of_cards_in_row; i++) {
        if (cursor_col == invisible_card[i].pos->col) {
            x_coord = invisible_card[i].frame->x;
            y_coord = invisible_card[i].frame->y;

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
        SDL_Log("Ni jedna karta nije selektovana\n");
        return 0;
    }

    if (card == NULL) {
        //TODO - da probas da postavis kralja
        SDL_Log("Karta na koju postavljas kartu/e nije pronadjena\n");
        if (place_king(s_card, num_of_selected_cards, game->cursor) == 0) {
            SDL_Log("Nije uspeo da postavi kralja\n");
            return 0;
        }
        if (same_card_selected(view_top_card_in_queue(game->deck->new_cards), *s_card) == 0) {
            SDL_Log("Kralj je bio na fildu\n");
            game_update = 1;
        } else {
            pop_top(game->deck->new_cards);
        }
        deselect_all_cards(game->deck);
        game->cursor->mode = 0;
        return 1;
    }

    if (card == not_visible) {
        deselect_all_cards(game->deck);
        game->cursor->mode = 0;
        return 0;
    }

    if (same_card_selected(card, *s_card) == 1 && num_of_selected_cards == 1) {
        //TODO dodaj da vuce kartu ako je selektovao deck card
        if (same_card_selected(card, game->deck->deck_card)) {
            CARD *next_card = draw_next_card(game->deck);
            //push(game->deck->new_cards, next_card);
            if (next_card == NULL) {
                SDL_Log("error u same_card_selected\n");
            }
            deselect_all_cards(game->deck);
            game->cursor->mode = 0;
            return 1;

        }
        game_update = sort_a_card(*s_card, game->deck);
        if (game_update != 0) {
            if (same_card_selected(*s_card, view_top_card_in_queue(game->deck->new_cards))) {
                pop_top(game->deck->new_cards);
            }
            deselect_all_cards(game->deck);
            game->cursor->mode = 0;
            return 1;
        }
        SDL_Log("Karta ne moze da se sortira\n");
    }       

    if (same_card_selected(*s_card, game->deck->deck_card)) {
        deselect_all_cards(game->deck);
        game->cursor->mode = 0;
        return 0;
    }

    if (card->on_field == 0) {
        //SDL_Log("Tu karta ne moze da se postavi\n");
        deselect_all_cards(game->deck);
        game->cursor->mode = 0;
        return 0;
    }
    
    if (can_card_be_placed(*s_card, card) == 0) {
        //SDL_Log(
        //    "card1 addr: %ld & card2: %ld",
        //    s_card,
        //    card
        //);
        SDL_Log("Karta ne moze da se postavi\n");
        game->cursor->mode = 0;
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

    //SDL_Log("postavljam kartu\n");
    game->cursor->mode = 0;

    CARD *new_card = view_top_card_in_queue(game->deck->new_cards);
    if (same_card_selected(*s_card, new_card)) {
        SDL_Log("da li sam usao ovde\n");
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
        SDL_Log("Selected card not found\n");
        exit(0);
    }
    //SDL_Log(
    //    "card addr: %ld & deck_card: %ld",
    //    card,
    //    game->deck->deck_card
    //);
    if (same_card_selected(card, game->deck->deck_card)) {
        card->selected = !card->selected;
        game->cursor->mode = 1;
        return 1;
    }
    if (same_card_selected(card, view_top_card_in_queue(game->deck->new_cards))) {
        card->selected = !card->selected;
        game->cursor->mode = 1;
        return 1;
    }
    if (card->visible == not_visible) {
        return 1;
    }
    card->selected = !card->selected;
    int selected_cards = select_card_below(card, game->deck);
    game->cursor->mode = 1;
    return 1;
}

CARD*
is_there_a_card(GAME *game, int *changed_row, int *changed_col) {
    int col, row;
    col = game->cursor->pos->col;
    row = game->cursor->pos->row;
    if (changed_col != NULL) {
        col = *changed_col;
    } 
    if (changed_row != NULL) {
        row = *changed_row;
    }
    for (int i = 0; i < 52; i++) {
        if (game->deck->cards[i].pos->col == col &&
            game->deck->cards[i].pos->row == row) {
                return &game->deck->cards[i];
        }
    }
    return NULL;
}

int
change_cursor_frame(GAME *game) {
    CARD *card = find_card(
        game->deck,
        game->cursor->pos->col,
        game->cursor->pos->row
    );
    if (card == NULL) {
        return 0;
    }
    if (same_card_selected(card, view_top_card_in_queue(game->deck->new_cards)) == 1) {
        game->cursor->cursor->x = padding_of_card * 3/2 + card_width - padding_of_card / 4; 
        game->cursor->cursor->y = padding_of_card / 4;    
    } else {
        game->cursor->cursor->x = card->frame->x - padding_of_card / 4; 
        game->cursor->cursor->y = card->frame->y - padding_of_card / 4;    

    }
    //SDL_Log(
    //    "(%f, %f) [%f, %f]\n",
    //    game->cursor->cursor->x,
    //    game->cursor->cursor->y,
    //    game->cursor->cursor->w,
    //    game->cursor->cursor->h
    //);
    return 1;
}

int
go_to_invisible_card(GAME *game, int col) {
    game->cursor->pos->col = col;
    game->cursor->cursor->x = invisible_card[col - 1].frame->x - padding_of_card / 4;
    game->cursor->cursor->y = invisible_card[col - 1].frame->y - padding_of_card / 4;
}

int
interact(GAME *game) {
    if (game->cursor->mode == 0) {
        select_a_card(game);
    } else if (game->cursor->mode == 1) {
        place_a_card(game);
    }
}

int
gamaplay_event_handler(GAME *game, const SDL_Event *event) {
    int tmp;
    int status;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                push_user_event(g_change_scene_event_type, game_state_game_over);
                break;
            case SDLK_X:
                game->cursor->mode = CURSOR_NORMAL_MODE;
                break;
            case SDLK_C:
                //TODO omoguci da se koristi fly mode
                //SDL_Log("Trenutno je core mehanika u izgradnji");
                //break;
                game->cursor->mode = CURSOR_FLY_MODE;
                break;
            case SDLK_1:
                tmp = 1;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_1) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_1;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_1);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_2:
                tmp = 2;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_3:
                tmp = 3;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_4:
                tmp = 4;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_5:
                tmp = 5;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_6:
                tmp = 6;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_7:
                tmp = 7;
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE) == 0) {
                    break;
                }
                if (have_a_flag(game->cursor, CURSOR_HOVER_2) == 0) {
                    game->cursor->mode = game->cursor->mode | CURSOR_HOVER_2;
                    break;
                } else if (is_there_a_card(game, NULL, &(tmp))) {
                    game->cursor->pos->col = tmp;
                    delete_hover_flag(game->cursor, CURSOR_HOVER_2);
                    change_cursor_frame(game);
                }
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
                    break;
                }
                if (game->cursor->pos->col == 7) {
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
                //SDL_Log("Nasao je kartu desno\n");
                game->cursor->pos->col++;
                change_cursor_frame(game);
                break;                
            case SDLK_A:
            case SDLK_LEFT:
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
                    break;
                }
                if (game->cursor->pos->col == 1) {
                    break;
                }
                new_col = game->cursor->pos->col - 1;
                if (find_card(game->deck, new_col, game->cursor->pos->row) == 0) {
                    if (game->cursor->pos->row == 1) {
                        go_to_invisible_card(game, new_col);
                        break;
                    }
                    break;
                }
                game->cursor->pos->col--;
                change_cursor_frame(game);
                break;                
            case SDLK_S:
            case SDLK_DOWN:
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
                    break;
                }
                int new_row = game->cursor->pos->row + 1;
                if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
                    break;
                }
                game->cursor->pos->row++;
                change_cursor_frame(game);
                break;                
            case SDLK_W:
            case SDLK_UP:
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
                    break;
                }
                if (game->cursor->pos->row == 1) {
                    if (game->cursor->pos->col != 1) {
                        break;
                    }
                }
                new_row = game->cursor->pos->row - 1;
                if (find_card(game->deck, game->cursor->pos->col, new_row) == 0) {
                    break;
                }
                //SDL_Log("promenio sam poziciju kursora\n");
                game->cursor->pos->row--;
                change_cursor_frame(game);
                break;                
            case SDLK_N:
                CARD *card = draw_next_card(game->deck);
                //push(game->deck->new_cards, card);
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                if (have_a_flag(game->cursor, CURSOR_FLY_MODE)) {
                    break;
                }
                interact(game);
                break;
            default:
                break;
        }
    }
    return 1;
}

int
gameplay_update(GAME* game) {
    int status;
    if (game_update == 1) {
        status = reveal_card_below(game);
        if (status != 0) {
            game_update = 0;
        }
    }
    // TODO - auto sort da update sam sortira karte koje su na polju

    return 1;
}

int render_deck(GAME *game) {

    return 1;
}

int render_cursor(GAME *game) {
    int status = 0;

    const char *path = "../assets/cursor.png";
    SDL_Texture *texture = create_texture_from_image(game->renderer, path);
    if (texture == NULL) {
        SDL_Log("create_texture_from_image error...\n");
        return 0;
    }

    if (game->cursor->cursor == NULL) {
        SDL_Log("nema kursor FRect-a\n");
    } 

    //SDL_Log(
    //    "(%f, %f) [%f, %f]\n",
    //    game->cursor->cursor->x,
    //    game->cursor->cursor->y,
    //    game->cursor->cursor->w,
    //    game->cursor->cursor->h
    //);
    //SDL_Log(
    //    "(%d, %d)\n",
    //    game->cursor->pos->row,
    //    game->cursor->pos->col
    //);

    //SDL_Log(
    //    "queue->p: %d\nqueue->q: %d",
    //    game->deck->new_cards->p,
    //    game->deck->new_cards->q
    //);
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
        game->background_texture = create_texture_from_image(game->renderer, "../assets/background.png");
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

    //TODO popravi ovo ovo je samo za test za kursor
    // *RENDERING DECK AND ONE MORE CARD ON LEFT RIGHT CORNER

    //TODO Prepravi ovo
    if (game->deck->deck_card != NULL) {
        status = render_card(
            game->renderer,
            game->deck->deck_card,
            game->deck->deck_card->frame
        );
        if (status == 0) {
            SDL_Log("render card error...\n");
            //return 0;
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
                .x = padding_of_card / 2 + padding_of_card + card_width,
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
    //SDL_Log("mode: %d\n", mode);
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