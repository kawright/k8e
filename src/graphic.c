// Copyright (C) 2024  KA Wright

// grpahic.c - Graphics routines

#include <stdbool.h>
#include <stdint.h>

#include "err.h"
#include "graphic.h"

void init_win(Win *this, uint32_t bg, uint32_t fg, uint8_t px_sz) {
    this->sdl_win           = NULL;
    this->sdl_surf          = NULL;
    this->bg                = bg;
    this->fg                = fg;
    this->px_sz             = px_sz;
    for (uint8_t y = 0; y < 32; y++) {
        for (uint8_t x = 0; x < 64; x++) {
            this->px_map[x][y] = false;
        } 
    }
}

void open_win(Win *this, Err *err) {
    this->sdl_win = SDL_CreateWindow("k8e", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 64*this->px_sz, 32*this->px_sz,
        SDL_WINDOW_SHOWN);
    if (this->sdl_win == NULL) {
        err->code = ERR_INIT;
        strcpy(err->msg, "Could not open window");
        return;
    }
    this->sdl_surf = SDL_GetWindowSurface(this->sdl_win);
}

void clear_win(Win *this, Err *err) {
    for (uint8_t y = 0; y < 32; y++) {
        for (uint8_t x = 0; x < 64; x++) {
            this->px_map[x][y] = false;
        }
    }
    uint8_t     r           = (this->bg & 0xff0000) >> 16;
    uint8_t     g           = (this->bg & 0x00ff00) >> 8;
    uint8_t     b           = (this->bg & 0x0000ff);
    int         results;
    results = SDL_FillRect(this->sdl_surf, NULL, 
        SDL_MapRGB(this->sdl_surf->format, r, g, b));
    if (results != 0) {
        err->code = ERR_SUBSYS;
        strcpy(err->msg, "Could not perform clear operation");
    }
}

void close_win(Win *this) {
    SDL_DestroyWindow(this->sdl_win);
}

bool draw_px(Win *win, uint8_t x, uint8_t y, Err *err) {
    uint16_t    real_x      = win->px_sz * x;
    uint16_t    real_y      = win->px_sz * y;
    SDL_Rect    px_rect     = {real_x, real_y, win->px_sz, win->px_sz};
    int         results;
    bool        ret         = false;
    uint8_t     r;
    uint8_t     g;
    uint8_t     b;
    

    if (win->px_map[x][y]) {
        ret     = true;
        r       = (win->bg & 0xff0000) >> 16;
        g       = (win->bg & 0x00ff00) >> 8;
        b       = (win->bg & 0x0000ff);
        results = SDL_FillRect(win->sdl_surf, &px_rect, 
            SDL_MapRGB(win->sdl_surf->format, r, g, b));
        win->px_map[x][y] = false;
    } else {
        r       = (win->fg & 0xff0000) >> 16;
        g       = (win->fg & 0x00ff00) >> 8;
        b       = (win->fg & 0x0000ff);
        results = SDL_FillRect(win->sdl_surf, &px_rect, 
            SDL_MapRGB(win->sdl_surf->format, r, g, b));
        win->px_map[x][y] = true;
        
    }
    if (results != 0) {
        err->code = ERR_SUBSYS;
        strcpy(err->msg, "Could not perform draw operation");
        return ret;
    }
    return ret;
}

void redraw_win(Win *this) {
    SDL_UpdateWindowSurface(this->sdl_win); 
}
