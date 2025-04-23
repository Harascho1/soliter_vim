#include "field.h"

int
load_field(FIELD *field, int screen_width, int screen_height, FONT *font) {
    field->screen_height = screen_height;
    field->screen_width = screen_width;

    float screen_width_height_ratio = screen_width / screen_height;

    float card_width_height_ratio = 7/5.0f;
    field->card_padding_width = screen_height / 30;
    field->card_padding_height = screen_height / 35;
    field->screen_padding = screen_width / 45;
    field->gameplay_screen_padding_width = (screen_width - screen_height) / 2;
    field->gameplay_screen_padding_height = field->screen_padding;
    field->card_width = (screen_width - 6 * field->card_padding_width - field->gameplay_screen_padding_width * 2) / 7;
    field->card_height = field->card_width * card_width_height_ratio;
    field->cursor_padding = field->card_padding_width / 4;

    int tmp_text_height;
    int tmp_text_width;
    int status;

    field->title_font = screen_height / 10;

    status = get_text_size(
        font, 
        "soVIMter", 
        field->title_font, 
        &tmp_text_width, 
        &tmp_text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    field->title_padding = tmp_text_height / 2;

    field->item_font = screen_height / 20;

    status = get_text_size(
        font, 
        "soVIMter", 
        field->item_font, 
        &tmp_text_width, 
        &tmp_text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    field->item_padding = tmp_text_height / 2;

    field->hover_item_font = screen_height / 15;

    field->text_font = screen_height / 25;

    status = get_text_size(
        font, 
        "soVIMter", 
        field->item_font, 
        &tmp_text_width, 
        &tmp_text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    field->text_padding = tmp_text_height / 2;

    return 1;
}
