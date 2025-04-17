#include "config.h"

int *config_commands = NULL;

int
does_config_file_exist() {
    FILE *config_file = fopen("assets/bin/config.bin", "rb");

    if (config_file == NULL) {
        return 0;
    }

    fseek(config_file, 0, SEEK_END);
    long bytes_of_file = ftell(config_file);

    if (bytes_of_file != sizeof(int) * 14) {
        return 0;
    }

    fclose(config_file);

    return 1;
}

void
create_config_file() {
    FILE *config_file = fopen("assets/bin/config.bin", "wb+");

    if (config_file == NULL) {
        return;
    }

    int arr[14] = {
        SDLK_X,
        SDLK_TAB,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_4,
        SDLK_5,
        SDLK_6,
        SDLK_7,
        SDLK_8,
        SDLK_9,
        SDLK_0,
        SDLK_Q,
        SDLK_SPACE
    };
    fwrite(arr, sizeof(int), 14, config_file);

    rewind(config_file);

    int print_arr[14];
    fread(print_arr, sizeof(int), 14, config_file);
    for (int i = 0; i < 14; i++) {
        SDL_Log("%d ", print_arr[i]);
    }
    SDL_Log("\n");

    fclose(config_file);
}

int*
load_config() {
    FILE *config_file = fopen("assets/bin/config.bin", "rb");

    config_commands = (int*)SDL_malloc(sizeof(int) * 14);
    fread(config_commands, sizeof(int), 14, config_file);
    fclose(config_file);


    return config_commands;
}