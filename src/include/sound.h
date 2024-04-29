// Copyright (C) 2024  KA Wright

// sound.h - Sound operations

#ifndef __SOUND_H__
#define __SOUND_H__

#include <stdbool.h>
#include <stdint.h>

#include <alsa/asoundlib.h>

#include "err.h"

#define PCM_RATE    44100
#define DUR         6           // Longer than full sound timer

// Stores sound playback configuration.
typedef struct __SND__ {
    bool        muted;
    uint32_t    freq;
    uint8_t     wav[PCM_RATE * DUR];
    snd_pcm_t   *pcm_dev;
    bool        playing;
} Snd;

// Initialize a Snd.
void init_snd(Snd *this, uint32_t freq, bool muted);

// Play the PC speaker at the given pitch.
void play_snd(Snd *this, Err *err);

// Stop PC speaker sounds.
void stop_snd(Snd *this);

#endif
