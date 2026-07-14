#include "log.h"
#include "SDL3/SDL_log.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static FILE* g_log_file = NULL;

#ifdef DEBUG
static void SDLCALL file_output_callback(
  void* userdata, int category, SDL_LogPriority priority, const char* message
) {
  (void)userdata;
  (void)category;
  (void)priority;
  if (g_log_file != NULL) {
    fprintf(g_log_file, "%s\n", message);
    fflush(g_log_file);
  }
}
#else
static void SDLCALL discard_output(
  void* userdata, int category, SDL_LogPriority priority, const char* message
) {
  (void)userdata;
  (void)category;
  (void)priority;
  (void)message;
}
#endif

void log_init(int argc, char** argv) {
#ifdef DEBUG
  bool to_file = false;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--log-file") == 0) {
      to_file = true;
    } else if (strcmp(argv[i], "--log-console") == 0) {
      to_file = false;
    }
  }

  if (to_file) {
    g_log_file = fopen("sovimter.log", "w");
    if (g_log_file != NULL) {
      SDL_SetLogOutputFunction(file_output_callback, NULL);
    } else {
      SDL_Log("could not open sovimter.log, logging to console instead");
    }
  }
#else
  (void)argc;
  (void)argv;
  SDL_SetLogOutputFunction(discard_output, NULL);
#endif
}

void log_quit(void) {
  if (g_log_file != NULL) {
    fclose(g_log_file);
    g_log_file = NULL;
  }
}
