// Copyright (C) 2024  KA Wright

// savest.h - Savestate operations.

#ifndef __SAVEST_H__
#define __SAVEST_H__

#include <stdint.h>

#include "cpu.h"
#include "err.h"
#include "graphic.h"
#include "ram.h"

// Models the structure of a savestate file.
typedef struct __SV_ST__ {
    char        head[4];
    uint8_t     v_regs[16];
    uint16_t    i_reg;
    uint8_t     del_timer;
    uint8_t     snd_timer;
    uint16_t    pc;
    uint8_t     sp;
    uint16_t    stk[16];
    uint8_t     ram[4096];
    uint8_t     vid[8][32];
    char        foot[4];
} SvSt;

// Initialize a SvSt.
void init_sv_st(SvSt *this);

// Dump the current system state to a SvSt object.
void dump_sv_st(SvSt *this, const Cpu *cpu, const Win *win, const Ram *ram); 

// Load a savestate file into a SvSt object.
void ld_sv_st(SvSt *this, const char *fname, Err *err);

// Save a SvSt object to file.
void sv_sv_st(const SvSt *this, const char *fname, Err *err);

// Update the system state based on a SvSt.
void apply_sv_st(const SvSt *this, Cpu *cpu, Win *win, Ram *ram, Err *err);

#endif
