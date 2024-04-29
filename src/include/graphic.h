// Copyright (C) 2024  KA Wright

// graphic.h - Graphics routines

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "err.h"

// Handles a single graphical window.
typedef struct __WIN__ {
    SDL_Window      *sdl_win;
    SDL_Surface     *sdl_surf;
    uint32_t        bg;
    uint32_t        fg;
    uint8_t         px_sz;
    bool            px_map[64][32];
} Win;

// Initialize a Win.
void init_win(Win *this, uint32_t bg, uint32_t fg, uint8_t px_sz);

// Open a Win and make it visible.
void open_win(Win *this, Err *err);

// Clear a Win to only its bg color.
void clear_win(Win *this, Err *err);

// Close a Win.
void close_win(Win *this);

// Draw a pixel.
bool draw_px(Win *win, uint8_t x, uint8_t y, Err *err);

// Draw changes to a Win.
void redraw_win(Win *this);

#endif
