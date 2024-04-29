// Copyright (C) 2024  KA Wright

// savest.c - Savestate operations.


#include <stdint.h>

#include "cpu.h"
#include "err.h"
#include "graphic.h"
#include "ram.h"
#include "savest.h"

void init_sv_st(SvSt *this) {
    for (int i=0; i<4; i++) {
        this->head[i] = 0;
    }
    for (int i=0; i<16; i++) {
        this->v_regs[i] = 0;
    }
    this->i_reg = 0;
    this->del_timer = 0;
    this->snd_timer = 0;
    this->pc = 0;
    this->sp = 0;
    for (int i=0; i<16; i++) {
        this->stk[i] = 0;
    }
    for (int i=0; i<4096; i++) {
        this->ram[i] = 0;
    }
    for (int y=0; y<32; y++) {
        for (int x=0; x<8; x++) {
            this->vid[x][y] = 0;
        }
    }
    for (int i=0; i<4; i++) {
        this->foot[i] = 0;
    }
}

void dump_sv_st(SvSt *this, const Cpu *cpu, const Win *win, const Ram *ram) {
    uint8_t     temp_byte;

    strcpy(this->head, "K8E");
    for (int i=0; i<=0xf; i++) {
        this->v_regs[i] = cpu->v_regs[i];
    }
    this->i_reg = cpu->i_reg;
    this->del_timer = cpu->del_timer;
    this->snd_timer = cpu->snd_timer;
    this->pc = cpu->pc;
    this->sp = cpu->sp;
    for (int i=0; i<16; i++) {
        this->stk[i] = cpu->stk[i];
    }
    for (int i=0; i<4096; i++) {
        this->ram[i] = ram->data[i];
    }
    for (int y=0; y<32; y++) {
        
        // Compress rows into 1-bit-per-pixel
        for (int x=0; x<8; x++) {
            temp_byte = 0;
            for (int i=7; i>=0; i--) {
                temp_byte += win->px_map[(x*8)+i][y] ? 0x1<<i : 0;
            }
            this->vid[x][y] = temp_byte;
        }
    }
    strcpy(this->foot, "FIN");
} 

void ld_sv_st(SvSt *this, const char *fname, Err *err) {
    uint8_t     temp_byte;
    uint16_t    temp_word;

    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not open file %s", fname);
        return;
    }
    for (int i=0; i<4; i++) {
        this->head[i] = fgetc(fp);
    } 
    if (strcmp(this->head, "K8E") != 0) {
        err->code = ERR_DATA;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "File %s is invalid or malformed - Header", fname);
        fclose(fp);
        return;
    }
    for (int i=0; i<16; i++) {
        if (fread(&temp_byte, 1, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN, 
                "Could not read file %s - V Registers", fname);
            fclose(fp);
            return;
        }
        this->v_regs[i] = temp_byte;   
    }
    
    // I Registers
    if (fread(&temp_word, 2, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not read file %s - I Register", fname);
        fclose(fp);
        return;
    }
    this->i_reg = temp_word;

    // Delay Timer
    if (fread(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not read file %s - Delay Timer", fname);
        fclose(fp);
        return;
    }
    this->del_timer = temp_byte;
    
    // Sound Timer
    if (fread(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not read file %s - Sound Timer", fname);
        fclose(fp);
        return;
    }
    this->snd_timer = temp_byte;

    // PC
    if (fread(&temp_word, 2, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s - PC", 
            fname);
        fclose(fp);
        return;
    }
    this->pc = temp_word;

    // SP
    if (fread(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s - SP", 
            fname);
        fclose(fp);
        return;
    }
    this->sp = temp_byte;

    // Stack
    for (int i = 0; i < 16; i++) {
        if (fread(&temp_word, 2, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s - STK", 
                fname);
            fclose(fp);
            return;
        }
        this->stk[i] = temp_word;
    }

    // RAM
    for (int i = 0; i <= 0xfff; i++) {
        if (fread(&temp_byte, 1, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s - RAM", 
                fname);
            fclose(fp);
            return;
        }
        this->ram[i] = temp_byte;
    }
 
    // Video
    for (int y = 0; y < 32; y++) {

        // Draw rows bit-by-bit
        for (int x=0; x<8; x++) {
            if (fread(&temp_byte, 1, 1, fp) != 1) {
                err->code = ERR_IO;
                snprintf(err->msg, MAX_ERR_MSG_LEN, 
                    "Could not read file %s - Video", fname);
                fclose(fp);
                return;
            }
            this->vid[x][y] = temp_byte;
        }
    }

    // Footer
    for (int i=0; i<4; i++) {
        this->foot[i] = fgetc(fp);
    } 
    if (strcmp(this->foot, "FIN") != 0) {
        err->code = ERR_DATA;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "File %s is invalid or malformed - Footer", fname);
        fclose(fp);
        return;
    }
}

void sv_sv_st(const SvSt *this, const char *fname, Err *err) {
    uint8_t     temp_byte;
    uint16_t    temp_word;

    FILE *fp = fopen(fname, "wb");
    if (fp == NULL) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not open file %s", fname);
        return;
    }
  
    // Header
    if (fputs(this->head, fp) == EOF) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Header", fname);
        fclose(fp);
        return;
    }
    if (fputc('\0', fp) == EOF) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Header", fname);
        fclose(fp);
        return;
    }

    // V Registers
    for (int i=0; i<16; i++) {
        temp_byte = this->v_regs[i];
        if (fwrite(&temp_byte, 1, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN,
                "Could not write to file %s - V Registers", fname);
            fclose(fp);
            return;
        }
    }

    // I Register
    temp_word = this->i_reg;
    if (fwrite(&temp_word, 2, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - I Register", fname);
        fclose(fp);
        return;
    }

    // Delay Timer
    temp_byte = this->del_timer;
    if (fwrite(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Delay Timer", fname);
        fclose(fp);
        return;
    }
    
    // Sound Timer
    temp_byte = this->snd_timer;
    if (fwrite(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Sound Timer", fname);
        fclose(fp);
        return;
    }
    
    // PC
    temp_word = this->pc;
    if (fwrite(&temp_word, 2, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - PC", fname);
        fclose(fp);
        return;
    }
    
    // SP
    temp_byte = this->sp;
    if (fwrite(&temp_byte, 1, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - SP", fname);
        fclose(fp);
        return;
    }

    // Stack
    for (int i=0; i<16; i++) {
        temp_word = this->stk[i];
        if (fwrite(&temp_word, 2, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN, 
                "Could not write to file %s - Stack", fname);
            fclose(fp);
            return;
        }
    }

    // RAM
    for (int i=0; i<=0xfff; i++) {
        temp_byte = this->ram[i];
        if (fwrite(&temp_byte, 1, 1, fp) != 1) {
            err->code = ERR_IO;
            snprintf(err->msg, MAX_ERR_MSG_LEN, 
                "Could not write to file %s - RAM", fname);
            fclose(fp);
            return;
        }
    }

    // Video
    for (int y=0; y<32; y++) {
        for (int x=0; x<8; x++) {
            temp_byte = this->vid[x][y];
            if (fwrite(&temp_byte, 1, 1, fp) != 1) {
                err->code = ERR_IO;
                snprintf(err->msg, MAX_ERR_MSG_LEN, 
                    "Could not write to file %s - Video", fname);
                fclose(fp);
                return;
            }
        }
    }

    // Footer
    if (fputs(this->foot, fp) == EOF) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Footer", fname);
        fclose(fp);
        return;
    }
    if (fputc('\0', fp) == EOF) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, 
            "Could not write to file %s - Footer", fname);
        fclose(fp);
        return;
    }

    fclose(fp);
}

void apply_sv_st(const SvSt *this, Cpu *cpu, Win *win, Ram *ram, Err *err) {
    uint8_t temp_byte;
 
    // Cpu
    for (int i=0; i<16; i++) {
        cpu->v_regs[i] = this->v_regs[i];
    }
    cpu->i_reg = this->i_reg;
    cpu->del_timer = this->del_timer;
    cpu->snd_timer = this->snd_timer;
    cpu->pc = this->pc;
    cpu->sp = this->sp;
    for (int i=0; i<16; i++) {
        cpu->stk[i] = this->stk[i];
    }

    // Ram
    for (int i=0; i<=0xfff; i++) {
        ram->data[i] = this->ram[i];
    }
    
    // Video
    for (int y=0; y<32; y++) {
        
        // Draw rows bit-by-bit
        for (int x=0; x<8; x++) {
            temp_byte = this->vid[x][y];
            for (int i=7; i>=0; i--) {
                if (temp_byte & (0x1 << i)) {
                    draw_px(win, (x*8)+i, y, err);
                    if (is_err(err)) return;
                    redraw_win(win);
                }    
            }
        }
    }
}


