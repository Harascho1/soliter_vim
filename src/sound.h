#ifndef SOUND_H
#define SOUND_H

#include "SDL3/SDL_audio.h"
#include "SDL3_mixer/SDL_mixer.h"

enum {
    click_sound = 0,
    select_card_sound,
    draw_card_sound,
    blip_select_sound,
    sort_card_sound,
    place_card_sound,
    move_sound
};

typedef struct _SOUNDBOARD {
    MIX_Audio **sounds;
    int lenght;
    int count;
    MIX_Mixer *mixer;
} SOUNDBOARD;

SOUNDBOARD* create_soundboard(MIX_Mixer *mixer);
void destroy_soundboard(SOUNDBOARD *sb);

int play_sound(SOUNDBOARD *soundboard, int index);

#endif
