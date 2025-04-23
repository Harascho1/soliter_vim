#include "config.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include <stdio.h>
#include <string.h>

int *config_commands = NULL;
int *config_options = NULL;
char *commands_keys[14];
char *options_set[3];

int
insert_command(SDL_Keycode command, int index) {
    if (config_commands == NULL) {
        SDL_Log("config_commands is NULL");
        return 0;
    }
    config_commands[index] = command;
    const char *buffer = SDL_GetKeyName(command);
    int size_of_buffer = strlen(buffer);
    commands_keys[index] = (char*)SDL_realloc((char*)commands_keys, sizeof(char) * (size_of_buffer + 1));
    strcpy(commands_keys[index], buffer);
    SDL_Log("zamenjena komanda je: %s\n", commands_keys[index]);

    return 1;
}

int
does_option_file_exist() {
    FILE *option_file = fopen("assets/bin/option.bin", "rb");

    if (option_file == NULL) {
        return 0;
    }

    fseek(option_file, 0, SEEK_END);
    long bytes_of_file = ftell(option_file);

    if (bytes_of_file != sizeof(int) * 3) {
        return 0;
    }

    fclose(option_file);
    return 1;
}

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
        SDL_Log("config_file is NULL\n");
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
    fclose(config_file);

    FILE *option_file = fopen("assets/bin/option.bin", "wb+");
    if (option_file == NULL) {
        SDL_Log("option_file is NULL\n");
        return;
    }

    int opt_arr[3] = {
        0, //fullscreen
        1, //music
        50 //%percent of volume
    };

    fwrite(opt_arr, sizeof(int), 3, option_file);
    fclose(option_file);
}

void
create_option_file() {
    FILE *option_file = fopen("assets/bin/option.bin", "wb+");
    if (option_file == NULL) {
        SDL_Log("option_file is NULL\n");
        return;
    }

    int opt_arr[3] = {
        0, //fullscreen
        1, //music
        50 //%percent of volume
    };

    fwrite(opt_arr, sizeof(int), 3, option_file);
    fclose(option_file);
}

int*
load_config() {
    FILE *config_file = fopen("assets/bin/config.bin", "rb");

    config_commands = (int*)SDL_malloc(sizeof(int) * 14);
    fread(config_commands, sizeof(int), 14, config_file);
    fclose(config_file);

    for (int i = 0; i < 14; i++) {
        const char *buffer = SDL_GetKeyName(config_commands[i]);
        int size_of_buffer = strlen(buffer);
        commands_keys[i] = (char*)SDL_malloc(sizeof(buffer) * (size_of_buffer + 1));
        strcpy(commands_keys[i], buffer);
    }

    FILE *option_file = fopen("assets/bin/option.bin", "rb");
    config_options = (int*)SDL_malloc(sizeof(int) * 3);
    fread(config_options, sizeof(int), 3, option_file);
    fclose(option_file);
    for (int i = 0; i < 3; i++) {
        options_set[i] = (char*)SDL_malloc(sizeof(char) * 10);
        if (i == 2) {
            sprintf(options_set[i], "%d", config_options[i]);
            continue;
        }
        if (config_options[i] == 1) {
            strcpy(options_set[i], "Yes");
        } else {
            strcpy(options_set[i], "No");
        }
    }
    SDL_Log("Prosao sam");
    return config_commands;
}
