#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#endif