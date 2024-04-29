// Copyright (C) 2024  KA Wright

// key.h - Keyboard state

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "key.h"

void init_key_st(KeySt *this) {
    this->st_len    = 0;
    this->st        = NULL;
}

uint8_t poll_key() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                
                case SDLK_7:
                return 0x1;
                
                case SDLK_8:
                return 0x2;
    
                case SDLK_9:
                return 0x3;

                case SDLK_0:
                return 0xc;

                case SDLK_u:
                return 0x4;

                case SDLK_i:
                return 0x5;

                case SDLK_o:
                return 0x6;
            
                case SDLK_p:
                return 0xd;

                case SDLK_j:
                return 0x7;

                case SDLK_k:
                return 0x8;

                case SDLK_l:
                return 0x9;

                case SDLK_SEMICOLON:
                return 0xe;

                case SDLK_m:
                return 0xa;

                case SDLK_COMMA:
                return 0x0;

                case SDLK_PERIOD:
                return 0xb;

                case SDLK_SLASH:
                return 0xf;

            }
        }
    }
    return 0x0;
}

bool read_key(KeySt *this, uint8_t key) {
    if (this->st == NULL) {
        this->st = SDL_GetKeyboardState(&this->st_len);
    }
    switch (key) {

        // KEYBOARD KEYS

        case 0x1:
        return this->st[SDL_SCANCODE_7];

        case 0x2:
        return this->st[SDL_SCANCODE_8];

        case 0x3:
        return this->st[SDL_SCANCODE_9];

        case 0xc:
        return this->st[SDL_SCANCODE_0];
    
        case 0x4:
        return this->st[SDL_SCANCODE_U];

        case 0x5:
        return this->st[SDL_SCANCODE_I];

        case 0x6:
        return this->st[SDL_SCANCODE_O];

        case 0xd:
        return this->st[SDL_SCANCODE_P];
        
        case 0x7:
        return this->st[SDL_SCANCODE_J];

        case 0x8:
        return this->st[SDL_SCANCODE_K];

        case 0x9:
        return this->st[SDL_SCANCODE_L];

        case 0xe:
        return this->st[SDL_SCANCODE_SEMICOLON];

        case 0xa:
        return this->st[SDL_SCANCODE_M];

        case 0x0:
        return this->st[SDL_SCANCODE_COMMA];

        case 0xb:
        return this->st[SDL_SCANCODE_PERIOD];

        case 0xf:
        return this->st[SDL_SCANCODE_SLASH];

        // COMMAND KEYS

        case 'q':                                   // Quit
        return this->st[SDL_SCANCODE_Q];

        case 'x':                                   // Pause
        return this->st[SDL_SCANCODE_X];

        case 'r':                                   // Resume
        return this->st[SDL_SCANCODE_R];

        case 's':                                   // Step
        return this->st[SDL_SCANCODE_S];

        case 'd':                                   // Dump RAM
        return this->st[SDL_SCANCODE_D];

        case 't':                                   // Dump Savestate
        return this->st[SDL_SCANCODE_T];

        default:
        break;
    }
    return false; 
}

void update_key_st() {
    SDL_PumpEvents();
}

