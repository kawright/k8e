// Copyright (C) 2024  KA Wright

// ram.c - RAM operations

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "err.h"
#include "ram.h"

void init_ram(Ram *this) {
    for (int i = 0; i <= ADDR_PROG_END; i++) {
        this->data[i] = 0;
    }
    this->prog_len = 0;
}

void dump_ram(Ram *this, Err *err) {
    char fname[32];
    sprintf(fname, "k8e_ram_dump_%lu.txt", (unsigned long) time(NULL));
    FILE *fp = fopen(fname, "w");
    if (fp == NULL) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not open file %s", fname);
        return;
    }
    for (int i = 0; i <= ADDR_PROG_END; i += 8) {
        fprintf(fp, "@%03x    ", i);
        for (int j = i; j < i + 8; j++) {
            if (j >= ADDR_PROG_END) {
                break;
            }
            fprintf(fp, "%02x ", this->data[j]);
        }
        fprintf(fp, "%s\n", "");
    }
    fclose(fp);
}

void ld_ram_char(Ram *this) {

    // 0
    this->data[0x000] = 0xf0;
    this->data[0x001] = 0x90;
    this->data[0x002] = 0x90;
    this->data[0x003] = 0x90;
    this->data[0x004] = 0xf0;

    // 1
    this->data[0x005] = 0x20;
    this->data[0x006] = 0x60;
    this->data[0x007] = 0x20;
    this->data[0x008] = 0x20;
    this->data[0x009] = 0x70;

    // 2
    this->data[0x00a] = 0xf0;
    this->data[0x00b] = 0x10;
    this->data[0x00c] = 0xf0;
    this->data[0x00d] = 0x80;
    this->data[0x00e] = 0xf0;

    // 3
    this->data[0x00f] = 0xf0;
    this->data[0x010] = 0x10;
    this->data[0x011] = 0xf0;
    this->data[0x012] = 0x10;
    this->data[0x013] = 0xf0;

    // 4
    this->data[0x014] = 0x90;
    this->data[0x015] = 0x90;
    this->data[0x016] = 0xf0;
    this->data[0x017] = 0x10;
    this->data[0x018] = 0x10;

    // 5
    this->data[0x019] = 0xf0;
    this->data[0x01a] = 0x80;
    this->data[0x01b] = 0xf0;
    this->data[0x01c] = 0x10;
    this->data[0x01d] = 0xf0;
    
    // 6
    this->data[0x01e] = 0xf0;
    this->data[0x01f] = 0x80;
    this->data[0x020] = 0xf0;
    this->data[0x021] = 0x90;
    this->data[0x022] = 0xf0;
    
    // 7
    this->data[0x023] = 0xf0;
    this->data[0x024] = 0x10;
    this->data[0x025] = 0x20;
    this->data[0x026] = 0x40;
    this->data[0x027] = 0x40;
    
    // 8
    this->data[0x028] = 0xf0;
    this->data[0x029] = 0x90;
    this->data[0x02a] = 0xf0;
    this->data[0x02b] = 0x90;
    this->data[0x02c] = 0xf0;
    
    // 9
    this->data[0x02d] = 0xf0;
    this->data[0x02e] = 0x90;
    this->data[0x02f] = 0xf0;
    this->data[0x030] = 0x10;
    this->data[0x031] = 0xf0;
    
    // A
    this->data[0x032] = 0xf0;
    this->data[0x033] = 0x90;
    this->data[0x034] = 0xf0;
    this->data[0x035] = 0x90;
    this->data[0x036] = 0x90;
    
    // B
    this->data[0x037] = 0xe0;
    this->data[0x038] = 0x90;
    this->data[0x039] = 0xe0;
    this->data[0x03a] = 0x90;
    this->data[0x03b] = 0xe0;
    
    // C
    this->data[0x03c] = 0xf0;
    this->data[0x03d] = 0x80;
    this->data[0x03e] = 0x80;
    this->data[0x03f] = 0x80;
    this->data[0x040] = 0xf0;
    
    // D
    this->data[0x041] = 0xe0;
    this->data[0x042] = 0x90;
    this->data[0x043] = 0x90;
    this->data[0x044] = 0x90;
    this->data[0x045] = 0xe0;
    
    // E
    this->data[0x046] = 0xf0;
    this->data[0x047] = 0x80;
    this->data[0x048] = 0xf0;
    this->data[0x049] = 0x80;
    this->data[0x04a] = 0xf0;
    
    // F
    this->data[0x04b] = 0xf0;
    this->data[0x04c] = 0x80;
    this->data[0x04d] = 0xf0;
    this->data[0x04e] = 0x80;
    this->data[0x04f] = 0x80;
}

void ld_ram(Ram *this, char *fname, Err *err) {
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not open file %s", fname);
        return;
    }
    fseek(fp, 0, SEEK_END);
    this->prog_len = ftell(fp);
    if (this->prog_len > (ADDR_PROG_END - ADDR_PROG_START + 1)) {
        err->code = ERR_DATA;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "File %s too large", fname);
        return;
    } 
    fseek(fp, 0, SEEK_SET);
    if (fread(this->data + ADDR_PROG_START, this->prog_len, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s", fname);
        return;
    }
    fclose(fp);
}

void reset_ram(Ram *this) {
    for (uint16_t addr = ADDR_PROG_START; addr <= ADDR_PROG_END; addr++) {
        this->data[addr] = 0x0;
    }
}


