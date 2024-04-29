// Copyright (C) 2024  KA Wright

// main.c - Kristoffer's Chip-8 Emulator; entry point

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "argv.h"
#include "asset.h"
#include "clean.h"
#include "clock.h"
#include "cpu.h"
#include "err.h"
#include "graphic.h"
#include "idle.h"
#include "key.h"
#include "ram.h"
#include "savest.h"
#include "sound.h"

#define TIMER_RATE      60      // Hz

// Entry Point
int main(int argc, char *argv[]) {
    Argv        argv_obj;
    Clk         sys_clk;
    Clk         timer_clk;
    Cpu         cpu;
    Err         err;
    KeySt       key_st;
    Ram         ram;
    Snd         snd;
    Win         win;

    // Argv Parsing
    init_argv(&argv_obj);
    init_err(&err);
    parse_argv(&argv_obj, argc, argv, &err);
    if (is_err(&err)) {
        err_alert(&err);
        return err.code;
    }
    
    // Struct Initialization
    init_cpu(&cpu);
    init_key_st(&key_st);
    init_ram(&ram);
    init_clk(&timer_clk, TIMER_RATE);
    init_win(&win, argv_obj.bg, argv_obj.fg, argv_obj.px_sz);
    init_clk(&sys_clk, argv_obj.clk_freq);
    init_snd(&snd, argv_obj.pitch, argv_obj.mute);    

    // Static Output Options
    if (argv_obj.about) {
        char about_txt[MAX_ASSET_ABOUT_SZ];
        ld_asset_about(about_txt);
        printf("\n%s\n", about_txt);
        return ERR_OK;
    }
    if (argv_obj.help) {
        char help_txt[MAX_ASSET_HELP_SZ];
        ld_asset_help(help_txt);
        printf("\n%s\n", help_txt);
        return ERR_OK;
    }

    // Resource Initialization and Setup
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        err.code = ERR_INIT;
        strcpy(err.msg, "Could not initialize SDL subsystem.");
        err_alert(&err);
        return err.code;
    }
    ld_ram_char(&ram);
    ld_ram(&ram, argv_obj.fname, &err);
    open_win(&win, &err);
    if (is_err(&err)) {
        err_alert(&err);
        return err.code;
    }
    clear_win(&win, &err);
    if (is_err(&err)) {
        err_alert(&err);
        clean_res(&win);
        return err.code;
    }
    redraw_win(&win);
    start_clk(&sys_clk);
    start_clk(&timer_clk);
    printf("\e[?25l");          // Hide cursor 

    // Initial Pause
    if (argv_obj.paused || (argv_obj.svst != NULL)) {
        cpu.paused = true;
    }

    // Load Savestate
    if (argv_obj.svst != NULL) {
        SvSt sv_st;
        init_sv_st(&sv_st);
        ld_sv_st(&sv_st, argv_obj.svst, &err);
        if (is_err(&err)) {
            err_alert(&err);
            clean_res(&win);
            return err.code;
        }
        apply_sv_st(&sv_st, &cpu, &win, &ram, &err);
        if (is_err(&err)) {
            err_alert(&err);
            clean_res(&win);
            return err.code;
        }
    }

    /***** MAIN PROGRAM LOOP *****/
    while (true) {
        
        // Pause at Breakpoint
        for (int i=0; i<argv_obj.brkpts_len; i++) {
            if (argv_obj.brkpts[i] == cpu.pc) {
                cpu.paused = true;
                break;
            }
        }
       
        // Idle Loop (Timing, Input, Savestate & RAM Dump...) 
        do_idle_loop(&sys_clk, &key_st, &cpu, &ram, &win, &err);
        if (is_err(&err)) {
            if (err.code != ERR_QUIT) err_alert(&err);
            clean_res(&win);
            return err.code;
        }
        if (update_clk(&timer_clk)) {
            if (cpu.del_timer > 0) cpu.del_timer--;
            if (cpu.snd_timer > 0) {
                cpu.snd_timer--;
            } else {
                stop_snd(&snd);
            }
        }

        // Execute CPU Instruction
        do_cpu_op(&cpu, &ram, &win, &key_st, &err);
        if (is_err(&err)) {
            err_alert(&err);
            clean_res(&win);
            return err.code;
        }
        if (cpu.snd_timer > 0) {
            play_snd(&snd, &err);
            if (is_err(&err)) {
                err_alert(&err);
                clean_res(&win);
                return err.code;
            }
        }

        // Debug Output
        if (argv_obj.debug) {
            printf("PC:%03x  INSTR:%04x  V:%02x %02x %02x %02x %02x %02x "
                "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x  D:%02x  "
                "S:%02x  SP:%02x  STK:%03x\r", cpu.pc, cpu.instr, 
                cpu.v_regs[0x0], cpu.v_regs[0x1], cpu.v_regs[0x2], 
                cpu.v_regs[0x3], cpu.v_regs[0x4], cpu.v_regs[0x5], 
                cpu.v_regs[0x6], cpu.v_regs[0x7], cpu.v_regs[0x8], 
                cpu.v_regs[0x9], cpu.v_regs[0xa], cpu.v_regs[0xb], 
                cpu.v_regs[0xc], cpu.v_regs[0xd], cpu.v_regs[0xe], 
                cpu.v_regs[0xf], cpu.del_timer, cpu.snd_timer, cpu.sp, 
                cpu.stk[cpu.sp]);    
        }
    }   

}
