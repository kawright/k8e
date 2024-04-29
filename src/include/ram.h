// Copyright (C) 2024  KA Wright

// ram.h - RAM operations

#ifndef __RAM_H__
#define __RAM_H__

#include "err.h"

#define SPRITE_LEN              5
#define ADDR_PROG_START         0x200
#define ADDR_PROG_END           0xfff
#define ADDR_SPRITE_0           0x000
#define ADDR_SPRITE_1           0x005
#define ADDR_SPRITE_2           0x00a
#define ADDR_SPRITE_3           0x00f
#define ADDR_SPRITE_4           0x014
#define ADDR_SPRITE_5           0x019
#define ADDR_SPRITE_6           0x01e
#define ADDR_SPRITE_7           0x023
#define ADDR_SPRITE_8           0x028
#define ADDR_SPRITE_9           0x02d
#define ADDR_SPRITE_A           0x032
#define ADDR_SPRITE_B           0x037
#define ADDR_SPRITE_C           0x03c
#define ADDR_SPRITE_D           0x041
#define ADDR_SPRITE_E           0x046
#define ADDR_SPRITE_F           0x04b

// Stores system RAM.
typedef struct __RAM__ {
    uint8_t     data[ADDR_PROG_END + 1];
    uint16_t    prog_len;
} Ram;

// Initialize a Ram.
void init_ram(Ram *this);

// Dump a Ram to file.
void dump_ram(Ram *this, Err *err);

// Load character data into a Ram.
void ld_ram_char(Ram *this);

// Load a binary file into a Ram.
void ld_ram(Ram *this, char *fname, Err *err);

// Reset a Ram.
void reset_ram(Ram *this);

#endif
