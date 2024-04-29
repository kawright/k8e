// Copyright (C) 2024  KA Wright

// clock.c - Clock for system and timer

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "clock.h"

static uint64_t _get_micros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec;
}

void init_clk(Clk *this, uint16_t freq) {
    this->freq = freq;
    this->ticks = 0;
    this->next_tick = 0;
}

void start_clk(Clk *this) {
    uint64_t period = 1000000 / this->freq;
    uint64_t micros = _get_micros();
    this->next_tick = micros + period;
}

bool update_clk(Clk *this) {
    uint64_t micros = _get_micros();
    uint64_t period = 1000000 / this->freq;
    if (micros >= this->next_tick) {
        this->ticks++;
        this->next_tick = micros + period;
        return true;
    }
    return false;
}
