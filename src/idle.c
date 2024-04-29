// Copyright (C) 2024  KA Wright

// idle.c - Idle loop


#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "clean.h"
#include "clock.h"
#include "cpu.h"
#include "err.h"
#include "idle.h"
#include "key.h"
#include "ram.h"
#include "savest.h"

void do_idle_loop(Clk *clk, KeySt *key_st, Cpu *cpu, Ram *ram, Win *win, 
        Err *err) {
    
    bool    d_press             = false;
    bool    x_press             = false;
    bool    r_press             = false;
    bool    s_press             = false;
    bool    t_press             = false;
    char    sv_st_fname[31]; 
    SvSt    sv_st;
    
    init_sv_st(&sv_st);

    while (!update_clk(clk)) {
        update_key_st(key_st);

        if (read_key(key_st, 'q')) {
            err->code = ERR_QUIT;
            return; 
        }

        if (read_key(key_st, 'x')) {
            if (x_press) continue;
            x_press = true;
            cpu->paused = true;
        } else {
            x_press = false;
        }

        if (read_key(key_st, 'r')) {
            if (r_press) continue;
            r_press = true;
            cpu->paused = false;
        } else {
            r_press = false;
        }

        if (read_key(key_st, 's')) {
            if (s_press) continue;
            s_press = true;
            cpu->step = true;
        } else {
            s_press = false;
        }

        if (read_key(key_st, 'd')) {
            if (d_press) continue;
            d_press = true;
            dump_ram(ram, err);
            if (is_err(err)) return;
        } else {
            d_press = false;
        }

        if (read_key(key_st, 't')) {
            if (t_press) continue;
            t_press = true;
            dump_sv_st(&sv_st, cpu, win, ram);
            sprintf(sv_st_fname, "savestate_%lu.k8e", 
                (unsigned long) time(NULL));
            sv_sv_st(&sv_st, sv_st_fname, err);
            if (is_err(err)) return;
        } else {
            t_press = false;
        }
    }   
}
