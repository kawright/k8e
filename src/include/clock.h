// Copyright (C) 2024  KA Wright

// clock.h - Clock for system and timers

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdbool.h>
#include <stdint.h>

// Stores a single clock. One should be used for system time, and another for
// the delay and sound timers.
typedef struct __CLK__ {
    uint16_t    freq;
    uint64_t    ticks;
    uint64_t    next_tick;
} Clk;

// Initialize a clock with given frequency.
void init_clk(Clk *this, uint16_t freq);

// Start a clock.
void start_clk(Clk *this);

// Update a clock. Return data indicates if the clock ticked.
bool update_clk(Clk *this);

#endif
