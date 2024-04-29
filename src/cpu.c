// Copyright (C) 2024  KA Wright

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "err.h"
#include "graphic.h"
#include "key.h"
#include "ram.h"
#include "sound.h"

void init_cpu(Cpu *this) {
    for (uint8_t i = 0; i < 16; i++) {
        this->v_regs[i] = 0;
        this->stk[i] = 0;
    }
    this->i_reg = 0;
    this->del_timer = 0;
    this->snd_timer = 0;
    this->pc = 0;
    this->sp = 0;
    this->instr = 0;
    this->paused = false;
    this->step = false; 
}

void do_cpu_op(Cpu *this, Ram *ram, Win *win, KeySt *key_st, Err *err) {
    uint8_t     nib_a;
    uint8_t     nib_b;
    uint8_t     nib_c;
    uint8_t     nib_d;
    uint8_t     kk;
    uint16_t    nnn;
    uint8_t     temp_reg;
    uint8_t     temp_byte;

    if (this->paused && !this->step) {
        return;
    }    
    if (this->step) {
        this->step = false;
    }

    this->instr = (ram->data[this->pc] << 8) + ram->data[this->pc + 1];

    nib_a   = (this->instr & 0xf000) >> 12;
    nib_b   = (this->instr & 0x0f00) >> 8;
    nib_c   = (this->instr & 0x00f0) >> 4;
    nib_d   = (this->instr & 0x000f);
    kk      = (nib_c << 4) + nib_d;
    nnn     = (nib_b << 8) + kk;

    switch (nib_a) {
        
        case 0x0:
        
        // 00e0 - CLS
        if (nnn == 0x0e0) {
            clear_win(win, err);
            if (is_err(err)) return;
            redraw_win(win); 
            break;
        }
 
        // 00ee - RET
        if (nnn == 0x0ee) {
            this->pc = this->stk[this->sp];
            if (this->sp == 0) {
                err->code = ERR_RANGE;
                strcpy(err->msg, "Stack pointer cannot be negative");
                return;
            }
            this->sp--;
            break;
        }
        
        // 0nnn - SYS addr (Treat as NOP)
        break;


        // 1nnn - JP addr
        case 0x1:
        this->pc = nnn - 2;
        break;

        
        // 2nnn - CALL addr
        case 0x2:
        this->sp++;
        if (this->sp >= 16) {
            err->code = ERR_RANGE;
            strcpy(err->msg, "Stack limit exceeded");
            return;
        }
        this->stk[this->sp] = this->pc;
        this->pc = nnn - 2;
        break;

        // 3xkk - SE Vx, byte
        case 0x3:
        if (this->v_regs[nib_b] == kk) {
            this->pc += 2;
        }
        break;

        // 4xkk - SNE Vx, byte
        case 0x4:
        if (this->v_regs[nib_b] != kk) {
            this->pc += 2;
        }
        break;

        // 5xy0 - SE Vx, Vy
        case 0x5:
        if (this->v_regs[nib_b] == this->v_regs[nib_c]) {
            this->pc += 2;
        }
        break;

        // 6xkk - LD Vx, byte
        case 0x6:
        this->v_regs[nib_b] = kk;
        break;

        // 7xkk - ADD Vx, byte
        case 0x7:
        this->v_regs[nib_b] += kk;
        break;

        case 0x8:
        switch (nib_d) {
            
            // 8xy0 - LD Vx, Vy
            case 0x0:
            this->v_regs[nib_b] = this->v_regs[nib_c];
            break;

            // 8xy1 - OR Vx, Vy
            case 0x1:
            this->v_regs[nib_b] |= this->v_regs[nib_c];
            break;

            // 8xy2 - AND Vx, Vy
            case 0x2:
            this->v_regs[nib_b] &= this->v_regs[nib_c];
            break;

            // 8xy3 - XOR Vx, Vy
            case 0x3:
            this->v_regs[nib_b] ^= this->v_regs[nib_c];
            break;

            // 8xy4 - ADD Vx, Vy
            case 0x4:
            if (this->v_regs[nib_b] + this->v_regs[nib_c] > 255) {
                this->v_regs[0xf] = 1;
            } else {
                this->v_regs[0xf] = 0;
            }
            this->v_regs[nib_b] += this->v_regs[nib_c];
            break;

            // 8xy5 - SUB Vx, Vy
            case 0x5:
            if (this->v_regs[nib_b] > this->v_regs[nib_c]) {
                this->v_regs[0xf] = 1;
            } else {
                this->v_regs[0xf] = 0;
            }
            this->v_regs[nib_b] -= this->v_regs[nib_c];
            break;

            // 8xy6 - SHR Vx, {Vy}
            case 0x6:
            if ((this->v_regs[nib_b] & 1) == 1) {
                this->v_regs[0xf] = 1;
            }
            this->v_regs[nib_b] >>= 1;
            break;

            // 8xy7 - SUBN Vx, Vy
            case 0x7:
            if (this->v_regs[nib_c] > this->v_regs[nib_b]) {
                this->v_regs[0xf] = 1;
            } else {
                this->v_regs[0xf] = 0;
            }
            this->v_regs[nib_b] = this->v_regs[nib_c] - this->v_regs[nib_b];
            break;

            // 8xye - SHL Vx, {Vy}
            case 0xe:
            if ((this->v_regs[nib_b] & 0x80) == 0x80) {
                this->v_regs[0xf] = 1;
            } else {
                this->v_regs[0xf] = 0;
            }
            this->v_regs[nib_b] <<= 1;
            break;

            // Illegal opcode
            default:
            break;

        }
        break;

        // 9xy0 - SNE Vx, Vy
        case 0x9:
        if (this->v_regs[nib_b] != this->v_regs[nib_c]) {
            this->pc += 2;
        }
        break;

        // annn - LD I, addr
        case 0xa:
        this->i_reg = nnn;
        break;

        // bnnn - JP V0, addr
        case 0xb:
        this->pc += (this->v_regs[0] + nnn - 2);
        break;

        // cxkk - RND Vx, byte
        case 0xc:
        this->v_regs[nib_b] = ((uint8_t) rand()) & kk;
        break;

        // dxyn - DRW Vx, Vy, nibble
        case 0xd:
        temp_reg = this->v_regs[nib_c];
        this->v_regs[0xf] = 0;
        for (uint16_t addr = this->i_reg; addr < (this->i_reg + nib_d); 
                addr++) {
            temp_byte = ram->data[addr]; 
            if (temp_byte & 0x80) {
                if (draw_px(win, this->v_regs[nib_b], temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x40) {
                if (draw_px(win, this->v_regs[nib_b]+1, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x20) {
                if (draw_px(win, this->v_regs[nib_b]+2, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x10) {
                if (draw_px(win, this->v_regs[nib_b]+3, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x8) {
                if (draw_px(win, this->v_regs[nib_b]+4, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x4) {
                if (draw_px(win, this->v_regs[nib_b]+5, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x2) {
                if (draw_px(win, this->v_regs[nib_b]+6, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            if (temp_byte & 0x1) {
                if (draw_px(win, this->v_regs[nib_b]+7, temp_reg, err)) {
                    this->v_regs[0xf] = 1;
                }
                if (is_err(err)) return;
                redraw_win(win);
            }
            temp_reg++;
        }
        break; 

        case 0xe:
        switch (kk) {
            
            // ex9e - SKP Vx
            case 0x9e:
            if (read_key(key_st, this->v_regs[nib_b])) {
                this->pc += 2;  
            }
            break;

            // exa1 - SKNP Vx
            case 0xa1:
            if (!read_key(key_st, this->v_regs[nib_b])) {
                this->pc += 2;
            }
            break;

            // Illegal opcode
            default:
        }
        break;

        case 0xf:
        switch (kk) {

            // fx07 - LD Vx, DT
            case 0x07:
            this->v_regs[nib_b] = this->del_timer;
            break;

            // fx0a - LD Vx, K
            case 0x0a:
            this->v_regs[nib_b] = poll_key();
            break;

            // fx15 - LD DT, Vx
            case 0x15:
            this->del_timer = this->v_regs[nib_b];
            break;

            // fx18 - LD ST, Vx
            case 0x18:
            this->snd_timer = this->v_regs[nib_b];
            break;

            // fx1e - ADD I, Vx
            case 0x1e:
            this->i_reg += this->v_regs[nib_b];
            break;

            // fx29 - LD F, Vx
            case 0x29:
            switch (this->v_regs[nib_b]) {

                case 0x0:
                this->i_reg = ADDR_SPRITE_0;
                break;

                case 0x1:
                this->i_reg = ADDR_SPRITE_1;
                break;

                case 0x2:
                this->i_reg = ADDR_SPRITE_2;
                break;
                
                case 0x3:
                this->i_reg = ADDR_SPRITE_3;
                break;

                case 0x4:
                this->i_reg = ADDR_SPRITE_4;
                break;

                case 0x5:
                this->i_reg = ADDR_SPRITE_5;
                break;

                case 0x6:
                this->i_reg = ADDR_SPRITE_6;
                break;

                case 0x7:
                this->i_reg = ADDR_SPRITE_7;
                break;

                case 0x8:
                this->i_reg = ADDR_SPRITE_8;
                break;
        
                case 0x9:
                this->i_reg = ADDR_SPRITE_9;
                break;

                case 0xa:
                this->i_reg = ADDR_SPRITE_A;
                break;

                case 0xb:
                this->i_reg = ADDR_SPRITE_B;
                break;

                case 0xc:
                this->i_reg = ADDR_SPRITE_C;
                break;

                case 0xd:
                this->i_reg = ADDR_SPRITE_D;
                break;

                case 0xe:
                this->i_reg = ADDR_SPRITE_E;
                break;

                case 0xf:
                this->i_reg = ADDR_SPRITE_F;
                break;

                default:

            }
            break;

            // fx33 - LD B, Vx
            case 0x33:
            if (this->i_reg >= (ADDR_PROG_END - 2)) {
                err->code = ERR_RANGE;
                strcpy(err->msg, "Out-of-bounds RAM access.");
                return;
            }
            uint8_t ones = this->v_regs[nib_b] % 10;
            uint8_t tens = (this->v_regs[nib_b] % 100) / 10;
            uint8_t cents = this->v_regs[nib_b] / 100;
            ram->data[this->i_reg] = cents;
            ram->data[this->i_reg+1] = tens;
            ram->data[this->i_reg+2] = ones;
            break;

            // fx55 - LD [I], Vx
            case 0x55:
            for (int i = 0; i <= nib_b; i++) {
                if ((this->i_reg + i) >= ADDR_PROG_END) {
                    err->code = ERR_RANGE;
                    strcpy(err->msg, "Out-of-bounds RAM access.");
                    return;
                }
                ram->data[this->i_reg+i] = this->v_regs[i];
            }
            break;

            // fx65 - LD Vx, [I]
            case 0x65:
            for (int i = 0; i <= nib_b; i++) {
                if ((this->i_reg + i) >= ADDR_PROG_END) {
                    err->code = ERR_RANGE;
                    strcpy(err->msg, "Out-of-bounds RAM access.");
                }
                this->v_regs[i] = ram->data[this->i_reg+i];
                if (is_err(err)) {
                    return;
                }
            }            
            break;

            default:
            break;
            
        }
        break;
    }

    this->pc += 2;
    

    return;
}
