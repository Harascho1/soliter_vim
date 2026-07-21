#include "sound.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "config.h"
#include "res.h"

static const int QUEUE_SOUNDS = 3;
static const int num_of_sounds = 7;

SOUNDBOARD* create_soundboard(MIX_Mixer* mixer) {
  SOUNDBOARD* sb = SDL_malloc(sizeof(SOUNDBOARD));
  sb->lenght = num_of_sounds;
  sb->mixer = mixer;
  sb->count = 0;
  sb->sounds = (MIX_Audio**)SDL_malloc(sizeof(MIX_Audio*) * sb->lenght);
  for (int i = 0; i < num_of_sounds; ++i) {
    sb->sounds[i] = MIX_LoadAudio(mixer, paths.sounds[i], true);
    if (sb->sounds[i] == NULL) {
      SDL_Log("sb->sounds[%d] je NULL i evo greske %s\n", i, SDL_GetError());
      return NULL;
    }
  }
  return sb;
}

void destroy_soundboard(SOUNDBOARD* sb) {
  // TODO ne moze ovo jer ne moze posle da se obrise ako se ni jednom nije
  // koristio sound pthread_join(sb->thread, NULL);
  if (sb == NULL) {
    SDL_Log("Soundboard is NULL");
    return;
  }
  if (sb->sounds != NULL) {
    for (int i = 0; i < sb->lenght; i++) {
      MIX_DestroyAudio(sb->sounds[i]);
    }
    SDL_free((MIX_Audio*)sb->sounds);
  }
  SDL_free(sb);
}

typedef struct {
  MIX_Mixer* mixer;
  MIX_Audio* sound;
} packet;

void* play_sound_on_thread(void* args) {
  const packet* tmp = (packet*)args;
  MIX_Audio* sound = (MIX_Audio*)tmp->sound;
  if (sound == NULL) {
    SDL_Log("tmp is NULL\n");
    return NULL;
  }
  if (!MIX_PlayAudio(tmp->mixer, sound)) {
    SDL_Log("MIX_PlayAudio error: %s\n", SDL_GetError());
    return NULL;
  }
  return (void*)sound;
}

bool play_sound(SOUNDBOARD* soundboard, const int index) {
  if (soundboard == NULL) {
    SDL_Log("Soundboard is NULL\n");
    return false;
  }

  MIX_SetMixerGain(soundboard->mixer, (float)config_options[2] / 128.0F);
  if (config_options[1] == 0) {
    return true;
  }

  if (soundboard->count > 0) {
    // pthread_join(sb->thread, NULL);
    soundboard->count--;
  }

  if (index < 0 || index > soundboard->lenght - 1) {
    SDL_Log("Invalid index\n");
    return false;
  }

  if (soundboard->count >= QUEUE_SOUNDS) {
    return true;
  }

  packet pckg = {soundboard->mixer, soundboard->sounds[index]};
  play_sound_on_thread((void*)&pckg);
  // pthread_create(&sb->thread, NULL, play_sound_on_thread, (void*)(&pckg));
  soundboard->count++;

  return true;
}
