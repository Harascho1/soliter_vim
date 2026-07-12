#include "game.h"
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_video.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "config.h"
#include "field.h"
#include "sound.h"
#include "texture.h"
#include <ctype.h>
#include <string.h>

const char *main_menu_items[] = {"Play", "Settings", "Scores", "Exit"};

const char *game_over_items[] = {"Restart", "Scores", "Main Menu"};

const char *setting_items[] = {"Options", "Cancel", "Macros"};

static FILE *saves_file;

Uint32 g_change_scene_event_type = (Uint32)-1;

int game_update = 0;
int g_game_win = 0;
CARD g_invisible_card[7];

int load_game_field(DECK *deck, FIELD *field) {
  int count = 0;
  int x_coord = field->gameplay_screen_padding_width;

  for (int i = 0; i < number_of_cards_in_row; ++i) {
    int y_coord = field->gameplay_screen_padding_height +
                  field->card_padding_height + field->card_height;
    g_invisible_card[i].frame = SDL_malloc(sizeof(SDL_FPoint));
    g_invisible_card[i].pos = SDL_malloc(sizeof(POSITION));
    g_invisible_card[i].pos->row = 1;
    g_invisible_card[i].pos->col = i + 1;
    g_invisible_card[i].frame->x = x_coord;
    g_invisible_card[i].frame->y = y_coord;
    g_invisible_card[i].value = 14;
    for (int j = 0; j < i + 1; ++j) {
      if (i == j) {
        deck->cards[count].visible = visible;
      }
      deck->cards[count].frame->x = x_coord;
      deck->cards[count].frame->y = y_coord;
      deck->cards[count].pos->col = i + 1;
      deck->cards[count].pos->row = j + 1;
      deck->cards[count].on_field = 1;

      y_coord += field->card_padding_height;
      count++;
    }
    x_coord += field->card_padding_width + field->card_width;
  }
  for (; count < 52; count++) {
    deck->cards[count].frame->x = field->gameplay_screen_padding_width;
    deck->cards[count].frame->y = field->gameplay_screen_padding_height;
    deck->cards[count].pos->col = 1;
    deck->cards[count].pos->row = 0;
    deck->cards[count].on_field = 0;
  }
  for (int suit = suit_clubs; suit <= suit_hearts; ++suit) {
    deck->sorted_cards[suit] = NULL;
  }
  return 1;
}

int fullscree_mode(GAME *game) {
  int status;
  SDL_DisplayID id;
  status = SDL_SetWindowFullscreen(game->window, config_options[0]);
  if (status == 0) {
    SDL_Log("SDL_SetWindowFullscreen error %s", SDL_GetError());
    return status;
  }
  return status;
}

int reload_window(GAME *game) {
  int status;
  status = fullscree_mode(game);
  if (status == 0) {
    SDL_Log("fullscreen is not set\n");
    return 0;
  }

  SDL_SyncWindow(game->window);
  int w, h;
  status = SDL_GetWindowSizeInPixels(game->window, &w, &h);

  status = load_field(&game->field, w, h, game->font);
  if (status == 0) {
    SDL_Log("load_field error...\n");
    game_quit(game);
    return 0;
  }

  return 1;
}

int game_init(GAME *game, const char *title, const RESOLUTION *res) {
  int status = 0;

  game->mixer = NULL;
  game->soundboard = NULL;

  SDL_AudioDeviceID devid;
  SDL_AudioSpec spec;
  SDL_memset(&spec, 0, sizeof(spec));
  spec.format = SDL_AUDIO_S32;
  spec.channels = 2;
  spec.freq = 48000;

  devid = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
  if (devid == 0) {
    SDL_Log("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
    game_quit(game);
    return status;
  }

  game->mixer = MIX_CreateMixerDevice(devid, &spec);
  if (game->mixer == NULL) {
    SDL_Log("MIX_CreateMixerDevice failed: %s\n", SDL_GetError());
    game_quit(game);
    return 0;
  }

  game->soundboard = create_soundboard(game->mixer);
  if (game->soundboard == NULL) {
    SDL_Log("create_soundboard error\n");
    game_quit(game);
    return 0;
  }

  if (does_config_file_exist() == 0) {
    create_config_file();
  }
  if (does_option_file_exist() == 0) {
    create_option_file();
  }
  load_config();

  game->window = SDL_CreateWindow(title, res->width, res->height, 0);
  if (game->window == NULL) {
    SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
    return status;
  }

  status = fullscree_mode(game);
  if (status == 0) {
    SDL_Log("fullscreen is not set\n");
  }

  SDL_SyncWindow(game->window);

  game->font = create_font("assets/font.ttf");
  if (game->font == NULL) {
    SDL_Log("font_init failed.\n");
    game_quit(game);
    return status;
  }

  int w, h;
  status = SDL_GetWindowSizeInPixels(game->window, &w, &h);

  status = load_field(&game->field, w, h, game->font);
  if (status == 0) {
    SDL_Log("load_field error...\n");
    game_quit(game);
    return 0;
  }

  game->renderer = SDL_CreateRenderer(game->window, NULL);
  if (game->renderer == NULL) {
    SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
    game_quit(game);
    return status;
  }

  game->main_menu = create_menu(
      main_menu_items, sizeof(main_menu_items) / sizeof(main_menu_items[0]));
  if (game->main_menu == NULL) {
    SDL_Log("menu_init failed.\n");
    game_quit(game);
    return status;
  }

  game->game_over_menu = create_menu(
      game_over_items, sizeof(game_over_items) / sizeof(game_over_items[0]));
  if (game->game_over_menu == NULL) {
    SDL_Log("menu_init failed.\n");
    game_quit(game);
    return status;
  }

  game->setting_menu = create_menu(setting_items, sizeof(setting_items) /
                                                      sizeof(setting_items[0]));
  if (game->setting_menu == NULL) {
    SDL_Log("menu_init failed.\n");
    game_quit(game);
    return status;
  }

  game->timer = create_timer();
  if (game->timer == NULL) {
    SDL_Log("create_timer failed\n");
    game_quit(game);
    return status;
  }

  game->background_texture =
      create_texture_from_image(game->renderer, "assets/table.png");
  if (game->background_texture == NULL) {
    SDL_Log("Background texture is NULL\n");
    game_quit(game);
    return status;
  }

  game->menu_texture =
      create_texture_from_image(game->renderer, "assets/logo.png");
  if (game->menu_texture == NULL) {
    SDL_Log("Background texture is NULL\n");
    game_quit(game);
    return status;
  }

  Uint32 event_type = SDL_RegisterEvents(1);
  if (event_type == (Uint32)-1) {
    SDL_Log("SDL_RegisterEvents failed: %s\n", SDL_GetError());
    game_quit(game);
    return status;
  }
  g_change_scene_event_type = event_type;

  game->deck = NULL;
  game->cursor = NULL;

  return 1;
}

void game_quit(GAME *game) {
  if (game->soundboard != NULL) {
    destroy_soundboard(game->soundboard);
  }
  if (game->mixer != NULL) {
    MIX_DestroyMixer(game->mixer);
  }
  if (game->renderer != NULL) {
    SDL_DestroyRenderer(game->renderer);
  }
  if (game->window != NULL) {
    SDL_DestroyWindow(game->window);
  }
  if (game->main_menu != NULL) {
    destroy_menu(game->main_menu);
  }
  if (game->game_over_menu != NULL) {
    destroy_menu(game->game_over_menu);
  }
  if (game->setting_menu != NULL) {
    destroy_menu(game->setting_menu);
  }
  if (game->font != NULL) {
    destroy_font(game->font);
  }
  if (game->deck != NULL) {
    destroy_deck(game->deck);
  }
  if (game->cursor != NULL) {
    destroy_cursor(game->cursor);
  }
  if (game->background_texture != NULL) {
    SDL_DestroyTexture(game->background_texture);
  }
  if (game->background_texture != NULL) {
    SDL_DestroyTexture(game->menu_texture);
  }
  if (game->timer != NULL) {
    destroy_timer(game->timer);
  }
  SDL_Log("End\n");
}

void run_a_game(GAME *game) {
  if (game == NULL) {
    SDL_Log("game is NULL\n");
  }

  destroy_cursor(game->cursor);
  destroy_deck(game->deck);

  game->deck = create_deck(game->renderer);
  if (game->deck == NULL) {
    SDL_Log("create_deck error\n");
    game_quit(game);
  }

  game->cursor = create_cursor(&game->field, 1, 1);
  if (game->cursor == NULL) {
    SDL_Log("create_cursor error\n");
    game_quit(game);
  }

  g_game_win = 0;
  game_update = 0;
  load_game_field(game->deck, &game->field);
  SDL_Log("je loadan\n");
}

int push_user_event(Uint32 type, Sint32 code) {
  SDL_Event event;
  memset(&event, 0, sizeof(event));
  event.type = type;
  event.user.code = code;
  return SDL_PushEvent(&event);
}

void free_resurses(char **array_of_strings) {
  for (int i = 0; i < 10; i++) {
    SDL_free(array_of_strings[i]);
  }
  SDL_free(array_of_strings);
}

int make_string_array(char **array_of_strings, int *i) {
  FILE *saves_files_bin = fopen("assets/bin/saves.bin", "rb");
  if (saves_files_bin == NULL) {
    // SDL_Log("prazan je\n");
    return 1;
  }
  char ptr;
  int index = 0;
  int prev_index = index;
  int status;
  status = fread(&ptr, sizeof(char), 1, saves_files_bin);
  if (status == 0) {
    return 0;
  }

  while (*i < 10 && ptr != EOF) {
    // printf("ptr = %d, ", ptr);
    if (ptr == '\0') {
      int n_size = index - prev_index + 1;
      // printf("n_size = %d\n", n_size);
      if (n_size == 1) {
        break;
      }
      fseek(saves_files_bin, prev_index, SEEK_SET);
      status = fread(array_of_strings[(*i)++], sizeof(char), n_size,
                     saves_files_bin);
      if (status == 0) {
        return 0;
      }
      prev_index = index + 1;
      printf("index = %d\n", index);
      printf("\n");
    }
    status = fread(&ptr, sizeof(char), 1, saves_files_bin);
    if (status == 0) {
      return 1;
    }
    index++;
  }

  fflush(stdout);
  fclose(saves_files_bin);

  return 1;
}

int make_array_of_times(char **array_of_strings, int array[], const int i) {
  const char *seconds = "name:name seconds:";
  const int jmp = (int)strlen(seconds);
  for (int j = 0; j < i; j++) {
    int num_of_digits = 1;
    while (num_of_digits <= 4 &&
           array_of_strings[j][jmp + num_of_digits] != ' ') {
      num_of_digits++;
    }
    char num_string[5];
    memcpy(num_string, &array_of_strings[j][jmp + 0], num_of_digits);
    num_string[num_of_digits] = '\0';

    // TODO: neads investigating I changed atoi -> strtoimax
    array[j] = (int)strtoimax(num_string, NULL, 10);
  }
  return 1;
}

int get_new_insert_index(const int *array, int *i, const unsigned int num) {
  int new_insert;
  for (int j = 0; j < 10; j++) {
    SDL_Log("[%d] broj je: [%d]\n", j, array[j]);
    if (array[j] == 0) {
      (*i)++;
      new_insert = j;
      return new_insert;
    }
    if (num < array[j]) {
      (*i)++;
      new_insert = j;
      return new_insert;
    }
  }
  return -1;
}

int make_new_array_of_strings(char **strings_of_array, int i, char *new_string,
                              int new_insert_index) {
  char tmp[255];
  int j = new_insert_index;
  strcpy(tmp, strings_of_array[j]);
  strcpy(strings_of_array[j++], new_string);
  for (; j < i && j < 10; j++) {
    char new_tmp[255];
    strcpy(new_tmp, strings_of_array[j]);
    strcpy(strings_of_array[j], tmp);
    strcpy(tmp, new_tmp);
  }
  for (int idx = 0; idx < 10; idx++) {
    SDL_Log("[%d] %s\n", idx, strings_of_array[idx]);
  }
  return 1;
}

int print_in_bin(char **strings_of_array, int i) {
  FILE *bin_eg = fopen("assets/bin/saves.bin", "wb+");
  for (int j = 0; j < i && j < 10; j++) {
    const int n_size = (int)strlen(strings_of_array[j]) + 1;
    fwrite(strings_of_array[j], sizeof(char), n_size, bin_eg);
  }
  fclose(bin_eg);
  return 1;
}

void save_score(const GAME *game, const char *name) {
  // TODO ovo mora da ostane zakomentarisano pre nego sto se pablisuje
  if (g_game_win != 1) {
    return;
  }

  time_t t;
  struct tm *tm_info;
  time(&t);
  tm_info = localtime(&t);
  char time_buff[80];
  strftime(time_buff, 80, "%Y-%m-%d %H:%M:%S", tm_info);

  char buff[255];
  const unsigned int time_in_sec = game->timer->time_elapsed;
  sprintf(buff, "name:%s seconds:%d mode:none time:%s", name, time_in_sec,
          time_buff);

  char **saves = SDL_malloc(sizeof(char *) * 10);
  for (int i = 0; i < 10; i++) {
    saves[i] = (char *)SDL_malloc(sizeof(char) * 255);
    memset(saves[i], 0, 255);
  }

  int i = 0;
  int status = make_string_array(saves, &i);
  if (status == 0) {
    free_resurses(saves);
    SDL_Log("make_string_array error...\n");
  }

  int best_times[10] = {0};
  status = make_array_of_times(saves, best_times, i);

  const int new_index = get_new_insert_index(best_times, &i, time_in_sec);
  if (new_index == -1) {
    free_resurses(saves);
    return;
  }
  // SDL_Log("new index: %d", new_index);

  status = make_new_array_of_strings(saves, i, buff, new_index);

  if (status == false) {
    SDL_Log("make_new_array_of_string error...\n");
    return;
  }

  status = print_in_bin(saves, i);
  if (status == false) {
    SDL_Log("print_in_bin error...\n");
  }
  free_resurses(saves);
}
