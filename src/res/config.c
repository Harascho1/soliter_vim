#include "config.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "res.h"

int* config_commands = NULL;
unsigned int* config_options = NULL;
char* commands_keys[14];
char* options_set[3];

bool insert_option(const unsigned int command, const int idx) {
  if (config_options == NULL) {
    SDL_Log("config_options is NULL\n");
    return false;
  }
  char buffer[10];
  if (idx == 2) {
    config_options[idx] = command;
    sprintf(buffer, "%d", command);
    strlcpy(options_set[idx], buffer, sizeof(options_set[idx]));

    return true;
  }
  config_options[idx] = command;
  if (command == 1) {
    strlcpy(buffer, "On", sizeof(buffer));
  } else {
    strlcpy(buffer, "Off", sizeof(buffer));
  }
  const uint32_t size_of_buffer = (uint32_t)strlen(buffer) + 1;
  options_set[idx] =
    (char*)SDL_realloc(options_set[idx], sizeof(char) * (size_of_buffer + 1));
  strlcpy(options_set[idx], buffer, size_of_buffer);
  return true;
}

int insert_command(SDL_Keycode command, int idx) {
  if (config_commands == NULL) {
    SDL_Log("config_commands is NULL");
    return 0;
  }

  for (int i = 0; i < 13; i++) {
    if (config_commands[i] == command) {
      return 0;
    }
  }

  config_commands[idx] = (int)command;

  const char* buffer = SDL_GetKeyName(command);
  const uint32_t size_of_buffer = (uint32_t)strlen(buffer);
  commands_keys[idx] =
    (char*)SDL_realloc(commands_keys[idx], sizeof(char) * (size_of_buffer + 1));
  strlcpy(commands_keys[idx], buffer, size_of_buffer + 1);

  return 1;
}

bool does_option_file_exist() {
  FILE* option_file = fopen(paths.bins.option, "rb");

  if (option_file == NULL) {
    return false;
  }

  fseek(option_file, 0, SEEK_END);
  const long bytes_of_file = ftell(option_file);

  fclose(option_file);

  if (bytes_of_file != sizeof(int) * 3) {
    return false;
  }
  return true;
}

bool does_config_file_exist() {
  FILE* config_file = fopen(paths.bins.config, "rb");

  if (config_file == NULL) {
    return false;
  }

  fseek(config_file, 0, SEEK_END);
  const long bytes_of_file = ftell(config_file);

  fclose(config_file);

  if (bytes_of_file != sizeof(int) * 14) {
    SDL_Log("config_file does not contain all config options");
    return false;
  }
  return true;
}

void create_config_file() {
  FILE* config_file = fopen(paths.bins.config, "wb+");

  if (config_file == NULL) {
    SDL_Log("config_file is NULL\n");
    return;
  }

  const int arr[14] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,   SDLK_5, SDLK_6,
                       SDLK_7, SDLK_8, SDLK_9, SDLK_X, SDLK_TAB, SDLK_Q, SDLK_SPACE};

  fwrite(arr, sizeof(int), 14, config_file);
  fclose(config_file);
}

void update_option_file() {
  FILE* option_file = fopen(paths.bins.option, "wb+");

  if (option_file == NULL) {
    SDL_Log("config_file is NULL\n");
    return;
  }

  fwrite(config_options, sizeof(int), 3, option_file);
  fclose(option_file);
}

bool update_config_file() {
  FILE* config_file = fopen(paths.bins.config, "wb+");

  if (config_file == NULL) {
    SDL_Log("config_file is NULL\n");
    return false;
  }

  fwrite(config_commands, sizeof(int), 14, config_file);
  fclose(config_file);
  return true;
}

void create_option_file() {
  FILE* option_file = fopen(paths.bins.option, "wb+");
  if (option_file == NULL) {
    SDL_Log("option_file is NULL\n");
    return;
  }

  const int opt_arr[3] = {
    1, // fullscreen
    1, // music
    50 // % percentage of volume
  };

  fwrite(opt_arr, sizeof(int), 3, option_file);
  fclose(option_file);
}

bool load_config() {
  FILE* config_file = fopen(paths.bins.config, "rb");

  if (config_file == NULL) {
    return false;
  }

  config_commands = (int*)SDL_malloc(sizeof(int) * 14);
  int status = (int)fread(config_commands, sizeof(int), 14, config_file);
  if (!status) {
    fclose(config_file);
    return false;
  }
  fclose(config_file);

  for (int i = 0; i < 14; i++) {
    const char* buffer = SDL_GetKeyName(config_commands[i]);
    const uint32_t size_of_buffer = strlen(buffer);
    commands_keys[i] = (char*)SDL_malloc(sizeof(buffer) * (size_of_buffer + 1));
    strlcpy(commands_keys[i], buffer, size_of_buffer + 1);
  }

  FILE* option_file = fopen(paths.bins.option, "rb");
  if (option_file == NULL) {
    return false;
  }
  config_options = (uint32_t*)SDL_malloc(sizeof(int) * 3);
  status = (int)fread(config_options, sizeof(int), 3, option_file);
  if (!status) {
    return false;
  }
  fclose(option_file);
  for (int i = 0; i < 3; i++) {
    options_set[i] = SDL_malloc(sizeof(char) * 10);
    if (i == 2) {
      snprintf(options_set[i], sizeof(options_set[i]), "%d", config_options[i]);
      continue;
    }
    if (config_options[i] == 1) {
      strlcpy(options_set[i], "On", sizeof(options_set[i]));
    } else {
      strlcpy(options_set[i], "Off", sizeof(options_set[i]));
    }
  }
  return true;
}
