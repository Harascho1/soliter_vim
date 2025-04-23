#include "gameplay.h"
#include "config.h"

int
normal_select_mode(GAME *game, const SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_ESCAPE:
                save_score(game);
                push_user_event(g_change_scene_event_type, game_state_game_over);
                break;
            case SDLK_C:
                deselect_all_cards(game->deck);
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
convert_controls(unsigned int key) {
    int i = 0;
    while (i < 14) {
        if (key == config_commands[i]) {
            return i;
        }
        i++;
    }
    return -1;
}
//
//int
//conver_control(unsigned int key) {
//    if (switch_controls(key)) {
//        return 0;
//    }
//
//    return -1;
//}
//
//const int switch_mode = SDLK_X;

int
fly_mode(GAME *game, const SDL_Event *event) {
    int tmp;
    int status;
    int num;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        int key = convert_controls(num = event->key.key);
        switch (key) {
        //switch ((num = event->key.key)) {
            //case SDLK_X:
            case 0:
                deselect_all_cards(game->deck);
                game->cursor->mode = CURSOR_NORMAL_MODE;
                break;

            //case SDLK_TAB:
            case 1:
                deselect_all_cards(game->deck);
                status = have_number_hover(game->cursor);
                if (status != 0) {
                    break;
                }
                game->cursor->pos->row = 0;
                game->cursor->pos->col = 2;
                change_cursor_frame(game);
                break;
            //case SDLK_KP_1:
            //case SDLK_1:
            case 2:
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
            //case SDLK_KP_2:
            //case SDLK_2:
            case 3:
                tmp = 2;
                SDL_Log("pressed key %d", event->key.key);
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
            case SDLK_KP_3:
            case SDLK_3:
            case 4:
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
            case SDLK_KP_4:
            case SDLK_4:
            case 5:
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
            case SDLK_KP_5:
            case SDLK_5:
            case 6:
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
            case SDLK_KP_6:
            case SDLK_6:
            case 7:
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
            case SDLK_KP_7:
            case SDLK_7:
            case 8:
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
            case SDLK_KP_8:
            case SDLK_8:
            case 9:
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
            case SDLK_KP_9:
            case SDLK_9:
            case 10:
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
            case SDLK_KP_0:
            case SDLK_0:
            case 11:
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
            case 12:
                CARD *card = draw_next_card(game->deck);
                deselect_all_cards(game->deck);
                set_a_flag(game->cursor, CURSOR_NORMAL_MODE);
                break;
            //case SDLK_KP_ENTER:
            //case SDLK_RETURN:
            //case SDLK_SPACE:
            case 13:
                interact(game);
                buffer[0] = '0';
                break;
            default:
                break;
        }
        //unsigned int num_pad = num;
        //num_pad = num_pad - SDLK_KP_1 + 1;
        //if (num_pad >= 1 && num_pad <= 10) {
        //    char slovo[2];
        //    if (num_pad == 10) {
        //        num_pad = 0;
        //    }
        //    sprintf(slovo, "%d", num_pad);
        //    strcat(buffer, slovo);
        //    if (strlen(buffer) > 3) {
        //        strcpy(buffer, "");
        //    }
        //}
        //num = num - 0x30u;
        //if (num >= 0 && num <= 9) {
        //    char slovo[2];
        //    sprintf(slovo, "%d", num);
        //    strcat(buffer, slovo);
        //    if (strlen(buffer) > 3) {
        //        strcpy(buffer, "");
        //    }
        //}
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
