#include "sound.h"
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "config.h"
#include <pthread.h>

const int QUEUE_SOUNDS = 3;
const int num_of_sounds = 7;
static const char* paths_to_sounds[] = {
    "assets/sounds/click_sound.wav",
    "assets/sounds/select_card_sound.wav",
    "assets/sounds/draw_card_sound.wav",
    "assets/sounds/blip_select_sound.wav",
    "assets/sounds/sort_card_sound.wav",
    "assets/sounds/place_card_sound.wav",
    "assets/sounds/move_sound.wav"
};

SOUNDBOARD*
create_soundboard(SDL_AudioDeviceID devid) {
    SOUNDBOARD *sb = (SOUNDBOARD*)SDL_malloc(sizeof(SOUNDBOARD));
    sb->lenght = num_of_sounds;
    sb->devid = devid;
    sb->count = 0;
    sb->sounds = (Mix_Chunk**)SDL_malloc(sizeof(Mix_Chunk*) * sb->lenght);
    for (int i = 0; i < num_of_sounds; ++i) {
        sb->sounds[i] = Mix_LoadWAV(paths_to_sounds[i]);
        if (sb->sounds[i] == NULL) {
            SDL_Log("sb->sounds[%d] je NULL i evo greske %s\n", i, SDL_GetError());
            return NULL;
        }
    }
    return sb;
}

void
destroy_soundboard(SOUNDBOARD *sb) {
    //TODO ne moze ovo jer ne moze posle da se obrise ako se ni jednom nije koristio sound
    //pthread_join(sb->thread, NULL);
    if (sb == NULL) {
        SDL_Log("sb is NULL");
        return;
    } 
    if (sb->sounds != NULL) {
        for (int i = 0; i < sb->lenght; i++) {
            Mix_FreeChunk(sb->sounds[i]);
        }
        SDL_free(sb->sounds);
    }
    SDL_free(sb);
}

typedef struct _packet {
    SDL_AudioDeviceID devid;
    Mix_Chunk *sound;
} packet;

void*
play_sound_on_thread(void *args) {
    packet *tmp = (packet *) args;
    Mix_Chunk *sound = (Mix_Chunk*)tmp->sound;
    if (sound == NULL) {
        SDL_Log("tmp is NULL\n");
        return NULL;
    }
    int status = Mix_PlayChannel(-1, sound, 0);
    if (status == -1) {
        SDL_Log("Mix_PlayChannel error: %s\n", SDL_GetError());
        return NULL;
    }
    return (void*)sound;
}

int
play_sound(SOUNDBOARD *sb, int index) {
    Mix_Volume(-1, config_options[2]);
    if (config_options[1] == 0) {
        return 1;
    }
    if (sb == NULL) {
        SDL_Log("SoundBoard is NULL\n");
        return 0;
    }
    if (sb->count > 0) {
        pthread_join(sb->thread, NULL);
        sb->count--;
    }
    if (index < 0 || index > sb->lenght - 1) {
        SDL_Log("Invalid index\n");
        return 0;
    }
    if (sb->count >= QUEUE_SOUNDS) {
        return 1;
    }
    packet pckg = {sb->devid, sb->sounds[index]};
    void* tmp = play_sound_on_thread((void*) &pckg);
    //pthread_create(&sb->thread, NULL, play_sound_on_thread, (void*)(&pckg));
    sb->count++;
    return 1;
}
