// Copyright (C) 2024  KA Wright

// clean.h - Resource cleanup

#include <stdio.h>

#include <SDL2/SDL.h>

#include "clean.h"
#include "graphic.h"

void clean_res(Win *win) {
    close_win(win);
    SDL_Quit();
    printf("\e[?25h");      // Show the cursor
    printf("\33[2K\r");     // Clear the current line
}
