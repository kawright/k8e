// Copyright (C) 2024  KA Wright

// argv.h - Argument vector parsing.

#ifndef __ARGV_H__
#define __ARGV_H__

#include <stdbool.h>
#include <stdint.h>

#include "err.h"

#define ARGV_OPTSTR             "ab:B:c:F:dhl:mpP:t:"
#define ARGV_MAX_BRKPTS         16

// Stores parsed command-line args as fields.
typedef struct __ARGV__ {
    bool        about;
    uint16_t    brkpts[16];
    uint8_t     brkpts_len;
    uint32_t    bg;
    uint32_t    fg;
    uint16_t    clk_freq;
    bool        debug;
    bool        help;
    char        *svst;
    bool        mute;
    bool        paused;
    uint16_t    pitch;
    uint8_t     px_sz;
    char        *fname;
} Argv;

// Initialize an Argv.
void init_argv(Argv *this);

// Parse an arg vector.
void parse_argv(Argv *this, uint16_t argc, char *argv[], Err *err);

#endif
