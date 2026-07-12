#include "field.h"

int
load_field(FIELD *field, int screen_width, int screen_height, FONT *font) {
    SDL_Log(
        "width: %d & height: %d",
        screen_width,
        screen_height
    );
    field->screen_height = screen_height;
    field->screen_width = screen_width;

    const float card_width_height_ratio = 7/5.0f;
    field->card_padding_width = screen_height / 30;
    SDL_Log("card_padding_width: %d", field->card_padding_width);
    field->card_padding_height = screen_height / 35;
    SDL_Log("card_padding_height: %d", field->card_padding_height);
    field->screen_padding = screen_width / 45;
    SDL_Log("screen_padding: %d", field->screen_padding);
    field->gameplay_screen_padding_width = (screen_width - screen_height + field->screen_padding) / 2;
    SDL_Log("gameplay_screen_padding_width: %d", field->gameplay_screen_padding_width);
    field->square_screen_padding_width = (screen_width - screen_height) / 2;
    SDL_Log("square_screen_padding_width: %d", field->square_screen_padding_width);
    field->gameplay_screen_padding_height = field->screen_padding;
    SDL_Log("gameplay_screen_padding_height: %d", field->gameplay_screen_padding_height);
    field->card_width = (screen_width - 6 * field->card_padding_width - field->gameplay_screen_padding_width * 2) / 7;
    SDL_Log("card_width: %d", field->card_width);
    field->card_height = card_width_height_ratio * field->card_width;
    SDL_Log("card_height: %d", field->card_height);
    field->cursor_padding = field->card_padding_width / 4;
    SDL_Log("cursor_padding: %d", field->cursor_padding);

    field->name_textbox_width = screen_width - 2 * (field->gameplay_screen_padding_width);
    SDL_Log("name_textbox_width: %d", field->name_textbox_width);
    field->name_textbox_height = field->name_textbox_width / 7;
    SDL_Log("name_textbox_height: %d", field->name_textbox_height);

    int tmp_text_height;
    int tmp_text_width;

    field->title_font = screen_height / 10;
    SDL_Log("title_font: %d", field->title_font);

    int status = get_text_size(font, "soVIMter", field->title_font,
                               &tmp_text_width, &tmp_text_height);
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    field->title_padding = tmp_text_height / 2;
    SDL_Log("title_padding: %d", field->title_padding);

    field->item_font = screen_height / 20;
    SDL_Log("item_font: %d", field->item_font);

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
    SDL_Log("item_padding: %d", field->item_padding);

    field->hover_item_font = screen_height / 15;
    SDL_Log("hover_item_font: %d", field->hover_item_font);

    field->text_font = screen_height / 30;
    SDL_Log("text_font: %d", field->text_font);

    status = get_text_size(
        font,
        "soVIMter",
        field->text_font,
        &tmp_text_width,
        &tmp_text_height
    );
    if (status == 0) {
        SDL_Log("get_text_size error...\n");
        return 0;
    }

    field->text_padding = tmp_text_height / 2;
    SDL_Log("text_padding: %d", field->text_padding);

    return 1;
}
