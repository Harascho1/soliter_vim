#ifndef SOUND_H
#define SOUND_H

#include "SDL3/SDL_audio.h"
#include "SDL3_mixer/SDL_mixer.h"
typedef struct _SOUNDBOARD {
    Mix_Chunk **sounds;
    int lenght;
    SDL_AudioDeviceID devid;
    pthread_t thread;
} SOUNDBOARD;

SOUNDBOARD* create_soundboard(SDL_AudioDeviceID devid);
void destroy_soundboard(SOUNDBOARD *sb);

int play_sound(SOUNDBOARD *soundboard, int index);

#endif
