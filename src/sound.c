// Copyright (C) 2024  KA Wright

// sound.h - Sound operations

#include <stdlib.h>
#include <string.h>

#include <alsa/asoundlib.h>
#include <math.h>

#include "err.h"
#include "sound.h"

#define PCM_RATE        44100
#define DUR             6           // Longer than full sound timer
#define CHANNELS        1
#define RESAMPLE        1
#define LATENCY         5000000

void init_snd(Snd *this, uint32_t freq, bool muted) {
    this->muted = muted;
    this->freq = freq;
    for (long i=0; i<sizeof(this->wav); i++) {
        this->wav[i] = 0xff * sin(2 * M_PI * this->freq * i / PCM_RATE);
    }
    this->pcm_dev = NULL;
    this->playing = false;
}

void play_snd(Snd *this, Err *err) {
    if (!this->muted && !this->playing) {
        if (snd_pcm_open(&this->pcm_dev, "default", SND_PCM_STREAM_PLAYBACK,
                SND_PCM_NONBLOCK) != 0) {
            err->code = ERR_INIT;
            strcpy(err->msg, "Could not open PCM device");
            return;
        }
        snd_pcm_set_params(this->pcm_dev, SND_PCM_FORMAT_U8, 
            SND_PCM_ACCESS_RW_INTERLEAVED, CHANNELS, PCM_RATE, RESAMPLE, 
            LATENCY);
        if (snd_pcm_writei(this->pcm_dev, this->wav, sizeof(this->wav)) < 0) {
            err->code = ERR_SUBSYS;
            strcpy(err->msg, "Could not play sound");
            return;
        }
        this->playing = true;

    }
}

void stop_snd(Snd *this) {
    if (!this->muted && this->playing) {
        snd_pcm_drop(this->pcm_dev);
        snd_pcm_close(this->pcm_dev);
        this->pcm_dev = NULL;
        this->playing = false;
    }
}
