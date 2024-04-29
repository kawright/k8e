// Copyright (C) 2024  KA Wright

// cpu.h - CPU operations

#ifndef __CPU_H__
#define __CPU_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "err.h"
#include "graphic.h"
#include "key.h"
#include "ram.h"

// Stores CPU registers and other state.
typedef struct __CPU__ {
    uint8_t     v_regs[16];
    uint16_t    i_reg;
    uint8_t     del_timer;
    uint8_t     snd_timer;
    uint16_t    pc;
    uint8_t     sp;
    uint16_t    stk[16];
    uint16_t    instr;
    bool        paused;
    bool        step;
} Cpu;

// Initialize a Cpu.
void init_cpu(Cpu *this);

// Perform a single Cpu operation.
void do_cpu_op(Cpu *this, Ram *ram, Win *win, KeySt *key_st, Err *err);

#endif
