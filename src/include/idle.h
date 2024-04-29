// Copyright (C) 2024  KA Wright

// idle.h - Idle loop

#ifndef __IDLE_H__
#define __IDLE_H__

#include "clock.h"
#include "cpu.h"
#include "err.h"
#include "key.h"

// Perform the idle loop until the next system clock tick.
void do_idle_loop(Clk *clk, KeySt *key_st, Cpu *cpu, Ram *ram, Win *win, 
    Err *err);

#endif
